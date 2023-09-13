#ifndef __TSELECTION__
#define __TSELECTION__

#include "TSettings.h"
#include "TEAction.h"
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
        int *mWidth;
        int *mHeight;
        int *mScale;
        int init(int cAreaX, int cAreaY, int *cWidth, int *cHeight, int *cScale);
        int resize(int cAreaX, int cAreaY, int *cWidth, int *cHeight, int *cScale);
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
        bool isSelectionRectangular(int &cFirstX, int &cFirstY, int &cLastX, int &cLastY);
        bool getSelectionBounds(int &nLeft, int &nTop, int &nRight, int &nButtom);
        int getSelection(std::vector<SDL_Rect> &sRects, SDL_Rect &cSel, int xdelta, int ydelta);
        int getXY(int X,int Y);
        int getXYFromIndex(int index,int careax, int careay, int &mx, int &my);
        int getTileIndex(int index, int careax, int careay,int &tIndex, TextureParameters *mTexParam);
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
        std::vector<SDL_Rect> BrushElementAreas;
        int UUID;
        int mBrushType;
        int mBrushWidth = 1;
        int mBrushHeight = 1;
        int mLastClickX;
        int mLastClickY;        
        int mCursorPos=0;        
        ImVec2 mWinSize;
        ImVec2 mWinPos;
        bool bIsSelected = false;
        bool bIsEditing = false;        
        int setElementNext(int element);
        void clearAllElements();
        void makeCircle(bool bInvert, float ms = 0.0);
        int flipElementV();
        int flipElementH();
        int nextElement();
        int prevElement();
        int nextLine();
        int prevLine();
        int configBrush(int nWidth, int nHeight, int bType, TBrushList *cParent);        
        int getBrushSelection(int bx, int by, std::vector<SDL_Rect> &sRects);
        int renderSelection();
        SDL_Rect renderIm(int xpos, int ypos);
        SDL_Rect renderTile(int xpos, int ypos);
        SDL_Rect renderPixel(int xpos, int ypos);
        int writeToFile(std::ofstream &outfile);
        int readFromFile(std::ifstream &infile);
        TileProperties getElementProps(int element);
        int getElementFlip(int element);
        int getElementOffset(int element);
        int setElementFlip(int element, int cFlip);
        int setElementOffset(int element, int cOffset);
        void swapBrushElements(int eVal1, int eVal2);        
        void removeBrushElement(int cDropTile);
};

class TBrushList{
    public:
        int mBrushType;
        std::vector<TBrush*> mBrushes;
        std::string mTitle;
        std::string mType;
        TextureParameters *mTexParam;
        TextureParameters mLocalTexParam;
        bool *bIsShown;
        int *mDeltaScale;
        bool bIsEditing = false;
        bool bSetAllElements = false;
        int mLastElement = -1;
        int mWinHeight = 600;
        int mMaxX = 16;
        int mMaxY = 16;
        int mDeltaBaseX;
        int mDeltaBaseY;        
        int mRenderScale;
        int mNewBrushX=1;
        int mNewBrushY=1;
        int mSelectedBrush=0;
        int mBrushOffset = 0;
        bool bCircleInvert = false;
        int mCircleSize = 0;
        bool bUseTileOffset = false;
        int setBrushDeltas(int nDeltaX, int nDeltaY, int *nDeltaScale, int nRenderScale, TextureParameters *mTexParam);
   		std::vector<SDL_Rect> BrushAreas;
        int init(std::string cTitle, std::string cType, int cBrushType, bool *cIsShown, int nDeltaX, int nDeltaY, int *cDeltaScale, int cRenderScale, TBrush **cCurrentBrush, TextureParameters *mTexParam);
        TBrush **mCurrentBrush;
        TBrush* getBrush();
        TBrush* getNextBrush();
        void swapBrushElements(int eVal1, int eVal2);
        void removeBrushElement(int cDropTile);
        int addBrush(int sizex, int sizey);
        int addBrushElement(int element);
        int removeBrush();        
        int renderIm();
        int saveToFile(std::string cBrushPath);
        int loadFromFile(std::string cBrushPath);
        void close();
        void closeEdit();
        int MinScale = 4;
        int MaxScale = 16;
};

//class TOverlay;

class TSelectionEditor{
    public:
        TSelection* mCurrentSelection;
        TSelection mSelection;
        int mSelectionWidth = 0;
        int mSelectionHeight = 0;
        TOverlay mOverlay;
        void initoverlay();
        void setoverlay();
        std::vector<SDL_Rect> EditPixelAreas;	        
        std::vector<SDL_Rect> TilesAreas;
        bool bRenderOverlay = false;
        int renderEd(int xpos, int ypos);
        int setSelection(TSelection* cNewSelection, int nWidth, int nHeight);
        void resizeEdit();
        int getXY(int xpos, int ypos, int cxpos, int cypos);
        bool bUpdateEditSelection = false;
		bool bUpdateEditSelectionScale = false;
        //int mSelEdWidth=4;
		int mCurEdScale=10;
		int mSelectionAreaX;
		int mSelectionAreaY;
        int mPixelScale = 1;
        const static int MaxScale = 24;
        const static int MinScale = 4;
        TEActionUndoStack mActionStack;
};

class TClipboard : public TBrushList{
    public:
        TClipboard(){mLocalTexParam.TexPixelSize-=12;MinScale = 2;};
        TBrush* createClipTile(Tile* cNewTile);
        TBrush* createClipMap(TileMap* cNewMap);
        TBrush* createClipTileSet(TileSet* cNewSet);
        TBrush* getLastClip();
        TBrush* getPrevClip();
        int renderIm();        
};

#endif