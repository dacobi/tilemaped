#include "TSettings.h"
#include "TTFTexture.h"
#include "TOverlay.h"

extern TSettings mGlobalSettings;
        
void TOverlayText::init(){
    addNumbers(255);

    addText("X");
    addText("Y");    
    addText("XY");    

    addText("C");
}

void TOverlayText::setColor(int cnewcol){

    if( (cnewcol < 0) || (cnewcol >= MOVERLAYMAXCOL)){
        cnewcol = 1;
    }

    mGlobalSettings.OverlayTextColor = mColors[cnewcol];

    mGlobalSettings.mProjectSettings.Editor_OverlayTextColor->ivalue = cnewcol;

    reloadColors();
}

void TOverlayText::nextColor(){
    mGlobalSettings.mProjectSettings.Editor_OverlayTextColor->ivalue++;

    if(mGlobalSettings.mProjectSettings.Editor_OverlayTextColor->ivalue >= MOVERLAYMAXCOL){
        mGlobalSettings.mProjectSettings.Editor_OverlayTextColor->ivalue = 0;
    }

    setColor(mGlobalSettings.mProjectSettings.Editor_OverlayTextColor->ivalue);
}

void TOverlayText::reloadColors(){
    std::stringstream conv;
    std::string snum;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    for(int ci = 0; ci <= mMaxNum; ci++){
        conv << ci << std::endl;
        conv >> snum;

        mNumbers[ci]->loadTTFFromString(snum, mGlobalSettings.OverlayTextColor);
    }

    auto ti = mText.begin();

    while(ti != mText.end()){
        ti->second->loadTTFFromString(ti->first, mGlobalSettings.OverlayTextColor);
        ti++;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    
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

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    for(int ni = cMax; ni <= nmax; ni++){
        addNumber(ni);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
}

void TOverlayText::addText(std::string ntext){

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    TTFTexture* newText = new TTFTexture;
        
    newText->loadTTFFromString(ntext, mGlobalSettings.OverlayTextColor);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

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
        mText[ctext]->render(cx, cy);
    } else {
        cText->render(cx, cy);
    }    
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

    if((*mGrid)[0].w  < mScale){
        return;
    }

    for(int ii = 0; ii < mGridY; ii++){
        for(int jj = 0; jj < mGridX; jj++){
            mIndex = (ii * mGridX) + jj;

            if(mIndex < (*mGrid).size()){

                mX = (*mGrid)[mIndex].x + 3;
                mY = (*mGrid)[mIndex].y + 3;

                if( (mX >= 0) && (mY >= 0) && (mX <= mGlobalSettings.WindowWidth) && (mY <= mGlobalSettings.WindowHeight) ){
                    mRender();
                }
            }
        }
    }
}