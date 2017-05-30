/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Game.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "RouteEditorWindow.h"
#include "LoadWindow.h"
#include "CELoadWindow.h"
#include "SoundList.h"
#include "ShapeLib.h"
#include "EngLib.h"
#include <QColor>

TDB *Game::trackDB = NULL;
TDB *Game::roadDB = NULL;    
SoundList *Game::soundList = NULL;    

QString Game::AppName = "TSRE5";
QString Game::AppVersion = "v0.6911";
QString Game::root = "F:/Train Simulator";
QString Game::route = "bbb1";
QString Game::routeName = "bbb";
QString Game::trkName = "bbb";
//QString Game::route = "traska";
//QString Game::route = "cmk";
int Game::allowObjLag = 1000;
int Game::maxObjLag = 10;
bool Game::ignoreLoadLimits = false;
int Game::startTileX = 0;
int Game::startTileY = 0;
float Game::objectLod = 3000;
int Game::tileLod = 2;
int Game::start = 0;
bool Game::ignoreMissingGlobalShapes = false;
bool Game::deleteTrWatermarks = false;
bool Game::deleteViewDbSpheres = false;
bool Game::createNewRoutes = false;
bool Game::writeEnabled = false;
bool Game::writeTDB = false;
bool Game::systemTheme = false;
bool Game::toolsHidden = false;
bool Game::usenNumPad = false;
float Game::cameraFov = 55.0f;
float Game::cameraSpeedMin = 1.0;
float Game::cameraSpeedStd = 3.0;
float Game::cameraSpeedMax = 40.0;
bool Game::cameraStickToTerrain = false;
bool Game::mstsShadows = false;

bool Game::viewWorldGrid = true;
bool Game::viewTileGrid = true;
bool Game::viewTerrainGrid = false;
bool Game::viewTerrainShape = true;
bool Game::viewInteractives = true;
bool Game::viewTrackDbLines = true;
bool Game::viewTsectionLines = true;
bool Game::viewPointer3d = true;
bool Game::viewMarkers = false;
bool Game::viewSnapable = false;
bool Game::warningBox = false;
bool Game::leaveTrackShapeAfterDelete = false;
bool Game::renderTrItems = false;
int Game::newRouteX = -5000;
int Game::newRouteZ = 15000;
bool Game::consoleOutput = true;
int Game::fpsLimit = 0;
bool Game::ortsEngEnable = true;
bool Game::sortTileObjects = true;
int Game::oglDefaultLineWidth = 1;
bool Game::showWorldObjPivotPoints = false;
int Game::shadowMapSize = 2048;
int Game::shadowsEnabled = 1;
float Game::sunLightDirection[] = {-1.0,2.0,1.0};
int Game::textureQuality = 1;
float Game::snapableRadius = 20;
bool Game::snapableOnlyRot = false;
float Game::trackElevationMaxPm = 700.0;

QString Game::geoPath = "hgst";

RouteEditorWindow* Game::window = NULL;
LoadWindow* Game::loadWindow = NULL;
ShapeLib *Game::currentShapeLib = NULL;
EngLib *Game::currentEngLib = NULL;
Route *Game::currentRoute = NULL;
QColor *Game::colorConView = NULL;
QColor *Game::colorShapeView = NULL;

QString Game::StyleMainLabel = "#770000";
QString Game::StyleGreenButton = "#55FF55";
QString Game::StyleRedButton = "#FF5555";
QString Game::StyleYellowButton = "#FFFF55";
QString Game::StyleGreenText = "#009900";
QString Game::StyleRedText = "#990000";

QString Game::imageMapsUrl;
int Game::mapImageResolution = 4096;

bool Game::autoNewTiles = false;
bool Game::autoGeoTerrain = false;

int Game::AASamples = 0;
float Game::PixelRatio = 1.0;

void Game::load() {
    
    QString sh;
    QString path;
    path = "settings.txt";
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;
    qDebug() << path;

    QTextStream in(&file);
    QString line;
    QStringList args;
    QString val;
    while (!in.atEnd()) {
        line = in.readLine();
        //qDebug() << line;
        if(line.startsWith("#", Qt::CaseInsensitive)) continue;
        //args = line.split("=");
        args.clear();
        args.push_back(line.section('=', 0, 0));
        args.push_back(line.section('=', 1));
        //qDebug() << args[0] << args[1];
        if(args.count() < 2) continue;
        val = args[0].trimmed();
        //qDebug() << args[0].trimmed() << " "<< args[1].trimmed();
        if(val == "consoleOutput"){
            if(args[1].trimmed().toLower() == "true")
                Game::consoleOutput = true;
            else
                Game::consoleOutput = false;
        }
        if(val == "gameRoot")
            root = args[1].trimmed();
        if(val == "routeName")
            route = args[1].trimmed();

        if(val == "startTileX"){
            Game::start++;
            startTileX = args[1].trimmed().toInt();
        }
        if(val == "startTileY"){
            Game::start++;
            startTileY = args[1].trimmed().toInt();
        }
        if(val == "deleteTrWatermarks"){
            if(args[1].trimmed().toLower() == "true")
                deleteTrWatermarks = true;
            else
                deleteTrWatermarks = false;
        }
        if(val == "deleteViewDbSpheres"){
            if(args[1].trimmed().toLower() == "true")
                deleteViewDbSpheres = true;
            else
                deleteViewDbSpheres = false;
        }
        if(val == "createNewIfNotExist"){
            if(args[1].trimmed().toLower() == "true")
                createNewRoutes = true;
            else
                createNewRoutes = false;
        }
        if(val == "writeEnabled"){
            if(args[1].trimmed().toLower() == "true")
                writeEnabled = true;
            else
                writeEnabled = false;
        }
        if(val == "writeTDB"){
            if(args[1].trimmed().toLower() == "true")
                writeTDB = true;
            else
                writeTDB = false;
        }
        if(val == "systemTheme"){
            if(args[1].trimmed().toLower() == "true")
                systemTheme = true;
            else
                systemTheme = false;
        }
        if(val == "toolsHidden"){
            if(args[1].trimmed().toLower() == "true")
                toolsHidden = true;
            else
                toolsHidden = false;
        }
        if(val == "usenNumPad"){
            if(args[1].trimmed().toLower() == "true")
                usenNumPad = true;
            else
                usenNumPad = false;
        }
        if(val == "tileLod"){
            tileLod = args[1].trimmed().toInt();
        }
        if(val == "objectLod"){
            objectLod = args[1].trimmed().toInt();
        }
        if(val == "maxObjLag"){
            maxObjLag = args[1].trimmed().toInt();
        }
        if(val == "allowObjLag"){
            allowObjLag = args[1].trimmed().toInt();
        }
        if(val == "fpsLimit"){
            fpsLimit = args[1].trimmed().toInt();
        }
        if(val == "cameraFov"){
            cameraFov = args[1].trimmed().toFloat();
        }
        if(val == "warningBox"){
            if(args[1].trimmed().toLower() == "true")
                warningBox = true;
            else
                warningBox = false;
        }
        if(val == "leaveTrackShapeAfterDelete"){
            if(args[1].trimmed().toLower() == "true")
                leaveTrackShapeAfterDelete = true;
            else
                leaveTrackShapeAfterDelete = false;
        }
        if(val == "renderTrItems"){
            if(args[1].trimmed().toLower() == "true")
                renderTrItems = true;
            else
                renderTrItems = false;
        }
        if(val == "geoPath")
            geoPath = args[1].trimmed();
        if(val == "colorConView")
            colorConView = new QColor(args[1].trimmed());
        if(val == "colorShapeView")
            colorShapeView = new QColor(args[1].trimmed());
        if(val == "ortsEngEnable"){
            if(args[1].trimmed().toLower() == "true")
                ortsEngEnable = true;
            else
                ortsEngEnable = false;
        }
        if(val == "sortTileObjects"){
            if(args[1].trimmed().toLower() == "true")
                sortTileObjects = true;
            else
                sortTileObjects = false;
        }
        if(val == "ignoreMissingGlobalShapes"){
            if(args[1].trimmed().toLower() == "true")
                ignoreMissingGlobalShapes = true;
            else
                ignoreMissingGlobalShapes = false;
        }
        if(val == "snapableOnlyRot"){
            if(args[1].trimmed().toLower() == "true")
                snapableOnlyRot = true;
            else
                snapableOnlyRot = false; 
        }
        if(val == "oglDefaultLineWidth"){
            oglDefaultLineWidth = args[1].trimmed().toInt();
        }
        if(val == "shadowsEnabled"){
            shadowsEnabled = args[1].trimmed().toInt();
        }
        if(val == "textureQuality"){
            textureQuality = args[1].trimmed().toInt();
        }
        if(val == "imageMapsUrl"){
            imageMapsUrl = args[1].trimmed();
        }
        if(val == "mapImageResolution"){
            mapImageResolution = args[1].trimmed().toInt();
        }
        if(val == "AASamples"){
            AASamples = args[1].trimmed().toInt();
        }
        if(val == "cameraStickToTerrain"){
            if(args[1].trimmed().toLower() == "true")
                cameraStickToTerrain = true;
            else
                cameraStickToTerrain = false; 
        }
        if(val == "cameraSpeedMin"){
            cameraSpeedMin = args[1].trimmed().toFloat();
        }
        if(val == "cameraSpeedStd"){
            cameraSpeedStd = args[1].trimmed().toFloat();
        }
        if(val == "cameraSpeedMax"){
            cameraSpeedMax = args[1].trimmed().toFloat();
        }
        if(val == "trackElevationMaxPm"){
            trackElevationMaxPm = args[1].trimmed().toFloat();
        }
    }
}

bool Game::loadRouteEditor(){
    if(Game::warningBox){
        QMessageBox msgBox;
        msgBox.setText("This is experimental version.\nUsing it may seriously damage your routes."
                       "\nMake backup first!\n\nTo disable this window, set 'warningBox = false' in settings.txt.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
    
    window = new RouteEditorWindow();
    window->resize(1280, 800);
    
    loadWindow = new LoadWindow();
    QObject::connect(window, SIGNAL(exitNow()),
                      loadWindow, SLOT(exitNow()));
    
    QObject::connect(loadWindow, SIGNAL(showMainWindow()),
                      window, SLOT(show()));
    
    if(Game::checkRoot(Game::root) && (Game::checkRoute(Game::route) || Game::createNewRoutes))
        Game::window->show();
    else
        Game::loadWindow->show();
}

bool Game::loadConEditor(){
    if(Game::warningBox){
        QMessageBox msgBox;
        msgBox.setText("This is experimental version.\nUsing it may seriously damage your consists."
                       "\nMake backup first!\n\nTo disable this window, set 'warningBox = false' in settings.txt.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
    CELoadWindow* ceLoadWindow = new CELoadWindow();
    ceLoadWindow->show();
    //ConEditorWindow* cwindow = new ConEditorWindow();
    //cwindow->resize(1280, 720);
    //cwindow->show();
}

bool Game::checkRoot(QString dir){
    QString path;
    path = dir + "/routes";
    path.replace("//", "/");
    QFile file(path);
    if (!file.exists()) {
        qDebug () << "/routes not exist: "<< file.fileName();
        return false;
    }
    path = dir + "/global";
    path.replace("//", "/");
    file.setFileName(path);
    if (!file.exists()) {
        qDebug () << "/global not exist: "<< file.fileName();
        return false;
    }
    path = dir + "/global/tsection.dat";
    path.replace("//", "/");
    file.setFileName(path);
    if (!file.exists()) {
        qDebug () << "/global/tsection.dat not exist: "<< file.fileName();
        return false;
    }
    
    return true;
}

bool Game::checkCERoot(QString dir){
    QString path;
    path = dir + "/trains";
    path.replace("//", "/");
    QFile file(path);
    if (!file.exists()) {
        qDebug () << "/trains not exist: "<< file.fileName();
        return false;
    }
    path = dir + "/trains/trainset";
    path.replace("//", "/");
    file.setFileName(path);
    if (!file.exists()) {
        qDebug () << "/trains/trainset not exist: " << file.fileName();
        return false;
    }
    path = dir + "/trains/consists";
    path.replace("//", "/");
    file.setFileName(path);
    if (!file.exists()) {
        qDebug () << "/trains/consists not exist: " << file.fileName();
        return false;
    }
    
    return true;
}

bool Game::checkRoute(QString dir){
    QFile file;
    file.setFileName(Game::root+"/routes/"+dir+"/"+dir+".trk");
    if(file.exists()){
        Game::trkName = dir;
        return true;
    }
    QDir folder(Game::root+"/routes/"+dir+"/");
    folder.setNameFilters(QStringList() << "*.trk");
    folder.setFilter(QDir::Files);
    foreach(QString dirFile, folder.entryList()){
        Game::trkName = dirFile.split(".")[0];
        //qDebug() << Game::trkName;
        return true;
    }
    
    //qDebug() << file.fileName();
    //qDebug() << file.exists();
    return false;
}

template<class T>
void Game::check_coords(T&& x, T&& z, float* p) {
    if (p[0] >= 1024) {
        p[0] -= 2048;
        x++;
    }
    if (p[0] < -1024) {
        p[0] += 2048;
        x--;
    }
    if (p[2] >= 1024) {
        p[2] -= 2048;
        z++;
    }
    if (p[2] < -1024) {
        p[2] += 2048;
        z--;
    }
}
template void Game::check_coords(int& x, int& z, float* p);
template void Game::check_coords(float& x, float& z, float* p);

template<class T, class K>
void Game::check_coords(T&& x, T&& z, K&& px, K&& pz) {
    if (px >= 1024) {
        px -= 2048;
        x++;
    }
    if (px < -1024) {
        px += 2048;
        x--;
    }
    if (pz >= 1024) {
        pz -= 2048;
        z++;
    }
    if (pz < -1024) {
        pz += 2048;
        z--;
    }
}
template void Game::check_coords(int& x, int& z, int& px, int& pz);
template void Game::check_coords(int& x, int& z, float& px, float& pz);
template void Game::check_coords(float& x, float& z, float& px, float& pz);