#ifndef __TEDITOR__
#define __TEDITOR__

#include "TSettings.h"
#include "TileMap.h"
#include "TDialogs.h"

class TEditor{
	public:
		void shutdown();
		TPalette mPalette;
		TileSet mTileSet;
		TileMap mTileMap;
		TBDialog mTopBar;
		int mMapSelectedTile;
		Tile* mTileSelectedTile;
		int mColorSelected;
		TPixel* mColorSelectedTile;
		bool bEditorRunning=true;
		int handleEvents(SDL_Event* cEvent);
		int handleEvents();
		int resizeWindowEvent(SDL_Event* event);
		void initDialogs();
		int loadFromFolder(std::string path);
		int saveToFolder(std::string path);
		int createNewProject();
		int render();
		int switchMode();
		int mCurMode = EMODE_MAP;
		int mouseSelX=0;
		int mouseSelY=0;
		int mTileMapScrollX = 0;
		int mTileMapScrollY = 0;
		int mTileSetScrollY = 0;
		int applyScroll(int mx,int my, int amount, int xamount);
		bool leftMouseButtonDown = false;
		bool waitLeftMouseButton = false;
		bool rightMouseButtonDown = false;
		bool waitRightMouseButton = false;
		bool bLCTRLisDown = false;
		bool bTileMapGrapped = false;
		bool bTileSetGrapped = false;
		int flipSelectedTile();
		int selectTile(int mx, int my);
		int replaceSelectedColor(int x, int y);
		int replaceSelectedTiles(int x, int y);
		int searchRectsXY(std::vector<SDL_Rect> &sRects, int mx, int my);
		int searchRects(std::vector<SDL_Rect> &sRects);
		int selectTiles(std::vector<int> &cNewSelection, int cTileType);
		int findSelected();
		int toggleSelectedTile();
		//std::vector<TEActionGroup*> mUndoStack;
		//std::vector<TEActionGroup*> mRedoStack;
		TEActionUndoStack mActionStack;
		void undoLastActionGroup();
		void redoLastActionGroup();
		Dialog *mActiveDialog = NULL;
		SDialog mSaveDialog;
		SADialog mSaveAsDialog;
		ITDialog mOpenTileDialog;
		HDialog mHelpDialogMap;
		HDialog mHelpDialogTile;
		PIDialog mProjectInfo;
		MEDialog mInfoMessage;
		RTDialog mRemoveUnused;
		MEDialog mErrorMessage;
		RNDialog mInputNumber;
		Tile* createNewTile();
		Tile* createNewTileCopy(Tile* cCopyTile);
		Tile* createNewTileFromFile(std::string newTilePath);
		int dropUnusedTiles();
		int dropUnusedTile(int cDropTile);
		int showMessage(std::string cMessage, bool isError=false);
		int activateSaveDialog();
		int activateOpenTileDialog();
		int activateHelpDialog();
		int cancelActiveDialog();
		int activateSaveAsDialog();
		int activateProjectInfo();
		int activateDropUnusedTiles();
};

#endif