#include "TSettings.h"
#include "TSprite.h"
#include "TEditor.h"

void TSFrame::renderEd(int xpos, int ypos, TPalette* tpal){
	for(int i=0; i < mTexParam->TileSizeY; i++){
		for(int j=0; j < mTexParam->TileSizeX; j++){
			PixelAreas[j+(mTexParam->TileSizeX*i)] = tpal->renderSpriteEd(xpos + (mTexParam->TilePixelSize * mTexParam->mTileEdScale)*j, ypos + (mTexParam->TilePixelSize * mTexParam->mTileEdScale)*i, getPixel(j+(i*mTexParam->TileSizeX)), mTexParam); 			
		}
	}
	
	if(mGlobalSettings.CurrentEditor->mSprite->mCurrentBrushPixel && !mGlobalSettings.CurrentEditor->mBrushesSprite->bIsEditing){
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

int TSprite::selectNext(){
	if(mSelectedFrame < mFrames.size()-1){
		selectFrame(mSelectedFrame + 1);
		return 0;
	}
	return 1;
}

int TSprite::selectPrev(){
	if(mSelectedFrame > 0){
		selectFrame(mSelectedFrame - 1);
		return 0;
	}
	return 1;

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

int TSprite::saveToFile(std::string spath, std::string sfile){

	std::ofstream outfile(spath+DIRDEL+sfile, std::ios::binary );

	std::vector<unsigned char> obuffer;

	int magic1, magic2;

	magic1 = mGlobalSettings.mFrameSizeOut[mTexParam.TileSizeX];
	magic1 = (magic1 << 4) + mGlobalSettings.mFrameSizeOut[mTexParam.TileSizeY];

	magic2 = mTexParam.TileSetBPP;

	obuffer.push_back(magic1);
	obuffer.push_back(magic2);

	for(auto &cFrame : mFrames){		
		obuffer.insert(obuffer.end(), cFrame->FileData.begin(), cFrame->FileData.end());
	}

	outfile.write((char*)obuffer.data(),obuffer.size());
	outfile.close();

	return 0;

}

int TSprite::loadFromBuffer(std::vector<unsigned char> sBuf, TPalette* tpal){

	int tmpFrameSize = ((mTexParam.TileSizeX * mTexParam.TileSizeY)/mGlobalSettings.mTileBPPSize[mTexParam.TileSetBPP]);

	if(sBuf.size() % tmpFrameSize){		
		return 1;
	}

	int nframes = sBuf.size() / tmpFrameSize;
	FrameAreas.resize(nframes);

	TSFrame *cFrame;

	for(int tCount = 0; tCount < nframes; tCount++){
		cFrame = new TSFrame(&mTexParam);
		std::vector<unsigned char>::const_iterator first = sBuf.begin() + (tCount * tmpFrameSize);
		std::vector<unsigned char>::const_iterator last = sBuf.begin() + ((tCount * tmpFrameSize) + (tmpFrameSize));
		std::vector<unsigned char> tbuffer2(first, last);
		cFrame->loadFromBuffer(tbuffer2 ,tpal);
		mFrames.push_back(cFrame);
	}

	selectFrame(0);

	return 0;
}

int TSprite::importFromBuffer(std::vector<unsigned char> sBuf, TPalette* tpal){

	int tmpFrameSize = ((mTexParam.TileSizeX * mTexParam.TileSizeY)/mGlobalSettings.mTileBPPSize[mTexParam.TileSetBPP]);

	if(sBuf.size() % tmpFrameSize){		
		return 1;
	}

	int nframes = sBuf.size() / tmpFrameSize;
	
	TSFrame *cFrame;

	for(int tCount = 0; tCount < nframes; tCount++){
		cFrame = new TSFrame(&mTexParam);
		std::vector<unsigned char>::const_iterator first = sBuf.begin() + (tCount * tmpFrameSize);
		std::vector<unsigned char>::const_iterator last = sBuf.begin() + ((tCount * tmpFrameSize) + (tmpFrameSize));
		std::vector<unsigned char> tbuffer2(first, last);
		cFrame->loadFromBuffer(tbuffer2 ,tpal);
		mFrames.push_back(cFrame);	
	}

	FrameAreas.resize(mFrames.size());

	selectFrame(0);

	return 0;

}

int TSprite::importPNG(SDL_Surface *newSurf, TPalette* tpal){
	if(newSurf->format->BitsPerPixel == 8){
		if(!(newSurf->w % mTexParam.TileSizeX) && !(newSurf->h % mTexParam.TileSizeY)){
			int cTilesX,cTilesY;
			cTilesX = newSurf->w / mTexParam.TileSizeX;
			cTilesY = newSurf->h / mTexParam.TileSizeY;
			
			std::cout << "Import PNG: " << cTilesX << "," <<  cTilesY << std::endl;

			std::vector<std::vector<unsigned char>> tbuffers;

			tbuffers.resize(cTilesX*cTilesY);

			for(auto &tbuf : tbuffers){
				tbuf.resize((mTexParam.TileSizeX*mTexParam.TileSizeY) / mGlobalSettings.mTileBPPSize[mTexParam.TileSetBPP],0);
			}

			//Tile tmpTile;
			int cBufIndex;
			int cPixIndex;

			TSelection mSelection;

			for(int ii=0; ii < newSurf->h; ii++){
				for(int jj=0; jj < newSurf->w; jj++){
					cBufIndex = mSelection.getTileIndex((ii*newSurf->w)+jj, newSurf->w, newSurf->h, cPixIndex, &mTexParam);					
					//tmpTile.
					TTexture::setPixel(cPixIndex, ((unsigned char*)(newSurf->pixels))[(ii*newSurf->w)+jj], tbuffers[cBufIndex], &mTexParam);
				}
			}

			for(auto &tbufl : tbuffers){
				createNewFromBuffer(tbufl, tpal);
			}

			FrameAreas.resize(mFrames.size());

			selectFrame(0);

					
			SDL_FreeSurface(newSurf);
			return 0;
		}
	}
			
	SDL_FreeSurface(newSurf);
	return 1;
}


TSFrame* TSprite::createNewFromFile(std::string newPath, TPalette* tpal){
	if(fs::exists(fs::status(newPath))){
		if(fs::is_directory(fs::status(newPath))){
			return NULL;
		}

		SDL_Surface *newSurf = IMG_Load(newPath.c_str());
		if(newSurf && (mTexParam.TileSetBPP > 0x2)){
			if(newSurf->format->BitsPerPixel == 8){
				if((newSurf->w == mTexParam.TileSizeX) && (newSurf->h == mTexParam.TileSizeY)){
					std::vector<unsigned char> tbuffer;
					if(mTexParam.TileSetBPP == 0x4){
						unsigned char tmpChar;
						bool sndPix=false;
						for(int i = 0; i < (mTexParam.TileSizeX*mTexParam.TileSizeY); i++){
							if(sndPix){
								tmpChar = (tmpChar << 4) + ((unsigned char*)(newSurf->pixels))[i]%16;
								tbuffer.push_back(tmpChar);
								sndPix = false;
							} else {
								tmpChar = ((unsigned char*)(newSurf->pixels))[i]%16;
								sndPix = true;
							}
							
						}
					} else {
						for(int i = 0; i < (mTexParam.TileSizeX*mTexParam.TileSizeY); i++){
							tbuffer.push_back(((unsigned char*)(newSurf->pixels))[i]);
						}
					}
					SDL_FreeSurface(newSurf);
					return createNewFromBuffer(tbuffer, tpal);
				}
			}
			SDL_FreeSurface(newSurf);
			return NULL;

		} else {
			std::ifstream infile(newPath, std::ios::binary );

			std::vector<unsigned char> tbuffer(std::istreambuf_iterator<char>(infile), {});

			if(tbuffer.size() == ((mTexParam.TileSizeX * mTexParam.TileSizeY)/mGlobalSettings.mTileBPPSize[mTexParam.TileSetBPP])){
				return createNewFromBuffer(tbuffer, tpal);
			} else {
				return NULL;				
			}
		}
	}
	return NULL;

}

	
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

	ImGui::BeginChild("SFrames", ImVec2(0,0), false, ImGuiWindowFlags_NoNav);

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

    ImGui::EndChild();
	
    ImGui::End();

	/*
	int cMax = (int)( (float)( ( ( (mCurFrameScale*mTexParam.TileSizeY ) +mColSpace ) * mFrames.size() )  / mCurColumns )) + (4 * mTexParam.TileSizeY);
	if((cMax - mFramesBackGround.h) > 0 ){
		mMaxScrollY = -(cMax - mFramesBackGround.h);
	} else {
		mMaxScrollY = 0;
	}
	*/

}
