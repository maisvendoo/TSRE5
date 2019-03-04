/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "FileBuffer.h"
#include <QString>
#include <unordered_map>

#ifndef TFILE_H
#define	TFILE_H

class TFile {
public:
    struct Mat {
        QString* tex[2];
        QString* name = nullptr;
        int atex[2][2];
        int itex[2][4];
        int count153 = 0;
        int count155 = 0;
        
        Mat(){
            tex[0] = nullptr;
            tex[1] = nullptr;
        }
    };
    bool loaded = false;
    bool used = false;
    float* tdata;
    float* erroeBias;
    //mat* materials;
    std::unordered_map<int, Mat> materials;
    std::unordered_map<int, Mat> amaterials;
    int* flags;
    float WSW = 0;
    float WSE = 0;
    float WNE = 0;
    float WNW = 0;
    float* errthresholdScale = nullptr;
    float* alwaysselectMaxdist = nullptr;
    int* nsamples = nullptr;
    float* sampleRotation = nullptr;
    float* sampleSize = nullptr;
    float floor;
    float scale;
    
    bool waterLevel = false;
    int materialsCount = 0;
    
    QString* sampleFbuffer = nullptr;
    QString* sampleYbuffer = nullptr;
    QString* sampleEbuffer = nullptr;
    QString* sampleNbuffer = nullptr;
    char* sampleASbuffer = nullptr;
    
    int patchsetDistance;
    int patchsetNpatches;
    
    TFile();
    TFile(const TFile& orig);
    virtual ~TFile();
    bool readT(QString fSfile);
    void save(QString name);
    int newMat();
    int cloneMat(int id);
    int getMatByTexture(QString tname);
    void removeMat(int id);
    void setBufferNames(QString name);
    void initNew(QString name, int samples, int sampleS, int patches);
    
private:
    void get139(FileBuffer* data, int length);
    void get151(FileBuffer* data);
    void get153(FileBuffer* data, TFile::Mat* m);
    void get156(FileBuffer* data, TFile::Mat* m);
    void get157(FileBuffer* data);
    void get163(FileBuffer* data, int n);
    void get251(FileBuffer* data);
    
    void print();
    int cloneAMat(int id);
};

#endif	/* TFILE_H */

