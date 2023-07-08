#include "TSettings.h"
#include "TEditor.h"
#include "imgui_internal.h"

extern TSettings mGlobalSettings;

void TEditor::closeProject(){

	mActionStack.redoClearStack();
	mActionStack.undoClearStack();

	mTileSet.mActionStack.redoClearStack();
	mTileSet.mActionStack.undoClearStack();

	mTileSet.close();

	mBrushesPixel.close();
	mBrushesPixelSelEd.close();
	mBrushesTile.close();
	
	for(auto *cMap : mTileMaps){
		delete cMap;
	}

	for(auto *cSprite : mSprites){
		cSprite->close();
	}

	mTileMaps.clear();	
	mTileMap = NULL;
	
	mSprites.clear();
	mSprite = NULL;

	mLastSelEditX=0;
	mLastSelEditY=0;
	mLastSelEditWidth=0;
	mLastSelEditHeight=0;
	bTileMapWasChanged = true;			
	mMapSelectedTile=0;
	mTileSelectedTile = NULL;
	mColorSelected = 0;
	mColorSelectedTile = NULL;
	mColorSelectedEdit = 0;
	mColorSelectedTileEdit = NULL;

	mLastPixelOffset = 0;
		
	mCurMode = EMODE_MAP;
	mLastMode = EMODE_TILE;
	mouseSelX=0;
	mouseSelY=0;
	mTileMapScrollX = 0;
	mTileMapScrollY = 0;
	mTileSetScrollY = 0;
	mTileSetScrollX = 0;		
	mSelEditScrollY = 0;
	mSelEditScrollX = 0;		

	leftMouseButtonDown = false;		
	rightMouseButtonDown = false;
	leftMouseButtonClicks = 0;
	rightMouseButtonClicks = 0;
	bLCTRLisDown = false;
	bLShiftIsDown = false;
	bTileMapGrapped = false;
	bTileSetGrapped = false;
	bSelEditGrapped = false;
	bShowBrushesTile = false;
	bShowBrushesPixel = false;
	bShowBrushesPixelTileSet = false;		
	bShowBrushesPixelSelEdit = false;			

	bShowCollisionEditor = false;	

	mCurrentBrushTile = NULL;
	mCurrentBrushPixel = NULL;
	mCurrentBrushPixelTileSet = NULL;
	mCurrentBrushPixelSelEdit = NULL;		

	mActiveDialog = NULL;
	mActiveMessage = NULL;

	mGlobalSettings.close();

	bEditorRunning=false;
}

void TEditor::shutdown(){
	mTileSet.shutdown();
}

int TEditor::createNewProject(){
	std::cout << "Creating Project: " << mGlobalSettings.ProjectPath << std::endl;
	std::cout << "TileMapWidth: " << mGlobalSettings.TileMapWidth << " TileMapHeight: " << mGlobalSettings.TileMapHeight << " TileSizeX: " << mGlobalSettings.mGlobalTexParam.TexSizeX << " TileSizeY: " << mGlobalSettings.mGlobalTexParam.TexSizeY << std::endl;	
	if(mGlobalSettings.bProjectHasPalette){
		if(mGlobalSettings.testPaletteFile(mGlobalSettings.ProjectPalettePath) == 2){
			if(mPalette.importGimpPalette(mGlobalSettings.ProjectPalettePath)){				
				std::cout << "Error in Palette File! Using default" << std::endl;
				mPalette.initPalette();
			}
		} else {
			if(mPalette.loadFromFile(mGlobalSettings.ProjectPalettePath)){
				std::cout << "Error in Palette File! Using default" << std::endl;
				mPalette.initPalette();
			}
		}
	} else {
		mPalette.initPalette();
	}

	mPalette.initTPixels();
	mTileMap = new TileMap();
	mTileMap->createNew();
	mTileMaps.push_back(mTileMap);		


	//mTileSet.mSelEdWidth = 4;

	if(fs::exists(fs::status(mGlobalSettings.mNewTilePath))){
		std::vector<Tile*> cNewTiles;
		if(mTileSet.importTileSet(mGlobalSettings.mNewTilePath, cNewTiles)){
			std::cout << "Error in TileSet File! Using empty" << std::endl;
			mTileSet.createNew(&mPalette);
		}
	} else if(mGlobalSettings.mNewTileSize){
		if((mGlobalSettings.mNewTileSize == 4) || (mGlobalSettings.mNewTileSize == 8) || (mGlobalSettings.mNewTileSize == 16)){
			mTileSet.mSelEdWidth = mGlobalSettings.mNewTileSize;
			mGlobalSettings.mTileSetEditWidth = mGlobalSettings.mNewTileSize;
			for(int i = 0; i < (mGlobalSettings.mNewTileSize*mGlobalSettings.mNewTileSize); i++){
				mTileSet.createNew(&mPalette);		
			}			
		} else {
			std::cout << "Unknown TileSet Error! Using empty" << std::endl;
			mTileSet.createNew(&mPalette);
		}
	} else {
		mTileSet.createNew(&mPalette);
	}

	mTileSet.mMaxColumns = 4;
	mTileSet.resizeEdit();		

	mTileSet.mClipboardTiles.init("Tiles","Pixel", TBRUSH_PIXEL, &mTileSet.bShowClipboardTiles, mGlobalSettings.mGlobalTexParam.TexPixelSize, mGlobalSettings.mGlobalTexParam.TexPixelSize, &mGlobalSettings.mGlobalTexParam.TexEditScale, mGlobalSettings.mGlobalTexParam.TexEditScale, &mGlobalSettings.CurrentEditor->mCurrentBrushPixel, &mGlobalSettings.mGlobalTexParam);
	mTileSet.mClipboardTileSet.init("TileSet","Pixel", TBRUSH_PIXEL, &mTileSet.bShowClipboardTileSet, mGlobalSettings.mGlobalTexParam.TexPixelSize, mGlobalSettings.mGlobalTexParam.TexPixelSize, &mGlobalSettings.mGlobalTexParam.TexEditScale, mGlobalSettings.mGlobalTexParam.TexEditScale, &mGlobalSettings.CurrentEditor->mCurrentBrushPixel, &mGlobalSettings.mGlobalTexParam);

	initDialogs();

	return 0;
}

void TEditor::showProjectInfo(){

	mProjectInfo->setValue(0, mGlobalSettings.mSelectedTile % mGlobalSettings.TileMapWidth);
	mProjectInfo->setValue(1, mGlobalSettings.mSelectedTile / mGlobalSettings.TileMapWidth);
	mProjectInfo->setValue(2, (mTileMap->getFlip(mGlobalSettings.mSelectedTile) & 0x1) ? 1 : 0);
	mProjectInfo->setValue(3, (mTileMap->getFlip(mGlobalSettings.mSelectedTile) & 0x2) ? 1 : 0);

	int cSpriteBPP = 0;

	if(mSprites.size()){
		cSpriteBPP = mSprite->mTexParam.TexBPP;
	}

	mProjectInfo->setValue(4, cSpriteBPP);

	mProjectInfo->setCondition(mSprites.size() > 0 ? 1 : -1);

	mProjectInfo->update();

	mProjectInfo->render(0,mGlobalSettings.TopBarHeight);
}

void TEditor::handleState(){
	if((mGlobalSettings.mEditorState >= 0) && (mGlobalSettings.mEditorState < mStates.size())){
		mStates[mGlobalSettings.mEditorState](this);
	}	
}

void TEditor::initStates(){
	
	mStates[ESTATE_NONE] = &TEditor::stateNone;
	mStates[ESTATE_PROGRAMQUIT] = &TEditor::stateProgramQuit;
	mStates[ESTATE_PROJECTSAVE] = &TEditor::stateProjectSave;
	mStates[ESTATE_PROJECTCREATE] = &TEditor::stateProjectCreate;
	mStates[ESTATE_PROJECTOPEN] = &TEditor::stateProjectOpen;
	mStates[ESTATE_PROJECTCLOSE] = &TEditor::stateProjectClose;
	mStates[ESTATE_TILEIMPORT] = &TEditor::stateTileImport;
	mStates[ESTATE_TILECREATE] = &TEditor::stateTileCreate;
	mStates[ESTATE_TILEDELETE] = &TEditor::stateTileDelete;
	mStates[ESTATE_TILEDELETEALL] = &TEditor::stateTileDeleteAll;
	mStates[ESTATE_TILESETIMPORT] = &TEditor::stateTileSetImport;
	mStates[ESTATE_TILEMAPIMPORT] = &TEditor::stateTileMapImport;
	mStates[ESTATE_TILEMAPCREATE] = &TEditor::stateTileMapCreate;
	mStates[ESTATE_TILEMAPDELETE] = &TEditor::stateTileMapDelete;
	mStates[ESTATE_PALETTEUPDATE] = &TEditor::statePaletteUpdate;
	mStates[ESTATE_COLMAPREMOVE] = &TEditor::stateColmapRemove;
	mStates[ESTATE_SPRITECREATE] = &TEditor::stateSpriteCreate;
	mStates[ESTATE_FRAMEDELETE] = &TEditor::stateFrameDelete;
	mStates[ESTATE_SPRITEDELETE] = &TEditor::stateSpriteDelete;
	mStates[ESTATE_FRAMEIMPORT] = &TEditor::stateFrameImport;
	mStates[ESTATE_SPRITEIMPORT] = &TEditor::stateSpriteImport;
	mStates[ESTATE_SPRITECOPY] = &TEditor::stateSpriteCopy;
	mStates[ESTATE_SPRITESCALEDCOPY] = &TEditor::stateSpriteScaledCopy;
	mStates[ESTATE_SPRITEUPSCALEDCOPY] = &TEditor::stateSpriteUpscaledCopy;
	mStates[ESTATE_SPRITEDOWNSCALEDCOPY] = &TEditor::stateSpriteDownscaledCopy;
	mStates[ESTATE_SPRITEROTATIONRANGE] = &TEditor::stateSpriteRotationRange;
	mStates[ESTATE_SPRITEROTATIONS] = &TEditor::stateSpriteRotations;
	mStates[ESTATE_FRAMESIMPORT] = &TEditor::stateFramesImport;
	mStates[ESTATE_FRAMEROTATE] = &TEditor::stateFrameRotate;
	mStates[ESTATE_FRAMESCALE] = &TEditor::stateFrameScale;
	mStates[ESTATE_SPRITECONVERTBPP] = &TEditor::stateSpriteConvertBPP;
	mStates[ESTATE_THEMECOLOR] = &TEditor::stateThemeColor;	
}

void TEditor::stateNone(){std::cout << "ESTATE_NONE"  << std::endl;}

void TEditor::stateProgramQuit(){
	bEditorRunning = false;
}

void TEditor::stateProjectSave(){
	if(saveToFolder(mGlobalSettings.ProjectPath)){		
		showMessage("Error Creating Project Folder!", true);
	} else {
		showMessage("Project Saved Successfully");
	}
}

void TEditor::stateProjectCreate(){
	mGlobalSettings.bRunningOCD = false;
}

void TEditor::stateProjectOpen(){
	mGlobalSettings.bRunningOCD = false;
}

void TEditor::stateProjectClose(){
	bEditorRunning = false;
	mGlobalSettings.bRunningOCD = true;
}

void TEditor::stateTileImport(){
	Tile* newTile = createNewTileFromFile(mGlobalSettings.mNewTilePath);
	if(newTile){
		TEActionAddTile* newActionTile = new TEActionAddTile();
		newActionTile->doAction(newTile, this, &mTileSet);
		mActionStack.newActionGroup();	
		mActionStack.addAction(newActionTile);
		mActionStack.mLastAction = newActionTile;
		mActionStack.redoClearStack();
					
		showMessage("Tile Loaded Successfully");		
	} else {							
		showMessage("Error Loading Tile!", true);							
	}
}

void TEditor::stateTileCreate(){}

void TEditor::stateTileDelete(){
	removeSelectedTile();
}

void TEditor::stateTileDeleteAll(){
	dropUnusedTiles();
}

void TEditor::stateTileSetImport(){
	std::vector<Tile*> cNewTiles;
	if(mTileSet.importTileSet(mGlobalSettings.mNewTilePath, cNewTiles)){		
		showMessage("Error Importing TileSet", true);		
	} else {

		mActionStack.newActionGroup();
				
		for(auto cTile : cNewTiles){
			TEActionAddTiles* newActionTile = new TEActionAddTiles();
			newActionTile->doAction(cTile, this, &mTileSet);	       			
	    	mActionStack.addAction(newActionTile);
	    	mActionStack.mLastAction = newActionTile;	       			
		}
				
		mActionStack.redoClearStack();
					
		showMessage("TileSet Imported Successfully");
	}
}

void TEditor::stateTileMapImport(){
	int cretval = 0;
				
	if(mGlobalSettings.bNewTileMapOffset){
		std::cout << "Importing TileMap with offset: " << mGlobalSettings.mNewTileMapOffset << std::endl;
				
		if(mGlobalSettings.mGlobalTexParam.TexBPP < 0x8){
			cretval = importTileMap(mGlobalSettings.mNewTileMapPath, mGlobalSettings.mNewTileMapOffset, mGlobalSettings.mNewTileMapPaletteOffset);
		} else {
			cretval = importTileMap(mGlobalSettings.mNewTileMapPath, mGlobalSettings.mNewTileMapOffset, 0);
		}
	} else {
		if(mGlobalSettings.mGlobalTexParam.TexBPP < 0x8){
			cretval = importTileMap(mGlobalSettings.mNewTileMapPath, 0, mGlobalSettings.mNewTileMapPaletteOffset);
		} else {
			cretval = importTileMap(mGlobalSettings.mNewTileMapPath, 0, 0);
		}
	}
				
	if(cretval){		
		if(cretval == 2){
			showMessage("Error Importing TileMap, Tiles are out of bound", true);
		} else {
			showMessage("Error Importing TileMap", true);
		}								
	} else {										
		mActionStack.redoClearStack();
		mActionStack.undoClearStack();
		
		showMessage("TileMap Imported Successfully");
		switchTileMap(mTileMaps.size()-1);
	}
}

void TEditor::stateTileMapCreate(){
	if(mGlobalSettings.mGlobalTexParam.TexBPP < 0x8){
		createTileMap(mGlobalSettings.mNewTileMapX, mGlobalSettings.mNewTileMapY, mGlobalSettings.mNewTileMapOffset, mGlobalSettings.mNewTileMapPaletteOffset);				
	} else {
		createTileMap(mGlobalSettings.mNewTileMapX, mGlobalSettings.mNewTileMapY, mGlobalSettings.mNewTileMapOffset);				
	}
										
	mActionStack.redoClearStack();
	mActionStack.undoClearStack();
	
	showMessage("TileMap Created Successfully");

	switchTileMap(mTileMaps.size()-1);
}

void TEditor::stateTileMapDelete(){
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
							
		showMessage("TileMap Removed Successfully");

		switchTileMap(0);	
	}
}

void TEditor::statePaletteUpdate(){
	updatePalette();
}

void TEditor::stateColmapRemove(){
	mTileMap->removeCollisionMap();
}

void TEditor::stateSpriteCreate(){
	bool bFirstSprite = mSprite ? false : true;

	mSprite = new TSprite(mGlobalSettings.mNewSpriteX, mGlobalSettings.mNewSpriteY, mGlobalSettings.mNewSpriteBPP);

	mSprite->createFrame(&mPalette);

	mSprites.push_back(mSprite);

	if(bFirstSprite){
		setMode(EMODE_SPRITE);					
	} else {
		switchSprite(mSprites.size()-1);
	}
}

void TEditor::stateFrameDelete(){
	removeSelectedFrame();
}

void TEditor::stateSpriteDelete(){
	int cDelSprite = -1;
	int i = 0;
	
	for(auto * cSprite : mSprites){
		if(mSprite == cSprite){
			cDelSprite = i;
		}
		i++;
	}

	if(cDelSprite != -1){								
		mSprites.erase(mSprites.begin() + cDelSprite);
	}

	if(mSprites.size() == 0){
		mSprite = NULL;					
		setMode(EMODE_MAP);
		mLastMode = EMODE_TILE;
	} else {
		if(cDelSprite > 0){
			cDelSprite--;
		}
		switchSprite(cDelSprite);
	}
}

void TEditor::stateFrameImport(){
	TSFrame* newFrame = createNewFrameFromFile(mGlobalSettings.mNewFramePath);
	if(newFrame){
		TEActionAddFrame* newActionTile = new TEActionAddFrame();
		newActionTile->doAction(newFrame, this, mSprite);
	    mSprite->mActionStack.newActionGroup();	
	    mSprite->mActionStack.addAction(newActionTile);
	    mSprite->mActionStack.mLastAction = newActionTile;
	    mSprite->mActionStack.redoClearStack();
					
		showMessage("Frame Loaded Successfully");		
	} else {							
		showMessage("Error Loading Frame!", true);							
	}
}

void TEditor::stateSpriteImport(){
	std::vector<unsigned char> sbuffer;
	std::vector<TSFrame*> cNewFrames;
	fs::path cSpritePath;
	SDL_Surface *newSurf;
	
	bool bSpriteImportSuccess = false;

	switch (mGlobalSettings.mNewSpriteType){
		
		case 0:												
			if(mGlobalSettings.getSpriteFileHeader(mGlobalSettings.mNewSpritePath, mGlobalSettings.mNewSpriteX,  mGlobalSettings.mNewSpriteY,  mGlobalSettings.mNewSpriteBPP, sbuffer)){
				mSprite = new TSprite(mGlobalSettings.mNewSpriteX,  mGlobalSettings.mNewSpriteY,  mGlobalSettings.mNewSpriteBPP);
				mSprite->loadFromBuffer(sbuffer, &mPalette);
				mSprites.push_back(mSprite);
				bSpriteImportSuccess = true;						
			} else {
				std::cout << "Error Importing Sprite: " <<  mGlobalSettings.mNewSpritePath << std::endl;
				showMessage("Error Importing Sprite", true);
			}	
			break;
			
		case 1:				
			newSurf = IMG_Load(mGlobalSettings.mNewSpritePath.c_str());
			if(newSurf){
				TSprite *cSprite = new TSprite(mGlobalSettings.mNewSpriteX,  mGlobalSettings.mNewSpriteY,  mGlobalSettings.mNewSpriteBPP);
				if(cSprite->importPNG(newSurf,  &mGlobalSettings.CurrentEditor->mPalette, cNewFrames)){
					std::cout << "Error Importing Sprite: " << mGlobalSettings.mNewSpritePath << std::endl;								
					showMessage("Error Importing Sprite", true);
				} else {
					mSprite = cSprite;								
					mSprites.push_back(mSprite);
					bSpriteImportSuccess = true;
				}	
			}else {
				std::cout << "Error Importing Sprite: " <<  mGlobalSettings.mNewSpritePath << std::endl;	
				showMessage("Error Importing Sprite", true);
			}			
			break;					

		case 2:
			cSpritePath = mGlobalSettings.mNewSpritePath;

			if((fs::exists(fs::status(cSpritePath)))  && !(fs::is_directory(fs::status(cSpritePath)))){		
				std::ifstream infile(cSpritePath, std::ios::binary );
    			std::vector<unsigned char> tbuffer(std::istreambuf_iterator<char>(infile), {});

				if(tbuffer.size() % ((mGlobalSettings.mNewSpriteX * mGlobalSettings.mNewSpriteY) /mGlobalSettings.mTileBPPSize[mGlobalSettings.mNewSpriteBPP])){
					std::cout << "Error Importing Sprite: " <<  mGlobalSettings.mNewSpritePath << std::endl;
					showMessage("Error Importing Sprite", true);
				} else {
					mSprite = new TSprite(mGlobalSettings.mNewSpriteX,  mGlobalSettings.mNewSpriteY,  mGlobalSettings.mNewSpriteBPP);
					mSprite->loadFromBuffer(tbuffer, &mPalette);
					mSprites.push_back(mSprite);
					bSpriteImportSuccess = true;
				}
			} else {
				std::cout << "Error Importing Sprite: " <<  mGlobalSettings.mNewSpritePath << std::endl;
				showMessage("Error Importing Sprite", true);
			}
			break;
				
		default:
			break;
	};

	if(bSpriteImportSuccess){
		if(mCurMode == EMODE_SPRITE){
			switchSprite(mSprites.size()-1);
		} else {
			setMode(EMODE_SPRITE);
		}
	}
}

void TEditor::stateSpriteCopy(){
	createNewSpriteCopy(mSprite);
	showMessage("Sprite Copied Successfully");
}

void TEditor::stateSpriteScaledCopy(){
	createNewSpriteScaledCopy(mSprite);
	showMessage("Sprite Copied Successfully");
}

void TEditor::stateSpriteUpscaledCopy(){
	createNewSpriteUpscaledCopy(mSprite);
	showMessage("Sprite Copied Successfully");
}

void TEditor::stateSpriteDownscaledCopy(){
	createNewSpriteDownscaledCopy(mSprite);
	showMessage("Sprite Copied Successfully");
}

void TEditor::stateSpriteRotationRange(){
	createSpriteRotationRange(mGlobalSettings.mNewSpriteRange, mGlobalSettings.mNewSpriteRangeIntervals);
	showMessage("Sprite Rotation Range Complete");
}

void TEditor::stateSpriteRotations(){
	createSpriteRotations(mGlobalSettings.mNewSpriteRotations, mGlobalSettings.mNewSpriteRotationsAngle);
	showMessage("Sprite Frame Rotations Complete");
}

void TEditor::stateFramesImport(){
	std::vector<unsigned char> fbuffer;
	std::vector<TSFrame*> cNewFrames;
	fs::path cFramesPath;
	SDL_Surface *newSurf;
	
	bool bFramesImportSuccess = false;

	newSurf = IMG_Load(mGlobalSettings.mNewFramesPath.c_str());
	if(newSurf){
		if(!mSprite->importPNG(newSurf, &mGlobalSettings.CurrentEditor->mPalette, cNewFrames)){
			bFramesImportSuccess = true;							
		} 	
	}else if (mGlobalSettings.getSpriteFileHeader(mGlobalSettings.mNewFramesPath, mGlobalSettings.mNewSpriteX,  mGlobalSettings.mNewSpriteY,  mGlobalSettings.mNewSpriteBPP, fbuffer)){
		if( (mGlobalSettings.mNewSpriteX == mSprite->mTexParam.TexSizeX) && (mGlobalSettings.mNewSpriteY == mSprite->mTexParam.TexSizeY) && (mGlobalSettings.mNewSpriteBPP == mSprite->mTexParam.TexBPP) ){
			if(!mSprite->importFromBuffer(fbuffer, &mPalette, cNewFrames)){
				bFramesImportSuccess = true;
			} 
		} 
	} else {
		cFramesPath = mGlobalSettings.mNewFramesPath;

		if((fs::exists(fs::status(cFramesPath)))  && !(fs::is_directory(fs::status(cFramesPath)))){		
			std::ifstream infile(cFramesPath, std::ios::binary );
    		std::vector<unsigned char> tbuffer(std::istreambuf_iterator<char>(infile), {});

			if((tbuffer.size() % ((mSprite->mTexParam.TexSizeX * mSprite->mTexParam.TexSizeY) / mGlobalSettings.mTileBPPSize[mSprite->mTexParam.TexBPP])) == 0){							
				if(!mSprite->importFromBuffer(tbuffer, &mPalette, cNewFrames)){
					bFramesImportSuccess = true;
				} 
			}
		}
	}

	if(bFramesImportSuccess){
					
		mSprite->mActionStack.newActionGroup();
				
		for(auto cFrame : cNewFrames){
			TEActionAddFrames* newActionTile = new TEActionAddFrames();
			newActionTile->doAction(cFrame, this, mSprite);	       			
			mSprite->mActionStack.addAction(newActionTile);
	       	mSprite->mActionStack.mLastAction = newActionTile;	       			
		}
					
		mSprite->mActionStack.redoClearStack();

		showMessage("Frame(s) Imported Successfully");
		std::cout << "Frames Imported Successfully" << std::endl;
	} else {										
		showMessage("Error Importing Frame(s)", true);
		std::cout << "Error Importing Frames: " <<  mGlobalSettings.mNewFramesPath << std::endl;
	}
}

void TEditor::stateFrameRotate(){
	if(mCurMode == EMODE_SPRITE){		
		TSFrame* newFrame = new TSFrame(&mSprite->mTexParam); 
		newFrame->loadFromBuffer(mSprite->mFrame->FileData, &mPalette);
		newFrame->rotate(mGlobalSettings.mRotateFrameAngle);					
		TEActionReplaceFrame* newAction = new TEActionReplaceFrame();				
		newAction->doAction(mSprite->mFrame, newFrame->FileData, this, mSprite);
      	mSprite->mActionStack.newActionGroup();	
      	mSprite->mActionStack.addAction(newAction);

		mSprite->mActionStack.mLastAction = newAction;
       	mSprite->mActionStack.redoClearStack();		
	}
}

void TEditor::stateFrameScale(){
	if(mCurMode == EMODE_SPRITE){		
		TSFrame* newFrame = new TSFrame(&mSprite->mTexParam); 
		newFrame->loadFromBuffer(mSprite->mFrame->FileData, &mPalette);
		newFrame->scale(mGlobalSettings.mScaleFrameFactor);					
		TEActionReplaceFrame* newAction = new TEActionReplaceFrame();				
		newAction->doAction(mSprite->mFrame, newFrame->FileData, this, mSprite);
      	mSprite->mActionStack.newActionGroup();	
      	mSprite->mActionStack.addAction(newAction);

		mSprite->mActionStack.mLastAction = newAction;
       	mSprite->mActionStack.redoClearStack();		
	}
}

void TEditor::stateSpriteConvertBPP(){
	if(mSprite->mTexParam.TexBPP == 8){
		createNewSpriteConvertBPP4(mSprite);
	} else {
		createNewSpriteConvertBPP8(mSprite);
	}
				
	showMessage("Sprite Converted Successfully");
}

void TEditor::stateThemeColor(){
	setThemeColors();
}

void TEditor::createDialogs(){
	
	initStates();

	mHelpDialog.init();
	
	mDTDialogs[EDIALOG_ERROR] = NULL;
	mDTDialogs[EDIALOG_SPRITECREATECOPY] = DTDialog::createSpriteCopyDialog();
	mDTDialogs[EDIALOG_SPRITECREATESCALEDCOPY] = DTDialog::createSpriteScaledCopyDialog();
	mDTDialogs[EDIALOG_SPRITECREATEUPSCALEDCOPY] = DTDialog::createSpriteUpscaledCopyDialog();
	mDTDialogs[EDIALOG_SPRITECREATEDOWNSCALEDCOPY] = DTDialog::createSpriteDownscaledCopyDialog();
	mDTDialogs[EDIALOG_SPRITECREATEROTATIONRANGE] = DTDialog::createSpriteRotationRangeDialog();
	mDTDialogs[EDIALOG_SPRITECREATEFRAMEROTATIONS] = DTDialog::createSpriteRotationsDialog();	
	mDTDialogs[EDIALOG_SPRITEFRAMEIMPORT] =  DTDialog::createSpriteFrameImportDialog();
	mDTDialogs[EDIALOG_SPRITEFRAMESIMPORT] =  DTDialog::createSpriteFramesImportDialog();
	mDTDialogs[EDIALOG_TILEMAPIMPORT] = DTDialog::createTileMapImportDialog();
	mDTDialogs[EDIALOG_SPRITECONVERTBPP] =  DTDialog::createSpriteConvertBPPDialog();
	mDTDialogs[EDIALOG_HELPMENU] = &mHelpDialog;
	mDTDialogs[EDIALOG_THEMECOLOR] = DTDialog::createThemeColorDialog();
	mDTDialogs[EDIALOG_PROJECTOPEN] = DTDialog::createProjectOpenDialog();
	mDTDialogs[EDIALOG_PROJECTCREATE] = DTDialog::createProjectCreateDialog();
	mDTDialogs[EDIALOG_PROJECTCLOSE] = DTDialog::createProjectCloseDialog();
	mDTDialogs[EDIALOG_PROGRAMQUIT] = DTDialog::createProgramQuitDialog();
	mDTDialogs[EDIALOG_PROJECTSAVE] = DTDialog::createProjectSaveDialog();
	mDTDialogs[EDIALOG_PROJECTSAVEAS] = DTDCDialog::createProjectSaveAsDialog();
	mDTDialogs[EDIALOG_TILESETIMPORT] = DTDialog::createTileSetImportDialog();
	mDTDialogs[EDIALOG_TILEMAPDELETE] = DTDialog::createTileMapRemoveDialog();
	mDTDialogs[EDIALOG_COLMAPREMOVE] = DTDialog::createColMapRemoveDialog();
	mDTDialogs[EDIALOG_TILEDELETEALL] = DTDialog::createTilesUnusedRemoveDialog();
	mDTDialogs[EDIALOG_FRAMEDELETE] = DTDialog::createFrameRemoveDialog();
	mDTDialogs[EDIALOG_SPRITEDELETE] = DTDialog::createSpriteRemoveDialog();
	mDTDialogs[EDIALOG_TILEDELETE] = DTDialog::createTileRemoveDialog();
	mDTDialogs[EDIALOG_TILEIMPORT] = DTDialog::createTileImportDialog();
	mDTDialogs[EDIALOG_FRAMESCALE] = DTDialog::createFrameScaleDialog();
	mDTDialogs[EDIALOG_FRAMEROTATE] = DTDialog::createFrameRotateDialog();
	mDTDialogs[EDIALOG_TILEMAPCREATE] = DTDialog::createTileMapCreateDialog();

	mProjectInfo = IDDialog::createProjectInfoDialog();
	mProgramInfo = IDDialog::createMessageDialog();
}

int TEditor::activateDTDialog(int cOpenDialog, int cCond, int cVal0, int cVal1){
	
	DTDialog* cDialog = mDTDialogs[cOpenDialog];

	if(cDialog){
		cancelActiveDialog();
		cDialog->setCondition(cCond);
		cDialog->setValue(0, cVal0);
		cDialog->setValue(1, cVal1);
		cDialog->update();
		mActiveDialog = cDialog;
		return 0;
	}

	return 1;
}

void TEditor::initDialogs(){
	mMapSelectedTile = 0;
	mTileSelectedTile = mTileSet.TTiles[0];
	mTileSelectedTile->bIsSelected = true;
	mColorSelectedTile = mPalette.TPixels[0];
	mColorSelectedTile->bPixelSelected = true;
	mColorSelected = 0;

	mTopBar.mEditor = this;
	mTopBar.init();
	
	mOpenSpriteDialog.init();

	//mNewTileMapDialog.init();
	mNewSpriteDialog.init();
	
	mPaletteUpdate.init();

	mPaletteOffset.init();

	if(mGlobalSettings.mGlobalTexParam.TexBPP < 0x8){
		mGlobalSettings.bShowPaletteOffset = true;
	}

	//mBrushesTile.MaxScale = 32;
	mBrushesTile.init("Tiles","Tile", TBRUSH_TILE, &bShowBrushesTile,mGlobalSettings.mGlobalTexParam.TexSizeX, mGlobalSettings.mGlobalTexParam.TexSizeY, &mGlobalSettings.TileMapScale, mGlobalSettings.TileMapScale, &mCurrentBrushTile, &mGlobalSettings.mGlobalTexParam);
	
	mBrushesPixel.init("Pixels","Pixel", TBRUSH_PIXEL, &bShowBrushesPixel, mGlobalSettings.mGlobalTexParam.TexPixelSize, mGlobalSettings.mGlobalTexParam.TexPixelSize, &mGlobalSettings.mGlobalTexParam.TexEditScale, mGlobalSettings.mGlobalTexParam.TexEditScale, &mCurrentBrushPixel, &mGlobalSettings.mGlobalTexParam);
	mBrushesPixelSelEd.init("Pixels Selection","Pixel", TBRUSH_PIXEL, &bShowBrushesPixel, mGlobalSettings.mGlobalTexParam.TexPixelSize, mGlobalSettings.mGlobalTexParam.TexPixelSize, &mGlobalSettings.mGlobalTexParam.TexEditScale, mGlobalSettings.mGlobalTexParam.TexEditScale, &mCurrentBrushPixel, &mGlobalSettings.mGlobalTexParam);
    
    if(mGlobalSettings.mGlobalTexParam.TexSizeX < 16){
        mBrushesPixelSelEd.mMaxX = 8;        
    }
    if(mGlobalSettings.mGlobalTexParam.TexSizeY < 16){
        mBrushesPixelSelEd.mMaxY = 8;        
    }

	mClipboardMap.init("Maps","Tile", TBRUSH_TILE, &bShowClipboardMap,mGlobalSettings.mGlobalTexParam.TexSizeX, mGlobalSettings.mGlobalTexParam.TexSizeY, &mGlobalSettings.TileMapScale, mGlobalSettings.TileMapScale, &mCurrentBrushTile, &mGlobalSettings.mGlobalTexParam);
	mClipboardMap.bUseTileOffset = true;
	mClipboardMap.MinScale = 4;
	//mClipboardMap.MaxScale = 32;

	mSprite4.TexBPP = 4;

	mBrushesPixelSprite4.init("Sprite 4BPP","Pixel", TBRUSH_PIXEL, &bShowBrushesPixel, mSprite4.TexPixelSize, mSprite4.TexPixelSize, &mSprite4.TexEditScale, mSprite4.TexEditScale, &mCurrentBrushPixel, &mSprite4);
	mBrushesPixelSprite8.init("Sprite 8BPP","Pixel", TBRUSH_PIXEL, &bShowBrushesPixel, mSprite8.TexPixelSize, mSprite8.TexPixelSize, &mSprite8.TexEditScale, mSprite8.TexEditScale, &mCurrentBrushPixel, &mSprite8);	

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

	mTileSet.mClipboardTiles.init("Tiles","Pixel", TBRUSH_PIXEL, &mTileSet.bShowClipboardTiles, mGlobalSettings.mGlobalTexParam.TexPixelSize, mGlobalSettings.mGlobalTexParam.TexPixelSize, &mGlobalSettings.mGlobalTexParam.TexEditScale, mGlobalSettings.mGlobalTexParam.TexEditScale, &mGlobalSettings.CurrentEditor->mCurrentBrushPixel, &mGlobalSettings.mGlobalTexParam);
	mTileSet.mClipboardTileSet.init("TileSet","Pixel", TBRUSH_PIXEL, &mTileSet.bShowClipboardTileSet, mGlobalSettings.mGlobalTexParam.TexPixelSize, mGlobalSettings.mGlobalTexParam.TexPixelSize, &mGlobalSettings.mGlobalTexParam.TexEditScale, mGlobalSettings.mGlobalTexParam.TexEditScale, &mGlobalSettings.CurrentEditor->mCurrentBrushPixel, &mGlobalSettings.mGlobalTexParam);

	//TODO maybe

	mTileMap = new TileMap();

	if(mTileMap->loadFromFile(path, "map0.bin", true)){
		std::cout << "Error: can't read: " << path << DIRDEL << "map0.bin" << std::endl;
		return 1;
	}

	mTileMaps.push_back(mTileMap);

	fs::path cTileMaps;
	int mMapNum = 1;
	std::string cMapNum;
	std::stringstream convert;

	convert << mMapNum << std::endl;
	convert >> cMapNum;

	cTileMaps = std::string(path + DIRDEL + "map" + cMapNum + ".bin");

	while(fs::exists(fs::status(cTileMaps))){
		TileMap *newTileMap = new TileMap();
		if(newTileMap->loadFromFile(cTileMaps.parent_path().string(), cTileMaps.filename().string(), true)){
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
		if(mGlobalSettings.mProjectSettings.load(path + DIRDEL + "settings.ini")){
			std::cout << "Error Loading Settings" << std::endl;
		} else {
			std::cout << "Loading Settings" << std::endl;
			mGlobalSettings.bSelectionMode = mGlobalSettings.mProjectSettings.Editor_SelectionAppend->getBool();

			mGlobalSettings.bShowPixelGrid = mGlobalSettings.mProjectSettings.Tile_ShowPixelGrid->getBool();
			
			mGlobalSettings.bShowTilePixelGrid = mGlobalSettings.mProjectSettings.TileSet_ShowPixelGrid->getBool();
			mGlobalSettings.bShowTileGrid = mGlobalSettings.mProjectSettings.TileSet_ShowTileGrid->getBool();
			
			mGlobalSettings.bTileSetOrderUpdateTileMap = mGlobalSettings.mProjectSettings.TileSet_UpdateMaps->getBool();
			mGlobalSettings.bTileSetWarnBeforeDelete = mGlobalSettings.mProjectSettings.TileSet_WarnBeforeDelete->getBool();
			
			mTileSet.mSelEdWidth = mGlobalSettings.mProjectSettings.TileSet_EditWidth->getInteger();
			mGlobalSettings.mTileSetEditWidth = mTileSet.mSelEdWidth;
			mGlobalSettings.mTileSetEditScale = mGlobalSettings.mProjectSettings.TileSet_PixelScale->getInteger();

			mTileSet.mMaxColumns = mGlobalSettings.mProjectSettings.TileSet_MaxGridWidth->getInteger();

			mTileSet.resizeEdit();		

			mGlobalSettings.bShowTilePixelSelGrid = mGlobalSettings.mProjectSettings.SelectionEdit_ShowPixelGrid->getBool();
			mGlobalSettings.bShowTileSelGrid = mGlobalSettings.mProjectSettings.SelectionEdit_ShowTileGrid->getBool();
			mGlobalSettings.mSelectionEditScale = mGlobalSettings.mProjectSettings.SelectionEdit_PixelScale->getInteger();

			mGlobalSettings.bShowPixelGridSprite = mGlobalSettings.mProjectSettings.Sprite_ShowPixelGrid->getBool();
			mGlobalSettings.bSpriteWarnBeforeDelete = mGlobalSettings.mProjectSettings.Sprite_WarnBeforeDelete->getBool();

			mGlobalSettings.mUseTextureFiltering = mGlobalSettings.mProjectSettings.Editor_UseTextureFiltering->getInteger();

			mGlobalSettings.mGlobalTexParam.TexPixelSize = mGlobalSettings.mProjectSettings.Editor_PixelScale->getInteger();
			
		}
	}

	/* Sprites */

	fs::path cSprites;
	int nSpriteNum = 0;
	std::string sSpriteNum;
	

	convert << nSpriteNum << std::endl;
	convert >> sSpriteNum;

	cSprites = std::string(path + DIRDEL + "sprite" + sSpriteNum + ".bin");

	while(fs::exists(fs::status(cSprites))){
		

		int nSpriteX, nSpriteY, nSpriteBPP;
		std::vector<unsigned char> sbuffer;


		if(mGlobalSettings.getSpriteFileHeader(cSprites.string(), nSpriteX, nSpriteY, nSpriteBPP, sbuffer)){
			mSprite = new TSprite(nSpriteX, nSpriteY, nSpriteBPP);
			mSprite->loadFromBuffer(sbuffer, &mPalette);
			mSprites.push_back(mSprite);

			sKey* cSKey = mGlobalSettings.mProjectSettings.getSpriteScaleKey(nSpriteNum);

			if(cSKey){				
				mSprite->mTexParam.TexPixelSize = cSKey->getInteger();
			}

			cSKey = mGlobalSettings.mProjectSettings.getSpriteGridKey(nSpriteNum);

			if(cSKey){				
				mSprite->mMaxColumns = cSKey->getInteger();
			}

			if(mGlobalSettings.mProjectSettings.Clipboard_SaveToProject->bvalue){
				if(fs::exists(fs::status(path + DIRDEL + "clipboards" + sSpriteNum + ".dat"))){
					mSprite->mClipboard.loadFromFile(path + DIRDEL + "clipboards" + sSpriteNum + ".dat");
				}
			}

		} else {
			std::cout << "Error reading file: " <<  cSprites.string() << std::endl;
		}
		
		nSpriteNum++;
		convert << nSpriteNum << std::endl;
		convert >> sSpriteNum;

		cSprites = std::string(path + DIRDEL + "sprite" + sSpriteNum + ".bin");
	}

	/* Sprites End */
	
	initDialogs();

	for(int mi = 0; mi < mTileMaps.size(); mi++){
		sKey* cSKey = mGlobalSettings.mProjectSettings.getMapTileZeroKey(mi);

		if(cSKey){				
			mTileMaps[mi]->bIsTileZeroTransparent = cSKey->getBool();
		}
	}

	if(mGlobalSettings.mProjectSettings.Clipboard_SaveToProject->bvalue){
		if(fs::exists(fs::status(path + DIRDEL + "clipboardmaps.dat"))){			
			mClipboardMap.loadFromFile(path + DIRDEL + "clipboardmaps.dat");
		}
		if(fs::exists(fs::status(path + DIRDEL + "clipboardtiles.dat"))){
			mTileSet.mClipboardTiles.loadFromFile(path + DIRDEL + "clipboardtiles.dat");
		}
		if(fs::exists(fs::status(path + DIRDEL + "clipboardtileset.dat"))){
			mTileSet.mClipboardTileSet.loadFromFile(path + DIRDEL + "clipboardtileset.dat");
		}
	}

	switchTileMap(0);

	if(mSprites.size()){

		switchSprite(0);

		if(fs::exists(fs::status(path+DIRDEL+"s4brushes.dat"))){
			mBrushesPixelSprite4.loadFromFile(path+DIRDEL+"s4brushes.dat");
		}

		if(fs::exists(fs::status(path+DIRDEL+"s8brushes.dat"))){
			mBrushesPixelSprite8.loadFromFile(path+DIRDEL+"s8brushes.dat");
		}
	}


	if(fs::exists(fs::status(path+DIRDEL+"tbrushes.dat"))){
		mBrushesTile.loadFromFile(path+DIRDEL+"tbrushes.dat");
	}

	if(fs::exists(fs::status(path+DIRDEL+"pbrushes.dat"))){
		mBrushesPixel.loadFromFile(path+DIRDEL+"pbrushes.dat");
	}

	if(fs::exists(fs::status(path+DIRDEL+"pbrushesseled.dat"))){
		mBrushesPixelSelEd.loadFromFile(path+DIRDEL+"pbrushesseled.dat");
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

	for(int mi = 0; mi < mTileMaps.size(); mi++){
		sKey* cSKey = mGlobalSettings.mProjectSettings.getMapTileZeroKey(mi);

		if(cSKey){				
			cSKey->bvalue = mTileMaps[mi]->bIsTileZeroTransparent;
		} else {
			cSKey = mGlobalSettings.mProjectSettings.createMapTileZeroKey(mi);
			cSKey->bvalue = mTileMaps[mi]->bIsTileZeroTransparent;
		}
	}
	
	mTileSet.saveToFolder(path);
	//TODO maybe
	mTileMaps[0]->saveToFolder(path, "map0.bin");

	int mMapNum = 1;
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

		std::string cColMapPrefix;
		cColMapPrefix = "map" + cMapNum;

		fs::path cColDat = path + DIRDEL + "col" + cColMapPrefix + ".dat";
		if(fs::exists(fs::status(cColDat))){
			fs::remove(cColDat);
		}

		fs::path cColBin = path + DIRDEL + "col" + cColMapPrefix + ".bin";
		if(fs::exists(fs::status(cColBin))){
			fs::remove(cColBin);
		}

		sKey* cSKey = mGlobalSettings.mProjectSettings.getMapTileZeroKey(mMapNum);

		if(cSKey){				
			mGlobalSettings.mProjectSettings.removeMapTileZeroKey(mMapNum);
		}
					
		mMapNum++;
		convert << mMapNum << std::endl;
		convert >> cMapNum;

		cTileMaps = std::string(path + DIRDEL + "map" + cMapNum + ".bin");
	}

	mGlobalSettings.mTileMapFileCount = mTileMaps.size();

	mPalette.saveToFolder(path);

	mBrushesTile.saveToFile(path + DIRDEL + "tbrushes.dat");
	mBrushesPixel.saveToFile(path + DIRDEL + "pbrushes.dat");
	mBrushesPixelSelEd.saveToFile(path + DIRDEL + "pbrushesseled.dat");

	if(mGlobalSettings.mProjectSettings.Clipboard_SaveToProject->bvalue){
		mClipboardMap.saveToFile(path + DIRDEL + "clipboardmaps.dat");
		mTileSet.mClipboardTiles.saveToFile(path + DIRDEL + "clipboardtiles.dat");
		mTileSet.mClipboardTileSet.saveToFile(path + DIRDEL + "clipboardtileset.dat");
	} else {
		if(fs::exists(fs::status(path + DIRDEL + "clipboardmaps.dat"))){
			fs::remove(path + DIRDEL + "clipboardmaps.dat");
		}
		if(fs::exists(fs::status(path + DIRDEL + "clipboardtiles.dat"))){
			fs::remove(path + DIRDEL + "clipboardtiles.dat");
		}
		if(fs::exists(fs::status(path + DIRDEL + "clipboardtileset.dat"))){
			fs::remove(path + DIRDEL + "clipboardtileset.dat");
		}
	}

	int nSpriteNum = 0;
	std::string sSprite;
	std::string sSpriteNum;


	if(mSprites.size()){
		for(int i = 0; i < mSprites.size(); i++){
			convert << nSpriteNum << std::endl;
			convert >> sSpriteNum;
			sSprite = "sprite" + sSpriteNum + ".bin";
			mSprites[i]->saveToFile(path, sSprite);

			sKey *cSKey = mGlobalSettings.mProjectSettings.getSpriteScaleKey(nSpriteNum);

			if(cSKey){
				cSKey->ivalue = mSprites[i]->mTexParam.TexPixelSize;
			} else {
				sKey *cNewSKey = mGlobalSettings.mProjectSettings.createSpriteScaleKey(nSpriteNum);
				cNewSKey->ivalue = mSprites[i]->mTexParam.TexPixelSize;
			}

			cSKey = mGlobalSettings.mProjectSettings.getSpriteGridKey(nSpriteNum);

			if(cSKey){
				cSKey->ivalue = mSprites[i]->mMaxColumns;
			} else {
				sKey *cNewSKey = mGlobalSettings.mProjectSettings.createSpriteGridKey(nSpriteNum);
				cNewSKey->ivalue = mSprites[i]->mMaxColumns;
			}

			if(mGlobalSettings.mProjectSettings.Clipboard_SaveToProject->bvalue){
				mSprites[i]->mClipboard.saveToFile(path + DIRDEL + "clipboards" + sSpriteNum + ".dat");
			} else {
				if(fs::exists(fs::status(path + DIRDEL + "clipboards" + sSpriteNum + ".dat"))){
					fs::remove(path + DIRDEL + "clipboards" + sSpriteNum + ".dat");
				}
			}

			nSpriteNum++;
		}

		mBrushesPixelSprite4.saveToFile(path + DIRDEL + "s4brushes.dat");
		mBrushesPixelSprite8.saveToFile(path + DIRDEL + "s8brushes.dat");

	} else {
		if(fs::exists(fs::status(path+DIRDEL+"s4brushes.dat"))){
			fs::remove(path+DIRDEL+"s4brushes.dat");
		}

		if(fs::exists(fs::status(path+DIRDEL+"s8brushes.dat"))){
			fs::remove(path+DIRDEL+"s8brushes.dat");
		}
	}

	convert << nSpriteNum << std::endl;
	convert >> sSpriteNum;
	
	sSprite = std::string(path + DIRDEL + "sprite" + sSpriteNum + ".bin");


	while(fs::exists(fs::status(sSprite))){
		std::string cBackupNum;
		int mBackupNum = 0;

		convert << mBackupNum << std::endl;
		convert >> cBackupNum;

		while(fs::exists(fs::status(sSprite + cBackup + cBackupNum))){
			mBackupNum++;
			convert << mBackupNum << std::endl;
			convert >> cBackupNum;
		}

		fs::rename(sSprite, sSprite + cBackup + cBackupNum);

		if(fs::exists(fs::status(path + DIRDEL + "clipboards" + sSpriteNum + ".dat"))){
			fs::remove(path + DIRDEL + "clipboards" + sSpriteNum + ".dat");
		}

		sKey *cDelSKey = mGlobalSettings.mProjectSettings.getSpriteScaleKey(nSpriteNum);

		if(cDelSKey){
			mGlobalSettings.mProjectSettings.removeSpriteScaleKey(nSpriteNum);
		}

		cDelSKey = mGlobalSettings.mProjectSettings.getSpriteGridKey(nSpriteNum);

		if(cDelSKey){
			mGlobalSettings.mProjectSettings.removeSpriteGridKey(nSpriteNum);
		}
					
		nSpriteNum++;
		convert << nSpriteNum << std::endl;
		convert >> sSpriteNum;

		sSprite = std::string(path + DIRDEL + "sprite" + sSpriteNum + ".bin");
	}


	mGlobalSettings.mProjectSettings.Editor_SelectionAppend->bvalue = mGlobalSettings.bSelectionMode;

	mGlobalSettings.mProjectSettings.Tile_ShowPixelGrid->bvalue = mGlobalSettings.bShowPixelGrid;

	mGlobalSettings.mProjectSettings.TileSet_ShowPixelGrid->bvalue = mGlobalSettings.bShowTilePixelGrid;
	mGlobalSettings.mProjectSettings.TileSet_ShowTileGrid->bvalue = mGlobalSettings.bShowTileGrid;

 	mGlobalSettings.mProjectSettings.TileSet_UpdateMaps->bvalue = mGlobalSettings.bTileSetOrderUpdateTileMap;
	mGlobalSettings.mProjectSettings.TileSet_WarnBeforeDelete->bvalue = mGlobalSettings.bTileSetWarnBeforeDelete;
 	mGlobalSettings.mProjectSettings.TileSet_EditWidth->ivalue = mTileSet.mSelEdWidth;
	mGlobalSettings.mProjectSettings.TileSet_PixelScale->ivalue = mGlobalSettings.mTileSetEditScale;

	mGlobalSettings.mProjectSettings.TileSet_MaxGridWidth->ivalue = mTileSet.mMaxColumns;

	mGlobalSettings.mProjectSettings.SelectionEdit_ShowPixelGrid->bvalue = mGlobalSettings.bShowTilePixelSelGrid;
	mGlobalSettings.mProjectSettings.SelectionEdit_ShowTileGrid->bvalue = mGlobalSettings.bShowTileSelGrid;
	mGlobalSettings.mProjectSettings.SelectionEdit_PixelScale->ivalue = mGlobalSettings.mSelectionEditScale;

	mGlobalSettings.mProjectSettings.Sprite_ShowPixelGrid->bvalue = mGlobalSettings.bShowPixelGridSprite;
	mGlobalSettings.mProjectSettings.Sprite_WarnBeforeDelete->bvalue = mGlobalSettings.bSpriteWarnBeforeDelete;

	mGlobalSettings.mProjectSettings.Editor_UseTextureFiltering->ivalue = mGlobalSettings.mUseTextureFiltering;

	mGlobalSettings.mProjectSettings.Editor_PixelScale->ivalue = mGlobalSettings.mGlobalTexParam.TexPixelSize;

	mGlobalSettings.mProjectSettings.writedefault(path + DIRDEL + "settings.ini");

	return 0;
}

int TEditor::switchSprite(int cSprite){
	if((cSprite > -1) &&  (cSprite < mSprites.size())){
		mSprite = mSprites[cSprite];		
		mPalette.bUpdateWinPos = true;
		mSprite->updateWinPos = true;
		mSelectedSprite = cSprite;
		setSpriteBrushes();
		return 0;
	}
	return 1;
}

int TEditor::switchSprite(int cSprite, int cFrame){
	switchSprite(cSprite);
	mSprite->selectFrame(cFrame);
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

		mSelectedTileMap = cTileMap;

		bTileMapWasChanged = true;
		bShowCollisionEditor = false;
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
		mTileSet.renderIm(mGlobalSettings.TopBarHeight, 0);//mTileSetScrollY);
		if(mGlobalSettings.bShowProjectInfo){			
			showProjectInfo();
		}
		if(mGlobalSettings.bShowPaletteOffset){			
			mPaletteOffset.render(mProjectInfo->mDialogWidth,mGlobalSettings.TopBarHeight, &mGlobalSettings.mGlobalTexParam);
		}

		mTileMap->mSelection.renderSelection();	 

		if(mCurrentBrushTile){
			mCurrentBrushTile->renderSelection();
		}

		if(bShowBrushesTile){
			mBrushesTile.renderIm();
		} /*else {
			mBrushesTile.closeEdit();
		}*/

		if(bShowClipboardMap){
			mClipboardMap.renderIm();
		}

		if(bShowCollisionEditor){
			mColMapEdit.render();
		}
	}
	
	if(mCurMode == EMODE_TILE){
		if(mCurrentBrushPixel){
			mCurrentBrushPixel->getBrushSelection(cx, cy, mTileSelectedTile->PixelAreas);
		}

		mPalette.renderIm(100+mGlobalSettings.mGlobalTexParam.TexEditScale*mGlobalSettings.mGlobalTexParam.TexSizeX*mGlobalSettings.mGlobalTexParam.TexPixelSize,50+mGlobalSettings.TopBarHeight, &mGlobalSettings.mGlobalTexParam);	
		if(!mGlobalSettings.bShowPixelType) mColorSelectedTile->bPixelSelected = false;
		mTileSelectedTile->renderEd(mTileScrollX + 50,mTileScrollY + 50 + mGlobalSettings.TopBarHeight,&mPalette);
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
			showProjectInfo();
		}	

		if(mCurrentBrushPixel){
			mCurrentBrushPixel->renderSelection();
		}

		if(bShowBrushesPixel){
			mBrushesPixel.renderIm();
		} /* else {
			mBrushesPixel.closeEdit();
		}*/
		
		if(mTileSet.bShowClipboardTiles){
			mTileSet.mClipboardTiles.renderIm();
		}

	}

	if(mCurMode == EMODE_SPRITE){
		if(mSprite->mCurrentBrushPixel){
			mSprite->mCurrentBrushPixel->getBrushSelection(cx, cy, mSprite->mFrame->PixelAreas);
		}

		mPalette.renderIm(100+mSprite->mTexParam.TexEditScale*mSprite->mTexParam.TexSizeX*mSprite->mTexParam.TexPixelSize,50+mGlobalSettings.TopBarHeight, &mSprite->mTexParam);	
		if(!mGlobalSettings.bShowPixelTypeSprite) mColorSelectedTile->bPixelSelected = false;
		mSprite->renderEd(mSprite->mSpriteScrollX + 50, mSprite->mSpriteScrollY + 50 + mGlobalSettings.TopBarHeight,&mPalette);
		mColorSelectedTile->bPixelSelected = true;

		mSprite->mFrame->mSelection.renderSelection();	    

		if(mActiveDialog){
			mActiveDialog->render();
		}
		if(mActiveMessage){
			mActiveMessage->render();
		}
		
		mTopBar.render();

		if(mSprite->bShowSpriteFrames){
			mSprite->renderIm(mGlobalSettings.TopBarHeight, 0);// mSprite->mFrameScrollY);
		}

		if(mGlobalSettings.bShowProjectInfo){			
			showProjectInfo();
		}	

		if(mSprite->mCurrentBrushPixel){
			mSprite->mCurrentBrushPixel->renderSelection();
		}

		if(mSprite->bShowBrushesPixel){
			mBrushesSprite->renderIm();
		} /*else {
			mBrushesSprite->closeEdit();
		}*/

		if(mSprite->bShowClipboard){
			mSprite->mClipboard.renderIm();
		}

	}


	if(mCurMode == EMODE_TILESET){		
		
		if(mCurrentBrushPixelTileSet){
			mCurrentBrushPixelTileSet->getBrushSelection(cx, cy, mTileSet.EditPixelAreas);
		}

		mPalette.renderIm(100 + (mGlobalSettings.CurrentEditor->mTileSet.mCurEdScale * mGlobalSettings.CurrentEditor->mTileSet.mSelectionAreaX),50+mGlobalSettings.TopBarHeight, &mGlobalSettings.mGlobalTexParam);			
		
		if(!mGlobalSettings.bShowPixelType) mColorSelectedTile->bPixelSelected = false;
		mTileSet.renderEd(mTileSetScrollX + 50,mTileSetScrollY + 50+mGlobalSettings.TopBarHeight);
		mColorSelectedTile->bPixelSelected = true;

		if(mActiveDialog){			
			mActiveDialog->render();
		}
		if(mActiveMessage){			
			mActiveMessage->render();
		}

		mTopBar.render();

		if(mGlobalSettings.bShowProjectInfo){			
			showProjectInfo();
		}

		if(mCurrentBrushPixelTileSet){
			mCurrentBrushPixelTileSet->renderSelection();
		}

		if(bShowBrushesPixelTileSet){
			mBrushesPixel.renderIm();
		} /*else {
			mBrushesPixel.closeEdit();
		}*/

		if(mTileSet.bShowClipboardTileSet){
			mTileSet.mClipboardTileSet.renderIm();
		}

		mTileSet.mSelection.renderSelection();	 
	}

	if(mCurMode == EMODE_SELEDIT){		
		

		if(mCurrentBrushPixelSelEdit){
			mCurrentBrushPixelSelEdit->getBrushSelection(cx, cy, mSelEdit.EditPixelAreas);
		}

		mPalette.renderIm(100 + (mGlobalSettings.CurrentEditor->mSelEdit.mCurEdScale * mGlobalSettings.CurrentEditor->mSelEdit.mSelectionAreaX),50+mGlobalSettings.TopBarHeight, &mGlobalSettings.mGlobalTexParam);			
		
		if(!mGlobalSettings.bShowPixelType) mColorSelectedTile->bPixelSelected = false;
		mSelEdit.renderEd(50 + mSelEditScrollX ,50 + mSelEditScrollY + mGlobalSettings.TopBarHeight);
		mColorSelectedTile->bPixelSelected = true;

		if(mActiveDialog){			
			mActiveDialog->render();
		}
		if(mActiveMessage){			
			mActiveMessage->render();
		}

		mTopBar.render();

		if(mGlobalSettings.bShowProjectInfo){			
			showProjectInfo();
		}

		if(mCurrentBrushPixelSelEdit){
			mCurrentBrushPixelSelEdit->renderSelection();
		}

		if(bShowBrushesPixelSelEdit){
			mBrushesPixelSelEd.renderIm();
		} /*else {
			mBrushesPixel.closeEdit();
		}*/



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

void TEditor::setSpriteBrushes(){

		if(mSprite->mTexParam.TexBPP < 8){
			mBrushesSprite = &mBrushesPixelSprite4;
		} else {
			mBrushesSprite = &mBrushesPixelSprite8;
		}

		mBrushesSprite->setBrushDeltas(mSprite->mTexParam.TexPixelSize, mSprite->mTexParam.TexPixelSize, &mSprite->mTexParam.TexEditScale, mSprite->mTexParam.TexEditScale, &mSprite->mTexParam);
		mBrushesSprite->bIsShown = &mSprite->bShowBrushesPixel;

		mSprite->mClipboard.setBrushDeltas(mSprite->mTexParam.TexPixelSize, mSprite->mTexParam.TexPixelSize, &mSprite->mTexParam.TexEditScale, mSprite->mTexParam.TexEditScale, &mSprite->mTexParam);
}

int TEditor::handleCopyPaste(bool cCutSelection){

	if(mCurMode == EMODE_SPRITE){

		mSprite->mCurrentBrushPixel = mSprite->mClipboard.createClipTile(mSprite->mFrame);

		if(cCutSelection && mSprite->mCurrentBrushPixel){
			if(mSprite->mFrame->mSelection.mSelected.size()){
				TEActionReplacePixels* newAction = new TEActionReplacePixels();
				newAction->doAction(mSprite->mFrame, mSprite->mFrame->mSelection.mSelected, -1, 0, &mPalette);
				
				if(!(newAction == mSprite->mActionStack.mLastAction)){
					mSprite->mActionStack.mLastAction = newAction;
					mSprite->mActionStack.newActionGroup();
					mSprite->mActionStack.addSubActions(newAction->mSubActions);
					mSprite->mActionStack.redoClearStack();
				}
			}
		}
	}

	if(mCurMode == EMODE_TILE){

		mCurrentBrushPixel = mTileSet.mClipboardTiles.createClipTile(mTileSelectedTile);

		if(cCutSelection && mCurrentBrushPixel){
			if(mTileSelectedTile->mSelection.mSelected.size()){
				TEActionReplacePixels* newAction = new TEActionReplacePixels();
				newAction->doAction(mTileSelectedTile, mTileSelectedTile->mSelection.mSelected, -1, 0, &mPalette);
				if(!(newAction == mTileSelectedTile->mActionStack.mLastAction)){
					mTileSelectedTile->mActionStack.mLastAction = newAction;
					mTileSelectedTile->mActionStack.newActionGroup();
					mTileSelectedTile->mActionStack.addSubActions(newAction->mSubActions);
					mTileSelectedTile->mActionStack.redoClearStack();
				}
			}
		}
	}

	if(mCurMode == EMODE_TILESET){

		mCurrentBrushPixelTileSet = mTileSet.mClipboardTileSet.createClipTileSet(&mTileSet);

		if(cCutSelection && mCurrentBrushPixelTileSet){
			if(mTileSet.mSelection.mSelected.size()){
				TEActionReplacePixelsSel* newAction = new TEActionReplacePixelsSel();
				newAction->doAction(&mTileSet, mTileSet.mSelection, mTileSet.mSelectionAreaX, mTileSet.mSelectionAreaY, 0, &mPalette);
				if(!(newAction == mTileSet.mActionStack.mLastAction)){
					mTileSet.mActionStack.mLastAction = newAction;
					mTileSet.mActionStack.newActionGroup();
					mTileSet.mActionStack.addSubActions(newAction->mSubActions);
					mTileSet.mActionStack.redoClearStack();
				}
			}
		}
	}

	if(mCurMode == EMODE_MAP){

		mCurrentBrushTile = mClipboardMap.createClipMap(mTileMap);

		if(cCutSelection && mCurrentBrushTile){
			if(mTileMap->mSelection.mSelected.size()){			
				TEActionReplaceTiles* newAction = new TEActionReplaceTiles();
				newAction->doAction(mTileMap, mTileMap->mSelection.mSelected, -1, 0);
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

int TEditor::handleClipboardPaste(bool cCycle){

	if(mCurMode == EMODE_SPRITE){
		if(cCycle){
			mSprite->mCurrentBrushPixel = mSprite->mClipboard.getPrevClip();
		} else {
			mSprite->mCurrentBrushPixel = mSprite->mClipboard.getLastClip();
		}
	}


	if(mCurMode == EMODE_TILE){
		if(cCycle){
			mCurrentBrushPixel = mTileSet.mClipboardTiles.getPrevClip();
		} else {
			mCurrentBrushPixel = mTileSet.mClipboardTiles.getLastClip();
		}
	}


	if(mCurMode == EMODE_TILESET){
		if(cCycle){
			mCurrentBrushPixelTileSet = mTileSet.mClipboardTileSet.getPrevClip();
		} else {
			mCurrentBrushPixelTileSet = mTileSet.mClipboardTileSet.getLastClip();
		}
	}


	if(mCurMode == EMODE_MAP){
		if(cCycle){
			mCurrentBrushTile = mClipboardMap.getPrevClip();
		} else {
			mCurrentBrushTile = mClipboardMap.getLastClip();
		}
	}

	return 0;
}


int TEditor::setMode(int newMode){

	cancelActiveDialog();

	if(newMode == EMODE_PALED){
		activatePaletteEdit();
	}

	if(newMode == EMODE_TILE){
		mBrushesPixel.setBrushDeltas(mGlobalSettings.mGlobalTexParam.TexPixelSize, mGlobalSettings.mGlobalTexParam.TexPixelSize, &mGlobalSettings.mGlobalTexParam.TexEditScale, mGlobalSettings.mGlobalTexParam.TexEditScale, &mGlobalSettings.mGlobalTexParam);
		mBrushesPixel.bIsShown = &bShowBrushesPixel;
		mTileSet.mClipboardTiles.setBrushDeltas(mGlobalSettings.mGlobalTexParam.TexPixelSize, mGlobalSettings.mGlobalTexParam.TexPixelSize, &mGlobalSettings.mGlobalTexParam.TexEditScale, mGlobalSettings.mGlobalTexParam.TexEditScale, &mGlobalSettings.mGlobalTexParam);
	}

	if(newMode == EMODE_TILESET){
		mBrushesPixel.setBrushDeltas(1, 1, &mTileSet.mCurEdScale, mGlobalSettings.mGlobalTexParam.TexEditScale * mGlobalSettings.mGlobalTexParam.TexPixelSize, &mGlobalSettings.mGlobalTexParam);
		mTileSet.mClipboardTileSet.setBrushDeltas(1, 1, &mTileSet.mCurEdScale, mGlobalSettings.mGlobalTexParam.TexEditScale * mGlobalSettings.mGlobalTexParam.TexPixelSize, &mGlobalSettings.mGlobalTexParam);
		mBrushesPixel.bIsShown = &bShowBrushesPixelTileSet;		
	}

	if(newMode == EMODE_SPRITE){
		setSpriteBrushes();		
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
			}        		        	

			mBrushesPixelSelEd.setBrushDeltas(1, 1, &mSelEdit.mCurEdScale, mGlobalSettings.mGlobalTexParam.TexEditScale * mGlobalSettings.mGlobalTexParam.TexPixelSize, &mGlobalSettings.mGlobalTexParam);
			mBrushesPixelSelEd.bIsShown = &bShowBrushesPixelSelEdit;

    	} else {        	
			showMessage("Selection is invalid\nMust be Rectangle of Min 2x2 and Max 16x16 Tiles");
			bTileMapWasChanged = true;
			return 1;
    	}
	}

	mPalette.bUpdateWinPos = true;

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

	if(mCurMode == EMODE_TILE){
		if(!mGlobalSettings.mio->WantCaptureMouse){
			if(amount > 0){
				mGlobalSettings.mGlobalTexParam.TexPixelSize++;
				if(mGlobalSettings.mGlobalTexParam.TexPixelSize > Tile::MaxScale){
					mGlobalSettings.mGlobalTexParam.TexPixelSize = Tile::MaxScale;
				}
			}
			if(amount < 0){
				mGlobalSettings.mGlobalTexParam.TexPixelSize--;
				if(mGlobalSettings.mGlobalTexParam.TexPixelSize < Tile::MinScale){
					mGlobalSettings.mGlobalTexParam.TexPixelSize = Tile::MinScale;
				}
			}
			mBrushesPixel.setBrushDeltas(mGlobalSettings.mGlobalTexParam.TexPixelSize, mGlobalSettings.mGlobalTexParam.TexPixelSize, &mGlobalSettings.mGlobalTexParam.TexEditScale, mGlobalSettings.mGlobalTexParam.TexEditScale, &mGlobalSettings.mGlobalTexParam);
			mTileSet.mClipboardTiles.setBrushDeltas(mGlobalSettings.mGlobalTexParam.TexPixelSize, mGlobalSettings.mGlobalTexParam.TexPixelSize, &mGlobalSettings.mGlobalTexParam.TexEditScale, mGlobalSettings.mGlobalTexParam.TexEditScale, &mGlobalSettings.mGlobalTexParam);
		}
	}

	if(mCurMode == EMODE_SPRITE){ 		
		if(!mGlobalSettings.mio->WantCaptureMouse && bLShiftIsDown){
			if(amount > 0){
				mSprite->selectNext();
			}
			if(amount < 0){
				mSprite->selectPrev();
			}
		} else if(!mGlobalSettings.mio->WantCaptureMouse && !bLShiftIsDown){
			if(amount > 0){
				mSprite->mTexParam.TexPixelSize++;
				if(mSprite->mTexParam.TexPixelSize > TSprite::MaxScale){
					mSprite->mTexParam.TexPixelSize = TSprite::MaxScale;
				}
			}
			if(amount < 0){
				mSprite->mTexParam.TexPixelSize--;
				if(mSprite->mTexParam.TexPixelSize < TSprite::MinScale){
					mSprite->mTexParam.TexPixelSize = TSprite::MinScale;
				}
			}
			setSpriteBrushes();
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
	if(mCurMode == EMODE_SPRITE){
		TEActionGroup *mGroup;
		bool bFrameSwitch = false;
		if(mSprite->mActionStack.mUndoStack.size()){
			mGroup = *(mSprite->mActionStack.mUndoStack.end()-1);
			if(mGroup->mActions.size()){
				if(mGroup->mActions[0]->checkFrame()){
					mSprite->selectFrame(mGroup->mActions[0]->getFrame());
					bFrameSwitch = true;
				}
			}
		}
		
		if(!bFrameSwitch){
			mSprite->mActionStack.undoLastActionGroup();
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
	if(mCurMode == EMODE_SPRITE){
		TEActionGroup *mGroup;
		bool bFrameSwitch = false;
		if(mSprite->mActionStack.mRedoStack.size()){
			mGroup = *(mSprite->mActionStack.mRedoStack.end()-1);
			if(mGroup->mActions.size()){
				if(mGroup->mActions[0]->checkFrame()){
					mSprite->selectFrame(mGroup->mActions[0]->getFrame());
					bFrameSwitch = true;
				}
			}
		}
		
		if(!bFrameSwitch){
			mSprite->mActionStack.redoLastActionGroup();
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

int TEditor::removeSelectedTile(){
	mActionStack.redoClearStack();	
	mActionStack.undoClearStack();
	mTileSet.mActionStack.redoClearStack();
	mTileSet.mActionStack.undoClearStack();

	mTileSelectedTile->bIsSelected = false;
				
	dropUnusedTile(mMapSelectedTile);

	if(mMapSelectedTile > 0){
		mMapSelectedTile--;
	}

	mTileSelectedTile = mTileSet.TTiles[mMapSelectedTile];

	mTileSelectedTile->bIsSelected = true;				
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
			showMessage("Selected Tile is used in TileMap(s)   \n  Replace Tile in TileMap(s) before removal   ");
		} else {
			if(mGlobalSettings.bTileSetWarnBeforeDelete){
				activateDTDialog(EDIALOG_TILEDELETE);
			} else {
				removeSelectedTile();
			}

		}
	}
	return 0;
}

int TEditor::activateDropUnusedTiles(){
	if(mCurMode == EMODE_MAP){
		activateDTDialog(EDIALOG_TILEDELETEALL);
	}
	return 0;
}

int TEditor::activateRemoveFrame(){
	if(mCurMode == EMODE_SPRITE){
		if(mGlobalSettings.bSpriteWarnBeforeDelete){
			activateDTDialog(EDIALOG_FRAMEDELETE);
		} else {
			removeSelectedFrame();
		}
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
		mPalette.setEditColor();	
		if(mColorSelectedTileEdit){
			mColorSelectedTileEdit->bPixelSelectedEdit = false;
		}
		mColorSelectedTileEdit = mColorSelectedTile;	
		mColorSelectedTileEdit->bPixelSelectedEdit = true;
		mColorSelectedEdit = mColorSelected;

		mGlobalSettings.CurrentEditor->mPalette.bUpdateEditColor = false;
	}	

	if(mColorSelectedEdit == 0){
		if(mColorSelectedTileEdit){
			mColorSelectedTileEdit->bPixelSelectedEdit = false;
		}
		mColorSelectedTileEdit = mPalette.TPixels[1];	
		mColorSelectedTileEdit->bPixelSelectedEdit = true;

		mColorSelectedEdit++;
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

	for(int i = 0; i < mSprites.size(); i++){		
		for(int j = 0; j < mSprites[i]->mFrames.size(); j++){		
			mSprites[i]->mFrames[j]->updateTexture(&mPalette);	
		}	
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

TSFrame* TEditor::createNewFrame(){
	if(mCurMode == EMODE_SPRITE){	
	TSFrame* newFrame = mSprite->createNew(&mPalette);
		if(newFrame){
				TEActionAddFrame* newActionFrame = new TEActionAddFrame();				
				newActionFrame->doAction(newFrame, this, mSprite);
      			mSprite->mActionStack.newActionGroup();	
      			mSprite->mActionStack.addAction(newActionFrame);
      			
				mSprite->mActionStack.mLastAction = newActionFrame;
       			mSprite->mActionStack.redoClearStack();
				
				return newFrame;
		}
	}
	return NULL;
}

int TEditor::createSpriteRotations(int cNum, float cInterval){

		if(mCurMode == EMODE_SPRITE){	
		TSFrame* orgFrame = mSprite->mFrame;
		std::vector<TSFrame*> cNewFrames;
	
		for(int i = 0; i < cNum; i++){
			TSFrame *newFrame = mSprite->createNewCopy(orgFrame, &mPalette); 

			newFrame->rotate(cInterval*(i+1));

			cNewFrames.push_back(newFrame);			
		}

		mSprite->mActionStack.newActionGroup();
				
		for(auto cFrame : cNewFrames){
			TEActionAddFrames* newActionTile = new TEActionAddFrames();
			newActionTile->doAction(cFrame, this, mSprite);	       			
	    	mSprite->mActionStack.addAction(newActionTile);
	    	mSprite->mActionStack.mLastAction = newActionTile;	       			
		}

		mSprite->mActionStack.redoClearStack();
	}

	return 0;
}

int TEditor::createSpriteRotationRange(int cRange, int cIntervals){

	if(mCurMode == EMODE_SPRITE){	
		TSFrame* orgFrame = mSprite->mFrame;
		std::vector<TSFrame*> cNewFrames;
	
		int nIntervals = 1;

		for(int ri = 0; ri < cIntervals; ri++){
			nIntervals += nIntervals+1;
		}

		nIntervals++;
		float nAngle = (float)cRange / (float)nIntervals;

		for(int i = 0; i < nIntervals; i++){
			TSFrame *newFrame = mSprite->createNewCopy(orgFrame, &mPalette); 

			newFrame->rotate(nAngle*(i+1));

			cNewFrames.push_back(newFrame);			
		}

		mSprite->mActionStack.newActionGroup();
				
		for(auto cFrame : cNewFrames){
			TEActionAddFrames* newActionTile = new TEActionAddFrames();
			newActionTile->doAction(cFrame, this, mSprite);	       			
	    	mSprite->mActionStack.addAction(newActionTile);
	    	mSprite->mActionStack.mLastAction = newActionTile;	       			
		}

		mSprite->mActionStack.redoClearStack();
	}


	return 0;
}


TSFrame* TEditor::createNewFrameCopy(TSFrame* cCopyFrame){
if(mCurMode == EMODE_SPRITE){	
	TSFrame* newFrame = mSprite->createNewCopy(cCopyFrame, &mPalette); //mSprite->mFrame, &mPalette);
		if(newFrame){
				TEActionAddFrame* newActionFrame = new TEActionAddFrame();				
				newActionFrame->doAction(newFrame, this, mSprite);
      			mSprite->mActionStack.newActionGroup();	
      			mSprite->mActionStack.addAction(newActionFrame);
      			
				mSprite->mActionStack.mLastAction = newActionFrame;
       			mSprite->mActionStack.redoClearStack();
				
				return newFrame;
		}
	}
	return NULL;

}
int TEditor::removeSelectedFrame(){
	int preFrame = mSprite->mSelectedFrame-1;
	if(preFrame < 0) preFrame = 0;
	mSprite->removeFrame(mSprite->mSelectedFrame);
	mSprite->selectFrame(preFrame);

	mSprite->mActionStack.redoClearStack();
	mSprite->mActionStack.undoClearStack();

	return 0;
}

int TEditor::createNewSpriteDownscaledCopy(TSprite *newSprite){
	mSprite = new TSprite(newSprite->mTexParam.TexSizeX / mGlobalSettings.mNewSpriteDownscale, newSprite->mTexParam.TexSizeY / mGlobalSettings.mNewSpriteDownscale, newSprite->mTexParam.TexBPP);

	std::cout << "Downscaled Sprite by X: " << mGlobalSettings.mNewSpriteDownscale << std::endl;

	for(auto *cFrame : newSprite->mFrames){
		mSprite->createNewUpscaledCopy(cFrame, &mPalette);		
	}

	mSprites.push_back(mSprite);
	
	switchSprite(mSprites.size()-1, 0);

	return 0;
}


int TEditor::createNewSpriteUpscaledCopy(TSprite *newSprite){
	mSprite = new TSprite(newSprite->mTexParam.TexSizeX * mGlobalSettings.mNewSpriteUpscale, newSprite->mTexParam.TexSizeY * mGlobalSettings.mNewSpriteUpscale, newSprite->mTexParam.TexBPP);

	std::cout << "Upscaled Sprite by X: " << mGlobalSettings.mNewSpriteUpscale << std::endl;

	for(auto *cFrame : newSprite->mFrames){
		mSprite->createNewUpscaledCopy(cFrame, &mPalette);		
	}

	mSprites.push_back(mSprite);
	
	switchSprite(mSprites.size()-1, 0);

	return 0;
}

int TEditor::createNewSpriteScaledCopy(TSprite *newSprite){
	mSprite = new TSprite(newSprite->mTexParam.TexSizeX,newSprite->mTexParam.TexSizeY,newSprite->mTexParam.TexBPP);

	for(auto *cFrame : newSprite->mFrames){
		mSprite->createNewFromBuffer(cFrame->FileData, &mPalette);
		mSprite->mFrames[mSprite->mFrames.size()-1]->scale(mGlobalSettings.mNewSpriteScale);
	}

	mSprites.push_back(mSprite);
	
	switchSprite(mSprites.size()-1, 0);

	return 0;
}

int TEditor::createNewSpriteCopy(TSprite *newSprite){
	mSprite = new TSprite(newSprite->mTexParam.TexSizeX,newSprite->mTexParam.TexSizeY,newSprite->mTexParam.TexBPP);

	for(auto *cFrame : newSprite->mFrames){
		mSprite->createNewFromBuffer(cFrame->FileData, &mPalette);		
	}

	mSprites.push_back(mSprite);
	
	switchSprite(mSprites.size()-1, 0);

	return 0;
}

int TEditor::createNewSpriteConvertBPP8(TSprite *copySprite){
	mSprite = new TSprite(copySprite->mTexParam.TexSizeX, copySprite->mTexParam.TexSizeY, 8);

	for(auto *cFrame : copySprite->mFrames){
		TSFrame *newFrame = mSprite->createFrame(&mPalette);

		for(int pi = 0; pi < mSprite->mTexParam.TexSizeX * mSprite->mTexParam.TexSizeY; pi++){
			newFrame->FileData[pi] = cFrame->getPixel(pi);
		}
		newFrame->updateTexture(&mPalette);
	}

	mSprites.push_back(mSprite);
	
	switchSprite(mSprites.size()-1, 0);

	return 0;
}

int TEditor::createNewSpriteConvertBPP4(TSprite *copySprite){
	mSprite = new TSprite(copySprite->mTexParam.TexSizeX, copySprite->mTexParam.TexSizeY, 4);

	for(auto *cFrame : copySprite->mFrames){
		TSFrame *newFrame = mSprite->createFrame(&mPalette);

		for(int pi = 0; pi < mSprite->mTexParam.TexSizeX * mSprite->mTexParam.TexSizeY; pi++){
			newFrame->setPixel(pi,cFrame->FileData[pi] % 16);
		}
		newFrame->updateTexture(&mPalette);
	}

	mSprites.push_back(mSprite);
	
	switchSprite(mSprites.size()-1, 0);

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
			
		} else {			
			TEActionSwapTiles* newAction = new TEActionSwapTiles();				
			newAction->doAction(this, &mTileSet, source, target, mGlobalSettings.bTileSetOrderUpdateTileMap);
      		mActionStack.newActionGroup();	
      		mActionStack.addAction(newAction);
			mActionStack.mLastAction = newAction;
			mActionStack.redoClearStack();						
		}
		return 0;
	}
	return 1;
}

int TEditor::swapFrames(int source, int target, bool bDoCopy){
if(mCurMode == EMODE_SPRITE){
		if(bLCTRLisDown && bDoCopy){
			TEActionReplaceFrame* newAction = new TEActionReplaceFrame();				
			newAction->doAction(mSprite->mFrames[target], mSprite->mFrames[source]->FileData, this, mSprite);
      		mSprite->mActionStack.newActionGroup();	
      		mSprite->mActionStack.addAction(newAction);

			mSprite->mActionStack.mLastAction = newAction;
       		mSprite->mActionStack.redoClearStack();
			
		} else {			
			TEActionSwapFrames* newAction = new TEActionSwapFrames();				
			newAction->doAction(this, mSprite, source, target);
      		mSprite->mActionStack.newActionGroup();	
      		mSprite->mActionStack.addAction(newAction);
			mSprite->mActionStack.mLastAction = newAction;
			mSprite-> mActionStack.redoClearStack();						
		}
		return 0;
	}
	return 1;
}

int TEditor::moveFrameUp(){
	if(mCurMode == EMODE_SPRITE){		
		if(mSprite->mSelectedFrame > 0){				
			swapFrames(mSprite->mSelectedFrame, mSprite->mSelectedFrame-1);				
			return 0;
		}		
	}
	return 1;
}

int TEditor::moveFrameDown(){
	if(mCurMode == EMODE_SPRITE){		
		if(mSprite->mSelectedFrame < (mSprite->mFrames.size()-1)){				
			swapFrames(mSprite->mSelectedFrame, mSprite->mSelectedFrame+1);
			return 0;
		}
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

int TEditor::filterFrame(){
	if(mCurMode == EMODE_SPRITE){		
		TSFrame* newFrame = new TSFrame(&mSprite->mTexParam); 
		newFrame->loadFromBuffer(mSprite->mFrame->FileData, &mPalette);
		newFrame->applyFilter();
		TEActionReplaceFrame* newAction = new TEActionReplaceFrame();				
		newAction->doAction(mSprite->mFrame, newFrame->FileData, this, mSprite);
      	mSprite->mActionStack.newActionGroup();	
      	mSprite->mActionStack.addAction(newAction);

		mSprite->mActionStack.mLastAction = newAction;
       	mSprite->mActionStack.redoClearStack();
		return 0;		
	}
	return 1;
}

int TEditor::rotateFrameRight(){
if(mCurMode == EMODE_SPRITE){		
		TSFrame* newFrame = new TSFrame(&mSprite->mTexParam); 
		newFrame->loadFromBuffer(mSprite->mFrame->FileData, &mPalette);
		newFrame->rotater();		
		TEActionReplaceFrame* newAction = new TEActionReplaceFrame();				
		newAction->doAction(mSprite->mFrame, newFrame->FileData, this, mSprite);
      	mSprite->mActionStack.newActionGroup();	
      	mSprite->mActionStack.addAction(newAction);

		mSprite->mActionStack.mLastAction = newAction;
       	mSprite->mActionStack.redoClearStack();
		return 0;		
	}
	return 1;
}

int TEditor::rotateFrameLeft(){
if(mCurMode == EMODE_SPRITE){		
		TSFrame* newFrame = new TSFrame(&mSprite->mTexParam); 
		newFrame->loadFromBuffer(mSprite->mFrame->FileData, &mPalette);
		newFrame->rotatel();
		TEActionReplaceFrame* newAction = new TEActionReplaceFrame();				
		newAction->doAction(mSprite->mFrame, newFrame->FileData, this, mSprite);
      	mSprite->mActionStack.newActionGroup();	
      	mSprite->mActionStack.addAction(newAction);

		mSprite->mActionStack.mLastAction = newAction;
       	mSprite->mActionStack.redoClearStack();
		return 0;		
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

TSFrame* TEditor::createNewFrameFromFile(std::string newFramePath){
	return mSprite->createNewFromFile(newFramePath, &mPalette);
}

Tile* TEditor::createNewTileFromFile(std::string newTilePath){
	return mTileSet.createNewFromFile(newTilePath, &mPalette);
}

int TEditor::showMessage(std::string cMessage, bool isError){
	if(isError){		
		mGlobalSettings.mProgramMessage = cMessage;
		mProgramInfo->setCondition(1);
		mProgramInfo->update();
		mActiveMessage = mProgramInfo;
	} else {		
		mGlobalSettings.mProgramMessage = cMessage;
		mProgramInfo->setCondition(-1);
		mProgramInfo->update();
		mActiveMessage = mProgramInfo;
	}
	return 0;
}

bool TEditor::checkQuit(){

	if(mGlobalSettings.mio->WantCaptureKeyboard) return true;

	if(mTopBar.bIsMenuOpen) return true;

	if(mCurMode == EMODE_MAP){
		if(bShowCollisionEditor){
			bShowCollisionEditor = false;
			return true;
		}

		if(bShowBrushesTile){
			bShowBrushesTile = false;
			mBrushesTile.closeEdit();
			return true;
		}

		if(bShowClipboardMap){
			bShowClipboardMap = false;
			mClipboardMap.closeEdit();
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
			mBrushesPixel.closeEdit(); 
			return true;
		}
		if(mTileSet.bShowClipboardTiles){
			mTileSet.bShowClipboardTiles = false;
			mTileSet.mClipboardTiles.closeEdit();
			return true;
		}
		if(mCurrentBrushPixel){
			mCurrentBrushPixel = NULL;
			return true;
		}
	}

	if(mCurMode == EMODE_SPRITE){
		if(mSprite->bShowBrushesPixel){
			mSprite->bShowBrushesPixel = false;
			mBrushesSprite->closeEdit();
			return true;
		}
		if(mSprite->bShowClipboard){
			mSprite->bShowClipboard = false;
			mSprite->mClipboard.closeEdit();
			return true;
		}

		if(mSprite->mCurrentBrushPixel){
			mSprite->mCurrentBrushPixel = NULL;
			return true;
		}
	}

	if(mCurMode == EMODE_TILESET){
        if(bShowBrushesPixelTileSet){
            bShowBrushesPixelTileSet = false;
			mBrushesPixel.closeEdit(); 
            return true;
        }
		if(mTileSet.bShowClipboardTileSet){
			mTileSet.bShowClipboardTileSet = false;
			mTileSet.mClipboardTileSet.closeEdit();
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
			mBrushesPixelSelEd.closeEdit(); 
            return true;
        }
        if(mCurrentBrushPixelSelEdit){
            mCurrentBrushPixelSelEdit = NULL;
            return true;
        }
    }

	if(mCurMode == EMODE_PALED){
		if(mPalette.bIsPickerOpen) return true;	
	}

	return false;
}

int TEditor::activateHelpDialog(){
	mGlobalSettings.bShowHelpDialog = true;
	mActiveDialog = &mHelpDialog;
	
	return 0;
}

void TEditor::activateSaveDialog(){
	if(fs::exists(fs::status(mGlobalSettings.ProjectPath))){
		activateDTDialog(EDIALOG_PROJECTSAVE);
	} else {
		stateProjectSave();
	}
}

int TEditor::activateOpenSpriteDialog(){
		mActiveDialog = &mOpenSpriteDialog;
		mActiveDialog->bDialogIsWatingForText = true;		
		return 0;
}

/*
int TEditor::activateScaleFrameDialog(){
	if(mCurMode == EMODE_SPRITE){
		mActiveDialog = &mScaleFrame;		
	}
	return 0;
}

int TEditor::activateRotateFrameDialog(){
	if(mCurMode == EMODE_SPRITE){
		mActiveDialog = &mRotateFrame;		
	}
	return 0;
}
*/

/*
int TEditor::activateOpenTileDialog(){
	if(mCurMode == EMODE_MAP){
		mActiveDialog = &mOpenTileDialog;
		mActiveDialog->bDialogIsWatingForText = true;		
	}
	return 0;
}
*/

int TEditor::activateRemoveSpriteDialog(){

	if(mCurMode == EMODE_SPRITE){		
		activateDTDialog(EDIALOG_SPRITEDELETE);
	}
	return 0;
}

int TEditor::activateRemoveTileMapDialog(){
	if(mCurMode == EMODE_MAP){
		if(mTileMaps.size() > 1){
			activateDTDialog(EDIALOG_TILEMAPDELETE);
		}
	}
	return 0;
}

/*
int TEditor::activateNewTileMapDialog(){
	if(mCurMode == EMODE_MAP){
		mActiveDialog = &mNewTileMapDialog;		
	}
	return 0;
}
*/

int TEditor::activateNewSpriteDialog(){
	//if(mCurMode == EMODE_MAP){
		mActiveDialog = &mNewSpriteDialog;		
	//}
	return 0;
}

int TEditor::removeColMapDialog(){
	if(mCurMode == EMODE_MAP){
		activateDTDialog(EDIALOG_COLMAPREMOVE);
	}
	return 0;
}

int TEditor::activateColMapDialog(bool bCreateColMap){
	if(bCreateColMap){		
		mTileMap->createCollisionMap();
	}
		
	mColMapEdit.startEdit(mTileMap);
	bShowCollisionEditor = true;

	return 0;
}

int TEditor::activateClipboard(bool bChangeState){
	if(mCurMode == EMODE_SPRITE){
		if(bChangeState) mSprite->bShowClipboard = !mSprite->bShowClipboard;

		if(mSprite->bShowClipboard){
			mSprite->bShowBrushesPixel = false;
			mBrushesSprite->closeEdit();
		} else {
			mSprite->mClipboard.closeEdit();
		}

	}

	if(mCurMode == EMODE_TILE){
		if(bChangeState) mTileSet.bShowClipboardTiles = !mTileSet.bShowClipboardTiles;

		if(mTileSet.bShowClipboardTiles){
			bShowBrushesPixel = false;
			mBrushesPixel.closeEdit();
		} else {
			mTileSet.mClipboardTiles.closeEdit();
		}
	}

	if(mCurMode == EMODE_TILESET){
		if(bChangeState) mTileSet.bShowClipboardTileSet = !mTileSet.bShowClipboardTileSet;

		if(mTileSet.bShowClipboardTileSet){
			bShowBrushesPixelTileSet = false;
			mBrushesPixel.closeEdit();
		} else {
			mTileSet.mClipboardTileSet.closeEdit();
		}
	}

	if(mCurMode == EMODE_MAP){
		if(bChangeState) bShowClipboardMap = !bShowClipboardMap;

		if(bShowClipboardMap){
			bShowBrushesTile = false;
			mBrushesTile.closeEdit();
		} else {
			mClipboardMap.closeEdit();
		}
	}

	return 0;
}

void TEditor::nextBrushPos(){
	if(mCurMode != EMODE_PALED){
		mGlobalSettings.mBrushPosition++;
		if(mGlobalSettings.mBrushPosition > 4){
			mGlobalSettings.mBrushPosition = 0;
		}
	}
}

int TEditor::activateBrushes(bool bChangeState){
	
	if(mCurMode == EMODE_MAP){
		if(bChangeState) bShowBrushesTile = !bShowBrushesTile;

		if(bShowBrushesTile){
			bShowClipboardMap = false;
			mClipboardMap.closeEdit();
		} else {
			mBrushesTile.closeEdit();
		}
	}

	if(mCurMode == EMODE_TILE){
		if(bChangeState) bShowBrushesPixel = !bShowBrushesPixel;

		if(bShowBrushesPixel){
			mTileSet.bShowClipboardTiles = false;
			mTileSet.mClipboardTiles.closeEdit();
		} else {
			mBrushesPixel.closeEdit();
		}		
	}

	if(mCurMode == EMODE_TILESET){
		if(bChangeState){
			bShowBrushesPixelTileSet = !bShowBrushesPixelTileSet;			
		}
		if(bShowBrushesPixelTileSet){
			mTileSet.bShowClipboardTileSet = false;			
			mTileSet.mClipboardTileSet.closeEdit();
		} else {
			mBrushesPixel.closeEdit();
		}		
	}

	if(mCurMode == EMODE_SPRITE){
		if(bChangeState) mSprite->bShowBrushesPixel = !mSprite->bShowBrushesPixel;

		if(mSprite->bShowBrushesPixel){
			mSprite->bShowClipboard = false;
			mSprite->mClipboard.closeEdit();
		} else {
			mBrushesSprite->closeEdit();
		}
	}

	if(mCurMode == EMODE_SELEDIT){
		if(bChangeState) bShowBrushesPixelSelEdit = !bShowBrushesPixelSelEdit;

		if(bShowBrushesPixelSelEdit){

		} else {
			mBrushesPixelSelEd.closeEdit();
		}
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
	if(mCurMode == EMODE_SPRITE){
		if(mSprite->mCurrentBrushPixel){
			mSprite->mCurrentBrushPixel = NULL;
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
	if(mCurMode == EMODE_SPRITE){		
		if(!mSprite->mCurrentBrushPixel){
				mSprite->mCurrentBrushPixel = mBrushesSprite->getBrush();
		} else {		
				mSprite->mCurrentBrushPixel = mBrushesSprite->getNextBrush();
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
            mCurrentBrushPixelSelEdit = mBrushesPixelSelEd.getBrush();
        } else {                
            mCurrentBrushPixelSelEdit = mBrushesPixelSelEd.getNextBrush();
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
				mTileSelectedTile->mSelection.selectRange(0, mGlobalSettings.mGlobalTexParam.TexSizeX *  mGlobalSettings.mGlobalTexParam.TexSizeY);
			break;
			case SELMODE_NONE:
				mTileSelectedTile->mSelection.clearSelection();
			break;
			case SELMODE_INVERT:
				mTileSelectedTile->mSelection.invertSelection(0,  mGlobalSettings.mGlobalTexParam.TexSizeX *  mGlobalSettings.mGlobalTexParam.TexSizeY);
			break;
			default:
			break;
		}
	}

	if(mCurMode == EMODE_SPRITE){
		switch (SELMODE)
		{
			case SELMODE_ALL:
				mSprite->mFrame->mSelection.selectRange(0, mSprite->mTexParam.TexSizeX *  mSprite->mTexParam.TexSizeY);
			break;
			case SELMODE_NONE:
				mSprite->mFrame->mSelection.clearSelection();
			break;
			case SELMODE_INVERT:
				mSprite->mFrame->mSelection.invertSelection(0,  mSprite->mTexParam.TexSizeX *  mSprite->mTexParam.TexSizeY);
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

			if(mGlobalSettings.mGlobalTexParam.TexBPP < 8){
				mOldColor = mColorSelected % 16;
			}

			int tSel = searchRectsXY(mPalette.PixelAreas, mx, my);
			if(tSel > -1){					
				if(mTileSelectedTile->mSelection.mSelected.size()){
					TEActionReplacePixels* newAction = new TEActionReplacePixels();
						newAction->doAction(mTileSelectedTile, mTileSelectedTile->mSelection.mSelected, -1, mGlobalSettings.mGlobalTexParam.TexBPP == 8 ? tSel : tSel % 16, &mPalette);
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
					for(int i = 0; i < (mGlobalSettings.mGlobalTexParam.TexSizeX *mGlobalSettings.mGlobalTexParam.TexSizeY); i++){
						auto cPixel = mTileSelectedTile->getPixel(i);
						if(cPixel == mOldColor){
							newSelection.push_back(cPixIndex);
						}
						cPixIndex++;
					}
					if(newSelection.size()){
						TEActionReplacePixels* newAction = new TEActionReplacePixels();
						newAction->doAction(mTileSelectedTile, newSelection, mOldColor, mGlobalSettings.mGlobalTexParam.TexBPP == 8 ? tSel : tSel % 16, &mPalette);
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

	if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_SPRITE){

		
		if(mGlobalSettings.bShowPixelTypeSprite || mSprite->mFrame->mSelection.mSelected.size()){
			int mOldColor = mColorSelected;
			int tSel = searchRectsXY(mPalette.PixelAreas, mx, my);
			
			if(mSprite->mTexParam.TexBPP < 8){
				mOldColor = mColorSelected % 16;
			}
			
			if(tSel > -1){									
				if(mSprite->mFrame->mSelection.mSelected.size()){
					TEActionReplacePixels* newAction = new TEActionReplacePixels();
						newAction->doAction(mSprite->mFrame, mSprite->mFrame->mSelection.mSelected, -1, mSprite->mTexParam.TexBPP == 8 ? tSel : tSel % 16, &mPalette);						
						if(!(newAction == mSprite->mActionStack.mLastAction)){
							mSprite->mActionStack.mLastAction = newAction;
							mSprite->mActionStack.newActionGroup();
							mSprite->mActionStack.addSubActions(newAction->mSubActions);
							mSprite->mActionStack.redoClearStack();
						}

				} else {
					std::vector<int> newSelection;
					int cPixIndex = 0;
					//for(auto &cPixel : mTileSelectedTile->FileData){
					for(int i = 0; i < (mSprite->mTexParam.TexSizeX *mSprite->mTexParam.TexSizeY); i++){
						auto cPixel = mSprite->mFrame->getPixel(i);
						
						if(cPixel == mOldColor){
							newSelection.push_back(cPixIndex);
						}
						
						cPixIndex++;
					}
					if(newSelection.size()){
						TEActionReplacePixels* newAction = new TEActionReplacePixels();
						newAction->doAction(mSprite->mFrame, newSelection, mOldColor, mSprite->mTexParam.TexBPP == 8 ? tSel : tSel % 16, &mPalette);						
						if(!(newAction == mSprite->mActionStack.mLastAction)){
							mSprite->mActionStack.mLastAction = newAction;
							mSprite->mActionStack.newActionGroup();
							mSprite->mActionStack.addSubActions(newAction->mSubActions);
							mSprite->mActionStack.redoClearStack();
						}
					}
				}
			} 
		}
		
	}


	if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_TILESET){
		if(mGlobalSettings.bShowPixelType || mTileSet.mSelection.mSelected.size()){
			int mOldColor = mColorSelected;

			if(mGlobalSettings.mGlobalTexParam.TexBPP < 8){
				mOldColor = mColorSelected % 16;
			}

			int tSel = searchRectsXY(mPalette.PixelAreas, mx, my);
			if(tSel > -1){					
				if(mTileSet.mSelection.mSelected.size()){
					TEActionReplacePixelsSel* newAction = new TEActionReplacePixelsSel();
					newAction->doAction(&mTileSet, mTileSet.mSelection, mTileSet.mSelectionAreaX, mTileSet.mSelectionAreaY, mGlobalSettings.mGlobalTexParam.TexBPP == 8 ? tSel : tSel % 16, &mPalette);
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
						
						for(int i = 0; i < (mGlobalSettings.mGlobalTexParam.TexSizeX *mGlobalSettings.mGlobalTexParam.TexSizeY); i++){
							auto cPixel = mTile->getPixel(i);
							if(cPixel == mOldColor){
								newSelection.push_back(cPixIndex);
							}
							cPixIndex++;
						}
						if(newSelection.size()){
							newAction = new TEActionReplacePixels();
							newAction->doAction(mTile, newSelection, mOldColor, mGlobalSettings.mGlobalTexParam.TexBPP == 8 ? tSel : tSel % 16, &mPalette);
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

	if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_SELEDIT){		
		if(mGlobalSettings.bShowPixelType){
			int mOldColor = mColorSelected;

			if(mGlobalSettings.mGlobalTexParam.TexBPP < 8){
				mOldColor = mColorSelected % 16;
			}

			int tSel = searchRectsXY(mPalette.PixelAreas, mx, my);
			if(tSel > -1){									
				mActionStack.newActionGroup();
					
				TEActionReplacePixels* newAction;
				bool bDidAction = false;

				std::vector mSelectedTiles = mSelEdit.mCurrentSelection->mSelected;

				std::sort( mSelectedTiles.begin(), mSelectedTiles.end() );
				mSelectedTiles.erase( std::unique( mSelectedTiles.begin(), mSelectedTiles.end() ), mSelectedTiles.end() );

				for(int tt = 0; tt < mSelectedTiles.size(); tt++){
					std::vector<int> newSelection;
					int cPixIndex = 0;
					Tile* mTile = mTileSet.TTiles[mTileMap->getTile(mSelectedTiles[tt])];
						
					for(int i = 0; i < (mGlobalSettings.mGlobalTexParam.TexSizeX *mGlobalSettings.mGlobalTexParam.TexSizeY); i++){
						auto cPixel = mTile->getPixel(i);
						if(cPixel == mOldColor){
							newSelection.push_back(cPixIndex);
						}
							cPixIndex++;
						}
						if(newSelection.size()){
							newAction = new TEActionReplacePixels();
							newAction->doAction(mTile, newSelection, mOldColor, mGlobalSettings.mGlobalTexParam.TexBPP == 8 ? tSel : tSel % 16, &mPalette);
							//if(!(newAction == mTileSet.mActionStack.mLastAction)){							
							mActionStack.addSubActions(newAction->mSubActions);
							bDidAction = true;
						
							//}
						}
				}
				if(bDidAction){
					mActionStack.mLastAction = newAction;
					mActionStack.redoClearStack();
				} else {
					mActionStack.dropLastGroup();
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

int TEditor::findSelSprite(){

	if(leftMouseButtonDown && bLShiftIsDown && !mGlobalSettings.mio->WantCaptureMouse){
		if(!mSprite->mFrame->mSelection.bIsSelecting){
			mSprite->mFrame->mSelection.startSelection(cx, cy);	
		} else {
			mSprite->mFrame->mSelection.updateSelection(cx, cy);
		}
		
	} else {		
		if(mSprite->mFrame->mSelection.bIsSelecting){
			mSprite->mFrame->mSelection.confirmSelection(mSprite->mFrame->PixelAreas, mSprite->mTexParam.TexPixelSize * mSprite->mTexParam.TexEditScale, mSprite->mTexParam.TexPixelSize * mSprite->mTexParam.TexEditScale);
		}
		
		if(leftMouseButtonDown && !bLShiftIsDown && !mGlobalSettings.mio->WantCaptureMouse){
			if(bLCTRLisDown){
				if(bSpriteGrapped){
					mSprite->mSpriteScrollX += rx;
					mSprite->mSpriteScrollY += ry;					
				} else {
					bSpriteGrapped = true;					
				}				
			} else {
				int tSel = -1;

				if(mSprite->mCurrentBrushPixel){
					TEActionBrushPixels* newAction = new TEActionBrushPixels();
					newAction->doAction(mSprite->mFrame, *mSprite->mCurrentBrushPixel, &mPalette);
					if(!(*newAction == *mSprite->mActionStack.mLastAction)){								
						mSprite->mActionStack.mLastAction = newAction;
						mSprite->mActionStack.newActionGroup();
						mSprite->mActionStack.addSubActions(newAction->mSubActions);
       					mSprite->mActionStack.redoClearStack();
					}
				} else {
					tSel = searchRectsXY(mSprite->mFrame->PixelAreas, cx, cy);
					if(tSel != -1){

						TEActionReplacePixel *mCurAction = new TEActionReplacePixel();
						mCurAction->doAction(mSprite->mFrame, tSel, mSprite->mFrame->getPixel(tSel), mColorSelected, &mPalette);
				
						if(!(*mCurAction == * mSprite->mActionStack.mLastAction)){
   							mSprite->mActionStack.newActionGroup();	
   							mSprite->mActionStack.addAction(mCurAction);
   							mSprite->mActionStack.mLastAction = mCurAction;
   							mSprite->mActionStack.redoClearStack();
   						}
					}
				}
			}
		}
	}

	/* */

	int spriteWidthX = (mSprite->mTexParam.TexSizeX * mSprite->mTexParam.TexEditScale * mSprite->mTexParam.TexPixelSize) +  mGlobalSettings.TopBarHeight;
	int spriteWidthY = (mSprite->mTexParam.TexSizeY * mSprite->mTexParam.TexEditScale * mSprite->mTexParam.TexPixelSize) +  mGlobalSettings.TopBarHeight;

	if(mSprite->mSpriteScrollX > 0){mSprite->mSpriteScrollX = 0;}
	if(mSprite->mSpriteScrollY > 0){mSprite->mSpriteScrollY = 0;}
	
	if(mSprite->mSpriteScrollX < -(spriteWidthX - (mGlobalSettings.WindowWidth - 50 - mSprite->SpriteWidth))){mSprite->mSpriteScrollX = -(spriteWidthX - (mGlobalSettings.WindowWidth -50 - mSprite->SpriteWidth));}
	if(mSprite->mSpriteScrollY < -(spriteWidthY - (mGlobalSettings.WindowHeight - 50- mGlobalSettings.TopBarHeight))){mSprite->mSpriteScrollY = -(spriteWidthY - (mGlobalSettings.WindowHeight -50 - mGlobalSettings.TopBarHeight));}
	
	if(spriteWidthX < (mGlobalSettings.WindowWidth - mSprite->SpriteWidth)){
		mSprite->mSpriteScrollX = 0;
	}
	
	if(spriteWidthY < (mGlobalSettings.WindowHeight-mGlobalSettings.TopBarHeight)){
		mSprite->mSpriteScrollY = 0;
	}

	/* */

	if((rightMouseButtonClicks && bLShiftIsDown) && !mGlobalSettings.mio->WantCaptureMouse){		
		int tSel = -1;
		tSel = searchRectsXY(mSprite->mFrame->PixelAreas, cx, cy);
		if(tSel >-1){
			mSprite->mFrame->mSelection.modifySelection(tSel);
		}		
	}

	if(rightMouseButtonDown && !mGlobalSettings.mio->WantCaptureMouse  && !bLShiftIsDown){
		int tSel = -1;
		tSel = searchRectsXY(mSprite->mFrame->PixelAreas, cx, cy);
		if(tSel >-1){
			mColorSelectedTile->bPixelSelected = false;
			mColorSelected = (mSprite->mFrame->getPixel(tSel)+(mSprite->mTexParam.PaletteOffset*16));
			mColorSelectedTile = mPalette.TPixels[mColorSelected];
			mColorSelectedTile->bPixelSelected = true;
			mSprite->mFrame->mSelection.cancelSelection();
			mGlobalSettings.CurrentEditor->mPalette.bUpdateEditColor = true;
		}
	}

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
			mTileSelectedTile->mSelection.confirmSelection(mTileSelectedTile->PixelAreas, mGlobalSettings.mGlobalTexParam.TexPixelSize * mGlobalSettings.mGlobalTexParam.TexEditScale, mGlobalSettings.mGlobalTexParam.TexPixelSize * mGlobalSettings.mGlobalTexParam.TexEditScale);
		}
		
		if(leftMouseButtonDown && !mGlobalSettings.mio->WantCaptureMouse){
			if(bLCTRLisDown){
				if(bTileGrapped){
					mTileScrollX += rx;
					mTileScrollY += ry;					
				} else {
					bTileGrapped = true;					
				}				
			} else {
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
	}
	
	/* */

	int tileSizeX = (mGlobalSettings.mGlobalTexParam.TexSizeX * mGlobalSettings.mGlobalTexParam.TexEditScale * mGlobalSettings.mGlobalTexParam.TexPixelSize) +  mGlobalSettings.TopBarHeight;
	int tileSizeY = (mGlobalSettings.mGlobalTexParam.TexSizeY * mGlobalSettings.mGlobalTexParam.TexEditScale * mGlobalSettings.mGlobalTexParam.TexPixelSize) +  mGlobalSettings.TopBarHeight;

	if(mTileScrollX > 0){mTileScrollX = 0;}
	if(mTileScrollY > 0){mTileScrollY = 0;}
	
	if(mTileScrollX < -(tileSizeX - (mGlobalSettings.WindowWidth - 50 ))){mTileScrollX = -(tileSizeX - (mGlobalSettings.WindowWidth -50));}
	if(mTileScrollY < -(tileSizeY - (mGlobalSettings.WindowHeight - 50 - mGlobalSettings.TopBarHeight))){mTileScrollY = -(tileSizeY - (mGlobalSettings.WindowHeight - 50 - mGlobalSettings.TopBarHeight));}
	
	if(tileSizeX < (mGlobalSettings.WindowWidth)){
		mTileScrollX = 0;
	}
	
	if(tileSizeY < (mGlobalSettings.WindowHeight - mGlobalSettings.TopBarHeight)){
		mTileScrollY = 0;
	}

	/* */


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
			mColorSelected = (mTileSelectedTile->getPixel(tSel)+(mGlobalSettings.mGlobalTexParam.PaletteOffset*16));
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
		if(tSel > 0){
			mColorSelectedTileEdit->bPixelSelectedEdit = false;
			mColorSelectedEdit = tSel;
			mColorSelectedTileEdit = mPalette.TPixels[tSel];
			mColorSelectedTileEdit->bPixelSelectedEdit = true;
			mPalette.mEditColor = mPalette.getIm4Color(mPalette.TPaletteEdit[mGlobalSettings.CurrentEditor->mColorSelectedEdit]);
			mPalette.setEditColor();
		}
	}

	if(ImButtonsPalette.mRight.bButtonIsDown){
		int tSel = -1;
		tSel = searchRectsXY(mPalette.PixelAreas, ImButtonsPalette.mRight.mMousePos.x, ImButtonsPalette.mRight.mMousePos.y);
		if(tSel > 0){			
			mPalette.mEditColor = mPalette.getIm4Color(mPalette.TPaletteEdit[tSel]);
			mPalette.setEditColor();	
		}
	}

	return 0;
}

int TEditor::handlePalette(){
	TextureParameters *mTexParam;

	if(mCurMode != EMODE_SPRITE){
		mTexParam = &mGlobalSettings.mGlobalTexParam;
	} else {
		mTexParam = &mSprite->mTexParam;
	}

	/*
	if(mPalette.bHasBackupColor){
		mPalette.bHasBackupColor = false;
		mColorSelected = mPalette.mBackupColor;
		mColorSelectedTile->bPixelSelected = false;			
		mColorSelectedTile = mPalette.TPixels[mColorSelected];
		mColorSelectedTile->bPixelSelected = true;			
		mGlobalSettings.CurrentEditor->mPalette.bUpdateEditColor = true;
	}
	*/

	if(mCurMode != EMODE_SPRITE){		
		/*if(mPalette.bHasBackupColor){
		mPalette.bHasBackupColor = false;
		mColorSelected = mPalette.mBackupColor;
		mColorSelectedTile->bPixelSelected = false;			
		mColorSelectedTile = mPalette.TPixels[mColorSelected];
		mColorSelectedTile->bPixelSelected = true;			
		mGlobalSettings.CurrentEditor->mPalette.bUpdateEditColor = true;*/
		if(mLastPixelOffset != mTexParam->PaletteOffset){
			int tmpSel = mColorSelected % 16;
			tmpSel += (mTexParam->PaletteOffset * 16);
			mColorSelectedTile->bPixelSelected = false;
			mColorSelected = tmpSel;
			mColorSelectedTile = mPalette.TPixels[tmpSel];
			mColorSelectedTile->bPixelSelected = true;
			mLastPixelOffset = mTexParam->PaletteOffset;
			mGlobalSettings.CurrentEditor->mPalette.bUpdateEditColor = true;
		}		
	} else {
		if((mTexParam->TexBPP < mGlobalSettings.mGlobalTexParam.TexBPP)){ //&& !mPalette.bHasBackupColor
			//mPalette.bHasBackupColor = true;
			//mPalette.mBackupColor = mColorSelected;
			int tmpSel = mColorSelected % 16;
			tmpSel += (mTexParam->PaletteOffset * 16);
			mColorSelectedTile->bPixelSelected = false;
			mColorSelected = tmpSel;
			mColorSelectedTile = mPalette.TPixels[tmpSel];
			mColorSelectedTile->bPixelSelected = true;			
			mGlobalSettings.CurrentEditor->mPalette.bUpdateEditColor = true;
		}
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

	if(mCurMode == EMODE_SELEDIT){
		if(ImButtonsPalette.mRight.bButtonIsDown && mBrushesPixelSelEd.bIsEditing){
			int tSel = -1;		
			tSel = searchRectsXY(mPalette.PixelAreas, cx, cy);
		
			if(mTexParam->TexBPP < 0x8){
				mBrushesPixelSelEd.addBrushElement(tSel == -1 ? -1 : tSel%16);
			} else {
				mBrushesPixelSelEd.addBrushElement(tSel);
			}
		
		}else if(ImButtonsPalette.mRight.bButtonIsDown){			
			replaceSelectedColor(ImButtonsPalette.mRight.mMousePos.x, ImButtonsPalette.mRight.mMousePos.y);		
		}
	} else if(mCurMode != EMODE_SPRITE){
		if(ImButtonsPalette.mRight.bButtonIsDown && mBrushesPixel.bIsEditing){
			int tSel = -1;		
			tSel = searchRectsXY(mPalette.PixelAreas, cx, cy);
		
			if(mTexParam->TexBPP < 0x8){
				mBrushesPixel.addBrushElement(tSel == -1 ? -1 : tSel%16);
			} else {
				mBrushesPixel.addBrushElement(tSel);
			}
		
		} else if((mCurMode == EMODE_TILE) && ImButtonsPalette.mRight.bButtonIsDown && mTileSet.mClipboardTiles.bIsEditing){
			int tSel = -1;		
			tSel = searchRectsXY(mPalette.PixelAreas, cx, cy);
		
			if(mTexParam->TexBPP < 0x8){
				mTileSet.mClipboardTiles.addBrushElement(tSel == -1 ? -1 : tSel%16);
			} else {
				mTileSet.mClipboardTiles.addBrushElement(tSel);
			}
		} else if((mCurMode == EMODE_TILESET) && ImButtonsPalette.mRight.bButtonIsDown && mTileSet.mClipboardTileSet.bIsEditing){
			int tSel = -1;		
			tSel = searchRectsXY(mPalette.PixelAreas, cx, cy);
		
			if(mTexParam->TexBPP < 0x8){
				mTileSet.mClipboardTileSet.addBrushElement(tSel == -1 ? -1 : tSel%16);
			} else {
				mTileSet.mClipboardTileSet.addBrushElement(tSel);
			}
		} else 	if(ImButtonsPalette.mRight.bButtonIsDown){
			replaceSelectedColor(ImButtonsPalette.mRight.mMousePos.x, ImButtonsPalette.mRight.mMousePos.y);		
		}
	} else {
		if(ImButtonsPalette.mRight.bButtonIsDown && mBrushesSprite->bIsEditing){
			int tSel = -1;		
			tSel = searchRectsXY(mPalette.PixelAreas, cx, cy);
		
			if(mTexParam->TexBPP < 0x8){
				mBrushesSprite->addBrushElement(tSel == -1 ? -1 : tSel%16);
			} else {
				mBrushesSprite->addBrushElement(tSel);
			}
		
		} else if((mCurMode == EMODE_SPRITE) && ImButtonsPalette.mRight.bButtonIsDown && mSprite->mClipboard.bIsEditing){
			int tSel = -1;		
			tSel = searchRectsXY(mPalette.PixelAreas, cx, cy);
		
			if(mTexParam->TexBPP < 0x8){
				mSprite->mClipboard.addBrushElement(tSel == -1 ? -1 : tSel%16);
			} else {
				mSprite->mClipboard.addBrushElement(tSel);
			}
		} else if(ImButtonsPalette.mRight.bButtonIsDown){			
			replaceSelectedColor(ImButtonsPalette.mRight.mMousePos.x, ImButtonsPalette.mRight.mMousePos.y);		
		}
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
					ttile = mSelEdit.mSelection.getTileIndex(tSel, mSelEdit.mSelectionAreaX, mSelEdit.mSelectionAreaY,tindex, &mGlobalSettings.mGlobalTexParam);				
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
			ttile = mSelEdit.mSelection.getTileIndex(tSel, mSelEdit.mSelectionAreaX, mSelEdit.mSelectionAreaY,tindex, &mGlobalSettings.mGlobalTexParam);				
			stile = mSelEdit.mCurrentSelection->mSelected[ttile];
			mtile = mTileMap->getTile(stile);

			if(mtile > -1){					
				Tile* cSelectedTile = mTileSet.TTiles[mtile];
				int findex = cSelectedTile->getFlipIndex(tindex, mTileMap->getFlip(stile));

				mGlobalSettings.mGlobalTexParam.PaletteOffset = mTileMap->getOffset(stile);

				mColorSelected = (cSelectedTile->getPixel(findex)+(mGlobalSettings.mGlobalTexParam.PaletteOffset*16));
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
				ttile = mTileSet.mSelection.getTileIndex(tSel, mTileSet.mSelectionAreaX, mTileSet.mSelectionAreaY,tindex, &mGlobalSettings.mGlobalTexParam);
			
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
			tTile = mTileSet.mSelection.getTileIndex(tSel, mTileSet.mSelectionAreaX, mTileSet.mSelectionAreaY, tindex, &mGlobalSettings.mGlobalTexParam);
			Tile *mTile = mTileSet.TTiles[tTile];
			mColorSelected = (mTile->getPixel(tindex)+(mGlobalSettings.mGlobalTexParam.PaletteOffset*16));
			mColorSelectedTile = mPalette.TPixels[mColorSelected];
			mColorSelectedTile->bPixelSelected = true;
			mTileSet.mSelection.cancelSelection();
			mGlobalSettings.CurrentEditor->mPalette.bUpdateEditColor = true;
		}
	}

	return 0;
}

int TEditor::handleClipboard(){

	if(mCurMode == EMODE_SPRITE){
		if(ImButtonsClipboard.mLeft.bButtonIsDown){ 
			if(ImButtonsClipboard.mLeft.mMousePos.y < mSprite->mClipboard.mBrushOffset) {return 0;}
			int tSel = -1;
			tSel = searchRectsXY(mSprite->mClipboard.BrushAreas, ImButtonsClipboard.mLeft.mMousePos.x, ImButtonsClipboard.mLeft.mMousePos.y);
			if(tSel != -1){
				mSprite->mClipboard.mSelectedBrush = tSel;
				mSprite->mCurrentBrushPixel = mSprite->mClipboard.mBrushes[tSel];
				
				if(mSprite->mClipboard.bIsEditing){
					tSel = searchRectsXY(mSprite->mClipboard.mBrushes[mSprite->mClipboard.mSelectedBrush]->BrushElementAreas, ImButtonsClipboard.mLeft.mMousePos.x, ImButtonsClipboard.mLeft.mMousePos.y);
					if(tSel > -1){
						mSprite->mClipboard.mBrushes[mSprite->mClipboard.mSelectedBrush]->mCursorPos = tSel;
					}
				}		

			}		
		}

		if(ImButtonsClipboard.mRight.bButtonIsDown){ 
			if(ImButtonsClipboard.mRight.mMousePos.y < mSprite->mClipboard.mBrushOffset) {return 0;}			
			
			if(mSprite->mClipboard.bIsEditing){
				int tSel = searchRectsXY(mSprite->mClipboard.mBrushes[mSprite->mClipboard.mSelectedBrush]->BrushElementAreas, ImButtonsClipboard.mRight.mMousePos.x, ImButtonsClipboard.mRight.mMousePos.y);
				if(tSel > -1){
					mSprite->mClipboard.mBrushes[mSprite->mClipboard.mSelectedBrush]->mCursorPos = tSel;
					mSprite->mClipboard.addBrushElement(mSprite->mClipboard.mLastElement);
				}
			}
		}
		
	}

	if(mCurMode == EMODE_TILE){
		if(ImButtonsClipboard.mLeft.bButtonIsDown){ 
			if(ImButtonsClipboard.mLeft.mMousePos.y < mTileSet.mClipboardTiles.mBrushOffset) {return 0;}
			int tSel = -1;
			tSel = searchRectsXY(mTileSet.mClipboardTiles.BrushAreas, ImButtonsClipboard.mLeft.mMousePos.x, ImButtonsClipboard.mLeft.mMousePos.y);
			if(tSel != -1){
				mTileSet.mClipboardTiles.mSelectedBrush = tSel;
				mCurrentBrushPixel = mTileSet.mClipboardTiles.mBrushes[tSel];

				if(mTileSet.mClipboardTiles.bIsEditing){
					tSel = searchRectsXY(mTileSet.mClipboardTiles.mBrushes[mTileSet.mClipboardTiles.mSelectedBrush]->BrushElementAreas, ImButtonsClipboard.mLeft.mMousePos.x, ImButtonsClipboard.mLeft.mMousePos.y);
					if(tSel > -1){
						mTileSet.mClipboardTiles.mBrushes[mTileSet.mClipboardTiles.mSelectedBrush]->mCursorPos = tSel;
					}
				}
			}		
		}

		if(ImButtonsClipboard.mRight.bButtonIsDown){ 
			if(ImButtonsClipboard.mRight.mMousePos.y < mTileSet.mClipboardTiles.mBrushOffset) {return 0;}			
			
			if(mTileSet.mClipboardTiles.bIsEditing){
				int tSel = searchRectsXY(mTileSet.mClipboardTiles.mBrushes[mTileSet.mClipboardTiles.mSelectedBrush]->BrushElementAreas, ImButtonsClipboard.mRight.mMousePos.x, ImButtonsClipboard.mRight.mMousePos.y);
				if(tSel > -1){
					mTileSet.mClipboardTiles.mBrushes[mTileSet.mClipboardTiles.mSelectedBrush]->mCursorPos = tSel;
					mTileSet.mClipboardTiles.addBrushElement(mTileSet.mClipboardTiles.mLastElement);
				}
			}
		}
		
	}

	if(mCurMode == EMODE_TILESET){
		if(ImButtonsClipboard.mLeft.bButtonIsDown){ 
			if(ImButtonsClipboard.mLeft.mMousePos.y < mTileSet.mClipboardTileSet.mBrushOffset) {return 0;}
			int tSel = -1;
			tSel = searchRectsXY(mTileSet.mClipboardTileSet.BrushAreas, ImButtonsClipboard.mLeft.mMousePos.x, ImButtonsClipboard.mLeft.mMousePos.y);
			if(tSel != -1){
				mTileSet.mClipboardTileSet.mSelectedBrush = tSel;
				mCurrentBrushPixelTileSet = mTileSet.mClipboardTileSet.mBrushes[tSel];

				if(mTileSet.mClipboardTileSet.bIsEditing){
					tSel = searchRectsXY(mTileSet.mClipboardTileSet.mBrushes[mTileSet.mClipboardTileSet.mSelectedBrush]->BrushElementAreas, ImButtonsClipboard.mLeft.mMousePos.x, ImButtonsClipboard.mLeft.mMousePos.y);
					if(tSel > -1){
						mTileSet.mClipboardTileSet.mBrushes[mTileSet.mClipboardTileSet.mSelectedBrush]->mCursorPos = tSel;
					}
				}
			}		
		}

		if(ImButtonsClipboard.mRight.bButtonIsDown){ 
			if(ImButtonsClipboard.mRight.mMousePos.y < mTileSet.mClipboardTileSet.mBrushOffset) {return 0;}			
			
			if(mTileSet.mClipboardTileSet.bIsEditing){
				int tSel = searchRectsXY(mTileSet.mClipboardTileSet.mBrushes[mTileSet.mClipboardTileSet.mSelectedBrush]->BrushElementAreas, ImButtonsClipboard.mRight.mMousePos.x, ImButtonsClipboard.mRight.mMousePos.y);
				if(tSel > -1){
					mTileSet.mClipboardTileSet.mBrushes[mTileSet.mClipboardTileSet.mSelectedBrush]->mCursorPos = tSel;
					mTileSet.mClipboardTileSet.addBrushElement(mTileSet.mClipboardTileSet.mLastElement);
				}
			}
		}
		
	}


	if(mCurMode == EMODE_MAP){
		if(ImButtonsClipboard.mLeft.bButtonIsDown){ 
			if(ImButtonsClipboard.mLeft.mMousePos.y < mClipboardMap.mBrushOffset) {return 0;}
			int tSel = -1;
			tSel = searchRectsXY(mClipboardMap.BrushAreas, ImButtonsClipboard.mLeft.mMousePos.x, ImButtonsClipboard.mLeft.mMousePos.y);
			if(tSel != -1){
				mClipboardMap.mSelectedBrush = tSel;
				mCurrentBrushTile = mClipboardMap.mBrushes[tSel];

				if(mClipboardMap.bIsEditing){
					tSel = searchRectsXY(mClipboardMap.mBrushes[mClipboardMap.mSelectedBrush]->BrushElementAreas, ImButtonsClipboard.mLeft.mMousePos.x, ImButtonsClipboard.mLeft.mMousePos.y);
					if(tSel > -1){
						mClipboardMap.mBrushes[mClipboardMap.mSelectedBrush]->mCursorPos = tSel;
					}
				}				

			}		
		}

		if(ImButtonsClipboard.mRight.bButtonIsDown){ 
			if(ImButtonsClipboard.mRight.mMousePos.y < mClipboardMap.mBrushOffset) {return 0;}			
			
			if(mClipboardMap.bIsEditing){
				int tSel = searchRectsXY(mClipboardMap.mBrushes[mClipboardMap.mSelectedBrush]->BrushElementAreas, ImButtonsClipboard.mRight.mMousePos.x, ImButtonsClipboard.mRight.mMousePos.y);
				if(tSel > -1){
					mClipboardMap.mBrushes[mClipboardMap.mSelectedBrush]->mCursorPos = tSel;
					mClipboardMap.addBrushElement(mClipboardMap.mLastElement);
				}
			}
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
				if(mBrushesTile.bIsEditing){
					tSel = searchRectsXY(mBrushesTile.mBrushes[mBrushesTile.mSelectedBrush]->BrushElementAreas, ImButtonsBrushes.mLeft.mMousePos.x, ImButtonsBrushes.mLeft.mMousePos.y);
					if(tSel > -1){
						mBrushesTile.mBrushes[mBrushesTile.mSelectedBrush]->mCursorPos = tSel;
					}
				}
			}		
		}
		if(ImButtonsBrushes.mRight.bButtonIsDown){ 
			if(ImButtonsBrushes.mRight.mMousePos.y < mBrushesTile.mBrushOffset) {return 0;}			
			
			if(mBrushesTile.bIsEditing){
				int tSel = searchRectsXY(mBrushesTile.mBrushes[mBrushesTile.mSelectedBrush]->BrushElementAreas, ImButtonsBrushes.mRight.mMousePos.x, ImButtonsBrushes.mRight.mMousePos.y);
				if(tSel > -1){
					mBrushesTile.mBrushes[mBrushesTile.mSelectedBrush]->mCursorPos = tSel;
					mBrushesTile.addBrushElement(mBrushesTile.mLastElement);
				}
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
				if(mBrushesPixel.bIsEditing){
					tSel = searchRectsXY(mBrushesPixel.mBrushes[mBrushesPixel.mSelectedBrush]->BrushElementAreas, ImButtonsBrushes.mLeft.mMousePos.x, ImButtonsBrushes.mLeft.mMousePos.y);
					if(tSel > -1){
						mBrushesPixel.mBrushes[mBrushesPixel.mSelectedBrush]->mCursorPos = tSel;
					}
				}
			}		
		}
		if(ImButtonsBrushes.mRight.bButtonIsDown){ 
			if(ImButtonsBrushes.mRight.mMousePos.y < mBrushesPixel.mBrushOffset) {return 0;}			
			
			if(mBrushesPixel.bIsEditing){
				int tSel = searchRectsXY(mBrushesPixel.mBrushes[mBrushesPixel.mSelectedBrush]->BrushElementAreas, ImButtonsBrushes.mRight.mMousePos.x, ImButtonsBrushes.mRight.mMousePos.y);
				if(tSel > -1){
					mBrushesPixel.mBrushes[mBrushesPixel.mSelectedBrush]->mCursorPos = tSel;
					mBrushesPixel.addBrushElement(mBrushesPixel.mLastElement);
				}
			}
		}				
	}

	if(mCurMode == EMODE_SPRITE){
		if(ImButtonsBrushes.mLeft.bButtonIsDown){ 
			if(ImButtonsBrushes.mLeft.mMousePos.y < mBrushesSprite->mBrushOffset) {return 0;}
			int tSel = -1;
			tSel = searchRectsXY(mBrushesSprite->BrushAreas, ImButtonsBrushes.mLeft.mMousePos.x, ImButtonsBrushes.mLeft.mMousePos.y);
			if(tSel != -1){
				mBrushesSprite->mSelectedBrush = tSel;
				mSprite->mCurrentBrushPixel = mBrushesSprite->mBrushes[tSel];
				if(mBrushesSprite->bIsEditing){
					tSel = searchRectsXY(mBrushesSprite->mBrushes[mBrushesSprite->mSelectedBrush]->BrushElementAreas, ImButtonsBrushes.mLeft.mMousePos.x, ImButtonsBrushes.mLeft.mMousePos.y);
					if(tSel > -1){
						mBrushesSprite->mBrushes[mBrushesSprite->mSelectedBrush]->mCursorPos = tSel;
					}
				}
			}		
		}
		if(ImButtonsBrushes.mRight.bButtonIsDown){ 
			if(ImButtonsBrushes.mRight.mMousePos.y < mBrushesSprite->mBrushOffset) {return 0;}			
			
			if(mBrushesSprite->bIsEditing){
				int tSel = searchRectsXY(mBrushesSprite->mBrushes[mBrushesSprite->mSelectedBrush]->BrushElementAreas, ImButtonsBrushes.mRight.mMousePos.x, ImButtonsBrushes.mRight.mMousePos.y);
				if(tSel > -1){
					mBrushesSprite->mBrushes[mBrushesSprite->mSelectedBrush]->mCursorPos = tSel;
					mBrushesSprite->addBrushElement(mBrushesSprite->mLastElement);
				}
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
				if(mBrushesPixel.bIsEditing){
					tSel = searchRectsXY(mBrushesPixel.mBrushes[mBrushesPixel.mSelectedBrush]->BrushElementAreas, ImButtonsBrushes.mLeft.mMousePos.x, ImButtonsBrushes.mLeft.mMousePos.y);
					if(tSel > -1){
						mBrushesPixel.mBrushes[mBrushesPixel.mSelectedBrush]->mCursorPos = tSel;
					}
				}
            }               
        }
		if(ImButtonsBrushes.mRight.bButtonIsDown){ 
			if(ImButtonsBrushes.mRight.mMousePos.y < mBrushesPixel.mBrushOffset) {return 0;}			
			
			if(mBrushesPixel.bIsEditing){
				int tSel = searchRectsXY(mBrushesPixel.mBrushes[mBrushesPixel.mSelectedBrush]->BrushElementAreas, ImButtonsBrushes.mRight.mMousePos.x, ImButtonsBrushes.mRight.mMousePos.y);
				if(tSel > -1){
					mBrushesPixel.mBrushes[mBrushesPixel.mSelectedBrush]->mCursorPos = tSel;
					mBrushesPixel.addBrushElement(mBrushesPixel.mLastElement);
				}
			}
		}
    }

	if(mCurMode == EMODE_SELEDIT){
        if(ImButtonsBrushes.mLeft.bButtonIsDown){ 
            if(ImButtonsBrushes.mLeft.mMousePos.y < mBrushesPixelSelEd.mBrushOffset) {return 0;}
            int tSel = -1;
            tSel = searchRectsXY(mBrushesPixelSelEd.BrushAreas, ImButtonsBrushes.mLeft.mMousePos.x, ImButtonsBrushes.mLeft.mMousePos.y);
            if(tSel != -1){
                mBrushesPixelSelEd.mSelectedBrush = tSel;
                mCurrentBrushPixelSelEdit = mBrushesPixelSelEd.mBrushes[tSel];
				if(mBrushesPixelSelEd.bIsEditing){
					tSel = searchRectsXY(mBrushesPixelSelEd.mBrushes[mBrushesPixelSelEd.mSelectedBrush]->BrushElementAreas, ImButtonsBrushes.mLeft.mMousePos.x, ImButtonsBrushes.mLeft.mMousePos.y);
					if(tSel > -1){
						mBrushesPixelSelEd.mBrushes[mBrushesPixelSelEd.mSelectedBrush]->mCursorPos = tSel;
					}
				}
            }               
        }
		if(ImButtonsBrushes.mRight.bButtonIsDown){ 
			if(ImButtonsBrushes.mRight.mMousePos.y < mBrushesPixelSelEd.mBrushOffset) {return 0;}			
			
			if(mBrushesPixelSelEd.bIsEditing){
				int tSel = searchRectsXY(mBrushesPixelSelEd.mBrushes[mBrushesPixelSelEd.mSelectedBrush]->BrushElementAreas, ImButtonsBrushes.mRight.mMousePos.x, ImButtonsBrushes.mRight.mMousePos.y);
				if(tSel > -1){
					mBrushesPixelSelEd.mBrushes[mBrushesPixelSelEd.mSelectedBrush]->mCursorPos = tSel;
					mBrushesPixelSelEd.addBrushElement(mBrushesPixelSelEd.mLastElement);
				}
			}
		}
    }


	return 0;
}

int TEditor::handleColEdit(){

	if(ImButtonsColEdit.mLeft.bButtonIsDown){
		if(ImButtonsColEdit.mLeft.mMousePos.y < mColMapEdit.mColMapOffset) {return 0;}
		int tSel = -1;		
		tSel = searchRectsXY(mColMapEdit.TileAreas, ImButtonsColEdit.mLeft.mMousePos.x, ImButtonsColEdit.mLeft.mMousePos.y);		
		if((tSel > -1)){
			mColMapEdit.mSelectedTile = tSel;
			mColMapEdit.mCollisionValue = mTileMap->mColMap.MapData[tSel];
		}				
	}

	if(ImButtonsColEdit.mRight.bButtonIsDown){
		if(ImButtonsColEdit.mRight.mMousePos.y < mColMapEdit.mColMapOffset) {return 0;}
		int tSel = -1;		
		tSel = searchRectsXY(mColMapEdit.TileAreas, ImButtonsColEdit.mRight.mMousePos.x, ImButtonsColEdit.mRight.mMousePos.y);
		if((tSel > -1)){			
			mColMapEdit.mSelectedTile = tSel;
			mTileMap->mColMap.MapData[tSel] = mColMapEdit.mCollisionValue;
		}				
	}

	return 0;
}

int TEditor::handleSprite(){
	
	if(ImButtonsSprite.mLeft.bButtonIsDown){ 
		int tSel = -1;
		tSel = searchRectsXY(mSprite->FrameAreas, ImButtonsSprite.mLeft.mMousePos.x, ImButtonsSprite.mLeft.mMousePos.y);
		if(tSel != -1){
   	 		mSprite->selectFrame(tSel);   	 		
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
	} else if(ImButtonsTileSet.mRight.bButtonIsDown && mClipboardMap.bIsEditing){
		int tSel = -1;		
		tSel = searchRectsXY(mTileSet.TileAreas, cx, cy);
		mClipboardMap.addBrushElement(tSel);
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
				if(mGlobalSettings.mGlobalTexParam.TexBPP < 8){
					mGlobalSettings.mGlobalTexParam.PaletteOffset = mTileMap->getOffset(tSel);
				}
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
			mTileMap->mSelection.confirmSelection(mTileMap->TileAreas, mGlobalSettings.mGlobalTexParam.TexSizeX * mGlobalSettings.TileMapScale,  mGlobalSettings.mGlobalTexParam.TexSizeY * mGlobalSettings.TileMapScale);
		} else {
			findSelMap();
		}
	}

	return 0;
}

int TEditor::setThemeColors(){
	switch(mGlobalSettings.mThemeColorIndex){
		case 1 :
			mGlobalSettings.mINIFile.Theme_SelectionDark->ivalue = mGlobalSettings.mThemeColorNew;
			break;
		case 2 :
			mGlobalSettings.mINIFile.Theme_SelectionLight->ivalue = mGlobalSettings.mThemeColorNew;
			break;
		case 3 :
			mGlobalSettings.mINIFile.Theme_HighlightDark->ivalue = mGlobalSettings.mThemeColorNew;
			break;
		case 4 :
			mGlobalSettings.mINIFile.Theme_HighlightLight->ivalue = mGlobalSettings.mThemeColorNew;
			break;
		case 5 :
			mGlobalSettings.mINIFile.Theme_PixelGridDark->ivalue = mGlobalSettings.mThemeColorNew;
			break;
		case 6 :
			mGlobalSettings.mINIFile.Theme_PixelGridLight->ivalue = mGlobalSettings.mThemeColorNew;
			break;
		default:
			break;
	}

	//cancelActiveDialog();

	if(mGlobalSettings.mINIFile.Win_Theme->ivalue == 1){
		mGlobalSettings.setColors(true);
	} else {
		mGlobalSettings.setColors();
	}

	return 0;
}

int TEditor::handleEvents(){
	
	SDL_GetRelativeMouseState(&rx, &ry);
	mButtonState = SDL_GetMouseState(&cx, &cy);
	rx /= mGlobalSettings.mUIScale;
	ry /= mGlobalSettings.mUIScale;

	cx /= mGlobalSettings.mUIScale;
	cy /= mGlobalSettings.mUIScale;

	int mapWidthX = mGlobalSettings.TileMapWidth*mGlobalSettings.mGlobalTexParam.TexSizeX*mGlobalSettings.TileMapScale;					
	int mapWidthY = mGlobalSettings.TileMapHeight*mGlobalSettings.mGlobalTexParam.TexSizeY*mGlobalSettings.TileMapScale;
	
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

			handleState();
			cancelActiveDialog();
			mGlobalSettings.mEditorState = ESTATE_NONE;
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
				handleClipboard();
				if(bShowCollisionEditor){
					handleColEdit();
				}
				handleEMMAp();
			}
			if(mCurMode == EMODE_TILE){
				handleBrushes();
				handleClipboard();
				handleEMTile();
			}
			if(mCurMode == EMODE_SPRITE){
				handleBrushes();
				handleClipboard();				
				findSelSprite();
				handleSprite();
				handlePalette();
			}

			if(mCurMode == EMODE_PALED){
				handlePaletteEdit();
			}
			if(mCurMode == EMODE_TILESET){
				handleBrushes();
				handleClipboard();
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
			//activateQuitDialog();
			activateDTDialog(EDIALOG_PROGRAMQUIT);			  		
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
					
					//activateQuitDialog();
					activateDTDialog(EDIALOG_PROGRAMQUIT);
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_SPACE){
	  				switchMode();
	  			}
				if(cEvent->key.keysym.sym == SDLK_s){
					if(mCurMode == EMODE_MAP) mGlobalSettings.bShowTypeSelection = !mGlobalSettings.bShowTypeSelection;
					if((mCurMode == EMODE_TILE) || (mCurMode == EMODE_TILESET) || (mCurMode == EMODE_SELEDIT)) mGlobalSettings.bShowPixelType = !mGlobalSettings.bShowPixelType;					
					if(mCurMode == EMODE_SPRITE) mGlobalSettings.bShowPixelTypeSprite = !mGlobalSettings.bShowPixelTypeSprite;					
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_p){
		  			if(mCurMode == EMODE_TILE) mGlobalSettings.bShowPixelGrid = !mGlobalSettings.bShowPixelGrid;
					if(mCurMode == EMODE_SPRITE) mGlobalSettings.bShowPixelGridSprite = !mGlobalSettings.bShowPixelGridSprite;
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
				if(cEvent->key.keysym.sym == SDLK_c){
					handleCopyPaste();
				}
				if(cEvent->key.keysym.sym == SDLK_m){
					handleCopyPaste(true);
				}
				if(cEvent->key.keysym.sym == SDLK_v){
					if(bLCTRLisDown){
						handleClipboardPaste(true);
					} else {
						handleClipboardPaste();
					}
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
		  			//activateHelpDialog();
					mGlobalSettings.bShowHelpDialog = true;
					activateDTDialog(EDIALOG_HELPMENU);
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_F2){	  				
		  			activateProjectInfo();
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_F3){	  
					if(mCurMode == EMODE_MAP){				
		  				createNewTile();
					}
					if(mCurMode == EMODE_SPRITE){				
		  				createNewFrame();
					}
	  			}
				if(cEvent->key.keysym.sym == SDLK_F4){
					if(mCurMode == EMODE_MAP){								
						createNewTileCopy(mGlobalSettings.CurrentEditor->mTileSelectedTile);
					}
					if(mCurMode == EMODE_SPRITE){				
		  				createNewFrameCopy(mSprite->mFrame);
					}
	  			}
				if(cEvent->key.keysym.sym == SDLK_F5){	  														
					if(mCurMode == EMODE_MAP){
						if(mGlobalSettings.mGlobalTexParam.TexSizeX == mGlobalSettings.mGlobalTexParam.TexSizeY){					
							rotateTileLeft();
						}
					}
					if(mCurMode == EMODE_SPRITE){
						if(mSprite->mTexParam.TexSizeX == mSprite->mTexParam.TexSizeY){
							rotateFrameLeft();
						}
					}
	  			}
				if(cEvent->key.keysym.sym == SDLK_F6){	  									
					if(mCurMode == EMODE_MAP){
						if(mGlobalSettings.mGlobalTexParam.TexSizeX == mGlobalSettings.mGlobalTexParam.TexSizeY){					
							rotateTile();
						}
					}
					if(mCurMode == EMODE_SPRITE){
						if(mSprite->mTexParam.TexSizeX == mSprite->mTexParam.TexSizeY){
							rotateFrameRight();
						}
					}
	  			}
				if(cEvent->key.keysym.sym == SDLK_DELETE){	  									
					if(mCurMode == EMODE_MAP){
						activateDropUnusedTile();
					}
					if(mCurMode == EMODE_SPRITE){
						if(mGlobalSettings.CurrentEditor->mSprite->mFrames.size() > 1){
							activateRemoveFrame();
						} else {							
							activateRemoveSpriteDialog();
						}
					}					
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_F12){	  						  			
					activateSaveDialog();
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_F11){	  				
		  			//activateSaveAsDialog();
					activateDTDialog(EDIALOG_PROJECTSAVEAS);
	  			}
				if(cEvent->key.keysym.sym == SDLK_F7){	  				
					activateClipboard();
	  			}
				if(cEvent->key.keysym.sym == SDLK_F8){	  				
					activateBrushes();
	  			}
				if(cEvent->key.keysym.sym == SDLK_b){	  				
					nextBrushPos();
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
				bSpriteGrapped = false;
				bTileGrapped = false;
				
	  		}
			if(cEvent->key.keysym.sym == SDLK_LSHIFT){
	  			bLShiftIsDown = false;
			}
	  		break;
	  	case SDL_MOUSEBUTTONDOWN:
	        if (cEvent->button.button == SDL_BUTTON_LEFT){	  		
			    mouseSelX = cEvent->motion.x / mGlobalSettings.mUIScale;
			    mouseSelY = cEvent->motion.y / mGlobalSettings.mUIScale;
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
				x /= mGlobalSettings.mUIScale;
				y /= mGlobalSettings.mUIScale;
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

	if(mGlobalSettings.bHighDPI){
		mGlobalSettings.windowResizeHighDPI(newWidth, newHeight);
	} else {
 		mGlobalSettings.WindowWidth = newWidth;
 		mGlobalSettings.WindowHeight = newHeight; 		
	}
		
	mTileSet.updateWinPos = true;
	mTileSet.reCalculateScale();

	mPaletteOffset.bUpdateWinPos = true;
		
	mProjectInfo->bUpdateWinPos = true;
	
	mProgramInfo->bUpdateWinPos = true;
	
	mPaletteUpdate.bUpdateWinPos = true;
		
	//mNewTileMapDialog.bUpdateWinPos = true;

	mOpenSpriteDialog.bUpdateWinPos = true;
	mNewSpriteDialog.bUpdateWinPos = true;
	
	mPaletteUpdate.bUpdateWinPos = true;
		
	mColMapEdit.bUpdateWinPos = true;

	mPalette.bUpdateWinPos = true;

	for(int di = 1; di < mDTDialogs.size(); di++){
		mDTDialogs[di]->bUpdateWinPos = true;
	}

	for(auto *cSprite : mSprites){
		cSprite->updateWinPos = true;
	}

	return 0;
}
