#ifndef HORSE_H
#define HORSE_H

#include<gl/glew.h>
#include <QGLWidget>
#include <QGLFunctions>
#include <QGLShaderProgram>
#include <QStack>
#include <QMouseEvent>
#include "Angel.h"
#include <string>
#include <fstream>
#include <QThread>
#include <sstream>

#define MAXFRAMES 30
#define NUM_VERTICES 500
#define MODEL_SIZE 2
#define SLEEPTIME 50
typedef Angel::vec4 point4;
typedef Angel::vec4 color4;
typedef Angel::vec2 point2;

#define GROUND_X 30.0
#define GROUND_Y 2.0
#define GROUND_Z 30.0
#define TRUNK_X 1.0
#define TRUNK_Y 0.65
#define TRUNK_Z 0.65
#define UPPERTAIL_X 0.33
#define UPPERTAIL_Y 0.05
#define UPPERTAIL_Z 0.05
#define TAIL_X 0.3
#define TAIL_Y 0.05
#define TAIL_Z 0.05
#define NECK_X 0.35
#define NECK_Y 0.15
#define NECK_Z 0.15
#define HEAD_X 0.30
#define HEAD_Y 0.17
#define HEAD_Z 0.15
#define HIPA_X 0.12
#define HIPA_Y 0.3
#define HIPA_Z 0.12
#define HIPB_X 0.11
#define HIPB_Y 0.3
#define HIPB_Z 0.11
#define RULEG_X 0.09
#define RULEG_Y 0.35
#define RULEG_Z 0.09
#define RLLEG_X 0.07
#define RLLEG_Y 0.40
#define RLLEG_Z 0.07
#define SHOULDERA_X 0.12
#define SHOULDERA_Y 0.4
#define SHOULDERA_Z 0.12
#define SHOULDERB_X 0.11
#define SHOULDERB_Y 0.25
#define SHOULDERB_Z 0.11
#define FULEG_X 0.09
#define FULEG_Y 0.35
#define FULEG_Z 0.09
#define FLLEG_X 0.07
#define FLLEG_Y 0.35
#define FLLEG_Z 0.07
#define HOOF_X 0.09
#define HOOF_Y 0.08
#define HOOF_Z 0.08

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
    void wheelEvent(QWheelEvent *event);

    //ground
    void ground();
    //body-parts
    void trunk();
    void upperTail();
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
    void hoof();
    void nextJoint();
    void previousJoint();
    void constraintAngle(int ang, double low, double high);
    void reloadActiveFrame();
    void setShading(int a);


    point4 light_position;
    color4 light_ambient;
    color4 light_diffuse;
    color4 light_specular;
    color4 material_ambient;
    color4 material_diffuse;
    color4 material_specular;

    color4 horse_ambient;
    color4 horse_diffuse;
    color4 horse_specular;
    float horse_shininess;

    color4 ground_ambient;
    color4 ground_diffuse;
    color4 ground_specular;
    float ground_shininess;

    float  material_shininess = 100.0;
    float angles[MAXFRAMES][30];

private:
    float getRelativeX(int x);
    float getRelativeY(int y);
    void selectColor(int);
    void selectColor(int part1, int part2);
    void loadFrame(int act);
    int frameInterval[MAXFRAMES];
    float mouseDownX;
    float mouseDownY;
    float globalRotateX;
    float globalRotateY;
    int activeFrame;
    int totalFrames;
    double radius;
    GLuint fillMode;
    GLuint mytex[1];
    QImage imageT;
    QImage imageB;


    enum {
        NeckZ = 0,
        NeckY = 1,
        HeadZ = 2,
        HeadY = 3,
        HipRA = 4,
        HipRB = 5,
        RearRUpperLeg = 6,
        RearRLowerLeg = 7,
        HoofRR = 8,
        HipLA = 9,
        HipLB = 10,
        RearLUpperLeg = 11,
        RearLLowerLeg = 12,
        HoofRL = 13,
        ShoulderRA = 14,
        ShoulderRB = 15,
        FrontRUpperLeg = 16,
        FrontRLowerLeg = 17,
        HoofFR = 18,
        ShoulderLA = 19,
        ShoulderLB = 20,
        FrontLUpperLeg = 21,
        FrontLLowerLeg = 22,
        HoofFL = 23,
        UpperTail = 24,
        TailZ = 25,
        TailY = 26,
        Whole = 27
    };


    QStack<mat4> mvstack;
    mat4 model_view;
    mat4 projection;
    vec4 camera;
    QGLShaderProgram shaders;
    GLuint bufferId;
    point4 vertices[NUM_VERTICES];
    point2 tex_coord[NUM_VERTICES];
    vec3 normals[NUM_VERTICES];
    point4 cube_points[8];
    int Index;
    const int NumNodes = 36;
    color4 clearColor;
    int activePart;
    color4 highlightColor;
    color4 normalColor;

    color4 cube_colors[16];
signals:
    void frameChanged(QString str);

public slots:
    void loadAnglesFromFile(std::string fileName);
    void animate();
    void recordActiveScene(int interval);
    std::string dump();
    void changeFill(bool a);
    void changeActiveFrame(int fNumber);
    void editActiveScene();
    void selectWhole();

};

#endif // HORSE_H
