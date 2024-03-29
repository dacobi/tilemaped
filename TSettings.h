#ifndef __TSETTINGS__
#define __TSETTINGS__

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer.h"
//#include "misc/cpp/imgui_stdlib.h"
//
#include <stdio.h>
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
#include <algorithm>
#include <functional>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>

#include "TOverlay.h"

class TTexture;
class TTFTexture;
//class TOverlayText;
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
class TCollisionMap;
class TSFrame;
class TSprite;




namespace fs = std::filesystem;

//#define DIRDEL "\"
//#define DIRDEL "/"

#define DIRDEL (const char)(std::filesystem::path::preferred_separator)

template <class T> class TVec2 {
public:
	T x, y;
};

enum TKeyType{
    type_float,
    type_int,
    type_string,
	type_bool,
    error
};

class sKey{
public:
	sKey(std::string _kname, TKeyType _ktype, double _value);
	sKey(std::string _kname, TKeyType _ktype, std::string _value);
	sKey(std::string _kname, TKeyType _ktype, int _value);
	sKey(std::string _kname, TKeyType _ktype, bool _value);
	void sKeyEnable();
	void sKeyDisable();
	std::string getString();
	int getInteger();
	double getFloat();
	bool getBool();
	std::string kname;
	TKeyType ktype;
	bool bEnabled;
	int ivalue;
	bool bvalue;
	double fvalue;
	std::string svalue;
	std::vector<int> optionvalue;
	std::vector<std::string> optiontext;
	void addOption(std::string text, int value);
};

class Settings{
public:
	Settings();
	int load(std::string filename);
	bool bLoaded = false;
	int writedefault(std::string filename);
	
	std::map<std::string,int> keyindex;
	std::vector<sKey*> keys;
	
	sKey* createNewKey(std::string nKeyName, TKeyType nKeyType, double nKeyFloat);
	sKey* createNewKey(std::string nKeyName, TKeyType nKeyType, std::string nKeyString);
	sKey* createNewKey(std::string nKeyName, TKeyType nKeyType, int nKeyInteger);
	sKey* createNewKey(std::string nKeyName, TKeyType nKeyType, bool nKeyBool);

	sKey* getKey(std::string gKeyName);

	sKey *error_catch;
};

class ProjectSettings : public Settings{
public:
	ProjectSettings();
	void close();
	void init();

	int removeKey(std::string dKey);

	sKey* getSpriteScaleKey(int cSNum);
	sKey* createSpriteScaleKey(int cSNum);
	int removeSpriteScaleKey(int cSNum);

	sKey* getFrameScaleKey(int cSNum);
	sKey* createFrameScaleKey(int cSNum);
	int removeFrameScaleKey(int cSNum);

	sKey* getSpriteGridKey(int cSNum);
	sKey* createSpriteGridKey(int cSNum);
	int removeSpriteGridKey(int cSNum);

	sKey* getMapTileZeroKey(int cMNum);
	sKey* createMapTileZeroKey(int cMNum);
	int removeMapTileZeroKey(int cMNum);
	
	sKey *Editor_SelectionAppend;
	sKey *Editor_UseTextureFiltering;
	sKey *Editor_PixelScale;	
	sKey *Editor_OverlayTextColor;
	sKey *Editor_OverlayTextSize;
	sKey *Editor_OverlayToggleColVals;
	sKey *Editor_TileSetMaximized;
	sKey *Tile_ShowPixelGrid;
	sKey *TileSet_ShowPixelGrid;
	sKey *TileSet_ShowTileGrid;	
	sKey *TileSet_UpdateMaps;
	sKey *TileSet_WarnBeforeDelete;
	sKey *TileSet_EditWidth;
	sKey *TileSet_PixelScale;	
	sKey *TileSet_MaxGridWidth;	
	sKey *TileSet_MinTileScale;	
	sKey *TileSet_TileScale;	
	sKey *TileSet_ShowOverlay;
	sKey *ColMap_ShowOverlay;
	sKey *SelectionEdit_ShowPixelGrid;
	sKey *SelectionEdit_ShowTileGrid;
	sKey *SelectionEdit_PixelScale;	
	sKey *Sprite_ShowPixelGrid;
	sKey *Sprite_WarnBeforeDelete;
	sKey *Clipboard_SaveToProject;
};

class ProgramSettings : public Settings{
public:
	ProgramSettings();
	//void close();
	
	sKey *Sys_Renderer;
	sKey *Sys_VSYNC;
	sKey *Sys_FontPath;
	sKey *Win_WarnBeforeQuit;
	sKey *Win_Restore;
	sKey *Win_Width;
	sKey *Win_Height;	
	sKey *Win_Maximize;	
	sKey *Win_HighDPI;
	sKey *Win_UIScale;
	sKey *Win_Theme;
	sKey *Theme_SelectionDark;
	sKey *Theme_SelectionLight;
	sKey *Theme_HighlightDark;
	sKey *Theme_HighlightLight;
	sKey *Theme_PixelGridDark;
	sKey *Theme_PixelGridLight;
};

#define MTEXSIZEX 16
#define MTEXSIZEY 16
#define MTEXBPP 8
#define MTEXPIXELSIZE 16
#define MTEXPALETTEOFFSET 0
#define MTEXEDITSCALE 4
#define MTEXRENDERSIZE 16


struct TextureParameters{
	int TexSizeX = MTEXSIZEX;
	int TexSizeY = MTEXSIZEY;
	int TexBPP = MTEXBPP;
	int TexPixelSize = MTEXPIXELSIZE;
	int PaletteOffset = MTEXPALETTEOFFSET;
	int TexEditScale = MTEXEDITSCALE;
	int TexRenderSize = MTEXRENDERSIZE;	
};

void texParamReset(TextureParameters &mTexParam);

class TSettings{
	public:		
		ImGuiIO *mio;
		void shutdown();
		void close();
		void handleCreateError(int retvalcp);
		void handleLoadError(int retvallp);
		TEditor* mEditor;		
		SDL_Renderer *TRenderer;
		SDL_Window *TWindow;
		SDL_Texture *TTarget = NULL;
		ProjectSettings mProjectSettings;		
		TextureParameters mGlobalTexParam;

		TOverlayText mOverlayText;

		std::string mProgramMessage;

		bool bProjectIsDirty = false;
		int mDirtyCount = 0;
		
		void windowResizeHighDPI(int cWidth, int cHeight);
		void renderShow();

		std::string mErrorMessage = "";

		int mNewUIScale = 0;
		
		int WindowWidth=1900;
		int WindowHeight=1000;

		int WindowWidthActual=1900;
		int WindowHeightActual=1000;

		int TileSetMaxSize = 1024;

		int TopBarHeight = 50;
		int TileSetWidth=200;		
		int TileSetDefaultScale=10;
		int TileMapScale=3;
		int TileMapHeight=128;
		int TileMapWidth=128;	
				
		std::map<int, int> mTileBPPSize = {{8,1 },{4,2 },{2,4}};
		std::map<int, int> mFrameSizeIn = {{0,8},{1,16},{2,32},{3,64}};
		std::map<int, int> mFrameSizeOut = {{8,0},{16,1},{32,2},{64,3}};
		bool bShowPaletteOffset = false;		
		int PaletteScale=2;
		std::string ProjectPath;
		std::string ProjectPalettePath = "";
		std::vector<unsigned char> ProjectPalette;
		bool bProjectHasPalette = false;
		int mEditorState = 0;		
		int mOpenCreateProjectState = 0;
		bool bRunningOCD = false;		
		std::string mNewTileMapPath = "";
		int mNewTileMapPaletteOffset = 0;
		bool bNewTileMapOffset = false;
		int mNewTileMapOffset = 0;
		int mNewTileMapState = 0;
		int mNewTileMapX = 0;
		int mNewTileMapY = 0;

		std::string mNewPalettePath = "";
		int mNewPaletteStartExtern = 0;
		int mNewPaletteRangeExtern = 0;
		int mNewPaletteStartIntern = 0;

		std::string mExportPalettePath = "";
		int mExportPaletteType = 1;
		int mExportPaletteStart = 0;
		int mExportPaletteRange = 0;
		bool bExportPaletteHasRange = false;
		
		float mRotateFrameAngle = 0;
		float mScaleFrameFactor = 0;
		std::string mNewTilePath = "";
		std::string mNewFramePath = "";
		std::string mNewFramesPath = "";
		std::string mNewSpritePath = "";
		bool bNewTileSetCreate = false;
		bool bNewTileHasOffset = false;
		bool bNewTileOffsetZero = false;
		int mNewTileOffsetType = 1;
		int mNewTileColorOffset = 1;
		bool bNewSpriteHasOffset = false;
		int mNewSpriteOffsetType = 1;
		int mNewSpriteColorOffset = 1;
		bool bNewSpriteOffsetZero = false;
		int mNewSpriteType = 0;
		int mNewSpriteX = 0;
		int mNewSpriteY = 0;
		int mNewSpriteBPP = 8;
		int mNewTileSize = 0;
		float mNewSpriteScale = 1.0;
		int mNewSpriteUpscale = 2;
		int mNewSpriteRange = 90;
		int mNewSpriteRangeIntervals = 1;
		float mNewSpriteRotationsAngle = 0;
		int mNewSpriteRotations = 1;
		int mNewSpriteDownscale = 2;
		int mDeleteTileMapState = 0;			
		int mTileMapFileCount = 0;
		ImU32 mThemeColorNew = 0xFFFFFFFF;
		int mThemeColorIndex = 0;	
		bool bShowTypeSelection = false;
		bool bShowPixelGrid = true;
		bool bShowPixelGridSprite = true;
		bool bShowTileGrid = true;
		bool bShowTilePixelGrid = true;
		bool bShowTileSelGrid = true;
		bool bShowTilePixelSelGrid = true;
		bool bTileSetOrderUpdateTileMap = true;
		bool bTileSetWarnBeforeDelete = true;
		bool bSpriteWarnBeforeDelete = true;
		int mTileSetEditWidth = 4;
		int mTileSetEditScale = 10;		
		int mSelectionEditScale = 10;
		bool bShowPixelType = false;
		bool bShowPixelTypeSprite = false;
		bool bShowSelectedTile = true;
		bool bShowProjectInfo = false;		
		bool bShowHelpDialog = false;		
		int mSelectedTile = 0;
		int mBrushPosition = 0;		
		ImFont* DFont;
		ImFont* SFont;
		ImFont* LFont;
		SDL_Color DefaultBGColor = {0xc0,0xc0,0xc0,0xff};
		SDL_Color DefaultDarkBGColor = {0xb0,0xb0,0xb0,0xff};
		SDL_Color DefaultTextColor = {0xff,0xff,0xff,0xff};
		SDL_Color AltTextColor = {0x0,0x0,0xff,0xff};
		SDL_Color OverlayTextColor = {0x0,0x0,0x0,0xff};
		SDL_Color DefaultBorderColor = {0x00,0x00,0xff,0xff};
		SDL_Color DefaultButtonColor = {0x90,0x90,0x90,0xff};
		SDL_Color DefaultButtonBorderColor = {0x20,0x20,0x20,0xff};
		SDL_Color DefaultGUIBorderColor = {0x20,0x20,0x20,0xff};
		SDL_Color DefaultHighlightColor = {0xff,0xff,0xff,0xff};
		SDL_Color AltHighlightColor = {0x00,0xff,0xff,0xff};
		SDL_Color ErrorTextColor = {0xff,0x00,0x00,0xff};
		SDL_Color ErrorBorderColor = {0xc0,0x00,0x00,0xff};
		SDL_Color PixelGridColor = {0xa0,0xa0,0xa0,0xff};
		ImU32 ImAltHighLightColor = 0xFFFFFF00;		
		ImU32 ImHighLightColor = 0xFFFFFFFF;
		ImU32 ImPixelGridColor = 0xFFa0a0a0;
		ImU32 ImFrameHighLightColor = 0xFF0000FF;
		std::vector<std::string> mHelpText;
		std::vector<std::string> mHelpTextMap;
		std::vector<std::string> mHelpTextTile;
		std::vector<std::string> mHelpTextTileSet;
		std::vector<std::string> mHelpTextPalette;
		std::vector<std::string> mHelpTextImport;
		std::vector<std::string> mHelpTextSelection;
		std::vector<std::string> mHelpTextColMapEditor;
		std::vector<std::string> mHelpTextSprite;
		std::vector<std::string> mHelpTextAbout;
		std::vector<std::string> mHelpTextLicense;
		int mLastTick = 0;
		int mCurrentTick = 0;
		int initSettings();
		void initHelpText();
		int testPaletteFile(std::string palPath);
		int testProjectFolder(std::string cPath);
		bool getSpriteFileHeader(std::string sPath, int &cSpriteX, int &cSpriteY, int &cSpriteBPP, std::vector<unsigned char> &sBuf);
		void printHelpText();		
		int initTicks();
		int updateTicks();
		int getTicks();
		//int runOCD(int mode=0);
		void settingsMenu();
		void setColors(bool cLightMode = false);
		std::string mWindow = "\uf2d0";
		std::string mFloppy = "\ue240"; //Floppy disk		
		std::string mPrompt = "\uf120"; //Prompt
		std::string mFile = "\uf15c"; // File Icon
		std::string mBook = "\uf405"; //Open Book SEL		
		std::string mInfo = "\uf449"; // Info circle		
		std::string mImage = "\uf71e"; // Info circle	
		std::string mExit = "\uf705";
		std::string mGear = "\uf013";
		std::string mBall = "\uf111";
		std::string mTool = "\ue20f";
		std::string mClip = "\uf0ea";
		std::string mBrush = "\uf1fc";
		std::string mCopy = "\uf0c5";
		std::string mCut = "\uf0c4";		
		std::string mPoint = "\uf245";
		bool bSelectionMode = true;
		bool bSoftwareRendering = false;
		bool bVSync = true;
		bool bMaximize = false;
		bool bRenderingD3D = false;
		bool bHighDPI = false;
		float mUIScale = 1.0;
		int mUseTextureFiltering = 0;
		ProgramSettings mINIFile;		
};

enum {
	EMODE_MAP,
	EMODE_TILE,
	EMODE_PALED,
	EMODE_TILESET,
	EMODE_SELEDIT,
	EMODE_SPRITE
} emodes;

enum {
	EDIALOG_ERROR,
	EDIALOG_SPRITECREATECOPY,
	EDIALOG_SPRITECREATESCALEDCOPY,
	EDIALOG_SPRITECREATEUPSCALEDCOPY,
	EDIALOG_SPRITECREATEDOWNSCALEDCOPY,
	EDIALOG_SPRITECREATEROTATIONRANGE,
	EDIALOG_SPRITECREATEFRAMEROTATIONS,
	EDIALOG_SPRITEFRAMEIMPORT,
	EDIALOG_SPRITEFRAMESIMPORT,
	EDIALOG_TILEMAPIMPORT,
	EDIALOG_SPRITECONVERTBPP,
	EDIALOG_HELPMENU,
	EDIALOG_THEMECOLOR,
	EDIALOG_PROJECTOPEN,
	EDIALOG_PROJECTCREATE,
	EDIALOG_PROJECTCLOSE,
	EDIALOG_PROJECTCLOSESAVE,
	EDIALOG_PROGRAMQUIT,
	EDIALOG_PROGRAMQUITSAVE,
	EDIALOG_PROJECTSAVE,
	EDIALOG_PROJECTSAVEAS,
	EDIALOG_TILESETIMPORT,
	EDIALOG_TILEMAPDELETE,
	EDIALOG_COLMAPREMOVE,	
	EDIALOG_TILEDELETEALL,
	EDIALOG_FRAMEDELETE,	
	EDIALOG_SPRITEDELETE,
	EDIALOG_TILEDELETE,
	EDIALOG_TILEIMPORT,
	EDIALOG_FRAMESCALE,
	EDIALOG_FRAMEROTATE,
	EDIALOG_TILEMAPCREATE,
	EDIALOG_SPRITECREATE,
	EDIALOG_SPRITEIMPORT,
	EDIALOG_PALETTEUPDATE,
	EDIALOG_PALETTEIMPORT,
	EDIALOG_PALETTEEXPORT,
	EDIALOG_TILESETOFFSET,
	EDIALOG_TILEOFFSET,
	EDIALOG_SPRITEOFFSET,
	EDIALOG_FRAMEOFFSET
} edialogs;

enum {
	ESTATE_NONE,
	ESTATE_PROGRAMQUIT,
	ESTATE_PROGRAMQUITSAVE,
	ESTATE_PROJECTSAVE,	
	ESTATE_PROJECTCREATE,	
	ESTATE_PROJECTOPEN,	
	ESTATE_PROJECTCLOSE,
	ESTATE_PROJECTCLOSESAVE,
	ESTATE_TILEIMPORT,	
	ESTATE_TILEDELETE,
	ESTATE_TILEDELETEALL,
	ESTATE_TILESETIMPORT,
	ESTATE_TILEMAPIMPORT,	
	ESTATE_TILEMAPCREATE,
	ESTATE_TILEMAPDELETE,
	ESTATE_PALETTEUPDATE,
	ESTATE_COLMAPREMOVE,
	ESTATE_SPRITECREATE,
	ESTATE_FRAMEDELETE,
	ESTATE_SPRITEDELETE,
	ESTATE_FRAMEIMPORT,
	ESTATE_SPRITEIMPORT,
	ESTATE_SPRITECOPY,
	ESTATE_SPRITESCALEDCOPY,
	ESTATE_SPRITEUPSCALEDCOPY,
	ESTATE_SPRITEDOWNSCALEDCOPY,
	ESTATE_SPRITEROTATIONRANGE,
	ESTATE_SPRITEROTATIONS,
	ESTATE_FRAMESIMPORT,
	ESTATE_FRAMEROTATE,
	ESTATE_FRAMESCALE,
	ESTATE_SPRITECONVERTBPP,
	ESTATE_THEMECOLOR,
	ESTATE_PALETTEIMPORT,
	ESTATE_PALETTEEXPORT,
	ESTATE_TILESETOFFSET,
	ESTATE_TILEOFFSET,
	ESTATE_SPRITEOFFSET,
	ESTATE_FRAMEOFFSET
} estates;


enum {
	ACTION_EMPTY,
	ACTION_TILE,
	ACTION_PIXEL,
	ACTION_TILENEW,
	ACTION_TILEDROP,
	ACTION_PIXELS,
	ACTION_TILES,
	ACTION_TBRUSH,
	ACTION_PBRUSH,
	ACTION_TILEFLIP,
	ACTION_PIXELSSEL,
	ACTION_PBRUSHTILESET,
	ACTION_TILESNEW,
	ACTION_TILEREPLACE,
	ACTION_PIXELSSELEDIT,
	ACTION_SWAPTILES,
	ACTION_FRAMENEW,
	ACTION_FRAMESNEW,
	ACTION_FRAMEREPLACE,
	ACTION_SWAPFRAMES,
	ACTION_MOVETILES
	
} actions;

#endif
