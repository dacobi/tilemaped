#include <SDL.h> 
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <set>
#include <map>
#include <filesystem>
#include <sstream>

#include "cx16_palette.h"

//#define DIRDEL "\"
#define DIRDEL "/"

namespace fs = std::filesystem;

class TTexture;
class TTFTexture;
class Palette;
class Tile;
class TileSet;
class TileProperties;
class TileMap;
class TEditor;
class TEAction;
class TEActionGroup;
class TEActionReplaceTile;
class TEActionReplacePixel;
class Dialog;
class SDialog;
class SADialog;
class BDialog;
class HDialog;
class PIDialog;
class TIDialog;
class TBDialog;

class TSettings{
	public:
		int TileMapHeight=128;
		int TileMapWidth=128;
		int TileSize=16;
		std::string ProjectPath;
		int mProjectSaveState = 0;
		int TopBarHeight = 50;
		SDL_Renderer *TRenderer;
		SDL_Window *TWindow;
		int WindowWidth=1920;
		int WindowHeight=1080;
		int TileSetWidth=200;
		int TileMapScale=3;
		int initSettings();
		bool bShowTypeSelection=false;
		bool bShowPixelSelection=true;
		bool bShowSelectedTile = false;
		int mSelectedTile = 0;
		int mTileEdScale = 4;
		TTF_Font *TFont;
		TTF_Font *LFont;
		std::vector<std::string> mHelpText;
		void initHelpText();
		void printHelpText();
		int mLastTick = 0;
		int mCurrentTick = 0;
		int initTicks();
		int updateTicks();
		int getTicks();

};

TSettings mGlobalSettings;



class TTFTexture{
	public:
		SDL_Texture *TTFTex=NULL;
		std::string mTexText;
		int mTexWidth = 0;
		int mTexHeight = 0;		
		void setColor(Uint8 red, Uint8 green, Uint8 blue);
		void setBlendMode(SDL_BlendMode blend);
		void setAlpha(Uint8 alpha);
		int loadTTFFromString(std::string cTexText, SDL_Color textColor);
		int loadTTFFromUTF8(std::string cTexText, SDL_Color textColor);		
		int loadTTFFromUTF8(std::string cTexText, SDL_Color textColor, TTF_Font *cFont);		
		SDL_Rect render(int xpos,int ypos);

		std::string mWindow = "\uf2d0";

		std::string mFloppy = "\ue240"; //Floppy disk
		
		std::string mPrompt = "\uf120"; //Prompt
		//std::string mPrompt = "\ue7a2"; //TEST prompt good

		
		std::string mFile = "\uf15c"; // File Icon
//		std::string mBook = "\uf02d"; //lying book
//		std::string mBook = "\ufd2c"; //ring binder

		std::string mBook = "\uf405"; //Open Book SEL
		
		//std::string mBook = "\uf129"; Info 1
		
		std::string mInfo = "\uf449"; // Info circle
		
		
//		std::string mBook = "\uf7d5"  // ? mark
//		std::string mBook = "\ufc89"; // ? mark box
};	

int TTFTexture::loadTTFFromUTF8(std::string cTexText, SDL_Color textColor, TTF_Font *cFont){
	if(TTFTex){
		SDL_DestroyTexture(TTFTex);
		TTFTex = NULL;
	}
	
	if(!cTexText.size()){
		return 1;	
	}

	
	mTexText = cTexText;

	SDL_Surface* textSurf =  TTF_RenderUTF8_Solid(cFont, cTexText.c_str(), textColor);
	if( textSurf != NULL ){
        	TTFTex = SDL_CreateTextureFromSurface(mGlobalSettings.TRenderer, textSurf);
		if(TTFTex == NULL){
			std::cout << "SDL Error: " << SDL_GetError() << std::endl;
			return 1;
		} else {
			mTexWidth = textSurf->w;
			mTexHeight = textSurf->h;
		}
		SDL_FreeSurface(textSurf);
	}
	else{
		std::cout << "SDL_ttf Error: " << TTF_GetError() << std::endl;
		return 1;
	}

	return 0;
}

int TTFTexture::loadTTFFromUTF8(std::string cTexText, SDL_Color textColor){
	return loadTTFFromUTF8(cTexText, textColor, mGlobalSettings.TFont);
}

int TTFTexture::loadTTFFromString(std::string cTexText, SDL_Color textColor)
{
	if(TTFTex){
		SDL_DestroyTexture(TTFTex);
		TTFTex = NULL;
	}
	
	if(!cTexText.size()){
		return 1;	
	}

	
	mTexText = cTexText;

	SDL_Surface* textSurf = TTF_RenderText_Solid(mGlobalSettings.TFont, cTexText.c_str(), textColor);
	if( textSurf != NULL ){
        	TTFTex = SDL_CreateTextureFromSurface(mGlobalSettings.TRenderer, textSurf);
		if(TTFTex == NULL){
			std::cout << "SDL Error: " << SDL_GetError() << std::endl;
			return 1;
		} else {
			mTexWidth = textSurf->w;
			mTexHeight = textSurf->h;
		}
		SDL_FreeSurface(textSurf);
	}
	else{
		std::cout << "SDL_ttf Error: " << TTF_GetError() << std::endl;
		return 1;
	}

	return 0;
}

void TTFTexture::setColor(Uint8 red, Uint8 green, Uint8 blue){
	SDL_SetTextureColorMod( TTFTex, red, green, blue );
}

void TTFTexture::setBlendMode(SDL_BlendMode blend){
	SDL_SetTextureBlendMode( TTFTex, blend);
}
		
void TTFTexture::setAlpha(Uint8 alpha){
	SDL_SetTextureAlphaMod( TTFTex, alpha );
}

SDL_Rect TTFTexture::render(int xpos, int ypos){
	SDL_Rect renderQuad = {xpos, ypos, mTexWidth, mTexHeight};
	if(TTFTex) SDL_RenderCopy( mGlobalSettings.TRenderer, TTFTex, NULL, &renderQuad);
	return renderQuad;
}

class TTexture{
	public:
		SDL_Texture *TileTex;
		std::vector<unsigned char> FileData;
		std::vector<Uint32> PixelData;
		SDL_Rect CurrentArea;
		int initTexture();
		int updateTexture(Palette* tpal);
		int loadFromFile(std::string filename,Palette* tpal);
		int loadFromBuffer(std::vector<unsigned char> &cTileBuf,Palette* tpal);		
		int saveToFile(std::string filename);
		int setPixel(int xpos, int ypos, unsigned char tcolor, Palette* tpal);
		int setAllPixels(unsigned char tcolor, Palette* tpal);
		SDL_Rect render(int xpos, int ypos, int tscale=1, bool updateRect=false ,bool drawGrid=false);
		SDL_Rect renderEx(int xpos, int ypos, int tscale, SDL_RendererFlip flip);
		void renderEd(int xpos, int ypos, Palette* tpal);
		bool bPixelSelected = false;
};

class Palette{
	public:
		std::vector<SDL_Color> TPalette;
		int initPalette();
		std::vector<TTexture*> TPixels;
		std::vector<SDL_Rect> PixelAreas;
		int initTPixels();
		Uint32 mapPaletteColor(int tcolor);
		int render(int xpos, int ypos);
		SDL_Rect renderTileEd(int xpos,int ypos, int tcolor);
		
};

class Tile: public TTexture{
	public:
		std::vector<SDL_Rect> PixelAreas;
		int initTile();
		SDL_Rect render(int xpos, int ypos, int tscale=1, bool updateRect=false ,bool drawGrid=false);
		SDL_Rect render(int xpos, int ypos, int tscale, TileProperties tProps);
		void renderEd(int xpos, int ypos, Palette* tpal);
		int loadFromFile(std::string filename,Palette* tpal);
		int loadFromBuffer(std::vector<unsigned char> &cTileBuf,Palette* tpal);		
		bool bIsSelected = false;
};	

class TileSet{
	public:
		std::string DataPath;
		std::vector<Tile> TTiles;
		std::vector<SDL_Rect> TileAreas;
		int mCurTileScale=10;
		SDL_Rect mTileSetBackGround;
		int loadFromFolder(std::string tpath, Palette* tpal);
		int saveToFolder(std::string tpath);
		int render(int ypos, int mScroll);
};

class TileProperties{
	public:
		int mPaletteOffset=0;
		bool bFlipX = false;
		bool bFlipY = false;
};

class TileMap{
	public:
		std::string DataPath;
		std::string DataFile;
		std::vector<unsigned char> FileData;
		int getTile(int cTile);
		int setTile(int cTile, int cTileVal);
		int getFlip(int cTile);
		int setFlip(int cTile, int cTileFlip);
		TileProperties getTileProp(int cTile);
		std::vector<SDL_Rect> TileAreas;
		int loadFromFile(std::string path, std::string filename);
		int saveToFolder(std::string tpath);
		int render(int xpos,int ypos,TileSet* mTiles);
		std::map<int,int> mTilemapSizesIn = {{0,32},{1,64},{2,128},{3,256}};
		std::map<int,int> mTilemapSizesOut = {{32,0},{64,1},{128,2},{256,3}};
};


class TEAction{
	public:
		int TEActionType=0;
		virtual void undo();
		virtual void redo();
		bool operator==(const TEAction& rhs){ if(TEActionType == rhs.TEActionType){return this->doCompare(rhs);} else {return false;}}
		virtual bool doCompare(const TEAction& rhs){return false;}
};

class Dialog{
	public:
		SDL_Color mDialogColor= {0xff,0xff,0xff,0xff};
		SDL_Color mDialogBorderColor= {0x00,0x00,0xff,0xff};
		SDL_Color mTextColor = {0x00,0x00,0x00,0xff};
		int mDialogWidth=200;
		int mDialogHeight=100;
		int mDialogBorder=10;
		bool bDialogIsWatingForText=false;
		bool bInputIsAccept=false;
		bool bInputIsCancel=false;
		virtual void recieveInput(int mKey);
		virtual void recieveInput(std::string mTextInput);		
		virtual int recieveInput(int mx, int my);		
		virtual void dropLastInputChar();
		virtual SDL_Rect render(int xpos, int ypos);		
		virtual void init();
		virtual void update();
		virtual void cancel();
};

class BDialog: public Dialog{
	public:
		std::string mDialogTextMain;
		TTFTexture mTexDialogTextMain;
		SDL_Rect mButtonRect;
		int mMinDialogWidth=100;
		virtual void init();
		virtual int recieveInput(int mx, int my);		
		virtual SDL_Rect render(int xpos, int ypos);
};

class TIDialog: public BDialog{
	public:
		int mCursorTime = 0;
		bool bShowCursor=false;
		bool bIsInputActive=false;
		virtual void init();
		virtual int recieveInput(int mx, int my);		
		virtual SDL_Rect render(int xpos, int ypos);
		virtual void recieveInput(std::string mTextInput);
};

class SDialog: public Dialog{
	public:
		virtual void init();
		std::string mDialogTextMain;
		std::string mDialogTextInput;
		TTFTexture mTexDialogTextMain;
		TTFTexture mTexDialogTextInput;		
		BDialog mAcceptButton;
		BDialog mCancelButton;
		virtual void recieveInput(int mKey);		
		virtual int recieveInput(int mx, int my);
		virtual SDL_Rect render(int xpos, int ypos);
};

class SADialog: public SDialog{
	public:
		virtual void init();
		virtual void cancel();
		virtual void recieveInput(std::string mTextInput);
		virtual void recieveInput(int mKey);
		virtual int recieveInput(int mx, int my);
		virtual void dropLastInputChar();
		virtual SDL_Rect render(int xpos, int ypos);
		bool bSubDialogActive = false;	
		SDialog *mSubDialog;
		TIDialog mTextInput;
		void resize();
};

class HDialog: public SDialog{
	public:
		std::vector<TTFTexture*> mHelpText;
		BDialog mCloseButton;
		virtual void init();
		virtual void recieveInput(int mKey);
		virtual int recieveInput(int mx, int my);				
		virtual SDL_Rect render(int xpos, int ypos);
};

class PIDialog: public HDialog{
	public:
		virtual void init();
		virtual void update();
		int fliph,flipv;
		virtual SDL_Rect render(int xpos, int ypos);
};

class TBDialog: public Dialog{
	public:
		std::string mDialogTextActions;
		std::string mDialogTextWindow;		
		std::string mDialogTextProject;
		TTFTexture mTexDialogTextActions;
		TTFTexture mTexDialogTextWindow;		
		TTFTexture mTexDialogTextProject;
		TEditor* mEditor;
		virtual void init();
		virtual SDL_Rect render(int xpos, int ypos);
};

enum {
	EMODE_MAP,
	EMODE_TILE
} emodes;

class TEditor{
	public:
		Palette mPalette;
		TileSet mTileSet;
		TileMap mTileMap;
		TBDialog mTopBar;
		int mMapSelectedTile;
		Tile* mTileSelectedTile;
		int mColorSelected;
		TTexture* mColorSelectedTile;
		bool bEditorRunning=true;
		int handleEvents(SDL_Event* cEvent);
		int handleEvents();
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
		bool bLCTRLisDown = false;
		bool bTileMapGrapped = false;
		bool bTileSetGrapped = false;
		int flipSelectedTile();
		int searchRects(std::vector<SDL_Rect> &sRects);
		int findSelected();
		int toggleSelectedTile();
		std::vector<TEActionGroup*> mUndoStack;
		std::vector<TEActionGroup*> mRedoStack;
		TEAction mEmptyAction;
		TEAction *mLastAction;
		void newActionGroup();
		void addAction(TEAction *newAction);
		void undoLastActionGroup();
		void redoLastActionGroup();
		void redoClearStack();
		Dialog *mActiveDialog = NULL;
		SDialog mSaveDialog;
		SADialog mSaveAsDialog;
		HDialog mHelpDialog;
		PIDialog mProjectInfo;
		int activateSaveDialog();
		int activateHelpDialog();
		int cancelActiveDialog();
		int activateSaveAsDialog();
		int activateProjectInfo();
};

class TEActionGroup{
	public:
		std::vector<TEAction*> mActions;
		void undo();
		void redo();
};

class TEActionReplaceTile: public TEAction{
	public:
		int mCurrentTile;
		int mOldValue;
		int mNewValue;
		TileMap *mTileMap;
		void doAction(TileMap *cTileMap, int mCurTile, int mOld, int mNew);	
		virtual void undo();
		virtual void redo();
		virtual bool doCompare(const TEAction& rhs){
			const TEActionReplaceTile* mrhs =  dynamic_cast<const TEActionReplaceTile*>(&rhs); 
			if(mrhs){
				if((mCurrentTile == mrhs->mCurrentTile) &&  (mNewValue == mrhs->mNewValue)){ // Not testing mOldValue
					return true;
				}
			}	
			 return false;
		}
};

class TEActionReplacePixel: public TEAction{
	public:
		Tile* mCurrentTile;
		int mCurrentPixel;
		int mOldValue;
		int mNewValue;
		Palette* mCurrentPalette;
		void doAction(Tile* mCurTile, int mCurPix, int mOld, int mNew, Palette* mPal);	
		virtual void undo();
		virtual void redo();
		virtual bool doCompare(const TEAction& rhs){
			const TEActionReplacePixel* mrhs =  dynamic_cast<const TEActionReplacePixel*>(&rhs); 
			if(mrhs){
				if((mCurrentTile == mrhs->mCurrentTile) && (mCurrentPixel == mrhs->mCurrentPixel) && (mNewValue == mrhs->mNewValue)){ // Not testing mOldValue
					return true;
				}
			}	
			 return false;
		}
};

void TEAction::undo(){

}

void TEActionReplaceTile::undo(){
	mTileMap->setTile(mCurrentTile, mOldValue);
}

void TEActionReplacePixel::undo(){
	mCurrentTile->FileData[mCurrentPixel] = mOldValue;
	mCurrentTile->updateTexture(mCurrentPalette);
}

void TEAction::redo(){}

void TEActionReplaceTile::redo(){
	mTileMap->setTile(mCurrentTile, mNewValue);
}

void TEActionReplacePixel::redo(){
	mCurrentTile->FileData[mCurrentPixel] = mNewValue;
	mCurrentTile->updateTexture(mCurrentPalette);
}

void TEActionReplaceTile::doAction(TileMap *cTileMap, int mCurTile, int mOld, int mNew){
	mTileMap = cTileMap;
	mCurrentTile = mCurTile;
	mOldValue = mOld;
	mNewValue = mNew;
	mTileMap->setTile(mCurrentTile, mNew);
	TEActionType=1;
}

void TEActionReplacePixel::doAction(Tile* mCurTile, int mCurPix, int mOld, int mNew, Palette* mPal){
	mCurrentTile = mCurTile;
	mCurrentPixel = mCurPix;
	mOldValue = mOld;
	mNewValue = mNew;
	mCurrentPalette = mPal;
	mCurrentTile->FileData[mCurrentPixel] = mNewValue;
	mCurrentTile->updateTexture(mCurrentPalette);
	TEActionType=2;
}

void TEActionGroup::undo(){
	for(TEAction* mAction : mActions){
		mAction->undo();
	}
}

void TEActionGroup::redo(){
	for(TEAction* mAction : mActions){
		mAction->redo();
	}
}

int TTexture::loadFromFile(std::string filename,Palette* tpal){
    initTexture();
    std::ifstream infile(filename, std::ios::binary );

    FileData.resize(mGlobalSettings.TileSize * mGlobalSettings.TileSize);

    std::vector<unsigned char> tbuffer(std::istreambuf_iterator<char>(infile), {});
    FileData = tbuffer;

    updateTexture(tpal);
    return 0;
}

int TTexture::loadFromBuffer(std::vector<unsigned char> &cTileBuf,Palette* tpal){
	initTexture();
    FileData.resize(mGlobalSettings.TileSize * mGlobalSettings.TileSize);
	FileData = cTileBuf;
	updateTexture(tpal);
	return 0;
}

int TTexture::setAllPixels(unsigned char tcolor, Palette* tpal){
	FileData.resize(mGlobalSettings.TileSize * mGlobalSettings.TileSize, tcolor);
	updateTexture(tpal);
	return 0;
}

int TTexture::initTexture(){
	TileTex = SDL_CreateTexture(mGlobalSettings.TRenderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, mGlobalSettings.TileSize,mGlobalSettings.TileSize);
	return 0;
}

int TTexture::updateTexture(Palette* tpal){
	if(PixelData.size()){
		PixelData.erase(PixelData.begin(),PixelData.end());
	}
	for(int i = 0; i < (mGlobalSettings.TileSize * mGlobalSettings.TileSize); i++){
    		PixelData.push_back(tpal->mapPaletteColor(FileData[i]));
	}

        SDL_UpdateTexture(TileTex, NULL, PixelData.data(), mGlobalSettings.TileSize * sizeof(Uint32));
	return 0;
}

SDL_Rect TTexture::render(int xpos, int ypos, int tscale, bool updateRect ,bool drawGrid){
	SDL_Rect renderQuad = { xpos, ypos, mGlobalSettings.TileSize*tscale, mGlobalSettings.TileSize*tscale};
        SDL_RenderCopy(mGlobalSettings.TRenderer, TileTex, NULL, &renderQuad);	
        if(drawGrid){
		SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, 0x00,0x00,0x00,0xff);
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &renderQuad);
        }
        if(bPixelSelected){
		SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, 0xFF,0x00,0x00,0xff);
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &renderQuad);
		SDL_Rect sndRect = renderQuad;
		sndRect.x = sndRect.x-1;
		sndRect.y = sndRect.y-1;
		sndRect.w = sndRect.w+2;
		sndRect.h = sndRect.h+2;

		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &sndRect);
        }
        if(updateRect){
        	CurrentArea = renderQuad;
        }
        return renderQuad;
}

SDL_Rect TTexture::renderEx(int xpos, int ypos, int tscale, SDL_RendererFlip flip){
	SDL_Rect renderQuad = { xpos, ypos, mGlobalSettings.TileSize*tscale, mGlobalSettings.TileSize*tscale};
        SDL_RenderCopyEx(mGlobalSettings.TRenderer, TileTex, NULL, &renderQuad, 0, NULL, flip);	
        return renderQuad;
}

void TTexture::renderEd(int xpos, int ypos, Palette* tpal){
	for(int i=0; i < mGlobalSettings.TileSize; i++){
		for(int j=0; j < mGlobalSettings.TileSize; j++){
			tpal->renderTileEd(xpos + mGlobalSettings.TileSize*j, ypos + mGlobalSettings.TileSize*i, FileData[j+(i*mGlobalSettings.TileSize)]);
		}
	}

}

Uint32 Palette::mapPaletteColor(int tcolor){
	Uint32 tmpcol = TPalette[tcolor].r;
	tmpcol = tmpcol << 8;
	tmpcol += TPalette[tcolor].g;
	tmpcol = tmpcol << 8;
	tmpcol += TPalette[tcolor].b;
	tmpcol = tmpcol << 8;
	tmpcol += 255;

	return tmpcol;
}

int Palette::initPalette(){
	for(int i = 0; i < 256; i++){
		SDL_Color tmpcol;
		tmpcol.r = palette[i][0];
		tmpcol.g = palette[i][1];
		tmpcol.b = palette[i][2];
		tmpcol.a = 255;
		TPalette.push_back(tmpcol);
	}
	PixelAreas.resize(256);
	return 0;
}

int Palette::initTPixels(){
	TTexture *tmptpix;
	for(int i = 0; i < 256; i++){
		tmptpix = new TTexture;
		tmptpix->initTexture();
		tmptpix->setAllPixels(i, this);
		TPixels.push_back(tmptpix);
	}
	return 0;
}

SDL_Rect Palette::renderTileEd(int xpos,int ypos, int tcolor){
	return TPixels[tcolor]->render(xpos, ypos, mGlobalSettings.mTileEdScale,false,mGlobalSettings.bShowPixelSelection);
}

int Palette::render(int xpos,int ypos){

	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j++){
			PixelAreas[(i*16)+j] = TPixels[(i*16)+j]->render(xpos + ((mGlobalSettings.TileSize+2)*i), ypos + ((mGlobalSettings.TileSize+2)*j), 1,true,true);
		}
	}

return 0;
}

int Tile::initTile(){
	PixelAreas.resize(mGlobalSettings.TileSize*mGlobalSettings.TileSize);
	return 0;
}

SDL_Rect Tile::render(int xpos, int ypos, int tscale, bool updateRect ,bool drawGrid){
	SDL_Rect tmpRect = TTexture::render(xpos, ypos, tscale, updateRect, drawGrid);
	if(bIsSelected){
		SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, 0xFF,0x00,0x00,0xff);
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &tmpRect);
		SDL_Rect sndRect = tmpRect;
		sndRect.x = sndRect.x-1;
		sndRect.y = sndRect.y-1;
		sndRect.w = sndRect.w+2;
		sndRect.h = sndRect.h+2;
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &sndRect);
	}
	return tmpRect;
}

SDL_Rect Tile::render(int xpos, int ypos, int tscale,TileProperties tProps){
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	if((tProps.bFlipX) && (tProps.bFlipY)){
			flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
	} else {
	        if(tProps.bFlipX){
			flip = SDL_FLIP_HORIZONTAL;
		} 
		if(tProps.bFlipY){
			flip = SDL_FLIP_VERTICAL;
		}
	}

	SDL_Rect tmpRect = TTexture::renderEx(xpos, ypos, tscale, flip);
	
	if(bIsSelected){
		SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, 0xFF,0x00,0x00,0xff);
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &tmpRect);
		SDL_Rect sndRect = tmpRect;
		sndRect.x = sndRect.x-1;
		sndRect.y = sndRect.y-1;
		sndRect.w = sndRect.w+2;
		sndRect.h = sndRect.h+2;
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &sndRect);
	}

	
	return tmpRect;
}

void Tile::renderEd(int xpos, int ypos, Palette* tpal){
	for(int i=0; i < mGlobalSettings.TileSize; i++){
		for(int j=0; j < mGlobalSettings.TileSize; j++){
			PixelAreas[j+(mGlobalSettings.TileSize*i)] = tpal->renderTileEd(xpos + (mGlobalSettings.TileSize * mGlobalSettings.mTileEdScale)*j, ypos + (mGlobalSettings.TileSize * mGlobalSettings.mTileEdScale)*i, FileData[j+(i*mGlobalSettings.TileSize)]);
		}
	}
}

int Tile::loadFromFile(std::string filename,Palette* tpal){ 
	initTile();	
	return TTexture::loadFromFile(filename,tpal);
}

int Tile::loadFromBuffer(std::vector<unsigned char> &cTileBuf,Palette* tpal){ 
	initTile();	
	return TTexture::loadFromBuffer(cTileBuf,tpal);
}

int TileSet::loadFromFolder(std::string path, Palette* tpal){ 

	fs::path cTileFile;
	int ntiles=0;

	if(fs::is_directory(fs::status(path))){
		cTileFile = path + DIRDEL + "tiles.bin";
	} else {
		std::cout << "Folder Not Found: " << path << std::endl;
		return 1;		
	}

	if(!fs::exists(fs::status(cTileFile))){
		std::cout << "File Not Found: " << cTileFile << std::endl;
		return 1;
	}

	std::ifstream infile(cTileFile, std::ios::binary );

	std::vector<unsigned char> tbuffer(std::istreambuf_iterator<char>(infile), {});

	int twidth = tbuffer[0];
    int theigth = tbuffer[1];

	if((twidth == theigth) && ((twidth == 8) || (twidth == 16))){
		mGlobalSettings.TileSize = twidth;
	} else {
		std::cout << "Error in Tile File: " << cTileFile << std::endl;
		return 1;		
	}

	int tmpTileSize = twidth*twidth;

	tbuffer.erase(tbuffer.begin());
	tbuffer.erase(tbuffer.begin());

	if(tbuffer.size() % tmpTileSize){
		std::cout << "Error in Tile File: " << cTileFile << std::endl;
		return 1;
	}

	ntiles = tbuffer.size() / tmpTileSize;

	TTiles.resize(ntiles);
	TileAreas.resize(ntiles);

	int tCount = 0;

	for(auto &mTile : TTiles){
		std::vector<unsigned char>::const_iterator first = tbuffer.begin() + (tCount * tmpTileSize);
		std::vector<unsigned char>::const_iterator last = tbuffer.begin() + ((tCount * tmpTileSize) + (tmpTileSize));
		std::vector<unsigned char> tbuffer2(first, last);
		mTile.loadFromBuffer(tbuffer2 ,tpal);
		tCount++;
	}

	return 0;
}

int TileSet::saveToFolder(std::string tpath){
	std::ofstream outfile(tpath+DIRDEL+"tiles.bin", std::ios::binary );

	std::vector<unsigned char> obuffer;

	obuffer.push_back(mGlobalSettings.TileSize);
	obuffer.push_back(mGlobalSettings.TileSize);

	for(auto &mTile : TTiles){		
		obuffer.insert(obuffer.end(), mTile.FileData.begin(), mTile.FileData.end());
	}

	outfile.write((char*)obuffer.data(),obuffer.size());
	outfile.close();

	return 0;
}

int TileSet::render(int ypos, int mScroll){
	mTileSetBackGround.x = mGlobalSettings.WindowWidth - mGlobalSettings.TileSetWidth;
	mTileSetBackGround.y = ypos;
	mTileSetBackGround.w = mGlobalSettings.TileSetWidth; 
	mTileSetBackGround.h = mGlobalSettings.WindowHeight;
	SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, 0xFF,0xFF,0xFF,0xff);
	SDL_RenderFillRect(mGlobalSettings.TRenderer, &mTileSetBackGround);
	
	for(int i = 0; i < TTiles.size(); i++){
		TileAreas[i] = TTiles[i].render((mTileSetBackGround.x+(mGlobalSettings.TileSetWidth - (mCurTileScale*mGlobalSettings.TileSize))/2),mTileSetBackGround.y + mScroll + ((mGlobalSettings.TileSetWidth - (mCurTileScale * mGlobalSettings.TileSize))/2) + (((mGlobalSettings.TileSize*mCurTileScale)+2)*i), 10,true,true);
	}

	SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, 0x20,0x20,0x20,0xff);
	SDL_RenderDrawRect(mGlobalSettings.TRenderer, &mTileSetBackGround);

	
	mTileSetBackGround.x = mTileSetBackGround.x + 1;
	mTileSetBackGround.y = mTileSetBackGround.y + 1;
	mTileSetBackGround.w = mTileSetBackGround.w - 2; 
	mTileSetBackGround.h = mTileSetBackGround.h - 2;

	SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, 0x20,0x20,0x20,0xff);
	SDL_RenderDrawRect(mGlobalSettings.TRenderer, &mTileSetBackGround);

	mTileSetBackGround.x = mTileSetBackGround.x + 1;
	mTileSetBackGround.y = mTileSetBackGround.y + 1;
	mTileSetBackGround.w = mTileSetBackGround.w - 2; 
	mTileSetBackGround.h = mTileSetBackGround.h - 2;

	SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, 0x20,0x20,0x20,0xff);
	SDL_RenderDrawRect(mGlobalSettings.TRenderer, &mTileSetBackGround);


return 0;
}

int TileMap::loadFromFile(std::string path, std::string filename){
    	DataPath = path; 	
    	DataFile = filename;

    	std::ifstream infile(path+DIRDEL+filename, std::ios::binary );
    	
    	if(!infile){
		std::cout << "TileMap Not Found!" << std::endl;
		return 1;    	
    	}
    	
    	std::vector<unsigned char> tbuffer(std::istreambuf_iterator<char>(infile), {});

    	int twidth = tbuffer[0];
    	int theigth = tbuffer[1];
        
		if((twidth >= 0) && (twidth <= 3)){
			twidth = mTilemapSizesIn[twidth];
		} else {
			std::cout << "TileMap File Error!" << std::endl;
			return 1;
		}


		if((theigth >= 0) && (theigth <= 3)){
			theigth = mTilemapSizesIn[theigth];
		} else {
			std::cout << "TileMap File Error!" << std::endl;
			return 1;
		}


	tbuffer.erase(tbuffer.begin());
	tbuffer.erase(tbuffer.begin());

	if((twidth * theigth * 2) == tbuffer.size()){
		FileData.resize(twidth * theigth * 2);
		FileData = tbuffer;
		mGlobalSettings.TileMapHeight = theigth;
		mGlobalSettings.TileMapWidth = twidth;
		TileAreas.resize(twidth * theigth);
		
	} else {
		std::cout << "TileMap File Error!" << std::endl;
		return 1;
	}

    return 0;
}

int TileMap::saveToFolder(std::string tpath){
   	std::ofstream outfile(tpath+DIRDEL+"map.bin", std::ios::binary );

	std::vector<unsigned char> obuffer;

	obuffer.push_back(mTilemapSizesOut[mGlobalSettings.TileMapWidth]);
	obuffer.push_back(mTilemapSizesOut[mGlobalSettings.TileMapHeight]);

	obuffer.insert(obuffer.end(), FileData.begin(), FileData.end());

	outfile.write((char*)obuffer.data(),obuffer.size());
	outfile.close();

	return 0;
}


int TileMap::getTile(int cTile){
	if((cTile > -1) && (cTile < TileAreas.size())){
		int cTileVal = (FileData[(cTile*2)+1] & 0x3) << 8;
		cTileVal += FileData[(cTile*2)];
		return cTileVal;
	}
	return -1;
}

int TileMap::setTile(int cTile, int cTileVal){
	if((cTile > -1) && (cTile < TileAreas.size())){
		FileData[(cTile*2)+1] = (FileData[(cTile*2)+1] & 0xfc) + ((cTileVal >> 8) & 0x3);
		FileData[(cTile*2)] = cTileVal & 0xff;		
		return 0;
	}	
	return 1;
}

int TileMap::getFlip(int cTile){
	if((cTile > -1) && (cTile < TileAreas.size())){
		int cTileFlip = ((FileData[(cTile*2)+1]  >> 2) & 0x3);
		return cTileFlip;
	}
	return -1;
}

int TileMap::setFlip(int cTile, int cTileFlip){
	if((cTile > -1) && (cTile < TileAreas.size()) && (cTileFlip > -1) && (cTileFlip < 4) ){
		FileData[(cTile*2)+1] = (FileData[(cTile*2)+1] & 0xf3) + (cTileFlip << 2);
		return 0;
	}
	return 1;
}

void Dialog::init(){

}

void Dialog::update(){

}

void Dialog::cancel(){
	bDialogIsWatingForText=false;
	bInputIsAccept=false;
	bInputIsCancel=false;
}

void Dialog::recieveInput(int mKey){

}

void Dialog::recieveInput(std::string mTextInput){

}

void Dialog::dropLastInputChar(){

}

SDL_Rect Dialog::render(int xpos, int ypos){
	SDL_Rect tmpBorder;
	tmpBorder.x = xpos;
	tmpBorder.y = ypos;
	tmpBorder.w = mDialogWidth;
	tmpBorder.h = mDialogHeight;	
	SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, mDialogBorderColor.r,mDialogBorderColor.g,mDialogBorderColor.b,mDialogBorderColor.a);
	SDL_RenderFillRect(mGlobalSettings.TRenderer, &tmpBorder);
	SDL_Rect tmpBorder2;	
	tmpBorder2.x = xpos+mDialogBorder;
	tmpBorder2.y = ypos+mDialogBorder;
	tmpBorder2.w = mDialogWidth-(mDialogBorder*2);
	tmpBorder2.h = mDialogHeight-(mDialogBorder*2);	
	SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, mDialogColor.r,mDialogColor.g,mDialogColor.b,mDialogColor.a);
	SDL_RenderFillRect(mGlobalSettings.TRenderer, &tmpBorder2);
	return tmpBorder;
}

int Dialog::recieveInput(int mx, int my){
	return 0;
}

void TBDialog::init(){
	mDialogBorder=3;
	mDialogBorderColor= {0x20,0x20,0x20,0xff};
	mDialogWidth = mGlobalSettings.WindowWidth;
	mDialogHeight =  mGlobalSettings.TopBarHeight;
	mDialogTextActions = mTexDialogTextActions.mFloppy +" Save/Save As: F12/F11    " + mTexDialogTextActions.mBook + " Help: F1";	
	mTexDialogTextActions.loadTTFFromUTF8(mDialogTextActions, mTextColor);
	mDialogTextWindow = mEditor->mCurMode == EMODE_MAP ? mTexDialogTextActions.mWindow + " TileMap Editor" : mTexDialogTextActions.mWindow + " Tile Editor";
	mTexDialogTextWindow.loadTTFFromUTF8(mDialogTextWindow, mTextColor);
	mDialogTextProject = mTexDialogTextActions.mFile + " Project: " + mGlobalSettings.ProjectPath + "  " + mTexDialogTextActions.mInfo + " Info: F2";
	mTexDialogTextProject.loadTTFFromUTF8(mDialogTextProject, mTextColor);
}

SDL_Rect TBDialog::render(int xpos, int ypos){
	mDialogWidth = mGlobalSettings.WindowWidth;
	SDL_Rect tmpRect = Dialog::render(xpos, ypos);
	mDialogTextWindow = mEditor->mCurMode == EMODE_MAP ? mTexDialogTextActions.mWindow + " TileMap Editor" : mTexDialogTextActions.mWindow + " Tile Editor";
	mTexDialogTextWindow.loadTTFFromUTF8(mDialogTextWindow, mTextColor);
	mTexDialogTextActions.render(mDialogBorder*4,(mDialogHeight-mTexDialogTextActions.mTexHeight)/2);
	mTexDialogTextWindow.render((mGlobalSettings.WindowWidth/2)-(mTexDialogTextWindow.mTexWidth/2),(mDialogHeight-mTexDialogTextActions.mTexHeight)/2);
	mTexDialogTextProject.render(mGlobalSettings.WindowWidth -  mTexDialogTextProject.mTexWidth - mDialogBorder*4, (mDialogHeight-mTexDialogTextActions.mTexHeight)/2);
	return tmpRect;
}

void SDialog::init(){
//	mDialogTextMain = "\uf15c Overwrite Project On Disk";
	mDialogTextMain = mTexDialogTextMain.mFloppy +" Overwrite Project On Disk?";
//	mDialogTextInput = "(Y/N)?";

	mAcceptButton.mDialogTextMain = "Save";
	mCancelButton.mDialogTextMain = "Cancel";	
	
	mAcceptButton.init();	
	mCancelButton.init();
		
	mTexDialogTextMain.loadTTFFromUTF8(mDialogTextMain, mTextColor);
	//mTexDialogTextInput.loadTTFFromString(mDialogTextInput, mTextColor);
	
	mDialogWidth = mTexDialogTextMain.mTexWidth > mTexDialogTextInput.mTexWidth ? mTexDialogTextMain.mTexWidth : mTexDialogTextInput.mTexWidth;
	mDialogHeight = mTexDialogTextMain.mTexHeight > mTexDialogTextInput.mTexHeight ? mTexDialogTextMain.mTexHeight : mTexDialogTextInput.mTexHeight;
	
	mDialogHeight *=5;
	
	mDialogWidth += mDialogBorder * 4;

}

int SDialog::recieveInput(int mx, int my){
	if(mAcceptButton.recieveInput(mx,my)){
		recieveInput(SDLK_y);
	}
	if(mCancelButton.recieveInput(mx,my)){
		recieveInput(SDLK_n);		
	}
	return 0;
}

void SDialog::recieveInput(int mKey){
	std::cout << "recieveInput " << mKey << std::endl;
	if(mKey == SDLK_y){
		bInputIsAccept=true;
		mGlobalSettings.mProjectSaveState = 1;
		std::cout << "bInputIsAccept=true" << std::endl;
	}
	if(mKey == SDLK_n){
		bInputIsCancel=true;
		std::cout << "bInputIsCancel=true" << std::endl;
	}
}

SDL_Rect SDialog::render(int xpos, int ypos){
	SDL_Rect tmpBorder = Dialog::render(xpos, ypos);

	mTexDialogTextMain.render(tmpBorder.x+mDialogBorder*2,tmpBorder.y+mDialogBorder*2);
	//mTexDialogTextInput.render(tmpBorder.x+mDialogBorder*2,);	
	
	mAcceptButton.render(tmpBorder.x + ((tmpBorder.w / 4) - (mAcceptButton.mTexDialogTextMain.mTexWidth)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight+mDialogBorder*3);	
	mCancelButton.render(tmpBorder.x + (((tmpBorder.w / 4)*3) - (mCancelButton.mTexDialogTextMain.mTexWidth)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight+mDialogBorder*3);	
	
//	mCancelButton.render(tmpBorder.w, tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight+mDialogBorder*2);

	
	return tmpBorder;
}

int SADialog::recieveInput(int mx, int my){
	if(bSubDialogActive){
		mSubDialog->recieveInput(mx, my);
		if(mSubDialog->bInputIsAccept){
			mSubDialog->bInputIsAccept = false;
			recieveInput(SDLK_y);
		}
		if(mSubDialog->bInputIsCancel){
			mSubDialog->bInputIsCancel = false;
			recieveInput(SDLK_n);
		}				
	} else {
		return SDialog::recieveInput(mx, my);
	}
	return 0;
}

void SADialog::resize(){
	mDialogWidth = mTexDialogTextMain.mTexWidth > mTextInput.mTexDialogTextMain.mTexWidth ? mTexDialogTextMain.mTexWidth : mTextInput.mTexDialogTextMain.mTexWidth;
	mDialogHeight = mTexDialogTextMain.mTexHeight; // > mTexDialogTextInput.mTexHeight ? mTexDialogTextMain.mTexHeight : mTexDialogTextInput.mTexHeight;
	
	mDialogHeight *=6;
	
	mDialogWidth += mDialogBorder * 5;	
}

void SADialog::cancel(){
	Dialog::cancel();
	mSubDialog->cancel();
	bSubDialogActive = false;
}

void SADialog::init(){
	mDialogTextMain = mTexDialogTextMain.mFloppy + " Save Project As Folder";

	mAcceptButton.mDialogTextMain = "Save";
	mCancelButton.mDialogTextMain = "Cancel";	
	
	mAcceptButton.init();	
	mCancelButton.init();

		
	mTexDialogTextMain.loadTTFFromUTF8(mDialogTextMain, mTextColor);
	
	mTextInput.mDialogTextMain = mDialogTextInput;
	mTextInput.bIsInputActive = true;
	mTextInput.init();
	
	resize();
}

SDL_Rect SADialog::render(int xpos, int ypos){
//	SDL_Rect tmpBorder;// = SDialog::render(xpos, ypos);

	SDL_Rect tmpBorder = Dialog::render(xpos, ypos);

	mTexDialogTextMain.render(tmpBorder.x+mDialogBorder*2,tmpBorder.y+mDialogBorder*2);
	
	mAcceptButton.render(tmpBorder.x + ((tmpBorder.w / 4) - (mAcceptButton.mTexDialogTextMain.mTexWidth)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight * 2 + mDialogBorder*4);	
	mCancelButton.render(tmpBorder.x + (((tmpBorder.w / 4)*3) - (mCancelButton.mTexDialogTextMain.mTexWidth)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight * 2+ mDialogBorder*4);	
		
	mTextInput.render(tmpBorder.x+((mDialogWidth/2)-(mTextInput.mDialogWidth/2)) ,tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight+mDialogBorder*2);
	if(bSubDialogActive){
		tmpBorder = mSubDialog->render(xpos + 50, ypos + 50);
	}
	return tmpBorder;
}

void SADialog::recieveInput(std::string mText){		
	std::cout << "recieveInput(std::string mText)" << std::endl;
	mTextInput.mDialogTextMain += mText;
	mTextInput.init();
	resize();	
}

void SADialog::recieveInput(int mKey){
	std::cout << "recieveInput " << mKey << std::endl;
	if(bSubDialogActive){
			if(mKey == SDLK_y){
				bInputIsAccept=true;
				bSubDialogActive = false;
				mGlobalSettings.mProjectSaveState = 1;
				mGlobalSettings.ProjectPath = mTextInput.mDialogTextMain;
				std::cout << "bInputIsAccept=true" << std::endl;			
			}	
			if(mKey == SDLK_n){
				bSubDialogActive=false;
				bDialogIsWatingForText = true;
				SDL_StartTextInput();
				std::cout << "bInputIsCancel=true" << std::endl;
			}		
	} else {
		if(mKey == SDLK_y){
			if(fs::is_directory(fs::status(mTextInput.mDialogTextMain))){
				bSubDialogActive = true;
				bDialogIsWatingForText = false;
			} else {
				mGlobalSettings.mProjectSaveState = 1;
				mGlobalSettings.ProjectPath = mTextInput.mDialogTextMain;
				bInputIsAccept=true;
				std::cout << "bInputIsAccept=true" << std::endl;
			}
			SDL_StopTextInput();
		}
		if(mKey == SDLK_n){
			bInputIsCancel=true;
			SDL_StopTextInput();
			std::cout << "bInputIsCancel=true" << std::endl;
		}		
	}
}

void SADialog::dropLastInputChar(){
	if(mTextInput.mDialogTextMain.size()){
		mTextInput.mDialogTextMain.pop_back();
		mTextInput.init();
		resize();
	}
}

void BDialog::init(){
	mDialogBorder=3;
	mDialogBorderColor= {0x20,0x20,0x20,0xff};
	
	mTexDialogTextMain.loadTTFFromString(mDialogTextMain, mTextColor);
	
	mDialogWidth = mTexDialogTextMain.mTexWidth;
	mDialogHeight = mTexDialogTextMain.mTexHeight;
	
	if(mMinDialogWidth > mDialogWidth) {
		mDialogWidth = mMinDialogWidth;
	}
	
	mDialogHeight += mDialogBorder * 3;
	
	mDialogWidth += mDialogBorder * 4;	
}

int BDialog::recieveInput(int mx, int my){
	if( (mx >= mButtonRect.x) && (mx <= (mButtonRect.x + mButtonRect.w)) && (my >= mButtonRect.y) && (my <= (mButtonRect.y + mButtonRect.h)) ){
		return 1;
	}
	return 0;
}

SDL_Rect BDialog::render(int xpos, int ypos){
	
	mButtonRect = Dialog::render(xpos, ypos);
	
//	int cTextX = xpos + (mDialogBorder *2);

	int cTextX = xpos + (mDialogWidth-mTexDialogTextMain.mTexWidth)/2;
	int cTextY = ypos + (mDialogBorder + (mDialogBorder/3));
	mTexDialogTextMain.render(cTextX, cTextY);
	
	return mButtonRect;
}

void TIDialog::init(){
	mDialogBorder=3;
	mDialogBorderColor= {0x20,0x20,0x20,0xff};
	mCursorTime += mGlobalSettings.getTicks();
	if(mCursorTime > 500){
		mCursorTime = 0;
		if(bIsInputActive){
			bShowCursor = !bShowCursor;
		}
	}
	
	if(bShowCursor){
		mTexDialogTextMain.loadTTFFromUTF8(mTexDialogTextMain.mPrompt + " " + mDialogTextMain +"_", mTextColor,mGlobalSettings.TFont);
	} else {
		mTexDialogTextMain.loadTTFFromUTF8(mTexDialogTextMain.mPrompt + " " + mDialogTextMain + " ", mTextColor,mGlobalSettings.TFont);
	}
	
	mDialogWidth = mTexDialogTextMain.mTexWidth;
	mDialogHeight = mTexDialogTextMain.mTexHeight;

	if(mMinDialogWidth > mDialogWidth) {
		mDialogWidth = mMinDialogWidth;
	}

	
	mDialogHeight += mDialogBorder * 3;
	
	mDialogWidth += mDialogBorder * 4;	
}

int TIDialog::recieveInput(int mx, int my){
	return BDialog::recieveInput(mx, my);
}

SDL_Rect TIDialog::render(int xpos, int ypos){
	init();
	mButtonRect = Dialog::render(xpos, ypos);
	int cTextX = xpos + (mDialogBorder *2);
	int cTextY = ypos + (mDialogBorder + (mDialogBorder/3));
	mTexDialogTextMain.render(cTextX, cTextY);
	
	return mButtonRect;
}

void TIDialog::recieveInput(std::string mTextInput){

}

void HDialog::init(){
//	mDialogTextInput = "Close";
//	mTexDialogTextInput.loadTTFFromString(mDialogTextInput, mTextColor);
	
	TTFTexture* mNewText = new TTFTexture();
	std::string tmpStr = mNewText->mBook + " UI Guide";
	mNewText->loadTTFFromUTF8(tmpStr, mTextColor, mGlobalSettings.LFont);
	mHelpText.push_back(mNewText);

	
	for(const auto& hStr : mGlobalSettings.mHelpText){
		mNewText = new TTFTexture();
		mNewText->loadTTFFromString(hStr, mTextColor);
		mHelpText.push_back(mNewText);
	}
	
	int cMaxWidth = mHelpText[0]->mTexWidth;

	for(const auto* hTex : mHelpText){
		if(hTex->mTexWidth > cMaxWidth){cMaxWidth = hTex->mTexWidth;}
	}
	
	
	mDialogWidth = cMaxWidth;
	mDialogHeight = mHelpText[1]->mTexHeight * mHelpText.size()*2 + mHelpText[1]->mTexHeight*4;
	
//	mDialogHeight *=4;
	
	mDialogWidth += mDialogBorder * 10;	
		
	mCloseButton.mDialogTextMain = "Close";
	mCloseButton.init();
}

void HDialog::recieveInput(int mKey){
	bInputIsCancel=true;
}

int HDialog::recieveInput(int mx, int my){
	if(mCloseButton.recieveInput(mx, my)){
		recieveInput(SDLK_n);
	}
	return 0;
}

SDL_Rect HDialog::render(int xpos, int ypos){
	SDL_Rect tmpBorder = Dialog::render(xpos, ypos);

	int cTextX = xpos + (3 * mDialogBorder);
	int cTextY = ypos + (3 * mDialogBorder);
		
	bool btfirst = true;
	for(auto* hTex : mHelpText){
		if(btfirst){
			hTex->render(xpos - (mDialogBorder) + ((mDialogWidth/2)-(hTex->mTexWidth/2)), cTextY);
			btfirst = false;
		} else {
			hTex->render(cTextX, cTextY);
		}
		cTextY += (hTex->mTexHeight * 2);
	}
	
	mCloseButton.render(xpos + ((mDialogWidth/2)-(mCloseButton.mDialogWidth/2)), cTextY - (mDialogBorder*1.5));
	
	return tmpBorder;
}

void PIDialog::update(){
	std::stringstream convert;
	
	std::string cFlipH;
	std::string cFlipV;
	
	convert << fliph << std::endl;
	convert >> cFlipH;

	convert << flipv << std::endl;
	convert >> cFlipV;


	std::string tmpStr = "FlipH: "+cFlipH + " FlipV: " + cFlipV;
	
	mHelpText[mHelpText.size()-1]->loadTTFFromUTF8(tmpStr, mTextColor);
	
	
	std::string cTileX,cTileY;
	int cx,cy;
	
	cx = mGlobalSettings.mSelectedTile % mGlobalSettings.TileMapWidth;
	cy = mGlobalSettings.mSelectedTile / mGlobalSettings.TileMapWidth;
	
	convert << cx << std::endl;
	convert >> cTileX;
	
	convert << cy << std::endl;
	convert >> cTileY;

	tmpStr = "Current Tile: (" + cTileX + "," + cTileY + ")";
	
	mHelpText[mHelpText.size()-2]->loadTTFFromUTF8(tmpStr, mTextColor);
}

void PIDialog::init(){
	
	
	TTFTexture* mNewText = new TTFTexture();
	std::string tmpStr = mNewText->mInfo + " Project: " + mGlobalSettings.ProjectPath;
	mNewText->loadTTFFromUTF8(tmpStr, mTextColor);//, mGlobalSettings.LFont);
	mHelpText.push_back(mNewText);

	std::stringstream convert;
	
	std::string cMapWidth;
	std::string cMapHeight;
	
	convert << mGlobalSettings.TileMapWidth << std::endl;
	convert >> cMapWidth;

	convert << mGlobalSettings.TileMapHeight << std::endl;
	convert >> cMapHeight;

	mNewText = new TTFTexture();
	tmpStr = "TileMap: " + cMapWidth + "x" + cMapHeight;
	mNewText->loadTTFFromUTF8(tmpStr, mTextColor);//, mGlobalSettings.LFont);
	mHelpText.push_back(mNewText);

	std::string cTileSize;
	convert << mGlobalSettings.TileSize << std::endl;
	convert >> cTileSize;

	mNewText = new TTFTexture();
	tmpStr = "TileSize: " + cTileSize;
	mNewText->loadTTFFromUTF8(tmpStr, mTextColor);//, mGlobalSettings.LFont);
	mHelpText.push_back(mNewText);


	mNewText = new TTFTexture();
	tmpStr = "Current Tile: (0,0)";
	mNewText->loadTTFFromUTF8(tmpStr, mTextColor);//, mGlobalSettings.LFont);
	mHelpText.push_back(mNewText);

	fliph=0;
	flipv=0;	

	mNewText = new TTFTexture();
	mHelpText.push_back(mNewText);
	update();
	
	int cMaxWidth = mHelpText[0]->mTexWidth;

	for(const auto* hTex : mHelpText){
		if(hTex->mTexWidth > cMaxWidth){cMaxWidth = hTex->mTexWidth;}
	}
	
	
	mDialogWidth = cMaxWidth;
	mDialogHeight = mHelpText.size()*mHelpText[0]->mTexHeight*2+mHelpText[0]->mTexHeight;
	
	mDialogWidth += mDialogBorder * 10;	
		
	mCloseButton.mDialogTextMain = "Close";
	mCloseButton.init();
}

SDL_Rect PIDialog::render(int xpos, int ypos){
	SDL_Rect tmpBorder = Dialog::render(xpos, ypos);

	int cTextX = xpos + (3 * mDialogBorder);
	int cTextY = ypos + (3 * mDialogBorder);
		
	for(auto* hTex : mHelpText){
		hTex->render(cTextX, cTextY);	
		cTextY += (hTex->mTexHeight*1.5);
	}
	
	mCloseButton.render(xpos + ((mDialogWidth/2)-(mCloseButton.mDialogWidth/2)), cTextY);
	
	return tmpBorder;
}

TileProperties TileMap::getTileProp(int cTile){
	TileProperties mCurTileProps;
	if((cTile > -1) && (cTile < TileAreas.size())){
		int tFlip = getFlip(cTile);
		mCurTileProps.bFlipX = (tFlip & 0x1);
		mCurTileProps.bFlipY = (tFlip & 0x2);
				
		mCurTileProps.mPaletteOffset = 0;
	}
	return mCurTileProps;
}

int TileMap::render(int xpos, int ypos, TileSet* mTiles){
	for(int i=0; i < mGlobalSettings.TileMapHeight; i++){
		for(int j=0; j < mGlobalSettings.TileMapWidth; j++){
			TileAreas[j+(i*mGlobalSettings.TileMapWidth)] = mTiles->TTiles[getTile(j+(i*mGlobalSettings.TileMapWidth))].render(xpos + (mGlobalSettings.TileSize * j * mGlobalSettings.TileMapScale), ypos + (mGlobalSettings.TileSize * i * mGlobalSettings.TileMapScale), mGlobalSettings.TileMapScale, getTileProp(j+(i*mGlobalSettings.TileMapWidth)));
			if(mGlobalSettings.bShowSelectedTile){
				if(mGlobalSettings.mSelectedTile == (j+(i*mGlobalSettings.TileMapWidth))){
					SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, 0xFF,0xFF,0xFF,0xff);
					SDL_RenderDrawRect(mGlobalSettings.TRenderer, &TileAreas[j+(i*mGlobalSettings.TileMapWidth)]);
				}
			}
		}
	}

	return 0;
}

int TEditor::createNewProject(){
	std::cout << "Creating Project" << std::endl;
	std::cout << "TileMapWidth: " << mGlobalSettings.TileMapWidth << " TileMapHeight: " << mGlobalSettings.TileMapHeight << " TileSize: " << mGlobalSettings.TileSize << std::endl;												
	return 1;
}

int TEditor::loadFromFolder(std::string path){
	mPalette.initPalette();
	mPalette.initTPixels();
		
		
	if(mTileSet.loadFromFolder(path, &mPalette)){
		std::cout << "Error loading tiles: " << path << std::endl;
		return 1;
	}

	if(mTileMap.loadFromFile(path, "map.bin")){
		std::cout << "Error: can't read: " << path << DIRDEL << "map.bin" << std::endl;
		return 1;
	}
	
	mGlobalSettings.ProjectPath = path;
	
	mMapSelectedTile = 0;
	mTileSelectedTile = &mTileSet.TTiles[0];
	mTileSelectedTile->bIsSelected = true;
	mColorSelectedTile = mPalette.TPixels[0];
	mColorSelectedTile->bPixelSelected = true;
	mColorSelected = 0;

	mLastAction = &mEmptyAction;
	mSaveDialog.init();
	mSaveAsDialog.mDialogTextInput = mTileMap.DataPath;
	mSaveAsDialog.init();
	mSaveAsDialog.mSubDialog = &mSaveDialog;
	mHelpDialog.init();
	mTopBar.mEditor = this;
	mTopBar.init();
	mProjectInfo.init();
//	mActiveDialog = &mSaveAsDialog;
	return 0;
}

int TEditor::saveToFolder(std::string path){

	fs::path cpath = path;
	if(!fs::is_directory(fs::status(cpath))){
		fs::create_directory(cpath);
	}
	
	mTileSet.saveToFolder(path);
	mTileMap.saveToFolder(path);

	return 0;
}

int TEditor::render(){
	mGlobalSettings.updateTicks();

	if(mCurMode == EMODE_MAP){
 		if(!mGlobalSettings.bShowTypeSelection) mTileSelectedTile->bIsSelected = false;		
		mTileMap.render(mTileMapScrollX,mGlobalSettings.TopBarHeight+mTileMapScrollY,&mTileSet);
		mTileSelectedTile->bIsSelected = true;
		//mTileSet.render(50,50+mTileSetScrollY);
		mTileSet.render(mGlobalSettings.TopBarHeight, mTileSetScrollY);
		if(mActiveDialog){
			mActiveDialog->render((mGlobalSettings.WindowWidth/2)-(mActiveDialog->mDialogWidth/2),(mGlobalSettings.WindowHeight/2)-(mActiveDialog->mDialogHeight/2));
		}
		mTopBar.render(0,0);
	}
	
	if(mCurMode == EMODE_TILE){
		mPalette.render(100+mGlobalSettings.mTileEdScale*mGlobalSettings.TileSize*mGlobalSettings.TileSize,50+mTopBar.mDialogHeight);	
		mColorSelectedTile->bPixelSelected = false;
		mTileSelectedTile->renderEd(50,50+mTopBar.mDialogHeight,&mPalette);
		mColorSelectedTile->bPixelSelected = true;
		if(mActiveDialog){
			mActiveDialog->render((mGlobalSettings.WindowWidth/2)-(mActiveDialog->mDialogWidth/2),(mGlobalSettings.WindowHeight/2)-(mActiveDialog->mDialogHeight/2));
		}
		mTopBar.render(0,0);
	}

	return 0;
}

int TEditor::switchMode(){
	if(mCurMode == EMODE_MAP){
		mCurMode = EMODE_TILE;
	} else {
		mCurMode = EMODE_MAP;
	}
	mTopBar.init();
	return 0;
}

int TEditor::searchRects(std::vector<SDL_Rect> &sRects){
	for(int i = 0; i < sRects.size(); i++){
		if( (mouseSelX >= sRects[i].x) && (mouseSelX <= (sRects[i].x + sRects[i].w)) && (mouseSelY >= sRects[i].y) && (mouseSelY <= (sRects[i].y + sRects[i].h)) ){
			return i;
		}
	}
	return -1;
}

int TEditor::applyScroll(int mx,int my, int amount, int xamount){
	int tSel = -1;
	mouseSelX = mx;
	mouseSelY = my;
	if(mCurMode == EMODE_MAP){ 
		if( mx > (mGlobalSettings.WindowWidth - mGlobalSettings.TileSetWidth)){
			mTileSetScrollY += amount*20;		
		} else {
			//if(!bTileMapGrapped && !bLCTRLisDown){
				if(amount > 0){mGlobalSettings.TileMapScale++; if(mGlobalSettings.TileMapScale > 10){mGlobalSettings.TileMapScale=10; }}	
				if(amount < 0){mGlobalSettings.TileMapScale--; if(mGlobalSettings.TileMapScale < 1){mGlobalSettings.TileMapScale=1; }}
			//}
		}
	} else {
	
	}
	
	return 0;
}

void TEditor::newActionGroup(){
	TEActionGroup *newGroup = new TEActionGroup();
	mUndoStack.push_back(newGroup);
}

void TEditor::addAction(TEAction *newAction){
	if(mUndoStack.size()){
		TEActionGroup *mGroup = *(mUndoStack.end()-1);
		mGroup->mActions.push_back(newAction);
	}
}

void TEditor::undoLastActionGroup(){
	if(mUndoStack.size()){
		TEActionGroup *mGroup = *(mUndoStack.end()-1);
		mUndoStack.pop_back();
		mRedoStack.push_back(mGroup);
		mGroup->undo();
		mLastAction = &mEmptyAction;
	}
}

void TEditor::redoLastActionGroup(){
	if(mRedoStack.size()){
		TEActionGroup *mGroup = *(mRedoStack.end()-1);
		mRedoStack.pop_back();
		mUndoStack.push_back(mGroup);
		mGroup->redo();
	}
}

void TEditor::redoClearStack(){
	if(mRedoStack.size()){
		mRedoStack.erase(mRedoStack.begin(), mRedoStack.end());
	}	
}

int TEditor::activateProjectInfo(){

	if(mCurMode == EMODE_MAP){
		if(mGlobalSettings.mSelectedTile > -1){
			int cTileFlip = mTileMap.getFlip(mGlobalSettings.mSelectedTile);

			mProjectInfo.fliph = (cTileFlip & 0x1) ? 1 : 0;
			mProjectInfo.flipv = (cTileFlip & 0x2) ? 1 : 0;
			mProjectInfo.update();
			
			mActiveDialog = &mProjectInfo;
		}
	}
	return 0;
}

int TEditor::activateHelpDialog(){
	mActiveDialog = &mHelpDialog;
	return 0;
}

int TEditor::activateSaveDialog(){
	mActiveDialog = &mSaveDialog;
//	mActiveDialog->bDialogIsWatingForInput = true;
	return 0;
}

int TEditor::activateSaveAsDialog(){
	mActiveDialog = &mSaveAsDialog;
	mActiveDialog->bDialogIsWatingForText = true;
	SDL_StartTextInput();
//	mActiveDialog->bDialogIsWatingForInput = true;
	return 0;
}

int TEditor::cancelActiveDialog(){
//	mActiveDialog->bDialogIsWatingForText = false;	
//	mActiveDialog->bInputIsAccept = false;
//	mActiveDialog->bInputIsCancel = false;
	mActiveDialog->cancel();
	mActiveDialog = NULL;
	return 0;
}

int TEditor::toggleSelectedTile(){
	if(mCurMode == EMODE_MAP){
		if(mGlobalSettings.mSelectedTile > -1){
			mGlobalSettings.bShowSelectedTile = !mGlobalSettings.bShowSelectedTile;
			return 0;
		}
	}
	return 1;
}
		
int TEditor::flipSelectedTile(){
	if(mCurMode == EMODE_MAP){
		if(mGlobalSettings.mSelectedTile > -1){
			int cTileFlip = mTileMap.getFlip(mGlobalSettings.mSelectedTile);
			std::cout << "Flipping Tile: " << cTileFlip << std::endl;		
			cTileFlip++;
			if(cTileFlip > 3){cTileFlip = 0;}
			mTileMap.setFlip(mGlobalSettings.mSelectedTile, cTileFlip);
			return 0;
		}
	}
	return 1;
}

int TEditor::findSelected(){
	int tSel = -1;
	if(mCurMode == EMODE_MAP){
		tSel = searchRects(mTileSet.TileAreas);
		if(tSel != -1){
   	 		mMapSelectedTile = tSel;
   	 		mTileSelectedTile->bIsSelected = false;
   	 		mTileSelectedTile = &mTileSet.TTiles[tSel];
   	 		mTileSelectedTile->bIsSelected = true;
		} else {
		     	tSel = searchRects(mTileMap.TileAreas);
	       		if(tSel != -1){
	       			mGlobalSettings.mSelectedTile = tSel;		  	
	       			TEActionReplaceTile *mCurAction = new TEActionReplaceTile();
	       			mCurAction->doAction(&mTileMap, tSel, mTileMap.getTile(tSel), mMapSelectedTile);
	       			
	       			if(!(*mCurAction == *mLastAction)){
	       				newActionGroup();	
	       				addAction(mCurAction);
	       				mLastAction = mCurAction;
	       				redoClearStack();
	       			}
	       		}
		}
	} else {
		tSel = searchRects(mPalette.PixelAreas);
		if(tSel != -1){
			mColorSelectedTile->bPixelSelected = false;
			mColorSelected = tSel;
			mColorSelectedTile = mPalette.TPixels[tSel];
			mColorSelectedTile->bPixelSelected = true;
		} else {
			tSel = searchRects(mTileSelectedTile->PixelAreas);
			if(tSel != -1){

	       			TEActionReplacePixel *mCurAction = new TEActionReplacePixel();
				mCurAction->doAction(mTileSelectedTile, tSel, mTileSelectedTile->FileData[tSel], mColorSelected, &mPalette);
				
				if(!(*mCurAction == *mLastAction)){
	       				newActionGroup();	
	       				addAction(mCurAction);
	       				mLastAction = mCurAction;
	       				redoClearStack();
	       			}
			}
		}
	}
	
	return 0;
}

int TEditor::handleEvents(){
	int rx,ry;
	int x,y;
	SDL_GetRelativeMouseState(&rx, &ry);
	int mButtonState = SDL_GetMouseState(&x, &y);
	int mapWidthX = mGlobalSettings.TileMapWidth*mGlobalSettings.TileSize*mGlobalSettings.TileMapScale;					
	int mapWidthY = mGlobalSettings.TileMapHeight*mGlobalSettings.TileSize*mGlobalSettings.TileMapScale;
	
	if(mButtonState & SDL_BUTTON(SDL_BUTTON_LEFT)){
		leftMouseButtonDown = true;
	} else {
		waitLeftMouseButton=false;
		leftMouseButtonDown = false;
		bTileSetGrapped = false;
		bTileMapGrapped = false;
	}

	
	if(mActiveDialog){
		if(mActiveDialog->bInputIsAccept){
			if(mGlobalSettings.mProjectSaveState = 1){
				saveToFolder(mGlobalSettings.ProjectPath);
				mGlobalSettings.mProjectSaveState = 0;
			}
			cancelActiveDialog();
			return 0;
		}
		if(mActiveDialog->bInputIsCancel){
			cancelActiveDialog();
			return 0;
		}
		if(leftMouseButtonDown){
			mActiveDialog->recieveInput(x,y);
			//leftMouseButtonDown=false;
			waitLeftMouseButton=true;
		}
		return 0;
	} else {
		if(waitLeftMouseButton){
			return 0;
		}
		
		if(leftMouseButtonDown){
			if(bLCTRLisDown){
				if(mCurMode == EMODE_MAP){
					if(bTileSetGrapped){
						mTileSetScrollY += ry * 2;				
					} else if(bTileMapGrapped){
						
						//if(mapWidthX > (mGlobalSettings.WindowWidth-mGlobalSettings.TileSetWidth)) {
							mTileMapScrollX += rx;// * 2;

							
						//}
						
						
						//if(mapWidthY > (mGlobalSettings.WindowHeight)) {
							mTileMapScrollY += ry;// * 2;

							
						//}
						

	
												
					} else {
						mouseSelX = x;
						mouseSelY = y;
						if(x > (mGlobalSettings.WindowWidth - mGlobalSettings.TileSetWidth)){
							bTileSetGrapped = true;
						} else {
							bTileMapGrapped = true;
						}
					}
				}
			} else {
				mouseSelX = x;
				mouseSelY = y;
	      			findSelected();
	      		}
		}
	}
	if(mTileMapScrollX > 0){mTileMapScrollX = 0;}
	if(mTileMapScrollY > 0){mTileMapScrollY = 0;}
	
	
	if(mTileMapScrollX < -(mapWidthX - (mGlobalSettings.WindowWidth-mGlobalSettings.TileSetWidth))){mTileMapScrollX = -(mapWidthX - (mGlobalSettings.WindowWidth-mGlobalSettings.TileSetWidth));}
	if(mTileMapScrollY < -(mapWidthY - (mGlobalSettings.WindowHeight - mGlobalSettings.TopBarHeight))){mTileMapScrollY = -(mapWidthY - (mGlobalSettings.WindowHeight - mGlobalSettings.TopBarHeight));}
	
	if(mapWidthX < (mGlobalSettings.WindowWidth-mGlobalSettings.TileSetWidth)){
		mTileMapScrollX = -(mapWidthX - (mGlobalSettings.WindowWidth-mGlobalSettings.TileSetWidth))/2;
	}
	
	if(mapWidthY < (mGlobalSettings.WindowHeight-mGlobalSettings.TopBarHeight)){
		mTileMapScrollY = -(mapWidthY - (mGlobalSettings.WindowHeight-mGlobalSettings.TopBarHeight))/2;
	}

	return 0;	
}

int TEditor::handleEvents(SDL_Event* cEvent){

	switch (cEvent->type){
		case SDL_TEXTINPUT:
	  		if(mActiveDialog){
				if(mActiveDialog->bDialogIsWatingForText){
					mActiveDialog->recieveInput(std::string(cEvent->text.text));
				}
			}
			break;
		case SDL_TEXTEDITING:
	  		if(mActiveDialog){
	  			std::cout << "EDIT: " << cEvent->edit.text << std::endl;
				//if(mActiveDialog->bDialogIsWatingForInput){				
				//	mActiveDialog->recieveInput(std::string(cEvent->edit.text));
				//}
			}
			break;
		case SDL_QUIT:
	  		bEditorRunning = false;
  			std::cout << "Shutting Down..." << std::endl;
  			break;
			
	  	case SDL_KEYDOWN:
	  		if(mActiveDialog){
	  			if(cEvent->key.keysym.sym == SDLK_BACKSPACE){
	  				if(mActiveDialog->bDialogIsWatingForText){				
	  					mActiveDialog->dropLastInputChar();
	  				}
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_RETURN){
	  				//if(mActiveDialog->bDialogIsWatingForText){				
	  					mActiveDialog->recieveInput(SDLK_y);
	  				//}
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_ESCAPE){
					mActiveDialog->recieveInput(SDLK_n); //cancelActiveDialog();
	  			}	
	  			//if(cEvent->key.keysym.sym == SDLK_y){
	  			//	std::cout << "mActiveDialog->receiveInput(SDLK_y)" << std::endl;
	  			//	if(!mActiveDialog->bDialogIsWatingForText){				
	  			//		mActiveDialog->recieveInput(SDLK_y);
	  			//	}
	  			//}
	  			//if(cEvent->key.keysym.sym == SDLK_n){
	  			//	std::cout << "mActiveDialog->receiveInput(SDLK_n)" << std::endl;
	  			//	if(!mActiveDialog->bDialogIsWatingForText){
	  			//		mActiveDialog->recieveInput(SDLK_n);
	  			//	}
	  			
	  		} else {
	  			if(cEvent->key.keysym.sym == SDLK_ESCAPE){
  		  			bEditorRunning = false;
	  				std::cout << "Shutting Down..." << std::endl;
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_SPACE){
	  				switchMode();
	  			}
				if(cEvent->key.keysym.sym == SDLK_s){
	  				mGlobalSettings.bShowTypeSelection = !mGlobalSettings.bShowTypeSelection;
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_p){
		  			mGlobalSettings.bShowPixelSelection = !mGlobalSettings.bShowPixelSelection;
	  			}	  			  		
	  			if(cEvent->key.keysym.sym == SDLK_LCTRL){
	  				bLCTRLisDown = true;
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_u){
					undoLastActionGroup();	  		
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_r){
		  			redoLastActionGroup();
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_f){
		  			flipSelectedTile();
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_t){
		  			toggleSelectedTile();
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_F1){
	  				std::cout << "activateHelpDialog()" << std::endl;
		  			activateHelpDialog();
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_F2){
	  				std::cout << "activateProjectInfo();" << std::endl;
		  			activateProjectInfo();
	  			}	  				  			
	  			if(cEvent->key.keysym.sym == SDLK_F12){
	  				std::cout << "activateSaveDialog()" << std::endl;
		  			activateSaveDialog();
	  			}
	  			if(cEvent->key.keysym.sym == SDLK_F11){
	  				std::cout << "activateSaveAsDialog()" << std::endl;
		  			activateSaveAsDialog();
	  			}	  			
			}

	  		break;
	  	case SDL_KEYUP:
	  		if(cEvent->key.keysym.sym == SDLK_LCTRL){
	  			bLCTRLisDown = false;
	  			bTileSetGrapped = false;
	  			bTileMapGrapped = false;
	  		}
	  		break;
	  	case SDL_MOUSEBUTTONDOWN:
	            	if (cEvent->button.button == SDL_BUTTON_LEFT){
	  			//leftMouseButtonDown = true;
			      	mouseSelX = cEvent->motion.x;
			      	mouseSelY = cEvent->motion.y;
			      	//if(!bLCTRLisDown) findSelected();
	    		}
	    		break;
	    	case SDL_MOUSEBUTTONUP:
	            	if (cEvent->button.button == SDL_BUTTON_LEFT){
	  			//leftMouseButtonDown = false;
	  			//bTileSetGrapped = false;
	  			//bTileMapGrapped = false;
	    		}
	    		break;
	    		
	    	case SDL_MOUSEWHEEL:
	    		int x,y;
			SDL_GetMouseState(&x, &y);
	    		applyScroll(x,y,cEvent->wheel.y,cEvent->wheel.x);
	    		break;

	}
	return 0;
}

static int resizeEventHandler(void* data, SDL_Event* event) {
  if (event->type == SDL_WINDOWEVENT &&
      event->window.event == SDL_WINDOWEVENT_RESIZED) {
      	SDL_Window* win = SDL_GetWindowFromID(event->window.windowID);
      	if(win != data){return 0;}
 	int newWidth,newHeight;
 	SDL_GetWindowSize(mGlobalSettings.TWindow, &newWidth, &newHeight);
 	mGlobalSettings.WindowWidth = newWidth;
 	mGlobalSettings.WindowHeight = newHeight; 
  }
  return 0;
}

int TSettings::initSettings(){

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	TWindow = SDL_CreateWindow( "Simple Tilemap Editor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WindowWidth, WindowHeight, SDL_WINDOW_SHOWN );
	if( TWindow == NULL ){
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	TRenderer = SDL_CreateRenderer( TWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if( TRenderer == NULL ){
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	
	SDL_SetRenderDrawColor( TRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

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

void TSettings::initHelpText(){

	mHelpText.push_back("Left Mouse Button: Select Tiles/Colors and place in TileMap or Tile");
	mHelpText.push_back("Mouse Scroll Wheel: Scale TileMap and Scroll TileSet");
	mHelpText.push_back("LCTRL + Left Mouse Button: Move TileMap and Scroll TileSet");
	mHelpText.push_back("Space: Switch between TileMap and Tile Editor");
	mHelpText.push_back("F: Flip Selected Tile");
	mHelpText.push_back("T: Toggle Show Selected Tile in TileMap Editor");									
	mHelpText.push_back("S: Toggle Show Selected Tile Type in TileMap Editor");
	mHelpText.push_back("P: Toggle Show Pixel Grid in Tile Editor");
	mHelpText.push_back("U: Undo last Action");
	mHelpText.push_back("R: Redo last Action");
}

void TSettings::printHelpText(){
	initHelpText();
	for(const auto& cStr : mHelpText){
		std::cout << cStr << std::endl;	
	}
}

void printUsage(){
		std::cout << "Usage:" << std::endl;	
		std::cout << "tilemaped -o <folder>" << std::endl;
		std::cout << "Or" << std::endl;	
		std::cout << "tilemaped -n <mapsize> <tilesize>" << std::endl;
		std::cout << "Fx: tilemaped -n 128 16" << std::endl;
		std::cout << "tilemaped -h (For help on UI usage)" << std::endl;		
}

int main( int argc, char* args[] )
{
	TEditor mEditor;
	std::stringstream mConvert;
	int nTileSize = 0;
	int nMapSize = 0;
	bool mCreateNewProject=false;
	
	if((argc == 1) || (argc == 2) || (argc > 5)){
		if((argc == 2) && (std::string(args[1]) == "-h")){
			mGlobalSettings.printHelpText();
		} else {
			printUsage();
		}
		return 0;
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
	
	if((argc == 4) || (argc == 5)){
		if(std::string(args[1]) != "-n"){
			printUsage();
			return 1;
		}
		
		mConvert << std::string(args[2]) << std::endl;
		mConvert >> nMapSize;
		
		if((nMapSize == 32) || (nMapSize == 64) || (nMapSize == 128) || (nMapSize == 256)){	
			mGlobalSettings.TileMapHeight = nMapSize;
			mGlobalSettings.TileMapWidth = nMapSize;						
		} else {
			std::cout << "Wrong TileMap Size!" << std::endl;
			std::cout << "Valid Values are: 32, 64, 128, 256" << std::endl;						
			return 1;
		}
		
		if(argc == 5){
			mConvert << std::string(args[3]) << std::endl;
			mConvert >> nMapSize;
		
			if((nMapSize == 32) || (nMapSize == 64) || (nMapSize == 128) || (nMapSize == 256)){	
				mGlobalSettings.TileMapHeight = nMapSize;
			} else {
				std::cout << "Wrong TileMap Size!" << std::endl;
				std::cout << "Valid Values are: 32, 64, 128, 256" << std::endl;						
				return 1;
			}			
		}

		if(argc == 5){mConvert << std::string(args[4]) << std::endl;} else {
			mConvert << std::string(args[3]) << std::endl;}
		
		mConvert >> nTileSize;
	
		if((nTileSize == 16) || (nTileSize == 8)){	
			mGlobalSettings.TileSize = nTileSize;
		} else {
			std::cout << "Wrong TileSize!" << std::endl;
			std::cout << "Valid Values are: 8, 16" << std::endl;						
			return 1;
		}
		mCreateNewProject=true;
		
	}
	
	if( mGlobalSettings.initSettings() ){
		std::cout << "SDL Init Failed!" << std::endl;
	}
	else
	{			
		if(mCreateNewProject){
			if(mEditor.createNewProject()){
				return 1;
			}		
		} else {
			if(mEditor.loadFromFolder(args[2])){
				return 1;
			}		
		}
	
		SDL_AddEventWatch(resizeEventHandler, mGlobalSettings.TWindow);
		
		SDL_Event e;

		while( mEditor.bEditorRunning ){

			SDL_SetRenderDrawColor(  mGlobalSettings.TRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			SDL_RenderClear( mGlobalSettings.TRenderer );
				
			mEditor.render();
			//mTestTex.render(50,50);
 
			SDL_RenderPresent( mGlobalSettings.TRenderer );
				
			while( SDL_PollEvent( &e ) != 0 ){
				mEditor.handleEvents(&e);
			}
			mEditor.handleEvents();

		}
		
	}

	return 0;
}
