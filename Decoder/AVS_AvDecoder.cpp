
#include "AVS_AvDecoder.h"
#include <windows.h>
#include <Emmintrin.h>

//-----------------------------------------------------------------------------
// Types and defines:
//-----------------------------------------------------------------------------
#define INBUF_SIZE					(128*1024)
#define	OPENCLOSECODEC_MUTEX		"CODECMUTEX"

typedef int (*sws_scaleFunction)(struct SwsContext *c, const uint8_t *const srcSlice[],
              const int srcStride[], int srcSliceY, int srcSliceH,
              uint8_t *const dst[], const int dstStride[]);

typedef struct SwsContext* (*sws_getContextFunction)(int srcW, int srcH, enum PixelFormat srcFormat,
                                  int dstW, int dstH, enum PixelFormat dstFormat,
                                  int flags, SwsFilter *srcFilter,
                                  SwsFilter *dstFilter, const double *param);

typedef	void (*sws_freeContextFunction)(struct SwsContext *swsContext);


typedef struct tagAVSCodecParam
{
	AVDParam			AvdParam;
	struct SwsContext	*hImgConvertCtx;
	AVCodec				*codec;			//codec handle
	AVCodecContext		*context;		//codec context

	AVFrame				*picture;		//construct frame buffer
	AVPacket			avpkt;
	CodecID				codecID;
	int					nPreWidth;
	int					nPreHeight;

	char				*RgbBuffer;		//rgb buffer
	char				*YuvBuffer;		//rgb buffer
	unsigned char		*pVideo[3];		//真實輸出的buffer pointer

#ifdef CUDA
	CUDHANDLE			hCudaDecoder;
#endif //CUDA
	DecodedFrameFunc	pDecodedCB;
    int					nErrCount;
}AVSCodecParamm;

typedef AVSCodecParamm*  LPAVSCodecParamm;

//------------------------------------------------------------------------
//global variable
//------------------------------------------------------------------------
//HANDLE									g_hOpenCloseCodecMutex;
unsigned long		g_lTime = 0;
unsigned int		g_TotalFrame = 0;
CRITICAL_SECTION	g_hCodecMutex ;

sws_scaleFunction			SWS_scale;
sws_getContextFunction		SWS_getContext;
sws_freeContextFunction		SWS_freeContext;
//------------------------------------------------------------------------
//function
//------------------------------------------------------------------------
//extern void avs_printf( char* FormatString, ... );
extern void DEBUG_PRINTF( char* FormatString, ... );

int	Init_AVDecoder(){
	static int	b_init = 0;

	if (b_init == 0)
	{
		InitializeCriticalSection( &g_hCodecMutex );
		b_init = 1;
	}

	HINSTANCE hinstLib = LoadLibrary(TEXT("avs_swscale-2.dll")); 

	SWS_scale = (sws_scaleFunction)GetProcAddress(hinstLib, "sws_scale");  
    if (SWS_scale == NULL) {  
        printf("ERROR: unable to find DLL function\n");  
        FreeLibrary(hinstLib);  
        system("pause");  
        return 1;  
    } 

	SWS_getContext = (sws_getContextFunction)GetProcAddress(hinstLib, "sws_getContext");  
    if (SWS_scale == NULL) {  
        printf("ERROR: unable to find DLL function\n");  
        FreeLibrary(hinstLib);  
        system("pause");  
        return 1;  
    } 

	SWS_freeContext = (sws_freeContextFunction)GetProcAddress(hinstLib, "sws_freeContext");  
    if (SWS_scale == NULL) {  
        printf("ERROR: unable to find DLL function\n");  
        FreeLibrary(hinstLib);  
        system("pause");  
        return 1;  
    } 
	return 1;
}

void	AVCodec_ReInit(LPAVSCodecParamm h)
{

	if (!h)
	{
		return;
	}

	EnterCriticalSection( &g_hCodecMutex );

	h->nErrCount = 0;
	h->nPreWidth = 0;
	h->nPreHeight = 0;
	//close
	if (h->context)
	{
		avcodec_close(h->context);
		av_free(h->context);
		h->context = 0;
    }
	
	//reopen
	h->codec = avcodec_find_decoder(h->codecID);
	if (!h->codec)
	{
		goto ERR;
	}
	
	h->context = avcodec_alloc_context();
	if (!h->context)
	{
		goto ERR;
	}

	h->context->thread_count = 4;
	//for audio
	h->context->channels = 1;
	h->context->bit_rate = 8000;
	h->context->sample_rate = 8000;
	h->context->block_align = 8;
    h->context->bits_per_coded_sample = 8000;

	h->picture->key_frame= 1;

	//open codec handle
	if (avcodec_open(h->context, h->codec) < 0)
	{
		goto ERR;	
	}

	LeaveCriticalSection( &g_hCodecMutex );
	return;
ERR:

	if (h->context)
	{
		avcodec_close(h->context);
		av_free(h->context);
		h->context = 0;
	}
	if (h->picture)
	{
		av_free(h->picture);
		h->picture = 0;
	}
	LeaveCriticalSection( &g_hCodecMutex );

}

AVDHANDLE AVS_OpenAVDecoder(int	nChannelID, int	nFrameType)
{
	LPAVSCodecParamm			h = 0;

	if (!Init_AVDecoder())
	{
		return (AVDHANDLE)AVD_STATUS_FAIL_OPEN;
	}
	h = (LPAVSCodecParamm) av_malloc(sizeof(AVSCodecParamm));
	if(!h) 
	{
		return (AVDHANDLE)AVD_STATUS_FAIL_OPEN;
	} 
	memset(h , 0 , sizeof(AVSCodecParamm));

	EnterCriticalSection( &g_hCodecMutex );

	avcodec_register_all();

	h->AvdParam.nFrameType = nFrameType;
	switch(nFrameType)
	{
		case VS_VIDEOTYPE_H264:
			h->codecID = CODEC_ID_H264;
			break;
		case VS_AUDIOTYPE_PCMU:
			h->codecID = CODEC_ID_PCM_MULAW;
			break;
		case VS_AUDIOTYPE_PCMA:
			h->codecID = CODEC_ID_PCM_ALAW;
			break;
		case VS_VIDEOTYPE_MPEG4:
			h->codecID = CODEC_ID_MPEG4;
			break;
		case VS_VIDEOTYPE_MJPEG:
			h->codecID = CODEC_ID_MJPEG;
			break;
		default:
			goto ERR;
	}

	h->codec = avcodec_find_decoder(h->codecID);
    if (!h->codec){
        goto ERR;
	}
	h->picture = avcodec_alloc_frame();
    if (!h->picture){
        goto ERR;
	}
	h->context = avcodec_alloc_context();
    if (!h->context){
        goto ERR;
	}

	h->context->thread_count = 4;

	h->pDecodedCB = NULL;
	h->AvdParam.nChannelID = nChannelID;
    h->context->flags|=CODEC_FLAG_TRUNCATED;
	h->context->flags2 |=CODEC_FLAG2_CHUNKS;

	//for audio
	h->context->channels = 1;
	h->context->bit_rate = 8000;
	h->context->sample_rate = 8000;
	h->context->block_align = 8;
	h->context->bits_per_coded_sample = 8000;
	//init picture
	h->picture->key_frame= 1;	

	if (avcodec_open(h->context, h->codec) < 0 )
	{
		goto ERR;
	}

	h->hImgConvertCtx = 0;
	LeaveCriticalSection( &g_hCodecMutex );
	return (AVDHANDLE)h;
ERR:

	AVS_CloseAVDecoder((AVDHANDLE*)&h);
	LeaveCriticalSection( &g_hCodecMutex );
	return (AVDHANDLE)AVD_STATUS_FAIL_OPEN;
}

AVDSTATUS	AVS_DecodeRegistCB(AVDHANDLE hAvd, DecodedFrameFunc	pDecodedFunc)
{
	LPAVSCodecParamm h;
	h = (LPAVSCodecParamm)hAvd;
	if (h == (LPAVSCodecParamm)AVD_STATUS_FAIL_OPEN)
	{
		return AVD_STATUS_FAIL_CB;
	}
	h->pDecodedCB = pDecodedFunc;

	return AVD_STATUS_OK;
}

//PIX_FMT_GRAY8
AVDSTATUS AVS_DecodeVideo(AVDHANDLE hAvd, unsigned char *pBStream, int nFrameSize, void *pContext,
                          time_t tFrameTime, unsigned int nFrametimeMSec, PixelFormat outputFormat)
{
	LPAVSCodecParamm h;

	int len;
	h = (LPAVSCodecParamm)hAvd;



	if (h==(LPAVSCodecParamm)AVD_STATUS_FAIL_OPEN || pBStream==NULL || h->pDecodedCB==NULL || nFrameSize<0 )	
		return (AVDSTATUS)AVD_STATUS_FAIL_DE;

	h->AvdParam.nWidth=0;	h->AvdParam.nHeight = 0;


	if (nFrameSize <= 0)
	{
		return AVD_STATUS_FAIL_DE;	
	}
	h->avpkt.size = nFrameSize;
	h->avpkt.data = pBStream;

	while (h->avpkt.size > 0) 
	{
		len = 0;

		if (!h)
		{
			return AVD_STATUS_FAIL_DE;
		}
		if (h->context ==0 || h->picture == 0)
		{
			return AVD_STATUS_FAIL_DE;
		}
		len = avcodec_decode_video2( h->context, h->picture, &h->AvdParam.bGotFrame, &h->avpkt);
		if (len < 0) 
		{
			return (AVDSTATUS)AVD_STATUS_FAIL_DE;
		}
		if (h->AvdParam.bGotFrame) 
		{

			g_TotalFrame++;
			
			h->AvdParam.pDecodedFrame[0] = (unsigned char*)h->picture->data[0];
			h->AvdParam.pDecodedFrame[1] = (unsigned char*)h->picture->data[1];
			h->AvdParam.pDecodedFrame[2] = (unsigned char*)h->picture->data[2];

			h->AvdParam.nHeight = h->context->height;
			h->AvdParam.nWidth = h->context->width;

			if (	( (h->context->width != 0 && h->context->height != 0 ) && 
					  (h->context->width != h->nPreWidth || h->context->height != h->nPreHeight) ) ||
						( h->hImgConvertCtx == 0 )
				)
			{	//寬高跟之前的frame不同, 重新init swscalecontext
				if (h->hImgConvertCtx)
				{
					SWS_freeContext(h->hImgConvertCtx);
				}
				if (h->RgbBuffer)
				{
					_mm_free(h->RgbBuffer);
				}
				if (h->YuvBuffer)
				{
					_mm_free(h->YuvBuffer);
				}
				h->hImgConvertCtx = SWS_getContext(	h->context->width, h->context->height, (PixelFormat)h->picture->format, 
                                                    h->context->width, h->context->height, PIX_FMT_RGB24, SWS_POINT,
													NULL, NULL, NULL);
				//改成16bytes alignment的malloc
                h->RgbBuffer = (char*)_mm_malloc(h->context->width * h->context->height * 3,16);
				h->YuvBuffer = (char*)_mm_malloc(h->context->width * h->context->height * 2,16);

				h->nPreWidth = h->context->width;
				h->nPreHeight = h->context->height;

			}

			if (h->hImgConvertCtx)
			{
				uint8_t* pSrcSlice[4];
				uint8_t* pDstSlice[4];
				int inlinesize[4];
				int outlinesize[4];


				//準備input的buffer pointer
				pSrcSlice[0] = (uint8_t*)h->AvdParam.pDecodedFrame[0] ;		//Y buffer
				pSrcSlice[1] = (uint8_t*)h->AvdParam.pDecodedFrame[1] ;		//U buffer
				pSrcSlice[2] = (uint8_t*)h->AvdParam.pDecodedFrame[2] ;		//V buffer
				pSrcSlice[3] = NULL;

				inlinesize[0] = h->picture->linesize[0] ;
				inlinesize[1] = h->picture->linesize[1] ;
				inlinesize[2] = h->picture->linesize[2] ;
				inlinesize[3] = 0;
				//準備output的buffer pointer
				pDstSlice[0] = (uint8_t*)h->RgbBuffer;
				pDstSlice[1] =
				pDstSlice[2] =
				pDstSlice[3] = NULL;
                outlinesize[0] =  h->context->width * 3;
				outlinesize[1] = outlinesize[2] = outlinesize[3] = 0;

				SWS_scale(	h->hImgConvertCtx, pSrcSlice, inlinesize , 0, h->AvdParam.nHeight, pDstSlice, outlinesize);
				h->pVideo[0] = (unsigned char*)h->RgbBuffer;
				h->pVideo[1] = h->pVideo[2] = NULL;

				//store yuv
				
				{
					int i = 0;
					char *pYUV = h->YuvBuffer;
					for (i = 0 ; i < h->context->height; i++)
					{
						memcpy(pYUV, h->AvdParam.pDecodedFrame[0] + i * h->picture->linesize[0],  h->context->width);
						pYUV += h->context->width;
					}
					for (i = 0 ; i < h->context->height/2; i++)
					{
						memcpy(pYUV, h->AvdParam.pDecodedFrame[1] + i * h->picture->linesize[1],  h->context->width/2);
						pYUV += h->context->width/2;
					}
					for (i = 0 ; i < h->context->height/2; i++)
					{
						memcpy(pYUV, h->AvdParam.pDecodedFrame[2] + i * h->picture->linesize[2],  h->context->width/2);
						pYUV += h->context->width/2;
					}
					
				}	

                h->pDecodedCB(pContext, h->AvdParam.nChannelID, h->pVideo,(unsigned char*) h->YuvBuffer,
                              h->context->width, h->context->height, h->context->width, 0, tFrameTime, nFrametimeMSec);
			}

		}
		else
		{

			return AVD_STATUS_FAIL_DE;
		}
		h->avpkt.size -= len;
		h->avpkt.data += len; 
	}


	if (h->AvdParam.bGotFrame) 
	{
		return AVD_STATUS_OK;
	}
	return AVD_STATUS_FAIL_DE;

}

//--------------------------------------------------------
// Function Name : AVS_GetHeight
// Purpose       : 
// Input         : 
//                 handle: handle
// Output        : None
// return value  : 1: decode success 0:decode fail
//--------------------------------------------------------
int			AVS_GetWidth(AVDHANDLE hAvd)
{
	LPAVSCodecParamm h;
	h = (LPAVSCodecParamm)hAvd;
	return h->AvdParam.nWidth;
}
//--------------------------------------------------------
// Function Name : AVS_GetHeight
// Purpose       : 
// Input         : 
//                 handle: handle
// Output        : None
// return value  : 1: decode success 0:decode fail
//--------------------------------------------------------
int			AVS_GetHeight(AVDHANDLE hAvd)
{
	LPAVSCodecParamm h;
	h = (LPAVSCodecParamm)hAvd;
	return h->AvdParam.nHeight;
}
//--------------------------------------------------------
// Function Name : AVS_GetFrameType
// Purpose       : 
// Input         : 
//                 handle: handle
// Output        : None
// return value  : 1: decode success 0:decode fail
//--------------------------------------------------------
int			AVS_GetFrameType(AVDHANDLE hAvd)
{
	LPAVSCodecParamm h;
	h = (LPAVSCodecParamm)hAvd;
	return h->AvdParam.nFrameType;
}


void	AVS_CloseAVDecoder(AVDHANDLE *hAvd)
{

	LPAVSCodecParamm h;

	h = (LPAVSCodecParamm)*hAvd;
	if (h == (LPAVSCodecParamm)AVD_STATUS_FAIL_OPEN || h == 0)
	{
		return ;
	}

	EnterCriticalSection( &g_hCodecMutex );

#ifdef	CUDA
	//CUDA
	if (h->hCudaDecoder != NULL)
	{
		CloseDecoder(h->hCudaDecoder);
	}
	else
	{
#endif	//CUDA
		
		if (h->hImgConvertCtx)
		{
			SWS_freeContext(h->hImgConvertCtx);
			h->hImgConvertCtx = 0;
		}
		if (h->context)
		{
			avcodec_close(h->context);
			av_free(h->context);
			h->context = 0;
		}
		if (h->picture)
		{
			av_free(h->picture);
			h->picture = 0;
		}
		if (h->RgbBuffer)
		{
			_mm_free(h->RgbBuffer);
			h->RgbBuffer = 0;
		}
		if (h->YuvBuffer)
		{
			_mm_free(h->YuvBuffer);
			h->YuvBuffer = 0;
		}
#ifdef	CUDA
	}
#endif	//CUDA
	av_free(h);
	*hAvd = 0;
	LeaveCriticalSection( &g_hCodecMutex );

}



