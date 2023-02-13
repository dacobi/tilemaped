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

	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 ){
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN );
	TWindow = SDL_CreateWindow( "Tilemaped", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, window_flags);
	if( TWindow == NULL ){
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	TRenderer = SDL_CreateRenderer( TWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if( TRenderer == NULL ){
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	
	SDL_SetRenderDrawColor( TRenderer, mGlobalSettings.DefaultHighlightColor.r,mGlobalSettings.DefaultHighlightColor.g,mGlobalSettings.DefaultHighlightColor.b,0xff);

	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) ){
		std::cout << "SDL_image Error: " << IMG_GetError() << std::endl;
		return 1;
 	}
 	
 	TTF_Init();
	TFont =  TTF_OpenFont("nerdfont.ttf",20);

	
	if(TFont == NULL){
		std::cout << "SDL_TTF Error: " << TTF_GetError() << std::endl;
		return 1;
	}

	LFont =  TTF_OpenFont("nerdfont.ttf",30);	
	if(LFont == NULL){
		std::cout << "SDL_TTF Error: " << TTF_GetError() << std::endl;
		return 1;
	}
	UFont =  TTF_OpenFont("nerdfont.ttf",20);	
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

	mio->Fonts->AddFontFromFileTTF("nerdfont.ttf", 25.0,  NULL, ranges.Data);
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

	mHelpTextMap.push_back("Left Mouse Button: Select Tile and place in TileMap");
	mHelpTextTile.push_back("Left Mouse Button: Select Color and place in Tile");

	mHelpTextMap.push_back("Right Mouse Button: Select Tile or Tile Type in TileMap/TileSet");
	mHelpTextMap.push_back("                    Replace Selected Tile Type from TileSet");
	mHelpTextTile.push_back("Right Mouse Button: Replace Selected Pixel Color with new Color");
	mHelpTextMap.push_back("S: Toggle Show Selected Tile Type");
	mHelpTextMap.push_back("T: Toggle Show Selected Tile in TileMap Editor");

	mHelpTextMap.push_back("Mouse Scroll Wheel: Scale TileMap and Scroll TileSet");
	mHelpTextMap.push_back("LCTRL + Left Mouse Button: Move TileMap and Scroll TileSet");
	mHelpTextMap.push_back("F3: Create Empty Tile");
	mHelpTextMap.push_back("F4: Import Tile from file");	
	mHelpTextMap.push_back("F5: Copy Selected Tile");
	mHelpTextMap.push_back("F6: Remove Unused Tiles. Undo stack will be cleared!");
	

	
	mHelpTextTile.push_back("S: Toggle Show Selected Pixel Color");

	mHelpTextTile.push_back("P: Toggle Show Pixel Grid in Tile Editor");

	mHelpText.push_back("Space: Switch between TileMap and Tile Editor");	
	
	mHelpText.push_back("U: Undo last Action");
	mHelpText.push_back("R: Redo last Action");
}

void printUsage(){
		std::cout << std::endl;	
		std::cout << "Command Line Usage:" << std::endl;	
		std::cout << "tilemaped -o <folder>" << std::endl;		
		std::cout << "tilemaped -n <mapwidth> <mapheight> <tilewidth> <tileheight> <folder> [ -p <palette file> ]" << std::endl;
		std::cout << "tilemaped -c <Gimp Palette> <palfile.bin>" << std::endl;		
}


void TSettings::printHelpText(){
	initHelpText();
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
	std::cout << "General:" << std::endl;	

	for(const auto& cStr : mHelpText){
		std::cout << cStr << std::endl;	
	}

	printUsage();
}


int main( int argc, char* args[] )
{
	TEditor mEditor;
	std::stringstream mConvert;
	int nTileSize = 0;
	int nMapSizeX = 0;
	int nMapSizeY = 0;	
	bool mCreateNewProject=false;
	
	if(((argc < 3) && (argc > 1)) || ((argc > 4) && (argc < 7)) || ((argc > 9)) || (argc == 8)){
		if((argc == 2) && (std::string(args[1]) == "-h")){
			mGlobalSettings.printHelpText();
		} else {
			printUsage();
		}
		return 0;
	}

	if(argc == 1){
		mGlobalSettings.bRunningOCD = true;
	}
	
	if(argc == 4){
		if(std::string(args[1]) != "-c"){
			printUsage();
			return 1;
		}

		if(mGlobalSettings.testPaletteFile(args[2]) == 2){
				mEditor.mPalette.importGimpPalette(args[2]);
				mEditor.mPalette.saveToFile(args[3]);
				std::cout << "Gimp Palette converted and saved to: " << args[3] << std::endl;	
				return 0;
		} else {
			std::cout << "Gimp Palette not found" << std::endl;	
			return 1;
		}
	}

	if(argc == 3){
		if(std::string(args[1]) != "-o"){
			printUsage();
			return 1;
		}
		if(fs::is_directory(fs::status(args[2]))){
			std::cout << "Folder found" << std::endl;						
		} else {
			std::cout << "Folder not found!" << std::endl;						
			return 1;
		}
	}
	
	if((argc == 7) || (argc == 9)){
		if(std::string(args[1]) != "-n"){
			printUsage();
			return 1;
		}

		if(argc == 9){
			if(args[7] == std::string("-p")){
				if((fs::exists(fs::status(args[8]))) && !(fs::is_directory(fs::status(args[8])))){
					std::cout << "Palette found" << std::endl;						
					mGlobalSettings.bProjectHasPalette = true;
					mGlobalSettings.ProjectPalettePath = std::string(args[8]);
				} else {
					std::cout << "Palette file not found!" << std::endl;						
					return 1;
				}		
			} else {
				printUsage();
				return 1;
			}
		}
		
		mConvert << std::string(args[2]) << std::endl;
		mConvert >> nMapSizeX;
		
		if((nMapSizeX == 32) || (nMapSizeX == 64) || (nMapSizeX == 128) || (nMapSizeX == 256)){				
			mGlobalSettings.TileMapWidth = nMapSizeX;						
		} else {
			std::cout << "Wrong TileMap Size!" << std::endl;
			std::cout << "Valid Values are: 32, 64, 128, 256" << std::endl;						
			return 1;
		}
		
		mConvert << std::string(args[3]) << std::endl;
		mConvert >> nMapSizeY;
		
		if((nMapSizeY == 32) || (nMapSizeY == 64) || (nMapSizeY == 128) || (nMapSizeY == 256)){	
			mGlobalSettings.TileMapHeight = nMapSizeY;
		} else {
			std::cout << "Wrong TileMap Size!" << std::endl;
			std::cout << "Valid Values are: 32, 64, 128, 256" << std::endl;						
			return 1;
		}			

		mConvert << std::string(args[4]) << std::endl;		
		mConvert >> nTileSize;
			
		if((nTileSize == 16) || (nTileSize == 8)){	
			mGlobalSettings.TileSizeX = nTileSize;	
		} else {
			std::cout << "Wrong TileSize!" << std::endl;
			std::cout << "Valid Values are: 8, 16" << std::endl;						
			return 1;
		}

		mConvert << std::string(args[5]) << std::endl;		
		mConvert >> nTileSize;
			
		if((nTileSize == 16) || (nTileSize == 8)){	
			mGlobalSettings.TileSizeY = nTileSize;	
		} else {
			std::cout << "Wrong TileSize!" << std::endl;
			std::cout << "Valid Values are: 8, 16" << std::endl;						
			return 1;
		}

		if(fs::exists(fs::status(args[5]))){
			std::cout << "Error Folder Exists! " << std::endl;						
			return 1;
		} else {
			mGlobalSettings.ProjectPath = std::string(args[5]);						
		}		



		mCreateNewProject=true;		
	}
	
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
				if(mEditor.loadFromFolder(args[2])){
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
