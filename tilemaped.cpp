#include <SDL.h> 
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <set>
#include <map>
#include <filesystem>
#include <sstream>
#include <cstdlib>

//#include "cx16_palette.h"

namespace fs = std::filesystem;

class TTexture;
class TTFTexture;
class TPixel; 
class TPalette;
class Tile;
class TileSet;
class TileProperties;
class TileMap;
class TEditor;
class TEAction;
class TEActionGroup;
class TEActionReplaceTile;
class TEActionReplacePixel;
class TEActionAddTile;
class TEActionDropTile;
class TEActionReplaceMany;
class TEActionReplacePixels;
class TEActionReplaceTiles;
class TEActionUndoStack;
class Dialog;
class SDialog;
class SADialog;
class RTDialog;
class ITDialog;
class RNDialog;
class BDialog;
class HDialog;
class PIDialog;
class TIDialog;
class TBDialog;
class MEDialog;

#include "TSettings.h"
#include "TEAction.h"
#include "TTFTexture.h"
#include "TileMap.h"
#include "TEditor.h"
#include "TDialogs.h"


TSettings mGlobalSettings;


int TEditor::createNewProject(){
	std::cout << "Creating Project: " << mGlobalSettings.ProjectPath << std::endl;
	std::cout << "TileMapWidth: " << mGlobalSettings.TileMapWidth << " TileMapHeight: " << mGlobalSettings.TileMapHeight << " TileSize: " << mGlobalSettings.TileSize << std::endl;	
	if(mGlobalSettings.bProjectHasPalette){
		if(mPalette.loadFromFile(mGlobalSettings.ProjectPalettePath)){
			return 1;
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
	mSaveAsDialog.mDialogTextInput = mGlobalSettings.ProjectPath;
	mSaveAsDialog.init();
	mSaveAsDialog.mSubDialog = &mSaveDialog;
	
	mHelpDialogMap.mCurModeText = mGlobalSettings.mHelpTextMap;
	mHelpDialogMap.mCurModeHead = "TileMap";
	mHelpDialogMap.init();
	
	mHelpDialogTile.mCurModeText = mGlobalSettings.mHelpTextTile;
	mHelpDialogTile.mCurModeHead = "Tile";
	mHelpDialogTile.init();
	
	mTopBar.mEditor = this;
	mTopBar.init();
	mProjectInfo.mEditor = this;
	mProjectInfo.init();
	mOpenTileDialog.init();
	mInfoMessage.init();
	mErrorMessage.setColorScheme(1);
	mErrorMessage.init();

	mRemoveUnused.init();

	mInputNumber.init();
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

	return 0;
}

int TEditor::saveToFolder(std::string path){

	fs::path cpath = path;
	if(!fs::is_directory(fs::status(cpath))){
		fs::create_directory(cpath);
	}
	
	mTileSet.saveToFolder(path);
	mTileMap.saveToFolder(path);
	mPalette.saveToFolder(path);

	return 0;
}

int TEditor::render(){
	mGlobalSettings.updateTicks();

	if(mCurMode == EMODE_MAP){
 		if(!mGlobalSettings.bShowTypeSelection) mTileSelectedTile->bIsSelected = false;		
		mTileMap.render(mTileMapScrollX,mGlobalSettings.TopBarHeight+mTileMapScrollY,&mTileSet);
		mTileSelectedTile->bIsSelected = true;
		
		mTileSet.render(mGlobalSettings.TopBarHeight, mTileSetScrollY);
		if(mActiveDialog){
			mActiveDialog->render((mGlobalSettings.WindowWidth/2)-(mActiveDialog->mDialogWidth/2),(mGlobalSettings.WindowHeight/2)-(mActiveDialog->mDialogHeight/2));
		}
		mTopBar.render(0,0);
		if(mGlobalSettings.bShowProjectInfo){
			mProjectInfo.update();
			mProjectInfo.render(0,mGlobalSettings.TopBarHeight);
		}
	}
	
	if(mCurMode == EMODE_TILE){
		mPalette.render(100+mGlobalSettings.mTileEdScale*mGlobalSettings.TileSize*mGlobalSettings.TileSize,50+mTopBar.mDialogHeight);	
		if(!mGlobalSettings.bShowPixelType) mColorSelectedTile->bPixelSelected = false;
		mTileSelectedTile->renderEd(50,50+mTopBar.mDialogHeight,&mPalette);
		mColorSelectedTile->bPixelSelected = true;
		if(mActiveDialog){
			mActiveDialog->render((mGlobalSettings.WindowWidth/2)-(mActiveDialog->mDialogWidth/2),(mGlobalSettings.WindowHeight/2)-(mActiveDialog->mDialogHeight/2));
		}
		mTopBar.render(0,0);
	}

	return 0;
}

int TEditor::switchMode(){
	if(mCurMode == EMODE_MAP){
		mCurMode = EMODE_TILE;
	} else {
		mCurMode = EMODE_MAP;
	}
	mTopBar.init();
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
		if( mx > (mGlobalSettings.WindowWidth - mGlobalSettings.TileSetWidth)){
			mTileSetScrollY += amount*20;
			if(mTileSetScrollY > 0){mTileSetScrollY = 0;}
			if(mTileSetScrollY < mTileSet.mMaxScrollY){mTileSetScrollY = mTileSet.mMaxScrollY;}	
		} else {
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
	if(mCurMode == EMODE_MAP){
		mGlobalSettings.bShowProjectInfo = !mGlobalSettings.bShowProjectInfo;
	}
	return 0;
}

int TEditor::activateDropUnusedTiles(){
	if(mCurMode == EMODE_MAP){
		mActiveDialog = &mRemoveUnused;
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
		mActiveDialog = &mErrorMessage;
	} else {
		mInfoMessage.mDialogTextMain = cMessage;
		mInfoMessage.update();
		mActiveDialog = &mInfoMessage;
	}
	return 0;
}

int TEditor::activateHelpDialog(){
	if(mCurMode == EMODE_MAP){
		mActiveDialog = &mHelpDialogMap;
	}
	if(mCurMode == EMODE_TILE){
		mActiveDialog = &mHelpDialogTile;
	}

	return 0;
}

int TEditor::activateSaveDialog(){
	mActiveDialog = &mSaveDialog;
	return 0;
}

int TEditor::activateSaveAsDialog(){
	mActiveDialog = &mSaveAsDialog;
	mActiveDialog->bDialogIsWatingForText = true;
	SDL_StartTextInput();
	return 0;
}

int TEditor::activateOpenTileDialog(){
	if(mCurMode == EMODE_MAP){
		mActiveDialog = &mOpenTileDialog;
		mActiveDialog->bDialogIsWatingForText = true;
		SDL_StartTextInput();
	}
	return 0;
}

int TEditor::cancelActiveDialog(){
	mActiveDialog->cancel();
	mActiveDialog = NULL;
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
			int cTileFlip = mTileMap.getFlip(mGlobalSettings.mSelectedTile);
			std::cout << "Flipping Tile: " << cTileFlip << std::endl;		
			cTileFlip++;
			if(cTileFlip > 3){cTileFlip = 0;}
			mTileMap.setFlip(mGlobalSettings.mSelectedTile, cTileFlip);
			return 0;
		}
	}
	return 1;
}

int TEditor::replaceSelectedColor(int x, int y){
	if(mGlobalSettings.bShowPixelType){
				int mOldColor = mColorSelected;
				int tSel = searchRectsXY(mPalette.PixelAreas, x, y);
				if(tSel > -1){					
					std::vector<int> newSelection;
					int cPixIndex = 0;
					for(auto &cPixel : mTileSelectedTile->FileData){
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
						}
					}

				}
	}
	return 0;
}

int TEditor::replaceSelectedTiles(int mx, int my){
	if(mCurMode == EMODE_MAP){	
		int tSel = -1;
		if(mx > (mGlobalSettings.WindowWidth - mGlobalSettings.TileSetWidth)){
			
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
					}
				}
			}		
		}
	}
	return 0;
}


int TEditor::selectTile(int mx, int my){
	if(mCurMode == EMODE_MAP){
		int tSel = -1;
		if(mx > (mGlobalSettings.WindowWidth - mGlobalSettings.TileSetWidth)){
			if(mGlobalSettings.bShowTypeSelection){
				replaceSelectedTiles(mx,my);
			} else {

			tSel = searchRectsXY(mTileSet.TileAreas, mx, my);
			if(tSel != -1){
   	 			mMapSelectedTile = tSel;
   	 			mTileSelectedTile->bIsSelected = false;
   	 			mTileSelectedTile = mTileSet.TTiles[tSel];
   	 			mTileSelectedTile->bIsSelected = true;
			}		
			}
		} else {
			tSel = searchRectsXY(mTileMap.TileAreas, mx, my);
	       		if(tSel != -1){
	       			mGlobalSettings.mSelectedTile = tSel;
					mGlobalSettings.bShowSelectedTile = true;
					if(mGlobalSettings.bShowTypeSelection){
						mMapSelectedTile = mTileMap.getTile(tSel);
   	 					mTileSelectedTile->bIsSelected = false;
   	 					mTileSelectedTile = mTileSet.TTiles[mMapSelectedTile];
   	 					mTileSelectedTile->bIsSelected = true;
					}
				}
		}
	}
	return 0;
}

int TEditor::findSelected(){
	int tSel = -1;
	if(mCurMode == EMODE_MAP){
		tSel = searchRects(mTileSet.TileAreas);
		if(tSel != -1){
   	 		mMapSelectedTile = tSel;
   	 		mTileSelectedTile->bIsSelected = false;
   	 		mTileSelectedTile = mTileSet.TTiles[tSel];
   	 		mTileSelectedTile->bIsSelected = true;
		} else {
		    tSel = searchRects(mTileMap.TileAreas);
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
	} else {
		tSel = searchRects(mPalette.PixelAreas);
		if(tSel != -1){
			mColorSelectedTile->bPixelSelected = false;
			mColorSelected = tSel;
			mColorSelectedTile = mPalette.TPixels[tSel];
			mColorSelectedTile->bPixelSelected = true;
		} else {
			tSel = searchRects(mTileSelectedTile->PixelAreas);
			if(tSel != -1){

	       		TEActionReplacePixel *mCurAction = new TEActionReplacePixel();
				mCurAction->doAction(mTileSelectedTile, tSel, mTileSelectedTile->FileData[tSel], mColorSelected, &mPalette);
				
				if(!(*mCurAction == * mTileSelectedTile->mActionStack.mLastAction)){
	       				mTileSelectedTile->mActionStack.newActionGroup();	
	       				mTileSelectedTile->mActionStack.addAction(mCurAction);
	       				mTileSelectedTile->mActionStack.mLastAction = mCurAction;
	       				mTileSelectedTile->mActionStack.redoClearStack();
	       		}
			}
		}
	}
	return 0;
}

int TEditor::handleEvents(){
	int rx,ry;
	int x,y;
	SDL_GetRelativeMouseState(&rx, &ry);
	int mButtonState = SDL_GetMouseState(&x, &y);
	int mapWidthX = mGlobalSettings.TileMapWidth*mGlobalSettings.TileSize*mGlobalSettings.TileMapScale;					
	int mapWidthY = mGlobalSettings.TileMapHeight*mGlobalSettings.TileSize*mGlobalSettings.TileMapScale;
	
	if(mButtonState & SDL_BUTTON(SDL_BUTTON_LEFT)){
		leftMouseButtonDown = true;
	} else {
		waitLeftMouseButton=false;
		leftMouseButtonDown = false;
		bTileSetGrapped = false;
		bTileMapGrapped = false;
	}

	if(mButtonState & SDL_BUTTON(SDL_BUTTON_RIGHT)){
		rightMouseButtonDown = true;
	} else {
		rightMouseButtonDown = false;
		waitRightMouseButton = false;		
	}
	
	if(mActiveDialog){
		if(mActiveDialog->bInputIsAccept){
			if(mGlobalSettings.mDeleteUnusedTilesState){
				dropUnusedTiles();
				mGlobalSettings.mDeleteUnusedTilesState = 0;
			}
			if(mGlobalSettings.mProjectSaveState == 1){
				saveToFolder(mGlobalSettings.ProjectPath);
				mGlobalSettings.mProjectSaveState = 0;
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
		if(leftMouseButtonDown){
			mActiveDialog->recieveInput(x,y);
			waitLeftMouseButton=true;
		}
		return 0;
	} else {
		if(!waitRightMouseButton){
			if(mCurMode == EMODE_MAP){
				if(rightMouseButtonDown){										
					selectTile(x,y);
				}			
			}
			if(mCurMode == EMODE_TILE){
				if(rightMouseButtonDown){
					replaceSelectedColor(x,y);
				}			
			}
		}
		if(!waitLeftMouseButton){		
			if(leftMouseButtonDown){
				if(bLCTRLisDown){
					if(mCurMode == EMODE_MAP){
						if(bTileSetGrapped){
							mTileSetScrollY += ry * 2;
							if(mTileSetScrollY > 0){mTileSetScrollY = 0;}
							if(mTileSetScrollY < mTileSet.mMaxScrollY){mTileSetScrollY = mTileSet.mMaxScrollY;}
							
						} else if(bTileMapGrapped){
							mTileMapScrollX += rx;
							mTileMapScrollY += ry;;
						} else {
							mouseSelX = x;
							mouseSelY = y;
							if(x > (mGlobalSettings.WindowWidth - mGlobalSettings.TileSetWidth)){
								bTileSetGrapped = true;
							} else {
								bTileMapGrapped = true;
							}
						}
					}
				} else {
				mouseSelX = x;
				mouseSelY = y;
	      		findSelected();
	      		}
			}
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
				if(mActiveDialog->bDialogIsWatingForText){
					mActiveDialog->recieveInput(std::string(cEvent->text.text));
				}
			}
			break;
		case SDL_TEXTEDITING:
	  		if(mActiveDialog){
	  			
			}
			break;
		case SDL_QUIT:
	  		bEditorRunning = false;
  			std::cout << "Shutting Down..." << std::endl;
  			break;
			
	  	case SDL_KEYDOWN:
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
	  		} else {
	  			if(cEvent->key.keysym.sym == SDLK_ESCAPE){
  		  			bEditorRunning = false;
	  				std::cout << "Shutting Down..." << std::endl;
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
					activateDropUnusedTiles();
	  			}
				if(cEvent->key.keysym.sym == SDLK_F7){	  									
					SDL_StartTextInput();
					mActiveDialog = &mInputNumber;
					mInputNumber.mTextInput.bIsNumeric = true;
					//mInputNumber.mTextInput.mMaxRange = 256;
					mInputNumber.mTextInput.mMaxLength = 9;
					//mInputNumber.mTextInput.mMinRange = 1;
					//mInputNumber.mTextInput.bMustNotExist = true;
					//mInputNumber.mTextInput.bMustBeFolder = true;
					mActiveDialog->bDialogIsWatingForText = true;
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_F12){	  				
		  			activateSaveDialog();
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_F11){	  				
		  			activateSaveAsDialog();
	  			}	  			
			}
	  		break;
	  	case SDL_KEYUP:
	  		if(cEvent->key.keysym.sym == SDLK_LCTRL){
	  			bLCTRLisDown = false;
	  			bTileSetGrapped = false;
	  			bTileMapGrapped = false;
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
	
	mTileSet.reCalculateScale();

	return 0;
}

int TSettings::initSettings(){

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	TWindow = SDL_CreateWindow( "Simple Tilemap Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, SDL_WINDOW_SHOWN );
	if( TWindow == NULL ){
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	TRenderer = SDL_CreateRenderer( TWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if( TRenderer == NULL ){
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	
	SDL_SetRenderDrawColor( TRenderer, mGlobalSettings.DefaultHighlightColor.r,mGlobalSettings.DefaultHighlightColor.g,mGlobalSettings.DefaultHighlightColor.b,0xff);

	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) ){
		std::cout << "SDL_image Error: " << IMG_GetError() << std::endl;
		return 1;
 	}
 	
 	TTF_Init();
	TFont =  TTF_OpenFont("nerdfont.ttf",20);

	
	if(TFont == NULL){
		std::cout << "SDL_TTF Error: " << TTF_GetError() << std::endl;
		return 1;
	}

	LFont =  TTF_OpenFont("nerdfont.ttf",30);	
	if(LFont == NULL){
		std::cout << "SDL_TTF Error: " << TTF_GetError() << std::endl;
		return 1;
	}
	
	TTF_SetFontStyle(LFont, TTF_STYLE_UNDERLINE);
	
	SDL_SetWindowResizable(TWindow, SDL_TRUE);

	initHelpText();
	initTicks();

	int cWin = SDL_GetWindowDisplayIndex(TWindow);
	SDL_DisplayMode mCurDispMode;

	SDL_GetCurrentDisplayMode(cWin, &mCurDispMode);

	if(mCurDispMode.w <= 1920){
		mTileEdScale--;		
	}

	srand(time(0));

	return 0;	
}

int TSettings::initTicks(){
	 mLastTick = mCurrentTick = SDL_GetTicks();
	 return 0;
}

int TSettings::updateTicks(){
	mLastTick = mCurrentTick;
	mCurrentTick = SDL_GetTicks();
	return 0;
}

int TSettings::getTicks(){
	return mCurrentTick - mLastTick;
}



void TSettings::initHelpText(){

	mHelpTextMap.push_back("Left Mouse Button: Select Tile and place in TileMap");
	mHelpTextTile.push_back("Left Mouse Button: Select Color and place in Tile");

	mHelpTextMap.push_back("Right Mouse Button: Select Tile or Tile Type in TileMap/TileSet");
	mHelpTextMap.push_back("                    Replace Selected Tile Type from TileSet");
	mHelpTextTile.push_back("Right Mouse Button: Replace Selected Pixel Color with new Color");
	mHelpTextMap.push_back("S: Toggle Show Selected Tile Type");
	mHelpTextMap.push_back("T: Toggle Show Selected Tile in TileMap Editor");

	mHelpTextMap.push_back("Mouse Scroll Wheel: Scale TileMap and Scroll TileSet");
	mHelpTextMap.push_back("LCTRL + Left Mouse Button: Move TileMap and Scroll TileSet");
	mHelpTextMap.push_back("F3: Create Empty Tile");
	mHelpTextMap.push_back("F4: Import Tile from file");	
	mHelpTextMap.push_back("F5: Copy Selected Tile");
	mHelpTextMap.push_back("F6: Remove Unused Tiles. Undo stack will be cleared!");
	

	
	mHelpTextTile.push_back("S: Toggle Show Selected Pixel Color");

	mHelpTextTile.push_back("P: Toggle Show Pixel Grid in Tile Editor");

	mHelpText.push_back("Space: Switch between TileMap and Tile Editor");	
	
	mHelpText.push_back("U: Undo last Action");
	mHelpText.push_back("R: Redo last Action");
}

void TSettings::printHelpText(){
	initHelpText();
	std::cout << "TileMap Editor:" << std::endl;	
	
	for(const auto& cStr : mHelpTextMap){
		std::cout << cStr << std::endl;	
	}

	std::cout  << std::endl;	
	std::cout << "Tile Editor:" << std::endl;	
	
	for(const auto& cStr : mHelpTextTile){
		std::cout << cStr << std::endl;	
	}
		
	std::cout  << std::endl;	
	std::cout << "General:" << std::endl;	

	for(const auto& cStr : mHelpText){
		std::cout << cStr << std::endl;	
	}
}

void printUsage(){
		std::cout << "Usage:" << std::endl;	
		std::cout << "tilemaped -o <folder>" << std::endl;
		std::cout << "Or" << std::endl;	
		std::cout << "tilemaped -n <mapwidth> <mapheight> <tilesize> <folder> [ -p <palette file> ]" << std::endl;
		std::cout << "Fx: tilemaped -n 128 128 16 newfolder" << std::endl;
		std::cout << "tilemaped -h (For help on UI usage)" << std::endl;		
}

int main( int argc, char* args[] )
{
	TEditor mEditor;
	std::stringstream mConvert;
	int nTileSize = 0;
	int nMapSizeX = 0;
	int nMapSizeY = 0;	
	bool mCreateNewProject=false;
	
	if((argc < 3) || ((argc > 3) && (argc < 6)) || ((argc > 8)) || (argc == 7)){
		if((argc == 2) && (std::string(args[1]) == "-h")){
			mGlobalSettings.printHelpText();
		} else {
			printUsage();
		}
		return 0;
	}
	
	if(argc == 3){
		if(std::string(args[1]) != "-o"){
			printUsage();
			return 1;
		}
		if(fs::is_directory(fs::status(args[2]))){
			std::cout << "Folder found" << std::endl;						
		} else {
			std::cout << "Folder not found!" << std::endl;						
			return 1;
		}
	}
	
	if((argc == 6) || (argc == 8)){
		if(std::string(args[1]) != "-n"){
			printUsage();
			return 1;
		}

		if(argc == 8){
			if(args[6] == std::string("-p")){
				if((fs::exists(fs::status(args[7]))) && !(fs::is_directory(fs::status(args[7])))){
					std::cout << "Palette found" << std::endl;						
					mGlobalSettings.bProjectHasPalette = true;
					mGlobalSettings.ProjectPalettePath = std::string(args[7]);
				} else {
					std::cout << "Palette file not found!" << std::endl;						
					return 1;
				}		
			} else {
				printUsage();
				return 1;
			}
		}
		
		mConvert << std::string(args[2]) << std::endl;
		mConvert >> nMapSizeX;
		
		if((nMapSizeX == 32) || (nMapSizeX == 64) || (nMapSizeX == 128) || (nMapSizeX == 256)){				
			mGlobalSettings.TileMapWidth = nMapSizeX;						
		} else {
			std::cout << "Wrong TileMap Size!" << std::endl;
			std::cout << "Valid Values are: 32, 64, 128, 256" << std::endl;						
			return 1;
		}
		
		mConvert << std::string(args[3]) << std::endl;
		mConvert >> nMapSizeY;
		
		if((nMapSizeY == 32) || (nMapSizeY == 64) || (nMapSizeY == 128) || (nMapSizeY == 256)){	
			mGlobalSettings.TileMapHeight = nMapSizeY;
		} else {
			std::cout << "Wrong TileMap Size!" << std::endl;
			std::cout << "Valid Values are: 32, 64, 128, 256" << std::endl;						
			return 1;
		}			

		mConvert << std::string(args[4]) << std::endl;		
		mConvert >> nTileSize;
	
		if((nTileSize == 16) || (nTileSize == 8)){	
			mGlobalSettings.TileSize = nTileSize;
		} else {
			std::cout << "Wrong TileSize!" << std::endl;
			std::cout << "Valid Values are: 8, 16" << std::endl;						
			return 1;
		}

		if(fs::exists(fs::status(args[5]))){
			std::cout << "Error Folder Exists! " << std::endl;						
			return 1;
		} else {
			mGlobalSettings.ProjectPath = std::string(args[5]);						
		}		



		mCreateNewProject=true;		
	}
	
	if( mGlobalSettings.initSettings() ){
		std::cout << "SDL Init Failed!" << std::endl;
	}
	else
	{			
		if(mCreateNewProject){
			if(mEditor.createNewProject()){
				return 1;
			}		
		} else {
			if(mEditor.loadFromFolder(args[2])){
				return 1;
			}		
		}
		
		SDL_Event e;

		while( mEditor.bEditorRunning ){

			SDL_SetRenderDrawColor(  mGlobalSettings.TRenderer, mGlobalSettings.DefaultBGColor.r,  mGlobalSettings.DefaultBGColor.g,  mGlobalSettings.DefaultBGColor.b, 0xff); 
			SDL_RenderClear( mGlobalSettings.TRenderer );

			mEditor.render();
 
			SDL_RenderPresent( mGlobalSettings.TRenderer );

			while( SDL_PollEvent( &e ) != 0 ){
				mEditor.handleEvents(&e);
			}
			mEditor.handleEvents();
		}		
	}
	return 0;
}
