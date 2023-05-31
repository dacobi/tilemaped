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

#include "imgui.h"
#include "imgui_internal.h"

#define SCREEN_WIDTH 1900
#define SCREEN_HEIGHT 1000

#include "version.h"

//#define MAPPIMAGE

#ifdef _WIN64
#define MWIN64
#endif


#ifndef MWIN64
#define MNIXHOME
#include <stdlib.h>
#endif



namespace fs = std::filesystem;

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

#include "TSettings.h"
#include "TEAction.h"
#include "TileMap.h"
#include "TEditor.h"
#include "TDialogs.h"


TSettings mGlobalSettings;

#ifdef MNIXHOME
	std::string mINIPath = "~/.tilemaped/tilemaped.ini";
#else
	std::string mINIPath = "tilemaped.ini";
#endif

bool TSettings::getSpriteFileHeader(std::string sPath, int &cSpriteX, int &cSpriteY, int &cSpriteBPP, std::vector<unsigned char> &sBuf){

	fs::path cSpritePath = sPath;

	if((fs::exists(fs::status(cSpritePath)))  && !(fs::is_directory(fs::status(cSpritePath)))){
		
		std::ifstream infile(cSpritePath, std::ios::binary );
    	std::vector<unsigned char> tbuffer(std::istreambuf_iterator<char>(infile), {});

		int magic1,magic2;

		magic1 = tbuffer[0];
		magic2 = tbuffer[1];

		tbuffer.erase(tbuffer.begin());
		tbuffer.erase(tbuffer.begin());
	
		int nSpriteX, nSpriteY, nSpriteBPP;

		nSpriteX = (magic1 & 0xF0) >> 4;
		nSpriteY = (magic1 & 0x0F);

		nSpriteBPP = magic2;

		if( ( (nSpriteBPP == 4) || (nSpriteBPP == 8) ) && ( (nSpriteX >= 0) && (nSpriteX <= 3) ) && ( (nSpriteY >= 0) && (nSpriteY <= 3) ) ){

			int cTexX, cTexY;
			cTexX = mFrameSizeIn[nSpriteX];
			cTexY = mFrameSizeIn[nSpriteY];

			if((tbuffer.size() % ((cTexY*cTexX)/ mTileBPPSize[nSpriteBPP])) == 0){
				cSpriteX = cTexX;
				cSpriteY = cTexY;
				cSpriteBPP = nSpriteBPP;
				sBuf = tbuffer;
				return true;
			}
		}
	}

	return false;

}

int TSettings::testProjectFolder(std::string cPath){
	fs::path pPath = cPath;

	if((fs::exists(fs::status(pPath)))  && (fs::is_directory(fs::status(pPath)))){
		
		fs::path mPath = cPath  + DIRDEL + "map.bin";
		
		if(fs::exists(fs::status(mPath))){
			return 0;
		}
		
		mPath = cPath  + DIRDEL + "map0.bin";

		if(fs::exists(fs::status(mPath))){
			fs::path tPath = cPath  + DIRDEL + "tiles.bin";
			if(fs::exists(fs::status(tPath))){
				return 1;
			}
		}
	}

	return 0;
}

int TSettings::testPaletteFile(std::string palPath){

	fs::path tPath = palPath;

	if((fs::exists(fs::status(tPath)))  && !(fs::is_directory(fs::status(tPath)))){
		if(tPath.extension() == ".gpl"){
			std::cout << "Gimp palette found" << std::endl;
			return 2;
		} else {
			std::ifstream infile(tPath, std::ios::binary );
    		std::vector<unsigned char> tbuffer(std::istreambuf_iterator<char>(infile), {});

			int magic1,magic2;

			magic1 = tbuffer[0];
			magic2 = tbuffer[1];

			tbuffer.erase(tbuffer.begin());
			tbuffer.erase(tbuffer.begin());
	
			if((magic1 == 16) && (magic2 == 42) && (tbuffer.size() == 512)){
				std::cout << "X16 palette found" << std::endl;
				return 1;
			}
		}	
	}
	return 0;
}


int TSettings::initSettings(){

	std::string TTFPATH;
#ifdef MAPPIMAGE
	TTFPATH = std::string(getenv("TTFPATH"));
	std::cout << "ENV: " << TTFPATH << std::endl;
	TTFPATH += "/opt/tilemaped/nerdfont.ttf";
	std::cout << "ADD: " << TTFPATH << std::endl;
	#define NERDFONT TTFPATH.c_str()
#else
#define NERDFONT "nerdfont.ttf"
#endif

	
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 ){
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		return 1;
	}

    SDL_WindowFlags window_flags;
		
	if(bRenderingD3D || bSoftwareRendering){
		window_flags = (SDL_WindowFlags)( SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE); // SDL_WINDOW_ALLOW_HIGHDPI |
	} else {		
		window_flags = (SDL_WindowFlags)( SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	}

	WindowWidth = SCREEN_WIDTH;
	WindowHeight = SCREEN_HEIGHT;

	SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1");

	TWindow = SDL_CreateWindow( "TilemapEd", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags);
	if( TWindow == NULL ){
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	Uint32 mFlags = SDL_RENDERER_TARGETTEXTURE;

	if(bSoftwareRendering){
		mFlags |= SDL_RENDERER_SOFTWARE;
		std::cout << "Software Rendering"<< std::endl;
	} else {		
		mFlags |= SDL_RENDERER_ACCELERATED;
		if(bRenderingD3D){
			SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d");			
		} else {
			SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");			
		}
		std::cout << "Accelerated Rendering: "<< bRenderingD3D << std::endl;
	}

	if(bVSync){
		mFlags |= SDL_RENDERER_PRESENTVSYNC;
	} else {
		std::cout << "No VSYNC"<< std::endl;
	}

	TRenderer = SDL_CreateRenderer( TWindow, -1, mFlags);
	if( TRenderer == NULL ){
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_RendererInfo rendererInfo;
  	SDL_GetRendererInfo(TRenderer, &rendererInfo);
  	std::cout << "Renderer: " <<  rendererInfo.name << std::endl;
		
	SDL_SetRenderDrawColor( TRenderer, mGlobalSettings.DefaultHighlightColor.r,mGlobalSettings.DefaultHighlightColor.g,mGlobalSettings.DefaultHighlightColor.b,0xff);

	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) ){
		std::cout << "SDL_image Error: " << IMG_GetError() << std::endl;
		return 1;
 	}
	
 	//TTF_Init();
	//TFont =  TTF_OpenFont(NERDFONT,20);

	/*
	if(TFont == NULL){
		std::cout << "SDL_TTF Error: " << TTF_GetError() << std::endl;
		return 1;
	}

	LFont =  TTF_OpenFont(NERDFONT,30);	
	if(LFont == NULL){
		std::cout << "SDL_TTF Error: " << TTF_GetError() << std::endl;
		return 1;
	}
	UFont =  TTF_OpenFont(NERDFONT,20);	
	if(UFont == NULL){
		std::cout << "SDL_TTF Error: " << TTF_GetError() << std::endl;
		return 1;
	}
	
	TTF_SetFontStyle(LFont, TTF_STYLE_UNDERLINE);
	TTF_SetFontStyle(UFont, TTF_STYLE_UNDERLINE);
	*/
	
	SDL_SetWindowResizable(TWindow, SDL_TRUE);

	initHelpText();
	initTicks();

	int cWin = SDL_GetWindowDisplayIndex(TWindow);
	SDL_DisplayMode mCurDispMode;

	SDL_GetCurrentDisplayMode(cWin, &mCurDispMode);

	if(mCurDispMode.w <= 1920){
		mGlobalTexParam.mTileEdScale--;		
	}

	if(bMaximize){
		SDL_MaximizeWindow(TWindow);		
	}

	srand(time(0));

	IMGUI_CHECKVERSION();
    ImGuiContext* mg = ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
	mio = &io;

	mg->ConfigNavWindowingKeyNext = 0; //ImGuiMod_Ctrl | ImGuiKey_Tab;
	mg->ConfigNavWindowingKeyPrev = 0; //ImGuiMod_Ctrl | ImGuiMod_Shift | ImGuiKey_Tab;

	ImVector<ImWchar> ranges;
	ImFontGlyphRangesBuilder builder;
	
	builder.AddChar(0xf2d0);
	builder.AddChar(0xe240); 
	builder.AddChar(0xf120); 
	builder.AddChar(0xf15c); 
	builder.AddChar(0xf405); 
	builder.AddChar(0xf449); 
	builder.AddChar(0xf71e);
	builder.AddChar(0xf705);
	builder.AddChar(0xf013);
	builder.AddChar(0xf111);

	builder.AddRanges(io.Fonts->GetGlyphRangesDefault()); // Add one of the default ranges
	builder.BuildRanges(&ranges);       

	DFont = mio->Fonts->AddFontFromFileTTF(NERDFONT, 25.0,  NULL, ranges.Data);
	mio->Fonts->Build();

	SFont = mio->Fonts->AddFontFromFileTTF(NERDFONT, 20.0,  NULL, ranges.Data);
	mio->Fonts->Build();
	
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(mGlobalSettings.TWindow, mGlobalSettings.TRenderer);
    ImGui_ImplSDLRenderer_Init(mGlobalSettings.TRenderer);

	mio->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	mio->ConfigFlags |= ImGuiConfigFlags_NavNoCaptureKeyboard;
	
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

void TSettings::close(){	

	TileSetDefaultScale=10;
	TileMapScale=3;
	TileMapHeight=128;
	TileMapWidth=128;		
	
	mGlobalTexParam.TileSizeX=16;
	mGlobalTexParam.TileSizeY=16;
	mGlobalTexParam.TileSetBPP=8;
	mGlobalTexParam.TilePixelSize=16;	
	mGlobalTexParam.PaletteOffset=0;
	mGlobalTexParam.mTileEdScale = 4;
	mGlobalTexParam.TileRenderSize=16;
	
	


	bShowPaletteOffset = false;
	
	PaletteScale=2;
	ProjectPath = "";
	
	ProjectPalettePath = "";	
	bProjectHasPalette = false;
	ProjectPalette.clear();
	CurrentEditor->mPalette.close();

	mEditorState = 0;

	mNewFramePath = "";
	mNewFramesPath = "";

	mNewSpritePath = "";
	mNewSpriteType = 0;
	mNewSpriteX = 0;
	mNewSpriteY = 0;
	mNewSpriteBPP = 8;
	
	mNewTileMapPath = "";
	mNewTileMapPaletteOffset = 0;
	mNewTileMapOffset = 0;
	mNewTileMapState = 0;
	mNewTileMapX = 0;
	mNewTileMapY = 0;
	mNewTilePath = "";
	mNewTileSize = 0;
	mDeleteTileMapState = 0;			
	mTileMapFileCount = 0;			
	bShowTypeSelection = false;
	bShowPixelGrid = true;
	bShowPixelGridSprite = true;
	bShowTileGrid = true;
	bShowTilePixelGrid = true;
	bShowTileSelGrid = true;
	bShowTilePixelSelGrid = true;
	bTileSetOrderUpdateTileMap = true;
	bTileSetWarnBeforeDelete = true;
	bSpriteWarnBeforeDelete = true;
	mTileSetEditWidth = 4;
	mTileSetEditScale = 10;		
	mSelectionEditScale = 10;
	bShowPixelType = false;
	bShowPixelTypeSprite = false;
	bShowSelectedTile = true;
	bShowProjectInfo = false;		
	bShowHelpDialog = false;		
	mSelectedTile = 0;
	
	bSelectionMode = true;
	mUseTextureFiltering = 0;

	mProjectSettings.close();
}

void TSettings::shutdown(){	
	SDL_DestroyRenderer(TRenderer);
	std::cout << "SDL_DestroyRenderer(TSettings::TRenderer)" << std::endl;
}

void TSettings::settingsMenu(){
	if (ImGui::BeginMenu(std::string(mGear + " Settings").c_str())){
			ImGui::Text("Renderer");
			ImGui::RadioButton("OpenGL", &mINIFile.Sys_Renderer->ivalue, 0);
#ifdef MWIN64
			ImGui::RadioButton("DirectX", &mINIFile.Sys_Renderer->ivalue, 1);
#endif
			ImGui::RadioButton("Software", &mINIFile.Sys_Renderer->ivalue, 2);
			ImGui::Checkbox("VSYNC", &mINIFile.Sys_VSYNC->bvalue);
			ImGui::Separator();
			ImGui::Text("Window");
			ImGui::Checkbox("Maximize", &mINIFile.Win_Maximize->bvalue);
			ImGui::EndMenu();
	}
}

int TSettings::runOCD(int mode){

	OCDialog mOpenCreate;
	mOpenCreate.init();

	if(mode == ESTATE_PROJECTOPEN){
		mOpenCreate.bSubDialogActive = true;
		mOpenCreate.bSubDialogIsOpen = true;
		mOpenCreate.bDialogIsWatingForText = true;				
	}

	if(mode == ESTATE_PROJECTCREATE){
		mOpenCreate.bSubDialogActive = true;
		mOpenCreate.bSubDialogIsCreate = true;
		mOpenCreate.bDialogIsWatingForText = true;				
	}

	while( bRunningOCD ){

		if((mEditorState == ESTATE_PROJECTOPEN) || ( mEditorState == ESTATE_PROJECTCREATE)){
			bRunningOCD = false;
		}

		if(mOpenCreate.bInputIsCancel){
			bRunningOCD = false;
			//mProjectOpenState = 0;
			mEditorState = ESTATE_NONE;
		}

		SDL_SetRenderDrawColor( TRenderer, DefaultBGColor.r,  DefaultBGColor.g,  DefaultBGColor.b, 0xff); 
		SDL_RenderClear( TRenderer );

       	ImGui_ImplSDLRenderer_NewFrame();
       	ImGui_ImplSDL2_NewFrame();
       	ImGui::NewFrame();

		mOpenCreate.render();
		ImGui::Render();                						

       	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
 
		SDL_RenderPresent( TRenderer );

		SDL_Event e;

		while( SDL_PollEvent( &e ) != 0 ){
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type){
				case SDL_QUIT:
						bRunningOCD = false;
					break;
				case SDL_TEXTINPUT:
					
				break;
				case SDL_KEYDOWN:							
					if(e.key.keysym.sym == SDLK_BACKSPACE){
 						if(mOpenCreate.bDialogIsWatingForText){				
							mOpenCreate.dropLastInputChar();
  						}
					}
					if(e.key.keysym.sym == SDLK_RETURN){
						mOpenCreate.recieveInput(SDLK_y);
					}
					if(e.key.keysym.sym == SDLK_ESCAPE){
						mOpenCreate.recieveInput(SDLK_n);
					}
					if(e.key.keysym.sym == SDLK_TAB){
						mOpenCreate.recieveInput(SDLK_TAB);
					}
				break;
				case SDL_MOUSEBUTTONDOWN:
	   				
				break;
				case SDL_WINDOWEVENT:
				if (e.window.event == SDL_WINDOWEVENT_RESIZED){
					int newWidth,newHeight;
 					SDL_GetWindowSize(TWindow, &newWidth, &newHeight);
 					WindowWidth = newWidth;
 					WindowHeight = newHeight; 	
				}
			break;
			}
		}
		updateTicks();			
	}
	return 0;
}



void TSettings::initHelpText(){

	mHelpText.push_back("WINDOWS: TilemapEd has 5 main windows which are TileMap Editor, Tile Editor\nPalette Editor, TileSet Editor and Selection Editor.\nSelect Window in View menu and press (SPACE) to quickly switch back to the last active window.\nPress (F2) to view the Project Info dialog. When working with Tile BPPs\nlower than 8 the Palette Offset dialog can be shown/hidden in the View menu.");	
	mHelpText.push_back("UNDO: Most changes to TileMap(s) and Tiles can be undone. Press (U) to undo\nand (R) to redo action. Some operations will clear the Undo Stack.");
	mHelpText.push_back("SELECTION: Tiles and Pixels can be selected using the mouse. Hold (LEFT SHIFT)\nand (LEFT MOUSE BUTTON) to select a range by dragging.\nHold (LEFT SHIFT) and press (RIGHT MOUSE BUTTON)\nto modify Selection on a Tile/Pixel basis.\nPress (A) to select All, (N) to select None and (I) to Invert selection.");
	mHelpText.push_back("BRUSHES: Press (F8) to open the Brush List, then select <width> and <height>\nand Press <Add Brush>. Press (RIGHT MOUSE BUTTON) in TileSet or Palette to add Brush Elements.\nPress (F9) to activate (next) Brush and (F10) to drop selected Brush.");
	mHelpText.push_back("INPUT: When entering a value using a Slider Widget hold (LEFT CONTROL) and press\n(LEFT MOUSE BUTTON) on the widget to turn it into a TextInput Widget.");
	mHelpText.push_back("INI FILE: When using the CLI arguments \n\"--opengl, --d3d, --software, --window, --maximize, --vsync, --novsync\"\nthe chosen options will be saved to \""+mINIPath+"\"\nand used until other options are applied.");

	mHelpTextMap.push_back("PLACE TILE: To place a Tile in the TileMap press (LEFT MOUSE BUTTON) to select\na Tile in the TileSet and then (LEFT MOUSE BUTTON) to place it in the TileMap.");
	mHelpTextMap.push_back("SELECT TILE: Press (RIGHT MOUSE BUTTON) to select a Tile in the TileMap.\nPress (S) to show all Tiles of the Selected Tile Type.\nPress (RIGHT MOUSE BUTTON) on a Tile in the TileSet\nto replace all selected Tiles in the TileMap.");
	mHelpTextMap.push_back("SCROLL/SCALE: Use (MOUSE SCROLL WHEEL) to scale the TileMap and Scroll the TileSet.\nHold (LEFT CONTROL) and (LEFT MOUSE BUTTON) to move the TileMap.");	
	mHelpTextMap.push_back("TILESET: Press (F3) to create an empty Tile. Press (F4) to create a copy of the selected Tile.\nPress (F5) to Rotate Tile Left. Press (F6) to Rotate Tile Right.\nPress (DEL) to Remove the Selected Tile if it's not used in any TileMap.\nOr Select \"Remove Unused Tiles\" in the Edit menu to remove all Tiles not used in any TileMap.\nTiles can be reordered using drag and drop or using the <Move Up>/<Move Down> buttons.\nHolding (LEFT CONTROL) while dragging will replace the Target Tile with a copy of the Source.");
	mHelpTextMap.push_back("FLIP TILE: Press (F) to cycle through flip values. Press (X) to flip the selected Tile\nHorizontally and (Y) to flip Vertically.");
	mHelpTextMap.push_back("PALETTE OFFSET: Tiles from the TileSet will have the current Palette Offset.");

	
	mHelpTextTile.push_back("PLACE PIXEL: To place a pixel in the Tile press (LEFT MOUSE BUTTON) to select\na color in the Palette and then (LEFT MOUSE BUTTON) to place it in the Tile.");	
	mHelpTextTile.push_back("SELECT PIXEL: Press (RIGHT MOUSE BUTTON) to select a color in the Tile.\nPress (S) to show all Pixels of the Selected color.\nPress (RIGHT MOUSE BUTTON) on a color in the Palette\nto replace all selected Pixels in the Tile.");	
	mHelpTextTile.push_back("GRID: Press (P) to show Pixel Grid in Tile Editor.");

	mHelpTextTileSet.push_back("EDITOR: The TileSet Editor works much like the Tile Editor, only on the entire TileSet.\nYou can set the Tile Grid Width and Tile Scale in the Edit menu under \"TileSet Edit\"\nor Use (MOUSE WHEEL). Use (LEFT CONTROL) and (LEFT MOUSE BUTTON) to Scroll the TileSet.");
	mHelpTextTileSet.push_back("GRID: Press (P) to show Pixel Grid in TileSet Editor and Press (T) to show the Tile Grid.");

	mHelpTextSelection.push_back("EDITOR: The Selection Editor works much like the TileSet Editor, only without Pixel selection.\nTo enter Selection Editor you must make a Rectangular Selection in a TilaMap of min 2x2\nand max 16x16 Tiles. Then choose Selection in the View menu.\nIf the Selection is not a Rectangle of min 2x2 and max 16x16 an Error Message will be shown.\nYou can set the Tile Scale in the Edit menu under \"Selection Edit\" or Use (MOUSE WHEEL).\nUse (LEFT CONTROL) and (LEFT MOUSE BUTTON) to Scroll the Selection.");
	mHelpTextSelection.push_back("GRID: Press (P) to show Pixel Grid in Selection Editor and Press (T) to show the Tile Grid.");

	mHelpTextPalette.push_back("CHANGE COLOR: Press (LEFT MOUSE BUTTON) to select a Color in the Palette Editor.\nUse RGB sliders or Press (LEFT MOUSE BUTTON) on the Pick Selected Color Box.");
	mHelpTextPalette.push_back("COPY COLOR: Press (RIGHT MOUSE BUTTON) on a Color to copy its value to the selected Color.");
	mHelpTextPalette.push_back("IMPORT: Select <Import Palette> in the Edit menu to import a palette file.\nGimp palettes and TilemapEd palettes are supported.");
	mHelpTextPalette.push_back("APPLY/CANCEL: Press <Apply Changes> to update the Project Palette.\nPress <Cancel Changes> to revert the Palette Editor to the Project Palette.");

	mHelpTextColMapEditor.push_back("EDITOR: Click \"Edit->CollisionMap->Create\" to create a CollisionMap\nfor the current TileMap. Then press (LEFT MOUSE BUTTON) on a Tile to select it.");
	mHelpTextColMapEditor.push_back("VALUES: Use the <Collision Value> Slider Widget to set the \"Collision Value\" of the\nselected Tile. Press (RIGHT MOUSE BUTTON) on a Tile to select it and set its \"Collision Value\"\nto the value of the previously selected Tile. Click <Set All Values>\nto set the \"Collision Value\" of all Tiles to the value of the currently selected Tile.");

	mHelpTextImport.push_back("TILE: Tiles can be imported from PNG and RAW, when BPP is 8 or 4.\nPNGs must have BPP 8 and Height/Width equal to TileSizeX/TileSizeY.\nRAW binary files must have filesize of (TileSizeX*TileSizeY) divided by pixels pr byte.\nIf BPP is 4, Tile pixel values from PNG will be \"pixelvalue % 16\"");
	mHelpTextImport.push_back("TILESET: TileSets can be imported from Project file or PNG.\nProject files must have the same BPP as current project.\nPNGs must have Width and Height where \"Width % TileSizeX\" equals zero.\nIf BPP is 4, Tile pixel values from PNG will be \"pixelvalue % 16\"."); 
	mHelpTextImport.push_back("TILEMAP: TileMaps can be imported from Project files. A Tile value offset can be specified.\nThe final TileMap must not have any Tile values larger than the current TileSet.\nTo Import an existing TileMap with its TileSet, first Import the TileSet.\nThen open Project Info and take note of the Tile number of the first Tile in the TileSet.\nThen Import the TileMap with Tile offset set to the given Tile number.");

	
}

void printUsage(){
		std::cout << std::endl;	
		std::cout << "TilemapEd Version: " << TilemapEd_Version  << std::endl;
		std::cout << std::endl;	
		std::cout << "Command Line Usage:" << std::endl;
		std::cout << "tilemaped [ --opengl, --d3d, --software, --window, --maximize, --vsync, --novsync ] (options are saved to \""+mINIPath+"\")" << std::endl;		
		std::cout << "tilemaped -o <folder>" << std::endl;		
		std::cout << "tilemaped -n <mapwidth> <mapheight> <tilewidth> <tileheight> <folder> [ -p <palette file> ]" << std::endl;
		std::cout << "tilemaped -c <Gimp Palette> <palfile.bin>" << std::endl;		
}


void TSettings::printHelpText(){
	initHelpText();


	
	std::cout << "General:" << std::endl;	

	for(const auto& cStr : mHelpText){
		std::cout << cStr << std::endl;	
	}

	std::cout  << std::endl;	
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
	std::cout << "TileSet Editor:" << std::endl;	
	
	for(const auto& cStr : mHelpTextTileSet){
		std::cout << cStr << std::endl;	
	}

	std::cout  << std::endl;	
	std::cout << "Selection Editor:" << std::endl;	
	
	for(const auto& cStr : mHelpTextSelection){
		std::cout << cStr << std::endl;	
	}

	std::cout  << std::endl;		
	std::cout << "Palette Editor:" << std::endl;	

	for(const auto& cStr : mHelpTextPalette){
		std::cout << cStr << std::endl;	
	}

	std::cout  << std::endl;		
	std::cout << "CollisionMap:" << std::endl;	

	for(const auto& cStr : mHelpTextColMapEditor){
		std::cout << cStr << std::endl;	
	}

	std::cout  << std::endl;	

	std::cout  << std::endl;		
	std::cout << "Import:" << std::endl;	

	for(const auto& cStr : mHelpTextImport){
		std::cout << cStr << std::endl;	
	}

	std::cout  << std::endl;	
	

	printUsage();
}

std::string argStringVal;

int parseArgs(int argc, char *argv[]){

	int argpos = 1;
	int returnval = 0;
	int looptime = 12;

	std::stringstream mConvert;
	int nTileSize = 0;
	int nMapSizeX = 0;
	int nMapSizeY = 0;	

	if(argc == 1){
		return 1;
	}

	if(argc == 2){
		if(std::string(argv[argpos]) == "-h"){
			return 0;
		}		
	}	
	
	while(argpos < argc){

		if(std::string(argv[argpos]) == "-o"){
			if((argc-argpos) >= 2){
				argpos++;
				if(fs::is_directory(fs::status(argv[argpos]))){
					std::cout << "Folder found" << std::endl;
					mGlobalSettings.ProjectPath = std::string(argv[argpos]); 						
					returnval += 2;
					argpos++;
					continue;	
				} else {
					std::cout << "Folder not found!" << std::endl;						
					return -1;
				}
			}
		} else if(std::string(argv[argpos]) == "-c"){ 
			if((argc-argpos) >= 3){
				argpos++;			
				if(mGlobalSettings.testPaletteFile(argv[argpos]) == 2){					
					mGlobalSettings.ProjectPalettePath = argv[argpos];
					argpos++;
					argStringVal = argv[argpos];					
					returnval += 0x20;
					argpos++;
					continue;
				} else { 
					return -1;
				}
			} else { 
				return -1;
			}
		} else 	if(std::string(argv[argpos]) == "-n"){
			if((argc-argpos) >= 6){										
				argpos++;
				mConvert << std::string(argv[argpos]) << std::endl;
				mConvert >> nMapSizeX;
		
				if((nMapSizeX == 32) || (nMapSizeX == 64) || (nMapSizeX == 128) || (nMapSizeX == 256)){				
					mGlobalSettings.TileMapWidth = nMapSizeX;						
				} else {
					std::cout << "Wrong TileMap Size!" << std::endl;
					std::cout << "Valid Values are: 32, 64, 128, 256" << std::endl;						
					return -1;
				}

				argpos++;
				mConvert << std::string(argv[argpos]) << std::endl;
				mConvert >> nMapSizeY;
		
				if((nMapSizeY == 32) || (nMapSizeY == 64) || (nMapSizeY == 128) || (nMapSizeY == 256)){	
					mGlobalSettings.TileMapHeight = nMapSizeY;
				} else {
					std::cout << "Wrong TileMap Size!" << std::endl;
					std::cout << "Valid Values are: 32, 64, 128, 256" << std::endl;						
					return -1;
				}			

				argpos++;
				mConvert << std::string(argv[argpos]) << std::endl;		
				mConvert >> nTileSize;
			
				if((nTileSize == 16) || (nTileSize == 8)){	
					mGlobalSettings.mGlobalTexParam.TileSizeX = nTileSize;	
				} else {
					std::cout << "Wrong TileSize!" << std::endl;
					std::cout << "Valid Values are: 8, 16" << std::endl;						
					return -1;
				}

				argpos++;
				mConvert << std::string(argv[argpos]) << std::endl;		
				mConvert >> nTileSize;
			
				if((nTileSize == 16) || (nTileSize == 8)){	
					mGlobalSettings.mGlobalTexParam.TileSizeY = nTileSize;	
				} else {
					std::cout << "Wrong TileSize!" << std::endl;
					std::cout << "Valid Values are: 8, 16" << std::endl;						
					return -1;
				}

				argpos++;
				if(fs::exists(fs::status(argv[argpos]))){
					std::cout << "Error Folder Exists! " << std::endl;						
					return -1;
				} else {
					mGlobalSettings.ProjectPath = std::string(argv[argpos]);						
				}
				
				returnval += 4;
				argpos++;

				if(argpos == argc){	continue; }	
				
				if(std::string(argv[argpos]) == "-p"){
					argpos++;
					if(argpos == argc){					
						return -1;
					}	
					if((fs::exists(fs::status(argv[argpos]))) && !(fs::is_directory(fs::status(argv[argpos])))){
						std::cout << "Palette found" << std::endl;						
						mGlobalSettings.bProjectHasPalette = true;
						mGlobalSettings.ProjectPalettePath = std::string(argv[argpos]);
						argpos++;
						continue;							
					} else {
						std::cout << "Palette file not found!" << std::endl;						
						return -1;
					}
				}								
			} 
		} else if(std::string(argv[argpos]) == "--software"){
			argpos++;
			if(!(returnval & 0x8))	returnval += 8;
			continue;			
		} else 	if(std::string(argv[argpos]) == "--novsync"){
			argpos++;
			if(!(returnval & 0x10)) returnval += 16;
			continue;	
		} else 	if(std::string(argv[argpos]) == "--d3d"){
			argpos++;
#ifdef MWIN64
			if(!(returnval & 0x40))	returnval += 64;
#endif
			continue;	
		} else if(std::string(argv[argpos]) == "--opengl"){
			argpos++;
			if(!(returnval & 0x80))	returnval += 0x80;
			continue;	
		} else 	if(std::string(argv[argpos]) == "--vsync"){
			argpos++;
			if(!(returnval & 0x100)) returnval += 0x100;
			continue;	
		} else if(std::string(argv[argpos]) == "--window"){
			argpos++;
			if(!(returnval & 0x200)) returnval += 0x200;
			continue;	
		} else if(std::string(argv[argpos]) == "--maximize"){
			argpos++;
			if(!(returnval & 0x400)) returnval += 0x400;
			continue;	
		}


		
		looptime--;
		if(looptime == 0){			
			return -1;
		}
	}
	return returnval;

}


int main( int argc, char* args[] )
{
	TEditor mEditor;
	
	bool mCreateNewProject=false;
	bool bRunSoftware = false;
	bool bVsync = true;
	bool bRenderD3D = false;
	bool bMaximize = false;
	
#ifdef MNIXHOME
	fs::path inipath = std::string(getenv("HOME")) + DIRDEL + ".tilemaped";
	if(!fs::is_directory(fs::status(inipath))){		
		try{
			fs::create_directory(inipath);			
		} catch(...){
			std::cout << "Error Creating Folder: ~/.tilemaped" << std::endl;
			return 1;
		}
	}
	if(fs::exists(fs::status(inipath.string()+DIRDEL+"tilemaped.ini"))){
		mGlobalSettings.mINIFile.load(inipath.string()+DIRDEL+"tilemaped.ini");		
	}
#else
	if(fs::exists(fs::status(mINIPath))){
		mGlobalSettings.mINIFile.load(mINIPath);		
	}
#endif

#ifndef MWIN64
		if(mGlobalSettings.mINIFile.Sys_Renderer->ivalue == 1){
			mGlobalSettings.mINIFile.Sys_Renderer->ivalue = 0;			
		}
#endif

	switch(mGlobalSettings.mINIFile.Sys_Renderer->ivalue)
	{
	case 0:
		break;
	case 1:
		bRenderD3D = true;
		break;
	case 2:
		bRunSoftware = true;
		break;			
	default:
		break;
	}

	if(mGlobalSettings.mINIFile.Sys_VSYNC->bvalue == false){
		bVsync = false;		
	}

	if(mGlobalSettings.mINIFile.Win_Maximize->bvalue){
		bMaximize = true;		
	}

	int argvals = parseArgs(argc, args);

	if(argvals == -1){
		printUsage();
		return 1;
	}

	if(argvals == 0){
		mGlobalSettings.printHelpText();
		return 1;
	}

	if(argvals & 0x20){
		mEditor.mPalette.importGimpPalette(mGlobalSettings.ProjectPalettePath);
		mEditor.mPalette.saveToFile(argStringVal);
		std::cout << "Palette Exported to: " << argStringVal << std::endl;						
		return 0;
	}

	if(!(argvals & 0x2) && !(argvals & 0x4) ){ 
		mGlobalSettings.bRunningOCD = true;		
	}

	if((argvals & 0x2)){ 
		//Such empty
	}

	if((argvals & 0x4)){ 
		mCreateNewProject = true;		
	}

	if(argvals & 0x8){
		bRunSoftware = true;
		bRenderD3D = false;
	}
	if(argvals & 0x10){
		bVsync = false;
	}
	if(argvals & 0x40){
		bRenderD3D = true;
		bRunSoftware = false;
	}
	if(argvals & 0x80){
		bRunSoftware = false;
		bRenderD3D = false;
	}
	if(argvals & 0x100){
		bVsync = true;
	}
	if(argvals & 0x200){
		bMaximize = false;
	}
	if(argvals & 0x400){
		bMaximize = true;
	}

	mGlobalSettings.mINIFile.Sys_VSYNC->bvalue = bVsync;// ? 1 : 0;

	mGlobalSettings.mINIFile.Sys_Renderer->ivalue = 0;

	if(bRunSoftware){
		mGlobalSettings.mINIFile.Sys_Renderer->ivalue = 2;
	}

	if(bRenderD3D){
		mGlobalSettings.mINIFile.Sys_Renderer->ivalue = 1;
	}

	mGlobalSettings.mINIFile.Win_Maximize->bvalue = bMaximize;// ? 1 : 0;
	
	mGlobalSettings.bSoftwareRendering = bRunSoftware;
	mGlobalSettings.bRenderingD3D = bRenderD3D;
	mGlobalSettings.bVSync = bVsync;
	mGlobalSettings.bMaximize = bMaximize;

	if( mGlobalSettings.initSettings() ){
		std::cout << "SDL Init Failed!" << std::endl;
	}
	else
	{	
		mGlobalSettings.CurrentEditor = &mEditor;
		SDL_Event e;

		if(!mGlobalSettings.bRunningOCD) {

			if(mCreateNewProject){
				if(mEditor.createNewProject()){
					mEditor.bEditorRunning = false;
				}		
			} else {
				if(mGlobalSettings.testProjectFolder(mGlobalSettings.ProjectPath)){
					if(mEditor.loadFromFolder(mGlobalSettings.ProjectPath)){
						mEditor.bEditorRunning = false;
					}
				} else {
					std::cout << "Error in Project Folder: " << mGlobalSettings.ProjectPath << std::endl;
					mEditor.bEditorRunning = false;
					//return 1;
				}
			}
		
		}


		while( mEditor.bEditorRunning || mGlobalSettings.bRunningOCD){

			if(mGlobalSettings.bRunningOCD){			
				mGlobalSettings.runOCD(mGlobalSettings.mOpenCreateProjectState); //);
			
				mGlobalSettings.mOpenCreateProjectState = ESTATE_NONE;

				if(mGlobalSettings.mEditorState == ESTATE_PROJECTOPEN){
					if(mEditor.loadFromFolder(mGlobalSettings.ProjectPath)){
						mEditor.bEditorRunning = false;						
					}
					mEditor.bEditorRunning = true;
				} else if(mGlobalSettings.mEditorState == ESTATE_PROJECTCREATE){
					if(mEditor.createNewProject()){
						mEditor.bEditorRunning = false;
					}
					mEditor.bEditorRunning = true;
				} else {
					mEditor.bEditorRunning = false;
				}
			} 

			while( mEditor.bEditorRunning ){

				
				SDL_SetRenderDrawColor(  mGlobalSettings.TRenderer, mGlobalSettings.DefaultBGColor.r,  mGlobalSettings.DefaultBGColor.g,  mGlobalSettings.DefaultBGColor.b, 0xff); 
				SDL_RenderClear( mGlobalSettings.TRenderer );

        		ImGui_ImplSDLRenderer_NewFrame();
        		ImGui_ImplSDL2_NewFrame();
        		ImGui::NewFrame();

				mEditor.render();
				ImGui::Render();                
						
        		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
 
				SDL_RenderPresent( mGlobalSettings.TRenderer );

				while( SDL_PollEvent( &e ) != 0 ){
					ImGui_ImplSDL2_ProcessEvent(&e);
					mEditor.handleEvents(&e);				
				}
				mEditor.handleEvents();
			}			
			mEditor.closeProject();
		}

		mGlobalSettings.shutdown();		
	}
	
#ifdef MNIXHOME
	mGlobalSettings.mINIFile.writedefault(inipath.string()+DIRDEL+"tilemaped.ini");
#else
	mGlobalSettings.mINIFile.writedefault(mINIPath);
#endif
	
	return 0;
}
