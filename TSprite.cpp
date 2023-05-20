#include "TSettings.h"
#include "TSprite.h"
#include "TEditor.h"

void TSFrame::renderEd(int xpos, int ypos, TPalette* tpal){
	for(int i=0; i < mTexParam->TileSizeY; i++){
		for(int j=0; j < mTexParam->TileSizeX; j++){
			PixelAreas[j+(mTexParam->TileSizeX*i)] = tpal->renderSpriteEd(xpos + (mTexParam->TilePixelSize * mTexParam->mTileEdScale)*j, ypos + (mTexParam->TilePixelSize * mTexParam->mTileEdScale)*i, getPixel(j+(i*mTexParam->TileSizeX)), mTexParam); 			
		}
	}
	
	if(mGlobalSettings.CurrentEditor->mSprite->mCurrentBrushPixel && !mGlobalSettings.CurrentEditor->mBrushesPixel.bIsEditing){
		if(mGlobalSettings.CurrentEditor->mSprite->mCurrentBrushPixel->mSelected.size()){
			for(int i=0; i < mTexParam->TileSizeY; i++){
				for(int j=0; j < mTexParam->TileSizeX; j++){	
					if(mGlobalSettings.CurrentEditor->mSprite->mCurrentBrushPixel->findInSelection((j+(i*mTexParam->TileSizeX))) != -1){
						int findex = mGlobalSettings.CurrentEditor->mSprite->mCurrentBrushPixel->findInSelection((j+(i*mTexParam->TileSizeX)));
						if(mGlobalSettings.CurrentEditor->mSprite->mCurrentBrushPixel->mBrushElements[findex] != -1){
							tpal->renderSpriteEd(xpos + (mTexParam->TilePixelSize * mTexParam->mTileEdScale)*j, ypos + (mTexParam->TilePixelSize * mTexParam->mTileEdScale)*i, mGlobalSettings.CurrentEditor->mSprite->mCurrentBrushPixel->mBrushElements[findex], mTexParam); 							
						}
					}
				}
			}
		}
	}
	
	if(mSelection.bHasSelection){
		mSelection.renderSelection(xpos, ypos);
	}

}

TSFrame* TSprite::createNewFromBuffer(std::vector<unsigned char> &newBuf, TPalette* tpal){

	TSFrame *newTile = new TSFrame(&mTexParam);
	SDL_Rect newRect;

	newTile->loadFromBuffer(newBuf, tpal);
	FrameAreas.push_back(newRect);
	mFrames.push_back(newTile);
	
	return newTile;

}

TSFrame* TSprite::createNewCopy(TSFrame* cCopyTile, TPalette* tpal){	
	return createNewFromBuffer(cCopyTile->FileData, tpal);
}

TSFrame* TSprite::createNew(TPalette* tpal){	
	std::vector<unsigned char> tbuf;	
	tbuf.resize(((mTexParam.TileSizeX * mTexParam.TileSizeY)/mGlobalSettings.mTileBPPSize[mTexParam.TileSetBPP]),0);
	return createNewFromBuffer(tbuf, tpal);
}


TSFrame* TSprite::createFrame(TPalette* tpal){
    TSFrame* newFrame = new TSFrame(&mTexParam);

    newFrame->createNew(tpal);

    mFrames.push_back(newFrame);
    mFrame = newFrame;

    FrameAreas.resize(mFrames.size());

    selectFrame(mFrames.size()-1);   	 		
    
    return newFrame;
}

void TSprite::selectFrame(int cFrame){
    for(auto *sFrame : mFrames){
        sFrame->bIsSelected = false;        
    }
    mFrame = mFrames[cFrame];
    mFrame->bIsSelected = true;
    mSelectedFrame = cFrame;     
}

int TSprite::removeFrame(int cDropFrame){
    TSFrame* dFrame = *(mFrames.begin() +  cDropFrame); 
	mFrames.erase(mFrames.begin() +  cDropFrame);
	FrameAreas.erase(FrameAreas.begin() + cDropFrame);	
    return 0;
}

void TSprite::appendFrame(TSFrame* addFrame){
    mFrames.push_back(addFrame);
	SDL_Rect newRect;
	FrameAreas.push_back(newRect);
}


std::string TSprite::getSpriteSize(){
	std::string cSpriteSize;
	std::stringstream convert;
	std::string cSpriteWidth;
	std::string cSpriteHeight;
    std::string cSpriteNum;

	convert << mTexParam.TileSizeX << std::endl;
	convert >> cSpriteWidth;

	convert << mTexParam.TileSizeY << std::endl;
	convert >> cSpriteHeight;

    convert << mFrames.size() << std::endl;
	convert >> cSpriteNum;
	
	cSpriteSize = cSpriteWidth + "x" + cSpriteHeight + "x" + cSpriteNum;

	return cSpriteSize;
}


void TSprite::renderEd(int xpos, int ypos, TPalette* tpal){
    if(mFrame){
        mFrame->renderEd(xpos, ypos, tpal);
    }
}

void TSprite::close(){
	for(auto *dTile: mFrames){
		dTile->freeTexture();
	}
	mFrames.clear();
}


/*
void TSprite::resizeEdit(){
	int isOdd = TTiles.size() % mSelEdWidth;
	int cRowNum = TTiles.size() / mSelEdWidth;

	int cPad = 0;

	int dummy = 1;

	if(isOdd){cPad = 1;}

	SDL_Rect rEmpty;

	rEmpty.x = 0;
	rEmpty.y = 0;
	rEmpty.w = 0;
	rEmpty.h = 0;

	mSelectionAreaX = mSelEdWidth * mGlobalSettings.mGlobalTexParam.TileSizeX;
	mSelectionAreaY = mGlobalSettings.mGlobalTexParam.TileSizeY * (cRowNum + cPad);
	
	EditPixelAreas.resize(mSelectionAreaX*mSelectionAreaY);

	for(int i = 0; i <  EditPixelAreas.size(); i++){
		EditPixelAreas[i].x = rEmpty.x;
		EditPixelAreas[i].y = rEmpty.y;
		EditPixelAreas[i].w = rEmpty.w;
		EditPixelAreas[i].h = rEmpty.h;
	}
	
	mSelection.clearSelection();	
	mSelection.init(mSelectionAreaX , mSelectionAreaY, 1, 1, &mCurEdScale);	
}
*/

void TSprite::renderIm(int ypos, int mScroll){
    mFramesBackGround.h = mGlobalSettings.WindowHeight- mGlobalSettings.TopBarHeight;
	
	if(mCurColumns < 4){
		if( (int)( (float)( ( ( (mCurFrameScale*mTexParam.TileSizeX ) +mColSpace ) * mFrames.size() )  / mCurColumns ) ) > mFramesBackGround.h ){	
			mCurFrameScale--;
            if(mCurFrameScale == 0){
                mCurFrameScale = 1;
            }
		
			if(mCurFrameScale < 5){
				mCurColumns++;
				updateWinPos = true;
			}			
		}
	}

	mGlobalSettings.SpriteWidth = (((mCurFrameScale*mTexParam.TileSizeX)+mColSpace)*mCurColumns)+(mColSpace*3);

	int isOdd = mFrames.size() % mCurColumns;
	int cRowNum = mFrames.size() / mCurColumns;

	mFramesBackGround.x = mGlobalSettings.WindowWidth - mGlobalSettings.SpriteWidth;
	mFramesBackGround.y = ypos;
	mFramesBackGround.w = mGlobalSettings.SpriteWidth; 

	ImVec2 cWinPos;
	cWinPos.x = mFramesBackGround.x;
	cWinPos.y = mFramesBackGround.y;
	

	ImVec2 cWinSize;
	cWinSize.x = mFramesBackGround.w;
	cWinSize.y = mFramesBackGround.h;

	if(updateWinPos){
		ImGui::SetNextWindowPos(cWinPos, ImGuiCond_Always);
		ImGui::SetNextWindowSize(cWinSize, ImGuiCond_Always);
		updateWinPos = false;
	} else {
		ImGui::SetNextWindowPos(cWinPos, ImGuiCond_Once);
		ImGui::SetNextWindowSize(cWinSize, ImGuiCond_Once);
	}

	ImGui::Begin("Sprite Frames", &bShowSpriteFrames, ImGuiWindowFlags_NoNav);    

	if(ImGui::Button("Move Up")){
		mGlobalSettings.CurrentEditor->moveFrameUp();
	}

	if(ImGui::Button("Move Down")){
		mGlobalSettings.CurrentEditor->moveFrameDown();
	}

	bool bIsDragged = false;
	int mDragged = -1;
	int mDragSource = 0;
	int mDragTarget = 0;

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(mGlobalSettings.DefaultBGColor.r,mGlobalSettings.DefaultBGColor.g,mGlobalSettings.DefaultBGColor.b)));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(mGlobalSettings.DefaultBGColor.r,mGlobalSettings.DefaultBGColor.g,mGlobalSettings.DefaultBGColor.b)));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(mGlobalSettings.DefaultBGColor.r,mGlobalSettings.DefaultBGColor.g,mGlobalSettings.DefaultBGColor.b)));
	
	if(mCurColumns > 0){
		for(int i = 0; i < cRowNum; i++){
			for(int j = 0; j < mCurColumns; j++){
				FrameAreas[(i * mCurColumns) + j] = mFrames[(i*mCurColumns) + j]->renderIm((mFramesBackGround.x+ (mColSpace*2) +  ((mCurFrameScale*mTexParam.TileSizeX)+mColSpace)*j),mFramesBackGround.y + mScroll + (mColSpace*2) + (((mTexParam.TileSizeY*mCurFrameScale)+mColSpace)*i), (i*mCurColumns) + j, mDragged, mCurFrameScale,true,true);								
				if((mDragged > -1) && !bIsDragged){
					bIsDragged = true;
					mDragSource = mDragged;
					mDragTarget = (i*mCurColumns) + j;
				}
				if((mCurColumns > 1) && (j < (mCurColumns-1))){					
					ImGui::SameLine();
				} 
			}										
		}	
		
		if(isOdd){			
			int i = mCurColumns;
			for(int j = 0; j < isOdd; j++){
				FrameAreas[(i * cRowNum) + j] = mFrames[(i*cRowNum)+j]->renderIm((mFramesBackGround.x+ (mColSpace*2) +  ((mCurFrameScale*mTexParam.TileSizeX)+mColSpace)*j),mFramesBackGround.y + mScroll + (mColSpace*2) + (((mTexParam.TileSizeY*mCurFrameScale)+mColSpace)*cRowNum), (i*cRowNum)+j, mDragged,  mCurFrameScale,true,true);				
				if((mDragged > -1) && !bIsDragged){
					bIsDragged = true;
					mDragSource = mDragged;
					mDragTarget = (i*cRowNum)+j;
				}				
				if((j < (isOdd-1))){
					ImGui::SameLine();
				}
			}

		}		
	}

	ImGui::PopStyleColor(3);

	if(bIsDragged){		
		mGlobalSettings.CurrentEditor->swapFrames(mDragSource, mDragTarget, true);
	}

	mGlobalSettings.CurrentEditor->ImButtonsSprite.updateButtonStates();
	
    ImGui::End();


	int cMax = (int)( (float)( ( ( (mCurFrameScale*mTexParam.TileSizeY ) +mColSpace ) * mFrames.size() )  / mCurColumns )) + (4 * mTexParam.TileSizeY);
	if((cMax - mFramesBackGround.h) > 0 ){
		mMaxScrollY = -(cMax - mFramesBackGround.h);
	} else {
		mMaxScrollY = 0;
	}

}

