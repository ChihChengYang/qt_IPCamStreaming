#ifndef CVDGRAPHSCENE_H
#define CVDGRAPHSCENE_H

#include "CVDGraphicsItems.h"
#include <QGraphicsScene>
#include <QtOpenGL>



class CVDGraphScene : public QGraphicsScene
{          Q_OBJECT
public:
    CVDGraphScene();
    virtual ~CVDGraphScene();

    void drawBackground(QPainter *painter, const QRectF &rect);

    void paintBKPureColor();

    void initGL();

    void setRectangleWheel(qreal step);

    GLuint  rgbTexture;


    //---for DECODER_TYPE_FFMPEG,DECODER_TYPE_CUDA_RAW
    unsigned char* decRawData;
    int decDataWidth;
    int decDataHeight;

    bool decDrawInitedFlag;


    int glWidth,glHeight;

    int exglWidth, exglHeight;

    CVDGraphicsItemRectangle* rectangleItem;

public slots:

private:


};

#endif // CVDGRAPHSCENE_H
