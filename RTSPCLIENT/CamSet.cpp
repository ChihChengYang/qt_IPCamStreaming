#include "GlobalApp.h"
#include "CamSet.h"
#include <string.h>
#include <stdio.h>

RecvStreamFunc	CamSet::m_RecvVideoStreamFun = NULL;
void*			CamSet::m_Context = NULL;

CamSet::CamSet(void)
{
	memset(m_hConnect,0,MAX_LIVEVIEWER_NUM*sizeof(VSHANDLE));
	m_RecvVideoStreamFun = NULL;
}

CamSet::~CamSet(void)
{
}

int	CamSet::SetCamera(int nChidx,int nEnCh, char *szUser, char* szPwd, char* szUrl,int nPort,int nCompany,int nModel)
{
    if (m_hConnect[nChidx]){
		VS_Close(&m_hConnect[nChidx]);
		m_hConnect[nChidx] = 0;
	}

	if(nEnCh == 1)//enable webcam
	{
		VSPARAM		ConnectParam;
		
		//connect
		memset(&ConnectParam, 0, sizeof(VSPARAM));
			
		sprintf(ConnectParam.szUsername,"%s",szUser);
		sprintf(ConnectParam.szPassword,"%s",szPwd);
		sprintf(ConnectParam.szUrl,"%s",szUrl);
		ConnectParam.nPort		= nPort;
		ConnectParam.pContext	= NULL;
		ConnectParam.nChannel	= nChidx;	
		ConnectParam.pVideoCB	= m_RecvVideoStreamFun;
		ConnectParam.pContext	= m_Context;

		m_hConnect[nChidx] = VS_Open(&ConnectParam);
		if(m_hConnect[nChidx] == NULL) return -1;
		VS_Play(m_hConnect[nChidx]);
	}

	
	return 1;
}
 
void CamSet::SetRecvVideoStreamCB(RecvStreamFunc recvstream,void* context)
{
	m_RecvVideoStreamFun = recvstream;
	m_Context = context;
}
	
