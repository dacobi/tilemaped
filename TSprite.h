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
            mTexParam.TexSizeX = cSizeX;
            mTexParam.TexSizeY = cSizeY;
            mTexParam.TexBPP = cBPP;

            mTexParam.TexEditScale = mGlobalSettings.mGlobalTexParam.TexEditScale;
            
            if((cSizeX > 8) || (cSizeY > 8)){
                mTexParam.TexEditScale--;
                //mCurFrameScale-=1;
            }
            if((cSizeX > 16) || (cSizeY > 16)){
                mTexParam.TexEditScale--;
                //mCurFrameScale-=4;
            }
            if((cSizeX > 32) || (cSizeY > 32)){
                mTexParam.TexEditScale--;
                //mCurFrameScale-=3;
            }

            if(mTexParam.TexEditScale < 1){
                mTexParam.TexEditScale = 1;
                mTexParam.TexPixelSize-=4;
            }

            switch (cSizeX)
            {
            case 64:                
                MaxFrame = 6;
                mMinFrameScale = 3;
                break;
            case 32:
                MaxFrame = 10;
                break;
            case 16:                
                mMinFrameScale = 7;
                break;
            case 8:
                mMinFrameScale = 12;
                MaxFrame = 20;
                break;
            
            default:
                break;
            }
            
            mClipboard.init("Sprite","Pixel", TBRUSH_PIXEL, &bShowClipboard, mTexParam.TexPixelSize, mTexParam.TexPixelSize, &mTexParam.TexEditScale, mTexParam.TexEditScale, &mCurrentBrushPixel, &mTexParam);
        };

        bool bShowClipboard = false;
        TSFrame *mFrame = NULL;
        std::vector<TSFrame*> mFrames;
        TextureParameters mTexParam;
        TSFrame* createFrame(TPalette* tpal);
        TSFrame* createNew(TPalette* tpal);
		TSFrame* createNewCopy(TSFrame* cCopyTile, TPalette* tpal);
		TSFrame* createNewFromBuffer(std::vector<unsigned char> &newBuf, TPalette* tpal);
        TSFrame* createNewUpscaledCopy(TSFrame* cCopyFrame, TPalette* tpal);
		TSFrame* createNewFromFile(std::string newPath, TPalette* tpal);
        int removeFrame(int cDropFrame);
		void appendFrame(TSFrame* addFrame);		
        void renderEd(int xpos, int ypos, TPalette* tpal);
        void renderIm(int ypos, int mScroll);
        int saveToFile(std::string spath, std::string sfile);
        int loadFromBuffer(std::vector<unsigned char> sBuf, TPalette* tpal);
        int importFromBuffer(std::vector<unsigned char> sBuf, TPalette* tpal, std::vector<TSFrame*> &cNewFrames);
        int importPNG(SDL_Surface *cTiles, TPalette* tpal, std::vector<TSFrame*> &cNewFrames);   
        int mCurFrameScale = 10;         
		int mCurColumns = 1;
        int mMaxColumns = 2;
        int mMinFrameScale = 5;        
        int mChildTop = 0;
        int MaxFrame = 15;
        void resizeScale(){updateWinPos = true; mCurColumns = 1; mCurFrameScale = MaxFrame;};
        const static int MaxCol = 5;
        const static int MinCol = 2;
		int mColSpace = 10;
        int SpriteWidth=200;		
        const static int MaxScale = 32;
        const static int MinScale = 4;        
        const static int MinFrame = 2;
        int mSpriteScrollX = 0;
        int mSpriteScrollY = 0;
        SDL_Rect mFramesBackGround;
        bool updateWinPos=false;
        int mSelectedFrame = 0;
        void selectFrame(int cFrame);
        int selectNext();
        int selectPrev();
        bool bShowSpriteFrames = true;
        TBrush *mCurrentBrushPixel = NULL;
        bool bShowBrushesPixel = false;
        std::string getSpriteSize();        
        std::string getSize(){return getSpriteSize();};
        std::vector<SDL_Rect> FrameAreas;		
        TEActionUndoStack mActionStack;
        TClipboard mClipboard;
};

#endif
