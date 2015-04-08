/*
 * CVDParameters.h
 *
 *  	created on: 2011
 *      Author: Jeff Yang pc524
 */
#ifndef CVDPARAMETERS_H
#define CVDPARAMETERS_H


#include <QString>
#include <QRectF>

#include <QDateTime>

#define POINTSSETTINGPATHNAME "./points.ini"
#define PARASETTINGPATHNAME "./paras.ini"

const char * const MIME_MULTIIPCHANNEL       = "data/multiipchannel";
const char * const MIME_ONEIPCHANNEL_CHANGE       = "data/oneipchannelchange";
const char * const MIME_ONEIPCHANNEL_DELETE       = "data/oneipchanneldelete";
const char * const MIME_DEVTREECHANNELINFO       = "application/DevTree";


#define SNAPSHOTPATH "/home/Snapshot"

#define ICONPATH "/home/QQTTEST_home2/Thread-build-desktop-Desktop_Qt_4_8_0_for_GCC__Qt_SDK__Release/"


               #define CVDDECMODULE

               #define CVDDECPLAYBACK
/***************************************
   On/Off functions
****************************************/
//=== 0:Test shmem thread for decoder ; 1: Use rtsp with decoder ; Others: Nothing. ===
#ifdef CVDDECMODULE
    #define USING_RTSP 2
#else
    #define USING_RTSP 0
#endif
//===
 //--- for OSD Text
    #define OSD_TEXT
 //--- for OSD ZOOM
   /// #define OSD_ZOOM
 //--- for OSD_ZOOM MOUSE EVENT
  ///  #define MOUSE_EVENT
 //--- for OSD_ZOOM MOUSE CURSOR
    /// #define MOUSE_CURSOR
 //--- for IVA items
    /// #define CVDIVA_ITEMS
//--- for MultiThreads Render
   /// #define MULTITHREADS_RENDER
//--- for RTSP Client
   /// #define USING_RTSP_CLIENT
//--- for Drag Drop StackWidget
     #define DRAGDROP_STACKWIDGET

//--- for CVDSubWindowBar
      #define CVDSUBWINDOWBAR

//--- for EXCHANGE SUBWINDOW
      #define EXCHANGE_SUBWINDOW

//--- for ePTZ
    //  #define ePTZ

//--- for CVDHTTPGETTHREAD
      #define CVDHTTPGETTHREAD

//--- for Graph View
      #define CVDGRAPHVIEW
//--- for CVDGLWidgetWithBar
    ///  #define CVDGLWIDGETWITHBAR

//--- for snapshot flash
      #define SNAPSHOTFLASH

//--- for SHARE WIDGET
   ///   #define SHAREWIDGET

/***************************************
    Parameters
****************************************/
    #define MAX_DISPLAY 64
	#define MAX_TARGET_WIDTH 1920
    #define MAX_TARGET_HEIGHT 1088
    #define MAX_PICTURE_TYPE 5

    #define MAX_SUBWINDOWBAR_HEIGHT 22

    #define MAXDISPLAYXMAXMONITORS MAX_DISPLAY*MAX_MONITORS
/***************************************
    Parameters for CVDIVA
****************************************/

const int MIN_ITEM_SIZE = 16;

//------------------------------------------------------------
typedef	int (*RecvFunction)(void *Handle, unsigned char *pData, unsigned int Len);

typedef enum _FANCYTOOLBUTTON_TYPE{
    TOOLBUTTON_TYPE_NONE = -1,
    TOOLBUTTON_TYPE_EPTZ ,
    TOOLBUTTON_TYPE_EPTZFIXED ,
    TOOLBUTTON_TYPE_SNAPSHOT ,
    TOOLBUTTON_TYPE_SNAPSHOTVIEW,
    TOOLBUTTON_TYPE_MANUALEVENT,
    TOOLBUTTON_TYPE_SOUND ,
    TOOLBUTTON_TYPE_REC ,
    TOOLBUTTON_TYPE_PLAY ,
    TOOLBUTTON_TYPE_PAUSE ,
    TOOLBUTTON_TYPE_STOP ,
    TOOLBUTTON_TYPE_STREAMING_PREVIEW ,
    TOOLBUTTON_TYPE_STREAMING_RECORD ,
    TOOLBUTTON_TYPE_STREAMING_MOBILE
}FANCYTOOLBUTTON_TYPE;


#define MAX_POINTS_ITEM 10
#define MAX_ITEMS 8

typedef enum{
    USER_TYPE_BASE = 0, //nothing
    USER_TYPE_IVA = 1,
    USER_TYPE_PANO = 2
}GRAPHICS_ITEM_USER_TYPE;

/// ===========================================================================
#define UNDOREDO

typedef enum{
    TYPE_ITEMS_BASE = 0, //nothing
    TYPE_ITEMS_RECTANGLE = 1,
    TYPE_ITEMS_POLYGON = 2,
    TYPE_ITEMS_COUNTING = 4,
    TYPE_ITEMS_RECT = 8,
    TYPE_ITEMS_MOUSERECT = 16
}TYPE_ITEMS;
/// ===========================================================================


typedef struct{
    int x,y;
}CVDIVA_Point;

typedef struct{
    TYPE_ITEMS typeItem;
    CVDIVA_Point  points[MAX_POINTS_ITEM];
    int pointsNumber;
}CVDIVA_Items;

//=============================================================
#define MAX_NVRCHANNELSLISTNUMBERS 8

typedef struct _NVRChannelsList
{
    QString channelNo,channelType;
    QString receiverToken,recordingToken;

}NVRChannelsList;

typedef struct _ChannelsInfo
{
    int fDisplayRectChID;
    int channelID;
//-------------
    QRectF rectangleItem,rectangleItem2;
    qreal percentage;
//-------------
    QString nvr_URL;// QString URLString = QString("http://%1:%2@%3").arg("Admin").arg(g_LoginPwd).arg(g_LoginHost)
    QString nvr_deviceID,nvr_deviceUUID;
    int nvr_channelsListNumbers;
    int nvr_channelCurrentType;
    int nvr_channelCurrentIndex;
    NVRChannelsList nvr_channelsList[MAX_NVRCHANNELSLISTNUMBERS];
    int recordingToken; //for PB ,channelType is record
//-------------
    bool currentPlaystopFlag; // true: non play
//-------------
    bool currentNonPauseFlag; // true: play
//-------------
    unsigned long playbackTime;
    QDateTime timeControled;
//-------------
} ChannelsInfo;
//=============================================================

typedef enum _CGIEVENT_TYPE{
    CGIEVENT_NONE = -1 ,
    CGIEVENT_MANUALEVENT ,
    CGIEVENT_GETDEVICELIST ,
    CGIEVENT_MANUALRECORD ,
    CGIEVENT_SNAPSHOT ,
    CGIEVENT_GETSERVERSTATUS
}CGIEVENT_TYPE;

//=============================================================

//==================================================================

const QString iconSubWindowBar = ":/subWindowBar/images/";
const QString iconSubWindowBar_snapshotN = iconSubWindowBar + "Remote-Tool-snapshot-Normal.png";
const QString iconSubWindowBar_snapshotH = iconSubWindowBar + "Remote-Tool-snapshot-Hover.png";

const QString iconSubWindowBar_manualrecordingN = iconSubWindowBar + "Remote-Tool-manual-recording-Normal.png";
const QString iconSubWindowBar_manualrecordingH = iconSubWindowBar + "Remote-Tool-manual-recording-Hover.png";

const QString iconSubWindowBar_instantplaybackN = iconSubWindowBar + "Remote-Tool-instant-playback-Normal.png";
const QString iconSubWindowBar_instantplaybackH = iconSubWindowBar + "Remote-Tool-instant-playback-Hover.png";

const QString iconSubWindowBar_instantpauseN = iconSubWindowBar + "Remote-Tool-pause-Normal.png";
const QString iconSubWindowBar_instantpauseH = iconSubWindowBar + "Remote-Tool-pause-Hover.png";

const QString iconSubWindowBar_instantstopN = iconSubWindowBar + "Remote-Tool-manual-recording-Normal.png";
const QString iconSubWindowBar_instantstopH = iconSubWindowBar + "Remote-Tool-manual-recording-Hover.png";

const QString iconSubWindowBar_instantplayN = iconSubWindowBar + "Remote-Tool-instant-playback-Normal.png";
const QString iconSubWindowBar_instantplayH = iconSubWindowBar + "Remote-Tool-instant-playback-Hover.png";

const QString iconSubWindowBar_manualeventN = iconSubWindowBar + "Remote-Tool-manual-event-Normal.png";
const QString iconSubWindowBar_manualeventH = iconSubWindowBar + "Remote-Tool-manual-event-Hover.png";

//const QString iconSubWindowBar_closeN = iconSubWindowBar + "Window-Close-BTN-normal.png";
//const QString iconSubWindowBar_closeH = iconSubWindowBar + "Window-Close-BTN-hover.png";
const QString iconSubWindowBar_closeN = iconSubWindowBar + "darkclose.png";
const QString iconSubWindowBar_closeH = iconSubWindowBar + "darkclosebutton.png";
//===================================================================

#endif // CVDPARAMETERS_H
