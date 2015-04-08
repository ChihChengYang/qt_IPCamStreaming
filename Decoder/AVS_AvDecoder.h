
#ifndef	AVS_CODEC
#define	AVS_CODEC
#ifdef	WIN32

#include <windows.h>
#endif
//-----------------------------------------------------------------------------
// Types and defines:
//-----------------------------------------------------------------------------
#include "../RTSPCLIENT/VideoTypes.h"
extern "C"
{
#include "libavutil/mathematics.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libavutil/samplefmt.h"
}

typedef unsigned int	AVDHANDLE;
//codec parameter
typedef struct tagAVDParam{
	int					nChannelID;
	int					nFrameType;			//參考VideoTypes
    int					bGotFrame;			//是否解完一張frame
    int					nWidth;				//寬度
    int					nHeight;			//高度
	int					nPitch;				//寬的Pitch
	unsigned char		*pDecodedFrame[3];	//YUV Buffer, 0: Y 1:U 2:V
	unsigned char       *pRgbFrame;             //存放RGB的Buffer
}AVDParam;

//frame type
typedef enum _AVD_FRAMETYPE
{
	//VIDEO
	AVD_FRAMETYPE_I				=	 0	,		//I Frame / Slice
	AVD_FRAMETYPE_P						,		//P Frame / Slice

	AVD_FRAMETYPE_NODEF					,		//no definition, 如果是MJPEG則為此種frame type

}AVD_FRAMETYPE;

//Decode status
typedef enum _AVDSTATUS
{
	AVD_STATUS_OK				=	 0, 
	AVD_STATUS_FAIL_OPEN		=	-1,
	AVD_STATUS_FAIL_DE			=	-2,
	AVD_STATUS_FAIL_CB			=	-3,
}AVDSTATUS;

enum FileFrame
{
	FileFrame_NoData = -1,
	FileFrame_VideoType = 0,
	FileFrame_OtherType 	
};

//Decoder callback funtion
typedef void (*DecodedFrameFunc)(void *pContext, int nChannel, unsigned char *pRGBData[3], unsigned char *pYUVData, unsigned int nWidth, unsigned int nHeight, unsigned int nPitch, unsigned int	nFrameType, time_t tFrameTime, unsigned int nFrametimeMSec);
int			Init_AVDecoder();
AVDSTATUS   AVS_DecodeRegistCB(AVDHANDLE hAvd, DecodedFrameFunc	pDecodedFunc);
AVDHANDLE   AVS_OpenAVDecoder(int	nChannelID, int	nFrameType);

AVDSTATUS   AVS_DecodeVideo(AVDHANDLE hAvd, unsigned char *pBStream, int nFrameSize, void *pContext,
                            time_t tFrameTime, unsigned int nFrametimeMSec, PixelFormat outputFormat);

void        AVS_CloseAVDecoder(AVDHANDLE *hAvd);

int			AVS_GetWidth(AVDHANDLE hAvd);
int			AVS_GetHeight(AVDHANDLE hAvd);
int			AVS_GetFrameType(AVDHANDLE hAvd);

int			AVS_GetDecodeVideoTime(AVDHANDLE hAvd);
#endif
