#ifndef __TOVERLAY__
#define __TOVERLAY__

#include "TTFTexture.h"
#include <functional>

class TOverlayText{
    public:
        std::vector<TTFTexture*> mNumbers;
        std::map<std::string, TTFTexture*> mText;
        int mMaxNum = -1;
        void init();
        void addNumber(int nnum);
        void addNumbers(int nmax);
        void addText(std::string ntext);
        void renderNum(int cnum, int cx, int cy);
        void renderText(std::string ctext, int cx, int cy);
        void reloadColors();   
};

class TOverlay{
    public:
        int mGridX;
        int mGridY;        
        int mSizeX;
        int mSizeY;        
        int mScale;

        int mX;
        int mY;

        int mIndex;

        void setGrid(int gx, int gy);
        void setSize(int sx, int sy);        
        void setScale(int mscale);
        void setRects(std::vector<SDL_Rect> *cGrid);

        std::vector<SDL_Rect> *mGrid;

        std::function<void()> mRender;

        void render();
};

#endif