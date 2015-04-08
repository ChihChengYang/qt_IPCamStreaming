#include "GlobalApp.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>


GlobalApp theApp;

GlobalApp::GlobalApp(void)
{
	memset(m_LPRChSet,0,MAX_LIVEVIEWER_NUM * sizeof(CamSetInfo));

	for(int i = 0;i < MAX_LIVEVIEWER_NUM;i++)
	{
		sprintf(m_LPRChSet[i].szUrl,"null");
		sprintf(m_LPRChSet[i].szUsername,"null");
		sprintf(m_LPRChSet[i].szPwd,"null");
		m_LPRChSet[i].nPort = 554;
  
	}
 
 
}

GlobalApp::~GlobalApp(void)
{
}
 
//camera
void GlobalApp::SetRecvVideoStreamCB(RecvStreamFunc recvstream,void* context)
{
    CamSet::SetRecvVideoStreamCB(recvstream,context);
}

int GlobalApp::SetIpCam(CamSetInfo* camset,int nChidx)
{

    int nRet;

    nRet = m_CamSet.SetCamera(
            nChidx, \
            camset->nEnable, \
            camset->szUsername, \
            camset->szPwd, \
            camset->szUrl, \
            camset->nPort, \
            camset->nCompayn, \
            camset->nModel);

    if(nRet != -1){
         return 0;
    }else return -1;
}
