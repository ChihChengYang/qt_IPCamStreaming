
#include "CVDGraphScene.h"
#include <QPainter>
#include <iostream>
#include <QDebug>
using namespace std;

CVDGraphScene::CVDGraphScene()
{
      decDrawInitedFlag = false;
      decDataWidth = 0;
      decDataHeight =0;
      decRawData = NULL;

       glWidth = 0;
       glHeight = 0;

    exglWidth = 0; exglHeight = 0;

    rectangleItem = new CVDGraphicsItemRectangle(0,0,352,240);
    this->addItem(rectangleItem);


}

CVDGraphScene::~CVDGraphScene()
{

}


void CVDGraphScene::initGL()
{
    glEnable( GL_TEXTURE_2D );
    glGenTextures(1, &rgbTexture);
    glBindTexture( GL_TEXTURE_2D, rgbTexture );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->decDataWidth, this->decDataHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glBindTexture(GL_TEXTURE_2D, 0);

}


void CVDGraphScene::drawBackground(QPainter *painter, const QRectF &)
{

    painter->beginNativePainting();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

        if(decRawData != NULL)
        {
            if( !decDrawInitedFlag )
            {
                if(decDataWidth > 0 && decDataHeight > 0)
                {
                    decDrawInitedFlag = true;
                    initGL();
                }
            }
            else
            {

                 glBindTexture(GL_TEXTURE_2D, rgbTexture);
                 glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, decDataWidth, decDataHeight,
                              GL_RGB , GL_UNSIGNED_BYTE, decRawData);
                 glBegin(GL_QUADS);

                 glTexCoord2f(0, 1); glVertex2f(-1, -1);
                 glTexCoord2f(1, 1); glVertex2f( 1, -1);
                 glTexCoord2f(1, 0); glVertex2f( 1,  1);
                 glTexCoord2f(0, 0); glVertex2f(-1,  1);

               glEnd();
               glBindTexture(GL_TEXTURE_2D, 0);

            }

        }


    glFlush();
    painter->endNativePainting();;
}

void CVDGraphScene::setRectangleWheel(qreal step)
{
}
