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

//#define MAPPIMAGE

#include "version.h"

#ifdef MAPPIMAGE
#include <stdlib.h>
#endif



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

int TSettings::testProjectFolder(std::string cPath){
	fs::path pPath = cPath;

	if((fs::exists(fs::status(pPath)))  && (fs::is_directory(fs::status(pPath)))){
		fs::path mPath = cPath  + DIRDEL + "map.bin";
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
		
	if(!bRenderingD3D){
		window_flags = (SDL_WindowFlags)( SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
	} else {
		window_flags = (SDL_WindowFlags)( SDL_WINDOW_SHOWN );
	}


	TWindow = SDL_CreateWindow( "Tilemaped", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WindowWidth, WindowHeight, window_flags);
	if( TWindow == NULL ){
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	//SDL_SetWindowSize(TWindow, WindowWidth, WindowHeight);

	Uint32 mFlags = 0;

	if(bSoftwareRendering){
		mFlags += SDL_RENDERER_SOFTWARE;
		std::cout << "Software Rendering"<< std::endl;
	} else {		
		mFlags += SDL_RENDERER_ACCELERATED;
		if(bRenderingD3D){
			SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d");
		} else {
			SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
		}
		std::cout << "Accelerated Rendering: "<< bRenderingD3D << std::endl;
	}

	if(!bNoVSync){
		mFlags += SDL_RENDERER_PRESENTVSYNC;
	} else {
		std::cout << "No VSYNC"<< std::endl;
	}

	TRenderer = SDL_CreateRenderer( TWindow, -1, mFlags);// SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
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
	
 	TTF_Init();
	TFont =  TTF_OpenFont(NERDFONT,20);

	
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

	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
	mio = &io;

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

	builder.AddRanges(io.Fonts->GetGlyphRangesDefault()); // Add one of the default ranges
	builder.BuildRanges(&ranges);       

	DFont = mio->Fonts->AddFontFromFileTTF(NERDFONT, 25.0,  NULL, ranges.Data);
	mio->Fonts->Build();

	SFont = mio->Fonts->AddFontFromFileTTF(NERDFONT, 20.0,  NULL, ranges.Data);
	mio->Fonts->Build();
	
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(mGlobalSettings.TWindow, mGlobalSettings.TRenderer);
    ImGui_ImplSDLRenderer_Init(mGlobalSettings.TRenderer);

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

void TSettings::shutdown(){	
	SDL_DestroyRenderer(TRenderer);
	std::cout << "SDL_DestroyRenderer(TSettings::TRenderer)" << std::endl;
}

int TSettings::runOCD(){

	OCDialog mOpenCreate;
	mOpenCreate.init();

	while( bRunningOCD ){

		if((mProjectOpenState == 1) || ( mProjectOpenState == 2)){
			bRunningOCD = false;
		}

		if(mOpenCreate.bInputIsCancel){
			bRunningOCD = false;
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
			}
		}
		updateTicks();			
	}
	return 0;
}



void TSettings::initHelpText(){

	mHelpText.push_back("WINDOWS: Tilemaped has 4 main windows which are TileMap Editor, Tile Editor\nPalette Editor and TileSet Editor. Select window in View menu and press (SPACE)\nto quickly switch back to the last active window.\nPress (F2) to view the Project Info dialog. When working with Tile BPPs\nlower than 8 the Palette Offset dialog can be shown/hidden in the View menu.");	
	mHelpText.push_back("UNDO: Most changes to TileMap and Tiles can be undone. Press (U) to undo\nand (R) to redo action. Some operations will clear the Undo Stack.\nIf you wish to keep an Action but undo previous Actions Press (D)\nto drop the Action from the Undo Stack.");
	mHelpText.push_back("SELECTION: Tiles and Pixels can be selected using the mouse. Hold (LEFT SHIFT)\nand (LEFT MOUSE BUTTON) to select a range by dragging.\nHold (LEFT SHIFT) and press (RIGHT MOUSE BUTTON)\nto modify Selection on a Tile/Pixel basis.\nPress (A) to select All, (N) to select None and (I) to Invert selection.");
	mHelpText.push_back("BRUSHES: Press (F8) to open the Brush List, then select <width> and <height>\nand Press <Add Brush>. Press (RIGHT MOUSE BUTTON) in TileSet or Palette to add Brush Elements.\nPress (F9) to activate (next) Brush and (F10) to drop selected Brush.");

	mHelpTextMap.push_back("PLACE TILE: To place a Tile in the TileMap press (LEFT MOUSE BUTTON) to select\na Tile in the TileSet and then (LEFT MOUSE BUTTON) to place it in the TileMap.");
	mHelpTextMap.push_back("SELECT TILE: Press (RIGHT MOUSE BUTTON) to select a Tile in the TileMap.\nPress (S) to show all Tiles of the Selected Tile Type.\nPress (RIGHT MOUSE BUTTON) on a Tile in the TileSet\nto replace all selected Tiles in the TileMap.");
	mHelpTextMap.push_back("SCROLL/SCALE: Use (MOUSE SCROLL WHEEL) to scale the TileMap and Scroll the TileSet.\nHold (LEFT CONTROL) and (LEFT MOUSE BUTTON) to move the TileMap.");	
	mHelpTextMap.push_back("TILESET: Press (F3) to create an empty Tile. Press (F4) to import a Tile from file.\nPress (F5) to create a copy of the selected Tile.\nPress (F6) to Rotate Tile. Press (F7) to remove all unused Tiles.");	
	mHelpTextMap.push_back("FLIP TILE: Press (F) to flip the selected Tile.");
	mHelpTextMap.push_back("PALETTE OFFSET: Tiles from the TileSet will have the current Palette Offset.");

	
	mHelpTextTile.push_back("PLACE PIXEL: To place a pixel in the Tile press (LEFT MOUSE BUTTON) to select\na color in the Palette and then (LEFT MOUSE BUTTON) to place it in the Tile.");	
	mHelpTextTile.push_back("SELECT PIXEL: Press (RIGHT MOUSE BUTTON) to select a color in the Tile.\nPress (S) to show all Pixels of the Selected color.\nPress (RIGHT MOUSE BUTTON) on a color in the Palette\nto replace all selected Pixels in the Tile.");	
	mHelpTextTile.push_back("GRID: Press (P) to show Pixel Grid in Tile Editor.");

	mHelpTextTileSet.push_back("EDITOR: The TileSet Editor works much like the Tile Editor, only on the entire TileSet.\nYou can set the Tile Grid Width and Tile Scale in the Edit menu under \"TileSet Edit\".\nUse (LEFT CONTROL) and (LEFT MOUSE BUTTON) to Scroll the TileSet.");
	mHelpTextTileSet.push_back("GRID: Press (P) to show Pixel Grid in TileSet Editor and Press (T) to show the Tile Grid.");

	mHelpTextPalette.push_back("CHANGE COLOR: Press (LEFT MOUSE BUTTON) to select a Color in the Palette Editor.\nUse RGB sliders or Press (LEFT MOUSE BUTTON) on the Pick Selected Color Box.");
	mHelpTextPalette.push_back("COPY COLOR: Press (RIGHT MOUSE BUTTON) on a Color to copy its value to the selected Color.");
	mHelpTextPalette.push_back("IMPORT: Select <Import Palette> in the Edit menu to import a palette file.\nGimp palettes and Tilemaped palettes are supported.");
	mHelpTextPalette.push_back("APPLY/CANCEL: Press <Apply Changes> to update the Project Palette.\nPress <Cancel Changes> to revert the Palette Editor to the Project Palette.");
}

void printUsage(){
		std::cout << std::endl;	
		std::cout << "TilemapEd Version: " << Tilemaped_Version  << std::endl;
		std::cout << std::endl;	
		std::cout << "Command Line Usage:" << std::endl;
		std::cout << "tilemaped [ --software --novsync ]" << std::endl;		
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
	std::cout << "Palette Editor:" << std::endl;	

	for(const auto& cStr : mHelpTextPalette){
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
					mGlobalSettings.TileSizeX = nTileSize;	
				} else {
					std::cout << "Wrong TileSize!" << std::endl;
					std::cout << "Valid Values are: 8, 16" << std::endl;						
					return -1;
				}

				argpos++;
				mConvert << std::string(argv[argpos]) << std::endl;		
				mConvert >> nTileSize;
			
				if((nTileSize == 16) || (nTileSize == 8)){	
					mGlobalSettings.TileSizeY = nTileSize;	
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
		} else 	if(std::string(argv[argpos]) == "--software"){
			argpos++;
			if(!(returnval & 0x8))	returnval += 8;
			continue;			
		} else 	if(std::string(argv[argpos]) == "--novsync"){
			argpos++;
			if(!(returnval & 0x10)) returnval += 16;
			continue;	
		} else 	if(std::string(argv[argpos]) == "--d3d"){
			argpos++;
			if(!(returnval & 0x40))	returnval += 64;
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
	//std::stringstream mConvert;
	//int nTileSize = 0;
	//int nMapSizeX = 0;
	//int nMapSizeY = 0;	
	bool mCreateNewProject=false;
	bool bRunSoftware = false;
	bool bNoVsync = false;
	bool bRenderD3D = false;
	
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

	if((argvals == 1) || (argvals == 8) || (argvals == 16) || (argvals == 24 ) || (argvals == 64)){
		mGlobalSettings.bRunningOCD = true;
		bRunSoftware = argvals & 0x8;
		bNoVsync = argvals & 0x10;
		bRenderD3D = argvals & 0x40;
	}

	if((argvals == 2) || (argvals == 10) || (argvals == 18) || (argvals == 26) || (argvals == 66)){
		bRunSoftware = argvals & 0x8;
		bNoVsync = argvals & 0x10;
		bRenderD3D = argvals & 0x40;
	}

	if((argvals == 4) || (argvals == 12) || (argvals == 20) || (argvals == 28) || (argvals == 68)){
		mCreateNewProject = true;
		bRunSoftware = argvals & 0x8;
		bNoVsync = argvals & 0x10;
		bRenderD3D = argvals & 0x40;
	}

	mGlobalSettings.bSoftwareRendering = bRunSoftware;
	mGlobalSettings.bRenderingD3D = bRenderD3D;
	mGlobalSettings.bNoVSync = bNoVsync;

	if( mGlobalSettings.initSettings() ){
		std::cout << "SDL Init Failed!" << std::endl;
	}
	else
	{	
		mGlobalSettings.CurrentEditor = &mEditor;

		if(mGlobalSettings.bRunningOCD){			
			mGlobalSettings.runOCD();
			
			if(mGlobalSettings.mProjectOpenState == 1){
				if(mEditor.loadFromFolder(mGlobalSettings.ProjectPath)){
					return 1;
				}

			} else if(mGlobalSettings.mProjectOpenState == 2){
				if(mEditor.createNewProject()){
					mEditor.bEditorRunning = false;
				}
			} else {
				mEditor.bEditorRunning = false;
			}

		} else {

			if(mCreateNewProject){
				if(mEditor.createNewProject()){
					mEditor.bEditorRunning = false;
				}		
			} else {
				if(mEditor.loadFromFolder(mGlobalSettings.ProjectPath)){
					mEditor.bEditorRunning = false;
				}		
			}
		
		}

		SDL_Event e;

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

		mEditor.shutdown();
		mGlobalSettings.shutdown();		
	}
	return 0;
}
