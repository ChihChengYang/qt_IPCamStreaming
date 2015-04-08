
#include "CVDGraphView.h"
#include <QGLWidget>

#include <iostream>
#include <QDebug>
using namespace std;


CVDGraphView::CVDGraphView(QWidget *parent): QGraphicsView(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    //-------------------------------------------------
    cvdGraphScene = new CVDGraphScene();
    setScene(cvdGraphScene);
    setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    //-------------------------------------------------
    QPalette pp = viewport()->palette();
    pp.setColor(QPalette::Base,Qt::transparent);
    viewport()->setPalette(pp);
    //-------------------------------------------------

    m_timer = NULL;
    paintGraphSceneFlag = false;

    this->setStyleSheet("border: 0px");
//-------------------------------------------------------------------------
    connect(cvdGraphScene->rectangleItem, &CVDGraphicsItemRectangle::updateRectangleSignal,
            this, &CVDGraphView::updateRectangleSignal);

    connect(this, &CVDGraphView::updateInOutSignal,
            cvdGraphScene->rectangleItem, &CVDGraphicsItemRectangle::updateInOutSlot);
//-------------------------------------------------------------------------

}

QRectF CVDGraphView::getRectangle()
{
    return cvdGraphScene->rectangleItem->rectangle;
}

CVDGraphView::~CVDGraphView()
{
    if(cvdGraphScene){
        delete cvdGraphScene;
        cvdGraphScene = NULL;
    }
    if(m_timer){
        delete m_timer;
        m_timer = NULL;
    }
}

void CVDGraphView::updateGLSize(QRect &glSize)
{
    if(cvdGraphScene){
        cvdGraphScene->glWidth = (glSize.width());
        cvdGraphScene->glHeight = (glSize.height());
        cvdGraphScene->setSceneRect(0,0, cvdGraphScene->glWidth ,cvdGraphScene->glHeight  );
    }
}

void CVDGraphView::updateGLSize(int w, int h)
{
    if(cvdGraphScene){
        cvdGraphScene->glWidth = (w  - 2   );
        cvdGraphScene->glHeight = (h  - 2   );
        cvdGraphScene->setSceneRect(0,0, cvdGraphScene->glWidth ,cvdGraphScene->glHeight  );
    }
}

void CVDGraphView::updateBK()
{

}

void CVDGraphView::updateBackground( unsigned char* decRawData, int decDataWidth, int decDataHeight)
{
    cvdGraphScene->decRawData = decRawData;
    cvdGraphScene->decDataWidth = decDataWidth;
    cvdGraphScene->decDataHeight = decDataHeight;
    cvdGraphScene->update();
}

void CVDGraphView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);

}


void CVDGraphView::wheelEvent(QWheelEvent *event)
{
      int numDegrees = event->delta() / 8;

}


void CVDGraphView::mouseDoubleClickEvent( QMouseEvent *event)
{

}
