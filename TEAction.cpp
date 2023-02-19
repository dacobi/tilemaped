#include "TSettings.h"
#include "TEAction.h"
#include "TileMap.h"
#include "TEditor.h"


extern TSettings mGlobalSettings;

void TEAction::undo(){

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

void TEActionBrushTiles::doAction(TileMap* cTileMap, TBrush &mBrush){
	UUID = mBrush.UUID;
	mNewValues = mBrush.mBrushElements;
	mSelection = mBrush.mSelected;
	mTileMap = cTileMap;
	TEActionType=ACTION_TBRUSH;

	int eindex = 0;

	for(auto &mSelElement : mSelection){
		if(mSelElement > -1){ 
			TEActionReplaceTileFlip* newAction = new TEActionReplaceTileFlip();			
			if(mNewValues[eindex] != -1){
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
				TEActionReplaceTile* newAction = new TEActionReplaceTile();
				newAction->doAction(mTileMap, mSelElement, mTileMap->getTile(mSelElement), mNewValue);
				mSubActions.push_back(newAction);
			} 
		}
	} else {
		for(auto &mSelElement : mSelection){
			if(mSelElement > -1){ 	
				TEActionReplaceTile* newAction = new TEActionReplaceTile();
				newAction->doAction(mTileMap, mSelElement, mOldValue, mNewValue);
				mSubActions.push_back(newAction);
			} 
		}
	}
}

TEActionAddTile::~TEActionAddTile(){	
	delete mNewTile;
}

		

void TEActionAddTile::doAction(Tile* cNewTile, TEditor* cEditor, TileSet *cTiles){
	mTiles = cTiles;
	mNewTile = cNewTile;
	mEditor = cEditor;
	mOldMapTile = mEditor->mMapSelectedTile;
	mEditor->mMapSelectedTile = mTiles->TTiles.size()-1;
	mOldTile = mEditor->mTileSelectedTile;
	mOldTile->bIsSelected = false;
	mEditor->mTileSelectedTile = mNewTile;
	mNewTile->bIsSelected = true;
	TEActionType=ACTION_TILENEW;	
}

void TEActionDropTile::doAction(Tile* cDroppedTile, TEditor* cEditor, TileSet *cTiles){
	mTiles = cTiles;
	mNewTile = cDroppedTile;
	mEditor = cEditor;
	
	//mOldMapTile = mEditor->mMapSelectedTile;	
	//mOldTile = mEditor->mTileSelectedTile;

	mNewTile->bIsSelected = false;

	mEditor->mMapSelectedTile = 0;
	mEditor->mTileSelectedTile = mTiles->TTiles[0];
	
	TEActionType=ACTION_TILEDROP;	
}

void TEActionDropTile::redo(){
	mTiles->dropLastTile();
	mNewTile->bIsSelected = false;

	mEditor->mMapSelectedTile = 0;
	mEditor->mTileSelectedTile = mTiles->TTiles[0];
	//mEditor->mTileSelectedTile->bIsSelected = false;
	//mEditor->mTileSelectedTile = mOldTile;
	//mEditor->mTileSelectedTile->bIsSelected = true;	
	//mEditor->mMapSelectedTile = mOldMapTile;
}

void TEActionDropTile::undo(){
	mTiles->appendTile(mNewTile);
	//mEditor->mTileSelectedTile->bIsSelected = false;
	//mEditor->mTileSelectedTile = mNewTile;
	//mEditor->mTileSelectedTile->bIsSelected = true;
	//mEditor->mMapSelectedTile = mTiles->TTiles.size()-1;
}

void TEActionAddTile::undo(){
	mTiles->dropLastTile();
	mEditor->mTileSelectedTile->bIsSelected = false;
	mEditor->mTileSelectedTile = mOldTile;
	mEditor->mTileSelectedTile->bIsSelected = true;	
	mEditor->mMapSelectedTile = mOldMapTile;
}

void TEActionAddTile::redo(){
	mTiles->appendTile(mNewTile);
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
				TEActionAddTile* ddAction = dynamic_cast<TEActionAddTile*>(dAction);
				if(ddAction){
					delete ddAction;
				} else {
					delete dAction;
				}
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
				if(!ddAction){
					delete dAction;
				}
			}		
		}
		mUndoStack.erase(mUndoStack.begin(), mUndoStack.end());
	}	
}
