#include "horse.h"
Horse::Horse(QWidget *parent) :
    QGLWidget(parent)
{
    angles[TailZ] = 0.0;
    angles[TailY] = 0.0;
    angles[NeckZ] = 60.0;
    angles[NeckY] = 0.0;
    angles[HeadZ] = -90.0;
    angles[HeadY] = 0.0;
    angles[HipRA] = -20.0;
    angles[HipRB] = 25.0;
    angles[HipLA] = -20.0;
    angles[HipLB] = 25.0;
    angles[RearRUpperLeg] = -25.0;
    angles[RearRLowerLeg] = 20.0;
    angles[RearLUpperLeg] = -25.0;
    angles[RearLLowerLeg] = 20.0;
    angles[ShoulderRA] = 20.0;
    angles[ShoulderRB] = -40.0;
    angles[ShoulderLA] = 20.0;
    angles[ShoulderLB] = -40.0;
    angles[FrontRUpperLeg] = 20.0;
    angles[FrontRLowerLeg] = -5.0;
    angles[FrontLUpperLeg] = 20.0;
    angles[FrontLLowerLeg] = -105.0;

    globalRotateX = 10.0;
    globalRotateY = 30.0;

    Index = 0;
    projection = mat4(1.0);

    cube_points[0] = point4( -0.5, -0.5, 0.5, 1.0 );
    cube_points[1] = point4( -0.5, 0.5, 0.5, 1.0 );
    cube_points[2] = point4( 0.5, 0.5, 0.5, 1.0 );
    cube_points[3] = point4( 0.5, -0.5, 0.5, 1.0 );
    cube_points[4] = point4( -0.5, -0.5, -0.5, 1.0 );
    cube_points[5] = point4( -0.5, 0.5, -0.5, 1.0 );
    cube_points[6] = point4( 0.5, 0.5, -0.5, 1.0 );
    cube_points[7] = point4( 0.5, -0.5, -0.5, 1.0 );

    cube_colors[0] = color4( 0.0, 0.0, 0.0, 1.0 );  // black
    cube_colors[1] = color4( 1.0, 0.0, 0.0, 1.0 );  // red
    cube_colors[2] = color4( 1.0, 1.0, 0.0, 1.0 );  // yellow
    cube_colors[3] = color4( 0.0, 1.0, 0.0, 1.0 );  // green
    cube_colors[4] = color4( 0.0, 0.0, 1.0, 1.0 );  // blue
    cube_colors[5] = color4( 1.0, 0.0, 1.0, 1.0 );  // magenta
    cube_colors[6] = color4( 1.0, 1.0, 1.0, 1.0 );  // white
    cube_colors[7] = color4( 0.0, 1.0, 1.0, 1.0 );  // cyan

    colorcube();

    clearColor = color4(0.0,0.0,0.0,1.0);

}
float Horse::getRelativeX(int x) {
    return (float)x / (width() / 2) - 1.0;
}

float Horse::getRelativeY(int y) {
    return (float)(height() - y) / (height() / 2) - 1.0;
}

void Horse::mousePressEvent(QMouseEvent *event) {
    mouseDownX = getRelativeX(event->x());
    mouseDownY = getRelativeY(event->y());
}

void Horse::mouseMoveEvent(QMouseEvent *event) {
    qDebug() << "angle " << angles[FrontLLowerLeg];
    float x = getRelativeX(event->x());
    float y = getRelativeY(event->y());

    globalRotateX = globalRotateX + (y-mouseDownY)/2*180;
    if(globalRotateX > 360.0)
        globalRotateX -= 360;
    else if(globalRotateX < 0)
        globalRotateX += 360;
    globalRotateY = globalRotateY + (x-mouseDownX)/2*180;
    if(globalRotateY > 360.0)
        globalRotateY -= 360;
    else if(globalRotateY < 0)
        globalRotateY += 360;

    qDebug() << globalRotateX << globalRotateY;
    mouseDownX = x;
    mouseDownY = y;
    //qDebug() << getRelativeX(event->x()) << getRelativeY(event->y());
    update();
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

    glEnable( GL_DEPTH_TEST );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );


}

void Horse::paintGL() {
    glClearColor(EXPANDVEC4(clearColor));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    model_view = Translate(0.0,0.0,0.0)*RotateX(-globalRotateX)*RotateY(globalRotateY)*Scale(0.8,0.8,0.8);
    trunk();

    mvstack.push(model_view);
    model_view *= (Translate(-(TRUNK_X/2),TRUNK_Y/2,0.0));

    tail();


    model_view = mvstack.pop();

    mvstack.push(model_view);
    model_view *= (Translate(TRUNK_X/2,TRUNK_Y/2,0.0)*RotateZ(angles[NeckZ])*RotateY(angles[NeckY]));
    neck();

    mvstack.push(model_view);
    model_view *= Translate(NECK_X,-NECK_Y/2,0.0)*RotateZ(angles[TailZ])*RotateY(angles[TailY]);
    head();

    model_view = mvstack.pop();

    model_view = mvstack.pop();


    mvstack.push(model_view);
    model_view *= Translate(-TRUNK_X/5,TRUNK_Y/2-TRUNK_Y/10,-TRUNK_Z/3)*RotateZ(angles[HipRA]);
    hipa();

    mvstack.push(model_view);
    model_view *= (Translate(0.0,-HIPA_Y,0.0)*RotateZ(angles[HipRB]));
    hipb();

    mvstack.push(model_view);
    model_view *= (Translate(0.0,-HIPB_Y,0.0)*RotateZ(angles[RearRUpperLeg]));
    rearUpperLeg();


    mvstack.push(model_view);
    model_view *= (Translate(0.0,-RULEG_Y,0.0)*RotateZ(angles[RearRLowerLeg]));
    rearLowerLeg();
    model_view = mvstack.pop();

    model_view = mvstack.pop();

    model_view = mvstack.pop();

    model_view = mvstack.pop();

    mvstack.push(model_view);
    model_view *= Translate(-TRUNK_X/5,TRUNK_Y/2-TRUNK_Y/10,TRUNK_Z/3)*RotateZ(angles[HipLA]);
    hipa();

    mvstack.push(model_view);
    model_view *= (Translate(0.0,-HIPA_Y,0.0)*RotateZ(angles[HipLB]));
    hipb();

    mvstack.push(model_view);
    model_view *= (Translate(0.0,-HIPB_Y,0.0)*RotateZ(angles[RearLUpperLeg]));
    rearUpperLeg();


    mvstack.push(model_view);
    model_view *= (Translate(0.0,-RULEG_Y,0.0)*RotateZ(angles[RearLLowerLeg]));
    rearLowerLeg();
    model_view = mvstack.pop();

    model_view = mvstack.pop();

    model_view = mvstack.pop();

    model_view = mvstack.pop();

    mvstack.push(model_view);
    model_view *= Translate(TRUNK_X/2-SHOULDERA_X,TRUNK_Y/2-TRUNK_Y/10,TRUNK_Z/3)*RotateZ(angles[ShoulderRA]);
    shouldera();

    mvstack.push(model_view);
    model_view *= Translate(0.0,-SHOULDERA_Y,0.0)*RotateZ(angles[ShoulderRB]);
    shoulderb();

    mvstack.push(model_view);
    model_view *= (Translate(0.0,-SHOULDERB_Y,0.0)*RotateZ(angles[FrontRUpperLeg]));
    frontUpperLeg();


    mvstack.push(model_view);
    model_view *= (Translate(0.0,-FULEG_Y,0.0)*RotateZ(angles[FrontRLowerLeg]));
    frontLowerLeg();
    model_view = mvstack.pop();

    model_view = mvstack.pop();

    model_view = mvstack.pop();
    model_view = mvstack.pop();

    mvstack.push(model_view);
    model_view *= Translate(TRUNK_X/2-SHOULDERA_X,TRUNK_Y/2-TRUNK_Y/10,-TRUNK_Z/3)*RotateZ(angles[ShoulderLA]);
    shouldera();

    mvstack.push(model_view);
    model_view *= Translate(0.0,-SHOULDERA_Y,0.0)*RotateZ(angles[ShoulderLB]);
    shoulderb();

    mvstack.push(model_view);
    model_view *= (Translate(0.0,-SHOULDERB_Y,0.0)*RotateZ(angles[FrontLUpperLeg]));
    frontUpperLeg();


    mvstack.push(model_view);
    model_view *= (Translate(0.0,-FULEG_Y,0.0)*RotateZ(angles[FrontLLowerLeg]));
    frontLowerLeg();

    model_view = mvstack.pop();

    model_view = mvstack.pop();

    model_view = mvstack.pop();
    model_view = mvstack.pop();



}

void Horse::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
    shaders.setUniformValue("Projection",QMatrix4x4(EXPANDMAT44(projection)));
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

void Horse::neck() {
    mvstack.push( model_view );

    mat4 instance = model_view * (Translate((NECK_X/2),-NECK_Y/2,0)*Scale(NECK_X,NECK_Y,NECK_Z));

    shaders.setUniformValue("ModelView",QMatrix4x4(EXPANDMAT44(instance)));
    glDrawArrays( GL_TRIANGLES, 0, NumNodes );

    model_view = mvstack.pop();
}
void Horse::head() {
    mvstack.push( model_view );

    mat4 instance = model_view * (RotateZ(angles[HeadZ])*RotateY(angles[HeadY])*Translate((HEAD_X/2),0.0,0)*Scale(HEAD_X,HEAD_Y,HEAD_Z));

    shaders.setUniformValue("ModelView",QMatrix4x4(EXPANDMAT44(instance)));
    glDrawArrays( GL_TRIANGLES, 0, NumNodes );

    model_view = mvstack.pop();
}

void Horse::tail() {
    mvstack.push( model_view );

    mat4 instance = model_view * (Translate(-(TAIL_X/2),-TAIL_Y/2,0)*Scale(TAIL_X,TAIL_Y,TAIL_Z));

    shaders.setUniformValue("ModelView",QMatrix4x4(EXPANDMAT44(instance)));
    glDrawArrays( GL_TRIANGLES, 0, NumNodes );

    model_view = mvstack.pop();
}
void Horse::trunk() {
    mvstack.push( model_view );

    mat4 instance = model_view * (Scale(TRUNK_X,TRUNK_Y,TRUNK_Z));

    shaders.setUniformValue("ModelView",QMatrix4x4(EXPANDMAT44(instance)));
    glDrawArrays( GL_TRIANGLES, 0, NumNodes );

    model_view = mvstack.pop();
}
void Horse::hipa() {
    mvstack.push( model_view );

    mat4 instance = model_view * (Translate(0.0,-HIPA_Y/2,0.0)*Scale(HIPA_X,HIPA_Y,HIPA_Z));

    shaders.setUniformValue("ModelView",QMatrix4x4(EXPANDMAT44(instance)));
    glDrawArrays( GL_TRIANGLES, 0, NumNodes );

    model_view = mvstack.pop();
}
void Horse::hipb() {
    mvstack.push( model_view );

    mat4 instance = model_view * (Translate(0.0,-HIPB_Y/2,0.0)*Scale(HIPB_X,HIPB_Y,HIPB_Z));

    shaders.setUniformValue("ModelView",QMatrix4x4(EXPANDMAT44(instance)));
    glDrawArrays( GL_TRIANGLES, 0, NumNodes );

    model_view = mvstack.pop();
}
void Horse::rearUpperLeg() {
    mvstack.push( model_view );

    mat4 instance = model_view * (Translate(0.0,-RULEG_Y/2,0.0)*Scale(RULEG_X,RULEG_Y,RULEG_Z));

    shaders.setUniformValue("ModelView",QMatrix4x4(EXPANDMAT44(instance)));
    glDrawArrays( GL_TRIANGLES, 0, NumNodes );

    model_view = mvstack.pop();
}
void Horse::rearLowerLeg() {
    mvstack.push( model_view );

    mat4 instance = model_view * (Translate(0.0,-RLLEG_Y/2,0.0)*Scale(RLLEG_X,RLLEG_Y,RLLEG_Z));

    shaders.setUniformValue("ModelView",QMatrix4x4(EXPANDMAT44(instance)));
    glDrawArrays( GL_TRIANGLES, 0, NumNodes );

    model_view = mvstack.pop();
}
void Horse::frontUpperLeg() {
    mvstack.push( model_view );

    mat4 instance = model_view * (Translate(0.0,-FULEG_Y/2,0.0)*Scale(FULEG_X,FULEG_Y,FULEG_Z));

    shaders.setUniformValue("ModelView",QMatrix4x4(EXPANDMAT44(instance)));
    glDrawArrays( GL_TRIANGLES, 0, NumNodes );

    model_view = mvstack.pop();
}
void Horse::frontLowerLeg() {
    mvstack.push( model_view );

    mat4 instance = model_view * (Translate(0.0,-FLLEG_Y/2,0.0)*Scale(FLLEG_X,FLLEG_Y,FLLEG_Z));

    shaders.setUniformValue("ModelView",QMatrix4x4(EXPANDMAT44(instance)));
    glDrawArrays( GL_TRIANGLES, 0, NumNodes );

    model_view = mvstack.pop();
}
void Horse::shouldera() {
    mvstack.push( model_view );

    mat4 instance = model_view * (Translate(0.0,-SHOULDERA_Y/2,0.0)*Scale(SHOULDERA_X,SHOULDERA_Y,SHOULDERA_Z));

    shaders.setUniformValue("ModelView",QMatrix4x4(EXPANDMAT44(instance)));
    glDrawArrays( GL_TRIANGLES, 0, NumNodes );

    model_view = mvstack.pop();
}
void Horse::shoulderb() {
    mvstack.push( model_view );

    mat4 instance = model_view * (Translate(0.0,-SHOULDERB_Y/2,0.0)*Scale(SHOULDERB_X,SHOULDERB_Y,SHOULDERB_Z));

    shaders.setUniformValue("ModelView",QMatrix4x4(EXPANDMAT44(instance)));
    glDrawArrays( GL_TRIANGLES, 0, NumNodes );

    model_view = mvstack.pop();
}
void Horse::quad( int a, int b, int c, int d )
{
    colors[Index] = cube_colors[a]; vertices[Index] = cube_points[a]; Index++;
    colors[Index] = cube_colors[a]; vertices[Index] = cube_points[b]; Index++;
    colors[Index] = cube_colors[a]; vertices[Index] = cube_points[c]; Index++;
    colors[Index] = cube_colors[a]; vertices[Index] = cube_points[a]; Index++;
    colors[Index] = cube_colors[a]; vertices[Index] = cube_points[c]; Index++;
    colors[Index] = cube_colors[a]; vertices[Index] = cube_points[d]; Index++;
}

void Horse::colorcube( void )
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}
