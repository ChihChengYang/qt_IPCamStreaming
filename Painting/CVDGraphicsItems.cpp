#include "CVDGraphicsItems.h"

#include <iostream>
#include <QDebug>
using namespace std;

CVDGraphicsItemBase::CVDGraphicsItemBase( int x, int y, int sceneWidth, int sceneHeight ) :
    m_isResizing(false), sceneWidth(sceneWidth) , sceneHeight(sceneHeight)
{
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setAcceptHoverEvents(true);
    setPos(x, y);
    m_pointSize = 5;
    m_rectF.setX(0);
    m_rectF.setY(0);
    m_rectF.setWidth(480);
    m_rectF.setHeight(360);

    typeItems = TYPE_ITEMS_BASE;

}

CVDGraphicsItemBase::~CVDGraphicsItemBase()
{
}


QRectF CVDGraphicsItemBase::boundingRect() const{
    return m_rectF;
}

QPainterPath CVDGraphicsItemBase::shape() const
{
    QPainterPath path;
    path.addPolygon(m_rectF);
    return path;
}

void CVDGraphicsItemBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);

}

int CVDGraphicsItemBase::type() const
{
    return Type;
}


QRectF CVDGraphicsItemBase::resizeHandle(QRectF rect) const
{
    QPointF br = rect.bottomRight();
    return QRectF(br - QPointF(9, 9), br);
}

bool CVDGraphicsItemBase::isInResizeArea(QRectF rect ,const QPointF &pos)
{
    return resizeHandle(rect).contains(pos);//(-pos.y() < pos.x() - m_size + 9);
}

void CVDGraphicsItemBase::setRectangleRect(qreal x1, qreal y1, qreal x2, qreal y2 )
{
    rectangle.setCoords( x1, y1, x2, y2 );
}



//============================================================================//
//                                 RectangleItem                              //
//============================================================================//
CVDGraphicsItemRectangle::CVDGraphicsItemRectangle( int x, int y, int sceneWidth, int sceneHeight )
     : CVDGraphicsItemBase( x, y,sceneWidth, sceneHeight)
{
    m_pointsRect << QPointF(20,20) <<  QPointF(150,20) << QPointF(150,100) << QPointF(20,100);
    rectangle.setCoords(m_pointsRect.at(0).x(),m_pointsRect.at(0).y(),
                    m_pointsRect.at(2).x(),m_pointsRect.at(2).y() );

    QRectF gle;
    loadPoints(gle);
    rectangle = gle;
//------------------------------------------------------------

//------------------------------------------------------------

    typeItems = TYPE_ITEMS_RECTANGLE;

//=========================
    rectangleWheelPercentage = 100.0;
    rectangleWheel = rectangle;
//=========================

    itemRectangleSceneX2 = sceneWidth;
    itemRectangleSceneY2 = sceneHeight;
    itemRectangleSceneX1 = 0;
    itemRectangleSceneY1 = 0;

    solidLineColor =  Qt::yellow;

    //--------------------------------------
          in =0; out=0;
    //--------------------------------------

}

QRectF CVDGraphicsItemRectangle::boundingRect() const
{
   QRectF rectF;
   rectF.setCoords(itemRectangleSceneX1,itemRectangleSceneY1,itemRectangleSceneX2,itemRectangleSceneY2);
   return rectF;
}

QPainterPath CVDGraphicsItemRectangle::shape() const
{
    QPainterPath path,pathNew;
    QPainterPathStroker pathStroker ;
    pathStroker.setWidth(5);
    int pointsRectSize;
    pointsRectSize = m_pointsRect.size();
    for (int i=0; i < pointsRectSize; ++i)
    {
        path.moveTo(m_pointsRect.at(i));
        path.lineTo(m_pointsRect.at((i+1)%pointsRectSize));
    }
    path.closeSubpath();
    pathNew = (pathStroker.createStroke(path) + path ).simplified();

    return pathNew;
}

//--------------------------------------
void CVDGraphicsItemRectangle::updateInOutSlot(int in, int out)
{
    this->in = in;
    this->out = out;
}
//--------------------------------------


void CVDGraphicsItemRectangle::setRectangle(int x1, int y1, int x2, int y2 )
{

    rectangle.setCoords( x1 ,y1 , x2, y2);

    m_pointsRect[0] = rectangle.topLeft();
    m_pointsRect[1] = rectangle.topRight();
    m_pointsRect[2] = rectangle.bottomRight();
    m_pointsRect[3] = rectangle.bottomLeft();

    prepareGeometryChange();
}

//=======================================================================
void CVDGraphicsItemRectangle::loadPoints(QRectF &rectangle)
{

    int arrayCount;
    QString rectangleString = QString("rectangle");
    QString rectangleArrayString = QString("rectangleArray");
    QString pointsString = QString("pointsString");
//======================================
    QSettings PointsSetting(POINTSSETTINGPATHNAME,QSettings::IniFormat);

    PointsSetting.beginGroup(rectangleString);

//======================================
    rectangle = PointsSetting.value(pointsString).toRectF();

//======================================
    PointsSetting.endGroup();
//======================================
}

void CVDGraphicsItemRectangle::savePoints(QRectF rectangle)
{
    QString rectangleString = QString("rectangle");
    QString rectangleArrayString = QString("rectangleArray");
    QString pointsString = QString("pointsString");


//======================================
    QSettings PointsSetting(POINTSSETTINGPATHNAME,QSettings::IniFormat);

    PointsSetting.beginGroup(rectangleString);

//======================================
    /// ---------
    PointsSetting.remove("");
    /// ---------

//======================================
           PointsSetting.setValue(pointsString,rectangle);
//======================================
    PointsSetting.endGroup();
//======================================

}

void CVDGraphicsItemRectangle::clearPoints( )
{
}

//=======================================================================
void CVDGraphicsItemRectangle::setRectangleRectList(qreal w, qreal h )
{

}

void CVDGraphicsItemRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QPainterPath path;
    QPainterPath path2;
    painter->setRenderHint(QPainter::Antialiasing);

    m_pointsRect[0] = rectangle.topLeft();
    m_pointsRect[1] = rectangle.topRight();
    m_pointsRect[2] = rectangle.bottomRight();
    m_pointsRect[3] = rectangle.bottomLeft();

    QPen myPen;
    if ( (option->state & QStyle::State_Selected ) || m_isResizing)
    {
        myPen.setStyle(Qt::DotLine);
        myPen.setColor(Qt::blue);
    }
    else
    {
        myPen.setStyle(Qt::SolidLine);
        myPen.setColor( solidLineColor );
    }
    myPen.setWidth(2);


    painter->setPen(myPen);
    path2.moveTo(rectangle.topRight());
    path2.lineTo(rectangle.bottomRight());
    path2.lineTo(rectangle.bottomLeft());
    path2.lineTo(rectangle.topLeft());
    painter->drawPath(path2);

    path2.closeSubpath();

    myPen.setStyle(Qt::SolidLine);
    myPen.setColor(QColor(255, 0, 0, 255));
    painter->setPen(myPen);
    painter->drawText(10,10,QString("In: %1   Out: %2").arg(this->in).arg(this->out));

    if ( (option->state & QStyle::State_Selected)  || m_isResizing)
    {
       painter->setPen(QColor(50, 100, 120, 200));
       painter->setBrush(QColor(200, 200, 210, 120));
       QPointF pos = rectangle.bottomRight();
       int doublempointSize =  2*m_pointSize;
       painter->drawRoundedRect(QRectF(pos.x() - doublempointSize,
                                 pos.y() -doublempointSize,
                                 doublempointSize , doublempointSize ),1,1);
    }

}

CVDGraphicsItemBase *CVDGraphicsItemRectangle::createNew( int x, int y, int sceneWidth, int sceneHeight)
{
    return new CVDGraphicsItemRectangle( x, y, sceneWidth, sceneHeight);
}

void CVDGraphicsItemRectangle::setPointsrectToRectangle(){
                  update();
}

void CVDGraphicsItemRectangle::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
   if (m_isResizing || (isInResizeArea(rectangle,event->pos()) && isSelected()))
       setCursor(Qt::SizeFDiagCursor);
   else
        setCursor(Qt::ClosedHandCursor);

   if(!isSelected())
       setCursor(Qt::PointingHandCursor);

    QGraphicsItem::hoverMoveEvent(event);
}

void CVDGraphicsItemRectangle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if (event->button() == Qt::LeftButton && isInResizeArea(rectangle,event->pos()))
    {
        m_mousePressOffset = rectangle.bottomRight() - event->pos();
        m_isResizing = true;
        this->setSelected(true);

    }
    else
    {
        m_mousePress = event->pos();
        rectangleTemp = rectangle;
         CVDGraphicsItemBase::mousePressEvent(event);


    }

}

void CVDGraphicsItemRectangle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    QRectF rectA = boundingRect();
    QRectF checkPointsRectangle;
    bool checkPointsRectangleFlag = true;
    if (m_isResizing)
    {
       checkPointsRectangle = QRectF(rectangle.topLeft(), event->pos() + m_mousePressOffset);
       checkPointsRectangleFlag = rectA.contains(checkPointsRectangle.topLeft())?
                                  rectA.contains(checkPointsRectangle.topRight())?
                                  rectA.contains(checkPointsRectangle.bottomLeft())?
                                  rectA.contains(checkPointsRectangle.bottomRight()):false:false:false;
       if(checkPointsRectangleFlag)
       {
          if (checkPointsRectangle.width() < MIN_ITEM_SIZE)
             checkPointsRectangle.setWidth(MIN_ITEM_SIZE);
          if (checkPointsRectangle.height() < MIN_ITEM_SIZE)
             checkPointsRectangle.setHeight(MIN_ITEM_SIZE);
       }
       if(checkPointsRectangleFlag)
       {
          rectangle = checkPointsRectangle;
          prepareGeometryChange();
       }

    }
    else
    {
        qreal x1,x2,y1,y2;
        rectangleTemp.getCoords( &x1,&y1,&x2,&y2);
        checkPointsRectangle.setCoords( x1 - ( m_mousePress.x() - event->pos().x()),
                             y1 - ( m_mousePress.y() - event->pos().y()),
                             x2 - ( m_mousePress.x() - event->pos().x()),
                             y2 - ( m_mousePress.y() - event->pos().y()));
        checkPointsRectangleFlag = rectA.contains(checkPointsRectangle.topLeft())?
                                   rectA.contains(checkPointsRectangle.topRight())?
                                   rectA.contains(checkPointsRectangle.bottomLeft())?
                                   rectA.contains(checkPointsRectangle.bottomRight()):false:false:false;
        if(checkPointsRectangleFlag)
        {
           rectangle = checkPointsRectangle;
           update();
        }
        QGraphicsItem::mouseMoveEvent(event);
    }

    savePoints( rectangle);

}

void CVDGraphicsItemRectangle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    if (event->button() == Qt::LeftButton && m_isResizing) {
        m_isResizing = false;

    } else {
        QGraphicsItem::mouseReleaseEvent(event);
    }

//------------------------------------------------------------
    emit updateRectangleSignal(rectangle);
//------------------------------------------------------------

    emit updateRectangleItemSignal();
}




