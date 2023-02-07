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

int TSettings::initSettings(){

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	TWindow = SDL_CreateWindow( "Simple Tilemap Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, SDL_WINDOW_SHOWN );
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
	
	TTF_SetFontStyle(LFont, TTF_STYLE_UNDERLINE);
	
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

		if( mProjectOpenState == 1){
			bRunningOCD = false;
		}

		if(mOpenCreate.bInputIsCancel){
			bRunningOCD = false;
		}

		SDL_SetRenderDrawColor( TRenderer, DefaultBGColor.r,  DefaultBGColor.g,  DefaultBGColor.b, 0xff); 
		SDL_RenderClear( TRenderer );

		mOpenCreate.render((WindowWidth/2) - (mOpenCreate.mDialogWidth/2),  (WindowHeight/2)- (mOpenCreate.mDialogHeight/2));
 
		SDL_RenderPresent( TRenderer );

		SDL_Event e;

		while( SDL_PollEvent( &e ) != 0 ){
			switch (e.type){
				case SDL_TEXTINPUT:
					if(mOpenCreate.bDialogIsWatingForText){
						mOpenCreate.recieveInput(std::string(e.text.text));
					}
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
				break;
				case SDL_MOUSEBUTTONDOWN:
	   				if (e.button.button == SDL_BUTTON_LEFT){	  		
	    				mOpenCreate.recieveInput(e.motion.x, e.motion.y);							
					}
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
		std::cout << "tilemaped -o <folder> OR" << std::endl;		
		std::cout << "tilemaped -n <mapwidth> <mapheight> <tilesize> <folder> [ -p <palette file> ]" << std::endl;		
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
	
	if(((argc < 3) && (argc > 1)) || ((argc > 3) && (argc < 6)) || ((argc > 8)) || (argc == 7)){
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
	
	if((argc == 6) || (argc == 8)){
		if(std::string(args[1]) != "-n"){
			printUsage();
			return 1;
		}

		if(argc == 8){
			if(args[6] == std::string("-p")){
				if((fs::exists(fs::status(args[7]))) && !(fs::is_directory(fs::status(args[7])))){
					std::cout << "Palette found" << std::endl;						
					mGlobalSettings.bProjectHasPalette = true;
					mGlobalSettings.ProjectPalettePath = std::string(args[7]);
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
			mGlobalSettings.TileSize = nTileSize;
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
		if(mGlobalSettings.bRunningOCD){			
			mGlobalSettings.runOCD();
			if(mGlobalSettings.mProjectOpenState == 1){
				if(mEditor.loadFromFolder(mGlobalSettings.ProjectPath)){
					return 1;
				}
			} else { 
				mEditor.bEditorRunning = false;
			}

		} else {

			if(mCreateNewProject){
				if(mEditor.createNewProject()){
					return 1;
				}		
			} else {
				if(mEditor.loadFromFolder(args[2])){
					return 1;
				}		
			}
		
		}

		SDL_Event e;

		while( mEditor.bEditorRunning ){

			SDL_SetRenderDrawColor(  mGlobalSettings.TRenderer, mGlobalSettings.DefaultBGColor.r,  mGlobalSettings.DefaultBGColor.g,  mGlobalSettings.DefaultBGColor.b, 0xff); 
			SDL_RenderClear( mGlobalSettings.TRenderer );

			mEditor.render();
 
			SDL_RenderPresent( mGlobalSettings.TRenderer );

			while( SDL_PollEvent( &e ) != 0 ){
				mEditor.handleEvents(&e);
			}
			mEditor.handleEvents();
		}

		mEditor.shutdown();
		mGlobalSettings.shutdown();		
	}
	return 0;
}
