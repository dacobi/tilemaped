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
//#include <SDL_ttf.h>
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

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>


class TTexture;
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
	
	sKey *Editor_SelectionAppend;
	sKey *Tile_ShowPixelGrid;
	sKey *TileSet_ShowPixelGrid;
	sKey *TileSet_ShowTileGrid;	
	sKey *TileSet_UpdateMaps;
	sKey *TileSet_WarnBeforeDelete;
	sKey *TileSet_EditWidth;
	sKey *SelectionEdit_ShowPixelGrid;
	sKey *SelectionEdit_ShowTileGrid;
};

class ProgramSettings : public Settings{
public:
	ProgramSettings();
	void close();
	
	sKey *Sys_Renderer;
	sKey *Sys_VSYNC;
	sKey *Win_Maximize;	
};


class TSettings{
	public:
		ImGuiIO *mio;
		void shutdown();
		void close();
		TEditor* CurrentEditor;		
		SDL_Renderer *TRenderer;
		SDL_Window *TWindow;
		ProjectSettings mProjectSettings;		
		int WindowWidth=1900;
		int WindowHeight=1000;
		int TopBarHeight = 50;
		int TileSetWidth=200;
		int TileSetDefaultScale=10;
		int TileMapScale=3;
		int TileMapHeight=128;
		int TileMapWidth=128;		
		int TileSizeX=16;
		int TileSizeY=16;
		int TileSetBPP=8;
		int TilePixelSize=16;
		std::map<int, int> mTileBPPSize = {{8,1 },{4,2 },{2,4}};
		int PaletteOffset=0;
		bool bShowPaletteOffset = false;
		int TileRenderSize=16;
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
		int mNewTileMapOffset = 0;
		int mNewTileMapState = 0;
		int mNewTileMapX = 0;
		int mNewTileMapY = 0;		
		std::string mNewTilePath = "";
		int mNewTileSize = 0;
		int mDeleteTileMapState = 0;			
		int mTileMapFileCount = 0;			
		bool bShowTypeSelection = false;
		bool bShowPixelGrid = true;
		bool bShowTileGrid = true;
		bool bShowTilePixelGrid = true;
		bool bShowTileSelGrid = true;
		bool bShowTilePixelSelGrid = true;
		bool bTileSetOrderUpdateTileMap = true;
		bool bTileSetWarnBeforeDelete = true;
		int mTileSetEditWidth = 4;
		int mTileSetEditScale = 10;		
		int mSelectionEditScale = 10;
		bool bShowPixelType = false;
		bool bShowSelectedTile = true;
		bool bShowProjectInfo = false;		
		bool bShowHelpDialog = false;		
		int mSelectedTile = 0;
		int mTileEdScale = 4;		
		ImFont* DFont;
		ImFont* SFont;
		SDL_Color DefaultBGColor = {0xc0,0xc0,0xc0,0xff};
		SDL_Color DefaultDarkBGColor = {0xb0,0xb0,0xb0,0xff};
		SDL_Color DefaultTextColor = {0xff,0xff,0xff,0xff};
		SDL_Color AltTextColor = {0x0,0x0,0xff,0xff};
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
		std::vector<std::string> mHelpText;
		std::vector<std::string> mHelpTextMap;
		std::vector<std::string> mHelpTextTile;
		std::vector<std::string> mHelpTextTileSet;
		std::vector<std::string> mHelpTextPalette;
		std::vector<std::string> mHelpTextImport;
		std::vector<std::string> mHelpTextSelection;
		std::vector<std::string> mHelpTextColMapEditor;
		int mLastTick = 0;
		int mCurrentTick = 0;
		int initSettings();
		void initHelpText();
		int testPaletteFile(std::string palPath);
		int testProjectFolder(std::string cPath);
		void printHelpText();		
		int initTicks();
		int updateTicks();
		int getTicks();
		int runOCD(int mode=0);
		void settingsMenu();
		std::string mWindow = "\uf2d0";
		std::string mFloppy = "\ue240"; //Floppy disk		
		std::string mPrompt = "\uf120"; //Prompt
		std::string mFile = "\uf15c"; // File Icon
		std::string mBook = "\uf405"; //Open Book SEL		
		std::string mInfo = "\uf449"; // Info circle		
		std::string mImage = "\uf71e"; // Info circle	
		std::string mExit = "\uf705";
		int mSelectionMode = 1;
		bool bSoftwareRendering = false;
		bool bVSync = true;
		bool bMaximize = false;
		bool bRenderingD3D = false;
		ProgramSettings mINIFile;		
};

enum {
	EMODE_MAP,
	EMODE_TILE,
	EMODE_PALED,
	EMODE_TILESET,
	EMODE_SELEDIT
} emodes;

enum {
	ESTATE_NONE,	
	ESTATE_PROJECTSAVE,	
	ESTATE_PROJECTCREATE,	
	ESTATE_PROJECTOPEN,	
	ESTATE_PROJECTCLOSE,
	ESTATE_TILEIMPORT,
	ESTATE_TILECREATE,
	ESTATE_TILEDELETE,
	ESTATE_TILEDELETEALL,
	ESTATE_TILESETIMPORT,
	ESTATE_TILEMAPIMPORT,
	ESTATE_TILEMAPIMPORTOFFSET,
	ESTATE_TILEMAPCREATE,
	ESTATE_TILEMAPDELETE,
	ESTATE_PALETTEUPDATE,
	ESTATE_COLMAPREMOVE
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
	ACTION_SWAPTILES
} actions;

#endif
