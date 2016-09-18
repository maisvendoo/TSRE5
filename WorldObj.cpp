/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "WorldObj.h"
#include "SFile.h"
#include "ShapeLib.h"
#include "ParserX.h"
#include "GLMatrix.h"
#include <math.h>
#include <QString>
#include "StaticObj.h"
#include "DynTrackObj.h"
#include "ForestObj.h"
#include "TransferObj.h"
#include "TrackObj.h"
#include "SpeedpostObj.h"
#include "SignalObj.h"
#include "PlatformObj.h"
#include "TrWatermarkObj.h"
#include "LevelCrObj.h"
#include "PickupObj.h"
#include "HazardObj.h"
#include "SoundRegionObj.h"
#include "SoundSourceObj.h"
#include "Game.h"
#include "TS.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int WorldObj::isTrackObj(QString sh) {
    if (sh == "signal") {
        return 1;
    } else if (sh == "speedpost") {
        return 1;
    } else if (sh == "collideobject") {
        return 0;
    } else if (sh == "platform") {
        return 1;
    } else if (sh == "siding") {
        return 1;
    } else if (sh == "carspawner") {
        return 2;
    } else if (sh == "levelcr") {
        return 3;
    } else if (sh == "pickup") {
        return 1;
    } else if (sh == "hazard") {
        return 1;
    } else if (sh == "soundregion") {
        return 1;
    } 
    return 0;
}


WorldObj* WorldObj::createObj(int sh) {
    WorldObj* nowy;
    if (sh == TS::Static) {
        nowy = (WorldObj*) (new StaticObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
        (nowy)->typeID = (nowy)->sstatic;
    } else if(sh == TS::Signal) {
        nowy = (WorldObj*) (new SignalObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
        (nowy)->typeID = (nowy)->signal;
    } else if (sh == TS::Speedpost) {
        nowy = (WorldObj*) (new SpeedpostObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
        (nowy)->typeID = (nowy)->speedpost;
    } else if (sh == TS::TrackObj) {
        nowy = (WorldObj*) (new TrackObj());
        (nowy)->resPath = Game::root + "/global/shapes";
        (nowy)->typeID = (nowy)->trackobj;
    } else if (sh == TS::Gantry || sh == TS::Gantry2) {
        nowy = (WorldObj*) (new StaticObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
        (nowy)->typeID = (nowy)->gantry;
    } else if (sh == TS::CollideObject) {
        nowy = (WorldObj*) (new StaticObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";    
        (nowy)->typeID = (nowy)->collideobject;
    } else if (sh == TS::Dyntrack || sh == TS::DynTrack2) {
        nowy = (WorldObj*) (new DynTrackObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/textures";
        (nowy)->typeID = (nowy)->dyntrack;
    } else if (sh == TS::Forest) {
        nowy = (WorldObj*) (new ForestObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/textures";
        (nowy)->typeID = (nowy)->forest;
    } else if (sh == TS::Transfer || sh == TS::Transfer2) {
        nowy = (WorldObj*) (new TransferObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/textures";
        (nowy)->typeID = (nowy)->transfer;
    } else if (sh == TS::Platform) {
        nowy = (WorldObj*) (new PlatformObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
        (nowy)->typeID = (nowy)->platform;
    } else if (sh == TS::Siding || sh == TS::Siding2) {
        nowy = (WorldObj*) (new PlatformObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";       
        (nowy)->typeID = (nowy)->siding;
    } else if (sh == TS::CarSpawner || sh == TS::CarSpawner2) {
        nowy = (WorldObj*) (new PlatformObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";     
        (nowy)->typeID = (nowy)->carspawner;
    } else if (sh == TS::LevelCr) {
        nowy = (WorldObj*) (new LevelCrObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";   
        (nowy)->typeID = (nowy)->levelcr;
    } else if (sh == TS::Pickup || sh == TS::Pickup2) {
        nowy = (WorldObj*) (new PickupObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";       
        (nowy)->typeID = (nowy)->pickup;
    } else if (sh == TS::Hazard) {
        nowy = (WorldObj*) (new HazardObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";    
        (nowy)->typeID = (nowy)->hazard;
    } /*else if (sh == TS::So) {
        nowy = (WorldObj*) (new HazardObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";    
        (nowy)->typeID = (nowy)->hazard;
    } */else {
        qDebug() << " Unsupported WorldObj !!! " << sh;
        //(*nowy) = new WorldObj();
        return NULL;
        //
    }
    (nowy)->type = TS::IdName[sh];
    return nowy;
}

WorldObj* WorldObj::createObj(QString sh) {
    WorldObj* nowy;
    if (sh == "static") {
        nowy = (WorldObj*) (new StaticObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
        (nowy)->typeID = (nowy)->sstatic;
    } else if (sh == "signal") {
        nowy = (WorldObj*) (new SignalObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
        (nowy)->typeID = (nowy)->signal;
    } else if (sh == "speedpost") {
        nowy = (WorldObj*) (new SpeedpostObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
        (nowy)->typeID = (nowy)->speedpost;
    } else if (sh == "trackobj") {
        nowy = (WorldObj*) (new TrackObj());
        (nowy)->resPath = Game::root + "/global/shapes";
        (nowy)->typeID = (nowy)->trackobj;
    } else if (sh == "gantry") {
        nowy = (WorldObj*) (new StaticObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
        (nowy)->typeID = (nowy)->gantry;
    } else if (sh == "collideobject") {
        nowy = (WorldObj*) (new StaticObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";    
        (nowy)->typeID = (nowy)->collideobject;
    } else if (sh == "dyntrack") {
        nowy = (WorldObj*) (new DynTrackObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/textures";
        (nowy)->typeID = (nowy)->dyntrack;
    } else if (sh == "forest") {
        nowy = (WorldObj*) (new ForestObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/textures";
        (nowy)->typeID = (nowy)->forest;
    } else if (sh == "transfer") {
        nowy = (WorldObj*) (new TransferObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/textures";
        (nowy)->typeID = (nowy)->transfer;
    } else if (sh == "platform") {
        nowy = (WorldObj*) (new PlatformObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
        (nowy)->typeID = (nowy)->platform;
    } else if (sh == "siding") {
        nowy = (WorldObj*) (new PlatformObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";       
        (nowy)->typeID = (nowy)->siding;
    } else if (sh == "carspawner") {
        nowy = (WorldObj*) (new PlatformObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";     
        (nowy)->typeID = (nowy)->carspawner;
    } else if (sh == "levelcr") {
        nowy = (WorldObj*) (new LevelCrObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";   
        (nowy)->typeID = (nowy)->levelcr;
    } else if (sh == "pickup") {
        nowy = (WorldObj*) (new PickupObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";       
        (nowy)->typeID = (nowy)->pickup;
    } else if (sh == "hazard") {
        nowy = (WorldObj*) (new HazardObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";    
        (nowy)->typeID = (nowy)->hazard;
    } else if (sh == "soundsource") {
        nowy = (WorldObj*) (new SoundSourceObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";    
        (nowy)->typeID = (nowy)->soundsource;
    } else if (sh == "soundregion") {
        nowy = (WorldObj*) (new SoundRegionObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";    
        (nowy)->typeID = (nowy)->soundregion;
    } else {
        qDebug() << " Unsupported WorldObj !!! " + sh;
        //(*nowy) = new WorldObj();
        return NULL;
        //
    }
    (nowy)->type = sh;
    return nowy;
}

QString WorldObj::getResPath(Ref::RefItem* sh) {
    if (sh->type == "static") {
        return Game::root + "/routes/" + Game::route + "/shapes/"+sh->filename;
    } else if (sh->type == "signal") {
        return Game::root + "/routes/" + Game::route + "/shapes/"+sh->filename;
    } else if (sh->type == "speedpost") {
        return Game::root + "/routes/" + Game::route + "/shapes/"+sh->filename;
    } else if (sh->type == "trackobj") {
        return Game::root + "/global/shapes/"+sh->filename;
    } else if (sh->type == "gantry") {
        return Game::root + "/routes/" + Game::route + "/shapes/"+sh->filename;
    } else if (sh->type == "collideobject") {
        return Game::root + "/routes/" + Game::route + "/shapes/"+sh->filename;
    } else {
        return "";
    }
    return "";
}

WorldObj::WorldObj() {
    this->shape = -1;
    this->loaded = false;
    this->selected = false;
    this->modified = false;
    this->tRotation[0] = 0;
    this->tRotation[1] = 0;
}

WorldObj::WorldObj(const WorldObj& orig) {
}

WorldObj::~WorldObj() {
}

bool WorldObj::allowNew(){
    return false;
}

bool WorldObj::isTrackItem(){
    return false;
}

bool WorldObj::isSoundItem(){
    return false;
}

void WorldObj::load(int x, int y) {
    this->x = x;
    this->y = y;
    this->loaded = false;
    this->selected = false;
}

void WorldObj::set(QString sh, int val) {
    if (sh == ("x")) {
        this->x = val;
        return;
    }
    if (sh == ("z")) {
        this->y = val;
        return;
    }
}

void WorldObj::set(QString sh, float* val) {

}

void WorldObj::set(QString sh, QString val) {

}

void WorldObj::set(int sh, FileBuffer* data) {
    if (sh == TS::UiD) {
        data->off++;
        UiD = data->getUint();
        return;
    }
    if (sh == TS::StaticFlags) {
        data->off++;
        staticFlags = data->getUint();
        return;
    }
    if (sh == TS::Position) {
        data->off++;
        position[0] = data->getFloat();
        position[1] = data->getFloat();
        position[2] = data->getFloat();
        jestPQ++;
        return;
    }
    if (sh == TS::QDirection) {
        data->off++;
        qDirection[0] = data->getFloat();
        qDirection[1] = data->getFloat();
        qDirection[2] = data->getFloat();
        qDirection[3] = data->getFloat();
        if(fabs(qDirection[0]) + fabs(qDirection[1]) + fabs(qDirection[2]) + fabs(qDirection[3]) < 3)
            jestPQ++;
        return;
    }
    if (sh == TS::Matrix3x3) {
        data->off++;
        matrix3x3 = new float[9];
        matrix3x3[0] = data->getFloat();
        matrix3x3[1] = data->getFloat();
        matrix3x3[2] = data->getFloat();
        matrix3x3[3] = data->getFloat();
        matrix3x3[4] = data->getFloat();
        matrix3x3[5] = data->getFloat();
        matrix3x3[6] = data->getFloat();
        matrix3x3[7] = data->getFloat();
        matrix3x3[8] = data->getFloat();
        Quat::fromMat3((float*)&qDirection, matrix3x3);
        Vec4::normalize((float*)&qDirection,(float*)&qDirection);
        //Vec4::normalize((float*)&qDirection,(float*)&qDirection);
        jestPQ++;
        return;
    }
    if (sh == TS::VDbId) {
        data->off++;
        vDbId = data->getUint();
        return;
    }
    if (sh == TS::StaticDetailLevel) {
        data->off++;
        staticDetailLevel = data->getUint();
        return;
    }
    if (sh == TS::CollideFlags) {
        data->off++;
        collideFlags = data->getUint();
        return;
    }
    qDebug() << "worldObj "<<this->type<<" unknown: " << sh;
    return;
}

void WorldObj::set(QString sh, FileBuffer* data) {
    if (sh == ("uid")) {
        UiD = ParserX::GetUInt(data);
        return;
    }
    if (sh == ("staticflags")) {
        staticFlags = ParserX::GetHex(data);
        return;
    }
    if (sh == ("position")) {
        position[0] = ParserX::GetNumber(data);
        position[1] = ParserX::GetNumber(data);
        position[2] = ParserX::GetNumber(data);
        jestPQ++;
        return;
    }
    if (sh == ("qdirection")) {
        qDirection[0] = ParserX::GetNumber(data);
        qDirection[1] = ParserX::GetNumber(data);
        qDirection[2] = ParserX::GetNumber(data);
        qDirection[3] = ParserX::GetNumber(data);
        if(fabs(qDirection[0]) + fabs(qDirection[1]) + fabs(qDirection[2]) + fabs(qDirection[3]) < 3)
            jestPQ++;
        return;
    }
    if (sh == ("matrix3x3")) {
        matrix3x3 = new float[9];
        matrix3x3[0] = ParserX::GetNumber(data);
        matrix3x3[1] = ParserX::GetNumber(data);
        matrix3x3[2] = ParserX::GetNumber(data);
        matrix3x3[3] = ParserX::GetNumber(data);
        matrix3x3[4] = ParserX::GetNumber(data);
        matrix3x3[5] = ParserX::GetNumber(data);
        matrix3x3[6] = ParserX::GetNumber(data);
        matrix3x3[7] = ParserX::GetNumber(data);
        matrix3x3[8] = ParserX::GetNumber(data);
        Quat::fromMat3((float*)&qDirection, matrix3x3);
        Vec4::normalize((float*)&qDirection,(float*)&qDirection);
        //Vec4::normalize((float*)&qDirection,(float*)&qDirection);
        jestPQ++;
        return;
    }
    if (sh == ("vdbid")) {
        //qDebug()<< ParserX::GetNumber(data);
        vDbId = ParserX::GetUInt(data);
        return;
    }
    if (sh == ("staticdetaillevel")) {
        staticDetailLevel = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("collideflags")) {
        collideFlags = ParserX::GetNumber(data);
        return;
    }
    qDebug() << "worldObj "<<this->type<<" unknown: " << sh;
    return;
}

void WorldObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor) {
}

void WorldObj::deleteVBO(){
    
}

void WorldObj::setMartix(){
    Mat4::fromRotationTranslation(this->matrix, qDirection, position);
    Mat4::rotate(this->matrix, this->matrix, M_PI, 0, -1, 0);
}

void WorldObj::translate(float px, float py, float pz){
    this->position[0]+=px;
    this->position[1]+=py;
    this->position[2]+=pz;
    this->placedAtPosition[0] = this->position[0];
    this->placedAtPosition[1] = this->position[1];
    this->placedAtPosition[2] = this->position[2];
    this->modified = true;
    setMartix();
}

void WorldObj::rotate(float x, float y, float z){
    this->tRotation[0] += x;
    this->tRotation[1] += y;
    if(matrix3x3 != NULL) matrix3x3 = NULL;
    if(x!=0) Quat::rotateX(this->qDirection, this->qDirection, x);
    if(y!=0) Quat::rotateY(this->qDirection, this->qDirection, y);
    if(z!=0) Quat::rotateZ(this->qDirection, this->qDirection, z);
    this->modified = true;
    setMartix();
}

void WorldObj::resize(float x, float y, float z){
    
}

void WorldObj::setPosition(int x, int z, float* p){
    this->position[0] = -2048*(this->x-x) + p[0];
    this->position[1] = p[1];
    this->position[2] = -2048*(this->y-z) + p[2];
    this->placedAtPosition[0] = this->position[0];
    this->placedAtPosition[1] = this->position[1];
    this->placedAtPosition[2] = this->position[2];
    deleteVBO();
}

void WorldObj::setPosition(float* p){
    this->position[0] = p[0];
    this->position[1] = p[1];
    this->position[2] = p[2];
    this->placedAtPosition[0] = this->position[0];
    this->placedAtPosition[1] = this->position[1];
    this->placedAtPosition[2] = this->position[2];
    deleteVBO();
}

void WorldObj::setNewQdirection(){
    this->qDirection[0] = 0;
    this->qDirection[1] = 0;
    this->qDirection[2] = 0;
    this->qDirection[3] = 1;
}

void WorldObj::setQdirection(float* q){
    this->qDirection[0] = q[0];
    this->qDirection[1] = q[1];
    this->qDirection[2] = q[2];
    this->qDirection[3] = q[3];
}

void WorldObj::initPQ(float* p, float* q){
    this->position[0] = p[0];
    this->position[1] = p[1];
    this->position[2] = -p[2];
    this->placedAtPosition[0] = this->position[0];
    this->placedAtPosition[1] = this->position[1];
    this->placedAtPosition[2] = this->position[2];
    this->qDirection[0] = q[0];
    this->qDirection[1] = q[1];
    this->qDirection[2] = -q[2];
    this->qDirection[3] = q[3];
    this->jestPQ = 2;
}

void WorldObj::save(QTextStream* out){
    
}

Ref::RefItem* WorldObj::getRefInfo(){
    Ref::RefItem* r = new Ref::RefItem();
    r->type = this->type;
    r->filename = this->fileName;
    return r;
}

bool WorldObj::getBorder(float* border){
    return false;
}

void WorldObj::drawBox(){

    if (!box.loaded) {
        float bound[6];
        if (!getBorder((float*)&bound)) return;
        
        float* punkty = new float[72];
        float* ptr = punkty;
        
        for(int i=0; i<2; i++)
            for(int j=4; j<6; j++){
                *ptr++ = bound[i];
                *ptr++ = bound[2];
                *ptr++ = bound[j];
                *ptr++ = bound[i];
                *ptr++ = bound[3];
                *ptr++ = bound[j];
            }
        for(int i=0; i<2; i++)
            for(int j=2; j<4; j++){
                *ptr++ = bound[i];
                *ptr++ = bound[j];
                *ptr++ = bound[4];
                *ptr++ = bound[i];
                *ptr++ = bound[j];
                *ptr++ = bound[5];
            }
        for(int i=4; i<6; i++)
            for(int j=2; j<4; j++){
                *ptr++ = bound[0];
                *ptr++ = bound[j];
                *ptr++ = bound[i];
                *ptr++ = bound[1];
                *ptr++ = bound[j];
                *ptr++ = bound[i];
            }

        box.setMaterial(0.0, 0.0, 1.0);
        box.init(punkty, ptr-punkty, box.V, GL_LINES);

        delete[] punkty;
    }
    

    box.render();
};

bool WorldObj::select(){
    this->selected = true;
    return true;
}

bool WorldObj::select(int value){
    return select();
}

bool WorldObj::unselect(){
    this->selected = false;
}

bool WorldObj::isSelected(){
    return this->selected;
}

void WorldObj::initTrItems(float *tpos){
    
}

void WorldObj::deleteTrItems(){
    
}

QString WorldObj::getShapePath(){
    return "";
}

int WorldObj::getDefaultDetailLevel(){
    return 0;
}

int WorldObj::getCustomDetailLevel(){
    return staticDetailLevel;
}

int WorldObj::getCurrentDetailLevel(){
    if(staticDetailLevel >= 10)
        return 10;
    if(staticDetailLevel >= 0)
        return staticDetailLevel;
    else
        return getDefaultDetailLevel();
}

void WorldObj::setCustomDetailLevel(int val){
    if(val < 0) val = -1;
    if(val > 10) val = 10;
    staticDetailLevel = val;
    this->modified = true;
}

bool WorldObj::customDetailLevelEnabled(){
    if(staticDetailLevel >= 0) return true;
    return false;
}

