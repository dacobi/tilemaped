#ifndef __TOVERLAY__
#define __TOVERLAY__

#include "TTFTexture.h"
#include <functional>

#define MOVERLAYMAXCOL 8

class TOverlay;

class TOverlayText{
    public:
        std::string mFontFile;
        int mFontSize = 14;
        TTF_Font *TFont = NULL;
        std::vector<TTFTexture*> mNumbers;
        std::map<std::string, TTFTexture*> mText;
        std::vector<TOverlay*> mOverlays;
        SDL_Color mColors[MOVERLAYMAXCOL] = {{0xff, 0xff, 0xff, 0xff},{0x0, 0x0, 0x0, 0xff},{0xff, 0x0, 0x0, 0xff},{0x0, 0xff, 0x0, 0xff},{0x0, 0x0, 0xff, 0xff},{0xff, 0x0, 0xff, 0xff},{0x0, 0xff, 0xff, 0xff},{0xff, 0xff, 0x0, 0xff}};
        int mMaxNum = -1;
        int mLastWidth = 0;
        int mLastHeight = 0;
        void init(std::string cFontFile);
        void setFontSize(int cFontSize);
        void reg(TOverlay* cOverlay);
        void unreg(TOverlay* cOverlay);
        void addNumber(int nnum);
        void addNumbers(int nmax);
        void addText(std::string ntext);
        void renderNum(int cnum, int cx, int cy);
        void renderText(std::string ctext, int cx, int cy);
        void setColor(int cnewcol);
        void nextColor();
        void reloadTextures();   
};

class TOverlay{
    public:
        int mGridX;
        int mGridY;        
        int mSizeX;
        int mSizeY;        
        
        int mScaleX;
        int mScaleY;
        int mScaleLines;

        std::string mScaleText;

        int mX;
        int mY;

        int mIndex;

        ~TOverlay();

        void setGrid(int gx, int gy);
        void setSize(int sx, int sy);        
        void setScale(std::string cscale, int cLines);
        void updateScale();
        void setRects(std::vector<SDL_Rect> *cGrid);

        std::vector<SDL_Rect> *mGrid;

        std::function<void()> mRender;

        void render();
};

#endif