#ifndef	__VIDEOSOURCEDLL_H_
#define	__VIDEOSOURCEDLL_H_

#include	<time.h>
#include	"VideoTypes.h"
//-----------------------------------------------------------------------------
// defines:
//-----------------------------------------------------------------------------
#ifdef __cplusplus
#define VIDEOSOURCE_CNAME extern "C"
#else
#define VIDEOSOURCE_CNAME
#endif


#if (defined(WIN32) || defined(_WIN32) || defined(_WIN64))

#ifdef VIDEOSOURCE_EXPORTS
#define VIDEOSOURCE_API VIDEOSOURCE_CNAME __declspec(dllexport)
#else
#define VIDEOSOURCE_API VIDEOSOURCE_CNAME __declspec(dllimport)
#endif

#else	///LINUX

#define VIDEOSOURCE_API	VIDEOSOURCE_CNAME __attribute__ ((visibility("default")))

#endif	///OS definition
//-----------------------------------------------------------------------------
// Types:
//-----------------------------------------------------------------------------

typedef unsigned long VSHANDLE;

typedef struct tagMediaInfo
{
	unsigned char	    *pData;
	unsigned int	    nDataSize;
	int				    nMediaType;	
	int				    bMotion;
	unsigned long long	nFrameTime;
	unsigned int	    nFrameTimeMsec;

	int				    nWidth;
	int				    nHeight;

	int				    nSamplerate;
	int				    nNumOfAudioChannel;
    int                 Reserved;
}MEDIAINFO;

typedef void (*RecvStreamFunc)(void	*pContext, MEDIAINFO *pMediaInfo);
typedef void (*RecvEventFunc)(void	*pContext, int	nEventType, unsigned char *pData, unsigned int nDataSize);


typedef struct tagVSPLAYBACK
{
	int				bPlaybackMode;		//0: normal mode ; 1:playbackmode
	//start time
	int				nYear;
	int				nMonth;
	int				nDay;
	int				nHour;
	int				nMin;
	int				nSec;
}VSPLAYBACK;

typedef struct tagVSPARAM{
	int				nChannel;
	int				nBrand;
	int				nModule;

	char			szUsername[128];
	char			szPassword[128];
	char			szUrl[128];
	int				nPort;
	int				nSubChannel;
	//internal camera
	int				nWidth;
	int				nHeight;
	int				nGOP;
	int				nFPS;
	int				nBitrate;
    //DVBT
    int             nFrequence;
    int             nBandwidth;
    int             nVID;
    int             nAID;
	//CB
	RecvStreamFunc	pVideoCB;
	RecvStreamFunc	pAudioCB;
	RecvEventFunc	pEventCB;

	//for Playback
	VSPLAYBACK		PlaybackMode;
	//Original Context
	void			*pContext;			//for called usage
}VSPARAM;

//-----------------------------------------------------------------------------
// API prototype:
//-----------------------------------------------------------------------------
//init
VIDEOSOURCE_API int				VS_Init(void);
VIDEOSOURCE_API void			VS_Destroy(void);
VIDEOSOURCE_API void			VS_GetVersion(int *a,int *b,int *c,int *d);
 
//Source Setting
VIDEOSOURCE_API VSHANDLE		VS_Open(VSPARAM			*VsParam);
VIDEOSOURCE_API int				VS_Play(VSHANDLE		hVS);
VIDEOSOURCE_API int				VS_Pause(VSHANDLE		hVS);
VIDEOSOURCE_API void			VS_Close(VSHANDLE		*hVS);
VIDEOSOURCE_API unsigned int	VS_GetStatus(VSHANDLE	hVS);

#endif