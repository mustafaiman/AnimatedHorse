#include "horse.h"
Horse::Horse(QWidget *parent) :
    QGLWidget(parent)
{

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    if(!imageB.load(":/skin.bmp")) {
        qDebug() << "Image not found";
        imageB = QImage(512,512, QImage::Format_RGB32);
        imageB.fill(1);
    }
    imageT = QGLWidget::convertToGLFormat(imageB);
    normalColor = color4(0.0,0.0,1.0,1.0);
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

    globalRotateX = -10.0;
    globalRotateY = 45.0;

    Index = 0;

    cube_points[0] = point4( -0.5, -0.5, 0.5, 1.0 );
    cube_points[1] = point4( -0.5, 0.5, 0.5, 1.0 );
    cube_points[2] = point4( 0.5, 0.5, 0.5, 1.0 );
    cube_points[3] = point4( 0.5, -0.5, 0.5, 1.0 );
    cube_points[4] = point4( -0.5, -0.5, -0.5, 1.0 );
    cube_points[5] = point4( -0.5, 0.5, -0.5, 1.0 );
    cube_points[6] = point4( 0.5, 0.5, -0.5, 1.0 );
    cube_points[7] = point4( 0.5, -0.5, -0.5, 1.0 );


    light_position = point4( 0.0, 0.0, -1.0, 0.0 );
    light_ambient = color4( 1.0, 1.0, 1.0, 1.0 );
    light_diffuse = color4( 1.0, 1.0, 1.0, 1.0 );
    light_specular = color4( 1.0, 1.0, 1.0, 1.0 );


    horse_ambient = color4( 0.8, 0.8, 0.05, 1.0 );
    horse_diffuse = color4( 0.5, 0.2, 0.0, 1.0 );
    horse_specular = color4( 1.0, 0.8, 0.0, 1.0 );

    ground_ambient = color4( 0.5, 1.0, 0.3, 1.0 );
    ground_diffuse = color4( 0.0, 0.0, 0.0, 1.0 );
    ground_specular = color4( 0.0, 0.0, 0.0, 1.0 );

    horse_shininess = 100.0;
    ground_shininess = 100.0;

    activePart = 0;

    projection = Ortho(-2.0,2.0,-2.0,2.0,-4.0,4.0);
    colorcube();

    fillMode =  GL_FILL;

    clearColor = color4(0.8,1.0,1.0,1.0);

}
float Horse::getRelativeX(int x) {
    return (float)x / (width() / 2) - 1.0;
}

float Horse::getRelativeY(int y) {
    return (float)(height() - y) / (height() / 2) - 1.0;
}

void Horse::mousePressEvent(QMouseEvent *event) {
    setFocus();
    if(event->button() == Qt::LeftButton) {
        mouseDownX = getRelativeX(event->x());
        mouseDownY = getRelativeY(event->y());
    }
    updateGL();
}

void Horse::wheelEvent(QWheelEvent *event) {
    if(event->orientation() == Qt::Vertical) {
        if(event->delta() > 0) {
            nextJoint();
        } else if(event->delta() < 0) {
            previousJoint();
        }
    }
}

void Horse::selectWhole() {
    activePart = Whole;
    updateGL();
}

void Horse::nextJoint() {
    activePart = (activePart + 1)%Whole;
    updateGL();
}

void Horse::previousJoint() {
    activePart = (activePart + Whole -1)%Whole;
    updateGL();
}

void Horse::mouseMoveEvent(QMouseEvent *event) {
    float x = getRelativeX(event->x());
    float y = getRelativeY(event->y());

    if(activePart == Whole) {
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

    } else {
        angles[0][activePart] = angles[0][activePart] + (y-mouseDownY)/2*180;
        angles[0][activePart] = angles[0][activePart] + (x-mouseDownX)/2*180;

    }

    mouseDownX = x;
    mouseDownY = y;
    updateGL();
}

void Horse::initializeGL() {
    initializeGLFunctions();

    initShaders();

    glGenTextures(1,mytex);
    glBindTexture(GL_TEXTURE_2D, mytex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageT.width(), imageT.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imageT.bits());
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );


    glGenBuffers(1,&bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals) + sizeof(tex_coord), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals), sizeof(tex_coord), tex_coord);

    GLuint vPosition = shaders.attributeLocation("vPosition");
    shaders.enableAttributeArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vNormal = shaders.attributeLocation("vNormal");
    shaders.enableAttributeArray(vNormal);
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

    GLuint texLoc = shaders.attributeLocation("texcoord");
    shaders.enableAttributeArray(texLoc);
    glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)+sizeof(normals)));


    glEnable( GL_DEPTH_TEST  );
//    glShadeModel(GL_FLAT);

    shaders.setUniformValue("texMap",0);
    shaders.setUniformValue("Projection",QMatrix4x4(EXPANDMAT44(projection)));



    shaders.setUniformValue("LightPosition",EXPANDVEC4(light_position));
    shaders.setUniformValue("designMode",false);


}

void Horse::setShading(int a) {
    if(a==1) {
        shaders.setUniformValue("AmbientProduct",EXPANDVEC4((light_ambient * horse_ambient)));
        shaders.setUniformValue("DiffuseProduct",EXPANDVEC4((light_diffuse * horse_diffuse)));
        shaders.setUniformValue("SpecularProduct",EXPANDVEC4((light_specular * horse_specular)));
        shaders.setUniformValue("Shininess",horse_shininess);
    } else {
        shaders.setUniformValue("AmbientProduct",EXPANDVEC4((light_ambient * ground_ambient)));
        shaders.setUniformValue("DiffuseProduct",EXPANDVEC4((light_diffuse * ground_diffuse)));
        shaders.setUniformValue("SpecularProduct",EXPANDVEC4((light_specular * ground_specular)));
        shaders.setUniformValue("Shininess",ground_shininess);
    }

}

void Horse::changeFill(bool a) {
    if(a) {
        shaders.setUniformValue("designMode",true);
        fillMode = GL_LINE;
    } else {
        shaders.setUniformValue("designMode",false);
        fillMode = GL_FILL;
    }
    updateGL();
}

void Horse::loadFrame(int act) {
    for(int i=0;i<Whole;i++)
        angles[0][i] = angles[act][i];
}

void Horse::paintGL() {
    glPolygonMode( GL_FRONT_AND_BACK, fillMode );
    glClearColor(EXPANDVEC4(clearColor));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(activeFrame > 0) {


        constraintAngle(UpperTail,0,15);
        constraintAngle(TailY,-30.0,30.0);
        constraintAngle(TailZ,-10.0,90.0);
        constraintAngle(NeckZ,3.0,90.0);
        constraintAngle(NeckY,-50.0,50.0);
        constraintAngle(HeadZ,-110.0,0);
        constraintAngle(HeadY,-60.0,60.0);
        constraintAngle(HipRA,-75,10);
        constraintAngle(HipLA,-75,10);
        constraintAngle(HipRB,5,60);
        constraintAngle(HipLB,5,60);
        constraintAngle(RearRUpperLeg,-170,5);
        constraintAngle(RearRLowerLeg,5,170);
        constraintAngle(RearLUpperLeg,-170,5);
        constraintAngle(RearLLowerLeg,5,170);
        constraintAngle(ShoulderRA,-10,75);
        constraintAngle(ShoulderRB,-80,-5);
        constraintAngle(ShoulderLA,-10,75);
        constraintAngle(ShoulderLB,-80,-5);
        constraintAngle(FrontRUpperLeg,-5,170);
        constraintAngle(FrontRLowerLeg,-170,-5);
        constraintAngle(FrontLUpperLeg,-5,170);
        constraintAngle(FrontLLowerLeg,-170,-5);
        constraintAngle(HoofRR,-55,20);
        constraintAngle(HoofRL,-55,20);
        constraintAngle(HoofFR,-55,20);
        constraintAngle(HoofFL,-55,20);




        model_view = Translate(0.0,0.0,0.0)*RotateX(-globalRotateX)*RotateY(globalRotateY);
        mvstack.push(model_view);
        model_view *= Translate(0.0,-GROUND_Y/2-1.0,0.0);
        setShading(2);
        selectColor(1000);
        ground();

        model_view = mvstack.pop();

        mvstack.push(model_view);
        model_view *= Translate(0.0,0.0,0.0);
        selectColor(1000);
        setShading(1);
        trunk();
        model_view = mvstack.pop();

        mvstack.push(model_view);
        selectColor(UpperTail);
        model_view *= (Translate(-(TRUNK_X/5),TRUNK_Y/2,0.0)*RotateZ(angles[0][UpperTail]));
        upperTail();

        mvstack.push(model_view);
        selectColor(TailY,TailZ);
        model_view *= Translate(-(UPPERTAIL_X),0.0,0.0)*RotateZ(angles[0][TailZ])*RotateY(angles[0][TailY]);
        tail();

        model_view = mvstack.pop();
        model_view = mvstack.pop();

        mvstack.push(model_view);
        model_view *= (Translate(TRUNK_X/2-NECK_Y,TRUNK_Y/2,0.0)*RotateZ(angles[0][NeckZ])*RotateY(angles[0][NeckY]));
        selectColor(NeckY,NeckZ);
        neck();

        mvstack.push(model_view);
        model_view *= Translate(NECK_X,-NECK_Y/2,0.0)*RotateZ(angles[0][HeadZ])*RotateY(angles[0][HeadY]);
        selectColor(HeadY,HeadZ);
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
        model_view *= Translate(HOOF_X/2-RLLEG_X/2,-RLLEG_Y,0.0)*RotateZ(angles[0][HoofRR]);
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
        model_view *= Translate(HOOF_X/2-RLLEG_X/2,-RLLEG_Y,0.0)*RotateZ(angles[0][HoofRL]);
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

    setFocus();

}

void Horse::selectColor(int part1, int part2) {
    if(activePart == part1)
        selectColor(part1);
    else
        selectColor(part2);
}

void Horse::selectColor(int part) {
    if(activePart == part)
        shaders.setUniformValue("vColor",EXPANDVEC4(highlightColor));
    else
        shaders.setUniformValue("vColor",EXPANDVEC4(normalColor));
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


void Horse::ground() {
    mvstack.push( model_view );
    mat4 instance = model_view * (Scale(GROUND_X,GROUND_Y,GROUND_Z));

    shaders.setUniformValue("ModelView",QMatrix4x4(EXPANDMAT44(instance)));
    glDrawArrays( GL_TRIANGLES, 0, NumNodes );
    model_view = mvstack.pop();
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
    vec4 u = vertices[b] - vertices[a];
    vec4 v = vertices[c] - vertices[b];

    vec3 normal = normalize( cross(u, v) );


    tex_coord[Index] = point2(0.0,0.0); normals[Index] = normal; vertices[Index] = cube_points[a]; Index++;
    tex_coord[Index] = point2(0.0,1.0); normals[Index] = normal; vertices[Index] = cube_points[b]; Index++;
    tex_coord[Index] = point2(1.0,1.0); normals[Index] = normal; vertices[Index] = cube_points[c]; Index++;
    tex_coord[Index] = point2(0.0,0.0); normals[Index] = normal; vertices[Index] = cube_points[a]; Index++;
    tex_coord[Index] = point2(1.0,1.0); normals[Index] = normal; vertices[Index] = cube_points[c]; Index++;
    tex_coord[Index] = point2(1.0,0.0); normals[Index] = normal; vertices[Index] = cube_points[d]; Index++;
}

void Horse::animate() {
    update();
    for(int i=2;i<=totalFrames;i++) {
        for(int j=0;j<=frameInterval[i-1]/SLEEPTIME;j++) {
            for(int k=0;k<Whole;k++) {
                angles[0][k] = (float)j*(angles[i][k]-angles[i-1][k])/(frameInterval[i-1]/SLEEPTIME) + angles[i-1][k];
            }
            QThread::msleep(SLEEPTIME);
            updateGL();
        }
    }
}

void Horse::reloadActiveFrame() {
    for(int k=0;k<Whole;k++) {
        angles[0][k] = angles[activeFrame][k];
    }
}

void Horse::loadAnglesFromFile(std::string fileName) {
    activePart = 0;
    std::ifstream file(fileName);
    file >> totalFrames;
    for(int i=1;i<= totalFrames;i++) {
        activeFrame = i;
        for(int i=0;i<Whole; i++) {
            file >> angles[activeFrame][i];
        }
        file >> frameInterval[i];
    }
    file.close();
    loadFrame(activeFrame);
    update();
}

std::string Horse::dump() {
    std::ostringstream str;
    str << totalFrames << "\n";
    for(int i=1;i<= totalFrames;i++) {
        for(int j=0;j<Whole; j++) {
            str << angles[i][j] << "\n";
        }
        str << frameInterval[i] << "\n";
    }
    return str.str();
}


void Horse::recordActiveScene(int interval) {
    frameInterval[activeFrame] = interval;
    totalFrames++;
    activeFrame = totalFrames;
    for(int i=0;i<Whole;i++) {
        angles[activeFrame][i] = angles[0][i];
    }
    emit frameChanged(QString::number(totalFrames));
    frameInterval[activeFrame] = 0;
}

void Horse::editActiveScene() {
    for(int i=0;i<Whole;i++) {
        angles[activeFrame][i] = angles[0][i];
    }
}

void Horse::changeActiveFrame(int fNumber) {
    if(fNumber > totalFrames)
        fNumber = totalFrames;
    activeFrame = fNumber;
    reloadActiveFrame();
    updateGL();
    update();
}

void Horse::constraintAngle(int ang, double low, double high) {
    if(angles[0][ang] < low)
        angles[0][ang] = low;
    if(angles[0][ang] > high)
        angles[0][ang] = high;
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
