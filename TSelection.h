#ifndef __TSELECTION__
#define __TSELECTION__

#include "TSettings.h"
#include "TDialogs.h"

enum{
    SELMODE_ALL,
    SELMODE_NONE,
    SELMODE_INVERT
} selectm;


class TSelection{
    public:
        std::vector<int> mSelected;
        SDL_Rect mCurSelection;
        bool bIsSelecting = false;
        bool bHasSelection = false;
        int startSelection(int mx, int my);
        int updateSelection(int rx, int ry);
        int confirmSelection(std::vector<SDL_Rect> &sRects, int xdelta, int ydelta);
        void cancelSelection();
        int renderSelection();
        int findInSelection(int item);
        int addToSelection(int item);
        int removeFromSelection(int item);
        int modifySelection(int item);
        void clearSelection();
        int invertSelection(int sstart, int send);
        int selectRange(int sstart, int send);
   		int searchSelection(std::vector<SDL_Rect> &sRects, int mx, int my);
        int getSelection(std::vector<SDL_Rect> &sRects, SDL_Rect &cSel, int xdelta, int ydelta);
};

enum{
    TBRUSH_TILE,
    TBRUSH_PIXEL
};

class TBrush: public TSelection{
    public:
        std::vector<int> mBrushElements;
        std::vector<TileProperties> mElementProps;
        int UUID;
        int mBrushWidth = 1;
        int mBrushHeight = 1;
        int mLastClickX;
        int mLastClickY;
        int mDeltaBaseX;
        int mDeltaBaseY;
        int *mDeltaScaleX;
        int *mDeltaScaleY;
        int mCursorPos=0;
        int mRenderScale;
        ImVec2 mWinSize;
        ImVec2 mWinPos;
        bool bIsSelected = false;
        bool bIsEditing = false;
        int setElementNext(int element);
        int flipElementV();
        int flipElementH();
        int nextElement();
        int configBrush(int nWidth, int nHeight, int bType, int nRenderScale);
        int setBrushDeltas(int nDeltaX, int nDeltaY, int *nScaleX,int *nScaleY);
        int getBrushSelection(int bx, int by, std::vector<SDL_Rect> &sRects);
        int renderSelection();
        SDL_Rect renderIm(int xpos, int ypos);
        int writeToFile(std::ofstream &outfile);
        int readFromFile(std::ifstream &infile);
        TileProperties getElementProps(int element);
        int getElementFlip(int element);

};

class TBrushList{
    public:
        int mBrushType;
        std::vector<TBrush*> mBrushes;
        std::string mTitle;
        std::string mType;
        bool *bIsShown;
        bool bIsEditing = false;
        int mWinHeight = 600;
        int mRenderScale;
        int mNewBrushX=1;
        int mNewBrushY=1;
        int mSelectedBrush=0;
   		std::vector<SDL_Rect> BrushAreas;
        int init(std::string cTitle, std::string cType, int cBrushType, bool *cIsShown, int cRenderScale);
        int addBrush(int sizex, int sizey);
        int addBrushElement(int element);
        int removeBrush();
        //int renderIm(int xpos, int ypos);
        int renderIm();
        int saveToFile(std::string cBrushPath);
        int loadFromFile(std::string cBrushPath);
};

#endif