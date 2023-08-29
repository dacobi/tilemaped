#include "TSettings.h"
#include "TTFTexture.h"
#include "TOverlay.h"

extern TSettings mGlobalSettings;
        
void TOverlayText::init(){
    addNumbers(255);

    addText("X");
    addText("Y");    
    addText("XY");    
}

void TOverlayText::addNumber(int nnum){
    std::stringstream conv;

    TTFTexture* newNum = new TTFTexture;
        
    conv << nnum << std::endl;
    std::string cnum;
    conv >> cnum;

    newNum->loadTTFFromString(cnum, mGlobalSettings.OverlayTextColor);

    mNumbers.push_back(newNum);

    mMaxNum = mNumbers.size() - 1;
}

void TOverlayText::addNumbers(int nmax){

    int cMax = mMaxNum + 1;

    for(int ni = cMax; ni <= nmax; ni++){
        addNumber(ni);
    }
}

void TOverlayText::addText(std::string ntext){

    TTFTexture* newText = new TTFTexture;
        
    newText->loadTTFFromString(ntext, mGlobalSettings.OverlayTextColor);

    mText[ntext] = newText;
}

void TOverlayText::renderNum(int cnum, int cx, int cy){

    if(cnum > mMaxNum){
        addNumbers(cnum);    
    }

    mNumbers[cnum]->render(cx, cy);
}

void TOverlayText::renderText(std::string ctext, int cx, int cy){

    TTFTexture* cText = mText[ctext];

    if(cText == NULL){
        addText(ctext);        
    }

    mText[ctext]->render(cx, cy);
}

void TOverlay::setGrid(int gx, int gy){
    mGridX = gx;
    mGridY = gy;    
}

void TOverlay::setSize(int sx, int sy){
    mSizeX = sx;
    mSizeY = sy;    
}       

void TOverlay::setScale(int mscale){
    mScale = mscale;
}

void TOverlay::setRects(std::vector<SDL_Rect> *cGrid){
    mGrid = cGrid;
}
        
void TOverlay::render(){
    for(int ii = 0; ii < mGridY; ii++){
        for(int jj = 0; jj < mGridX; jj++){
            mIndex = (ii * mGridX) + jj;
            mX = (*mGrid)[mIndex].x;
            mY = (*mGrid)[mIndex].y;

            if( (mX >= 0) && (mY >= 0) && (mX <= mGlobalSettings.WindowWidth) && (mY <= mGlobalSettings.WindowHeight) ){
                mRender();
            }           
        }
    }
}