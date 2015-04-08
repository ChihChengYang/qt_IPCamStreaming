#include "DISWindow.h"

#include <QPointF>

#include <iostream>
#include <QDebug>
using namespace std;


FWidgetWindow  fWidgetWindow;

#ifdef READ_FROM_STREAMING


unsigned char *pTestYData;

void RecvVideoStreamData(void	*pContext, MEDIAINFO	*pMediaInfo)
{
    VSPARAM				*pVsParam		=	(VSPARAM*)pContext;
    VideoContext		*pvideocontext	=	(VideoContext*)pVsParam->pContext;
    DISWindow				*pLpr			=	(DISWindow*)pvideocontext->pLprContext;

    int nCh;
    if (ISVIDEOTYPE(pMediaInfo->nMediaType))
    {
        nCh = pVsParam->nChannel;

            if (pLpr->m_hDecoder[nCh] == 0)
            {
                pLpr->m_hDecoder[nCh] = AVS_OpenAVDecoder(nCh, pMediaInfo->nMediaType);
                AVS_DecodeRegistCB(pLpr->m_hDecoder[nCh], &DISWindow::LiveVideoDecodedData);

            }
            AVS_DecodeVideo(pLpr->m_hDecoder[nCh], pMediaInfo->pData, pMediaInfo->nDataSize, pLpr, 0, 0, PIX_FMT_RGB24);

          //  fwrite(pMediaInfo->pData, pMediaInfo->nDataSize , 1 , pVideoData);

    }

}



void DISWindow::LiveVideoDecodedData(void *pContext, \
                                  int nChannel, \
                                  unsigned char *pRGBData[3], \
                                  unsigned char *pYUVData, \
                                  unsigned int nWidth, \
                                  unsigned int nHeight, \
                                  unsigned int nPitch, \
                                  unsigned int	nFrameType, \
                                  time_t tFrameTime, \
                                  unsigned int nFrametimeMSec)
{


    for(int i = 0 ; i <  nWidth*nHeight; ++i ){
        int j = i*3;
        pTestYData[j] = pTestYData[j+1] = pTestYData[j+2] = pYUVData[i];
    }

    fWidgetWindow.mGraphView->updateBackground(pTestYData,nWidth,nHeight);


}
//==================================================================================================


#endif

DISWindow::DISWindow()
{
    fWidgetWindow.subWindow = NULL;
    fWidgetWindow.mGraphView = NULL;
#ifdef READ_FROM_STREAMING
    memset(m_hDecoder,0,MAX_LIVEVIEWER_NUM * sizeof(AVDHANDLE));
#endif

}
DISWindow::~DISWindow()
{
}


void DISWindow::startThread()
{
    emit workRequested();
}

void DISWindow::doWork()
{

//==============================================================
#ifdef READ_FROM_STREAMING

    videocontext.pRtspServerContext	= NULL;
    videocontext.pLprContext		= this;
    LprRtspClient.RegRecvVideoStreamCB(RecvVideoStreamData,&videocontext);

    theApp.m_LPRChSet[0].nEnable = 1;
    theApp.m_LPRChSet[0].nPort = 554;


    sprintf(theApp.m_LPRChSet[0].szUsername,"%s","admin");
    sprintf(theApp.m_LPRChSet[0].szPwd,"%s","123456");
    sprintf(theApp.m_LPRChSet[0].szUrl,"%s","10.62.8.102/mpeg4");

    theApp.SetIpCam(&theApp.m_LPRChSet[0],0);
 #endif
//================================================================

    emit finished();

}

#ifdef READ_FROM_STREAMING
int DISWindow::initWindow(int areaWidth, int areaHeight)
{
    QRect displayRect;
    displayRect.setRect(0,0,areaWidth,areaHeight);

    createOneWidgetWindow( fWidgetWindow, displayRect);

     pTestYData  = (unsigned char*) malloc(areaWidth * areaHeight * 3 );

    return 0;
}
#endif

void DISWindow::getParas(int &m_Smax,int &m_Smin, int &m_Scount, int &m_Sensitivity, bool &m_isRocord)
{

}

void DISWindow::loadParas(int &m_Smax,int &m_Smin, int &m_Scount, int &m_Sensitivity, bool &m_isRocord)
{

}

void DISWindow::saveParas(int m_Smax,int m_Smin, int m_Scount, int m_Sensitivity, bool m_isRocord )
{

}


void  DISWindow::updateParametersSlot(int m_Smax,int m_Smin, int m_Scount, int m_Sensitivity, bool m_isRocord)
{

}

void  DISWindow::updateRectangleSlot(QRectF rectangle)
{

}

int DISWindow::createOneWidgetWindow(  FWidgetWindow &fWidgetWindow, QRect &subWindowRect)
{

   if(fWidgetWindow.subWindow != NULL)
        return 1;
        fWidgetWindow.subWindow = new QMdiSubWindow();

#ifdef CVDGRAPHVIEW
        if(fWidgetWindow.mGraphView != NULL)
                  return 1;
        fWidgetWindow.mGraphView  = new CVDGraphView(this);
#endif

//================================
   //---------
   fWidgetWindow.subWindow->setWidget( fWidgetWindow.mGraphView );

   fWidgetWindow.subWindow->setAttribute(Qt::WA_DeleteOnClose);

      fWidgetWindow.subWindow->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowTitleHint);
      fWidgetWindow.subWindow->setStyleSheet("border:1px solid White;");

   fWidgetWindow.subWindow->setGeometry(0,0,subWindowRect.width(),subWindowRect.height() );


#ifdef CVDGRAPHVIEW
   //-------------------------------------------------------------------------
       connect(fWidgetWindow.mGraphView, &CVDGraphView::updateRectangleSignal,
               this, &DISWindow::updateRectangleSlot);

   //-------------------------------------------------------------------------

    if( fWidgetWindow.mGraphView != NULL){
         fWidgetWindow.mGraphView->updateGLSize(subWindowRect);
    }

#endif

   this->addSubWindow( fWidgetWindow.subWindow);

   return 0;
}
