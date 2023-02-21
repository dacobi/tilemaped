#include "TSelection.h"
#include "TEditor.h"

extern TSettings mGlobalSettings;

bool TSelection::isInSelection(int item){
    if(std::find(mSelected.begin(), mSelected.end(), item) != mSelected.end()){
        return true;
    }
    return false;
}

int TSelection::findInSelection(int item){    	
    for(int i = 0; i < mSelected.size(); i++){
        if(item == mSelected[i]){
            return i;
        }
    }
    return -1;
}

int TSelection::addToSelection(int item){
    mSelected.push_back(item);
    
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
        calcSelectionBorder();
        return 1;
    } else {        
        addToSelection(item);
        calcSelectionBorder();
        return 0;
    }
}

int TSelection::init(int cAreaX, int cAreaY, int cWidth, int cHeight, int *cScale){
    mAreaX = cAreaX;
    mAreaY = cAreaY;
    mWidth = cWidth;
    mHeight = cHeight;
    mScale = cScale;

    mSelectionBorder.resize(mAreaX*mAreaY, false);
    mSelectionEdges.resize(mAreaX*mAreaY, 0);

    return 0;
}

void TSelection::clearSelection(){
    mSelected.erase(mSelected.begin(), mSelected.end());    
    calcSelectionBorder();
}

int TSelection::confirmSelection(std::vector<SDL_Rect> &sRects, int xdelta, int ydelta){
    bIsSelecting = false;
    bHasSelection = true;
    if(mCurSelection.w < 0){
        mCurSelection.w = -mCurSelection.w;
        mCurSelection.x -= mCurSelection.w;
    }
    if(mCurSelection.h < 0){
        mCurSelection.h = -mCurSelection.h;
        mCurSelection.y -= mCurSelection.h;
    }
    
    if(mGlobalSettings.mSelectionMode == 0){
        clearSelection();
    }
    getSelection(sRects, mCurSelection, xdelta, ydelta);    
    calcSelectionBorder();
    return 0;
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
            if((item = searchSelection(sRects, cSel.x + cStepX, cSel.y + cStepY)) != -1){
                if(findInSelection(item) == -1)  addToSelection(item);                
            }            
            cStepX += xdelta;
        }
        cStepX = 0;
        cStepY += ydelta;       
    }
    return 0;
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

int TSelection::invertSelection(int sstart, int send){
   
    std::vector<int> mSelectedOld;
    std::vector<bool> mSelectedBool;
    mSelectedBool.resize(mAreaX*mAreaY, true);

    mSelectedOld = mSelected;

    mSelected.clear();

    for(int i = 0; i < mSelectedOld.size(); i++){
        mSelectedBool[mSelectedOld[i]] = false;
    }

    for(int i = sstart; i < send; i++){        
        if(mSelectedBool[i]) mSelected.push_back(i);
    }

    calcSelectionBorder();
    return 0;
}

int TSelection::selectRange(int sstart, int send){
    clearSelection();
    for(int i = sstart; i < send; i++){        
        addToSelection(i);        
    }
    calcSelectionBorder();
    return 0;
}

int TSelection::renderSelection(int xpos, int ypos){	
	SDL_SetRenderDrawColor(mGlobalSettings.TRenderer,mGlobalSettings.DefaultHighlightColor.r,mGlobalSettings.DefaultHighlightColor.g,mGlobalSettings.DefaultHighlightColor.b, 0xff);
	for(int i=0; i < mAreaY; i++){
		for(int j=0; j < mAreaX; j++){
			if(mSelectionEdges[getXY(j,i)]){
				unsigned char edges = mSelectionEdges[getXY(j,i)];
				int xstart, xend, ystart, yend;
				xstart = xpos + (mWidth * j * *mScale);
				ystart = ypos + (mHeight * i * *mScale);
				xend = xstart + (mWidth * *mScale)-1;
				yend = ystart + (mHeight * *mScale)-1;
				if(edges & 0x1){
					SDL_RenderDrawLine(mGlobalSettings.TRenderer, xstart, ystart, xend, ystart);
                    SDL_RenderDrawLine(mGlobalSettings.TRenderer, xstart, ystart-1, xend, ystart-1);
                    SDL_RenderDrawLine(mGlobalSettings.TRenderer, xstart, ystart+1, xend, ystart+1);
				}
				if(edges & 0x2){
					SDL_RenderDrawLine(mGlobalSettings.TRenderer, xend, ystart, xend, yend);
                    SDL_RenderDrawLine(mGlobalSettings.TRenderer, xend+1, ystart, xend+1, yend);
                    SDL_RenderDrawLine(mGlobalSettings.TRenderer, xend-1, ystart, xend-1, yend);
				}
				if(edges & 0x4){
					SDL_RenderDrawLine(mGlobalSettings.TRenderer, xstart, yend, xend, yend);
                    SDL_RenderDrawLine(mGlobalSettings.TRenderer, xstart, yend+1, xend, yend+1);
                    SDL_RenderDrawLine(mGlobalSettings.TRenderer, xstart, yend-1, xend, yend-1);
				}
				if(edges & 0x8){
					SDL_RenderDrawLine(mGlobalSettings.TRenderer, xstart, ystart, xstart, yend);
                    SDL_RenderDrawLine(mGlobalSettings.TRenderer, xstart-1, ystart, xstart-1, yend);
                    SDL_RenderDrawLine(mGlobalSettings.TRenderer, xstart+1, ystart, xstart+1, yend);
				}
			}
		}
	}
    return 0;
}

int TSelection::getXY(int X,int Y){
    int index = X + (mAreaX * (Y));
    if(index < (mAreaX * mAreaY)){
        return index;
    }    
    return 0;
}

int TSelection::calcSelectionBorder(){	
    
		if(mSelected.size() == 0){
			bHasSelection = false;
			return 0;
		}

		bHasSelection = true;
    
		for(int i=0; i < mAreaY; i++){
			for(int j=0; j < mAreaX; j++){
				if(isInSelection((getXY(j,i)))){
					mSelectionBorder[(getXY(j,i))] = true;
				} else {
					mSelectionBorder[(getXY(j,i))] = false;
				}
				mSelectionEdges[(getXY(j,i))] = 0;
			}
		}
				
		for(int yy=0; yy < mAreaY; yy++){
			for(int xx=0; xx < mAreaX; xx++){    
				if(mSelectionBorder[getXY(xx,yy)]){					
					if(yy==0){
                            mSelectionEdges[getXY(xx,yy)] += 1;
                        } else {
						    if(!mSelectionBorder[getXY(xx,yy-1)]){
							    mSelectionEdges[getXY(xx,yy)] += 1;
						    }
					} 
					if(xx==0){
                            mSelectionEdges[getXY(xx,yy)] += 8;
                        } else {
						    if(!mSelectionBorder[getXY(xx-1,yy)]){
						        mSelectionEdges[getXY(xx,yy)] += 8;
						    }
                        
					}
					if(yy==(mAreaY-1)){
                        mSelectionEdges[getXY(xx,yy)] += 4;
                        } else {
						    if(!mSelectionBorder[getXY(xx,yy+1)]){
							    mSelectionEdges[getXY(xx,yy)] += 4;
						    }                        
					}
					if(xx==(mAreaX-1)){                        
                                mSelectionEdges[getXY(xx,yy)] += 2;
                        } else {
						    if(!mSelectionBorder[getXY(xx+1,yy)]){                        
							    mSelectionEdges[getXY(xx,yy)] += 2;
						    }
                        
					}					
				}
			}
		}
    return 0;
}


int TBrush::configBrush(int nWidth, int nHeight, int bType, int nRenderScale){
    mBrushWidth = nWidth;
    mBrushHeight = nHeight;
    mRenderScale = nRenderScale;
    mBrushType = bType;
    if(bType == TBRUSH_TILE){
        setBrushDeltas(mGlobalSettings.TileSizeX, mGlobalSettings.TileSizeY, &mGlobalSettings.TileMapScale, &mGlobalSettings.TileMapScale);
    } else if (bType == TBRUSH_PIXEL) {
        setBrushDeltas(mGlobalSettings.TilePixelSize, mGlobalSettings.TilePixelSize, &mGlobalSettings.mTileEdScale, &mGlobalSettings.mTileEdScale);
    } else {
        std::cout << "Error Creating Brush!" << std::endl;
        return 1;
    } 

    UUID = rand();

    mBrushElements.resize(mBrushWidth*mBrushHeight, -1);
    mElementProps.resize(mBrushWidth*mBrushHeight);


    return 0;
}

int TBrush::flipElementV(){
    mElementProps[mCursorPos].bFlipX = !mElementProps[mCursorPos].bFlipX;
    return 0;
}

int TBrush::flipElementH(){
    mElementProps[mCursorPos].bFlipY = !mElementProps[mCursorPos].bFlipY;
    return 0;
}

int TBrush::setBrushDeltas(int nDeltaX, int nDeltaY, int *nScaleX,int *nScaleY){
    mDeltaBaseX = nDeltaX;
    mDeltaBaseY = nDeltaY;
    mDeltaScaleX = nScaleX;
    mDeltaScaleY = nScaleY;
    return 0;
}
int TBrush::getBrushSelection(int bx, int by, std::vector<SDL_Rect> &sRects){

    mLastClickX = bx;
    mLastClickY = by;

    clearSelection();

    int cDeltaX = mDeltaBaseX * *mDeltaScaleX;
    int cDeltaY = mDeltaBaseY * *mDeltaScaleY;

    int cStepX = 0;
    int cStepY = 0;

    int item;

    for(int i = 0; i < mBrushHeight; i++){
        for(int j = 0; j < mBrushWidth; j++){            
            item = searchSelection(sRects, bx + cStepX, by + cStepY);
            if(item > -1){
                if(findInSelection(item) == -1){
                    addToSelection(item);
                } else {
                    addToSelection(-1);
                }
            } else {
                addToSelection(-1);
            }
            
            cStepX += cDeltaX;
        }
        cStepX = 0;
        cStepY += cDeltaY;
    }

    return 0;
}

int TBrush::renderSelection(){
    mCurSelection.x = mLastClickX - ((mDeltaBaseX * *mDeltaScaleX)/2);
    mCurSelection.y = mLastClickY - ((mDeltaBaseY * *mDeltaScaleY)/2);
    mCurSelection.w = mBrushWidth * mDeltaBaseX * *mDeltaScaleX;
    mCurSelection.h = mBrushHeight * mDeltaBaseY * *mDeltaScaleY;

    SDL_SetRenderDrawColor(mGlobalSettings.TRenderer,mGlobalSettings.DefaultHighlightColor.r,mGlobalSettings.DefaultHighlightColor.g,mGlobalSettings.DefaultHighlightColor.b, 0xff);
    SDL_RenderDrawRect(mGlobalSettings.TRenderer, &mCurSelection);

    return 0;
}

SDL_Rect TBrush::renderIm(int xpos, int ypos){
    SDL_Rect rRect;
    if(mBrushType == TBRUSH_TILE){
        rRect = renderTile(xpos, ypos);
    }
    if(mBrushType == TBRUSH_PIXEL){
        rRect = renderPixel(xpos, ypos);
    }
    return rRect;
}

SDL_Rect TBrush::renderPixel(int xpos, int ypos){
    ImDrawList *tList = ImGui::GetWindowDrawList();

    ImVec2 elmin;   
	ImVec2 elmax;

    ImVec2 cPos = ImGui::GetWindowPos();

    int cDeltaX = mDeltaBaseX * mRenderScale;
    int cDeltaY = mDeltaBaseY * mRenderScale;

    elmin.x = xpos + cPos.x;
    elmin.y = ypos + cPos.y;
    elmax.x = elmin.x  + cDeltaX * mBrushWidth;
    elmax.y = elmin.y  + cDeltaY * mBrushHeight;

    int cStepX = 0;
    int cStepY = 0;

    ImVec2 telmin;   
	ImVec2 telmax;

    telmin.x = elmin.x;
    telmin.y = elmin.y;

    telmax.x = elmin.x + cDeltaX;
    telmax.y = elmin.y + cDeltaY;

    for(int i=0; i < mBrushHeight; i++){
            for(int j=0; j < mBrushWidth; j++){
                if(mBrushElements[j+(i*mBrushWidth)] > -1){                    
                    tList->AddRectFilled(telmin, telmax, mGlobalSettings.CurrentEditor->mPalette.getImColor(mGlobalSettings.CurrentEditor->mPalette.TPalette[mBrushElements[j+(i*mBrushWidth)]]));
                }
                if(bIsEditing){
                    if((j+(i*mBrushWidth)) == mCursorPos){
                        tList->AddRect(telmin, telmax, mGlobalSettings.ImHighLightColor);
                    }
                }
                telmin.x += cDeltaX;
                telmax.x += cDeltaX;
            }
            telmin.x = elmin.x;
            telmax.x = elmin.x + cDeltaX;
            telmin.y += cDeltaY;
            telmax.y += cDeltaY;
    }

    tList->AddRect(elmin, elmax, mGlobalSettings.ImHighLightColor);

    if(bIsSelected) {
        tList->AddRect(elmin, elmax, mGlobalSettings.ImAltHighLightColor);
    }
    if(bIsEditing){
        tList->AddRect(ImVec2(elmin.x-1, elmin.y-1), ImVec2(elmax.x+2, elmax.y+2), mGlobalSettings.ImHighLightColor);
    } 
    
    mWinPos = elmin;

    mWinSize.x = cDeltaX * mBrushWidth;
    mWinSize.y = cDeltaY * mBrushHeight;

    SDL_Rect tmpRect;
    tmpRect.x = mWinPos.x;
    tmpRect.y = mWinPos.y;
    tmpRect.w = mWinSize.x;
    tmpRect.h = mWinSize.y;

    return tmpRect;
}

SDL_Rect TBrush::renderTile(int xpos, int ypos){

	ImDrawList *tList = ImGui::GetWindowDrawList();

    ImVec2 elmin;   
	ImVec2 elmax;

    ImVec2 cPos = ImGui::GetWindowPos();

    int cDeltaX = mDeltaBaseX * mRenderScale;
    int cDeltaY = mDeltaBaseY * mRenderScale;

    elmin.x = xpos + cPos.x;
    elmin.y = ypos + cPos.y;
    elmax.x = elmin.x  + cDeltaX * mBrushWidth;
    elmax.y = elmin.y  + cDeltaY * mBrushHeight;

    int cStepX = 0;
    int cStepY = 0;

    ImVec2 telmin;   
	ImVec2 telmax;

    ImVec2 edmin;   
	ImVec2 edmax;
    
    telmin.x = elmin.x;
    telmin.y = elmin.y;

    telmax.x = elmin.x + cDeltaX;
    telmax.y = elmin.y + cDeltaY;

    for(int i=0; i < mBrushHeight; i++){
            for(int j=0; j < mBrushWidth; j++){
                if(mBrushElements[j+(i*mBrushWidth)] > -1){
    
                    edmin = telmin;
                    edmax = telmax;

                    if(mElementProps[j+(i*mBrushWidth)].bFlipX){
                        int tmpInt;
                        tmpInt = edmin.x;
                        edmin.x = edmax.x;
                        edmax.x = tmpInt;
                    }

                    if(mElementProps[j+(i*mBrushWidth)].bFlipY){
                        int tmpInt;
                        tmpInt = edmin.y;
                        edmin.y = edmax.y;
                        edmax.y = tmpInt;
                    }

                    if(mGlobalSettings.TileSetBPP < 0x8){                        
                        tList->AddImage((ImTextureID)(intptr_t)mGlobalSettings.CurrentEditor->mTileSet.TTiles[mBrushElements[j+(i*mBrushWidth)]]->TPOffset[mGlobalSettings.PaletteOffset], edmin, edmax);
                    } else {
                        tList->AddImage((ImTextureID)(intptr_t)mGlobalSettings.CurrentEditor->mTileSet.TTiles[mBrushElements[j+(i*mBrushWidth)]]->TileTex, edmin, edmax);                        
                        
                    }
                }
                if(bIsEditing){
                    if((j+(i*mBrushWidth)) == mCursorPos){
                        tList->AddRect(telmin, telmax, mGlobalSettings.ImHighLightColor);
                    }
                }
                telmin.x += cDeltaX;
                telmax.x += cDeltaX;
            }
            telmin.x = elmin.x;
            telmax.x = elmin.x + cDeltaX;
            telmin.y += cDeltaY;
            telmax.y += cDeltaY;
    }

    tList->AddRect(elmin, elmax, mGlobalSettings.ImHighLightColor);

    if(bIsSelected) {
        tList->AddRect(elmin, elmax, mGlobalSettings.ImAltHighLightColor);
    }
    if(bIsEditing){
        tList->AddRect(ImVec2(elmin.x-1, elmin.y-1), ImVec2(elmax.x+2, elmax.y+2), mGlobalSettings.ImHighLightColor);
    } 
    
    mWinPos = elmin;

    mWinSize.x = cDeltaX * mBrushWidth;
    mWinSize.y = cDeltaY * mBrushHeight;

    SDL_Rect tmpRect;
    tmpRect.x = mWinPos.x;
    tmpRect.y = mWinPos.y;
    tmpRect.w = mWinSize.x;
    tmpRect.h = mWinSize.y;

    return tmpRect;
}

int TBrush::writeToFile(std::ofstream &outfile){
    std::string tmpStr, tmpStr2,tmpStr3;
    std::stringstream convert;
    tmpStr = "brush ";
    convert << mBrushWidth << std::endl;
    convert >> tmpStr2;
    tmpStr += tmpStr2;
    tmpStr += " ";
    convert << mBrushHeight << std::endl;
    convert >> tmpStr2;
    tmpStr += tmpStr2;
    outfile << tmpStr << std::endl;
    tmpStr = "elements";
    for(int i = 0; i < mBrushElements.size(); i++){
        convert << mBrushElements[i] << std::endl;
        convert >> tmpStr2;
        tmpStr += " ";
        tmpStr += tmpStr2;
        if(mBrushType == TBRUSH_TILE){
            convert << getElementFlip(i) << std::endl;
        }
        if(mBrushType == TBRUSH_PIXEL){
            convert << 0 << std::endl;
        }
        convert >> tmpStr2;
        tmpStr += " ";
        tmpStr += tmpStr2;
    }
    outfile << tmpStr << std::endl;
    return 0;
}

int TBrush::readFromFile(std::ifstream &infile){
    std::string tmpStr, tmpStr2;
    std::stringstream convert;
    int tmpInt, tmpInt2;
    std::getline(infile, tmpStr);
    convert << tmpStr << std::endl;
    convert >> tmpStr;
    if(tmpStr == "elements"){
        for(int i = 0; i < (mBrushWidth*mBrushHeight); i++){
            convert >> tmpInt;
            convert >> tmpInt2;
            setElementNext(tmpInt);
            if(mBrushType == TBRUSH_TILE){
                setElementFlip(i, tmpInt2);
            }
        }   
    } else {
        std::cout << "Error Reading Brush!" << std::endl;
        return 1; 
    }
    return 0;
}

int TBrushList::init(std::string cTitle, std::string cType, int cBrushType, bool *cIsShown, int cRenderScale, TBrush **cCurrentBrush){
    mTitle = cTitle;
    mType = cType;
    mBrushType = cBrushType;
    bIsShown = cIsShown;
    mRenderScale = cRenderScale;
    mCurrentBrush = cCurrentBrush;
    return 0;
}

int TBrushList::addBrush(int sizex, int sizey){

    TBrush* newBrush = new TBrush();

    mBrushes.push_back(newBrush);
   
    newBrush->configBrush(sizex, sizey, mBrushType, mRenderScale);

    BrushAreas.resize(mBrushes.size());

    return 0;
}

int TBrush::setElementNext(int element){
    mBrushElements[mCursorPos] = element;
    nextElement();
    return 0;
}

int TBrush::nextElement(){
    mCursorPos++;
    if(mCursorPos > (mBrushElements.size()-1)){
        mCursorPos = 0;
    }
    return 0;
}

int TBrush::prevElement(){
    mCursorPos--;
    if(mCursorPos < 0){
        mCursorPos = (mBrushElements.size()-1);
    }
    return 0;
}

int TBrush::nextLine(){
    if(mBrushHeight > 1){
        mCursorPos += mBrushWidth;
        if(mCursorPos > (mBrushElements.size()-1)){
            mCursorPos = mCursorPos % mBrushWidth;
        }
    }
    return 0;
}

int TBrush::prevLine(){
    if(mBrushHeight > 1){
        mCursorPos -= mBrushWidth;
        if(mCursorPos < 0){
            mCursorPos =  (mBrushElements.size()) + mCursorPos;
        }
    }
    return 0;
}


int TBrushList::addBrushElement(int element){
    if(bIsEditing){
        mBrushes[mSelectedBrush]->setElementNext(element);
    }
    return 0;
}

int TBrushList::removeBrush(){
    if(mBrushes.size()){
        mBrushes.erase(mBrushes.begin()+mSelectedBrush);
        BrushAreas.pop_back();
        if(mSelectedBrush > 0){
            mSelectedBrush--;
        }
    }
    return 0;
}

TileProperties TBrush::getElementProps(int element){
    TileProperties cProps;
    if(element > -1){
        cProps = mElementProps[element];
        cProps.mPaletteOffset = mGlobalSettings.PaletteOffset;
    }
    return cProps;
}

int TBrush::getElementFlip(int element){
    int cFlip = 0;
    if(element > -1){
        cFlip = mElementProps[element].bFlipY;
        cFlip = cFlip << 1;
        cFlip += mElementProps[element].bFlipX;        
    }
    return cFlip;
}

int TBrush::setElementFlip(int element, int cFlip){    
    if(element > -1){        
        mElementProps[element].bFlipX = (cFlip & 0x1);        
        mElementProps[element].bFlipY = (cFlip & 0x2);      
    }
    return 0;
}

TBrush* TBrushList::getBrush(){
    TBrush* cBrush = NULL;
    
    if(mBrushes.size()){
        cBrush = mBrushes[mSelectedBrush];
    }
    
    return cBrush;
}

TBrush* TBrushList::getNextBrush(){
    TBrush* cBrush = NULL;
    
    if(mBrushes.size()){
        mSelectedBrush++;
        if(mSelectedBrush > (mBrushes.size()-1)){
            mSelectedBrush = 0;
        }
    
        cBrush = mBrushes[mSelectedBrush];
    }

    return cBrush;
}

int TBrushList::renderIm(){

    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(600,800), ImGuiCond_Once);

    std::string cTitleType = "Brushes: " + mTitle;

    ImGui::Begin(cTitleType.c_str(), bIsShown);

    ImGui::SliderInt("Brush Width", &mNewBrushX, 1, 9);
    ImGui::SliderInt("Brush Height", &mNewBrushY, 1, 9);

    if(ImGui::Button("Add Brush")){
        addBrush(mNewBrushX, mNewBrushY);
        mSelectedBrush = mBrushes.size()-1;
        bIsEditing = true;
    }

    ImGui::SameLine();

    if(ImGui::Button("Remove Brush")){
        removeBrush();
        *mCurrentBrush = NULL;        
    }

    if(!bIsEditing){
        if(ImGui::Button("Brushes Edit")){
            if(mBrushes.size()){
                bIsEditing = true;
                mBrushes[mSelectedBrush]->mCursorPos = 0;
            }
        }
    }

    if(bIsEditing){

        if(ImGui::Button("Brushes Done")){
            bIsEditing = false;
            mBrushes[mSelectedBrush]->bIsEditing = false;
        }        

        if(ImGui::Button("Reset Cursor")){
            mBrushes[mSelectedBrush]->mCursorPos = 0;
        }        

        ImGui::SameLine();

        if(ImGui::Button("Empty Element")){
            mBrushes[mSelectedBrush]->setElementNext(-1);
        }

        if(ImGui::Button("Next Element")){
            mBrushes[mSelectedBrush]->nextElement();
        }

        ImGui::SameLine();

        if(ImGui::Button("Prev Element")){
            mBrushes[mSelectedBrush]->prevElement();
        }

        if(ImGui::Button("Next Line")){
            mBrushes[mSelectedBrush]->nextLine();
        }

        ImGui::SameLine();

        if(ImGui::Button("Prev Line")){
            mBrushes[mSelectedBrush]->prevLine();
        }

        if(ImGui::Button("Flip H")){
            mBrushes[mSelectedBrush]->flipElementH();
        }

        ImGui::SameLine();

        if(ImGui::Button("Flip V")){
            mBrushes[mSelectedBrush]->flipElementV();
        }

    }

    if(mBrushes.size()){
    
        int tmpWinH=150;

        for(auto cbrushs: BrushAreas){
            tmpWinH += cbrushs.h + 10;
        }

        if(tmpWinH > 800){
            mWinHeight = tmpWinH;
            ImGui::SetNextWindowContentSize(ImVec2(600,mWinHeight + 20));
        } else if ((tmpWinH < 750) && (mWinHeight > 800)){
            mWinHeight = 800;
            ImGui::SetNextWindowContentSize(ImVec2(600,mWinHeight));
        }
    }

    ImGui::BeginChild("Brush List");

    ImVec2 tSubWinPos = ImGui::GetWindowPos();

    mBrushOffset = tSubWinPos.y;

    int yoffset = 20 - ImGui::GetScrollY();

    for(int i=0;i < mBrushes.size(); i++){
        if(mSelectedBrush == i){
            if(bIsEditing){ mBrushes[i]->bIsEditing = true;} 
            mBrushes[i]->bIsSelected = true;
        } else {
            mBrushes[i]->bIsSelected = false;    
            mBrushes[i]->bIsEditing = false;    
        }

        BrushAreas[i] = mBrushes[i]->renderIm(20, yoffset);
        yoffset+=mBrushes[i]->mWinSize.y+=10;
    }

    ImGui::EndChild();

    mGlobalSettings.CurrentEditor->ImButtonsBrushes.updateButtonStates();

    ImGui::End();

    return 0;
}

int TBrushList::saveToFile(std::string cBrushPath){

    if(mBrushes.size() == 0){
        return 0;
    }

    std::string tmpStr,tmpStr2;
    std::ofstream output(cBrushPath, std::ios::out );
    std::stringstream convert;

    tmpStr = "brushes ";
    convert << mBrushes.size() << std::endl;
    convert >> tmpStr2;
     
    tmpStr += tmpStr2;

    output << tmpStr << std::endl;

    for(int i = 0; i < mBrushes.size(); i++){
        mBrushes[i]->writeToFile(output);
    }

    output.close();
    
    return 0;
}

int TBrushList::loadFromFile(std::string cBrushPath){

    std::string tmpStr,tmpStr2;
    int tmpInt,tmpInt2,tmpInt3;
    std::ifstream input(cBrushPath, std::ios::in );
    std::stringstream convert;

    std::getline(input, tmpStr);

    convert << tmpStr << std::endl;

    convert >> tmpStr;
    convert >> tmpInt;

    if(tmpStr == "brushes"){
        for(int i = 0; i < tmpInt; i++){
            std::getline(input, tmpStr);
            convert << tmpStr << std::endl;
            convert >> tmpStr;
            convert >> tmpInt2;
            convert >> tmpInt3;
            if(tmpStr == "brush"){
                addBrush(tmpInt2, tmpInt3);
                if(mBrushes[i]->readFromFile(input)){
                    std::cout << "Error Loading Brushes: " << cBrushPath << std::endl;                    
                    return 1;            
                }
            } else {
                std::cout << "Error Loading Brushes: " << cBrushPath << std::endl;
                return 1;            
            }
        }
    } else {
        std::cout << "Error Loading Brushes: " << cBrushPath << std::endl;
        return 1;
    }

    return 0;
}