#ifndef HORSE_H
#define HORSE_H

#include<gl/glew.h>
#include <QGLWidget>
#include <QGLFunctions>
#include <QGLShaderProgram>
#include <QStack>
#include <QMouseEvent>
#include "Angel.h"

#define NUM_VERTICES 500
#define MODEL_SIZE 2
typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

#define TRUNK_X 1.0
#define TRUNK_Y 0.65
#define TRUNK_Z 0.65
#define TAIL_X 0.15
#define TAIL_Y 0.05
#define TAIL_Z 0.05
#define NECK_X 0.25
#define NECK_Y 0.15
#define NECK_Z 0.15
#define HEAD_X 0.30
#define HEAD_Y 0.17
#define HEAD_Z 0.15
#define HIPA_X 0.08
#define HIPA_Y 0.3
#define HIPA_Z 0.08
#define HIPB_X 0.06
#define HIPB_Y 0.3
#define HIPB_Z 0.06
#define RULEG_X 0.06
#define RULEG_Y 0.35
#define RULEG_Z 0.06
#define RLLEG_X 0.04
#define RLLEG_Y 0.35
#define RLLEG_Z 0.04
#define SHOULDERA_X 0.09
#define SHOULDERA_Y 0.4
#define SHOULDERA_Z 0.09
#define SHOULDERB_X 0.07
#define SHOULDERB_Y 0.25
#define SHOULDERB_Z 0.07
#define FULEG_X 0.06
#define FULEG_Y 0.35
#define FULEG_Z 0.06
#define FLLEG_X 0.04
#define FLLEG_Y 0.35
#define FLLEG_Z 0.04

class Horse : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT
public:
    explicit Horse(QWidget *parent = 0);

protected:
    void initializeGL();
    void initShaders();
    void paintGL();
    void resizeGL(int width,int height);
    void quad( int a, int b, int c, int d );
    void colorcube( void );
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    //body-parts
    void trunk();
    void tail();
    void neck();
    void head();
    void hipa();
    void hipb();
    void rearUpperLeg();
    void rearLowerLeg();
    void shouldera();
    void shoulderb();
    void frontUpperLeg();
    void frontLowerLeg();

    float angles[30];

private:
    float getRelativeX(int x);
    float getRelativeY(int y);
    float mouseDownX;
    float mouseDownY;
    float globalRotateX;
    float globalRotateY;

    enum {
        TailZ = 0,
        TailY = 1,
        NeckZ = 2,
        NeckY = 3,
        HeadZ = 4,
        HeadY = 5,
        HipRA = 6,
        HipLA = 7,
        HipRB = 8,
        HipLB =9,
        RearRUpperLeg = 10,
        RearRLowerLeg = 11,
        RearLUpperLeg = 12,
        RearLLowerLeg = 13,
        ShoulderRA = 14,
        ShoulderRB = 15,
        ShoulderLA = 16,
        ShoulderLB = 17,
        FrontRUpperLeg = 18,
        FrontRLowerLeg = 19,
        FrontLUpperLeg = 20,
        FrontLLowerLeg = 21
    };


    QStack<mat4> mvstack;
    mat4 model_view;
    mat4 projection;
    QGLShaderProgram shaders;
    GLuint bufferId;
    point4 vertices[NUM_VERTICES];
    color4 colors[NUM_VERTICES];
    point4 cube_points[8];
    int Index;
    const int NumNodes = 36;
    color4 clearColor;

    color4 cube_colors[8];
signals:

public slots:

};

#endif // HORSE_H
