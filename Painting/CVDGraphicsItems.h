#ifndef CVDGRAPHICSITEMS_H
#define CVDGRAPHICSITEMS_H

#include "qmath.h"
#include "qmenu.h"
#include "qgraphicsview.h"
#include "qgraphicssceneevent.h"
#include "qtableview.h"

#include <QSettings>
#include <QGraphicsItem>

#include "CVDParameters.h"

class CVDGraphicsItemBase :   public QGraphicsItem
{
public:
    enum { Type = UserType + USER_TYPE_IVA };

    CVDGraphicsItemBase( int x, int y, int sceneWidth,int sceneHeight );

    virtual ~CVDGraphicsItemBase();
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    TYPE_ITEMS typeItems;

    void setRectangleRectSceneWidth(int w){ sceneWidth = w;}
    void setRectangleRectSceneHeight(int h){ sceneHeight = h;}

    void setRectangleRect(qreal x1, qreal y1, qreal x2, qreal y2 );
    QList<QPointF> getRectangleRectList(){return m_pointsRect;}

    QRectF rectangle;
    QList<QPointF> m_pointsRect;

    qreal rectangleWheelPercentage;
    QRectF rectangleWheel;

protected:

    virtual int type() const;
    QRectF resizeHandle(QRectF rect) const;
    bool isInResizeArea(QRectF rect ,const QPointF &pos);


    int sceneWidth,sceneHeight;
    bool m_isResizing;
    int m_pointSize;
    QRectF m_rectF;
    QPointF m_mousePressOffset;
    QPolygonF arrowHead;
    QLineF arrowLine,arrowRightLine,arrowLeftLine;
};


//============================================================================//
//                                 RectangleItem                              //
//============================================================================//
class CVDGraphicsItemRectangle : public QObject, public CVDGraphicsItemBase
{Q_OBJECT
public:
    CVDGraphicsItemRectangle( int x, int y, int sceneWidth,int sceneHeight);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;

    void setPointsrectToRectangle();

    void setRectangleRectList(qreal w, qreal h );

    void setRectangle(int x1, int y1, int x2, int y2 );

    QColor solidLineColor;

//--------------------------------------
    void clearPoints( );
    void savePoints(QRectF rectangle);
    void loadPoints(QRectF &rectangle);
//--------------------------------------

protected:
    virtual CVDGraphicsItemBase *createNew( int x, int y, int sceneWidth,int sceneHeight);

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    QPointF m_mousePress;
    QRectF rectangleTemp;

private:
    int itemRectangleSceneX1, itemRectangleSceneY1;
    int itemRectangleSceneX2, itemRectangleSceneY2;
//--------------------------------------
    int in, out;
//--------------------------------------
public slots:
    void setItemRectangleScenePostion(int x1,int y1,int x2,int y2){itemRectangleSceneX1 = x1; itemRectangleSceneY1=y1;
                                                               itemRectangleSceneX2 = x2; itemRectangleSceneY2 = y2;}
//--------------------------------------
    void updateInOutSlot(int in, int out);
//--------------------------------------

signals:
    void sendItemRectangleScenPostionSignal(int x1,int y1,int x2,int y2);

    void updateRectangleItemSignal();

//--------------------------------------
     void updateRectangleSignal(QRectF rectangle);
//--------------------------------------
};


#endif // CVDGRAPHICSITEMS_H
