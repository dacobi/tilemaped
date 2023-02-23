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

namespace fs = std::filesystem;

//#define DIRDEL "\"
//#define DIRDEL "/"

#define DIRDEL (const char)(std::filesystem::path::preferred_separator)

template <class T> class TVec2 {
public:
	T x, y;
};

class TSettings{
	public:
		ImGuiIO *mio;
		void shutdown();
		TEditor* CurrentEditor;		
		SDL_Renderer *TRenderer;
		SDL_Window *TWindow;			
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
		int mProjectSaveState = 0;
		int mProjectOpenState = 0;
		bool bRunningOCD = false;
		int mOpenTileState = 0;
		int mDeleteUnusedTilesState = 0;
		int mPaletteUpdateState = 0;
		std::string mNewTilePath = "";				
		bool bShowTypeSelection = false;
		bool bShowPixelGrid = true;
		bool bShowTileGrid = false;
		bool bShowTilePixelGrid = true;
		int mTileSetEditWidth = 4;
		int mTileSetEditScale = 10;
		bool bShowPixelType = false;
		bool bShowSelectedTile = true;
		bool bShowProjectInfo = false;		
		bool bShowHelpDialog = false;		
		int mSelectedTile = 0;
		int mTileEdScale = 4;
		TTF_Font *TFont;
		TTF_Font *UFont;
		TTF_Font *LFont;
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
		std::vector<std::string> mHelpTextPalette;
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
		int runOCD();
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
		bool bNoVSync = false;
};

enum {
	EMODE_MAP,
	EMODE_TILE,
	EMODE_PALED,
	EMODE_TILESET
} emodes;

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
	ACTION_TILEFLIP
} actions;



#endif
//TSettings mGlobalSettings;