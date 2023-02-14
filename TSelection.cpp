#include "TSelection.h"
#include "TEditor.h"

extern TSettings mGlobalSettings;


int TSelection::findInSelection(int item){
    std::vector<int>::iterator it;
	
	if((it = std::find(mSelected.begin(), mSelected.end(), item)) != mSelected.end()){
        return it - mSelected.begin();
	}

    return -1;
}

int TSelection::addToSelection(int item){
    mSelected.push_back(item);
    std::cout << "Add To Selection: " << item << std::endl;
    return mSelected.size()-1;
}

int TSelection::removeFromSelection(int item){
    int tSel = findInSelection(item);
    if(tSel != -1){
        mSelected.erase(mSelected.begin() + tSel);
        return 0;
    }
    return 1;
}

int TSelection::modifySelection(int item){
    if(findInSelection(item) != -1){
        removeFromSelection(item);
        return 1;
    } else {        
        addToSelection(item);
        return 0;
    }
}

void TSelection::clearSelection(){
    mSelected.erase(mSelected.begin(), mSelected.end());
}

int TSelection::confirmSelection(){
    bIsSelecting = false;
    bHasSelection = true;
    std::cout << "Got Selection" << std::endl;
    getSelection(mGlobalSettings.CurrentEditor->mTileMap.TileAreas, mCurSelection, mGlobalSettings.TileSizeX * mGlobalSettings.TileMapScale,  mGlobalSettings.TileSizeY * mGlobalSettings.TileMapScale);
}

int TSelection::searchSelection(std::vector<SDL_Rect> &sRects, int mx, int my){
    for(int i = 0; i < sRects.size(); i++){
		if( (mx >= sRects[i].x) && (mx <= (sRects[i].x + sRects[i].w)) && (my >= sRects[i].y) && (my <= (sRects[i].y + sRects[i].h)) ){
			return i;
		}
	}
	return -1;
}

int TSelection::getSelection(std::vector<SDL_Rect> &sRects, SDL_Rect &cSel, int xdelta, int ydelta){
    int wStep = cSel.w / xdelta;
    int hStep = cSel.h / ydelta;

    int cStepX = 0;
    int cStepY = 0;

    int item = -1;

    for(int i = 0; i <= hStep; i++){
        for(int j = 0; j <= wStep; j++){
            std::cout << "Getting Selection: " << cStepX << "," << cStepY << std::endl;
            if((item = searchSelection(sRects, cSel.x + cStepX, cSel.y + cStepY)) != -1){
                addToSelection(item);
            }            
            cStepX += xdelta;
        }
        cStepX = 0;
        cStepY += ydelta;       
    }
}

int TSelection::renderSelection(){
    if(bIsSelecting){
		SDL_SetRenderDrawColor(mGlobalSettings.TRenderer,mGlobalSettings.DefaultHighlightColor.r,mGlobalSettings.DefaultHighlightColor.g,mGlobalSettings.DefaultHighlightColor.b, 0xff);
        SDL_RenderDrawRect(mGlobalSettings.TRenderer, &mCurSelection);
        return 0;
    }
    return 1;
}

int TSelection::startSelection(int mx, int my){
    clearSelection();
    mCurSelection.x = mx;
    mCurSelection.y = my;
    mCurSelection.w = 0;
    mCurSelection.h = 0;
    bIsSelecting = true;
    return 0;
}


int TSelection::updateSelection(int rx, int ry){
    mCurSelection.w = rx - mCurSelection.x;
    mCurSelection.h = ry - mCurSelection.y;
    return 0;
}

void TSelection::cancelSelection(){
    bIsSelecting = false;
    clearSelection();
}