#ifndef __TEACTION__
#define __TEACTION__

#include "TSettings.h"
//#include "TSelection.h"

class TSelection;
class TBrushList;
class TBrush;


class TEAction{
	public:
		virtual ~TEAction(){};
		int TEActionType=ACTION_EMPTY;
		virtual void undo();
		virtual void redo();
		bool operator==(const TEAction& rhs){ if(TEActionType == rhs.TEActionType){return this->doCompare(rhs);} else {return false;}}
		virtual bool doCompare(const TEAction& rhs){return false;}
		virtual bool checkTileMap();
		virtual int getTileMap();
		virtual bool checkFrame();
		virtual int getFrame();
		virtual void swapTileValues(int tVal1, int tVal2);
};

class TEActionUndoStack{
	public:
		TEActionUndoStack();
		~TEActionUndoStack();		
		std::vector<TEActionGroup*> mUndoStack;
		std::vector<TEActionGroup*> mRedoStack;
		TEAction mEmptyAction;
		TEAction *mLastAction;
		void newActionGroup();
		void addAction(TEAction *newAction);
		void addSubActions(std::vector<TEAction*> &cSubActions);
		void undoLastActionGroup();
		void redoLastActionGroup();
		void redoClearStack();
		void undoClearStack();
		void dropLastGroup();
};


class TEActionGroup{
	public:
		std::vector<TEAction*> mActions;
		void undo();
		void redo();		
};

class TEActionReplaceMany: public TEAction{
	public:
		~TEActionReplaceMany(){};
		std::vector<int> mSelection;
		std::vector<TEAction*> mSubActions;
		
		bool compareSelection(TEActionReplaceMany *mCmp){

			if(mSelection == mCmp->mSelection){
				return true;
			}

			return false;
		}
};

class TEActionReplaceTile: public TEAction{
	public:
		~TEActionReplaceTile(){};
		int mCurrentTile;
		int mOldValue;
		int mNewValue;
		int mOldOffset;
		int mNewOffset;
		TileMap *mTileMap;
		virtual void swapTileValues(int tVal1, int tVal2);
		void doAction(TileMap *cTileMap, int mCurTile, int mOld, int mNew);	
		virtual void undo();
		virtual void redo();
		virtual bool checkTileMap();
		virtual int getTileMap();
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

class TEActionReplaceTileFlip: public TEActionReplaceTile{
	public:
		~TEActionReplaceTileFlip(){};
		int mOldFlip;
		int mNewFlip;
		virtual void swapTileValues(int tVal1, int tVal2);
		void doAction(TileMap *cTileMap, int mCurTile, int mOld, int mNew, int cFlipOld, int cFlipNew, int cOffset = -1);	
		virtual void undo();
		virtual void redo();
		virtual bool checkTileMap();
		virtual int getTileMap();
		virtual bool doCompare(const TEAction& rhs){
			const TEActionReplaceTileFlip* mrhs =  dynamic_cast<const TEActionReplaceTileFlip*>(&rhs); 
			if(mrhs){
				if((mCurrentTile == mrhs->mCurrentTile) &&  (mNewValue == mrhs->mNewValue) &&  (mNewFlip == mrhs->mNewFlip)){ // Not testing mOldValue
					return true;
				}
			}	
			 return false;
		}
};

class TEActionReplacePixel: public TEAction{
	public:
		~TEActionReplacePixel(){};
		Tile* mCurrentTile;
		TileMap* mCurrentTileMap;
		TSFrame* mCurrentFrame;		
		int mCurrentPixel;
		int mOldValue;
		int mNewValue;
		TPalette* mCurrentPalette;
		void doAction(Tile* mCurTile, int mCurPix, int mOld, int mNew, TPalette* mPal);	
		virtual void undo();
		virtual void redo();
		virtual bool checkTileMap();
		virtual int getTileMap();
		virtual bool checkFrame();
		virtual int getFrame();
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

class TEActionReplacePixels: public TEActionReplaceMany{
	public:
		~TEActionReplacePixels(){};
		Tile* mCurrentTile;		
		int mOldValue;
		int mNewValue;
		TPalette* mCurrentPalette;
		void doAction(Tile* mCurTile, std::vector<int> &newSel, int mOldColor, int mNewColor, TPalette* mPal);	
		
		virtual bool doCompare(const TEAction& rhs){
			const TEActionReplacePixels* mrhs =  dynamic_cast<const TEActionReplacePixels*>(&rhs); 
			if(mrhs){
				if((mCurrentTile == mrhs->mCurrentTile) &&  (mNewValue == mrhs->mNewValue)){ // Not testing mOldValue
					return compareSelection((TEActionReplaceMany*)mrhs);
				}
			}	
			 return false;
		}
};


class TEActionReplacePixelsSel: public TEActionReplaceMany{
	public:
		~TEActionReplacePixelsSel(){};		
		int mNewValue;
		TPalette* mCurrentPalette;
		TileSet *mTileSet;
		void doAction(TileSet *cTileSet, TSelection &mSelect, int mAreaX, int mAreaY, int cNewColor, TPalette* mPal);	
		
		virtual bool doCompare(const TEAction& rhs){
			const TEActionReplacePixelsSel* mrhs =  dynamic_cast<const TEActionReplacePixelsSel*>(&rhs); 
			if(mrhs){
				if((mNewValue == mrhs->mNewValue)){ // Not testing mOldValue
					return compareSelection((TEActionReplaceMany*)mrhs);
				}
			}	
			 return false;
		}
};


class TEActionReplaceTiles: public TEActionReplaceMany{
	public:	
		~TEActionReplaceTiles(){};
		int mOldValue;
		int mNewValue;
		TileMap *mTileMap;
		void doAction(TileMap* cTileMap, std::vector<int> &newSel, int mOld, int mNew);	
		
		virtual bool doCompare(const TEAction& rhs){
			const TEActionReplaceTiles* mrhs =  dynamic_cast<const TEActionReplaceTiles*>(&rhs); 
			if(mrhs){
				if((mNewValue == mrhs->mNewValue)){ // Not testing mOldValue
					return compareSelection((TEActionReplaceMany*)mrhs);
				}
			}	
			 return false;
		}
};


class TEActionBrushTiles: public TEActionReplaceMany{
	public:	
		~TEActionBrushTiles(){};		
		std::vector<int> mNewValues;
		int UUID;
		TileMap *mTileMap;
		void doAction(TileMap* cTileMap, TBrush &mBrush);	
		
		virtual bool doCompare(const TEAction& rhs){
			const TEActionBrushTiles* mrhs =  dynamic_cast<const TEActionBrushTiles*>(&rhs); 
			if(mrhs){
				if((mNewValues == mrhs->mNewValues) && (UUID ==mrhs->UUID )){ // Not testing mOldValue
					return compareSelection((TEActionReplaceMany*)mrhs);
				}
			}	
			 return false;
		}
};

class TEActionBrushPixels: public TEActionReplaceMany{
	public:	
		~TEActionBrushPixels(){};		
		std::vector<int> mNewValues;
		int UUID;
		Tile *mTile;
		TPalette* mPalette;
		void doAction(Tile* cTile, TBrush &mBrush, TPalette* cPalette);	
		
		virtual bool doCompare(const TEAction& rhs){
			const TEActionBrushPixels* mrhs =  dynamic_cast<const TEActionBrushPixels*>(&rhs); 
			if(mrhs){
				if((mNewValues == mrhs->mNewValues) && (UUID ==mrhs->UUID )){ // Not testing mOldValue
					return compareSelection((TEActionReplaceMany*)mrhs);
				}
			}	
			 return false;
		}
};

class TEActionBrushPixelsTileSet: public TEActionReplaceMany{
	public:	
		~TEActionBrushPixelsTileSet(){};		
		std::vector<int> mNewValues;
		int UUID;
		TileSet *mTileSet;
		TPalette* mPalette;
		void doAction(TileSet* cTileSet, TBrush &mBrush, int mAreaX, int mAreaY, TPalette* cPalette);	
		
		virtual bool doCompare(const TEAction& rhs){
			const TEActionBrushPixelsTileSet* mrhs =  dynamic_cast<const TEActionBrushPixelsTileSet*>(&rhs); 
			if(mrhs){
				if((mNewValues == mrhs->mNewValues) && (UUID ==mrhs->UUID )){ // Not testing mOldValue
					return compareSelection((TEActionReplaceMany*)mrhs);
				}
			}	
			 return false;
		}
};

class TEActionBrushPixelsSelEdit: public TEActionReplaceMany{
	public:	
		~TEActionBrushPixelsSelEdit(){};		
		std::vector<int> mNewValues;
		int UUID;
		TileMap *mTileMap;
		TileSet *mTileSet;
		TPalette* mPalette;
		void doAction(TileMap* cTileMap, TileSet* cTileSet, std::vector<int> &mSelected, TBrush &mBrush, int mAreaX, int mAreaY, TPalette* cPalette);	
		
		virtual bool doCompare(const TEAction& rhs){
			const TEActionBrushPixelsSelEdit* mrhs =  dynamic_cast<const TEActionBrushPixelsSelEdit*>(&rhs); 
			if(mrhs){
				if((mNewValues == mrhs->mNewValues) && (UUID ==mrhs->UUID )){ // Not testing mOldValue
					return compareSelection((TEActionReplaceMany*)mrhs);
				}
			}	
			 return false;
		}
};

class TEActionAddTile: public TEAction{
	public:
		~TEActionAddTile();
		Tile* mNewTile;
		Tile* mOldTile;
		TileSet *mTiles;
		TEditor *mEditor;
		int mOldMapTile;
		int mTileIndex;
		void doAction(Tile* cNewTile, TEditor* cEditor, TileSet *cTiles);	
		virtual void undo();
		virtual void redo();
		virtual bool doCompare(const TEAction& rhs){
			 return false;
		}
};

class TEActionSwapTiles: public TEAction{
	public:
		~TEActionSwapTiles();
		int mSource;
		int mTarget;
		int mOldSelTile;
		TileSet *mTiles;
		TEditor *mEditor;
		bool bUpdateTileMaps;		
		void doAction(TEditor* cEditor, TileSet *cTiles, int cSource, int cTarget, bool cUpdate);	
		virtual void undo();
		virtual void redo();
		virtual bool doCompare(const TEAction& rhs){
			const TEActionSwapTiles* mrhs =  dynamic_cast<const TEActionSwapTiles*>(&rhs); 
			if(mrhs){
				if((mSource == mrhs->mSource) && (mTarget == mrhs->mTarget) && (bUpdateTileMaps == mrhs->bUpdateTileMaps)){ // Not testing mOldValue
					return true;
				}
			}
			 return false;
		}
};

class TEActionMoveTile: public TEAction{
	public:
		~TEActionMoveTile();
		int mSource;
		int mTarget;
		int mOldSelTile;
		TileSet *mTiles;
		TEditor *mEditor;
		bool bUpdateTileMaps;		
		void doAction(TEditor* cEditor, TileSet *cTiles, int cSource, int cTarget, bool cUpdate);	
		virtual void undo();
		virtual void redo();
		virtual bool doCompare(const TEAction& rhs){
			const TEActionMoveTile* mrhs =  dynamic_cast<const TEActionMoveTile*>(&rhs); 
			if(mrhs){
				if((mSource == mrhs->mSource) && (mTarget == mrhs->mTarget) && (bUpdateTileMaps == mrhs->bUpdateTileMaps)){ // Not testing mOldValue
					return true;
				}
			}
			 return false;
		}
};

class TEActionReplaceTileSet: public TEAction{
	public:
		~TEActionReplaceTileSet();
		Tile* mTile;
		std::vector<unsigned char> mTileBuf;
		std::vector<unsigned char> mOldBuf;
		TileSet *mTiles;
		TEditor *mEditor;		
		void doAction(Tile* cTile, std::vector<unsigned char> &cTileBuf, TEditor* cEditor, TileSet *cTiles);	
		virtual void undo();
		virtual void redo();
		virtual bool doCompare(const TEAction& rhs){
			 return false;
		}
};

class TEActionAddTiles: public TEAction{
	public:
		~TEActionAddTiles();
		Tile* mNewTile;
		Tile* mOldTile;
		TileSet *mTiles;
		TEditor *mEditor;
		int mOldMapTile;
		int mTileIndex;
		void doAction(Tile* cNewTile, TEditor* cEditor, TileSet *cTiles);	
		virtual void undo();
		virtual void redo();
		virtual bool doCompare(const TEAction& rhs){
			 return false;
		}
};

class TEActionAddFrame: public TEAction{
	public:
		~TEActionAddFrame();
		TSFrame* mNewFrame;
		TSFrame* mOldFrame;
		TSprite *mSprite;
		TEditor *mEditor;		
		int mFrameIndex;
		int mOldFrameIndex;
		void doAction(TSFrame* cNewFrame, TEditor* cEditor, TSprite *cSprite);	
		virtual void undo();
		virtual void redo();
		//virtual bool checkFrame();
		//virtual int getFrame();
		virtual bool doCompare(const TEAction& rhs){
			 return false;
		}
};

class TEActionAddFrames: public TEAction{
	public:
		~TEActionAddFrames();
		TSFrame* mNewFrame;
		TSFrame* mOldFrame;
		TSprite *mSprite;
		TEditor *mEditor;		
		int mFrameIndex;
		void doAction(TSFrame* cNewFrame, TEditor* cEditor, TSprite *cSprite);	
		virtual void undo();
		virtual void redo();
		virtual bool doCompare(const TEAction& rhs){
			 return false;
		}
};


class TEActionSwapFrames: public TEAction{
	public:
		~TEActionSwapFrames();
		int mSource;
		int mTarget;
		int mOldSelFrame;
		TSprite *mSprite;
		TEditor *mEditor;		
		void doAction(TEditor* cEditor, TSprite *cSprite, int cSource, int cTarget);	
		virtual void undo();
		virtual void redo();
		virtual bool doCompare(const TEAction& rhs){
			const TEActionSwapFrames* mrhs =  dynamic_cast<const TEActionSwapFrames*>(&rhs); 
			if(mrhs){
				if((mSource == mrhs->mSource) && (mTarget == mrhs->mTarget)){ // Not testing mOldValue
					return true;
				}
			}
			return false;
		}
};

class TEActionReplaceFrame: public TEAction{
	public:
		~TEActionReplaceFrame();
		TSFrame* mFrame;
		std::vector<unsigned char> mFrameBuf;
		std::vector<unsigned char> mOldBuf;
		TSprite *mSprite;
		TEditor *mEditor;		
		void doAction(TSFrame* cFrame, std::vector<unsigned char> &cFrameBuf, TEditor* cEditor, TSprite *cSprite);	
		virtual void undo();
		virtual void redo();
		virtual bool checkFrame();
		virtual int getFrame();
		virtual bool doCompare(const TEAction& rhs){
			return false;
		}
};


#endif