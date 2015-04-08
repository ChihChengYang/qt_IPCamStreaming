#include "GlobalApp.h"
#include "RtspClient.h"

bool RtspClient::m_bVsInit = false;
RtspClient::RtspClient(void)
{
	if(m_bVsInit == false)
	{
		VS_Init();
		m_bVsInit = true;
	}
}

RtspClient::~RtspClient(void)
{
}

void RtspClient::RegRecvVideoStreamCB(RecvStreamFunc recvstream,void* context)
{
    theApp.SetRecvVideoStreamCB(recvstream,context);
}
 
