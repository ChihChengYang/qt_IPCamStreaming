#ifndef CVDGRAPHVIEW_H
#define CVDGRAPHVIEW_H

#include "CVDGraphScene.h"
#include <QGraphicsView>
#include <QTimer>


class CVDGraphView : public QGraphicsView
{
           Q_OBJECT
public:
    CVDGraphView(QWidget *parent);
    virtual ~CVDGraphView();

    CVDGraphScene *cvdGraphScene;

    void setPaintGraphSceneFlag(bool flag);

    void setPaintGraphSceneFlag(bool flag, ChannelsInfo *channelInfo);

    void updateGLSize(int w, int h);

    void updateBackground( unsigned char* decRawData, int decDataWidth, int decDataHeight);

    QRectF getRectangle();

protected:
    void resizeEvent(QResizeEvent *event);

    void wheelEvent(QWheelEvent *event);



    void mouseDoubleClickEvent(QMouseEvent *event);
#if 0
    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent( QMouseEvent *event);
    void mouseReleaseEvent( QMouseEvent *event);
#endif
private:
    QTimer *m_timer;

    int cvdWindowChannel;
    bool paintGraphSceneFlag;

    QRectF rectangleItem,rectangleItem2;
    qreal percentage;
    bool initePTZflag;

public slots:
    void updateGLSize(QRect &);



private slots:
   void updateBK();

signals:
   //--------------------------------------
        void updateInOutSignal(int in, int out);
   //--------------------------------------
   //--------------------------------------
        void updateRectangleSignal(QRectF rectangle);
   //--------------------------------------
};

#endif // CVDGRAPHVIEW_H
