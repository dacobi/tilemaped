#ifndef __TEDITOR__
#define __TEDITOR__

#include "TSettings.h"
#include "TileMap.h"
#include "TDialogs.h"
#include "TSelection.h"

class ImWinMouseButton{
	public:
		TVec2<int> mMousePos = {0,0};
		int ImMouseButton;
		bool bButtonIsDown = false;
		
		bool checkImWindow(){
			int mx = mGlobalSettings.mio->MousePos.x;
			int my = mGlobalSettings.mio->MousePos.y;

			ImVec2 mWinPos = ImGui::GetWindowPos();
			ImVec2 mWinSize = ImGui::GetWindowSize();

			if( ((mx >= mWinPos.x) && (mx <= mWinPos.x + mWinSize.x)) &&  ((my >= mWinPos.y) && (my <= mWinPos.y + mWinSize.y))){
				return true;
			} else {
				return false;
			}
		}

		void updateButtonState(){
			if( ImGui::IsMouseClicked(ImMouseButton) && checkImWindow() ){ 
				bButtonIsDown = true;
				mMousePos.x = mGlobalSettings.mio->MousePos.x;
				mMousePos.y = mGlobalSettings.mio->MousePos.y;
			} else {
				bButtonIsDown = false;
			}
		} 
};

class ImWinMouseState{
	public:
		bool bWindowHasFocus = false;
		ImWinMouseButton mLeft;
		ImWinMouseButton mRight;
		ImWinMouseState(){
			mLeft.ImMouseButton = ImGuiMouseButton_Left;
			mRight.ImMouseButton = ImGuiMouseButton_Right;
		}
		void updateButtonStates(){
			mLeft.updateButtonState();
			mRight.updateButtonState();
			bWindowHasFocus = ImGui::IsWindowFocused() || ImGui::IsWindowHovered();
		}
};

class TEditor{
	public:
		void shutdown();
		void closeProject();
		TPalette mPalette;
		TileSet mTileSet;
		TileMap *mTileMap;
		TSelectionEditor mSelEdit;
		int mLastSelEditX=0;
		int mLastSelEditY=0;
		int mLastSelEditWidth=0;
		int mLastSelEditHeight=0;
		bool bTileMapWasChanged = true;
		std::vector<TileMap*> mTileMaps;
		int switchTileMap(int cTileMap);
		TBDialog mTopBar;
		int mMapSelectedTile=0;
		Tile* mTileSelectedTile = NULL;
		int mColorSelected;
		TPixel* mColorSelectedTile;
		int mColorSelectedEdit;
		TPixel* mColorSelectedTileEdit = NULL;
		int mLastPixelOffset = 0;
		bool bEditorRunning=true;
		int handleSelection(int SELMODE);
		int handleEvents(SDL_Event* cEvent);
		int handleEvents();
		int handleEMMAp();
		int handleTileSet();
		int handleTileSetEdit();
		int handleSelEdit();
		int handleTileMap();
		int handlePalette();
		int handlePaletteEdit();
		int handleTile();
		int handleEMTile();
		int handleBrushes();	
		int findSelMap();
		int findSelTile();
		int resizeWindowEvent(SDL_Event* event);
		void initDialogs();
		int loadFromFolder(std::string path);
		int saveToFolder(std::string path);
		int createNewProject();
		int render();
		int switchMode();
		int setMode(int );
		int mCurMode = EMODE_MAP;
		int mLastMode = EMODE_TILE;
		int mouseSelX=0;
		int mouseSelY=0;
		int mTileMapScrollX = 0;
		int mTileMapScrollY = 0;
		int mTileSetScrollY = 0;
		int mTileSetScrollX = 0;		
		int mSelEditScrollY = 0;
		int mSelEditScrollX = 0;		
		int applyScroll(int mx,int my, int amount, int xamount);
		ImWinMouseState ImButtonsTileSet;		
		ImWinMouseState ImButtonsPalette;		
		ImWinMouseState ImButtonsBrushes;		
		bool leftMouseButtonDown = false;		
		bool rightMouseButtonDown = false;
		int leftMouseButtonClicks = 0;
		int rightMouseButtonClicks = 0;
		bool bLCTRLisDown = false;
		bool bLShiftIsDown = false;
		bool bTileMapGrapped = false;
		bool bTileSetGrapped = false;
		bool bSelEditGrapped = false;
		bool bShowBrushesTile = false;
		bool bShowBrushesPixel = false;
		bool bShowBrushesPixelTileSet = false;		
		bool bShowBrushesPixelSelEdit = false;				
		TBrush *mCurrentBrushTile = NULL;
		TBrush *mCurrentBrushPixel = NULL;
		TBrush *mCurrentBrushPixelTileSet = NULL;
		TBrush *mCurrentBrushPixelSelEdit = NULL;		
		TBrushList mBrushesTile;
		TBrushList mBrushesPixel;		
		int flipSelectedTile(int cFlipMode);		
		int replaceSelectedColor(int x, int y);
		int replaceSelectedTiles(int x, int y);
		int searchRectsXY(std::vector<SDL_Rect> &sRects, int mx, int my);
		int searchRects(std::vector<SDL_Rect> &sRects);
		int selectTiles(std::vector<int> &cNewSelection, int cTileType);
		int selectTilesMap(TileMap* cTileMap, std::vector<int> &cNewSelection, int cTileType);
		int toggleSelectedTile();		
		TEActionUndoStack mActionStack;		
		void undoLastActionGroup();
		void redoLastActionGroup();		
		int updatePalette();
		Dialog *mActiveDialog = NULL;
		Dialog *mActiveMessage = NULL;
		SDialog mSaveDialog;
		SADialog mSaveAsDialog;
		ITDialog mOpenTileDialog;
		ITSDialog mOpenTileSetDialog;
		ITMDialog mOpenTileMapDialog;
		CTMDialog mNewTileMapDialog;
		HDialog mHelpDialog;
		PIDialog mProjectInfo;
		MEDialog mInfoMessage;
		RTMDialog mRemoveTileMap;
		RTDialog mRemoveUnused;
		RTSDialog mRemoveSelUnused;
		MEDialog mErrorMessage;
		RNDialog mInputNumber;
		QDialog mQuitDialog;
		CCPDialog mCloseProjectDialog;
		PUDialog mPaletteUpdate;
		PODialog mPaletteOffset;
		int importTileMap(std::string cNewTileMap);
		int createTileMap(int nMapX, int nMapy, int nTileValue);
		int createTileMap(int nMapX, int nMapy, int nTileValue, int cPaletteOffset);
		int importTileMap(std::string cNewTileMap, int cTileOffset);
		int importTileMap(std::string cNewTileMap, int cTileOffset, int cPaletteOffset);
		Tile* createNewTile();
		Tile* createNewTileCopy(Tile* cCopyTile);
		Tile* createNewTileFromFile(std::string newTilePath);
		int rotateTile();
		int rotateTileLeft();
		int moveTileUp();
		int moveTileDown();
		int swapTiles(int source, int target, bool bDoCopy = false);
		int dropUnusedTiles();
		int dropUnusedTile(int cDropTile);
		int removeSelectedTile();
		int showMessage(std::string cMessage, bool isError=false);
		bool checkQuit();
		int activateSaveDialog();
		int activateOpenTileDialog();
		int activateOpenTileSetDialog();
		int activateOpenTileMapDialog();
		int activateNewTileMapDialog();
		int activateRemoveTileMapDialog();
		int activateHelpDialog();
		int activateQuitDialog();
		int activateOpenCreateDialog(int mode);
		int cancelActiveDialog();
		int activateSaveAsDialog();
		int activateProjectInfo();
		int activatePaletteEdit();
		int activatePaletteUpdate();
		int activateDropUnusedTiles();
		int activateDropUnusedTile();
		int activateBrushes();
		int activateBrush();
		int dropBrush();
		int rx,ry;
		int cx,cy;
		int mButtonState;
	
};

#endif