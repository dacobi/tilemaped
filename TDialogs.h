#ifndef __TDIALOGS__
#define __TDIALOGS__

#include "TSettings.h"
//#include "TTFTexture.h"

extern TSettings mGlobalSettings;

class DialogElement;
class DialogValueBase;
class DialogButton;
class DialogValueFile;
class TIDialog;

class DTDialog;

class Dialog{
	public:
		SDL_Color mTextColor = mGlobalSettings.DefaultTextColor;
		std::string mDialogTextMain;		
		std::string mDialogTextTitle;				
		void setColorScheme(int nScheme);
		int mDialogWidth = 400;
		int mDialogHeight = 200;
		bool bUpdateWinPos = false;
		bool bDialogIsWatingForText = false;
		bool bInputIsAccept = false;
		bool bInputIsCancel = false;
		virtual void recieveInput(int mKey);		
		virtual void dropLastInputChar();
		virtual int render(int xpos, int ypos);
		virtual int render(int ypos);
		virtual int render();		
		virtual void init();
		virtual void update();
		virtual void cancel();		
};

struct DialogValueOptionInt{
	int mValue = 0;
};

class DTDialog : public Dialog{
	public:
		std::vector<DialogElement*> mElements;
		std::vector<DialogElement*> mBasicElements;
		std::vector<DialogValueBase*> mValues;
		std::vector<DialogButton*> mButtons;
		std::vector<DialogValueFile*> mFiles;
		std::vector<DialogValueOptionInt> mOptionValues;
		int mDefaultValue = 0;
		TIDialog* mActiveInput = NULL;
		int mCondition = -1;
		int mConditionBackup = -1;
		int mRequiredCondition = -1;
		int mTargetState = 0;		
		virtual void setLabel(std::string cLabel){mDialogTextTitle = cLabel;};
		virtual void setTarget(int cTarget){mTargetState = cTarget;};
		virtual void setCondition(int cCond){mCondition = cCond; mConditionBackup = mCondition; if(mFiles.size()){bDialogIsWatingForText = true;}};
		virtual void setRequiredCondition(int cCond){mRequiredCondition = cCond;};
		virtual void clearRequiredCondition(){mRequiredCondition = -1;};
		virtual void createValues(int cValNums){mOptionValues.resize(cValNums);};
		virtual void setValue(int cIndex, int cVal){if(mOptionValues.size()){if( (cIndex >= 0) && (cIndex < mOptionValues.size()) ){mOptionValues[cIndex].mValue = cVal;}}};
		virtual int* getValue(int cIndex){if(mOptionValues.size()){if( (cIndex >= 0) && (cIndex < mOptionValues.size()) ){return &mOptionValues[cIndex].mValue;}} return &mDefaultValue;};
		virtual void init();	
		virtual int render();
		virtual void recieveInput(int mKey);
		virtual void dropLastInputChar();				
		virtual void cancel();
		void addText(std::string cText, bool bSameline = false);
		void addSeperator();
		void addSameLine();
		void addConditionRestore();
		void addBool(std::string cLabel, bool cDefault, bool *cTarget, bool bSameline = false);
		void addBoolCondition(std::string cLabel, bool cDefault, bool *cTarget, int cTargCond, bool bSameline = false);
		void addInt(std::string cLabel, int cDefault, int *cTarget, int cMin, int cMax, bool bSameline = false);
		void addIntMinMax(std::string cLabel, int cDefault, int *cTarget, int *cMin, int *cMax, bool bSameline = false);
		void addIntStrings(std::string cLabel, int cDefault, int *cTarget, std::vector<std::string> &cStrings, bool bSameline = false);
		void addFloat(std::string cLabel, float cDefault, float *cTarget, float cMin, float cMax, std::string cFormat = "%.2f", bool bSameline = false);
		void addButton(std::string cLabel, int cAction, bool cSameline = false);
		void addRadioGroup(int cDefault, int* cTarget);
		void addRadioButton(std::string cLabel, int cDefault, bool cSameline = false);
		void addIntTarget(int cDefault, int *cTarget);
		void addFile(std::string cLabel, std::string cFileExt, std::string cFileKey, std::string cDefault, std::string* cTarget, bool cMustExist = true, bool cMustBeFile = true, bool cMustBeFolder = false, bool cMustNotBeFile = false, bool cMustNotExist = false, bool cMustBeProject = false, bool cSameline = false);

		static DTDialog* createSpriteFrameImportDialog();
		static DTDialog* createSpriteFramesImportDialog();
		static DTDialog* createSpriteCopyDialog();
		static DTDialog* createSpriteScaledCopyDialog();
		static DTDialog* createSpriteUpscaledCopyDialog();
		static DTDialog* createSpriteDownscaledCopyDialog();
		static DTDialog* createSpriteRotationRangeDialog();
		static DTDialog* createSpriteRotationsDialog();
		static DTDialog* createTileMapImportDialog();
};


class DialogElement{
	public:
		DTDialog *mParent;
		bool bSameLine = false;
		int mCondition = -1;
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} if(bSameLine){ImGui::SameLine();}}
};

class DialogElementText : public DialogElement{
	public:		
		DialogElementText(DTDialog *cParent, int cCond, std::string cText, bool cSameline){mParent = cParent; mText = cText; bSameLine = cSameline; mCondition = cCond;};
		std::string mText;
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} DialogElement::render(); ImGui::Text("%s", mText.c_str()); }
};

class DialogElementSeperator : public DialogElement{
	public:
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} ImGui::Separator();}
};

class DialogValueBase : public DialogElement{	
	public:
		virtual void render(){}
		virtual void apply(){}
		virtual void cancel(){}
};

template <typename T> class DialogValueType : public DialogValueBase{
	public:
		std::string mLabel;
		T mValue;
		T mDefault;
		T *mTarget;
		virtual void apply(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} *mTarget = mValue;}
		virtual void cancel(){mValue = mDefault;}
};

class DialogValueBool : public DialogValueType<bool>{	
	public:
		DialogValueBool(DTDialog *cParent, int cCond, std::string cLabel, bool cDefault, bool* cTarget, bool cSameline){mParent = cParent; mLabel = cLabel; mDefault = cDefault; mValue = mDefault; mTarget = cTarget; bSameLine = cSameline; mCondition = cCond;}
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} DialogElement::render(); ImGui::Checkbox(mLabel.c_str(), &mValue);}
};

class DialogValueBoolCondition : public DialogValueType<bool>{	
	public:
		int mTargetCondition = -1;
		DialogValueBoolCondition(DTDialog *cParent, int cCond, std::string cLabel, bool cDefault, bool* cTarget, int cTargetCond, bool cSameline){mParent = cParent; mLabel = cLabel; mDefault = cDefault; mValue = mDefault; mTarget = cTarget; bSameLine = cSameline; mCondition = cCond; mTargetCondition = cTargetCond;}
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} DialogElement::render(); ImGui::Checkbox(mLabel.c_str(), &mValue); if(mValue){ mParent->mCondition = mTargetCondition; } else { mParent->mCondition = mParent->mConditionBackup; } }		
		virtual void apply(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} *mTarget = mValue; if(mValue){ mParent->mCondition = mTargetCondition; } else { mParent->mCondition = mParent->mConditionBackup; } }
};

class DialogValueInt : public DialogValueType<int>{	
	public:
		int mMin = 0;
		int mMax = 0;
		DialogValueInt(DTDialog *cParent, int cCond, std::string cLabel, int cDefault, int* cTarget, int cMin, int cMax, bool cSameline){mParent = cParent; mLabel = cLabel; mDefault = cDefault; mValue = mDefault; mTarget = cTarget; mMin = cMin; mMax = cMax; bSameLine = cSameline; mCondition = cCond;}
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} DialogElement::render(); ImGui::SliderInt(mLabel.c_str(), &mValue, mMin, mMax);}		
};

class DialogValueIntMinMax : public DialogValueType<int>{	
	public:
		int *mMin = NULL;
		int *mMax = NULL;
		DialogValueIntMinMax(DTDialog *cParent, int cCond, std::string cLabel, int cDefault, int* cTarget, int *cMin, int *cMax, bool cSameline){mParent = cParent; mLabel = cLabel; mDefault = cDefault; mValue = mDefault; mTarget = cTarget; mMin = cMin; mMax = cMax; bSameLine = cSameline; mCondition = cCond;}
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} DialogElement::render(); ImGui::SliderInt(mLabel.c_str(), &mValue, *mMin, *mMax);}		
};


class DialogValueIntStrings : public DialogValueType<int>{
	public:
		int mMin = 0;
		int mMax = 0;
		std::vector<std::string> mStrings;
		DialogValueIntStrings(DTDialog *cParent, int cCond, std::string cLabel, int cDefault, int* cTarget, std::vector<std::string> &cStrings, bool cSameline){mParent = cParent; mLabel = cLabel; mDefault = cDefault; mValue = mDefault; mTarget = cTarget; mStrings = cStrings; mMin = 0; mMax = mStrings.size()-1; bSameLine = cSameline; mCondition = cCond;}
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} DialogElement::render(); const char *cFormat = mStrings[mValue].c_str(); ImGui::SliderInt(mLabel.c_str(), &mValue, mMin, mMax, cFormat);}		
};


class DialogValueFloat : public DialogValueType<float>{	
	public:
		float mMin = 0;
		float mMax = 0;
		std::string mFormat = "%.2f";
		DialogValueFloat(DTDialog *cParent, int cCond, std::string cLabel, float cDefault, float* cTarget, float cMin, float cMax, std::string cFormat, bool cSameline){mParent = cParent; mLabel = cLabel; mDefault = cDefault; mValue = mDefault; mTarget = cTarget; mMin = cMin; mMax = cMax; mFormat = cFormat, bSameLine = cSameline; mCondition = cCond;}
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} DialogElement::render(); ImGui::SliderFloat(mLabel.c_str(), &mValue, mMin, mMax, mFormat.c_str());}		
};

class DialogButton : public DialogElement{
	public:
		DialogButton(DTDialog *cParent, int cCond, std::string cLabel, int cAction, bool cSameline){mParent = cParent; mLabel = cLabel; mAction = cAction; bSameLine = cSameline; mCondition = cCond;};
		std::string mLabel;
		int mAction;
		virtual void render();	
};

class DialogValueRadioButton : public DialogValueType<int>{
	public:
		DialogValueRadioButton(std::string cLabel, int cDefault, int* cTarget, bool cSameline){mLabel = cLabel; mDefault = cDefault; mValue = mDefault; mTarget = cTarget; bSameLine = cSameline;};
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} DialogElement::render(); ImGui::RadioButton(mLabel.c_str(), mTarget, mDefault);}
};

class DialogValueRadioGroup : public DialogValueType<int>{
	public:
		DialogValueRadioGroup(DTDialog *cParent, int cCond, int cDefault, int* cTarget){mParent = cParent; mDefault = cDefault; mValue = mDefault; mTarget = cTarget; mCondition = cCond;};
		std::vector<DialogValueRadioButton*> mButtons;
		virtual void render();
};

class DialogValueIntTarget : public DialogValueType<int>{	
	public:		
		DialogValueIntTarget(DTDialog *cParent, int cCond, int cDefault, int* cTarget){mParent = cParent; mDefault = cDefault; mValue = mDefault; mTarget = cTarget; mCondition = cCond;}
		virtual void render(){};
};

class DialogConditionRestore : public DialogValueBase{
	public:
		DialogConditionRestore(DTDialog *cParent){mParent = cParent;};		
		virtual void render(){mParent->mCondition = mParent->mConditionBackup;};
		virtual void apply(){mParent->mCondition = mParent->mConditionBackup;}		
};

class TIDialog: public Dialog{
	public:
		int mCursorTime = 0;
		bool bShowCursor=false;
		bool bIsInputActive=false;
		bool bInputIsAccepted=true;
		bool bIsNumeric=false;
		bool bAutoComplete=false;
		int mCompleteIndex=0;				
		static int MyCallback(ImGuiInputTextCallbackData* data);
		std::string mCompleteText;		
		std::vector<int> mAllowedValues;
		int mMaxLength=0;
		int mMaxRange=0;
		int mMinRange=0;
		bool bMustExist=false;
		bool bMustBeFile=false;
		bool bMustBeFolder=false;
		bool bMustNotBeFile=false;
		bool bMustNotExist=false;
		bool bMustBeProject=false;
		int checkCurrentText();
		void autoComplete();
		virtual void dropLastInputChar();
		virtual void init();		
		virtual int render();		
		std::string mInputLabel = "Filename";
		bool bIsActive=false;
};

class DialogValueFile : public DialogValueType<std::string>{	
	public:
		TIDialog mTextInput;
		std::string mLabel;
		std::string mFileExt;
		std::string mFileKey;

		bool bIsValid = false;
		
		DialogValueFile(DTDialog *cParent, int cCond, std::string cLabel, std::string cFileExt, std::string cFileKey, std::string cDefault, std::string* cTarget, bool cMustExist, bool cMustBeFile, bool cMustBeFolder, bool cMustNotBeFile, bool cMustNotExist, bool cMustBeProject){mParent = cParent; mDefault = cDefault; mValue = mDefault; mTarget = cTarget; mCondition = cCond; mTextInput.bMustExist = cMustExist; mTextInput.bMustBeFile = cMustBeFile; mTextInput.bMustBeFolder = cMustBeFolder; mTextInput.bMustNotBeFile = cMustNotBeFile; mTextInput.bMustNotExist = cMustNotExist; mTextInput.bMustBeProject = cMustBeProject; mLabel = cLabel; mFileKey = "ChooseFileDlgKey" + cFileKey; mFileExt = cFileExt; mTextInput.bIsInputActive = true; mTextInput.bAutoComplete = true;}		
		virtual void render();
		virtual void cancel(){mValue = mDefault; mTextInput.mDialogTextMain = mDefault; bIsValid = false;}
};


class SDialog: public Dialog{
	public:
		virtual void init();
		std::string mDialogButtonAccept;
		std::string mDialogButtonCancel;
		virtual void recieveInput(int mKey);				
		virtual int render();
};

class RTDialog: public SDialog{
	public:
		virtual void init();
		virtual void recieveInput(int mKey);
};


class RTSDialog: public RTDialog{
	public:
		virtual void init();
		virtual int render();
		virtual void recieveInput(int mKey);
};

class RSFDialog: public RTDialog{
	public:
		virtual void init();
		virtual int render();
		virtual void recieveInput(int mKey);
};


class RTMDialog: public RTDialog{
	public:
		virtual void init();
		virtual void recieveInput(int mKey);
};

class RSDialog: public RTDialog{
	public:
		virtual void init();
		virtual void recieveInput(int mKey);
};


class RCMDialog: public RTDialog{
	public:
		virtual void init();
		virtual void recieveInput(int mKey);
};

class PUDialog: public SDialog{
	public:
		virtual void init();
		virtual void recieveInput(int mKey);
};

class SADialog: public SDialog{
	public:
		virtual void init();
		virtual void cancel();		
		virtual void recieveInput(int mKey);		
		virtual void dropLastInputChar();
		virtual int render();
		bool bSubDialogActive = false;	
		SDialog *mSubDialog;
		TIDialog mTextInput;		
};

class ITDialog: public SADialog{
	public:
		virtual void init();
		virtual void cancel();
		virtual int render();
		virtual void recieveInput(int mKey);		
};

/*
class ISFDialog: public SADialog{
	public:
		virtual void init();
		virtual void cancel();
		virtual int render();
		virtual void recieveInput(int mKey);		
};

class ISFSDialog: public ISFDialog{
	public:		
		virtual int render();
		virtual void recieveInput(int mKey);		
};
*/

class RTSFDialog: public Dialog{
	public:
		virtual void init();
		virtual void cancel();
		virtual int render();
		virtual void recieveInput(int mKey);
		float frotateangle = 0;	
};

class SSFDialog: public RTSFDialog{
	public:
		virtual void init();
		virtual void cancel();
		virtual int render();
		virtual void recieveInput(int mKey);
		float fscalefactor = 0;	
};



class ISDialog: public SADialog{
	public:				
		virtual void init();				
		virtual void cancel();
		virtual void recieveInput(int mKey);				
		virtual int render();
		int spritex=32;
		int spritey=32;
		int spritebpp=8;
		int simporttype = 0;			
		bool bCreateNewSprite = false;
};


class ITSDialog: public ITDialog{
	public:				
		virtual int render();
		virtual void recieveInput(int mKey);		
};

class ITMDialog: public ITDialog{
	public:	
		bool bUseOffset = false;
		int mTileOffset = 1;
		int mPaletteOffset = 0;
		virtual void cancel();
		virtual int render();
		virtual void recieveInput(int mKey);		
};

/*
class RNDialog: public ITDialog{
	public:
		int mNumber = 0;
		bool show_another_window = false;
		bool show_demo_window = false;
		float f = 0;
    	int counter = 0;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		virtual void init();
		virtual void cancel();
		virtual void dropLastInputChar();
		virtual int render(int xpos, int ypos);
		virtual void recieveInput(int mKey);
		bool isShown = true;
		float wx,wy;
		ImVec2 wpos;
		ImVec2 wsize;
		ImVec2 elmin;
		ImVec2 elmax;
		ImVec2 esize;		
};
*/

class OPDialog: public ITDialog{
	public:
		virtual void init();
		virtual int render();
		virtual void recieveInput(int mKey);		

};

class CPDialog: public Dialog{
	public:		
		TIDialog mReadPath;
		TIDialog mReadPal;		
		TIDialog mReadTileSet;		
		TIDialog *mActiveInput;
		bool bInputIsAccepted = false;
		virtual void init();		
		virtual void dropLastInputChar();		
		virtual void recieveInput(int mKey);				
		virtual int render();
		int tmapx=32;
		int tmapy=32;
		int tilex=16;
		int tiley=16;
		int tbpp=8;
		int tilesettype = 0;
		int tilesetsize = 0;
		bool bHasPalette = false;
		bool bHasTileSet = false;
};


class CTMDialog: public Dialog{
	public:				
		virtual void init();				
		virtual void cancel();
		virtual void recieveInput(int mKey);				
		virtual int render();
		int tmapx=32;
		int tmapy=32;
		int toffset=1;		
		int mPaletteOffset = 0;
};

class CSDialog: public Dialog{
	public:				
		virtual void init();				
		virtual void cancel();
		virtual void recieveInput(int mKey);				
		virtual int render();
		int spritex=32;
		int spritey=32;
		int spritebpp=8;				
};

class OCDialog: public Dialog{
	public:
		virtual void init();		
		bool bSubDialogActive = false;
		bool bSubDialogIsOpen = false;	
		bool bSubDialogIsCreate = false;			
		OPDialog mOpenProject;
		CPDialog mCreateProject;
		virtual void dropLastInputChar();		
		virtual void recieveInput(int mKey);				
		virtual int render();
};

class HDialog: public SDialog{
	public:
		std::string	mHelpTitle;
		std::vector<std::string> mHelpTextMap;
		std::vector<std::string> mHelpTextTile;
		std::vector<std::string> mHelpTextTileSet;
		std::vector<std::string> mHelpTextPalette;
		std::vector<std::string> mHelpTextImport;
		std::vector<std::string> mHelpTextGeneral;
		std::vector<std::string> mHelpTextSelection;
		std::vector<std::string> mHelpTextColMapEditor;
		std::vector<std::string> mHelpTextSprite;
		virtual void init();
		virtual void recieveInput(int mKey);		
		virtual int render();
};

class PODialog: public Dialog{
	public:
		virtual void init();
		virtual int render(int xpos, int ypos, TextureParameters *mTexParam);
};

class PIDialog: public Dialog{
	public:
		TEditor* mEditor;
		virtual void init();
		virtual void update();
		int fliph,flipv,tilenum,curtile;
		std::string TileMap;
		std::string cMapWidth;
		std::string cMapHeight;
		std::string TileSizeX;
		std::string TileSizeY;
		std::string Flip;		
		std::string CurrentTile;
		std::string TilesInSet;
		std::string SelectedTile;
		std::string cTileSizeX;
		std::string cTileSizeY;
		virtual int render(int xpos, int ypos);
};

class TBDialog: public Dialog{
	public:
		std::string mDialogTextActions;
		std::string mDialogTextWindow;		
		std::string mDialogTextProject;		
		TEditor* mEditor;
		virtual void init();
		virtual int render();
		bool bIsMenuOpen = false;
};


class MEDialog: public HDialog{
	public:
		virtual void init();
		virtual void setColorScheme(int nScheme);
		virtual void update();		
		virtual int render();
};

class QDialog: public Dialog{
	public:
		virtual int render();
		virtual void recieveInput(int mKey);
};

class CCPDialog: public QDialog{
	public:
		virtual int render();
		virtual void recieveInput(int mKey);
};


#endif