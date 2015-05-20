#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <QDateTime>
#include <math.h>
#include "GLUU.h"
#include "SFile.h"
#include "ReadFile.h"
#include "FileBuffer.h"
#include "Route.h"
#include "GLMatrix.h"
#include "Eng.h"
#include "Tile.h"
#include "Game.h"
#include "GLH.h"
#include "Vector2f.h"
#include "TerrainLib.h"

GLWidget::GLWidget(QWidget *parent)
: QOpenGLWidget(parent),
m_xRot(0),
m_yRot(0),
m_zRot(0) {

}

GLWidget::~GLWidget() {
    cleanup();
}

QSize GLWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const {
    return QSize(1000, 700);
}

void GLWidget::cleanup() {
    makeCurrent();
    //delete gluu->m_program;
    //gluu->m_program = 0;
    doneCurrent();
}

void GLWidget::timerEvent(QTimerEvent * event) {

    timeNow = QDateTime::currentMSecsSinceEpoch();
    if (timeNow - lastTime < 1)
        fps = 1;
    else
        fps = 1000.0 / (timeNow - lastTime);
    if (fps < 10) fps = 10;
    lastTime = timeNow;


    if (Game::allowObjLag < Game::maxObjLag)
        Game::allowObjLag += 2;

    camera->update(fps);
    update();
}

void GLWidget::initializeGL() {
    
    gluu = GLUU::get();
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);

    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    gluu->initShader();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);


    //sFile = new SFile("F:/TrainSim/trains/trainset/pkp_sp47/pkp_sp47-001.s", "F:/TrainSim/trains/trainset/pkp_sp47");
    //sFile = new SFile("f:/train simulator/routes/cmk/shapes/cottage3.s", "cottage3.s", "f:/train simulator/routes/cmk/textures");
    //eng = new Eng("F:/Train Simulator/trains/trainset/PKP-ST44-992/","PKP-ST44-992.eng",0);
    //sFile->Load("f:/train simulator/routes/cmk/shapes/cottage3.s");
    //tile = new Tile(-5303,-14963);
    route = new Route();
    if(!route->loaded) return;
    
    float * aaa = new float[2]{0,0};
    camera = new Camera(aaa);
    float spos[3];
    if(Game::start == 2){
        camera->setPozT(Game::startTileX, -Game::startTileY);
    } else {
        camera->setPozT(route->startTileX, -route->startTileY);
        spos[0] = route->startpX;
        spos[2] = -route->startpZ;
    }
    if(TerrainLib::load(route->startTileX, -route->startTileY)){
        spos[1] = 20 + TerrainLib::getHeight(route->startTileX, -route->startTileY, route->startpX, -route->startpZ);
    } else {
        spos[1] = 0;
    }
    camera->setPos((float*)&spos);
    
    
    lastTime = QDateTime::currentMSecsSinceEpoch();
    timer.start(15, this);
    setFocus();
    setMouseTracking(true);
    pointer3d = new Pointer3d();
    selectedObj = NULL;
    
    emit routeLoaded(route);
}

void GLWidget::paintGL() {
    if(!route->loaded) return;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //if (!selection)
    glClearColor(gluu->skyc[0], gluu->skyc[1], gluu->skyc[2], 1.0);
    //else
    //    glClearColor(0, 0, 0, 1.0);

    
    
    Mat4::perspective(gluu->pMatrix, 45.0f, float(this->width()) / this->height(), 0.2f, 4000.0);
    float* lookAt = camera->getMatrix();
    Mat4::multiply(gluu->pMatrix, gluu->pMatrix, lookAt);
    Mat4::identity(gluu->mvMatrix);

    //mat4.translate(gluu.mvMatrix, gluu.mvMatrix, [0.0, -2.0, -15.0]);
    //mat4.rotate(gluu.mvMatrix, gluu.mvMatrix, gluu.degToRad(yRot), [0,1,0]);
    //mat4.translate(gluu.mvMatrix, gluu.mvMatrix, [0.0, 0.0, -30.0]);
    //mat4.rotate(gluu.mvMatrix, gluu.mvMatrix, gluu.degToRad(yRot), [0,1,0]);
    //Mat4::identity(gluu->objStrMatrix);
    //gluu->objStrMatrix = Mat4::create();
    //gluu->m_proj.setToIdentity();
    //gluu->m_proj.perspective(45.0f, GLfloat(this->width()) / this->height(), 0.1f, 6000.0f);

    //gluu->m_world.setToIdentity();
    //gluu->m_world.translate(0.0, 0.0, -20.0);
    //gluu->m_world.rotate(m_yRot / 16.0f, 0, 1, 0);
    //gluu->m_world.rotate(m_zRot / 16.0f, 0, 0, 1);

    //gluu->m_program->bind();
    //gluu->m_program->setUniformValue(gluu->m_projMatrixLoc, gluu->m_proj);
    //gluu->m_program->setUniformValue(gluu->m_mvMatrixLoc, gluu->m_camera * gluu->m_world);
    //QMatrix3x3 normalMatrix = gluu->m_world.normalMatrix();
    //gluu->m_program->setUniformValue(gluu->m_normalMatrixLoc, normalMatrix);

    //Mat4::identity(gluu->pMatrix);
    //Mat4::identity(gluu->mvMatrix);
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0.0, 0.0, -20.0);
    Mat4::identity(gluu->objStrMatrix);
    
    gluu->m_program->bind();
    gluu->setMatrixUniforms();
    //sFile->render();
    //eng->render();
    //tile->render();
    route->render(gluu, camera->pozT, camera->getPos(), camera->getTarget(), camera->getRotX(), 3.14f / 3, selection);
    
    bool showPos = true;
    if (showPos && !selection) {
        int x = mousex;
        int y = mousey;

        float winZ[4];
        int viewport[4];
        //float wcoord[4];

        glGetIntegerv(GL_VIEWPORT, viewport);
        //glGetFloatv(GL_MODELVIEW_MATRIX, mvmatrix);
        //glGetFloatv(GL_PROJECTION_MATRIX, projmatrix);
        int realy = viewport[3] - (int) y - 1;
        glReadPixels(x, realy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

        GLH::glhUnProjectf((float) x, (float) realy, winZ[0], // 
                gluu->mvMatrix,
                gluu->pMatrix,
                viewport,
                aktPointerPos);
        
        //if(selection)
        //    qDebug() << wcoord[0] << ", " << wcoord[1] << ", " << wcoord[2];
        //showPos = false;
        
        //aktPointerPos[0] = 
        
        gluu->mvPushMatrix();
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, aktPointerPos[0], aktPointerPos[1], aktPointerPos[2]);
        Mat4::identity(gluu->objStrMatrix);
        gluu->setMatrixUniforms();
        //gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        pointer3d->render();
        gluu->mvPopMatrix();
        /*gl.glDisable(GL2.GL_TEXTURE_2D);
        gl.glDisable(GL2.GL_LIGHTING);
        gl.glColor3f(1.0f, 1.0f, 0f);
        gl.glLineWidth(3);
        gl.glBegin(GL2.GL_LINES);
        gl.glVertex3f(wcoord[0], wcoord[1], wcoord[2]);
        gl.glVertex3f(wcoord[0], wcoord[1] + 2, wcoord[2]);
        gl.glVertex3f(wcoord[0] - 2, wcoord[1], wcoord[2]);
        gl.glVertex3f(wcoord[0] + 2, wcoord[1], wcoord[2]);
        gl.glVertex3f(wcoord[0], wcoord[1], wcoord[2] - 2);
        gl.glVertex3f(wcoord[0], wcoord[1], wcoord[2] + 2);
        gl.glEnd();
        gl.glLineWidth(1);
        gl.glEnable(GL2.GL_LIGHTING);*/

        ////////////
    }

    if (selection) {
        int x = mousex;
        int y = mousey;

        float winZ[4];

        int* viewport = new int[4];
        float* mvmatrix = new float[16];
        float* projmatrix = new float[16];
        float* wcoord = new float[4];

        glGetIntegerv(GL_VIEWPORT, viewport);
        glGetFloatv(GL_MODELVIEW_MATRIX, mvmatrix);
        glGetFloatv(GL_PROJECTION_MATRIX, projmatrix);
        int realy = viewport[3] - (int) y - 1;
        glReadPixels(x, realy, 1, 1, GL_RGBA, GL_FLOAT, &winZ);

        qDebug() << winZ[0] << " " << winZ[1] << " " << winZ[2] << " ";
        int ww = (int) (winZ[0]*255);
        int wx = (int) (ww / 10);
        int wz = (int) (ww - (wx)*10);
        wx = camera->pozT[0] + wx - 5;
        wz = camera->pozT[1] - (wz - 5);

        int UiD = (int) (winZ[1]*255)*256 + (int) (winZ[2]*255);

        qDebug() << wx << " " << wz << " " << UiD;
        if(selectedObj != NULL) selectedObj->unselect();
        selectedObj = route->getObj(wx, wz, UiD);
        if (!selectedObj->loaded){
            qDebug() << "brak obiektu";
            selectedObj = NULL;
        } else {
            selectedObj->select();
            //selectedObj->translate(0,10,0);
        }
        //int objHash = trasa.getObjectHash(wx,wz,UiD);
        //System.out.println( objHash + " ");

        //selectedObj = trasa.getObj(wx,wz,objHash);

        //selectedObj.select();
        //System.out.println(selectedObj.selected); 
        selection = !selection;
        paintGL();
    }

    emit this->naviInfo((int)camera->pozT[0], -(int)camera->pozT[1], route->getTileObjCount((int)camera->pozT[0], (int)camera->pozT[1]), route->getTileHiddenObjCount((int)camera->pozT[0], (int)camera->pozT[1]));
    
    gluu->m_program->release();

}

void GLWidget::resizeGL(int w, int h) {
    //gluu->m_proj.setToIdentity();
    //gluu->m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void GLWidget::keyPressEvent(QKeyEvent * event) {
    if(!route->loaded) return;
    camera->keyDown(event);
    switch (event->key()) {
        case 'M':
            route->save();
            //selection = true; //!selection;
            // paintGL();
            // selection = !selection;
            break;
        case Qt::Key_0:
            route->createNewPaths();
            break;
        //case Qt::Key_F:
            /*if(this->selectedObj != NULL){
                this->selectedObj->unselect();
                this->selectedObj = NULL;
            }
            this->selectedObj = route->makeFlexTrack((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
            if(this->selectedObj != NULL){
                this->selectedObj->select();
                lastNewObjPosT[0] = this->selectedObj->x;
                lastNewObjPosT[1] = this->selectedObj->y;
                lastNewObjPos[0] = this->selectedObj->position[0];
                lastNewObjPos[1] = this->selectedObj->position[1];
                lastNewObjPos[2] = this->selectedObj->position[2];
            }*/
        //    break;
        case Qt::Key_B:
            route->newTile((int)camera->pozT[0], (int)camera->pozT[1]);
        default:
            break;
    }
    if(toolEnabled == "selectTool" || toolEnabled == "placeTool"){
        Vector2f a;

        switch (event->key()) {
            case Qt::Key_Alt:
                moveStep = moveMinStep;
                break;
            case Qt::Key_8:
                if(translateTool && selectedObj != NULL){
                    a.y = moveStep;
                    a.rotate(-camera->getRotX(), 0);
                    this->selectedObj->translate(a.x,0,a.y);
                }
                if(resizeTool && selectedObj != NULL){
                    this->selectedObj->resize(moveStep, 0, 0);
                }
                if(rotateTool && selectedObj != NULL){
                    this->selectedObj->rotate(moveStep/10, 0, 0);
                }
                break;
            case Qt::Key_2:
                if(translateTool && selectedObj != NULL){
                    a.y = -moveStep;
                    a.rotate(-camera->getRotX(), 0);
                    this->selectedObj->translate(a.x,0,a.y);
                }
                if(resizeTool && selectedObj != NULL){
                    this->selectedObj->resize(-moveStep, 0, 0);
                }
                if(rotateTool && selectedObj != NULL){
                    this->selectedObj->rotate(-moveStep/10, 0, 0);
                }
                break;    
            case Qt::Key_4:
                if(translateTool && selectedObj != NULL){
                    a.x = moveStep;
                    a.rotate(-camera->getRotX(), 0);
                    this->selectedObj->translate(a.x,0,a.y);
                }
                if(resizeTool && selectedObj != NULL){
                    this->selectedObj->resize(0, moveStep, 0);
                }
                if(rotateTool && selectedObj != NULL){
                    this->selectedObj->rotate(0, -moveStep/10, 0);
                }
                break;
            case Qt::Key_6:
                if(translateTool && selectedObj != NULL){
                    a.x = -moveStep;
                    a.rotate(-camera->getRotX(), 0);
                    this->selectedObj->translate(a.x,0,a.y);
                }
                if(resizeTool && selectedObj != NULL){
                    this->selectedObj->resize(0, -moveStep, 0);
                }
                if(rotateTool && selectedObj != NULL){
                    this->selectedObj->rotate(0, moveStep/10, 0);
                }
                break;                 
            case Qt::Key_7:
                if(resizeTool && selectedObj != NULL){
                    this->selectedObj->resize(0, 0, -moveStep);
                }
                break;                
            case Qt::Key_9:
                if(translateTool && selectedObj != NULL){
                    this->selectedObj->translate(0,moveStep,0);
                }
                if(resizeTool && selectedObj != NULL){
                    this->selectedObj->resize(0, 0, moveStep);
                }
                if(rotateTool && selectedObj != NULL){
                    this->selectedObj->rotate(0, 0, moveStep/10);
                }
                break;
            case Qt::Key_3:
                if(translateTool && selectedObj != NULL){
                    this->selectedObj->translate(0,-moveStep,0);
                }
                if(rotateTool && selectedObj != NULL){
                    this->selectedObj->rotate(0, 0, -moveStep/10);
                }
                break;   
            case Qt::Key_R:
                resizeTool = false;
                translateTool = false;
                rotateTool = true;
                break;
            case Qt::Key_T:    
                resizeTool = false;
                rotateTool = false;
                translateTool = true;
                break;
            case Qt::Key_Y:    
                rotateTool = false;
                translateTool = false;
                resizeTool = true;
                break;
            case Qt::Key_F:
                route->setTerrainToTrackObj(selectedObj);
                break;
            case Qt::Key_Delete:
                if(selectedObj != NULL){
                    route->deleteObj(selectedObj);
                    selectedObj->unselect();
                    selectedObj = NULL;
                }
                break;                
            case Qt::Key_C:
                if(selectedObj != NULL){
                    selectedObj->unselect();
                    selectedObj = route->placeObject(selectedObj->x, selectedObj->y, selectedObj->position, selectedObj->qDirection, selectedObj->getRefInfo());
                    if(selectedObj != NULL){
                        selectedObj->select();
                    }
                }
            case Qt::Key_P:
                if(selectedObj != NULL){
                    route->ref->selected = selectedObj->getRefInfo();
                    emit itemSelected((int) route->ref->selected);
                }
            case Qt::Key_L:
                route->nextDefaultEnd();
                break;
            case Qt::Key_Z:
                //route->refreshObj(selectedObj);
                //route->trackDB->setDefaultEnd(0);
                //route->addToTDB(selectedObj, (float*)&lastNewObjPosT, (float*)&selectedObj->position);
                route->addToTDB(selectedObj, (float*)&lastNewObjPosT, (float*)&lastNewObjPos);
                if(selectedObj != NULL) selectedObj->unselect();
                selectedObj = NULL;
                break;
            case Qt::Key_X:
                //route->refreshObj(selectedObj);
                route->nextDefaultEnd();
                route->newPositionTDB(selectedObj, (float*)&lastNewObjPosT, (float*)&lastNewObjPos);                
                break;
            default:
                break;
        }
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent * event) {
    if(!route->loaded) return;
    camera->keyUp(event);
    if(toolEnabled == "selectTool" || toolEnabled == "placeTool"){
        switch (event->key()) {
            case Qt::Key_Alt:
                moveStep = moveMaxStep;
                break;
            default:
                break;
        }
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
    if(!route->loaded) return;
    m_lastPos = event->pos();
    mousePressed = true;
    
    if(event->buttons() == Qt::RightButton){
        camera->MouseDown(event);
    } else {
        if(toolEnabled == "placeTool"){
            if(selectedObj != NULL)
                selectedObj->unselect();
            lastNewObjPosT[0] = camera->pozT[0];
            lastNewObjPosT[1] = camera->pozT[1];
            lastNewObjPos[0] = aktPointerPos[0];
            lastNewObjPos[1] = aktPointerPos[1];
            lastNewObjPos[2] = aktPointerPos[2];
            selectedObj = route->placeObject((int)camera->pozT[0], (int)camera->pozT[1], aktPointerPos);
            if(selectedObj != NULL)
                selectedObj->select();
            //if(route->ref->selected != NULL){
            //qDebug() << route->ref->selected->description;
            //}
            //camera->MouseDown(event);
        }
        if(toolEnabled == "selectTool"){
            selection = true;
            mousePressed = false;
            if(selectedObj != NULL){
                mousePressed = true;
                lastPointerPos[0] = aktPointerPos[0];
                lastPointerPos[1] = aktPointerPos[1];
                lastPointerPos[2] = aktPointerPos[2];
            } //else {
                //camera->MouseDown(event);
            //}
        }
        if(toolEnabled == ""){
            camera->MouseDown(event);
        }
    }
    setFocus();
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event) {
    if(!route->loaded) return;
    camera->MouseUp(event);
    mousePressed = false;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    if(!route->loaded) return;
    /*int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {

    } else if (event->buttons() & Qt::RightButton) {

    }*/
    mousex = event->x();
    mousey = event->y();
    m_lastPos = event->pos();
    
    if(event->buttons() == Qt::RightButton){
        camera->MouseMove(event);
    } else {
        if(toolEnabled == "selectTool"){
            /*if(selectedObj != NULL && mousePressed){
                //selectedObj->translate(aktPointerPos[0] - lastPointerPos[0], aktPointerPos[1] - lastPointerPos[1], aktPointerPos[2] - lastPointerPos[2]);
                selectedObj->translate(0, aktPointerPos[1] - lastPointerPos[1], 0);
                lastPointerPos[0] = aktPointerPos[0];
                lastPointerPos[1] = aktPointerPos[1];
                lastPointerPos[2] = aktPointerPos[2];
            }*/ //else {
            //    camera->MouseMove(event);
            //}
        }
        if(toolEnabled == "placeTool"){
            camera->MouseMove(event);
        }
        if(toolEnabled == ""){
            camera->MouseMove(event);
        }
    }
}

void GLWidget::msg(QString text){
    qDebug() << text;
    if(text == "save"){
        route->save();
        return;
    }
    if(text == "createPaths"){
        route->createNewPaths();
        return;
    }
}

void GLWidget::enableTool(QString name){
    qDebug() << name;
    toolEnabled = name;
}

void GLWidget::jumpTo(int x, int y){
    qDebug() << "jump: "<< x << " "<< y;
    camera->setPozT(x, -y);
    
}