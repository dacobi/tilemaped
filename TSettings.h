#ifndef __TSETTINGS__
#define __TSETTINGS__

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

class TSettings{
	public:
		void shutdown();
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
		int TileSize=16;
		int PaletteScale=2;
		std::string ProjectPath;
		std::string ProjectPalettePath = "";
		std::vector<unsigned char> ProjectPalette;
		bool bProjectHasPalette = false;
		int mProjectSaveState = 0;
		int mOpenTileState = 0;
		int mDeleteUnusedTilesState = 0;
		std::string mNewTilePath = "";				
		bool bShowTypeSelection = false;
		bool bShowPixelGrip = true;
		bool bShowPixelType = false;
		bool bShowSelectedTile = true;
		bool bShowProjectInfo = false;
		int mSelectedTile = 0;
		int mTileEdScale = 4;
		TTF_Font *TFont;
		TTF_Font *LFont;
		SDL_Color DefaultBGColor = {0xc0,0xc0,0xc0,0xff};
		SDL_Color DefaultDarkBGColor = {0xb0,0xb0,0xb0,0xff};
		SDL_Color DefaultTextColor = {0x20,0x20,0x20,0xff};
		SDL_Color DefaultBorderColor = {0x00,0x00,0xff,0xff};
		SDL_Color DefaultButtonColor = {0x90,0x90,0x90,0xff};
		SDL_Color DefaultButtonBorderColor = {0x20,0x20,0x20,0xff};
		SDL_Color DefaultGUIBorderColor = {0x20,0x20,0x20,0xff};
		SDL_Color DefaultHighlightColor = {0xff,0xff,0xff,0xff};
		SDL_Color AltHighlightColor = {0x00,0xff,0xff,0xff};
		SDL_Color ErrorTextColor = {0xff,0x00,0x00,0xff};
		SDL_Color ErrorBorderColor = {0xc0,0x00,0x00,0xff};
		SDL_Color PixelGridColor = {0x20,0x20,0x20,0xff};
		std::vector<std::string> mHelpText;
		std::vector<std::string> mHelpTextMap;
		std::vector<std::string> mHelpTextTile;
		int mLastTick = 0;
		int mCurrentTick = 0;
		int initSettings();
		void initHelpText();
		void printHelpText();		
		int initTicks();
		int updateTicks();
		int getTicks();
};

enum {
	EMODE_MAP,
	EMODE_TILE
} emodes;

enum {
	ACTION_EMPTY,
	ACTION_TILE,
	ACTION_PIXEL,
	ACTION_TILENEW,
	ACTION_TILEDROP,
	ACTION_PIXELS,
	ACTION_TILES
} actions;



#endif
//TSettings mGlobalSettings;