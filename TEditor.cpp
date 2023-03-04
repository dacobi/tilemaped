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
	mTileMap = new TileMap();
	mTileMap->createNew();
	mTileMaps.push_back(mTileMap);		

	if(fs::exists(fs::status(mGlobalSettings.mNewTilePath))){
		std::vector<Tile*> cNewTiles;
		if(mTileSet.importTileSet(mGlobalSettings.mNewTilePath, cNewTiles)){
			return 1;
		}
	} else if(mGlobalSettings.mNewTileSize){
		if((mGlobalSettings.mNewTileSize == 4) || (mGlobalSettings.mNewTileSize == 8) || (mGlobalSettings.mNewTileSize == 16)){
			mTileSet.mSelEdWidth = mGlobalSettings.mNewTileSize;
			mGlobalSettings.mTileSetEditWidth = mGlobalSettings.mNewTileSize;
			for(int i = 0; i < (mGlobalSettings.mNewTileSize*mGlobalSettings.mNewTileSize); i++){
				mTileSet.createNew(&mPalette);		
			}			
		} else {
			return 1;
		}
	} else {
		mTileSet.createNew(&mPalette);
	}

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
	mOpenTileSetDialog.init();
	mOpenTileMapDialog.init();
	mNewTileMapDialog.init();
	mInfoMessage.init();
	mErrorMessage.setColorScheme(1);
	mErrorMessage.init();

	mRemoveTileMap.init();
	mRemoveUnused.init();
	mRemoveSelUnused.init();

	mPaletteUpdate.init();

	mInputNumber.init();

	mPaletteOffset.init();

	if(mGlobalSettings.TileSetBPP < 0x8){
		mGlobalSettings.bShowPaletteOffset = true;
	}

	mBrushesTile.init("Tiles","Tile", TBRUSH_TILE, &bShowBrushesTile,mGlobalSettings.TileSizeX, mGlobalSettings.TileSizeY, &mGlobalSettings.TileMapScale, mGlobalSettings.TileMapScale, &mCurrentBrushTile);
	mBrushesPixel.init("Pixels","Pixel", TBRUSH_PIXEL, &bShowBrushesPixel, mGlobalSettings.TilePixelSize, mGlobalSettings.TilePixelSize, &mGlobalSettings.mTileEdScale, mGlobalSettings.mTileEdScale, &mCurrentBrushPixel);
	
}

int TEditor::createTileMap(int nMapX, int nMapY, int nTileValue, int cPaletteOffset){

	TileMap* cNewMap = new TileMap();

	cNewMap->createNew(nMapX, nMapY, nTileValue, cPaletteOffset);

	mTileMaps.push_back(cNewMap);

	switchTileMap(mTileMaps.size()-1);

	return 0;
}

int TEditor::createTileMap(int nMapX, int nMapY, int nTileValue){

	TileMap* cNewMap = new TileMap();

	cNewMap->createNew(nMapX, nMapY, nTileValue);

	mTileMaps.push_back(cNewMap);

	switchTileMap(mTileMaps.size()-1);

	return 0;
}

int TEditor::importTileMap(std::string cNewTileMap, int cTileOffset, int cPaletteOffset){
	if(fs::exists(fs::status(cNewTileMap))){
		TileMap* cNewMap = new TileMap();
		fs::path cNewPath = cNewTileMap;
		if(cNewMap->loadFromFileOffset(cNewPath.parent_path().string(), cNewPath.filename().string(), cTileOffset, cPaletteOffset)){
			std::cout << "Error Importing TileMap: " << cNewTileMap << std::endl;
			return 1;
		}
		for(int i = 0; i < cNewMap->TileMapHeight; i++){
			for(int j = 0; j < cNewMap->TileMapWidth; j++){
				if(cNewMap->getTile((i * cNewMap->TileMapWidth) + j) >= mTileSet.TTiles.size()){
					std::cout << "Error Importing TileMap, Tiles out of bound: " << cNewTileMap << std::endl;
					return 2;
				}
			}
		}

		cNewMap->bIsSavedToFile = false;

		mTileMaps.push_back(cNewMap);		
		return 0;
	}

	return 1;
}

int TEditor::importTileMap(std::string cNewTileMap, int cTileOffset){
		if(fs::exists(fs::status(cNewTileMap))){
		TileMap* cNewMap = new TileMap();
		fs::path cNewPath = cNewTileMap;
		if(cNewMap->loadFromFileOffset(cNewPath.parent_path().string(), cNewPath.filename().string(), cTileOffset)){
			std::cout << "Error Importing TileMap: " << cNewTileMap << std::endl;
			return 1;
		}
		for(int i = 0; i < cNewMap->TileMapHeight; i++){
			for(int j = 0; j < cNewMap->TileMapWidth; j++){
				if(cNewMap->getTile((i * cNewMap->TileMapWidth) + j) >= mTileSet.TTiles.size()){
					std::cout << "Error Importing TileMap, Tiles out of bound: " << cNewTileMap << std::endl;
					return 2;
				}
			}
		}

		cNewMap->bIsSavedToFile = false;

		mTileMaps.push_back(cNewMap);		
		return 0;
	}

	return 1;
}


int TEditor::importTileMap(std::string cNewTileMap){
	if(fs::exists(fs::status(cNewTileMap))){
		TileMap* cNewMap = new TileMap();
		fs::path cNewPath = cNewTileMap;
		if(cNewMap->loadFromFile(cNewPath.parent_path().string(), cNewPath.filename().string())){
			std::cout << "Error Importing TileMap: " << cNewTileMap << std::endl;
			return 1;
		}
		for(int i = 0; i < cNewMap->TileMapHeight; i++){
			for(int j = 0; j < cNewMap->TileMapWidth; j++){
				if(cNewMap->getTile((i * cNewMap->TileMapWidth) + j) >= mTileSet.TTiles.size()){
					std::cout << "Error Importing TileMap, Tiles out of bound: " << cNewTileMap << std::endl;
					return 2;
				}
			}
		}

		cNewMap->bIsSavedToFile = false;

		mTileMaps.push_back(cNewMap);		
		return 0;
	}

	return 1;
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
			
	if(mTileSet.loadFromFile(path,"tiles.bin", &mPalette)){
		std::cout << "Error loading tiles: " << path << DIRDEL << "tiles.bin" << std::endl;
		return 1;
	}

	//TODO maybe

	mTileMap = new TileMap();

	if(mTileMap->loadFromFile(path, "map.bin")){
		std::cout << "Error: can't read: " << path << DIRDEL << "map.bin" << std::endl;
		return 1;
	}

	mTileMaps.push_back(mTileMap);

	fs::path cTileMaps;
	int mMapNum = 0;
	std::string cMapNum;
	std::stringstream convert;

	convert << mMapNum << std::endl;
	convert >> cMapNum;

	cTileMaps = std::string(path + DIRDEL + "map" + cMapNum + ".bin");

	while(fs::exists(fs::status(cTileMaps))){
		TileMap *newTileMap = new TileMap();
		if(newTileMap->loadFromFile(cTileMaps.parent_path().string(), cTileMaps.filename().string())){
			std::cout << "Error: can't read: " << cTileMaps.parent_path().string() << DIRDEL << cTileMaps.filename().string() << std::endl;
			return 1;
		}
		mTileMaps.push_back(newTileMap);
		mMapNum++;
		convert << mMapNum << std::endl;
		convert >> cMapNum;

		cTileMaps = std::string(path + DIRDEL + "map" + cMapNum + ".bin");
	}

	mGlobalSettings.mTileMapFileCount = mTileMaps.size();

	mGlobalSettings.ProjectPath = path;

	if(fs::exists(fs::status(path + DIRDEL + "settings.ini"))){
		if(mGlobalSettings.ProjectSettings.load(path + DIRDEL + "settings.ini")){
			std::cout << "Error Loading Settings" << std::endl;
		} else {
			std::cout << "Loading Settings" << std::endl;
			mGlobalSettings.mSelectionMode = mGlobalSettings.ProjectSettings.Editor_SelectionAppend->getBool();
			mGlobalSettings.bTileSetOrderUpdateTileMap = mGlobalSettings.ProjectSettings.TileSet_UpdateMaps->getBool();
			mGlobalSettings.bTileSetWarnBeforeDelete = mGlobalSettings.ProjectSettings.TileSet_WarnBeforeDelete->getBool();
			mTileSet.mSelEdWidth = mGlobalSettings.ProjectSettings.TileSet_EditWidth->getInteger();
			mGlobalSettings.mTileSetEditWidth = mTileSet.mSelEdWidth;
			mTileSet.resizeEdit();		
		}
	}

	
	
	initDialogs();

	switchTileMap(0);

	if(fs::exists(fs::status(path+DIRDEL+"tbrushes.dat"))){
		mBrushesTile.loadFromFile(path+DIRDEL+"tbrushes.dat");
	}

	if(fs::exists(fs::status(path+DIRDEL+"pbrushes.dat"))){
		mBrushesPixel.loadFromFile(path+DIRDEL+"pbrushes.dat");
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
	//TODO maybe
	mTileMaps[0]->saveToFolder(path, "map.bin");

	int mMapNum = 0;
	std::string cTileMap;
	std::string cMapNum;
	std::stringstream convert;

	if(mTileMaps.size() > 1){
		for(int i = 1; i < mTileMaps.size(); i++){
			convert << mMapNum << std::endl;
			convert >> cMapNum;
			cTileMap = "map" + cMapNum + ".bin";
			mTileMaps[i]->saveToFolder(path, cTileMap);
			mMapNum++;
		}
	}

	std::string cTileMaps;
	std::string cBackup = ".bak";	
	convert << mMapNum << std::endl;
	convert >> cMapNum;
	cTileMaps = std::string(path + DIRDEL + "map" + cMapNum + ".bin");

	while(fs::exists(fs::status(cTileMaps))){
		std::string cBackupNum;
		int mBackupNum = 0;

		convert << mBackupNum << std::endl;
		convert >> cBackupNum;

		while(fs::exists(fs::status(cTileMaps + cBackup + cBackupNum))){
			mBackupNum++;
			convert << mBackupNum << std::endl;
			convert >> cBackupNum;
		}


		fs::rename(cTileMaps, cTileMaps + cBackup + cBackupNum);
	
		mMapNum++;
		convert << mMapNum << std::endl;
		convert >> cMapNum;

		cTileMaps = std::string(path + DIRDEL + "map" + cMapNum + ".bin");
	}

	mGlobalSettings.mTileMapFileCount = mTileMaps.size();

	mPalette.saveToFolder(path);

	mBrushesTile.saveToFile(path + DIRDEL + "tbrushes.dat");
	mBrushesPixel.saveToFile(path + DIRDEL + "pbrushes.dat");


	mGlobalSettings.ProjectSettings.Editor_SelectionAppend->ivalue = mGlobalSettings.mSelectionMode;
 	mGlobalSettings.ProjectSettings.TileSet_UpdateMaps->ivalue = mGlobalSettings.bTileSetOrderUpdateTileMap;
	mGlobalSettings.ProjectSettings.TileSet_WarnBeforeDelete->ivalue = mGlobalSettings.bTileSetWarnBeforeDelete;
 	mGlobalSettings.ProjectSettings.TileSet_EditWidth->ivalue = mTileSet.mSelEdWidth;

	mGlobalSettings.ProjectSettings.writedefault(path + DIRDEL + "settings.ini");

	return 0;
}

int TEditor::switchTileMap(int cTileMap){
	if((cTileMap > -1) &&  (cTileMap < mTileMaps.size())){
		mTileMap->mMapSelectedTile = mMapSelectedTile;
		mTileMap->TileMapScale = mGlobalSettings.TileMapScale;
		mTileMap->mTileMapScrollX = mTileMapScrollX;
		mTileMap->mTileMapScrollY = mTileMapScrollY;
		mTileMap = mTileMaps[cTileMap];
		mMapSelectedTile = mTileMap->mMapSelectedTile;
		mTileSelectedTile->bIsSelected = false;
		mTileSelectedTile = mTileSet.TTiles[mMapSelectedTile];
		mTileSelectedTile->bIsSelected = true;
		mGlobalSettings.TileMapWidth = mTileMap->TileMapWidth;
		mGlobalSettings.TileMapHeight = mTileMap->TileMapHeight;
		mTileMapScrollX = mTileMap->mTileMapScrollX;
		mTileMapScrollY = mTileMap->mTileMapScrollY;
		mGlobalSettings.TileMapScale = mTileMap->TileMapScale;

		bTileMapWasChanged = true;
		return 0;
	}
	return 1;
}

int TEditor::render(){
	mGlobalSettings.updateTicks();

	if(mCurMode == EMODE_MAP){

		
 		if(!mGlobalSettings.bShowTypeSelection) mTileSelectedTile->bIsSelected = false;		
		mTileMap->render(mTileMapScrollX,mGlobalSettings.TopBarHeight+mTileMapScrollY,&mTileSet);
		mTileSelectedTile->bIsSelected = true;

		if(mCurrentBrushTile){
			mCurrentBrushTile->getBrushSelection(cx, cy, mTileMap->TileAreas);
		}

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

		mTileMap->mSelection.renderSelection();	 

		if(mCurrentBrushTile){
			mCurrentBrushTile->renderSelection();
		}

		if(bShowBrushesTile){
			mBrushesTile.renderIm();
		}
	}
	
	if(mCurMode == EMODE_TILE){
		if(mCurrentBrushPixel){
			mCurrentBrushPixel->getBrushSelection(cx, cy, mTileSelectedTile->PixelAreas);
		}

		mPalette.renderIm(100+mGlobalSettings.mTileEdScale*mGlobalSettings.TileSizeX*mGlobalSettings.TilePixelSize,50+mTopBar.mDialogHeight);	
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

		if(mCurrentBrushPixel){
			mCurrentBrushPixel->renderSelection();
		}

		if(bShowBrushesPixel){
			mBrushesPixel.renderIm();
		}
	}

	if(mCurMode == EMODE_TILESET){		
		mTopBar.render();

		if(mCurrentBrushPixelTileSet){
			mCurrentBrushPixelTileSet->getBrushSelection(cx, cy, mTileSet.EditPixelAreas);
		}

		mPalette.renderIm(100 + (mGlobalSettings.CurrentEditor->mTileSet.mCurEdScale * mGlobalSettings.CurrentEditor->mTileSet.mSelectionAreaX),50+mTopBar.mDialogHeight);			
		
		if(!mGlobalSettings.bShowPixelType) mColorSelectedTile->bPixelSelected = false;
		mTileSet.renderEd(mTileSetScrollX + 50,mTileSetScrollY + 50+mTopBar.mDialogHeight);
		mColorSelectedTile->bPixelSelected = true;

		if(mActiveDialog){			
			mActiveDialog->render();
		}
		if(mActiveMessage){			
			mActiveMessage->render();
		}

		if(mGlobalSettings.bShowProjectInfo){
			mProjectInfo.update();
			mProjectInfo.render(0,mGlobalSettings.TopBarHeight);
		}

		if(mCurrentBrushPixelTileSet){
			mCurrentBrushPixelTileSet->renderSelection();
		}

		if(bShowBrushesPixelTileSet){
			mBrushesPixel.renderIm();
		}


		mTileSet.mSelection.renderSelection();	 
	}

	if(mCurMode == EMODE_SELEDIT){		
		mTopBar.render();

		if(mCurrentBrushPixelSelEdit){
			mCurrentBrushPixelSelEdit->getBrushSelection(cx, cy, mSelEdit.EditPixelAreas);
		}

		mPalette.renderIm(100 + (mGlobalSettings.CurrentEditor->mSelEdit.mCurEdScale * mGlobalSettings.CurrentEditor->mSelEdit.mSelectionAreaX),50+mTopBar.mDialogHeight);			
		
		if(!mGlobalSettings.bShowPixelType) mColorSelectedTile->bPixelSelected = false;
		mSelEdit.renderEd(50 + mSelEditScrollX ,50 + mSelEditScrollY +mTopBar.mDialogHeight);
		mColorSelectedTile->bPixelSelected = true;

		if(mActiveDialog){			
			mActiveDialog->render();
		}
		if(mActiveMessage){			
			mActiveMessage->render();
		}

		if(mGlobalSettings.bShowProjectInfo){
			mProjectInfo.update();
			mProjectInfo.render(0,mGlobalSettings.TopBarHeight);
		}

		if(mCurrentBrushPixelSelEdit){
			mCurrentBrushPixelSelEdit->renderSelection();
		}

		if(bShowBrushesPixelSelEdit){
			mBrushesPixel.renderIm();
		}


		mSelEdit.mSelection.renderSelection();	 
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

	if(newMode == EMODE_TILE){
		mBrushesPixel.setBrushDeltas(mGlobalSettings.TilePixelSize, mGlobalSettings.TilePixelSize, &mGlobalSettings.mTileEdScale, mGlobalSettings.mTileEdScale);
		mBrushesPixel.bIsShown = &bShowBrushesPixel;
	}

	if(newMode == EMODE_TILESET){
		mBrushesPixel.setBrushDeltas(1, 1, &mTileSet.mCurEdScale, mGlobalSettings.mTileEdScale * mGlobalSettings.TilePixelSize);
		mBrushesPixel.bIsShown = &bShowBrushesPixelTileSet;
	}

	if(newMode == EMODE_SELEDIT){		
		int firstx, firsty, lastx, lasty;
		int width, height;

    	if(mTileMap->mSelection.isSelectionRectangular(firstx, firsty, lastx, lasty)){
			
			width = lastx-firstx;
			height = lasty-firsty;
			if(bTileMapWasChanged || (firstx != mLastSelEditX) || (firsty != mLastSelEditY) || (width != mLastSelEditWidth)|| (height != mLastSelEditHeight) ){							
				bTileMapWasChanged = false;

				mLastSelEditX=firstx;
				mLastSelEditY=firsty;
				mLastSelEditWidth=width;
				mLastSelEditHeight=height;

				if((width > 15) || (height > 15)){
					showMessage("Selection is to big");
					bTileMapWasChanged = true;
					return 1;		
				}

				mSelEdit.setSelection(&mTileMap->mSelection, width+1, height+1);
				mPalette.bUpdateWinPos = true;
			}        		        	

			mBrushesPixel.setBrushDeltas(1, 1, &mSelEdit.mCurEdScale, mGlobalSettings.mTileEdScale * mGlobalSettings.TilePixelSize);
			mBrushesPixel.bIsShown = &bShowBrushesPixelSelEdit;

    	} else {        	
			showMessage("Selection is invalid");
			bTileMapWasChanged = true;
			return 1;
    	}
	}

	mLastMode = mCurMode;
	mCurMode = newMode;
	return 0;
}


int TEditor::switchMode(){	
	return setMode(mLastMode);
}

int TEditor::selectTilesMap(TileMap* cTileMap, std::vector<int> &cNewSelection, int cTileType){
	for(int i = 0; i < (cTileMap->TileMapHeight*cTileMap->TileMapWidth); i++){
		if(cTileMap->getTile(i) == cTileType){
			cNewSelection.push_back(i);
		}
	}
	return cNewSelection.size();
}

int TEditor::selectTiles(std::vector<int> &cNewSelection, int cTileType){
	for(int i = 0; i < (mGlobalSettings.TileMapHeight*mGlobalSettings.TileMapWidth); i++){
		if(mTileMap->getTile(i) == cTileType){
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
	} 

	if(mCurMode == EMODE_TILESET){ 		
		if(!mGlobalSettings.mio->WantCaptureMouse){
			if(amount > 0){
				mGlobalSettings.mTileSetEditScale++; 
				mTileSet.bUpdateEditSelectionScale = true;
				if(mGlobalSettings.mTileSetEditScale > 24){
					mGlobalSettings.mTileSetEditScale = 24; 
				}
			}	
			if(amount < 0){
				mGlobalSettings.mTileSetEditScale--; 
				mTileSet.bUpdateEditSelectionScale = true;
				if(mGlobalSettings.mTileSetEditScale < 2){
					mGlobalSettings.mTileSetEditScale = 2; 
				}
			}
		}
	}

	if(mCurMode == EMODE_SELEDIT){ 		
		if(!mGlobalSettings.mio->WantCaptureMouse){
			if(amount > 0){
				mGlobalSettings.mSelectionEditScale++; 
				mSelEdit.bUpdateEditSelectionScale = true;
				if(mGlobalSettings.mSelectionEditScale > 24){
					mGlobalSettings.mSelectionEditScale = 24; 
				}
			}	
			if(amount < 0){
				mGlobalSettings.mSelectionEditScale--; 
				mSelEdit.bUpdateEditSelectionScale = true;
				if(mGlobalSettings.mSelectionEditScale < 2){
					mGlobalSettings.mSelectionEditScale = 2; 
				}
			}
		}
	}
	
	return 0;
}

void TEditor::undoLastActionGroup(){
	if((mCurMode == EMODE_MAP) || (mCurMode == EMODE_SELEDIT)){
		TEActionGroup *mGroup;
		bool bMapSwitch = false;
		if(mActionStack.mUndoStack.size()){
			mGroup = *(mActionStack.mUndoStack.end()-1);
			if(mGroup->mActions.size()){
				if(mGroup->mActions[0]->checkTileMap()){
					switchTileMap(mGroup->mActions[0]->getTileMap());
					bMapSwitch = true;
				}
			}
		}
		
		if(!bMapSwitch){
			mActionStack.undoLastActionGroup();
		}
	}
	if(mCurMode == EMODE_TILE){
		mTileSelectedTile->mActionStack.undoLastActionGroup();
	}
	if(mCurMode == EMODE_TILESET){
		mTileSet.mActionStack.undoLastActionGroup();
	}	
}

void TEditor::redoLastActionGroup(){
if((mCurMode == EMODE_MAP) || (mCurMode == EMODE_SELEDIT)){
		TEActionGroup *mGroup;
		bool bMapSwitch = false;
		if(mActionStack.mRedoStack.size()){
			mGroup = *(mActionStack.mRedoStack.end()-1);
			if(mGroup->mActions.size()){
				if(mGroup->mActions[0]->checkTileMap()){
					switchTileMap(mGroup->mActions[0]->getTileMap());
					bMapSwitch = true;
				}
			}
		}
		
		if(!bMapSwitch){
			mActionStack.redoLastActionGroup();
		}		
	}
	if(mCurMode == EMODE_TILE){
		mTileSelectedTile->mActionStack.redoLastActionGroup();
	}
	if(mCurMode == EMODE_TILESET){
		mTileSet.mActionStack.redoLastActionGroup();
	}
	//if(mCurMode == EMODE_SELEDIT){
	//	mSelEdit.mActionStack.redoLastActionGroup();
	//}
}

int TEditor::activateProjectInfo(){	
	mGlobalSettings.bShowProjectInfo = !mGlobalSettings.bShowProjectInfo;	
	return 0;
}

int TEditor::activateDropUnusedTile(){
	if(mCurMode == EMODE_MAP){
		int cTileCount = 0;
		std::vector<int> tmpTiles;

		for(auto *cMap : mTileMaps){
			cTileCount += selectTilesMap(cMap, tmpTiles, mMapSelectedTile);
		}

		if(cTileCount > 0){
			showMessage("Selected Tile is used in TileMap(s)\nReplace Tile in TileMap(s) before removal");
		} else {
			if(mGlobalSettings.bTileSetWarnBeforeDelete){
				mActiveDialog = &mRemoveSelUnused;
			} else {
				mActionStack.redoClearStack();	
				mActionStack.undoClearStack();
				mTileSet.mActionStack.redoClearStack();
				mTileSet.mActionStack.undoClearStack();

				mTileSelectedTile->bIsSelected = false;
				
				dropUnusedTile(mMapSelectedTile);

				mMapSelectedTile = 0;
				mTileSelectedTile = mTileSet.TTiles[0];

				mTileSelectedTile->bIsSelected = true;				
			}

		}
	}
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

	mTileSet.mActionStack.redoClearStack();
	mTileSet.mActionStack.undoClearStack();

	mTileSelectedTile->bIsSelected = false;

	mMapSelectedTile = 0;
	mTileSelectedTile = mTileSet.TTiles[0];

	//TODO maybe	
	for(int i = 0; i < mTileSet.TTiles.size(); i++){	

		int cTileCount = 0;

		for(auto *cMap : mTileMaps){
			cTileCount += selectTilesMap(cMap, tmpTiles, i);
		}

		if(cTileCount == 0){
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
			
	//TODO maybe
	for(auto * cTileMap : mTileMaps){
		cTileMap->removeTile(cDropTile);
	}
	mTileSet.deleteTile(cDropTile);

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

int TEditor::swapTiles(int source, int target, bool bDoCopy){
	if(mCurMode == EMODE_MAP){
		if(bLCTRLisDown && bDoCopy){
			TEActionReplaceTileSet* newAction = new TEActionReplaceTileSet();				
			newAction->doAction(mTileSet.TTiles[target], mTileSet.TTiles[source]->FileData, this, &mTileSet);
      		mActionStack.newActionGroup();	
      		mActionStack.addAction(newAction);

			mActionStack.mLastAction = newAction;
       		mActionStack.redoClearStack();

			//mMapSelectedTile = target;

			//mTileSet.TTiles[target]->replaceWithBuffer(mTileSet.TTiles[source]->FileData, &mPalette);
		} else {			


			//std::swap(mTileSet.TTiles[source], mTileSet.TTiles[target]);

			//mActionStack.undoClearStack();

			TEActionSwapTiles* newAction = new TEActionSwapTiles();				
			newAction->doAction(this, &mTileSet, source, target, mGlobalSettings.bTileSetOrderUpdateTileMap);
      		mActionStack.newActionGroup();	
      		mActionStack.addAction(newAction);
			mActionStack.mLastAction = newAction;
			mActionStack.redoClearStack();

	/*
			if(mGlobalSettings.bTileSetOrderUpdateTileMap){
				for(auto *cMap : mTileMaps){
					cMap->swapTileValues(source, target);
				}
				for(auto *dGroup : mActionStack.mUndoStack){
					for(auto *dAction: dGroup->mActions){
						dAction->swapTileValues(source, target);
					}
				}
			}
	*/			
			
		}
		return 0;
	}
	return 1;
}

int TEditor::moveTileUp(){
	if(mCurMode == EMODE_MAP){
		if(mTileSelectedTile){
			if(mMapSelectedTile > 0){				
				swapTiles(mMapSelectedTile, mMapSelectedTile-1);				
				return 0;
			}
		}
	}
	return 1;
}

int TEditor::moveTileDown(){
	if(mCurMode == EMODE_MAP){
		if(mTileSelectedTile){
			if(mMapSelectedTile < (mTileSet.TTiles.size()-1)){				
				swapTiles(mMapSelectedTile, mMapSelectedTile+1);
				return 0;
			}
		}
	}
	return 1;
}

int TEditor::rotateTile(){
	if(mCurMode == EMODE_MAP){
		if(mTileSelectedTile){
			Tile* newTile = new Tile(); 
			newTile->loadFromBuffer(mTileSelectedTile->FileData, &mPalette);
			newTile->rotater();
			TEActionReplaceTileSet* newAction = new TEActionReplaceTileSet();				
			newAction->doAction(mTileSelectedTile, newTile->FileData, this, &mTileSet);
      		mActionStack.newActionGroup();	
      		mActionStack.addAction(newAction);

			mActionStack.mLastAction = newAction;
       		mActionStack.redoClearStack();
			return 0;
		}
	}
	return 1;
}

int TEditor::rotateTileLeft(){
	if(mCurMode == EMODE_MAP){
		if(mTileSelectedTile){			
			Tile* newTile = new Tile();
			newTile->loadFromBuffer(mTileSelectedTile->FileData, &mPalette);
			newTile->rotatel();
			TEActionReplaceTileSet* newAction = new TEActionReplaceTileSet();				
			newAction->doAction(mTileSelectedTile, newTile->FileData, this, &mTileSet);
      		mActionStack.newActionGroup();	
      		mActionStack.addAction(newAction);

			mActionStack.mLastAction = newAction;
       		mActionStack.redoClearStack();

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

bool TEditor::checkQuit(){
	if(mCurMode == EMODE_MAP){
		if(bShowBrushesTile){
			bShowBrushesTile = false;
			return true;
		}
		if(mCurrentBrushTile){
			mCurrentBrushTile = NULL;
			return true;
		}
	}

	if(mCurMode == EMODE_TILE){
		if(bShowBrushesPixel){
			bShowBrushesPixel = false;
			return true;
		}
		if(mCurrentBrushPixel){
			mCurrentBrushPixel = NULL;
			return true;
		}
	}

	if(mCurMode == EMODE_TILESET){
        if(bShowBrushesPixelTileSet){
            bShowBrushesPixelTileSet = false;
            return true;
        }
        if(mCurrentBrushPixelTileSet){
            mCurrentBrushPixelTileSet = NULL;
            return true;
        }
    }

	if(mCurMode == EMODE_SELEDIT){
        if(bShowBrushesPixelSelEdit){
            bShowBrushesPixelSelEdit = false;
            return true;
        }
        if(mCurrentBrushPixelSelEdit){
            mCurrentBrushPixelSelEdit = NULL;
            return true;
        }
    }

	return false;
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

int TEditor::activateRemoveTileMapDialog(){
	if(mCurMode == EMODE_MAP){
		if(mTileMaps.size() > 1){
			mActiveDialog = &mRemoveTileMap;
		}
	}
	return 0;
}

int TEditor::activateNewTileMapDialog(){
	if(mCurMode == EMODE_MAP){
		mActiveDialog = &mNewTileMapDialog;		
	}
	return 0;
}

int TEditor::activateOpenTileMapDialog(){
	if(mCurMode == EMODE_MAP){
		mActiveDialog = &mOpenTileMapDialog;
		mActiveDialog->bDialogIsWatingForText = true;		
	}
	return 0;
}

int TEditor::activateOpenTileSetDialog(){
	if(mCurMode == EMODE_MAP){
		mActiveDialog = &mOpenTileSetDialog;
		mActiveDialog->bDialogIsWatingForText = true;		
	}
	return 0;
}

int TEditor::activateBrushes(){
	if(mCurMode == EMODE_MAP){
		bShowBrushesTile = !bShowBrushesTile;
	}
	if(mCurMode == EMODE_TILE){
		bShowBrushesPixel = !bShowBrushesPixel;
	}
	if(mCurMode == EMODE_TILESET){
		bShowBrushesPixelTileSet = !bShowBrushesPixelTileSet;
	}
	if(mCurMode == EMODE_SELEDIT){
		bShowBrushesPixelSelEdit = !bShowBrushesPixelSelEdit;
	}
	return 0;
}

int TEditor::dropBrush(){
	if(mCurMode == EMODE_MAP){
		if(mCurrentBrushTile){
			mCurrentBrushTile = NULL;
		}
	}
	if(mCurMode == EMODE_TILE){
		if(mCurrentBrushPixel){
			mCurrentBrushPixel = NULL;
		}
	}
	if(mCurMode == EMODE_TILESET){
        if(mCurrentBrushPixelTileSet){
            mCurrentBrushPixelTileSet = NULL;
        }
    }
	if(mCurMode == EMODE_SELEDIT){
        if(mCurrentBrushPixelSelEdit){
            mCurrentBrushPixelSelEdit = NULL;
        }
    }
	return 0;
}

int TEditor::activateBrush(){
	if(mCurMode == EMODE_MAP){		
		if(!mCurrentBrushTile){
				mCurrentBrushTile = mBrushesTile.getBrush();
		} else {		
				mCurrentBrushTile = mBrushesTile.getNextBrush();
		}
	}
	if(mCurMode == EMODE_TILE){		
		if(!mCurrentBrushPixel){
				mCurrentBrushPixel = mBrushesPixel.getBrush();
		} else {		
				mCurrentBrushPixel = mBrushesPixel.getNextBrush();
		}
	}

    if(mCurMode == EMODE_TILESET){          
        if(!mCurrentBrushPixelTileSet){
            mCurrentBrushPixelTileSet = mBrushesPixel.getBrush();
        } else {                
            mCurrentBrushPixelTileSet = mBrushesPixel.getNextBrush();
        }
    }

	if(mCurMode == EMODE_SELEDIT){          
        if(!mCurrentBrushPixelSelEdit){
            mCurrentBrushPixelSelEdit = mBrushesPixel.getBrush();
        } else {                
            mCurrentBrushPixelSelEdit = mBrushesPixel.getNextBrush();
        }
    }

	return 0;
}




int TEditor::handleSelection(int SELMODE){
	if(mCurMode == EMODE_MAP){
		switch (SELMODE)
		{
			case SELMODE_ALL:
				mTileMap->mSelection.selectRange(0, mGlobalSettings.TileMapWidth *  mGlobalSettings.TileMapHeight);
			break;
			case SELMODE_NONE:
				mTileMap->mSelection.clearSelection();
			break;
			case SELMODE_INVERT:
				mTileMap->mSelection.invertSelection(0, mGlobalSettings.TileMapWidth *  mGlobalSettings.TileMapHeight);
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

	if(mCurMode == EMODE_TILESET){
		switch (SELMODE)
		{
			case SELMODE_ALL:
				mTileSet.mSelection.selectRange(0, mTileSet.mSelectionAreaX *  mTileSet.mSelectionAreaY);				
				mTileSet.mSelection.removeRanges(mTileSet.getPadding());
			break;
			case SELMODE_NONE:
				mTileSet.mSelection.clearSelection();
			break;
			case SELMODE_INVERT:
				mTileSet.mSelection.invertSelection(0,  mTileSet.mSelectionAreaX *  mTileSet.mSelectionAreaY);
				mTileSet.mSelection.removeRanges(mTileSet.getPadding());
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
	if(mCurMode == EMODE_TILESET){		
		mGlobalSettings.bShowTileGrid = !mGlobalSettings.bShowTileGrid;
		return 0;		
	}
	if(mCurMode == EMODE_SELEDIT){		
		mGlobalSettings.bShowTileSelGrid = !mGlobalSettings.bShowTileSelGrid;
		return 0;		
	}
	return 1;
}
		
int TEditor::flipSelectedTile(int cFlipMode){
	if(mCurMode == EMODE_MAP){
		if(mGlobalSettings.mSelectedTile > -1){
			int cTileFlipOld = mTileMap->getFlip(mGlobalSettings.mSelectedTile);

			int cTileFlip = 0;
			
			if(cFlipMode == 0){
			 	cTileFlip = cTileFlipOld + 1;
				if(cTileFlip > 3){cTileFlip = 0;}				
			}

			if(cFlipMode == 1){
				if(cTileFlipOld & 0x1){
					cTileFlip = (cTileFlipOld & 0x2);
				} else {
					cTileFlip = (cTileFlipOld | 0x1);
				}				
			}

			if(cFlipMode == 2){
				if(cTileFlipOld & 0x2){
					cTileFlip = (cTileFlipOld & 0x1);
				} else {
					cTileFlip = (cTileFlipOld | 0x2);
				}				
			}
				
			TEActionReplaceTileFlip *mCurAction = new TEActionReplaceTileFlip();
	       	mCurAction->doAction(mTileMap, mGlobalSettings.mSelectedTile, mTileMap->getTile(mGlobalSettings.mSelectedTile), mTileMap->getTile(mGlobalSettings.mSelectedTile),cTileFlipOld, cTileFlip);
	       			
	       	if(!(*mCurAction == *mActionStack.mLastAction)){
	       		mActionStack.newActionGroup();	
	       		mActionStack.addAction(mCurAction);
	       		mActionStack.mLastAction = mCurAction;
	       		mActionStack.redoClearStack();
	       	}

			return 0;
		}
	}
	return 1;
}


int TEditor::replaceSelectedColor(int mx, int my){
	
	if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_TILE){

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
	}

	if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_TILESET){
		if(mGlobalSettings.bShowPixelType || mTileSet.mSelection.mSelected.size()){
			int mOldColor = mColorSelected;
			int tSel = searchRectsXY(mPalette.PixelAreas, mx, my);
			if(tSel > -1){					
				if(mTileSet.mSelection.mSelected.size()){
					TEActionReplacePixelsSel* newAction = new TEActionReplacePixelsSel();
					newAction->doAction(&mTileSet, mTileSet.mSelection, mTileSet.mSelectionAreaX, mTileSet.mSelectionAreaY, tSel, &mPalette);
					if(!(newAction == mTileSet.mActionStack.mLastAction)){
						mTileSet.mActionStack.mLastAction = newAction;
						mTileSet.mActionStack.newActionGroup();
						mTileSet.mActionStack.addSubActions(newAction->mSubActions);
						mTileSet.mActionStack.redoClearStack();
					}
				} else {					
					mTileSet.mActionStack.newActionGroup();
					
					TEActionReplacePixels* newAction;
					bool bDidAction = false;
					for(int tt = 0; tt < mTileSet.TTiles.size(); tt++){
						std::vector<int> newSelection;
						int cPixIndex = 0;
						Tile* mTile =  mTileSet.TTiles[tt];
						
						for(int i = 0; i < (mGlobalSettings.TileSizeX *mGlobalSettings.TileSizeY); i++){
							auto cPixel = mTile->getPixel(i);
							if(cPixel == mOldColor){
								newSelection.push_back(cPixIndex);
							}
							cPixIndex++;
						}
						if(newSelection.size()){
							newAction = new TEActionReplacePixels();
							newAction->doAction(mTile, newSelection, mOldColor, tSel, &mPalette);
							//if(!(newAction == mTileSet.mActionStack.mLastAction)){							
							mTileSet.mActionStack.addSubActions(newAction->mSubActions);
							bDidAction = true;
						
							//}
						}
					}
					if(bDidAction){
						mTileSet.mActionStack.mLastAction = newAction;
						mTileSet.mActionStack.redoClearStack();
					} else {
						mTileSet.mActionStack.dropLastGroup();
					}
				}
			}
		}
	}


	return 0;
}


int TEditor::replaceSelectedTiles(int mx, int my){
	
	if(mTileMap->mSelection.mSelected.size()){
		int tSel = -1;		
		tSel = searchRectsXY(mTileSet.TileAreas, mx, my);
		if(tSel != -1){ 
			TEActionReplaceTiles* newAction = new TEActionReplaceTiles();
			newAction->doAction(mTileMap, mTileMap->mSelection.mSelected, -1, tSel);
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
				newAction->doAction(mTileMap, newSelection, mMapSelectedTile, tSel);
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
				TEActionBrushTiles* newAction = new TEActionBrushTiles();
				newAction->doAction(mTileMap, *mCurrentBrushTile);							
				if(!(*newAction == *mActionStack.mLastAction)){							
					mActionStack.mLastAction = newAction;
					mActionStack.newActionGroup();
					mActionStack.addSubActions(newAction->mSubActions);
    				mActionStack.redoClearStack();
				}							
			} else {
				tSel = searchRectsXY(mTileMap->TileAreas,cx,cy);
	   			if(tSel != -1){
	   				mGlobalSettings.mSelectedTile = tSel;		  	
	   				TEActionReplaceTileFlip *mCurAction = new TEActionReplaceTileFlip();
	   				mCurAction->doAction(mTileMap, tSel, mTileMap->getTile(tSel), mMapSelectedTile, mTileMap->getFlip(tSel), 0);
	       			
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
			mTileSelectedTile->mSelection.confirmSelection(mTileSelectedTile->PixelAreas, mGlobalSettings.TilePixelSize * mGlobalSettings.mTileEdScale, mGlobalSettings.TilePixelSize * mGlobalSettings.mTileEdScale);
		}
		
		if(leftMouseButtonDown && !mGlobalSettings.mio->WantCaptureMouse){
			int tSel = -1;

			if(mCurrentBrushPixel){
				TEActionBrushPixels* newAction = new TEActionBrushPixels();
				newAction->doAction(mTileSelectedTile, *mCurrentBrushPixel, &mPalette);
				if(!(*newAction == *mTileSelectedTile->mActionStack.mLastAction)){								
					mTileSelectedTile->mActionStack.mLastAction = newAction;
					mTileSelectedTile->mActionStack.newActionGroup();
					mTileSelectedTile->mActionStack.addSubActions(newAction->mSubActions);
       				mTileSelectedTile->mActionStack.redoClearStack();
				}
			} else {
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

	if(!ImButtonsPalette.bWindowHasFocus){
		return 0;
	}

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

	if(!ImButtonsPalette.bWindowHasFocus){
		return 0;
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

	if(ImButtonsPalette.mRight.bButtonIsDown && mBrushesPixel.bIsEditing){
		int tSel = -1;		
		tSel = searchRectsXY(mPalette.PixelAreas, cx, cy);
		
		if(mGlobalSettings.TileSetBPP < 0x8){
			mBrushesPixel.addBrushElement(tSel%16);
		} else {
			mBrushesPixel.addBrushElement(tSel);
		}
		
	} else if(ImButtonsPalette.mRight.bButtonIsDown){
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

int TEditor::handleSelEdit(){
	handleBrushes();
	handlePalette();

	if(leftMouseButtonDown && !bLShiftIsDown && !mGlobalSettings.mio->WantCaptureMouse){
		if(bLCTRLisDown){
			if(bSelEditGrapped){
				mSelEditScrollX += rx;
				mSelEditScrollY += ry;;				
			} else {
				bSelEditGrapped = true;				
			}				
		} else {
			int tSel = -1;		
			if(mCurrentBrushPixelSelEdit){
				TEActionBrushPixelsSelEdit* newAction = new TEActionBrushPixelsSelEdit();
				newAction->doAction(mTileMap, &mTileSet, mSelEdit.mCurrentSelection->mSelected , *mCurrentBrushPixelSelEdit, mSelEdit.mSelectionAreaX, mSelEdit.mSelectionAreaY,  &mPalette);
				if(!(*newAction == *mActionStack.mLastAction)){								
					mActionStack.mLastAction = newAction;
					mActionStack.newActionGroup();
					mActionStack.addSubActions(newAction->mSubActions);
       				mActionStack.redoClearStack();
				}
			} else { 
				tSel = mSelEdit.mSelection.searchSelection(mSelEdit.EditPixelAreas, cx, cy);
				if(tSel > -1){
					int tindex;
					int ttile;
					int stile;
					int mtile;
					ttile = mSelEdit.mSelection.getTileIndex(tSel, mSelEdit.mSelectionAreaX, mSelEdit.mSelectionAreaY,tindex);				
					stile = mSelEdit.mCurrentSelection->mSelected[ttile];
					mtile = mTileMap->getTile(stile);

					if(mtile > -1){					
						Tile* cSelectedTile = mTileSet.TTiles[mtile];
						int findex = cSelectedTile->getFlipIndex(tindex, mTileMap->getFlip(stile));

						TEActionReplacePixel *mCurAction = new TEActionReplacePixel();
						mCurAction->doAction(cSelectedTile, findex, cSelectedTile->getPixel(findex), mColorSelected, &mPalette);
				
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
	}

	if(rightMouseButtonDown && !mGlobalSettings.mio->WantCaptureMouse  && !bLShiftIsDown){
		int tSel = -1;
		tSel = searchRectsXY(mSelEdit.EditPixelAreas, cx, cy);
		if(tSel >-1){

			mColorSelectedTile->bPixelSelected = false;		
			int tindex;
			int ttile;
			int stile;
			int mtile;
			ttile = mSelEdit.mSelection.getTileIndex(tSel, mSelEdit.mSelectionAreaX, mSelEdit.mSelectionAreaY,tindex);				
			stile = mSelEdit.mCurrentSelection->mSelected[ttile];
			mtile = mTileMap->getTile(stile);

			if(mtile > -1){					
				Tile* cSelectedTile = mTileSet.TTiles[mtile];
				int findex = cSelectedTile->getFlipIndex(tindex, mTileMap->getFlip(stile));

				mGlobalSettings.PaletteOffset = mTileMap->getOffset(stile);

				mColorSelected = (cSelectedTile->getPixel(findex)+(mGlobalSettings.PaletteOffset*16));
				mColorSelectedTile = mPalette.TPixels[mColorSelected];
				mColorSelectedTile->bPixelSelected = true;
				mTileSelectedTile->mSelection.cancelSelection();
				mGlobalSettings.CurrentEditor->mPalette.bUpdateEditColor = true;
			}
		}
	}

	int selEditWidthX = (mSelEdit.mSelectionAreaX * mSelEdit.mCurEdScale) +  mGlobalSettings.TopBarHeight;
	int selEditWidthY = (mSelEdit.mSelectionAreaY * mSelEdit.mCurEdScale) +  mGlobalSettings.TopBarHeight;

	if(mSelEditScrollX > 0){mSelEditScrollX = 0;}
	if(mSelEditScrollY > 0){mSelEditScrollY = 0;}
	
	if(mSelEditScrollX < -(selEditWidthX - (mGlobalSettings.WindowWidth- 50))){mSelEditScrollX = -(selEditWidthX - (mGlobalSettings.WindowWidth -50));}
	if(mSelEditScrollY < -(selEditWidthY - (mGlobalSettings.WindowHeight - 50- mGlobalSettings.TopBarHeight))){mSelEditScrollY = -(selEditWidthY - (mGlobalSettings.WindowHeight -50 - mGlobalSettings.TopBarHeight));}
	
	if(selEditWidthX < (mGlobalSettings.WindowWidth)){
		mSelEditScrollX = 0;
	}
	
	if(selEditWidthY < (mGlobalSettings.WindowHeight-mGlobalSettings.TopBarHeight)){
		mSelEditScrollY = 0;
	}


	return 0;
}

int TEditor::handleTileSetEdit(){

	handlePalette();

	if(leftMouseButtonDown && bLShiftIsDown && !mGlobalSettings.mio->WantCaptureMouse){
		if(!mTileSet.mSelection.bIsSelecting){
			mTileSet.mSelection.startSelection(cx, cy);	
		} else {
			mTileSet.mSelection.updateSelection(cx, cy);
		}
	} else {
		if(mTileSet.mSelection.bIsSelecting){
			mTileSet.mSelection.confirmSelection(mTileSet.EditPixelAreas, mTileSet.mCurEdScale,  mTileSet.mCurEdScale);
		} 
	}

	

	if(leftMouseButtonDown && !bLShiftIsDown && !mGlobalSettings.mio->WantCaptureMouse){
		if(bLCTRLisDown){
			if(bTileSetGrapped){
				mTileSetScrollX += rx;
				mTileSetScrollY += ry;;
			} else {
				bTileSetGrapped = true;						
			}				
		} else {
			int tSel = -1;

			if(mCurrentBrushPixelTileSet){
				TEActionBrushPixelsTileSet* newAction = new TEActionBrushPixelsTileSet();
				newAction->doAction(&mTileSet, *mCurrentBrushPixelTileSet, mTileSet.mSelectionAreaX, mTileSet.mSelectionAreaY,  &mPalette);
				if(!(*newAction == *mTileSet.mActionStack.mLastAction)){								
					mTileSet.mActionStack.mLastAction = newAction;
					mTileSet.mActionStack.newActionGroup();
					mTileSet.mActionStack.addSubActions(newAction->mSubActions);
       				mTileSet.mActionStack.redoClearStack();
				}
		} else {
			tSel = mTileSet.mSelection.searchSelection(mTileSet.EditPixelAreas, cx, cy);
			if(tSel > -1){
				int tindex;
				int ttile;
				ttile = mTileSet.mSelection.getTileIndex(tSel, mTileSet.mSelectionAreaX, mTileSet.mSelectionAreaY,tindex);
			
				if(ttile > -1){
					Tile* cSelectedTile = mTileSet.TTiles[ttile];

					TEActionReplacePixel *mCurAction = new TEActionReplacePixel();
					mCurAction->doAction(cSelectedTile, tindex, cSelectedTile->getPixel(tindex), mColorSelected, &mPalette);
				
					if(!(*mCurAction == *mTileSet.mActionStack.mLastAction)){
       					mTileSet.mActionStack.newActionGroup();	
       					mTileSet.mActionStack.addAction(mCurAction);
       					mTileSet.mActionStack.mLastAction = mCurAction;
       					mTileSet.mActionStack.redoClearStack();
       				}
				}			
			}				
		}
		}
	}

	int tileSetWidthX = (mTileSet.mSelectionAreaX * mTileSet.mCurEdScale) +  mGlobalSettings.TopBarHeight;
	int tileSetWidthY = (mTileSet.mSelectionAreaY * mTileSet.mCurEdScale) +  mGlobalSettings.TopBarHeight;

	if(mTileSetScrollX > 0){mTileSetScrollX = 0;}
	if(mTileSetScrollY > 0){mTileSetScrollY = 0;}
	
	if(mTileSetScrollX < -(tileSetWidthX - (mGlobalSettings.WindowWidth- 50))){mTileSetScrollX = -(tileSetWidthX - (mGlobalSettings.WindowWidth -50));}
	if(mTileSetScrollY < -(tileSetWidthY - (mGlobalSettings.WindowHeight - 50- mGlobalSettings.TopBarHeight))){mTileSetScrollY = -(tileSetWidthY - (mGlobalSettings.WindowHeight -50 - mGlobalSettings.TopBarHeight));}
	
	if(tileSetWidthX < (mGlobalSettings.WindowWidth)){
		mTileSetScrollX = 0;
	}
	
	if(tileSetWidthY < (mGlobalSettings.WindowHeight-mGlobalSettings.TopBarHeight)){
		mTileSetScrollY = 0;
	}

	if((rightMouseButtonClicks && bLShiftIsDown) && !mGlobalSettings.mio->WantCaptureMouse){		
		int tSel = -1;
		tSel = searchRectsXY(mTileSet.EditPixelAreas, cx, cy);
		if(tSel >-1){
			mTileSet.mSelection.modifySelection(tSel);
		}		
	}

	if(rightMouseButtonDown && !mGlobalSettings.mio->WantCaptureMouse  && !bLShiftIsDown){
		int tSel = -1;
		tSel = searchRectsXY(mTileSet.EditPixelAreas, cx, cy);
		if(tSel >-1){
			mColorSelectedTile->bPixelSelected = false;
			int tindex;
			int tTile;
			tTile = mTileSet.mSelection.getTileIndex(tSel, mTileSet.mSelectionAreaX, mTileSet.mSelectionAreaY, tindex);
			Tile *mTile = mTileSet.TTiles[tTile];
			mColorSelected = (mTile->getPixel(tindex)+(mGlobalSettings.PaletteOffset*16));
			mColorSelectedTile = mPalette.TPixels[mColorSelected];
			mColorSelectedTile->bPixelSelected = true;
			mTileSelectedTile->mSelection.cancelSelection();
			mGlobalSettings.CurrentEditor->mPalette.bUpdateEditColor = true;
		}
	}

	return 0;
}

int TEditor::handleBrushes(){

	if(mCurMode == EMODE_MAP){
		if(ImButtonsBrushes.mLeft.bButtonIsDown){ 
			if(ImButtonsBrushes.mLeft.mMousePos.y < mBrushesTile.mBrushOffset) {return 0;}
			int tSel = -1;
			tSel = searchRectsXY(mBrushesTile.BrushAreas, ImButtonsBrushes.mLeft.mMousePos.x, ImButtonsBrushes.mLeft.mMousePos.y);
			if(tSel != -1){
				mBrushesTile.mSelectedBrush = tSel;
				mCurrentBrushTile = mBrushesTile.mBrushes[tSel];
			}		
		}
	}
	if(mCurMode == EMODE_TILE){
		if(ImButtonsBrushes.mLeft.bButtonIsDown){ 
			if(ImButtonsBrushes.mLeft.mMousePos.y < mBrushesPixel.mBrushOffset) {return 0;}
			int tSel = -1;
			tSel = searchRectsXY(mBrushesPixel.BrushAreas, ImButtonsBrushes.mLeft.mMousePos.x, ImButtonsBrushes.mLeft.mMousePos.y);
			if(tSel != -1){
				mBrushesPixel.mSelectedBrush = tSel;
				mCurrentBrushPixel = mBrushesPixel.mBrushes[tSel];
			}		
		}
	}
	
	if(mCurMode == EMODE_TILESET){
        if(ImButtonsBrushes.mLeft.bButtonIsDown){ 
            if(ImButtonsBrushes.mLeft.mMousePos.y < mBrushesPixel.mBrushOffset) {return 0;}
            int tSel = -1;
            tSel = searchRectsXY(mBrushesPixel.BrushAreas, ImButtonsBrushes.mLeft.mMousePos.x, ImButtonsBrushes.mLeft.mMousePos.y);
            if(tSel != -1){
                mBrushesPixel.mSelectedBrush = tSel;
                mCurrentBrushPixelTileSet = mBrushesPixel.mBrushes[tSel];
            }               
        }
    }

	if(mCurMode == EMODE_SELEDIT){
        if(ImButtonsBrushes.mLeft.bButtonIsDown){ 
            if(ImButtonsBrushes.mLeft.mMousePos.y < mBrushesPixel.mBrushOffset) {return 0;}
            int tSel = -1;
            tSel = searchRectsXY(mBrushesPixel.BrushAreas, ImButtonsBrushes.mLeft.mMousePos.x, ImButtonsBrushes.mLeft.mMousePos.y);
            if(tSel != -1){
                mBrushesPixel.mSelectedBrush = tSel;
                mCurrentBrushPixelSelEdit = mBrushesPixel.mBrushes[tSel];
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
	} else if(ImButtonsTileSet.mRight.bButtonIsDown && (mGlobalSettings.bShowTypeSelection || mTileMap->mSelection.mSelected.size())){
		replaceSelectedTiles(ImButtonsTileSet.mRight.mMousePos.x ,ImButtonsTileSet.mRight.mMousePos.y);
	}

	return 0;
}


int TEditor::handleTileMap(){

	if((rightMouseButtonClicks && bLShiftIsDown) && !mGlobalSettings.mio->WantCaptureMouse){
		int tSel = -1;
		tSel = searchRectsXY(mTileMap->TileAreas, cx, cy);
	    if(tSel != -1){	    	
			mGlobalSettings.bShowSelectedTile = false;
			mTileMap->mSelection.modifySelection(tSel);
		}
	} else {
		int tSel = -1;

		if(rightMouseButtonDown && !mGlobalSettings.mio->WantCaptureMouse && !bLShiftIsDown){
			tSel = searchRectsXY(mTileMap->TileAreas, cx, cy);
	    	if(tSel != -1){
	    		mGlobalSettings.mSelectedTile = tSel;
				mGlobalSettings.bShowSelectedTile = true;			
				mMapSelectedTile = mTileMap->getTile(tSel);
   	 			mTileSelectedTile->bIsSelected = false;
   	 			mTileSelectedTile = mTileSet.TTiles[mMapSelectedTile];
   	 			mTileSelectedTile->bIsSelected = true;							
				mTileMap->mSelection.cancelSelection();
			}
		}
	}

	if(leftMouseButtonDown && bLShiftIsDown && !mGlobalSettings.mio->WantCaptureMouse){
		if(!mTileMap->mSelection.bIsSelecting){
			mTileMap->mSelection.startSelection(cx, cy);	
		} else {
			mTileMap->mSelection.updateSelection(cx, cy);
		}
	} else {
		if(mTileMap->mSelection.bIsSelecting){
			mTileMap->mSelection.confirmSelection(mTileMap->TileAreas, mGlobalSettings.TileSizeX * mGlobalSettings.TileMapScale,  mGlobalSettings.TileSizeY * mGlobalSettings.TileMapScale);
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
		bTileSetGrapped = false;
		bSelEditGrapped = false;
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
			if(mGlobalSettings.mDeleteUnusedTilesState == 1){
				dropUnusedTiles();
				mGlobalSettings.mDeleteUnusedTilesState = 0;				
			}
			if(mGlobalSettings.mDeleteUnusedTilesState == 2){
				mActionStack.redoClearStack();	
				mActionStack.undoClearStack();
				mTileSet.mActionStack.redoClearStack();
				mTileSet.mActionStack.undoClearStack();

				mTileSelectedTile->bIsSelected = false;
				
				dropUnusedTile(mMapSelectedTile);

				mMapSelectedTile = 0;
				mTileSelectedTile = mTileSet.TTiles[0];

				mTileSelectedTile->bIsSelected = true;

				mGlobalSettings.mDeleteUnusedTilesState = 0;				
			}
			if(mGlobalSettings.mProjectSaveState == 1){
				if(saveToFolder(mGlobalSettings.ProjectPath)){					
					showMessage("Error Creating Project Folder!", true);
				} 
				mGlobalSettings.mProjectSaveState = 0;								
			}
			if(mGlobalSettings.mPaletteUpdateState == 1){
				updatePalette();
				mGlobalSettings.mPaletteUpdateState = 0;				
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
			if(mGlobalSettings.mOpenTileState == 2){
				
				mGlobalSettings.mOpenTileState = 0;
				std::vector<Tile*> cNewTiles;
				if(mTileSet.importTileSet(mGlobalSettings.mNewTilePath, cNewTiles)){
					cancelActiveDialog();
					showMessage("Error Importing TileSet", true);
					return 0;
				}

				mActionStack.newActionGroup();
				
				for(auto cTile : cNewTiles){
					TEActionAddTiles* newActionTile = new TEActionAddTiles();
					newActionTile->doAction(cTile, this, &mTileSet);	       			
	       			mActionStack.addAction(newActionTile);
	       			mActionStack.mLastAction = newActionTile;	       			
				}
				

				mActionStack.redoClearStack();
				
				cancelActiveDialog();
				showMessage("TileSet Imported Successfully");
				return 0;
			}
			if(mGlobalSettings.mOpenTileMapState == 1){
				
				mGlobalSettings.mOpenTileMapState = 0;
				int cretval = 0;

				if(mGlobalSettings.TileSetBPP < 0x8){
					cretval = importTileMap(mGlobalSettings.mNewTileMapPath, 0, mGlobalSettings.mNewTileMapPaletteOffset);
				} else {
				 	cretval = importTileMap(mGlobalSettings.mNewTileMapPath, 0, 0);
				}


				if(cretval){
					cancelActiveDialog();
					if(cretval == 2){
						showMessage("Error Importing TileMap, Tiles are out of bound", true);
					} else {
						showMessage("Error Importing TileMap", true);
					}
					
					return 0;
				}

										
				mActionStack.redoClearStack();
				mActionStack.undoClearStack();

				cancelActiveDialog();
				showMessage("TileMap Imported Successfully");

				switchTileMap(mTileMaps.size()-1);

				return 0;
			}

			if(mGlobalSettings.mOpenTileMapState == 2){
				
				mGlobalSettings.mOpenTileMapState = 0;
				int cretval = 0;
				std::cout << "Importing TileMap with offset: " << mGlobalSettings.mNewTileMapOffset << std::endl;

				if(mGlobalSettings.TileSetBPP < 0x8){
					cretval = importTileMap(mGlobalSettings.mNewTileMapPath, mGlobalSettings.mNewTileMapOffset, mGlobalSettings.mNewTileMapPaletteOffset);
				} else {
				 	cretval = importTileMap(mGlobalSettings.mNewTileMapPath, mGlobalSettings.mNewTileMapOffset, 0);
				}

				if(cretval){
					cancelActiveDialog();
					if(cretval == 2){
						showMessage("Error Importing TileMap, Tiles are out of bound", true);
					} else {
						showMessage("Error Importing TileMap", true);
					}
					
					return 0;
				}

										
				mActionStack.redoClearStack();
				mActionStack.undoClearStack();

				cancelActiveDialog();
				showMessage("TileMap Imported Successfully");

				switchTileMap(mTileMaps.size()-1);

				return 0;
			}


			if(mGlobalSettings.mNewTileMapState == 1){
				
				mGlobalSettings.mNewTileMapState = 0;

				if(mGlobalSettings.TileSetBPP < 0x8){
					createTileMap(mGlobalSettings.mNewTileMapX, mGlobalSettings.mNewTileMapY, mGlobalSettings.mNewTileMapOffset, mGlobalSettings.mNewTileMapPaletteOffset);				
				} else {
					createTileMap(mGlobalSettings.mNewTileMapX, mGlobalSettings.mNewTileMapY, mGlobalSettings.mNewTileMapOffset);				
				}
										
				mActionStack.redoClearStack();
				mActionStack.undoClearStack();

				cancelActiveDialog();
				showMessage("TileMap Created Successfully");

				switchTileMap(mTileMaps.size()-1);

				return 0;
			}

			if(mGlobalSettings.mDeleteTileMapState == 1){
				
				mGlobalSettings.mDeleteTileMapState = 0;
											
				int cDelTileMap = -1;
				int i = 0;
				for(auto * cTileMap : mTileMaps){
					if(mTileMap == cTileMap){
						cDelTileMap = i;
					}
					i++;
				}

				if(cDelTileMap != -1){
			
					mActionStack.redoClearStack();
					mActionStack.undoClearStack();

					mTileMaps.erase(mTileMaps.begin() + cDelTileMap);
					
					cancelActiveDialog();
					showMessage("TileMap Removed Successfully");

					switchTileMap(0);

					return 0;
				}
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
				handleBrushes();
				handleEMTile();
			}
			if(mCurMode == EMODE_PALED){
				handlePaletteEdit();
			}
			if(mCurMode == EMODE_TILESET){
				handleBrushes();
				handleTileSetEdit();
			}
			if(mCurMode == EMODE_SELEDIT){
				handleSelEdit();
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
					
					if(checkQuit()){
						return 0;
					}
					
					activateQuitDialog();					
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_SPACE){
	  				switchMode();
	  			}
				if(cEvent->key.keysym.sym == SDLK_s){
					if(mCurMode == EMODE_MAP) mGlobalSettings.bShowTypeSelection = !mGlobalSettings.bShowTypeSelection;
					if((mCurMode == EMODE_TILE) || (mCurMode == EMODE_TILESET) || (mCurMode == EMODE_SELEDIT)) mGlobalSettings.bShowPixelType = !mGlobalSettings.bShowPixelType;					
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_p){
		  			if(mCurMode == EMODE_TILE) mGlobalSettings.bShowPixelGrid = !mGlobalSettings.bShowPixelGrid;
					if(mCurMode == EMODE_TILESET) mGlobalSettings.bShowTilePixelGrid = !mGlobalSettings.bShowTilePixelGrid;
					if(mCurMode == EMODE_SELEDIT) mGlobalSettings.bShowTilePixelSelGrid = !mGlobalSettings.bShowTilePixelSelGrid;
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
				if(cEvent->key.keysym.sym == SDLK_z){
					if(bLCTRLisDown){
						undoLastActionGroup();
					}
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_r){
		  			redoLastActionGroup();
	  			}
				if(cEvent->key.keysym.sym == SDLK_d){
					
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_f){
		  			flipSelectedTile(0);
	  			}
				if(cEvent->key.keysym.sym == SDLK_x){
		  			flipSelectedTile(1);
	  			}
				if(cEvent->key.keysym.sym == SDLK_y){
		  			flipSelectedTile(2);
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
					if(mCurMode == EMODE_MAP){				
		  				createNewTile();
					}
	  			}
				if(cEvent->key.keysym.sym == SDLK_F4){
					if(mCurMode == EMODE_MAP){								
						createNewTileCopy(mGlobalSettings.CurrentEditor->mTileSelectedTile);
					}
	  			}
				if(cEvent->key.keysym.sym == SDLK_F5){	  														
					if(mCurMode == EMODE_MAP){
						rotateTileLeft();
					}
	  			}
				if(cEvent->key.keysym.sym == SDLK_F6){	  									
					if(mCurMode == EMODE_MAP){
						rotateTile();
					}
	  			}
				if(cEvent->key.keysym.sym == SDLK_DELETE){	  									
					if(mCurMode == EMODE_MAP){
						activateDropUnusedTile();
					}
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
					dropBrush();
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
				bSelEditGrapped = false;
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
	mSaveDialog.bUpdateWinPos = true;
	mSaveAsDialog.bUpdateWinPos = true;
	mOpenTileDialog.bUpdateWinPos = true;
	mOpenTileSetDialog.bUpdateWinPos = true;
	mOpenTileMapDialog.bUpdateWinPos = true;
	mNewTileMapDialog.bUpdateWinPos = true;

	return 0;
}
