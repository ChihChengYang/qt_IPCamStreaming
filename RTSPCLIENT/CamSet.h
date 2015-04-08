#pragma once
extern "C"
{
#include "VideoSource.h"
}


//==============================================================
#define MAX_LIVEVIEWER_NUM				4

typedef struct _CamSetInfo
{
    int			nEnable;
    char		szUrl[512];
    int			nPort;
    char		szUsername[64];
    char		szPwd[64];
    int			nReliability;
    int			nCompayn;
    int			nModel;


}CamSetInfo;


#define		ENABLE_FLAG(status, flag)	status = (status | flag)
#define		DISABLE_FLAG(status, flag)	status = (status & (~flag))
#define		CHECK_FLAG(status, flag)	(status & flag)
//==============================================================
class CamSet
{
private:
	VSHANDLE		m_hConnect[MAX_LIVEVIEWER_NUM];
	static			RecvStreamFunc	m_RecvVideoStreamFun;
	static			void*			m_Context;
public:
	
	static	void	SetRecvVideoStreamCB(RecvStreamFunc recvstream,void* context);
	int		SetCamera(int nChidx,int nEnCh, char *szUser, char* szPwd, char* szUrl,int nPort,int nCompany,int nModel);
 
	
	CamSet(void);
	~CamSet(void);
};
