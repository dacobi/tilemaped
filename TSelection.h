#ifndef __TSELECTION__
#define __TSELECTION__

#include "TSettings.h"

class TSelection{
    public:
        std::vector<int> mSelected;
        SDL_Rect mCurSelection;
        bool bIsSelecting = false;
        bool bHasSelection = false;
        int startSelection(int mx, int my);
        int updateSelection(int rx, int ry);
        int confirmSelection();
        void cancelSelection();
        int renderSelection();
        int findInSelection(int item);
        int addToSelection(int item);
        int removeFromSelection(int item);
        int modifySelection(int item);
        void clearSelection();
   		int searchSelection(std::vector<SDL_Rect> &sRects, int mx, int my);
        int getSelection(std::vector<SDL_Rect> &sRects, SDL_Rect &cSel, int xdelta, int ydelta);
};

#endif