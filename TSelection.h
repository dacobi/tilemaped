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
        std::vector<bool> mSelectionBorder;
		std::vector<unsigned char> mSelectionEdges;		
        bool bSelectionUpdate = false;
        int mAreaX;
        int mAreaY;
        int mWidth;
        int mHeight;
        int *mScale;
        int init(int cAreaX, int cAreaY, int cWidth, int cHeight, int *cScale);
        int resize(int cAreaX, int cAreaY, int cWidth, int cHeight, int *cScale);
        int calcSelectionBorder();
		int renderSelection(int xpos, int ypos);
        int startSelection(int mx, int my);
        int updateSelection(int rx, int ry);
        int confirmSelection(std::vector<SDL_Rect> &sRects, int xdelta, int ydelta);
        void cancelSelection();
        int renderSelection();
        int findInSelection(int item);
        bool isInSelection(int item);
        int addToSelection(int item);
        int removeFromSelection(int item);
        int removeRanges(std::vector<int> cRanges);
        int modifySelection(int item);
        void clearSelection();
        int invertSelection(int sstart, int send);
        int selectRange(int sstart, int send);
   		int searchSelection(std::vector<SDL_Rect> &sRects, int mx, int my);
        int getSelection(std::vector<SDL_Rect> &sRects, SDL_Rect &cSel, int xdelta, int ydelta);
        int getXY(int X,int Y);
        int getXYFromIndex(int index,int careax, int careay, int &mx, int &my);
        int getTileIndex(int index, int careax, int careay,int &tIndex);
};

enum{
    TBRUSH_TILE,
    TBRUSH_PIXEL
};

class TBrushList;

class TBrush: public TSelection{
    public:
        std::vector<int> mBrushElements;
        std::vector<TileProperties> mElementProps;
        TBrushList *mParent;
        int UUID;
        int mBrushType;
        int mBrushWidth = 1;
        int mBrushHeight = 1;
        int mLastClickX;
        int mLastClickY;
        //int mDeltaBaseX;
        //int mDeltaBaseY;        
        //int *mDeltaScaleY;
        int mCursorPos=0;
        //int mRenderScale;
        ImVec2 mWinSize;
        ImVec2 mWinPos;
        bool bIsSelected = false;
        bool bIsEditing = false;
        int setElementNext(int element);
        int flipElementV();
        int flipElementH();
        int nextElement();
        int prevElement();
        int nextLine();
        int prevLine();
        int configBrush(int nWidth, int nHeight, int bType, TBrushList *cParent);
        //int setBrushDeltas(int nDeltaX, int nDeltaY);
        int getBrushSelection(int bx, int by, std::vector<SDL_Rect> &sRects);
        int renderSelection();
        SDL_Rect renderIm(int xpos, int ypos);
        SDL_Rect renderTile(int xpos, int ypos);
        SDL_Rect renderPixel(int xpos, int ypos);
        int writeToFile(std::ofstream &outfile);
        int readFromFile(std::ifstream &infile);
        TileProperties getElementProps(int element);
        int getElementFlip(int element);
        int setElementFlip(int element, int cFlip);
        

};

class TBrushList{
    public:
        int mBrushType;
        std::vector<TBrush*> mBrushes;
        std::string mTitle;
        std::string mType;
        bool *bIsShown;
        int *mDeltaScale;
        bool bIsEditing = false;
        int mWinHeight = 600;
        int mDeltaBaseX;
        int mDeltaBaseY;        
        int mRenderScale;
        int mNewBrushX=1;
        int mNewBrushY=1;
        int mSelectedBrush=0;
        int mBrushOffset = 0;
        int setBrushDeltas(int nDeltaX, int nDeltaY, int *nDeltaScale, int nRenderScale);
   		std::vector<SDL_Rect> BrushAreas;
        int init(std::string cTitle, std::string cType, int cBrushType, bool *cIsShown, int nDeltaX, int nDeltaY, int *cDeltaScale, int cRenderScale, TBrush **cCurrentBrush);
        TBrush **mCurrentBrush;
        TBrush* getBrush();
        TBrush* getNextBrush();
        int addBrush(int sizex, int sizey);
        int addBrushElement(int element);
        int removeBrush();
        //int renderIm(int xpos, int ypos);
        int renderIm();
        int saveToFile(std::string cBrushPath);
        int loadFromFile(std::string cBrushPath);
};

#endif