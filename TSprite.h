#ifndef __TSPRITE__
#define __TSPRITE__

#include "TileMap.h"

class TSFrame : public Tile{
    public:
        TSFrame(TextureParameters *cTexParam){mTexParam = cTexParam;};
};

class TSprite{
    public:
        TSprite(int cSizeX, int cSizeY, int cBPP){
            mTexParam.TileSizeX = cSizeX;
            mTexParam.TileSizeY = cSizeY;
            mTexParam.TileSetBPP = cBPP;
        };
        TSFrame *mFrame = NULL;
        std::vector<TSFrame*> mFrames;
        TextureParameters mTexParam;
        TSFrame* createFrame(TPalette* tpal);
        void renderEd(int xpos, int ypos, TPalette* tpal);
        void renderIm(int ypos, int mScroll);
        int mCurFrameScale=10;
		int mCurColumns=1;
		int mColSpace = 10;
		int mMaxScrollY=0;
        SDL_Rect mFramesBackGround;
        bool updateWinPos=false;
		//int mSelEdWidth=4;
		//int mCurEdScale=10;
        std::string getSpriteSize();
        std::vector<SDL_Rect> FrameAreas;
		std::vector<SDL_Rect> EditPixelAreas;	
        std::map<int, int> mFrameSizeIn = {{0,8},{1,16},{2,32},{3,64}};
		std::map<int, int> mFrameSizeOut = {{8,0},{16,1},{32,2},{64,3}};
        TEActionUndoStack mActionStack;
};

#endif
