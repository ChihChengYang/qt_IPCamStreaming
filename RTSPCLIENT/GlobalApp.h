#pragma once
#include <string>

#include "CamSet.h"

using namespace std;


class GlobalApp
{
private:
 
    CamSet		m_CamSet;

public:
 
	CamSetInfo	m_LPRChSet[MAX_LIVEVIEWER_NUM];

    //camera
    int		SetIpCam(CamSetInfo* camset,int nChidx);
    void	SetRecvVideoStreamCB(RecvStreamFunc recvstream,void* context);

    GlobalApp(void);
	~GlobalApp(void);
  
};

extern GlobalApp theApp;
