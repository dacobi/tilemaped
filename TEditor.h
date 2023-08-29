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
		TEditor(){mClipboardMap.mLocalTexParam.TexEditScale--;};
		void shutdown();
		void closeProject();
		int runOCD(int mode=0);
		OCDialog mOpenCreate;
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
		int mSelectedTileMap = 0;
		int mSelectedSprite = 0;
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
		int handleCopyPaste(bool cCutSelection = false);
		int handleClipboardPaste(bool cCycle = false);
		int handleClipboard();
		int resizeWindowEvent(SDL_Event* event);
		void createDialogs();
		void initDialogs();
		int loadFromFolder(std::string path);
		int saveToFolder(std::string path);
		int createNewProject();
		int render();
		int switchMode();
		int setMode(int );
		int canCopyToBrushes();
		int mCurMode = EMODE_MAP;
		int mLastMode = EMODE_TILE;
		int mouseSelX=0;
		int mouseSelY=0;
		int mTileMapScrollX = 0;
		int mTileMapScrollY = 0;
		int mTileScrollX = 0;
		int mTileScrollY = 0;
		int mTileSetScrollY = 0;
		int mTileSetScrollX = 0;		
		int mSelEditScrollY = 0;
		int mSelEditScrollX = 0;		
		int applyScroll(int mx,int my, int amount, int xamount);
		ImWinMouseState ImButtonsTileSet;		
		ImWinMouseState ImButtonsPalette;		
		ImWinMouseState ImButtonsBrushes;		
		ImWinMouseState ImButtonsClipboard;
		ImWinMouseState ImButtonsColEdit;
		ImWinMouseState ImButtonsSprite;		
		bool leftMouseButtonDown = false;		
		bool rightMouseButtonDown = false;
		int leftMouseButtonClicks = 0;
		int rightMouseButtonClicks = 0;
		bool bLCTRLisDown = false;
		bool bLShiftIsDown = false;
		bool bTileMapGrapped = false;
		bool bTileGrapped = false;
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
		TBrushList mBrushesPixelSelEd;
		TBrushList mBrushesPixelSprite4;
		TBrushList mBrushesPixelSprite8;
		TBrushList *mBrushesSprite;
		void nextBrushPos();
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
		TClipboard mClipboardMap;
		bool bShowClipboardMap = false;
		void undoLastActionGroup();
		void redoLastActionGroup();		
		int updatePalette();

		Dialog *mActiveDialog = NULL;
		Dialog *mActiveMessage = NULL;
					
		HDialog mHelpDialog;

		IDDialog *mProjectInfo;
		void showProjectInfo();

		IDDialog *mProgramInfo;
				
		std::map<int, DTDialog*> mDTDialogs;
		int activateDTDialog(int cOpenDialog, int cCond = -1, int cVal0 = 0, int cVal1 = 0);
		
		PODialog mPaletteOffset;

		void toggleOverlay();

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
		int createNewSpriteConvertBPP8(TSprite *copySprite);
		int createNewSpriteConvertBPP4(TSprite *copySprite);
		int createSpriteRotationRange(int cRange, int cIntervals);
		int createSpriteRotations(int cNum, float cInterval);
		int setThemeColors();
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
								
		int activateColMapDialog(bool bCreateColMap=false, int cFormat = 0);
				
		int cancelActiveDialog();
		void activateSaveDialog();

		int activateProjectInfo();
		int activatePaletteEdit();
		
		int activateDropUnusedTile();
		int activateRemoveFrame();
		
		int activateBrushes(bool bChangeState = true);
		int activateClipboard(bool bChangeState = true);
		int activateBrush();
		int dropBrush();
		int rx,ry;
		int cx,cy;
		int mButtonState;

		std::map<int, std::function<void(TEditor*)>> mStates;

		void initStates();	
		void handleState();	

		void stateNone();
		void stateProgramQuit();
		void stateProjectSave();
		void stateProjectCreate();
		void stateProjectOpen();
		void stateProjectClose();
		void stateTileImport();		
		void stateTileDelete();
		void stateTileDeleteAll();
		void stateTileSetImport();
		void stateTileMapImport();
		void stateTileMapCreate();
		void stateTileMapDelete();
		void statePaletteUpdate();
		void stateColmapRemove();
		void stateSpriteCreate();
		void stateFrameDelete();
		void stateSpriteDelete();
		void stateFrameImport();
		void stateSpriteImport();
		void stateSpriteCopy();
		void stateSpriteScaledCopy();
		void stateSpriteUpscaledCopy();
		void stateSpriteDownscaledCopy();
		void stateSpriteRotationRange();
		void stateSpriteRotations();
		void stateFramesImport();
		void stateFrameRotate();
		void stateFrameScale();
		void stateSpriteConvertBPP();
		void stateThemeColor();
		void statePaletteImport();
		void statePaletteExport();

	
};

#endif