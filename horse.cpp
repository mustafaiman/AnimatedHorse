#include "horse.h"
Horse::Horse(QWidget *parent) :
    QGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
    qDebug() << focusWidget();
    normalColor = color4(1.0,1.0,1.0,1.0);
    highlightColor = color4(1.0, 0.0, 0.0, 1.0);
    angles[0][TailZ] = 0.0;
    angles[0][TailY] = 30.0;
    angles[0][NeckZ] = 60.0;
    angles[0][NeckY] = 0.0;
    angles[0][HeadZ] = -90.0;
    angles[0][HeadY] = 0.0;
    angles[0][HipRA] = -20.0;
    angles[0][HipLA] = -20.0;
    angles[0][HipRB] = 25.0;
    angles[0][HipLB] = 25.0;
    angles[0][RearRUpperLeg] = -25.0;
    angles[0][RearRLowerLeg] = 20.0;
    angles[0][RearLUpperLeg] = -25.0;
    angles[0][RearLLowerLeg] = 20.0;
    angles[0][ShoulderRA] = 20.0;
    angles[0][ShoulderRB] = -40.0;
    angles[0][ShoulderLA] = 20.0;
    angles[0][ShoulderLB] = -40.0;
    angles[0][FrontRUpperLeg] = 20.0;
    angles[0][FrontRLowerLeg] = -5.0;
    angles[0][FrontLUpperLeg] = 20.0;
    angles[0][FrontLLowerLeg] = -105.0;
    angles[0][UpperTail] = 10.0;
    angles[0][HoofRR] = 0.0;
    angles[0][HoofRL] = 0.0;
    angles[0][HoofFR] = 0.0;
    angles[0][HoofFL] = 0.0;

    activeFrame = 0;
    totalFrames = 0;

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

    cube_colors[0] = color4( 1.0, 1.0, 1.0, 1.0 );  // black
    cube_colors[1] = color4( 1.0, 1.0, 1.0, 1.0 );  // red
    cube_colors[2] = color4( 1.0, 1.0, 1.0, 1.0 );  // yellow
    cube_colors[3] = color4( 1.0, 1.0, 1.0, 1.0 );  // green
    cube_colors[4] = color4( 1.0, 1.0, 1.0, 1.0 );  // blue
    cube_colors[5] = color4( 1.0, 1.0, 1.0, 1.0 );  // magenta
    cube_colors[6] = color4( 1.0, 1.0, 1.0, 1.0 );  // white
    cube_colors[7] = color4( 1.0, 1.0, 1.0, 1.0 );  // cyan

    cube_colors[8] = color4( 1.0, 0.0, 0.0, 1.0 );  // cyan
    cube_colors[9] = color4( 1.0, 0.0, 0.0, 1.0 );  // black
    cube_colors[10] = color4( 1.0, 0.0, 0.0, 0.0 );  // red
    cube_colors[11] = color4( 1.0, 0.0, 0.0, 0.0 );  // yellow
    cube_colors[12] = color4( 1.0, 0.0, 0.0, 0.0 );  // green
    cube_colors[13] = color4( 1.0, 0.0, 0.0, 0.0 );  // blue
    cube_colors[14] = color4( 1.0, 0.0, 0.0, 0.0 );  // magenta
    cube_colors[15] = color4( 1.0, 0.0, 0.0, 0.0 );  // white

    activePart = TailY;

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

    mouseDownX = x;
    mouseDownY = y;
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
/*
    GLuint vColor = shaders.attributeLocation("vColor");
    shaders.enableAttributeArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));
*/
    shaders.setUniformValue("color",EXPANDVEC4(cube_colors[13]));
    glEnable( GL_DEPTH_TEST );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );


}

void Horse::loadFrame(int act) {
    for(int i=0;i<Whole;i++)
        angles[0][i] = angles[act][i];
}

void Horse::paintGL() {
    qDebug() << "paintgl";
    glClearColor(EXPANDVEC4(clearColor));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(activeFrame > 0) {


        model_view = Translate(0.0,0.0,0.0)*RotateX(-globalRotateX)*RotateY(globalRotateY)*Scale(0.8,0.8,0.8);
        trunk();

        mvstack.push(model_view);
        selectColor(UpperTail);
        model_view *= (Translate(-(TRUNK_X/5),TRUNK_Y/2,0.0)*RotateZ(angles[0][UpperTail]));
        upperTail();

        mvstack.push(model_view);
        selectColor(TailY);
        model_view *= Translate(-(UPPERTAIL_X),0.0,0.0)*RotateZ(angles[0][TailZ])*RotateY(angles[0][TailY]);
        tail();

        model_view = mvstack.pop();
        model_view = mvstack.pop();

        mvstack.push(model_view);
        model_view *= (Translate(TRUNK_X/2,TRUNK_Y/2,0.0)*RotateZ(angles[0][NeckZ])*RotateY(angles[0][NeckY]));
        selectColor(NeckY);
        neck();

        mvstack.push(model_view);
        model_view *= Translate(NECK_X,-NECK_Y/2,0.0)*RotateZ(angles[0][HeadZ])*RotateY(angles[0][HeadY]);
        selectColor(HeadY);
        head();

        model_view = mvstack.pop();

        model_view = mvstack.pop();


        mvstack.push(model_view);
        model_view *= Translate(-TRUNK_X/5,TRUNK_Y/2-TRUNK_Y/10,-TRUNK_Z/3)*RotateZ(angles[0][HipRA]);
        selectColor(HipRA);
        hipa();

        mvstack.push(model_view);
        model_view *= (Translate(0.0,-HIPA_Y,0.0)*RotateZ(angles[0][HipRB]));
        selectColor(HipRB);
        hipb();

        mvstack.push(model_view);
        model_view *= (Translate(0.0,-HIPB_Y,0.0)*RotateZ(angles[0][RearRUpperLeg]));
        selectColor(RearRUpperLeg);
        rearUpperLeg();


        mvstack.push(model_view);
        model_view *= (Translate(0.0,-RULEG_Y,0.0)*RotateZ(angles[0][RearRLowerLeg]));
        selectColor(RearRLowerLeg);
        rearLowerLeg();

        mvstack.push(model_view);
        model_view *= Translate(HOOF_X/2-FLLEG_X/2,-FLLEG_Y,0.0)*RotateZ(angles[0][HoofRR]);
        selectColor(HoofRR);
        hoof();
        model_view = mvstack.pop();
        model_view = mvstack.pop();

        model_view = mvstack.pop();

        model_view = mvstack.pop();

        model_view = mvstack.pop();

        mvstack.push(model_view);
        model_view *= Translate(-TRUNK_X/5,TRUNK_Y/2-TRUNK_Y/10,TRUNK_Z/3)*RotateZ(angles[0][HipLA]);
        selectColor(HipLA);
        hipa();

        mvstack.push(model_view);
        model_view *= (Translate(0.0,-HIPA_Y,0.0)*RotateZ(angles[0][HipLB]));
        selectColor(HipLB);
        hipb();

        mvstack.push(model_view);
        model_view *= (Translate(0.0,-HIPB_Y,0.0)*RotateZ(angles[0][RearLUpperLeg]));
        selectColor(RearLUpperLeg);
        rearUpperLeg();


        mvstack.push(model_view);
        model_view *= (Translate(0.0,-RULEG_Y,0.0)*RotateZ(angles[0][RearLLowerLeg]));
        selectColor(RearLLowerLeg);
        rearLowerLeg();

        mvstack.push(model_view);
        model_view *= Translate(HOOF_X/2-FLLEG_X/2,-FLLEG_Y,0.0)*RotateZ(angles[0][HoofRL]);
        selectColor(HoofRL);
        hoof();
        model_view = mvstack.pop();

        model_view = mvstack.pop();

        model_view = mvstack.pop();

        model_view = mvstack.pop();

        model_view = mvstack.pop();

        mvstack.push(model_view);
        model_view *= Translate(TRUNK_X/2-SHOULDERA_X,TRUNK_Y/2-TRUNK_Y/10,TRUNK_Z/3)*RotateZ(angles[0][ShoulderRA]);
        selectColor(ShoulderRA);
        shouldera();

        mvstack.push(model_view);
        model_view *= Translate(0.0,-SHOULDERA_Y,0.0)*RotateZ(angles[0][ShoulderRB]);
        selectColor(ShoulderRB);
        shoulderb();

        mvstack.push(model_view);
        model_view *= (Translate(0.0,-SHOULDERB_Y,0.0)*RotateZ(angles[0][FrontRUpperLeg]));
        selectColor(FrontRUpperLeg);
        frontUpperLeg();


        mvstack.push(model_view);
        model_view *= (Translate(0.0,-FULEG_Y,0.0)*RotateZ(angles[0][FrontRLowerLeg]));
        selectColor(FrontRLowerLeg);
        frontLowerLeg();

        mvstack.push(model_view);
        model_view *= Translate(HOOF_X/2-FLLEG_X/2,-FLLEG_Y,0.0)*RotateZ(angles[0][HoofFR]);
        selectColor(HoofFR);
        hoof();
        model_view = mvstack.pop();

        model_view = mvstack.pop();

        model_view = mvstack.pop();

        model_view = mvstack.pop();
        model_view = mvstack.pop();

        mvstack.push(model_view);
        model_view *= Translate(TRUNK_X/2-SHOULDERA_X,TRUNK_Y/2-TRUNK_Y/10,-TRUNK_Z/3)*RotateZ(angles[0][ShoulderLA]);
        selectColor(ShoulderLA);
        shouldera();

        mvstack.push(model_view);
        model_view *= Translate(0.0,-SHOULDERA_Y,0.0)*RotateZ(angles[0][ShoulderLB]);
        selectColor(ShoulderLB);
        shoulderb();

        mvstack.push(model_view);
        model_view *= (Translate(0.0,-SHOULDERB_Y,0.0)*RotateZ(angles[0][FrontLUpperLeg]));
        selectColor(FrontLUpperLeg);
        frontUpperLeg();


        mvstack.push(model_view);
        model_view *= (Translate(0.0,-FULEG_Y,0.0)*RotateZ(angles[0][FrontLLowerLeg]));
        selectColor(FrontLLowerLeg);
        frontLowerLeg();

        mvstack.push(model_view);
        model_view *= Translate(HOOF_X/2-FLLEG_X/2,-FLLEG_Y,0.0)*RotateZ(angles[0][HoofFL]);
        selectColor(HoofFL);
        hoof();
        model_view = mvstack.pop();

        model_view = mvstack.pop();

        model_view = mvstack.pop();

        model_view = mvstack.pop();
        model_view = mvstack.pop();
    }


}

void Horse::selectColor(int part) {
    if(activePart == part)
        shaders.setUniformValue("color",EXPANDVEC4(highlightColor));
    else
        shaders.setUniformValue("color",EXPANDVEC4(normalColor));
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
void Horse::keyPressEvent(QKeyEvent *event) {
    if(event->key() ==  Qt::Key_Tab)
        activePart = (activePart + 1)%(Whole+1);
    if(event->key() == Qt::Key_R)
        animate();
    update();
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

    mat4 instance = model_view * (Translate((HEAD_X/2),0.0,0)*Scale(HEAD_X,HEAD_Y,HEAD_Z));

    shaders.setUniformValue("ModelView",QMatrix4x4(EXPANDMAT44(instance)));
    glDrawArrays( GL_TRIANGLES, 0, NumNodes );

    model_view = mvstack.pop();
}

void Horse::upperTail() {
    mvstack.push( model_view );

    mat4 instance = model_view * (Translate(-(UPPERTAIL_X/2),-UPPERTAIL_Y/2,0)*Scale(UPPERTAIL_X,UPPERTAIL_Y,UPPERTAIL_Z));

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
void Horse::hoof() {
    mvstack.push( model_view );

    mat4 instance = model_view * (Translate(0.0,-HOOF_Y/2,0.0)*Scale(HOOF_X,HOOF_Y,HOOF_Z));

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

void Horse::animate() {
    qDebug() << "start";
    for(int i=2;i<=totalFrames;i++) {
        for(int j=0;j<=FRAMELENGTH/SLEEPTIME;j++) {
            for(int k=0;k<Whole;k++) {
                angles[0][k] = (float)j*(angles[i][k]-angles[i-1][k])/(FRAMELENGTH/SLEEPTIME) + angles[i-1][k];
            }
            qDebug() << angles[0][FrontLLowerLeg];
            QThread::msleep(SLEEPTIME);
            updateGL();
        }
    }
}

void Horse::loadAnglesFromFile(std::string fileName) {
    totalFrames++;
    activeFrame = totalFrames;
    activePart = TAIL_Y;
    std::ifstream file(fileName);
    for(int i=0;i<Whole; i++) {
        file >> angles[activeFrame][i];
    }
    file.close();
    loadFrame(activeFrame);
    qDebug() << "totalFrames " << totalFrames << "activeFrame" << activeFrame;
    update();
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
