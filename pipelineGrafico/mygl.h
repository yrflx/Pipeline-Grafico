#ifndef _MYGL_H_
#define _MYGL_H_
#define GL_CLAMP_TO_EDGE 0x812F
#include <stdio.h>
#include <windows.h>
#include "definitions.h"
#include <math.h>
#define PI 3.1415
//*****************************************************************************
// Defina aqui as suas funções gráficas
//*****************************************************************************

    typedef struct{
        float R;
        float G;
        float B;
        float A = 255;
    } tColor;

    void putPixel(int x, int y, tColor cor){

        int pix = (4*x) + (4*y*IMAGE_WIDTH);

        FBptr[pix+0] = cor.R;
        FBptr[pix+1] = cor.G;
        FBptr[pix+2] = cor.B;
        FBptr[pix+3] = cor.A;
    }


    void drawLineNoIntercalled(int x0, int y0, int x1, int y1, tColor cor0){

         int dx = x1 - x0;
        int dy = y1 - y0;

        tColor cor = cor0;

        int inverter = 0;
        if(abs(dy)>abs(dx)){
            inverter = 1;
            int aux = x0;
            x0 = y0;
            y0 = aux;

            aux = x1;
            x1 = y1;
            y1 = aux;

            dx = x1 - x0;
            dy = y1 - y0;

        }
        if( dx < 0  ){
            int aux = x0;
            x0 = x1;
            x1 = aux;

            aux = y0;
            y0 = y1;
            y1 = aux;

            dx = x1 - x0;
            dy = y1 - y0;

        }

        int yi = 1;
        if(dy < 0 ){
            dy*=-1;
            yi=-1;
        }

        int d = 2 * dy - dx;
        int incr_e = 2 * dy;
        int incr_ne = 2 * (dy - dx);
        int x = x0;
        int y = y0;

        putPixel(x, y, cor0);
        while(x < x1){
            if(d <= 0){
                d+=incr_e;
                x++;
            }else{
                d+=incr_ne;
                x++;
                y+=yi;
            }


            if(inverter){
                putPixel(y, x, cor);
            }else{
                putPixel(x, y, cor);
            }



        }


    }


    void drawLine(int x0, int y0, int x1, int y1, tColor cor0,tColor cor1){
        int dx = x1 - x0;
        int dy = y1 - y0;

        tColor cor = cor0;

        int inverter = 0;
        if(abs(dy)>abs(dx)){
            inverter = 1;
            int aux = x0;
            x0 = y0;
            y0 = aux;

            aux = x1;
            x1 = y1;
            y1 = aux;

            dx = x1 - x0;
            dy = y1 - y0;

        }
        if( dx < 0  ){
            int aux = x0;
            x0 = x1;
            x1 = aux;

            aux = y0;
            y0 = y1;
            y1 = aux;

            dx = x1 - x0;
            dy = y1 - y0;

            tColor corAux = cor0;
            cor0 = cor1;
            cor1 = corAux;

        }

        int yi = 1;
        if(dy < 0 ){
            dy*=-1;
            yi=-1;
        }



        int d = 2 * dy - dx;
        int incr_e = 2 * dy;
        int incr_ne = 2 * (dy - dx);
        int x = x0;
        int y = y0;

        putPixel(x, y, cor0);
        while(x < x1){
            if(d <= 0){
                d+=incr_e;
                x++;
            }else{
                d+=incr_ne;
                x++;
                y+=yi;
            }

            cor.R += ((cor1.R - cor0.R)/abs(dx));
            cor.G += ((cor1.G - cor0.G)/abs(dx));
            cor.B += ((cor1.B - cor0.B)/abs(dx));


            if(inverter){
                putPixel(y, x, cor);
            }else{
                putPixel(x, y, cor);
            }



        }

    }

    void drawTriangle(int x0,int y0, int x1, int y1, int x2, int y2, tColor cor0, tColor cor1, tColor cor2){

        drawLine(x0,y0, x1,y1, cor0, cor1);
        drawLine(x1,y1, x2,y2, cor1, cor2);
        drawLine(x0,y0, x2,y2, cor0, cor2);

    }

    void drawTriangleSimple(int x0,int y0, int x1, int y1, int x2, int y2, tColor cor0){

        drawLineNoIntercalled(x0,y0, x1,y1, cor0);
        drawLineNoIntercalled(x1,y1, x2,y2, cor0);
        drawLineNoIntercalled(x0,y0, x2,y2, cor0);

    }


    void drawCircleFilled(int xc, int yc, int r, int angle, tColor cor0, tColor cor1){

        for(double i=30;i<angle;i++){
            double angulo = i;

            double xp = xc + r*cos(angulo*PI/180);
            double yp = yc + r*sin(angulo*PI/180);

            drawLine(xc,yc,xp,yp, cor0, cor1);
        }

    }


    void drawCircle(int xc, int yc, int r, int angle, tColor cor0, tColor cor1){

        tColor cor = cor0;

        for(double i=0;i<angle;i+=0.001){
            double angulo = i;

            cor.R += (cor1.R - cor0.R)/angle;
            cor.G += (cor1.G - cor0.G)/angle;
            cor.B += (cor1.G - cor0.B)/angle;


            double xp = xc + r*cos(angulo*PI/180);
            double yp = yc + r*sin(angulo*PI/180);

            putPixel(xp, yp, cor);
        }

    }




#endif // _MYGL_H_

