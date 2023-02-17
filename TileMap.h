#ifndef __TILEMAP__
#define __TILEMAP__

#include "TSettings.h"
#include "TEAction.h"
#include "TDialogs.h"
#include "TSelection.h"


class TTexture{
	public:
		~TTexture();
		SDL_Texture *TileTex = NULL;
		std::vector<unsigned char> FileData;
		std::vector<Uint32> PixelData;
		SDL_Rect CurrentArea;
		int initTexture();
		int updateTexture(TPalette* tpal);
		int loadFromFile(std::string filename,TPalette* tpal);
		int loadFromBuffer(std::vector<unsigned char> &cTileBuf,TPalette* tpal);		
		int saveToFile(std::string filename);
		int setPixel(int xpos, int ypos, unsigned char tcolor, TPalette* tpal);
		unsigned char getPixel(int pindex);
		unsigned char getPixel(int pindex, int poffset);
		int setPixel(int pindex, unsigned char pcolor);
		//int setAllPixels(unsigned char tcolor, TPalette* tpal);
		SDL_Rect render(int xpos, int ypos, int tscale=1, bool updateRect=false ,bool drawGrid=false);
		SDL_Rect renderEx(int xpos, int ypos, int tscale, SDL_RendererFlip flip);
		void renderEd(int xpos, int ypos, TPalette* tpal);
		bool bPixelSelected = false;
};



class TPixel{
	public:
		int PixelIndex;
		SDL_Color PixelColor;
		SDL_Rect CurrentArea;
		int setPixelColor(unsigned char tcolor, TPalette* tpal);
		SDL_Rect renderIm(int xpos, int ypos, int tscale=1, bool updateRect=false ,bool drawGrid=false);
		SDL_Rect renderImDisabled(int xpos, int ypos, int tscale=1, bool updateRect=false ,bool drawGrid=false);
		SDL_Rect renderEditor(int xpos, int ypos, int tscale=1, bool updateRect=false ,bool drawGrid=false);
		SDL_Rect render(int xpos, int ypos, int tscale=1, bool updateRect=false ,bool drawGrid=false);
		SDL_Rect renderEd(int xpos, int ypos, int tscale=1, bool updateRect=false ,bool drawGrid=false);
		bool bPixelSelected = false;
		bool bPixelSelectedEdit = false;

};



class TPalette : public Dialog{
	public:
		std::vector<SDL_Color> TPalette;
		std::vector<SDL_Color> TPaletteEdit;
		ImVec4 mEditColor;
		bool bUpdateEditColor = true;	
		int initPalette();
		int loadFromFile(std::string palPath);
		int saveToFolder(std::string palPath);
		int saveToFile(std::string palPath);
		int importGimpPalette(std::string palPath);
		int importPaletteEdit(std::string palPath);
		bool bImportingPalette = false;
		//int testPaletteFile(std::string palPath);
		std::vector<TPixel*> TPixels;
		std::vector<TPixel*> TPixelsEdit;
		std::vector<SDL_Rect> PixelAreas;
		int initTPixels();
		int updateTPixels();
		Uint32 mapPaletteColor(int tcolor);
		ImU32 getImColor(SDL_Color cColor);	
		ImVec4 getIm4Color(SDL_Color cColor);
		SDL_Color getSDLColor(ImVec4 cCol);
		int updatePalette();
		int renderEditor(int xpos, int ypos);
		int renderIm(int xpos, int ypos);
		int render(int xpos, int ypos);
		SDL_Rect renderTileEd(int xpos,int ypos, int tcolor);
		std::map<int,int> mMapColorIn = {{0,0},{1,17},{2,34},{3,51},{4,68},{5,85},{6,102},{7,119},{8,136},{9,153},{10,170},{11,187},{12,204},{13,221},{14,238},{15,255}};
		std::map<int,int> mMapColorOut = {{0,0},{17,1},{34,2},{51,3},{68,4},{85,5},{102,6},{119,7},{136,8},{153,9},{170,10},{187,11},{204,12},{221,13},{238,14},{255,15}};
};




class Tile: public TTexture{
	public:		
		~Tile();
		int initTile();
		int initTexture();
		std::vector<SDL_Texture*> TPOffset;		
		std::vector<SDL_Rect> PixelAreas;		
		SDL_Rect render(int xpos, int ypos, int tscale=1, bool updateRect=false ,bool drawGrid=false);
		SDL_Rect renderIm(int xpos, int ypos, int tscale=1, bool updateRect=false ,bool drawGrid=false);
		SDL_Rect render(int xpos, int ypos, int tscale, TileProperties tProps);
		void renderEd(int xpos, int ypos, TPalette* tpal);
		static int renderSelection(SDL_Rect &sRect, SDL_Color sColor);
		int updateTexture(TPalette* tpal);
		int loadFromFile(std::string filename,TPalette* tpal);
		int loadFromBuffer(std::vector<unsigned char> &cTileBuf,TPalette* tpal);	
		int createNew(TPalette* tpal);	
		bool bIsSelected = false;
		TEActionUndoStack mActionStack;
		TSelection mSelection;
};	

class TileSet{
	public:
		void shutdown();
		std::string DataPath;
		std::vector<Tile*> TTiles;
		std::vector<SDL_Rect> TileAreas;
		int mCurTileScale=10;
		int mCurColumns=1;
		int mColSpace = 10;
		int mMaxScrollY=0;
		int reCalculateScale();
		SDL_Rect mTileSetBackGround;
		int loadFromFolder(std::string tpath, TPalette* tpal);
		int saveToFolder(std::string tpath);
		Tile* createNew(TPalette* tpal);
		Tile* createNewCopy(Tile* cCopyTile, TPalette* tpal);
		Tile* createNewFromBuffer(std::vector<unsigned char> &newBuf, TPalette* tpal);
		Tile* createNewFromFile(std::string newPAth, TPalette* tpal);
		void dropLastTile();
		int removeTile(int cDropTile);
		void appendTile(Tile* addTile);
		int render(int ypos, int mScroll);
		int renderIm(int ypos, int mScroll);		
		bool updateWinPos=false;
		std::map<int, int> mTileSizeIn = {{0,8},{1,16}};
		std::map<int, int> mTileSizeOut = {{8,0},{16,1}};
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
		int getOffset(int cTile);
		int setOffset(int cTile, int cOffset);
		TileProperties getTileProp(int cTile);
		std::vector<SDL_Rect> TileAreas;
		int removeTile(int cDropTile);
		int loadFromFile(std::string path, std::string filename);
		int saveToFolder(std::string tpath);
		int createNew();
		int render(int xpos,int ypos,TileSet* mTiles);
		std::map<int,int> mTilemapSizesIn = {{0,32},{1,64},{2,128},{3,256}};
		std::map<int,int> mTilemapSizesOut = {{32,0},{64,1},{128,2},{256,3}};
};

#endif