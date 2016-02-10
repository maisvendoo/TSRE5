#ifndef TERRAINLIB_H
#define	TERRAINLIB_H

#include <unordered_map>
#include <QString>
#include "GLUU.h"

class Terrain;
class Brush;

class TerrainLib {
public:
    static std::unordered_map<int, Terrain*> terrain;
    
    TerrainLib();
    TerrainLib(const TerrainLib& orig);
    virtual ~TerrainLib();
    static void fillRAW(float** terrainData, int mojex, int mojez);
    static float getHeight(float x, float z, float posx, float posz);
    static float getHeight(float x, float z, float posx, float posz, bool addR);
    static void setHeight(float x, float z, float posx, float posz, float h);
    static int setHeight256(int x, int z, int posx, int posz, float h);
    static int setHeight256(int x, int z, int posx, int posz, float h, float diffC, float diffE);
    static bool isLoaded(int x, int z);
    static void paintHeightMap(Brush* brush, int x, int z, float* p);
    static void paintTexture(Brush* brush, int x, int z, float* p);
    static void setTerrainTexture(Brush* brush, int x, int z, float* p);
    static void setWaterDraw(int x, int z, float* p);
    static void setWaterLevelGui(int x, int z, float* p);
    static void setFixedTileHeight(Brush* brush, int x, int z, float* p);
    static void setDraw(int x, int z, float* p);
    static void setTerrainToTrackObj(Brush* brush, float* punkty, int length, int x, int z, float* matrix);
    static int getTexture(int x, int z, float* p);
    static bool load(int x, int z);
    static void save();
    static void refresh(int x, int z);
    static bool reload(int x, int z);
    static void render(GLUU *gluu, float* playerT, float* playerW, float* target, float fov);
private:

};

#endif	/* TERRAINLIB_H */

