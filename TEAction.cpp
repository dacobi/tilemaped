#include "TSettings.h"
#include "TEAction.h"
#include "TileMap.h"
#include "TEditor.h"


extern TSettings mGlobalSettings;

void TEAction::undo(){

}

bool TEAction::checkTileMap(){
	return false;
}
int TEAction::getTileMap(){
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

void TEActionReplacePixel::redo(){	
	mCurrentTile->setPixel(mCurrentPixel, mNewValue);
	mCurrentTile->updateTexture(mCurrentPalette);
}

bool TEActionReplaceTileFlip::checkTileMap(){
	if(mTileMap == mGlobalSettings.CurrentEditor->mTileMap){
		return false;
	}
	return true;
}
int TEActionReplaceTileFlip::getTileMap(){
	int cTileMap = -1;
	for(int i = 0; i < mGlobalSettings.CurrentEditor->mTileMaps.size(); i++){
		if(mTileMap == mGlobalSettings.CurrentEditor->mTileMaps[i]){
			cTileMap = i;
		}
	}
	return cTileMap;
}

void TEActionReplaceTileFlip::doAction(TileMap *cTileMap, int mCurTile, int mOld, int mNew, int cFlipOld, int cFlipNew){
	mTileMap = cTileMap;
	mCurrentTile = mCurTile;
	mOldValue = mOld;
	mNewValue = mNew;
	mOldFlip = cFlipOld;
	mNewFlip = cFlipNew;
	mNewOffset = mGlobalSettings.PaletteOffset;
	mOldOffset = mTileMap->getOffset(mCurrentTile);
	mTileMap->setTile(mCurrentTile, mNew);
	mTileMap->setFlip(mCurrentTile, mNewFlip);
	mTileMap->setOffset(mCurrentTile, mNewOffset);
	TEActionType=ACTION_TILEFLIP;
}

bool TEActionReplaceTile::checkTileMap(){
	if(mTileMap == mGlobalSettings.CurrentEditor->mTileMap){
		return false;
	}
	return true;
}
int TEActionReplaceTile::getTileMap(){
	int cTileMap = -1;
	for(int i = 0; i < mGlobalSettings.CurrentEditor->mTileMaps.size(); i++){
		if(mTileMap == mGlobalSettings.CurrentEditor->mTileMaps[i]){
			cTileMap = i;
		}
	}
	return cTileMap;
}

void TEActionReplaceTile::doAction(TileMap *cTileMap, int mCurTile, int mOld, int mNew){
	mTileMap = cTileMap;
	mCurrentTile = mCurTile;
	mOldValue = mOld;
	mNewValue = mNew;
	mNewOffset = mGlobalSettings.PaletteOffset;
	mOldOffset = mTileMap->getOffset(mCurrentTile);	
	mTileMap->setTile(mCurrentTile, mNew);
	mTileMap->setOffset(mCurrentTile, mNewOffset);
	TEActionType=ACTION_TILE;
}

void TEActionReplacePixel::doAction(Tile* mCurTile, int mCurPix, int mOld, int mNew, TPalette* mPal){
	mCurrentTile = mCurTile;
	mCurrentPixel = mCurPix;
	mOldValue = mOld;
	mNewValue = mNew;
	mCurrentPalette = mPal;
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
				int tTile = mBrush.getTileIndex(mSelElement, mAreaX, mAreaY, tindex);
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
				int tTile = mSelect.getTileIndex(mSelElement, mAreaX, mAreaY, tindex);
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
				newAction->doAction(mTileMap, mSelElement, mTileMap->getTile(mSelElement), mNewValues[eindex], mTileMap->getFlip(mSelElement), mBrush.getElementFlip(eindex));				
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

TEActionAddTiles::~TEActionAddTiles(){	
	mNewTile->freeTexture();
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
	}
}

void TEActionUndoStack::dropLastGroup(){
	if(mUndoStack.size()){			
		mUndoStack.pop_back();
		mLastAction = &mEmptyAction;				
	}		
}


void TEActionUndoStack::redoLastActionGroup(){
	if(mRedoStack.size()){
		TEActionGroup *mGroup = *(mRedoStack.end()-1);
		mRedoStack.pop_back();
		mUndoStack.push_back(mGroup);
		mGroup->redo();
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
}

void TEActionUndoStack::undoClearStack(){
	if(mUndoStack.size()){
		for(auto *dGroup : mUndoStack){
			for(auto *dAction: dGroup->mActions){
				TEActionAddTile* ddAction = dynamic_cast<TEActionAddTile*>(dAction);
				TEActionAddTiles* dddAction = dynamic_cast<TEActionAddTiles*>(dAction);
				if(!ddAction && !dddAction){
					delete dAction;
				}
			}		
		}
		mUndoStack.erase(mUndoStack.begin(), mUndoStack.end());
	}	
}
