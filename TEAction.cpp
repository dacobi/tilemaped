#include "TSettings.h"
#include "TEAction.h"
#include "TileMap.h"
#include "TEditor.h"
#include "TSprite.h"


extern TSettings mGlobalSettings;

void TEAction::undo(){

}

void TEAction::swapTileValues(int tVal1, int tVal2){}

bool TEAction::checkTileMap(){
	return false;
}
int TEAction::getTileMap(){
	return 0;
}

bool TEAction::checkFrame(){
	return false;
}
int TEAction::getFrame(){
	return 0;
}


void TEActionReplaceTile::undo(){
	mTileMap->setTile(mCurrentTile, mOldValue);
	mTileMap->setOffset(mCurrentTile, mOldOffset);

}

void TEActionReplaceTileFlip::undo(){
	mTileMap->setTile(mCurrentTile, mOldValue);
	mTileMap->setFlip(mCurrentTile, mOldFlip);
	mTileMap->setOffset(mCurrentTile, mOldOffset);

}

void TEActionReplacePixel::undo(){	
	mCurrentTile->setPixel(mCurrentPixel, mOldValue);
	mCurrentTile->updateTexture(mCurrentPalette);
}

void TEActionReplacePixel::redo(){	
	mCurrentTile->setPixel(mCurrentPixel, mNewValue);
	mCurrentTile->updateTexture(mCurrentPalette);
}

bool TEActionReplacePixel::checkTileMap(){
	if(mCurrentTileMap == mGlobalSettings.mEditor->mTileMap){
		return false;
	}
	return true;
}

int TEActionReplacePixel::getTileMap(){
	int cTileMap = -1;
	for(int i = 0; i < mGlobalSettings.mEditor->mTileMaps.size(); i++){
		if(mCurrentTileMap == mGlobalSettings.mEditor->mTileMaps[i]){
			cTileMap = i;
		}
	}
	return cTileMap;
}

bool TEActionReplacePixel::checkFrame(){
	if(mCurrentFrame == mGlobalSettings.mEditor->mSprite->mFrame){
		return false;
	}
	return true;
}

int TEActionReplacePixel::getFrame(){
	int cFrame = -1;
	for(int i = 0; i < mGlobalSettings.mEditor->mSprite->mFrames.size(); i++){
		if(mCurrentFrame == mGlobalSettings.mEditor->mSprite->mFrames[i]){
			cFrame = i;
		}
	}
	return cFrame;
}


void TEAction::redo(){}

void TEActionReplaceTile::redo(){
	mTileMap->setTile(mCurrentTile, mNewValue);
	mTileMap->setOffset(mCurrentTile, mNewOffset);

}

void TEActionReplaceTileFlip::redo(){
	mTileMap->setTile(mCurrentTile, mNewValue);
	mTileMap->setFlip(mCurrentTile, mNewFlip);
	mTileMap->setOffset(mCurrentTile, mNewOffset);

}

bool TEActionReplaceTileFlip::checkTileMap(){
	if(mTileMap == mGlobalSettings.mEditor->mTileMap){
		return false;
	}
	return true;
}

void TEActionReplaceTileFlip::swapTileValues(int tVal1, int tVal2){
	if(mOldValue == tVal1){
		mOldValue = tVal2;
	} else if(mOldValue == tVal2){
		mOldValue = tVal1;
	}
	if(mNewValue == tVal1){
		mNewValue = tVal2;
	} else if(mNewValue == tVal2){
		mNewValue = tVal1;
	}
}


int TEActionReplaceTileFlip::getTileMap(){
	int cTileMap = -1;
	for(int i = 0; i < mGlobalSettings.mEditor->mTileMaps.size(); i++){
		if(mTileMap == mGlobalSettings.mEditor->mTileMaps[i]){
			cTileMap = i;
		}
	}
	return cTileMap;
}

void TEActionReplaceTileFlip::doAction(TileMap *cTileMap, int mCurTile, int mOld, int mNew, int cFlipOld, int cFlipNew, int cOffset){
	mTileMap = cTileMap;
	mCurrentTile = mCurTile;
	mOldValue = mOld;
	mNewValue = mNew;
	mOldFlip = cFlipOld;
	mNewFlip = cFlipNew;
	if(cOffset > -1){
		mNewOffset = cOffset;
	} else {
		mNewOffset = mGlobalSettings.mGlobalTexParam.PaletteOffset;
	}
	mOldOffset = mTileMap->getOffset(mCurrentTile);
	mTileMap->setTile(mCurrentTile, mNew);
	mTileMap->setFlip(mCurrentTile, mNewFlip);
	mTileMap->setOffset(mCurrentTile, mNewOffset);
	TEActionType=ACTION_TILEFLIP;
}

bool TEActionReplaceTile::checkTileMap(){
	if(mTileMap == mGlobalSettings.mEditor->mTileMap){
		return false;
	}
	return true;
}
int TEActionReplaceTile::getTileMap(){
	int cTileMap = -1;
	for(int i = 0; i < mGlobalSettings.mEditor->mTileMaps.size(); i++){
		if(mTileMap == mGlobalSettings.mEditor->mTileMaps[i]){
			cTileMap = i;
		}
	}
	return cTileMap;
}

void TEActionReplaceTile::swapTileValues(int tVal1, int tVal2){
	if(mOldValue == tVal1){
		mOldValue = tVal2;
	} else if(mOldValue == tVal2){
		mOldValue = tVal1;
	}
	if(mNewValue == tVal1){
		mNewValue = tVal2;
	} else if(mNewValue == tVal2){
		mNewValue = tVal1;
	}
}

void TEActionReplaceTile::doAction(TileMap *cTileMap, int mCurTile, int mOld, int mNew){
	mTileMap = cTileMap;
	mCurrentTile = mCurTile;
	mOldValue = mOld;
	mNewValue = mNew;
	mNewOffset = mGlobalSettings.mGlobalTexParam.PaletteOffset;
	mOldOffset = mTileMap->getOffset(mCurrentTile);	
	mTileMap->setTile(mCurrentTile, mNew);
	mTileMap->setOffset(mCurrentTile, mNewOffset);
	TEActionType=ACTION_TILE;
}

void TEActionReplacePixel::doAction(Tile* mCurTile, int mCurPix, int mOld, int mNew, TPalette* mPal){
	mCurrentTile = mCurTile;
	mCurrentTileMap = mGlobalSettings.mEditor->mTileMap;
	mCurrentPixel = mCurPix;
	mOldValue = mOld;
	mNewValue = mNew;
	mCurrentPalette = mPal;
	mCurrentFrame = (TSFrame*)mCurrentTile;	
	mCurrentTile->setPixel(mCurrentPixel, mNewValue);
	mCurrentTile->updateTexture(mCurrentPalette);
	TEActionType=ACTION_PIXEL;
}

void TEActionBrushPixels::doAction(Tile* cTile, TBrush &mBrush, TPalette* cPalette){
	UUID = mBrush.UUID;
	mNewValues = mBrush.mBrushElements;
	mSelection = mBrush.mSelected;
	mTile = cTile;
	mPalette = cPalette;
	TEActionType=ACTION_PBRUSH;

	int eindex = 0;

	for(auto &mSelElement : mSelection){
		if(mSelElement > -1){ 			
			if(mNewValues[eindex] != -1){
				TEActionReplacePixel* newAction = new TEActionReplacePixel();							
				newAction->doAction(mTile, mSelElement, mTile->getPixel(mSelElement), mNewValues[eindex], mPalette);
				mSubActions.push_back(newAction);			
			}
		}
		eindex++;
	}
}


void TEActionBrushPixelsSelEdit::doAction(TileMap* cTileMap, TileSet* cTileSet, std::vector<int> &mSelected, TBrush &mBrush, int mAreaX, int mAreaY, TPalette* cPalette){
	UUID = mBrush.UUID;
	mNewValues = mBrush.mBrushElements;
	mSelection = mBrush.mSelected;
	mTileMap = cTileMap;
	mTileSet = cTileSet;
	mPalette = cPalette;
	TEActionType=ACTION_PIXELSSELEDIT;

	int eindex = 0;

	for(auto &mSelElement : mSelection){
		if(mSelElement > -1){ 			

			int ttindex;
			int ttile;
			int stile;
			int mtile;
			ttile = mTileSet->mSelection.getTileIndex(mSelElement, mAreaX, mAreaY,ttindex, &mGlobalSettings.mGlobalTexParam);
			stile = mSelected[ttile];
			mtile = mTileMap->getTile(stile);

			if((mtile > -1) && (mNewValues[eindex] > -1)){
				Tile* cSelectedTile = mTileSet->TTiles[mtile];
				int tindex = cSelectedTile->getFlipIndex(ttindex, mTileMap->getFlip(stile));				
				TEActionReplacePixel* newAction = new TEActionReplacePixel();							
				newAction->doAction(cSelectedTile, tindex, cSelectedTile->getPixel(tindex), mNewValues[eindex], mPalette);
				mSubActions.push_back(newAction);			
			}
		}
		eindex++;
	}
}


void TEActionBrushPixelsTileSet::doAction(TileSet* cTileSet, TBrush &mBrush, int mAreaX, int mAreaY, TPalette* cPalette){
	UUID = mBrush.UUID;
	mNewValues = mBrush.mBrushElements;
	mSelection = mBrush.mSelected;
	mTileSet = cTileSet;
	mPalette = cPalette;
	TEActionType=ACTION_PBRUSHTILESET;

	int eindex = 0;

	for(auto &mSelElement : mSelection){
		if(mSelElement > -1){ 			
			if(mNewValues[eindex] != -1){				
				int tindex;
				int tTile = mBrush.getTileIndex(mSelElement, mAreaX, mAreaY, tindex, &mGlobalSettings.mGlobalTexParam);
				Tile *mTile = mTileSet->TTiles[tTile];
				TEActionReplacePixel* newAction = new TEActionReplacePixel();							
				newAction->doAction(mTile, tindex, mTile->getPixel(tindex), mNewValues[eindex], mPalette);
				mSubActions.push_back(newAction);			
			}
		}
		eindex++;
	}
}

void TEActionReplacePixelsSel::doAction(TileSet *cTileSet, TSelection &mSelect, int mAreaX, int mAreaY, int cNewColor, TPalette* mPal){
	mNewValue = cNewColor;
	mCurrentPalette = mPal;
	mSelection = mSelect.mSelected;
	mTileSet = cTileSet;
	TEActionType=ACTION_PIXELSSEL;

	int eindex = 0;

	for(auto &mSelElement : mSelection){
		if(mSelElement > -1){ 	
				int tindex;
				int tTile = mSelect.getTileIndex(mSelElement, mAreaX, mAreaY, tindex, &mGlobalSettings.mGlobalTexParam);
				Tile *mTile = mTileSet->TTiles[tTile];
				TEActionReplacePixel* newAction = new TEActionReplacePixel();							
				newAction->doAction(mTile, tindex, mTile->getPixel(tindex), mNewValue, mCurrentPalette);
				mSubActions.push_back(newAction);			
		}
		eindex++;
	}
}

void TEActionBrushTiles::doAction(TileMap* cTileMap, TBrush &mBrush){
	UUID = mBrush.UUID;
	mNewValues = mBrush.mBrushElements;
	mSelection = mBrush.mSelected;
	mTileMap = cTileMap;
	TEActionType=ACTION_TBRUSH;

	int eindex = 0;

	for(auto &mSelElement : mSelection){
		if(mSelElement > -1){ 				
			if(mNewValues[eindex] != -1){
				TEActionReplaceTileFlip* newAction = new TEActionReplaceTileFlip();
				if(mBrush.mParent->bUseTileOffset){
					newAction->doAction(mTileMap, mSelElement, mTileMap->getTile(mSelElement), mNewValues[eindex], mTileMap->getFlip(mSelElement), mBrush.getElementFlip(eindex), mBrush.getElementProps(eindex).mPaletteOffset);				
				} else {
					newAction->doAction(mTileMap, mSelElement, mTileMap->getTile(mSelElement), mNewValues[eindex], mTileMap->getFlip(mSelElement), mBrush.getElementFlip(eindex));				
				}
				mSubActions.push_back(newAction);			
			}
		}
		eindex++;
	}
}

void TEActionReplacePixels::doAction(Tile* mCurTile, std::vector<int> &newSel,int mOldColor, int mNewColor, TPalette* mPal){
	mCurrentTile = mCurTile;		
	mNewValue = mNewColor;
	mCurrentPalette = mPal;
	mSelection = newSel;
	TEActionType=ACTION_PIXELS;

	if(mOldColor == -1){
		for(auto &mSelElement : mSelection){
			if(mSelElement > -1){ 
				TEActionReplacePixel* newAction = new TEActionReplacePixel();
				newAction->doAction(mCurrentTile, mSelElement, mCurTile->getPixel(mSelElement), mNewValue, mCurrentPalette);
				mSubActions.push_back(newAction);
			}
		}
	} else {
		for(auto &mSelElement : mSelection){
			if(mSelElement > -1){ 
				TEActionReplacePixel* newAction = new TEActionReplacePixel();
				newAction->doAction(mCurrentTile, mSelElement, mOldColor, mNewValue, mCurrentPalette);
				mSubActions.push_back(newAction);
			}
		}
	}
}

void TEActionReplaceTiles::doAction(TileMap* cTileMap, std::vector<int> &newSel,int mOld, int mNew){
	mTileMap = cTileMap;
	mOldValue = mOld;
	mNewValue = mNew;
	mSelection = newSel;

	TEActionType = ACTION_TILES;

	if(mOldValue == -1){
		for(auto &mSelElement : mSelection){
			if(mSelElement > -1){ 	
				TEActionReplaceTileFlip* newAction = new TEActionReplaceTileFlip();
				newAction->doAction(mTileMap, mSelElement, mTileMap->getTile(mSelElement), mNewValue, mTileMap->getFlip(mSelElement), 0);
				mSubActions.push_back(newAction);
			} 
		}
	} else {
		for(auto &mSelElement : mSelection){
			if(mSelElement > -1){ 	
				TEActionReplaceTileFlip* newAction = new TEActionReplaceTileFlip();
				newAction->doAction(mTileMap, mSelElement, mOldValue, mNewValue, mTileMap->getFlip(mSelElement), 0);
				mSubActions.push_back(newAction);
			} 
		}
	}
}

TEActionAddTile::~TEActionAddTile(){	
	mNewTile->freeTexture();
}

TEActionAddFrame::~TEActionAddFrame(){	
	mNewFrame->freeTexture();
}


TEActionAddTiles::~TEActionAddTiles(){	
	mNewTile->freeTexture();
}

TEActionAddFrames::~TEActionAddFrames(){	
	mNewFrame->freeTexture();
}


void TEActionReplaceTileSet::undo(){ 
	mTile->replaceWithBuffer(mOldBuf, &mEditor->mPalette);
}

void TEActionReplaceTileSet::redo(){ 
	mTile->replaceWithBuffer(mTileBuf, &mEditor->mPalette);
}

TEActionReplaceTileSet::~TEActionReplaceTileSet(){}


void TEActionReplaceTileSet::doAction(Tile* cTile, std::vector<unsigned char> &cTileBuf, TEditor* cEditor, TileSet *cTiles){
	mTile = cTile;
	mOldBuf = mTile->FileData;
	mTileBuf = cTileBuf;
	mTiles = cTiles;
	mEditor = cEditor;
	mTile->replaceWithBuffer(mTileBuf, &mEditor->mPalette);
	TEActionType = ACTION_TILEREPLACE;
}

void TEActionReplaceFrame::undo(){ 
	mFrame->replaceWithBuffer(mOldBuf, &mEditor->mPalette);
}

void TEActionReplaceFrame::redo(){ 
	mFrame->replaceWithBuffer(mFrameBuf, &mEditor->mPalette);
}

TEActionReplaceFrame::~TEActionReplaceFrame(){}


void TEActionReplaceFrame::doAction(TSFrame* cFrame, std::vector<unsigned char> &cFrameBuf, TEditor* cEditor, TSprite *cSprite){
	mFrame = cFrame;
	mOldBuf = mFrame->FileData;
	mFrameBuf = cFrameBuf;
	mSprite = cSprite;
	mEditor = cEditor;
	mFrame->replaceWithBuffer(mFrameBuf, &mEditor->mPalette);
	TEActionType = ACTION_FRAMEREPLACE;
}

bool TEActionReplaceFrame::checkFrame(){
	if(mFrame == mGlobalSettings.mEditor->mSprite->mFrame){
		return false;
	}
	return true;
}

int TEActionReplaceFrame::getFrame(){
	int cFrame = -1;
	for(int i = 0; i < mGlobalSettings.mEditor->mSprite->mFrames.size(); i++){
		if(mFrame == mGlobalSettings.mEditor->mSprite->mFrames[i]){
			cFrame = i;
		}
	}
	return cFrame;
}

void TEActionMoveTile::undo(){
	Tile* tmpTile = mTiles->TTiles[mTarget];

	mTiles->TTiles.erase(mTiles->TTiles.begin() + mTarget);

	mTiles->TTiles.insert(mTiles->TTiles.begin() + mSource, tmpTile);

	mEditor->mMapSelectedTile = mSource;

	if(bUpdateTileMaps){
		for(auto *cMap : mEditor->mTileMaps){
			cMap->moveTile(mTarget, mSource);
		}
		mEditor->mBrushesTile.moveBrushElement(mTarget, mSource);
		mEditor->mClipboardMap.moveBrushElement(mTarget, mSource);
	}
}

void TEActionMoveTile::redo(){
	
	Tile* tmpTile = mTiles->TTiles[mSource];

	mTiles->TTiles.erase(mTiles->TTiles.begin()+ mSource);

	mTiles->TTiles.insert(mTiles->TTiles.begin() + mTarget, tmpTile);

	mEditor->mMapSelectedTile = mTarget;

	if(bUpdateTileMaps){
		for(auto *cMap : mEditor->mTileMaps){
			cMap->moveTile(mSource, mTarget);
		}
		mEditor->mBrushesTile.moveBrushElement(mSource, mTarget);
		mEditor->mClipboardMap.moveBrushElement(mSource, mTarget);
	}

}

void TEActionMoveTile::doAction(TEditor* cEditor, TileSet *cTiles, int cSource, int cTarget, bool cUpdate){
	mEditor = cEditor;
	mTiles = cTiles;
	mSource = cSource;
	mTarget = cTarget;
	bUpdateTileMaps = cUpdate;

	TEActionType = ACTION_MOVETILES;

	Tile* tmpTile = mTiles->TTiles[mSource];

	mTiles->TTiles.erase(mTiles->TTiles.begin()+ mSource);

	mTiles->TTiles.insert(mTiles->TTiles.begin()+ mTarget, tmpTile);
	
	mEditor->mMapSelectedTile = mTarget;

	if(bUpdateTileMaps){
		for(auto *cMap : mEditor->mTileMaps){
			cMap->moveTile(mSource, mTarget);
		}
		mEditor->mBrushesTile.moveBrushElement(mSource, mTarget);
		mEditor->mClipboardMap.moveBrushElement(mSource, mTarget);
	}

}


TEActionSwapTiles::~TEActionSwapTiles(){	

}

TEActionMoveTile::~TEActionMoveTile(){	

}


void TEActionSwapTiles::undo(){
	std::swap(mTiles->TTiles[mTarget], mTiles->TTiles[mSource]);
	
	mEditor->mMapSelectedTile = mSource;

	if(bUpdateTileMaps){
		for(auto *cMap : mEditor->mTileMaps){
			cMap->swapTileValues(mTarget, mSource);
		}
		mEditor->mBrushesTile.swapBrushElements(mTarget, mSource);
		mEditor->mClipboardMap.swapBrushElements(mTarget, mSource);
	}
}

void TEActionSwapTiles::redo(){
	std::swap(mTiles->TTiles[mSource], mTiles->TTiles[mTarget]);

	mEditor->mMapSelectedTile = mTarget;

	if(bUpdateTileMaps){
		for(auto *cMap : mEditor->mTileMaps){
			cMap->swapTileValues(mSource, mTarget);
		}
		mEditor->mBrushesTile.swapBrushElements(mSource, mTarget);
		mEditor->mClipboardMap.swapBrushElements(mSource, mTarget);
	}	
}

void TEActionSwapTiles::doAction(TEditor* cEditor, TileSet *cTiles, int cSource, int cTarget, bool cUpdate){
	mEditor = cEditor;
	mTiles = cTiles;
	mSource = cSource;
	mTarget = cTarget;
	bUpdateTileMaps = cUpdate;

	TEActionType = ACTION_SWAPTILES;

	//mOldSelTile = mEditor->mMapSelectedTile;


	std::swap(mTiles->TTiles[mSource], mTiles->TTiles[mTarget]);

	mEditor->mMapSelectedTile = mTarget;

	if(bUpdateTileMaps){
		for(auto *cMap : mEditor->mTileMaps){
			cMap->swapTileValues(mSource, mTarget);
		}
		mEditor->mBrushesTile.swapBrushElements(mSource, mTarget);
		mEditor->mClipboardMap.swapBrushElements(mSource, mTarget);
	}

}

void TEActionAddTile::doAction(Tile* cNewTile, TEditor* cEditor, TileSet *cTiles){
	mTiles = cTiles;
	mNewTile = cNewTile;
	mEditor = cEditor;
	mOldMapTile = mEditor->mMapSelectedTile;
	mEditor->mMapSelectedTile = mTiles->TTiles.size()-1;
	mTileIndex = mTiles->TTiles.size()-1;
	mOldTile = mEditor->mTileSelectedTile;
	mOldTile->bIsSelected = false;
	mEditor->mTileSelectedTile = mNewTile;
	mNewTile->bIsSelected = true;
	TEActionType=ACTION_TILENEW;	
}

void TEActionAddFrame::doAction(TSFrame* cNewFrame, TEditor* cEditor, TSprite *cSprite){
	mSprite = cSprite;
	mNewFrame = cNewFrame;
	mEditor = cEditor;
	mOldFrame = mSprite->mFrame;	
	mOldFrameIndex = mSprite->mSelectedFrame;
	mFrameIndex = mSprite->mFrames.size()-1;	
	mOldFrame->bIsSelected = false;
	mSprite->mFrame = mNewFrame;
	mNewFrame->bIsSelected = true;
	mSprite->mSelectedFrame = mFrameIndex;
	TEActionType=ACTION_FRAMENEW;	
}


void TEActionAddTiles::doAction(Tile* cNewTile, TEditor* cEditor, TileSet *cTiles){
	mTiles = cTiles;
	mNewTile = cNewTile;
	mEditor = cEditor;

	mEditor->mTileSelectedTile->bIsSelected = false;
	mEditor->mTileSelectedTile = mTiles->TTiles[0];
	mEditor->mTileSelectedTile->bIsSelected = true;
	mEditor->mMapSelectedTile = 0;
	
	for(int i = 0; i < mTiles->TTiles.size(); i++){
		if(mTiles->TTiles[i] == mNewTile){
			mTileIndex = i;			
		}
	}

	TEActionType=ACTION_TILESNEW;	
}

void TEActionAddTiles::undo(){
	mEditor->mTileSelectedTile->bIsSelected = false;
	mEditor->mTileSelectedTile = mTiles->TTiles[0];
	mEditor->mTileSelectedTile->bIsSelected = true;
	mEditor->mMapSelectedTile = 0;

	for(int i = 0; i < mTiles->TTiles.size(); i++){
		if(mTiles->TTiles[i] == mNewTile){
			mTileIndex = i;			
		}
	}
	mTiles->removeTile(mTileIndex);
}

void TEActionAddTiles::redo(){
	mEditor->mTileSelectedTile->bIsSelected = false;
	mEditor->mTileSelectedTile = mTiles->TTiles[0];
	mEditor->mTileSelectedTile->bIsSelected = true;
	mEditor->mMapSelectedTile = 0;
	
	mTiles->appendTile(mNewTile);
	mTileIndex = mTiles->TTiles.size()-1;
	
}

/* AddFrames */

void TEActionAddFrames::doAction(TSFrame* cNewFrame, TEditor* cEditor, TSprite *cSprite){
	mSprite = cSprite;
	mNewFrame = cNewFrame;
	mEditor = cEditor;

	mSprite->selectFrame(0);
	
	for(int i = 0; i < mSprite->mFrames.size(); i++){
		if(mSprite->mFrames[i] == mNewFrame){
			mFrameIndex = i;			
		}
	}

	TEActionType=ACTION_FRAMESNEW;	
}

void TEActionAddFrames::undo(){
	
	for(int i = 0; i < mSprite->mFrames.size(); i++){
		if(mSprite->mFrames[i] == mNewFrame){
			mFrameIndex = i;			
		}
	}
	mSprite->removeFrame(mFrameIndex);

	mSprite->selectFrame(0);
}

void TEActionAddFrames::redo(){
	
	mSprite->appendFrame(mNewFrame);
	mFrameIndex = mSprite->mFrames.size()-1;
	
	mSprite->selectFrame(0);
}


/* AddFrames End */


void TEActionAddTile::undo(){	
	mTiles->removeTile(mTileIndex);
	mEditor->mTileSelectedTile->bIsSelected = false;
	mEditor->mTileSelectedTile = mOldTile;
	mEditor->mTileSelectedTile->bIsSelected = true;	
	mEditor->mMapSelectedTile = mOldMapTile;
}

void TEActionAddTile::redo(){
	mTiles->appendTile(mNewTile);
	mTileIndex = mTiles->TTiles.size()-1;
	mEditor->mTileSelectedTile->bIsSelected = false;
	mEditor->mTileSelectedTile = mNewTile;
	mEditor->mTileSelectedTile->bIsSelected = true;
	mEditor->mMapSelectedTile = mTiles->TTiles.size()-1;
}

void TEActionAddFrame::undo(){	
	mSprite->removeFrame(mFrameIndex);
	mSprite->selectFrame(mOldFrameIndex);
	//mEditor->mTileSelectedTile->bIsSelected = false;
	//mEditor->mTileSelectedTile = mOldTile;
	//mEditor->mTileSelectedTile->bIsSelected = true;	
	//mEditor->mMapSelectedTile = mOldMapTile;
}

void TEActionAddFrame::redo(){
	mSprite->appendFrame(mNewFrame);
	mSprite->selectFrame(mFrameIndex);
	//mTileIndex = mTiles->TTiles.size()-1;
	//mEditor->mTileSelectedTile->bIsSelected = false;
	//mEditor->mTileSelectedTile = mNewTile;
	//mEditor->mTileSelectedTile->bIsSelected = true;
	//mEditor->mMapSelectedTile = mTiles->TTiles.size()-1;
}

/*
bool TEActionAddFrame::checkFrame(){
	if(mOldFrame == mGlobalSettings.CurrentEditor->mSprite->mFrame){
		return false;
	}
	return true;
}

int TEActionAddFrame::getFrame(){
	int cFrame = -1;
	for(int i = 0; i < mGlobalSettings.CurrentEditor->mSprite->mFrames.size(); i++){
		if(mOldFrame == mGlobalSettings.CurrentEditor->mSprite->mFrames[i]){
			cFrame = i;
		}
	}
	return cFrame;
}
*/

TEActionSwapFrames::~TEActionSwapFrames(){	

}


void TEActionSwapFrames::undo(){
	std::swap(mSprite->mFrames[mTarget], mSprite->mFrames[mSource]);
	
	mSprite->selectFrame(mSource);	
}

void TEActionSwapFrames::redo(){
	std::swap(mSprite->mFrames[mSource], mSprite->mFrames[mTarget]);

	mSprite->selectFrame(mTarget);	
}

void TEActionSwapFrames::doAction(TEditor* cEditor, TSprite *cSprite, int cSource, int cTarget){
	mEditor = cEditor;
	mSprite = cSprite;
	mSource = cSource;
	mTarget = cTarget;
	

	TEActionType = ACTION_SWAPFRAMES;

	std::swap(mSprite->mFrames[mSource], mSprite->mFrames[mTarget]);

	mSprite->selectFrame(mTarget);	
}



void TEActionGroup::undo(){
	for(TEAction* mAction : mActions){
		mAction->undo();
	}
}

void TEActionGroup::redo(){
	for(TEAction* mAction : mActions){
		mAction->redo();
	}
}

TEActionUndoStack::~TEActionUndoStack(){
	redoClearStack();
	undoClearStack();
}

TEActionUndoStack::TEActionUndoStack(){
	mLastAction = &mEmptyAction;
}

void TEActionUndoStack::newActionGroup(){
	TEActionGroup *newGroup = new TEActionGroup();
	mUndoStack.push_back(newGroup);
	
	mGlobalSettings.mDirtyCount++;		
}

void TEActionUndoStack::addAction(TEAction *newAction){
	if(mUndoStack.size()){
		TEActionGroup *mGroup = *(mUndoStack.end()-1);
		mGroup->mActions.push_back(newAction);
	}
}

void TEActionUndoStack::addSubActions(std::vector<TEAction*> &cSubActions){
	for(auto *newAction: cSubActions){
		addAction(newAction);
	}
}

void TEActionUndoStack::undoLastActionGroup(){
	if(mUndoStack.size()){
		TEActionGroup *mGroup = *(mUndoStack.end()-1);
		mUndoStack.pop_back();
		mRedoStack.push_back(mGroup);
		mGroup->undo();
		mLastAction = &mEmptyAction;

		mGlobalSettings.mDirtyCount--;
	}
}

void TEActionUndoStack::dropLastGroup(){
	if(mUndoStack.size()){			
		mUndoStack.pop_back();
		mLastAction = &mEmptyAction;

		mGlobalSettings.mDirtyCount--;
	}		
}


void TEActionUndoStack::redoLastActionGroup(){
	if(mRedoStack.size()){
		TEActionGroup *mGroup = *(mRedoStack.end()-1);
		mRedoStack.pop_back();
		mUndoStack.push_back(mGroup);
		mGroup->redo();

		mGlobalSettings.mDirtyCount++;
	}
}

void TEActionUndoStack::redoClearStack(){	
	if(mRedoStack.size()){		
		for(auto *dGroup : mRedoStack){
			for(auto *dAction: dGroup->mActions){				
				delete dAction;				
			}		
		}
		mRedoStack.erase(mRedoStack.begin(), mRedoStack.end());		
	}	

	mGlobalSettings.bProjectIsDirty = true;	
}

void TEActionUndoStack::undoClearStack(){
	if(mUndoStack.size()){
		for(auto *dGroup : mUndoStack){
			for(auto *dAction: dGroup->mActions){
				TEActionAddTile* ddAction = dynamic_cast<TEActionAddTile*>(dAction);
				TEActionAddTiles* dddAction = dynamic_cast<TEActionAddTiles*>(dAction);
				TEActionAddFrame* ddddAction = dynamic_cast<TEActionAddFrame*>(dAction);
				TEActionAddFrames* dddddAction = dynamic_cast<TEActionAddFrames*>(dAction);
				if(!ddAction && !dddAction && !ddddAction && !dddddAction){
					delete dAction;
				}
			}		
		}
		mUndoStack.erase(mUndoStack.begin(), mUndoStack.end());

		
	}	

	mGlobalSettings.bProjectIsDirty = true;
}
