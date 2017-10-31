/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "TerrainWaterWindow2.h"
#include <QtWidgets>
#include <QDebug>
#include "Game.h"
#include "Terrain.h"

TerrainWaterWindow2::TerrainWaterWindow2(QWidget* parent) : QWidget(parent) {
    this->setWindowFlags(Qt::WindowType::Tool | Qt::MSWindowsFixedSizeDialogHint);
    this->setWindowTitle(tr("Water Level"));
    
    for(int i = 0; i < 17; i++)
        e[i].setFixedWidth(50);
    for(int i = 0; i < 17; i++)
        e[i].setText("10");
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++){
            if(i == 0 || j == 0 || i == 3 || j == 3)
                e[i*4+j].setDisabled(true);
        }
    eAvg.setFixedWidth(50);
    eNW.setFixedWidth(50);
    eNE.setFixedWidth(50);
    eSW.setFixedWidth(50);
    eSE.setFixedWidth(50);
    
    QGridLayout *vlist1 = new QGridLayout;
    vlist1->setSpacing(2);
    vlist1->setContentsMargins(3,0,3,0);
    int row = 0;
    vlist1->addWidget(new QLabel("Water:"), row, 0);
    vlist1->addWidget(&e[0], 0, 0);
    vlist1->addWidget(&e[1], 0, 2);
    vlist1->addWidget(&e[2], 0, 4);
    vlist1->addWidget(&e[3], 0, 6);
    vlist1->addWidget(new QLabel("NW"), 1, 1);
    vlist1->addWidget(new QLabel("  ---------"), 1, 2);
    vlist1->addWidget(new QLabel("  ---------"), 1, 3);
    vlist1->addWidget(new QLabel("  ---------"), 1, 4);
    vlist1->addWidget(new QLabel("NE"), 1, 5);
    vlist1->addWidget(&e[4], 2, 0);
    vlist1->addWidget(new QLabel("|"), 2, 1);
    vlist1->addWidget(&eNW, 2, 2);
    vlist1->addWidget(new QLabel(" Average:"), 2, 3);
    vlist1->addWidget(&eNE, 2, 4);
    vlist1->addWidget(new QLabel("|"), 2, 5);
    vlist1->addWidget(&e[7], 2, 6);
    vlist1->addWidget(new QLabel("|"), 3, 1);
    vlist1->addWidget(&eAvg, 3, 3);
    //vlist1->addWidget(new QLabel("TILE"), 3, 3);
    vlist1->addWidget(new QLabel("|"), 3, 5);
    vlist1->addWidget(&e[8], 4, 0);
    vlist1->addWidget(new QLabel("|"), 4, 1);
    vlist1->addWidget(&eSW, 4, 2);
    vlist1->addWidget(&eSE, 4, 4);
    vlist1->addWidget(new QLabel("|"), 4, 5);
    vlist1->addWidget(&e[11], 4, 6);
    vlist1->addWidget(new QLabel("SW"), 5, 1);
    vlist1->addWidget(new QLabel("  ---------"), 5, 2);
    vlist1->addWidget(new QLabel("  ---------"), 5, 3);
    vlist1->addWidget(new QLabel("  ---------"), 5, 4);
    vlist1->addWidget(new QLabel("SE"), 5, 5);
    vlist1->addWidget(&e[12], 6, 0);
    vlist1->addWidget(&e[13], 6, 2);
    vlist1->addWidget(&e[14], 6, 4);
    vlist1->addWidget(&e[15], 6, 6);
    QPushButton *bAdjust = new QPushButton("Adjust Adjacent Tiles");
    vlist1->addWidget(bAdjust, 7, 0, 1, 7);
    this->setLayout(vlist1);
    
    connect(&eAvg, SIGNAL (textEdited(QString)), this, SLOT (eAvgTextEdited(QString)));
    connect(&eSW, SIGNAL (textEdited(QString)), this, SLOT (eWaterEdited(QString)));
    connect(&eSE, SIGNAL (textEdited(QString)), this, SLOT (eWaterEdited(QString)));
    connect(&eNW, SIGNAL (textEdited(QString)), this, SLOT (eWaterEdited(QString)));
    connect(&eNE, SIGNAL (textEdited(QString)), this, SLOT (eWaterEdited(QString)));
}

TerrainWaterWindow2::~TerrainWaterWindow2() {
}

void TerrainWaterWindow2::setTerrain(Terrain* t){
    if(t == NULL)
        return;
    if(t == terrain)
        return;
    
    terrain = t;
    this->setWindowTitle(tr("Water Level ") + QString::number(terrain->mojex) + " "+ QString::number(-terrain->mojez));
    
    eAvg.setText(QString::number(terrain->getAvgVaterLevel()));
    float waterLevels[4];
    terrain->getWaterLevels(waterLevels);
    eNW.setText(QString::number(waterLevels[0]));
    eNE.setText(QString::number(waterLevels[1]));
    eSW.setText(QString::number(waterLevels[2]));
    eSE.setText(QString::number(waterLevels[3]));
}

void TerrainWaterWindow2::eAvgTextEdited(QString val){
    if(terrain == NULL)
        return;
    bool ok = false;
    float f = val.toFloat(&ok);
    if(!ok)
        return;
    terrain->setAvgWaterLevel(f);
}

void TerrainWaterWindow2::eWaterEdited(QString val){
    if(terrain == NULL)
        return;
    bool ok = false;
    float nw = eNW.text().toFloat(&ok);
    if(!ok)
        return;
    float ne = eNE.text().toFloat(&ok);
    if(!ok)
        return;    
    float sw = eSW.text().toFloat(&ok);
    if(!ok)
        return;    
    float se = eSE.text().toFloat(&ok);
    if(!ok)
        return;    
    terrain->setWaterLevel(nw, ne, sw, se);
}