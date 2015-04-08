#pragma once
#include "VideoSource.h"
class RtspClient
{
public:
	static	bool m_bVsInit;
	RtspClient(void);
	~RtspClient(void);
 

    void RegRecvVideoStreamCB(RecvStreamFunc recvstream,void* context);
 
};
