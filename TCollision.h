#ifndef __TCOLLISION__
#define __TCOLLISION__

#include "TSettings.h"

class TCollisionMap {
    public:   		
        std::vector<unsigned char> MapData;
        TileMap *mTileMap;
        int createNew(TileMap *cTileMap);
        int loadFromFile(std::string filename);
        int saveToFolder(std::string tpath, std::string tprefix);
        void swapTileValues(int tVal1, int tVal2);
        int removeTile(int cDropTile);
        int checkSize();
};

class TCollisionMapEditor {
    public:   	
        int render(TileMap *cTileMap);
        std::vector<SDL_Rect> TileAreas;
        bool bUpdateWinPos=false;
        int mSelectedTile = 0;
        int mCollisionValue = 0;
};

#endif