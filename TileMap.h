#ifndef __TILEMAP__
#define __TILEMAP__

#include "TSettings.h"
#include "TEAction.h"
#include "TDialogs.h"
#include "TSelection.h"
#include "TCollision.h"

class TTexture{
	public:
		TextureParameters *mTexParam;
		TTexture(){mTexParam = &mGlobalSettings.mGlobalTexParam;}
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
		unsigned char getPixel(int pindex, int poffset, int tflip);
		int getFlipIndex(int pindex, int tflip);
		int setPixel(int pindex, unsigned char pcolor);
		int setPixel(int pindex, unsigned char pcolor, std::vector<unsigned char> &tBuf);
		static int setPixel(int pindex, unsigned char pcolor, std::vector<unsigned char> &tBuf, TextureParameters *cTexParam);		
		//SDL_Rect render(int xpos, int ypos, int tscale=1, bool updateRect=false ,bool drawGrid=false);
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
		//SDL_Rect render(int xpos, int ypos, int tscale=1, bool updateRect=false ,bool drawGrid=false);
		SDL_Rect renderEd(int xpos, int ypos, TextureParameters *mTexParam, int tscale=1, bool updateRect=false ,bool drawGrid=false);
		SDL_Rect renderEdSel(int xpos, int ypos, int tscale, bool drawGrid=false);
		bool bPixelSelected = false;
		bool bPixelSelectedEdit = false;

};


class TPalette : public Dialog{
	public:
		std::vector<SDL_Color> TPalette;
		std::vector<SDL_Color> TPaletteEdit;
		ImVec4 mEditColor;
		ImVec4 mEditColorPick;
		bool bIsPickerOpen = false;
		void setEditColor();
		int mR;
		int mG;
		int mB;
		bool bUpdateEditColor = true;
		bool bHasBackupColor = false;
		int mBackupColor = 0;
		int initPalette();
		int loadFromFile(std::string palPath);
		int saveToFolder(std::string palPath);
		int saveToFile(std::string palPath);
		void close();
		int importGimpPalette(std::string palPath);
		int importPaletteEdit(std::string palPath);
		bool bImportingPalette = false;		
		std::vector<TPixel*> TPixels;
		std::vector<TPixel*> TPixelsEdit;
		std::vector<SDL_Rect> PixelAreas;
		int initTPixels();
		int updateTPixels();
		Uint32 mapPaletteColor(int tcolor);
		ImU32 getImColor(SDL_Color cColor);	
		ImVec4 getIm4Color(SDL_Color cColor);
		SDL_Color getSDLColor(ImVec4 cCol);
		SDL_Color getSDLColor4Bit(int cR, int cG, int cB);
		int updatePalette();
		int renderEditor(int xpos, int ypos);
		int renderIm(int xpos, int ypos, TextureParameters *mTexParam);
		//int render(int xpos, int ypos);
		SDL_Rect renderTileEd(int xpos,int ypos, int tcolor, TextureParameters *mTexParam);
		SDL_Rect renderSpriteEd(int xpos,int ypos, int tcolor, TextureParameters *mTexParam);
		SDL_Rect renderTileEd(int xpos,int ypos, int tcolor, int cScale, TextureParameters *mTexParam);
		SDL_Rect renderSelEd(int xpos,int ypos, int tcolor, int cScale);
		std::vector<std::string> mMapColorVals = {"%d/0","%d/17","%d/34","%d/51","%d/68","%d/85","%d/102","%d/119","%d/136","%d/153","%d/170","%d/187","%d/204","%d/221","%d/238","%d/255"};
		std::map<int,int> mMapColorIn = {{0,0},{1,17},{2,34},{3,51},{4,68},{5,85},{6,102},{7,119},{8,136},{9,153},{10,170},{11,187},{12,204},{13,221},{14,238},{15,255}};
		std::map<int,int> mMapColorOut = {{0,0},{17,1},{34,2},{51,3},{68,4},{85,5},{102,6},{119,7},{136,8},{153,9},{170,10},{187,11},{204,12},{221,13},{238,14},{255,15}};
};




class Tile: public TTexture{
	public:		
		~Tile();
		void freeTexture();
		int initTile();
		int initTexture();
		std::vector<SDL_Texture*> TPOffset;		
		std::vector<SDL_Rect> PixelAreas;		
		//SDL_Rect render(int xpos, int ypos, int tscale=1, bool updateRect=false ,bool drawGrid=false);
		SDL_Rect renderIm(int xpos, int ypos, int mIndex, int &mDragAndDropped, int tscale=1, bool updateRect=false ,bool drawGrid=false);
		SDL_Rect renderImCol(int xpos, int ypos, int mIndex, int tscale, bool bColEditSelected);
		SDL_Rect render(int xpos, int ypos, int tscale, TileProperties tProps);
		void renderEd(int xpos, int ypos, TPalette* tpal);
		void renderEdSel(int xpos, int ypos, TPalette* tpal, int cScale);
		static int renderSelection(SDL_Rect &sRect, SDL_Color sColor);		
		int updateTexture(TPalette* tpal);
		int recreateTexture(TPalette* tpal);
		int loadFromFile(std::string filename,TPalette* tpal);
		int loadFromBuffer(std::vector<unsigned char> &cTileBuf,TPalette* tpal);	
		int replaceWithBuffer(std::vector<unsigned char> &cTileBuf,TPalette* tpal);
		int createNew(TPalette* tpal);
		int rotater();
		int rotatel();
		int rotate(double cAngle);
		int scale(double cScale);
		int applyFilter();
		bool bIsSelected = false;
		const static int MaxScale = 24;
        const static int MinScale = 4;
		TEActionUndoStack mActionStack;
		TSelection mSelection;		
};	

class TileSet{
	public:
		void shutdown();
		void close();
		std::string DataPath;
		std::vector<Tile*> TTiles;
		std::vector<SDL_Rect> TileAreas;
		std::vector<SDL_Rect> EditPixelAreas;	
		void resizeEdit();
		void updateEditAreas(std::vector<SDL_Rect> &cTile, int xpos, int ypos);
		int getXY(int xpos, int ypos, int cxpos, int cypos);
		std::vector<int> getPadding();
		int mCurTileScale=10;
		int mCurColumns=1;
		int mColSpace = 10;
		int mMaxScrollY=0;
		int mSelEdWidth=4;
		int mCurEdScale=10;
		int mPixelScale = 1;
		const static int MaxScale = 24;
        const static int MinScale = 4;
		const static int MaxGrid = 16;
        const static int MinGrid = 2;
		int mSelectionAreaX;
		int mSelectionAreaY;
		int reCalculateScale();
		SDL_Rect mTileSetBackGround;
		int loadFromFile(std::string tpath,std::string tfile, TPalette* tpal);
		int saveToFolder(std::string tpath);
		Tile* createNew(TPalette* tpal);
		Tile* createNewCopy(Tile* cCopyTile, TPalette* tpal);
		Tile* createNewFromBuffer(std::vector<unsigned char> &newBuf, TPalette* tpal);
		Tile* createNewFromFile(std::string newPAth, TPalette* tpal);
		int importFile(std::string tpath,std::string tfile, TPalette* tpal);
		int importPNG(SDL_Surface *cTiles, TPalette* tpal);
		int importTileSet(std::string cTilePath, std::vector<Tile*> &cNewTiles);
		void dropLastTile();
		int deleteTile(int cDropTile);
		int removeTile(int cDropTile);
		void appendTile(Tile* addTile);		
		//int render(int ypos, int mScroll);
		int renderEd(int xpos, int ypos);
		int renderIm(int ypos, int mScroll);		
		bool updateWinPos=false;
		bool bUpdateEditSelection = false;
		bool bUpdateEditSelectionScale = false;
		std::map<int, int> mTileSizeIn = {{0,8},{1,16}};
		std::map<int, int> mTileSizeOut = {{8,0},{16,1}};
		TSelection mSelection;
		TEActionUndoStack mActionStack;
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
		bool bIsSavedToFile = false;
		int TileMapHeight=32;
		int TileMapWidth=32;
		int mMapSelectedTile=0;	
		int TileMapScale=3;
		int mTileMapScrollX = 0;
		int mTileMapScrollY = 0;
		void init();
		std::string getMapSize();
		std::vector<unsigned char> FileData;
		int getTile(int cTile);
		int setTile(int cTile, int cTileVal);
		int getFlip(int cTile);
		int setFlip(int cTile, int cTileFlip);
		int getOffset(int cTile);
		int setOffset(int cTile, int cOffset);
		void swapTileValues(int tVal1, int tVal2);
		TileProperties getTileProp(int cTile);
		std::vector<SDL_Rect> TileAreas;
		int removeTile(int cDropTile);
		int loadFromFile(std::string path, std::string filename, bool bLoadColMap = false);
		int loadFromFileOffset(std::string path, std::string filename, int cTileOffset);
		int loadFromFileOffset(std::string path, std::string filename, int cTileOffset, int cPaletteOffset);
		int saveToFolder(std::string tpath, std::string tfile);
		int createNew();
		int createNew(int nWidth, int nHeight, int nSValue);
		int createNew(int nWidth, int nHeight, int nSValue, int cPaletteOffset);
		int createCollisionMap();
		int removeCollisionMap();
		int render(int xpos,int ypos,TileSet* mTiles);
		std::map<int,int> mTilemapSizesIn = {{0,32},{1,64},{2,128},{3,256}};
		std::map<int,int> mTilemapSizesOut = {{32,0},{64,1},{128,2},{256,3}};
		SDL_Rect mBorder;     
		TSelection mSelection; 
		bool bHasCollisionMap = false;
		TCollisionMap mColMap;
};

#endif