#ifndef DISWINDOW_H
#define DISWINDOW_H

#include <QMdiArea>


#include "CVDParameters.h"

#ifdef CVDGRAPHVIEW
    #include "CVDGraphView.h"
#endif

//=======================================================
#define READ_FROM_X 1
#if (READ_FROM_X == 0)
    #define READ_FROM_FILE
#elif (READ_FROM_X == 1)
    #define READ_FROM_STREAMING
#endif


#ifdef READ_FROM_STREAMING
    #include "GlobalApp.h"
    #include "RtspClient.h"

    #include "AVS_AvDecoder.h"

    typedef struct _VideoContext{
        void*	pRtspServerContext;
        void*	pLprContext;
    }VideoContext;
#endif


//=======================================================

typedef struct{
  QMdiSubWindow *subWindow;
  CVDGraphView *mGraphView;
} FWidgetWindow;


class DISWindow : public QMdiArea
{
     Q_OBJECT

public:
    DISWindow();
    virtual ~DISWindow();

#ifdef READ_FROM_STREAMING
    int initWindow(int areaWidth, int areaHeight);

    RtspClient LprRtspClient;
    VideoContext	videocontext;

    static void LiveVideoDecodedData(void *pContext, int nChannel, unsigned char *pRGBData[3],
                unsigned char *pYUVData, unsigned int nWidth, unsigned int nHeight,
                unsigned int nPitch, unsigned int	nFrameType, time_t tFrameTime, unsigned int nFrametimeMSec);

    AVDHANDLE   m_hDecoder[MAX_LIVEVIEWER_NUM];

    void loadParas(int &m_Smax,int &m_Smin, int &m_Scount, int &m_Sensitivity, bool &m_isRocord);

    void saveParas(int m_Smax,int m_Smin, int m_Scount, int m_Sensitivity, bool m_isRocord );

    void  getParas(int &m_Smax,int &m_Smin, int &m_Scount, int &m_Sensitivity, bool &m_isRocord);
#endif

    void startThread();

    int createOneWidgetWindow(  FWidgetWindow &fWidgetWindow, QRect &subWindowRect);


signals:

    void workRequested();

    void finished();


public slots:

     void doWork();

//--------------------------------------
     void updateRectangleSlot(QRectF rectangle);
//--------------------------------------
     void updateParametersSlot(int Smax,int Smin, int Scount, int Sensitivity, bool isRocord);
//--------------------------------------
};

#endif // DISWINDOW_H
