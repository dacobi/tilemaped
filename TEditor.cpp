#include "TSettings.h"
#include "TEditor.h"

extern TSettings mGlobalSettings;

void TEditor::shutdown(){
	mTileSet.shutdown();
}

int TEditor::createNewProject(){
	std::cout << "Creating Project: " << mGlobalSettings.ProjectPath << std::endl;
	std::cout << "TileMapWidth: " << mGlobalSettings.TileMapWidth << " TileMapHeight: " << mGlobalSettings.TileMapHeight << " TileSizeX: " << mGlobalSettings.TileSizeX << " TileSizeY: " << mGlobalSettings.TileSizeY << std::endl;	
	if(mGlobalSettings.bProjectHasPalette){
		if(mGlobalSettings.testPaletteFile(mGlobalSettings.ProjectPalettePath) == 2){
			if(mPalette.importGimpPalette(mGlobalSettings.ProjectPalettePath)){				
				return 1;	
			}
		} else {
			if(mPalette.loadFromFile(mGlobalSettings.ProjectPalettePath)){
				return 1;
			}
		}
	} else {
		mPalette.initPalette();
	}

	mPalette.initTPixels();
	mTileMap.createNew();										
	mTileSet.createNew(&mPalette);

	initDialogs();

	return 0;
}

void TEditor::initDialogs(){
	mMapSelectedTile = 0;
	mTileSelectedTile = mTileSet.TTiles[0];
	mTileSelectedTile->bIsSelected = true;
	mColorSelectedTile = mPalette.TPixels[0];
	mColorSelectedTile->bPixelSelected = true;
	mColorSelected = 0;

	mSaveDialog.init();
	mSaveAsDialog.mTextInput.mDialogTextMain = mGlobalSettings.ProjectPath;
	mSaveAsDialog.init();
	mSaveAsDialog.mSubDialog = &mSaveDialog;
	
	mHelpDialog.init();

	mTopBar.mEditor = this;
	mTopBar.init();
	mProjectInfo.mEditor = this;
	mProjectInfo.init();
	mOpenTileDialog.init();
	mInfoMessage.init();
	mErrorMessage.setColorScheme(1);
	mErrorMessage.init();

	mRemoveUnused.init();

	mPaletteUpdate.init();

	mInputNumber.init();

	mPaletteOffset.init();

	if(mGlobalSettings.TileSetBPP < 0x8){
		mGlobalSettings.bShowPaletteOffset = true;
	}

	mBrushesTile.init("Tiles","Tile", TBRUSH_TILE, &bShowBrushesTile, mGlobalSettings.TileMapScale);

	//mBrushesTile.mBrushType = TBRUSH_TILE;
	//mBrushesTile.bIsShown = &bShowBrushesTile;
	//mBrushesTile.mRenderScale = mGlobalSettings.TileMapScale;
	//mBrushesTile.addBrush(8,2);
	//mBrushesTile.addBrush(3,3);

	//tmpBrush.configBrush(8,2,TBRUSH_TILE);
	//mCurBrush = &tmpBrush;
	
}

int TEditor::loadFromFolder(std::string path){

	if(fs::exists(fs::status(path+DIRDEL+"pal.bin"))){
		if(mPalette.loadFromFile(std::string(path+DIRDEL+"pal.bin"))){
			std::cout << "Error loading palette: " << path +DIRDEL+"pal.bin" << std::endl;
			return 1;
		}
	} else {
		mPalette.initPalette();
	}

	mPalette.initTPixels();
			
	if(mTileSet.loadFromFolder(path, &mPalette)){
		std::cout << "Error loading tiles: " << path << std::endl;
		return 1;
	}

	if(mTileMap.loadFromFile(path, "map.bin")){
		std::cout << "Error: can't read: " << path << DIRDEL << "map.bin" << std::endl;
		return 1;
	}
	
	mGlobalSettings.ProjectPath = path;
	
	initDialogs();

	if(fs::exists(fs::status(path+DIRDEL+"tbrushes.dat"))){
		mBrushesTile.loadFromFile(path+DIRDEL+"tbrushes.dat");
	}

	return 0;
}

int TEditor::saveToFolder(std::string path){

	fs::path cpath = path;
	if(!fs::is_directory(fs::status(cpath))){
		std::cout << "Creating Folder" << std::endl;
		try{
			fs::create_directory(cpath);			
		} catch(...){
			std::cout << "Error Creating Folder!" << std::endl;
			return 1;
		}
	}
	
	mTileSet.saveToFolder(path);
	mTileMap.saveToFolder(path);
	mPalette.saveToFolder(path);

	mBrushesTile.saveToFile(path + DIRDEL + "tbrushes.dat");

	return 0;
}

int TEditor::render(){
	mGlobalSettings.updateTicks();

	if(mCurMode == EMODE_MAP){

		if(mCurrentBrushTile){
			mCurrentBrushTile->getBrushSelection(cx, cy, mTileMap.TileAreas);
		}

 		if(!mGlobalSettings.bShowTypeSelection) mTileSelectedTile->bIsSelected = false;		
		mTileMap.render(mTileMapScrollX,mGlobalSettings.TopBarHeight+mTileMapScrollY,&mTileSet);
		mTileSelectedTile->bIsSelected = true;

		if(mActiveDialog){			
			mActiveDialog->render();
		}
		if(mActiveMessage){			
			mActiveMessage->render();
		}
		mTopBar.render();
		mTileSet.renderIm(mGlobalSettings.TopBarHeight, mTileSetScrollY);
		if(mGlobalSettings.bShowProjectInfo){
			mProjectInfo.update();
			mProjectInfo.render(0,mGlobalSettings.TopBarHeight);
		}
		if(mGlobalSettings.bShowPaletteOffset){			
			mPaletteOffset.render(mProjectInfo.mDialogWidth,mGlobalSettings.TopBarHeight);
		}

		mSelection.renderSelection();	 

		if(mCurrentBrushTile){
			mCurrentBrushTile->renderSelection();
		}

		if(bShowBrushesTile){
			mBrushesTile.renderIm();
		}
	}
	
	if(mCurMode == EMODE_TILE){
		mPalette.renderIm(100+mGlobalSettings.mTileEdScale*mGlobalSettings.TileSizeX*mGlobalSettings.TileSizeX,50+mTopBar.mDialogHeight);	
		if(!mGlobalSettings.bShowPixelType) mColorSelectedTile->bPixelSelected = false;
		mTileSelectedTile->renderEd(50,50+mTopBar.mDialogHeight,&mPalette);
		mColorSelectedTile->bPixelSelected = true;

		mTileSelectedTile->mSelection.renderSelection();	    

		if(mActiveDialog){
			mActiveDialog->render();
		}
		if(mActiveMessage){
			mActiveMessage->render();
		}
		
		mTopBar.render();

		if(mGlobalSettings.bShowProjectInfo){
			mProjectInfo.update();
			mProjectInfo.render(0,mGlobalSettings.TopBarHeight);
		}		
	}

	if(mCurMode == EMODE_PALED){
		mTopBar.render();
		mPalette.renderEditor(100,100);
		if(mActiveDialog){			
			mActiveDialog->render();
		}
		if(mActiveMessage){			
			mActiveMessage->render();
		}
	}

	return 0;
}

int TEditor::setMode(int newMode){

	if(newMode == EMODE_PALED){
		activatePaletteEdit();
	}

	mLastMode = mCurMode;
	mCurMode = newMode;
	return 0;
}


int TEditor::switchMode(){
	
	if(mLastMode == EMODE_PALED){
		activatePaletteEdit();
	}

	std::swap(mCurMode, mLastMode);
	return 0;
}

int TEditor::selectTiles(std::vector<int> &cNewSelection, int cTileType){
	for(int i = 0; i < (mGlobalSettings.TileMapHeight*mGlobalSettings.TileMapWidth); i++){
		if(mTileMap.getTile(i) == cTileType){
			cNewSelection.push_back(i);
		}
	}
	return cNewSelection.size();
}

int TEditor::searchRectsXY(std::vector<SDL_Rect> &sRects, int mx, int my){
	for(int i = 0; i < sRects.size(); i++){
		if( (mx >= sRects[i].x) && (mx <= (sRects[i].x + sRects[i].w)) && (my >= sRects[i].y) && (my <= (sRects[i].y + sRects[i].h)) ){
			return i;
		}
	}
	return -1;
}

int TEditor::searchRects(std::vector<SDL_Rect> &sRects){
	for(int i = 0; i < sRects.size(); i++){
		if( (mouseSelX >= sRects[i].x) && (mouseSelX <= (sRects[i].x + sRects[i].w)) && (mouseSelY >= sRects[i].y) && (mouseSelY <= (sRects[i].y + sRects[i].h)) ){
			return i;
		}
	}
	return -1;
}

int TEditor::applyScroll(int mx,int my, int amount, int xamount){
	int tSel = -1;
	mouseSelX = mx;
	mouseSelY = my;
	if(mCurMode == EMODE_MAP){ 		
		if(!mGlobalSettings.mio->WantCaptureMouse){
				if(amount > 0){mGlobalSettings.TileMapScale++; if(mGlobalSettings.TileMapScale > 10){mGlobalSettings.TileMapScale=10; }}	
				if(amount < 0){mGlobalSettings.TileMapScale--; if(mGlobalSettings.TileMapScale < 1){mGlobalSettings.TileMapScale=1; }}
		}
	} else {
	
	}
	
	return 0;
}

void TEditor::undoLastActionGroup(){
	if(mCurMode == EMODE_MAP){		
		mActionStack.undoLastActionGroup();
	}
	if(mCurMode == EMODE_TILE){
		mTileSelectedTile->mActionStack.undoLastActionGroup();
	}
}

void TEditor::redoLastActionGroup(){
if(mCurMode == EMODE_MAP){
		mActionStack.redoLastActionGroup();
	}
	if(mCurMode == EMODE_TILE){
		mTileSelectedTile->mActionStack.redoLastActionGroup();
	}
}

int TEditor::activateProjectInfo(){	
	mGlobalSettings.bShowProjectInfo = !mGlobalSettings.bShowProjectInfo;	
	return 0;
}

int TEditor::activateDropUnusedTiles(){
	if(mCurMode == EMODE_MAP){
		mActiveDialog = &mRemoveUnused;
	}
	return 0;
}

int TEditor::activatePaletteUpdate(){
	if(mCurMode == EMODE_PALED){
		mActiveDialog = &mPaletteUpdate;
	}
	return 0;
}


int TEditor::activatePaletteEdit(){
	if(mGlobalSettings.CurrentEditor->mPalette.bUpdateEditColor){		
		mPalette.mEditColor = mPalette.getIm4Color(mPalette.TPaletteEdit[mGlobalSettings.CurrentEditor->mColorSelected]);		
		if(mColorSelectedTileEdit){
			mColorSelectedTileEdit->bPixelSelectedEdit = false;
		}
		mColorSelectedTileEdit = mColorSelectedTile;	
		mColorSelectedTileEdit->bPixelSelectedEdit = true;
		mColorSelectedEdit = mColorSelected;
		mGlobalSettings.CurrentEditor->mPalette.bUpdateEditColor = false;
	}	
	return 0;
}

int TEditor::updatePalette(){
	mActionStack.redoClearStack();	
	mActionStack.undoClearStack();

	mPalette.updatePalette();

	for(int i = 0; i < mTileSet.TTiles.size(); i++){		
		mTileSet.TTiles[i]->updateTexture(&mPalette);		
	}

	return 0;
}

int TEditor::dropUnusedTiles(){	
	std::vector<int> tmpTiles;
	std::set<int> cUnused;

	mActionStack.redoClearStack();	
	mActionStack.undoClearStack();

	mTileSelectedTile->bIsSelected = false;

	mMapSelectedTile = 0;
	mTileSelectedTile = mTileSet.TTiles[0];

	for(int i = 0; i < mTileSet.TTiles.size(); i++){		
		if(selectTiles(tmpTiles, i) == 0){
			cUnused.insert(i);			
		}
		tmpTiles.erase(tmpTiles.begin(), tmpTiles.end());
	}
	
	if(cUnused.size()){
		std::set<int>::reverse_iterator rit;
       	for (rit = cUnused.rbegin(); rit != cUnused.rend(); rit++){
			dropUnusedTile(*rit);             
		}
	}
	return 0;	
}

int TEditor::dropUnusedTile(int cDropTile){	
			
	mTileMap.removeTile(cDropTile);
	mTileSet.removeTile(cDropTile);

	return 0;
}

Tile* TEditor::createNewTile(){
	if(mCurMode == EMODE_MAP){
	Tile* newTile = mTileSet.createNew(&mPalette);
		if(newTile){
				TEActionAddTile* newActionTile = new TEActionAddTile();
				
				newActionTile->doAction(newTile, this, &mTileSet);
      			mActionStack.newActionGroup();	
      			mActionStack.addAction(newActionTile);
      			
				mActionStack.mLastAction = newActionTile;
       			mActionStack.redoClearStack();
				
				return newTile;
		}
	}
	return NULL;
}

int TEditor::rotateTile(){
	if(mCurMode == EMODE_MAP){
		if(mTileSelectedTile){
			mTileSelectedTile->rotate();
			return 0;
		}
	}
	return 1;
}

Tile* TEditor::createNewTileCopy(Tile* cCopyTile){
	if(mCurMode == EMODE_MAP){
	Tile* newTile = mTileSet.createNewCopy(mTileSelectedTile,  &mPalette);
		if(newTile){
				TEActionAddTile* newActionTile = new TEActionAddTile();
				
				newActionTile->doAction(newTile, this, &mTileSet);
      			mActionStack.newActionGroup();	
      			mActionStack.addAction(newActionTile);
      			
				mActionStack.mLastAction = newActionTile;
       			mActionStack.redoClearStack();
				
				return newTile;
		}
	}
	return NULL;
}

Tile* TEditor::createNewTileFromFile(std::string newTilePath){
	return mTileSet.createNewFromFile(newTilePath, &mPalette);
}

int TEditor::showMessage(std::string cMessage, bool isError){
	if(isError){
		mErrorMessage.mDialogTextMain = cMessage;
		mErrorMessage.update();
		mActiveMessage = &mErrorMessage;
	} else {
		mInfoMessage.mDialogTextMain = cMessage;
		mInfoMessage.update();
		mActiveMessage = &mInfoMessage;
	}
	return 0;
}

int TEditor::activateQuitDialog(){
	mActiveDialog = &mQuitDialog;
	return 0;
}


int TEditor::activateHelpDialog(){
	mGlobalSettings.bShowHelpDialog = true;
	mActiveDialog = &mHelpDialog;
	
	return 0;
}

int TEditor::activateSaveDialog(){
	mActiveDialog = &mSaveDialog;
	return 0;
}

int TEditor::activateSaveAsDialog(){
	mActiveDialog = &mSaveAsDialog;
	mActiveDialog->bDialogIsWatingForText = true;	
	return 0;
}

int TEditor::activateOpenTileDialog(){
	if(mCurMode == EMODE_MAP){
		mActiveDialog = &mOpenTileDialog;
		mActiveDialog->bDialogIsWatingForText = true;		
	}
	return 0;
}

int TEditor::activateBrushes(){
	if(mCurMode == EMODE_MAP){
		bShowBrushesTile = !bShowBrushesTile;
	}
	return 0;
}

int TEditor::activateBrush(){
	if(mCurMode == EMODE_MAP){
		//if(mBrushesTile.mBrushes.size() && (mBrushesTile.mSelectedBrush > -1)){
		if(!mCurrentBrushTile){
				mCurrentBrushTile = mBrushesTile.getBrush();
		} else {		
				mCurrentBrushTile = mBrushesTile.getNextBrush();
		}
	}
	return 0;
}




int TEditor::handleSelection(int SELMODE){
	if(mCurMode == EMODE_MAP){
		switch (SELMODE)
		{
			case SELMODE_ALL:
				mSelection.selectRange(0, mGlobalSettings.TileMapWidth *  mGlobalSettings.TileMapHeight);
			break;
			case SELMODE_NONE:
				mSelection.clearSelection();
			break;
			case SELMODE_INVERT:
				mSelection.invertSelection(0, mGlobalSettings.TileMapWidth *  mGlobalSettings.TileMapHeight);
			break;
			default:
			break;
		}
	}

	if(mCurMode == EMODE_TILE){
		switch (SELMODE)
		{
			case SELMODE_ALL:
				mTileSelectedTile->mSelection.selectRange(0, mGlobalSettings.TileSizeX *  mGlobalSettings.TileSizeY);
			break;
			case SELMODE_NONE:
				mTileSelectedTile->mSelection.clearSelection();
			break;
			case SELMODE_INVERT:
				mTileSelectedTile->mSelection.invertSelection(0,  mGlobalSettings.TileSizeX *  mGlobalSettings.TileSizeY);
			break;
			default:
			break;
		}
	}

	return 0;
}

int TEditor::cancelActiveDialog(){
	if(mActiveDialog){	
		mActiveDialog->cancel();
		mActiveDialog = NULL;
	}
	return 0;
}

int TEditor::toggleSelectedTile(){
	if(mCurMode == EMODE_MAP){
		if(mGlobalSettings.mSelectedTile > -1){
			mGlobalSettings.bShowSelectedTile = !mGlobalSettings.bShowSelectedTile;
			return 0;
		}
	}
	return 1;
}
		
int TEditor::flipSelectedTile(){
	if(mCurMode == EMODE_MAP){
		if(mGlobalSettings.mSelectedTile > -1){
			int cTileFlipOld = mTileMap.getFlip(mGlobalSettings.mSelectedTile);			
			int cTileFlip = cTileFlipOld + 1;
			if(cTileFlip > 3){cTileFlip = 0;}

			/**/

				TEActionReplaceTileFlip *mCurAction = new TEActionReplaceTileFlip();
	       		mCurAction->doAction(&mTileMap, mGlobalSettings.mSelectedTile, mTileMap.getTile(mGlobalSettings.mSelectedTile), mTileMap.getTile(mGlobalSettings.mSelectedTile),cTileFlipOld, cTileFlip);
	       			
	       		if(!(*mCurAction == *mActionStack.mLastAction)){
	       			mActionStack.newActionGroup();	
	       			mActionStack.addAction(mCurAction);
	       			mActionStack.mLastAction = mCurAction;
	       			mActionStack.redoClearStack();
	       		}

			/**/

			//mTileMap.setFlip(mGlobalSettings.mSelectedTile, cTileFlip);
			return 0;
		}
	}
	return 1;
}


int TEditor::replaceSelectedColor(int mx, int my){
	
	if(mGlobalSettings.bShowPixelType || mTileSelectedTile->mSelection.mSelected.size()){
			int mOldColor = mColorSelected;
			int tSel = searchRectsXY(mPalette.PixelAreas, mx, my);
			if(tSel > -1){					
				if(mTileSelectedTile->mSelection.mSelected.size()){
					TEActionReplacePixels* newAction = new TEActionReplacePixels();
						newAction->doAction(mTileSelectedTile, mTileSelectedTile->mSelection.mSelected, -1, tSel, &mPalette);
						if(!(newAction == mTileSelectedTile->mActionStack.mLastAction)){
							mTileSelectedTile->mActionStack.mLastAction = newAction;
							mTileSelectedTile->mActionStack.newActionGroup();
							mTileSelectedTile->mActionStack.addSubActions(newAction->mSubActions);
							mTileSelectedTile->mActionStack.redoClearStack();
						}

				} else {
					std::vector<int> newSelection;
					int cPixIndex = 0;
					//for(auto &cPixel : mTileSelectedTile->FileData){
					for(int i = 0; i < (mGlobalSettings.TileSizeX *mGlobalSettings.TileSizeY); i++){
						auto cPixel = mTileSelectedTile->getPixel(i);
						if(cPixel == mOldColor){
							newSelection.push_back(cPixIndex);
						}
						cPixIndex++;
					}
					if(newSelection.size()){
						TEActionReplacePixels* newAction = new TEActionReplacePixels();
						newAction->doAction(mTileSelectedTile, newSelection, mOldColor, tSel, &mPalette);
						if(!(newAction == mTileSelectedTile->mActionStack.mLastAction)){
							mTileSelectedTile->mActionStack.mLastAction = newAction;
							mTileSelectedTile->mActionStack.newActionGroup();
							mTileSelectedTile->mActionStack.addSubActions(newAction->mSubActions);
							mTileSelectedTile->mActionStack.redoClearStack();
						}
					}
				}
			} 
		}

	return 0;
}


int TEditor::replaceSelectedTiles(int mx, int my){
	
	if(mSelection.mSelected.size()){
		int tSel = -1;		
		tSel = searchRectsXY(mTileSet.TileAreas, mx, my);
		if(tSel != -1){ 
			TEActionReplaceTiles* newAction = new TEActionReplaceTiles();
			newAction->doAction(&mTileMap, mSelection.mSelected, -1, tSel);
			if(!(newAction == mActionStack.mLastAction)){
				mActionStack.mLastAction = newAction;
				mActionStack.newActionGroup();
				mActionStack.addSubActions(newAction->mSubActions);
				mActionStack.redoClearStack();
			}				
		}
	} else {
		int tSel = -1;		
		tSel = searchRectsXY(mTileSet.TileAreas, mx, my);
		if(tSel != -1){ 
			if(mMapSelectedTile == tSel) { return 0; }
			std::vector<int> newSelection;
			if(selectTiles(newSelection, mMapSelectedTile)){
				TEActionReplaceTiles* newAction = new TEActionReplaceTiles();
				newAction->doAction(&mTileMap, newSelection, mMapSelectedTile, tSel);
				if(!(newAction == mActionStack.mLastAction)){
					mActionStack.mLastAction = newAction;
					mActionStack.newActionGroup();
					mActionStack.addSubActions(newAction->mSubActions);
					mActionStack.redoClearStack();
				}
			}
		}		
	}
	
	return 0;
}


int TEditor::findSelMap(){

	int tSel = -1;

		if(leftMouseButtonDown && !mGlobalSettings.mio->WantCaptureMouse){
			
			if(bLCTRLisDown){
						if(bTileMapGrapped){
							mTileMapScrollX += rx;
							mTileMapScrollY += ry;;
						} else {
							bTileMapGrapped = true;						
						}				
				} else {					
					
						if(mCurrentBrushTile){
							//mCurBrush->getBrushSelection(cx, cy, mTileMap.TileAreas);
							TEActionBrushTiles* newAction = new TEActionBrushTiles();
							newAction->doAction(&mTileMap, *mCurrentBrushTile);
							if(!(*newAction == *mActionStack.mLastAction)){								
								mActionStack.mLastAction = newAction;
								mActionStack.newActionGroup();
								mActionStack.addSubActions(newAction->mSubActions);
       							mActionStack.redoClearStack();
							}
						} else {
							tSel = searchRectsXY(mTileMap.TileAreas,cx,cy);
	       					if(tSel != -1){
	       						mGlobalSettings.mSelectedTile = tSel;		  	
	       						TEActionReplaceTile *mCurAction = new TEActionReplaceTile();
	       						mCurAction->doAction(&mTileMap, tSel, mTileMap.getTile(tSel), mMapSelectedTile);
	       			
	       						if(!(*mCurAction == *mActionStack.mLastAction)){
	       							mActionStack.newActionGroup();	
	       							mActionStack.addAction(mCurAction);
	       							mActionStack.mLastAction = mCurAction;
	       							mActionStack.redoClearStack();
	       						}
	       					}
						}
	      			}
		    
		}

	return 0;
}

int TEditor::handleTile(){
	findSelTile();
	return 0;
}

int TEditor::findSelTile(){

		if(leftMouseButtonDown && bLShiftIsDown &&!mGlobalSettings.mio->WantCaptureMouse){
			if(!mTileSelectedTile->mSelection.bIsSelecting){
				mTileSelectedTile->mSelection.startSelection(cx, cy);	
			} else {
				mTileSelectedTile->mSelection.updateSelection(cx, cy);
			}
	
		} else {

		
			if(mTileSelectedTile->mSelection.bIsSelecting){
				mTileSelectedTile->mSelection.confirmSelection(mTileSelectedTile->PixelAreas, mGlobalSettings.TileSizeX * mGlobalSettings.mTileEdScale, mGlobalSettings.TileSizeY * mGlobalSettings.mTileEdScale);
			}
		

			if(leftMouseButtonDown && !mGlobalSettings.mio->WantCaptureMouse){
				int tSel = -1;

				tSel = searchRectsXY(mTileSelectedTile->PixelAreas, cx, cy);
				if(tSel != -1){

       				TEActionReplacePixel *mCurAction = new TEActionReplacePixel();
					mCurAction->doAction(mTileSelectedTile, tSel, mTileSelectedTile->getPixel(tSel), mColorSelected, &mPalette);
				
					if(!(*mCurAction == * mTileSelectedTile->mActionStack.mLastAction)){
       					mTileSelectedTile->mActionStack.newActionGroup();	
       					mTileSelectedTile->mActionStack.addAction(mCurAction);
       					mTileSelectedTile->mActionStack.mLastAction = mCurAction;
       					mTileSelectedTile->mActionStack.redoClearStack();
       				}
				}
			}
		}

		if((rightMouseButtonClicks && bLShiftIsDown) && !mGlobalSettings.mio->WantCaptureMouse){		
			int tSel = -1;
			tSel = searchRectsXY(mTileSelectedTile->PixelAreas, cx, cy);
			if(tSel >-1){
				mTileSelectedTile->mSelection.modifySelection(tSel);
			}		
		}

	if(rightMouseButtonDown && !mGlobalSettings.mio->WantCaptureMouse  && !bLShiftIsDown){
		int tSel = -1;
		tSel = searchRectsXY(mTileSelectedTile->PixelAreas, cx, cy);
		if(tSel >-1){
			mColorSelectedTile->bPixelSelected = false;
			mColorSelected = (mTileSelectedTile->getPixel(tSel)+(mGlobalSettings.PaletteOffset*16));
			mColorSelectedTile = mPalette.TPixels[mColorSelected];
			mColorSelectedTile->bPixelSelected = true;
			mTileSelectedTile->mSelection.cancelSelection();
			mGlobalSettings.CurrentEditor->mPalette.bUpdateEditColor = true;
		}
	}

	return 0;
}

int TEditor::handlePaletteEdit(){

	if(ImButtonsPalette.mLeft.bButtonIsDown){
		int tSel = -1;
		tSel = searchRectsXY(mPalette.PixelAreas, ImButtonsPalette.mLeft.mMousePos.x, ImButtonsPalette.mLeft.mMousePos.y);
		if(tSel != -1){
			mColorSelectedTileEdit->bPixelSelectedEdit = false;
			mColorSelectedEdit = tSel;
			mColorSelectedTileEdit = mPalette.TPixels[tSel];
			mColorSelectedTileEdit->bPixelSelectedEdit = true;
			mPalette.mEditColor = mPalette.getIm4Color(mPalette.TPaletteEdit[mGlobalSettings.CurrentEditor->mColorSelectedEdit]);
		}
	}

	if(ImButtonsPalette.mRight.bButtonIsDown){
		int tSel = -1;
		tSel = searchRectsXY(mPalette.PixelAreas, ImButtonsPalette.mRight.mMousePos.x, ImButtonsPalette.mRight.mMousePos.y);
		if(tSel != -1){			
			mPalette.mEditColor = mPalette.getIm4Color(mPalette.TPaletteEdit[tSel]);
		}
	}

	return 0;
}

int TEditor::handlePalette(){

	if(mLastPixelOffset != mGlobalSettings.PaletteOffset){
		int tmpSel = mColorSelected % 16;
		tmpSel += (mGlobalSettings.PaletteOffset * 16);
		mColorSelectedTile->bPixelSelected = false;
		mColorSelected = tmpSel;
		mColorSelectedTile = mPalette.TPixels[tmpSel];
		mColorSelectedTile->bPixelSelected = true;
		mLastPixelOffset = mGlobalSettings.PaletteOffset;
		mGlobalSettings.CurrentEditor->mPalette.bUpdateEditColor = true;
	}

	if(ImButtonsPalette.mLeft.bButtonIsDown){
		int tSel = -1;
		tSel = searchRectsXY(mPalette.PixelAreas, ImButtonsPalette.mLeft.mMousePos.x, ImButtonsPalette.mLeft.mMousePos.y);
		if(tSel != -1){
			mColorSelectedTile->bPixelSelected = false;
			mColorSelected = tSel;
			mColorSelectedTile = mPalette.TPixels[tSel];
			mColorSelectedTile->bPixelSelected = true;
			mGlobalSettings.CurrentEditor->mPalette.bUpdateEditColor = true;
		}
	}

	if(ImButtonsPalette.mRight.bButtonIsDown){
		replaceSelectedColor(ImButtonsPalette.mRight.mMousePos.x, ImButtonsPalette.mRight.mMousePos.y);		
	}
	return 0;
}

int TEditor::handleEMTile(){

	handlePalette();
	handleTile();

	return 0;
}

int TEditor::handleEMMAp(){

	handleTileSet();
	handleTileMap();

	return 0;
}


int TEditor::handleBrushes(){

	if(mCurMode == EMODE_MAP){
		if(ImButtonsBrushesTile.mLeft.bButtonIsDown){ 
			if(ImButtonsBrushesTile.mLeft.mMousePos.y < mBrushesTile.mBrushOffset) {return 0;}
			int tSel = -1;
			tSel = searchRectsXY(mBrushesTile.BrushAreas, ImButtonsBrushesTile.mLeft.mMousePos.x, ImButtonsBrushesTile.mLeft.mMousePos.y);
			if(tSel != -1){
				mBrushesTile.mSelectedBrush = tSel;
				mCurrentBrushTile = mBrushesTile.mBrushes[tSel];
			}		
		}
	}
	return 0;
}

int TEditor::handleTileSet(){

	if(ImButtonsTileSet.mLeft.bButtonIsDown){ 
			int tSel = -1;
			tSel = searchRectsXY(mTileSet.TileAreas, ImButtonsTileSet.mLeft.mMousePos.x, ImButtonsTileSet.mLeft.mMousePos.y);
					if(tSel != -1){
   	 				mMapSelectedTile = tSel;
   	 				mTileSelectedTile->bIsSelected = false;
   	 				mTileSelectedTile = mTileSet.TTiles[tSel];
   	 				mTileSelectedTile->bIsSelected = true;
			}		

	}
	if(ImButtonsTileSet.mRight.bButtonIsDown && mBrushesTile.bIsEditing){
		int tSel = -1;		
		tSel = searchRectsXY(mTileSet.TileAreas, cx, cy);
		mBrushesTile.addBrushElement(tSel);
	} else if(ImButtonsTileSet.mRight.bButtonIsDown && (mGlobalSettings.bShowTypeSelection || mSelection.mSelected.size())){
		replaceSelectedTiles(ImButtonsTileSet.mRight.mMousePos.x ,ImButtonsTileSet.mRight.mMousePos.y);
	}


	return 0;
}


int TEditor::handleTileMap(){

	if((rightMouseButtonClicks && bLShiftIsDown) && !mGlobalSettings.mio->WantCaptureMouse){
		int tSel = -1;
		tSel = searchRectsXY(mTileMap.TileAreas, cx, cy);
	    if(tSel != -1){	    	
			mGlobalSettings.bShowSelectedTile = false;
			mSelection.modifySelection(tSel);
		}
	} else {
		int tSel = -1;

		if(rightMouseButtonDown && !mGlobalSettings.mio->WantCaptureMouse && !bLShiftIsDown){
		tSel = searchRectsXY(mTileMap.TileAreas, cx, cy);
	    if(tSel != -1){
	    	mGlobalSettings.mSelectedTile = tSel;
			mGlobalSettings.bShowSelectedTile = true;
			if(mGlobalSettings.bShowTypeSelection){
				mMapSelectedTile = mTileMap.getTile(tSel);
   	 			mTileSelectedTile->bIsSelected = false;
   	 			mTileSelectedTile = mTileSet.TTiles[mMapSelectedTile];
   	 			mTileSelectedTile->bIsSelected = true;				
			}		
			mSelection.cancelSelection();
		}
	}
	}

	if(leftMouseButtonDown && bLShiftIsDown && !mGlobalSettings.mio->WantCaptureMouse){
		if(!mSelection.bIsSelecting){
			mSelection.startSelection(cx, cy);	
		} else {
			mSelection.updateSelection(cx, cy);
		}
	} else {
		if(mSelection.bIsSelecting){
			mSelection.confirmSelection(mTileMap.TileAreas, mGlobalSettings.TileSizeX * mGlobalSettings.TileMapScale,  mGlobalSettings.TileSizeY * mGlobalSettings.TileMapScale);
		} else {
			findSelMap();
		}
	}

	return 0;
}

int TEditor::handleEvents(){
	
	SDL_GetRelativeMouseState(&rx, &ry);
	mButtonState = SDL_GetMouseState(&cx, &cy);
	int mapWidthX = mGlobalSettings.TileMapWidth*mGlobalSettings.TileSizeX*mGlobalSettings.TileMapScale;					
	int mapWidthY = mGlobalSettings.TileMapHeight*mGlobalSettings.TileSizeY*mGlobalSettings.TileMapScale;
	
	if(mButtonState & SDL_BUTTON(SDL_BUTTON_LEFT)){
		leftMouseButtonDown = true;
	} else {		
		leftMouseButtonDown = false;		
		bTileMapGrapped = false;
	}

	if(mButtonState & SDL_BUTTON(SDL_BUTTON_RIGHT)){
		rightMouseButtonDown = true;
	} else {
		rightMouseButtonDown = false;		
	}
	
	if(mActiveMessage){
		if(mActiveMessage->bInputIsCancel){
			mActiveMessage->cancel();
			mActiveMessage = NULL;
		}
	}

	if(mActiveDialog){
		if(mActiveDialog->bInputIsAccept){
			if(mGlobalSettings.mDeleteUnusedTilesState){
				dropUnusedTiles();
				mGlobalSettings.mDeleteUnusedTilesState = 0;
				//return 0;
			}
			if(mGlobalSettings.mProjectSaveState == 1){
				if(saveToFolder(mGlobalSettings.ProjectPath)){					
					showMessage("Error Creating Project Folder!", true);
				} 
				mGlobalSettings.mProjectSaveState = 0;				
				//return 0;
			}
			if(mGlobalSettings.mPaletteUpdateState == 1){
				updatePalette();
				mGlobalSettings.mPaletteUpdateState = 0;
				//return 0;
			}
			if(mGlobalSettings.mOpenTileState == 1){
				Tile* newTile = createNewTileFromFile(mGlobalSettings.mNewTilePath);
				if(newTile){
					TEActionAddTile* newActionTile = new TEActionAddTile();
					newActionTile->doAction(newTile, this, &mTileSet);
	       			mActionStack.newActionGroup();	
	       			mActionStack.addAction(newActionTile);
	       			mActionStack.mLastAction = newActionTile;
	       			mActionStack.redoClearStack();

					mGlobalSettings.mOpenTileState = 0;
					cancelActiveDialog();
					showMessage("Tile Loaded Successfully");
					return 0;
				} else {
					mGlobalSettings.mOpenTileState = 0;
					cancelActiveDialog();
					showMessage("Error Loading Tile!", true);					
					return 0;
				}
				mGlobalSettings.mOpenTileState = 0;
			}
			cancelActiveDialog();
			return 0;
		}
		if(mActiveDialog->bInputIsCancel){
			cancelActiveDialog();
			return 0;
		}
		
		return 0;
	} else {		
			if(mCurMode == EMODE_MAP){
				handleBrushes();
				handleEMMAp();
			}
			if(mCurMode == EMODE_TILE){
				handleEMTile();
			}
			if(mCurMode == EMODE_PALED){
				handlePaletteEdit();
			}		
	}


	if(mTileMapScrollX > 0){mTileMapScrollX = 0;}
	if(mTileMapScrollY > 0){mTileMapScrollY = 0;}
	
	if(mTileMapScrollX < -(mapWidthX - (mGlobalSettings.WindowWidth-mGlobalSettings.TileSetWidth))){mTileMapScrollX = -(mapWidthX - (mGlobalSettings.WindowWidth-mGlobalSettings.TileSetWidth));}
	if(mTileMapScrollY < -(mapWidthY - (mGlobalSettings.WindowHeight - mGlobalSettings.TopBarHeight))){mTileMapScrollY = -(mapWidthY - (mGlobalSettings.WindowHeight - mGlobalSettings.TopBarHeight));}
	
	if(mapWidthX < (mGlobalSettings.WindowWidth-mGlobalSettings.TileSetWidth)){
		mTileMapScrollX = -(mapWidthX - (mGlobalSettings.WindowWidth-mGlobalSettings.TileSetWidth))/2;
	}
	
	if(mapWidthY < (mGlobalSettings.WindowHeight-mGlobalSettings.TopBarHeight)){
		mTileMapScrollY = -(mapWidthY - (mGlobalSettings.WindowHeight-mGlobalSettings.TopBarHeight))/2;
	}

	leftMouseButtonClicks = 0;
	rightMouseButtonClicks = 0;

	return 0;	
}

int TEditor::handleEvents(SDL_Event* cEvent){

	switch (cEvent->type){
		case SDL_WINDOWEVENT:
			if (cEvent->window.event == SDL_WINDOWEVENT_RESIZED){
				resizeWindowEvent(cEvent);
			}
			break;
		case SDL_TEXTINPUT:
	  		if(mActiveDialog){
				
			}
			break;
		case SDL_TEXTEDITING:
	  		if(mActiveDialog){
	  			
			}
			break;
		case SDL_QUIT:
			activateQuitDialog();		
	  		//bEditorRunning = false;
  			//std::cout << "Shutting Down..." << std::endl;
  			break;
			
	  	case SDL_KEYDOWN:
			if(mActiveMessage){
				mActiveMessage->recieveInput(SDLK_n);
				break;
			}
	  		if(mActiveDialog){
	  			if(cEvent->key.keysym.sym == SDLK_BACKSPACE){
	  				if(mActiveDialog->bDialogIsWatingForText){				
	  					mActiveDialog->dropLastInputChar();
	  				}
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_RETURN){
	  					mActiveDialog->recieveInput(SDLK_y);
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_ESCAPE){
					mActiveDialog->recieveInput(SDLK_n);
	  			}
				if(cEvent->key.keysym.sym == SDLK_TAB){
					mActiveDialog->recieveInput(SDLK_TAB);
	  			}		  			
	  		} else {
	  			if(cEvent->key.keysym.sym == SDLK_ESCAPE){
					if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_MAP){
						if(mGlobalSettings.CurrentEditor->bShowBrushesTile){
							mGlobalSettings.CurrentEditor->bShowBrushesTile = false;
							return 0;
						}
						if(mCurrentBrushTile){
							mCurrentBrushTile = NULL;
							return 0;
						}
					}
					
					activateQuitDialog();					
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_SPACE){
	  				switchMode();
	  			}
				if(cEvent->key.keysym.sym == SDLK_s){
					if(mCurMode == EMODE_MAP) mGlobalSettings.bShowTypeSelection = !mGlobalSettings.bShowTypeSelection;
					if(mCurMode == EMODE_TILE) mGlobalSettings.bShowPixelType = !mGlobalSettings.bShowPixelType;					
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_p){
		  			if(mCurMode == EMODE_TILE) mGlobalSettings.bShowPixelGrip = !mGlobalSettings.bShowPixelGrip;
	  			}	  			  		
	  			if(cEvent->key.keysym.sym == SDLK_LCTRL){
	  				bLCTRLisDown = true;
	  			}
				if(cEvent->key.keysym.sym == SDLK_LSHIFT){
	  				bLShiftIsDown = true;
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_u){
					undoLastActionGroup();	  		
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_r){
		  			redoLastActionGroup();
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_f){
		  			flipSelectedTile();
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_t){
		  			toggleSelectedTile();
	  			}
				if(cEvent->key.keysym.sym == SDLK_a){
					handleSelection(SELMODE_ALL);
				}
				if(cEvent->key.keysym.sym == SDLK_i){
					handleSelection(SELMODE_INVERT);
				}
				if(cEvent->key.keysym.sym == SDLK_n){
					handleSelection(SELMODE_NONE);
				}
	  			if(cEvent->key.keysym.sym == SDLK_F1){	  				
		  			activateHelpDialog();
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_F2){	  				
		  			activateProjectInfo();
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_F3){	  				
		  			createNewTile();
	  			}
				if(cEvent->key.keysym.sym == SDLK_F4){	  				
					activateOpenTileDialog();		  			
	  			}
				if(cEvent->key.keysym.sym == SDLK_F5){	  									
					createNewTileCopy(mTileSelectedTile);
	  			}
				if(cEvent->key.keysym.sym == SDLK_F6){	  									
					rotateTile();
	  			}
				if(cEvent->key.keysym.sym == SDLK_F7){	  									
					activateDropUnusedTiles();
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_F12){	  				
		  			activateSaveDialog();
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_F11){	  				
		  			activateSaveAsDialog();
	  			}
				if(cEvent->key.keysym.sym == SDLK_F8){	  				
					activateBrushes();
	  			}
				if(cEvent->key.keysym.sym == SDLK_F10){	  				
					if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_MAP){
						if(mCurrentBrushTile){
							mCurrentBrushTile = NULL;
						}	  			}
				}
				if(cEvent->key.keysym.sym == SDLK_F9){	  				
					activateBrush();
	  			}	  			
			}
	  		break;
	  	case SDL_KEYUP:
	  		if(cEvent->key.keysym.sym == SDLK_LCTRL){
	  			bLCTRLisDown = false;
	  			bTileSetGrapped = false;
	  			bTileMapGrapped = false;
	  		}
			if(cEvent->key.keysym.sym == SDLK_LSHIFT){
	  			bLShiftIsDown = false;
			}
	  		break;
	  	case SDL_MOUSEBUTTONDOWN:
	        if (cEvent->button.button == SDL_BUTTON_LEFT){	  		
			    mouseSelX = cEvent->motion.x;
			    mouseSelY = cEvent->motion.y;			    
	    	}
	    	break;
	    	case SDL_MOUSEBUTTONUP:
	        if (cEvent->button.button == SDL_BUTTON_LEFT){
	  			leftMouseButtonClicks = cEvent->button.clicks;
	    	}
			if (cEvent->button.button == SDL_BUTTON_RIGHT){
	  			rightMouseButtonClicks = cEvent->button.clicks;
	    	}
	    	break;	    		
	    	case SDL_MOUSEWHEEL:
	    		int x,y;
				SDL_GetMouseState(&x, &y);
	    		applyScroll(x,y,cEvent->wheel.y,cEvent->wheel.x);
	    		break;
	}
	return 0;
}

int TEditor::resizeWindowEvent(SDL_Event* event){
  	
	SDL_Window* win = SDL_GetWindowFromID(event->window.windowID);    
	if(win != mGlobalSettings.TWindow){return 1;}
 	
	int newWidth,newHeight;
 	SDL_GetWindowSize(mGlobalSettings.TWindow, &newWidth, &newHeight);
 	mGlobalSettings.WindowWidth = newWidth;
 	mGlobalSettings.WindowHeight = newHeight; 
		
	mTileSet.updateWinPos = true;
	mTileSet.reCalculateScale();

	mPaletteOffset.bUpdateWinPos = true;
	mHelpDialog.bUpdateWinPos = true;
	mProjectInfo.bUpdateWinPos = true;
	mInfoMessage.bUpdateWinPos = true;
	mRemoveUnused.bUpdateWinPos = true;
	mErrorMessage.bUpdateWinPos = true;
	mQuitDialog.bUpdateWinPos = true;
	mPaletteUpdate.bUpdateWinPos = true;
	mSaveDialog.bUpdateWinPos = true;;
	mSaveAsDialog.bUpdateWinPos = true;;
	mOpenTileDialog.bUpdateWinPos = true;;

	return 0;
}
