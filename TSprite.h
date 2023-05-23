#ifndef __TSPRITE__
#define __TSPRITE__

#include "TileMap.h"

class TSFrame : public Tile{
    public:
        TSFrame(TextureParameters *cTexParam){mTexParam = cTexParam;};
        void renderEd(int xpos, int ypos, TPalette* tpal);
};

class TSprite{
    public:
        void close();
        TSprite(int cSizeX, int cSizeY, int cBPP){
            mTexParam.TileSizeX = cSizeX;
            mTexParam.TileSizeY = cSizeY;
            mTexParam.TileSetBPP = cBPP;

            mTexParam.mTileEdScale = mGlobalSettings.mGlobalTexParam.mTileEdScale;

            if((cSizeX > 16) || (cSizeY > 16)){
                mTexParam.mTileEdScale-=2;
                mCurFrameScale-=4;
            }
            if((cSizeX > 32) || (cSizeY > 32)){
                mTexParam.mTileEdScale--;
                mCurFrameScale-=3;
            }

            if(mTexParam.mTileEdScale < 1){
                mTexParam.mTileEdScale = 1;
                mTexParam.TilePixelSize-=4;
            }

        };
        TSFrame *mFrame = NULL;
        std::vector<TSFrame*> mFrames;
        TextureParameters mTexParam;
        TSFrame* createFrame(TPalette* tpal);
        TSFrame* createNew(TPalette* tpal);
		TSFrame* createNewCopy(TSFrame* cCopyTile, TPalette* tpal);
		TSFrame* createNewFromBuffer(std::vector<unsigned char> &newBuf, TPalette* tpal);
		TSFrame* createNewFromFile(std::string newPath, TPalette* tpal);
        int removeFrame(int cDropFrame);
		void appendFrame(TSFrame* addFrame);		
        void renderEd(int xpos, int ypos, TPalette* tpal);
        void renderIm(int ypos, int mScroll);
        int saveToFile(std::string spath, std::string sfile);
        int loadFromBuffer(std::vector<unsigned char> sBuf, TPalette* tpal);        
        int mCurFrameScale=10;
		int mCurColumns=1;
		int mColSpace = 10;
		int mMaxScrollY=0;
        int mFrameScrollY = 0;
        SDL_Rect mFramesBackGround;
        bool updateWinPos=false;
        int mSelectedFrame = 0;
        void selectFrame(int cFrame);
        int selectNext();
        int selectPrev();
        bool bShowSpriteFrames = true;
        TBrush *mCurrentBrushPixel = NULL;
        bool bShowBrushesPixel = false;
		//int mSelEdWidth=4;
		//int mCurEdScale=10;
        std::string getSpriteSize();
        //int reCalculateScale();
        //void resizeEdit();
		//void updateEditAreas(std::vector<SDL_Rect> &cTile, int xpos, int ypos);
        std::vector<SDL_Rect> FrameAreas;
		std::vector<SDL_Rect> EditPixelAreas;	        
        TEActionUndoStack mActionStack;
};

#endif
