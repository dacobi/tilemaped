#ifndef __TCOLLISION__
#define __TCOLLISION__

#include "TSettings.h"

class TCollisionMap {
    public:   		
        std::vector<unsigned char> MapData;
        TileMap *mTileMap;
        int mMapFormat = 0;
        TOverlay mOverlay;	
	    void initoverlay();
        int createNew(TileMap *cTileMap, int cFormat);
        int loadFromFile(std::string filename, TileMap *cTileMap);
        int saveToFolder(std::string tpath, std::string tprefix);
        void swapTileValues(int tVal1, int tVal2);
        int removeTile(int cDropTile);
        int checkSize();
        std::map<int, std::string> mFormatStrings = {{0,"8 Bits"},{1,"4 Bits"},{2,"Flip + 6 Bits"},{3,"Flip + 2 Bits"},{4,"Palette Offset + 4 Bits"},{5,"Flip + Palette Offset + 2 Bits"}};
        std::map<int, int> mFormatSizes = {{0,1},{1,2},{2,1},{3,2},{4,1},{5,1}};
        std::map<int, int> mFormatMax = {{0,255},{1,15},{2,63},{3,3},{4,15},{5,3}};
};

class TCollisionMapEditor {
    public:   	
        int render();
        int startEdit(TileMap *cTileMap);
        std::vector<SDL_Rect> TileAreas;
        TileMap *mTileMap;
        bool bUpdateWinPos=false;
        int mSelectedTile = 0;
        int mCollisionValue = 0;
        int mColMapOffset = 0;
};

#endif