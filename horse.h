#ifndef HORSE_H
#define HORSE_H

#include<gl/glew.h>
#include <QGLWidget>
#include <QGLFunctions>
#include <QGLShaderProgram>
#include "Angel.h"

#define NUM_VERTICES 10

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

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

private:
    QGLShaderProgram shaders;
    GLuint bufferId;
    point4 vertices[NUM_VERTICES];
    color4 colors[NUM_VERTICES];
    color4 clearColor;


signals:

public slots:

};

#endif // HORSE_H
