#ifndef __TEACTION__
#define __TEACTION__

#include "TSettings.h"

class TEAction{
	public:
		~TEAction(){};
		int TEActionType=ACTION_EMPTY;
		virtual void undo();
		virtual void redo();
		bool operator==(const TEAction& rhs){ if(TEActionType == rhs.TEActionType){return this->doCompare(rhs);} else {return false;}}
		virtual bool doCompare(const TEAction& rhs){return false;}
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
			if(mSelection.size() == mCmp->mSelection.size()){
				for(int i = 0; i < mSelection.size(); i++){
					if(mSelection[i] != mCmp->mSelection[i]){
						return false;
					}
				}
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
		~TEActionReplacePixel(){};
		Tile* mCurrentTile;
		int mCurrentPixel;
		int mOldValue;
		int mNewValue;
		TPalette* mCurrentPalette;
		void doAction(Tile* mCurTile, int mCurPix, int mOld, int mNew, TPalette* mPal);	
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



class TEActionAddTile: public TEAction{
	public:
		~TEActionAddTile();
		Tile* mNewTile;
		Tile* mOldTile;
		TileSet *mTiles;
		TEditor *mEditor;
		int mOldMapTile;
		void doAction(Tile* cNewTile, TEditor* cEditor, TileSet *cTiles);	
		virtual void undo();
		virtual void redo();
		virtual bool doCompare(const TEAction& rhs){
			 return false;
		}
};


class TEActionDropTile: public TEActionAddTile{
	public:
		~TEActionDropTile(){}
		void doAction(Tile* cNewTile, TEditor* cEditor, TileSet *cTiles);	
		virtual void undo();
		virtual void redo();

};

#endif