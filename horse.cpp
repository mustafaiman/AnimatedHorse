#include "horse.h"

Horse::Horse(QWidget *parent) :
    QGLWidget(parent)
{
    vertices[0] = point4(-0.5,0.5,0.0,1.0);
    vertices[1] = point4(0.5,0.5,0.0,1.0);
    vertices[2] = point4(0.5,-0.5,0.0,1.0);
    vertices[3] = point4(-0.5,-0.5,0.0,1.0);

    colors[0] = color4(1.0,0.0,0.0,1.0);
    colors[1] = color4(1.0,0.0,0.0,1.0);
    colors[2] = color4(1.0,1.0,0.0,1.0);
    colors[3] = color4(1.0,0.0,0.0,1.0);

    clearColor = color4(1.0,1.0,1.0,1.0);
}

void Horse::initializeGL() {
    initializeGLFunctions();

    initShaders();

    glGenBuffers(1,&bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

    GLuint vPosition = shaders.attributeLocation("vPosition");
    shaders.enableAttributeArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = shaders.attributeLocation("vColor");
    shaders.enableAttributeArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));



}

void Horse::paintGL() {
    glClearColor(EXPANDVEC4(clearColor));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDrawArrays(GL_LINE_LOOP, 0, 4);

}

void Horse::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
}

void Horse::initShaders() {
    Q_INIT_RESOURCE(resources);

    if(!shaders.addShaderFromSourceFile(QGLShader::Vertex, ":/vshader.glsl") )
        close();

    if(shaders.addShaderFromSourceFile(QGLShader::Fragment,":/fshader.glsl") )
        close();

    if(!shaders.link())
        close();

    if(!shaders.bind())
        close();

}
