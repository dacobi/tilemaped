#ifndef __TEDITOR__
#define __TEDITOR__

#include "TSettings.h"
#include "TileMap.h"
#include "TSprite.h"
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
		TSprite *mSprite = NULL;
		std::vector<TSprite*> mSprites;
		int switchTileMap(int cTileMap);
		int switchSprite(int cSprite);
		int switchSprite(int cSprite, int cFrame);
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
		int handleColEdit();	
		int findSelMap();
		int findSelTile();
		int findSelSprite();
		int handleSprite();
		int resizeWindowEvent(SDL_Event* event);
		void createDialogs();
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
		ImWinMouseState ImButtonsColEdit;
		ImWinMouseState ImButtonsSprite;		
		bool leftMouseButtonDown = false;		
		bool rightMouseButtonDown = false;
		int leftMouseButtonClicks = 0;
		int rightMouseButtonClicks = 0;
		bool bLCTRLisDown = false;
		bool bLShiftIsDown = false;
		bool bTileMapGrapped = false;
		bool bTileSetGrapped = false;
		bool bSelEditGrapped = false;
        bool bSpriteGrapped = false;
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
		TBrushList mBrushesPixelSprite4;
		TBrushList mBrushesPixelSprite8;
		TBrushList *mBrushesSprite;
		TextureParameters mSprite4;
		TextureParameters mSprite8;
		bool bShowCollisionEditor = false;	
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

		//ISFDialog mOpenFrameDialog;
		//ISFSDialog mOpenFramesDialog;


		RTSFDialog mRotateFrame;
		SSFDialog mScaleFrame;
		ISDialog mOpenSpriteDialog;
		ITSDialog mOpenTileSetDialog;
		ITMDialog mOpenTileMapDialog;
		CTMDialog mNewTileMapDialog;
		CSDialog mNewSpriteDialog;
		HDialog mHelpDialog;
		PIDialog mProjectInfo;
		MEDialog mInfoMessage;
		RTMDialog mRemoveTileMap;
		RCMDialog mRemoveColMap;
		RTDialog mRemoveUnused;
		RSFDialog mRemoveFrame;
		RSDialog mRemoveSprite;
		RTSDialog mRemoveSelUnused;
		
		std::map<int, DTDialog*> mDTDialogs;
		int activateDTDialog(int cOpenDialog, int cCond = -1);

		MEDialog mErrorMessage;				
		QDialog mQuitDialog;
		CCPDialog mCloseProjectDialog;
		PUDialog mPaletteUpdate;
		PODialog mPaletteOffset;

		TCollisionMapEditor mColMapEdit;
		int importTileMap(std::string cNewTileMap);
		int createTileMap(int nMapX, int nMapy, int nTileValue);
		int createTileMap(int nMapX, int nMapy, int nTileValue, int cPaletteOffset);
		int importTileMap(std::string cNewTileMap, int cTileOffset);
		int importTileMap(std::string cNewTileMap, int cTileOffset, int cPaletteOffset);
		Tile* createNewTile();
		Tile* createNewTileCopy(Tile* cCopyTile);
		Tile* createNewTileFromFile(std::string newTilePath);
		TSFrame* createNewFrameFromFile(std::string newFramePath);
		int createNewSpriteScaledCopy(TSprite *newSprite);
		int createNewSpriteUpscaledCopy(TSprite *newSprite);
		int createNewSpriteDownscaledCopy(TSprite *newSprite);
		int createNewSpriteCopy(TSprite *newSprite);
		int createSpriteRotationRange(int cRange, int cIntervals);
		int createSpriteRotations(int cNum, float cInterval);
		int rotateTile();
		int rotateTileLeft();
		int moveTileUp();
		int moveTileDown();
		int moveFrameUp();
		int moveFrameDown();
		int rotateFrameRight();
		int rotateFrameLeft();
		int filterFrame();
		int swapTiles(int source, int target, bool bDoCopy = false);
		int swapFrames(int source, int target, bool bDoCopy = false);
		int dropUnusedTiles();
		int dropUnusedTile(int cDropTile);
		int removeSelectedTile();
		TSFrame* createNewFrame();
		TSFrame* createNewFrameCopy(TSFrame* cCopyFrame);
		int removeSelectedFrame();
		int showMessage(std::string cMessage, bool isError=false);
		bool checkQuit();
		void setSpriteBrushes();

		int activateSaveDialog();
		int activateOpenTileDialog();

		//int activateOpenFrameDialog();
		//int activateOpenFramesDialog();

		int activateRotateFrameDialog();
		int activateScaleFrameDialog();
		
		int activateOpenSpriteDialog();
		int activateOpenTileSetDialog();
		int activateOpenTileMapDialog();
		int activateNewTileMapDialog();
		int activateNewSpriteDialog();
		int activateRemoveSpriteDialog();
		int activateRemoveTileMapDialog();
		int activateColMapDialog(bool bCreateColMap=false);
		int removeColMapDialog();
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
		int activateRemoveFrame();
		
		int activateBrushes();
		int activateBrush();
		int dropBrush();
		int rx,ry;
		int cx,cy;
		int mButtonState;
	
};

#endif