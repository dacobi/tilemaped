#include "TSettings.h"
#include "TSprite.h"
#include "TEditor.h"

TSFrame* TSprite::createFrame(TPalette* tpal){
    TSFrame* newFrame = new TSFrame(&mTexParam);

    newFrame->createNew(tpal);

    mFrames.push_back(newFrame);
    mFrame = newFrame;

    return newFrame;
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

void TSprite::renderIm(int ypos, int mScroll){
    mFramesBackGround.h = mGlobalSettings.WindowHeight- mGlobalSettings.TopBarHeight;
	
	if(mCurColumns < 4){
		if( (int)( (float)( ( ( (mCurFrameScale*mTexParam.TileSizeX ) +mColSpace ) * mFrames.size() )  / mCurColumns ) ) > mFramesBackGround.h ){	
			mCurFrameScale--;
		
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

	ImGui::Begin("Sprite", NULL, ImGuiWindowFlags_NoNav);    

	if(ImGui::Button("Move Up")){
		//TODO mGlobalSettings.CurrentEditor->moveTileUp();
	}

	if(ImGui::Button("Move Down")){
		//TODO mGlobalSettings.CurrentEditor->moveTileDown();
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
		//TODO mGlobalSettings.CurrentEditor->swapTiles(mDragSource, mDragTarget, true);
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

