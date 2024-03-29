#ifndef __TDIALOGS__
#define __TDIALOGS__

#include "TSettings.h"

extern TSettings mGlobalSettings;

class DialogElement;
class DialogValueBase;
class DialogButton;
class DialogValueFile;
class TIDialog;

class DTDialog;
class DTDCialog;


class IDDialog;
class DialogDisplayBase;

class Dialog{
	public:
		SDL_Color mTextColor = mGlobalSettings.DefaultTextColor;
		std::string mDialogTextMain;		
		std::string mDialogTextTitle;				
		void setColorScheme(int nScheme);
		int mDialogWidth = 400;
		int mDialogHeight = 200;
		bool bUpdateWinPos = false;		
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
		std::vector<DialogValueOptionInt> mOptionValuesLocal;
		std::vector<int> mConditionStack;
		int mDefaultValue = 0;
		TIDialog* mActiveInput = NULL;
		int mCondition = -1;
		int mConditionBackup = -1;		
		int mRequiredCondition = -1;
		int mConditionLast = -1;		
		int mTargetState = 0;
		std::vector<std::tuple<int, int>> mAltTargets;
		bool bHasColor = false;
		bool *bCloseBool = NULL;
		virtual void setLabel(std::string cLabel){mDialogTextTitle = cLabel;};
		virtual void setTarget(int cTarget){mTargetState = cTarget;};
		virtual void addAltTarget(int cAction, int cTarget){mAltTargets.push_back(std::tuple<int, int>(cAction, cTarget));};
		virtual void setCondition(int cCond){mCondition = cCond; mConditionBackup = mCondition; mConditionLast = mCondition;}; //mConditionLast = cCond;
		virtual void setCurrentCondition(int cCond){mCondition = cCond; mConditionLast = cCond;};
		virtual void restoreContition(){mCondition = mConditionBackup; mConditionLast = mConditionBackup;};
		virtual void restoreLastContition(){mCondition = mConditionLast;};
		virtual void setRequiredCondition(int cCond){mRequiredCondition = cCond;};
		virtual void clearRequiredCondition(){mRequiredCondition = -1;};
		virtual void createValues(int cValNums){mOptionValues.resize(cValNums);};
		virtual void createLocalValues(int cValNums){mOptionValuesLocal.resize(cValNums);};
		virtual void setValue(int cIndex, int cVal){if(mOptionValues.size()){if( (cIndex >= 0) && (cIndex < mOptionValues.size()) ){mOptionValues[cIndex].mValue = cVal;}}};
		virtual int* getValue(int cIndex){if(mOptionValues.size()){if( (cIndex >= 0) && (cIndex < mOptionValues.size()) ){return &mOptionValues[cIndex].mValue;}} return &mDefaultValue;};
		virtual void setLocalValue(int cIndex, int cVal){if(mOptionValuesLocal.size()){if( (cIndex >= 0) && (cIndex < mOptionValuesLocal.size()) ){mOptionValuesLocal[cIndex].mValue = cVal;}}};
		virtual int* getLocalValue(int cIndex){if(mOptionValuesLocal.size()){if( (cIndex >= 0) && (cIndex < mOptionValuesLocal.size()) ){return &mOptionValuesLocal[cIndex].mValue;}} return &mDefaultValue;};
		virtual void init();	
		virtual int render();
		virtual void update();
		virtual void setdefaults();
		virtual void recieveInput(int mKey);
		virtual void dropLastInputChar();				
		virtual void cancel();
		void addText(std::string cText, bool bSameline = false);
		void addSeperator();
		void addSameLine();
		void addConditionRestore();
		void addConditionPush();
		void addConditionPop();
		void addConditionSetIf(int cTarget, int cState);
		void addConditionSetIfInt(int *cTarget, int cValue, int cCond);
		void addConditionSetIfBool(bool *cTarget, bool cValue, int cCond);
		void addBool(std::string cLabel, bool cDefault, bool *cTarget, bool bSameline = false);
		void addBoolCondition(std::string cLabel, bool cDefault, bool *cTarget, int cTargCond, bool bSameline = false);
		void addSetLocalValue(int cTarget, int cTargetValue);
		void addInt(std::string cLabel, int cDefault, int *cTarget, int cMin, int cMax, bool bSameline = false);
		void addIntMinMax(std::string cLabel, int cDefault, int *cTarget, int *cMin, int *cMax, bool bSameline = false);
		void addIntStrings(std::string cLabel, int cDefault, int *cTarget, std::vector<std::string> &cStrings, bool bSameline = false);
		void addFloat(std::string cLabel, float cDefault, float *cTarget, float cMin, float cMax, std::string cFormat = "%.2f", bool bSameline = false);
		void addButton(std::string cLabel, int cAction, bool cSameline = false);
		void addRadioGroup(int cDefault, int* cTarget);
		void addRadioButton(std::string cLabel, int cDefault, bool cSameline = false);
		void addRadioGroupCondition(int cDefault, int* cTarget);
		void addIntTarget(int cDefault, int *cTarget);
		void addColor(std::string cLabel, ImU32 *cDefaultColor, ImU32 *cTarget, bool bSameline = false);
		void addFile(std::string cLabel, std::string cInputLabel, std::string cFileExt, std::string cFileKey, std::string cDefault, std::string* cTarget, bool cMustExist = true, bool cMustBeFile = true, bool cMustBeFolder = false, bool cMustNotBeFile = false, bool cMustNotExist = false, bool cMustBeProject = false, bool cMustNotBeFolder = false ,bool cSameline = false);
		void addFileDefault(std::string cLabel, std::string cInputLabel, std::string cFileExt, std::string cFileKey, std::string *cDefaultPath, std::string* cTarget, bool cMustExist = true, bool cMustBeFile = true, bool cMustBeFolder = false, bool cMustNotBeFile = false, bool cMustNotExist = false, bool cMustBeProject = false, bool cSameline = false);

		void addSetFileExt(std::string cKey, std::string cExt);

		void addIntActiveMinus(std::string cLabel, int cBase, int *cTarget);

		int* getIntValue(std::string cIntLabel);	
		int* getIntActiveValue(std::string cIntLabel);	

		int* getIntValueRadio(int cIntIndex);
		bool* getBoolValue(std::string cBoolLabel);

		std::string* getFilePath(std::string cFileVal);	
		std::string* getFileExt(std::string cFileVal);	

		static DTDialog* createSpriteFrameImportDialog();
		static DTDialog* createSpriteFramesImportDialog();
		static DTDialog* createSpriteCopyDialog();
		static DTDialog* createSpriteScaledCopyDialog();
		static DTDialog* createSpriteUpscaledCopyDialog();
		static DTDialog* createSpriteDownscaledCopyDialog();
		static DTDialog* createSpriteRotationRangeDialog();
		static DTDialog* createSpriteRotationsDialog();
		static DTDialog* createTileMapImportDialog();
		static DTDialog* createSpriteConvertBPPDialog();
		static DTDialog* createThemeColorDialog();
		static DTDialog* createProjectOpenDialog();
		static DTDialog* createProjectCreateDialog();
		static DTDialog* createProjectCloseDialog();
		static DTDialog* createProjectCloseDialogSave();
		static DTDialog* createProgramQuitDialog();
		static DTDialog* createProgramQuitDialogSave();
		static DTDialog* createProjectSaveDialog();
		static DTDialog* createTileSetImportDialog();
		static DTDialog* createTileMapRemoveDialog();
		static DTDialog* createColMapRemoveDialog();
		static DTDialog* createTilesUnusedRemoveDialog();
		static DTDialog* createFrameRemoveDialog();
		static DTDialog* createSpriteRemoveDialog();
		static DTDialog* createTileRemoveDialog();
		static DTDialog* createTileImportDialog();
		static DTDialog* createFrameRotateDialog();
		static DTDialog* createFrameScaleDialog();
		static DTDialog* createTileMapCreateDialog();
		static DTDialog* createSpriteCreateDialog();
		static DTDialog* createSpriteImportDialog();
		static DTDialog* createPaletteUpdateDialog();
		static DTDialog* createPaletteImportDialog();
		static DTDialog* createTileSetOffsetDialog();
		static DTDialog* createTileOffsetDialog();
		static DTDialog* createSpriteOffsetDialog();
		static DTDialog* createFrameOffsetDialog();

};

class DTDCDialog : public DTDialog{
	public:
		DTDialog mConfirmDialog;
		bool bConfirmIsActive = false;
		std::string *mConditionPath = NULL;
		bool bConditionState = false;

		bool bFirstConfirm = false;
		bool bStillConfirm = false;
		
		virtual int render();		
		virtual void recieveInput(int mKey);
		virtual void cancel();

		void setConfirmLabel(std::string cLabel){mConfirmDialog.mDialogTextTitle = cLabel; mConfirmDialog.mTargetState = -1;};
		void addConfirmText(std::string cText, bool bSameline = false);
		void setConfirmButtons(std::string cConfirm, std::string cCancel);
		void setConfirmConditionExists(std::string *cPath, bool cState);

		static DTDCDialog* createProjectSaveAsDialog();
		static DTDCDialog* createPaletteExportDialog();
};

class IDDialog : public DTDialog{
	public:
		std::vector<DialogDisplayBase*> mDisplays;
		//bool *bCloseBool = NULL;

		bool bSmallFonts = false;
		
		virtual int render(int xpos, int ypos);
		virtual void update();

		void setCloseBool(bool *cClose);
		void setFontsSmall(bool cSmall);

		void addDisplayInt(std::string cLabel, int* cTarget, bool cSameline = false);
		void addDisplayIntDual(std::string cLabel,std::string cCenter,std::string cEnd, int* cTarget1, int* cTarget2, bool cSameline = false);
		
		void addDisplayTileCount(std::string cLabel, std::vector<Tile*> *cTarget, bool cSameline = false);
		void addDisplayTileMapCount(std::string cLabel, std::vector<TileMap*> *cTarget, bool cSameline = false);
		void addDisplaySpriteCount(std::string cLabel, std::vector<TSprite*> *cTarget, bool cSameline = false);

		void addDisplayTileMapSize(std::string cLabel, TileMap **cTarget, bool cSameline = false);
		void addDisplaySpriteSize(std::string cLabel,  TSprite **cTarget, bool cSameline = false);

		void addDisplayString(std::string cLabel, std::string* cTarget, bool cSameline = false);

		void addDisplayColor(SDL_Color *cTarget);
		void addDisplayColorRestore();

		static IDDialog* createProjectInfoDialog();
		static IDDialog* createMessageDialog();
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
		virtual void update(){}
		virtual void setdefaults(){}
};

class DialogValueSetFileExt : public DialogValueBase{	
	public:
		std::string mFileExt;
		std::string mFileKey;
		DialogValueSetFileExt(DTDialog *cParent, int cCond, std::string cKey, std::string cExt){mParent = cParent; mFileKey = cKey; mFileExt = cExt; bSameLine = false; mCondition = cCond;}
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} std::string *cNewExt = mParent->getFileExt(mFileKey); if(cNewExt){*cNewExt = mFileExt;} }
};


template <typename T> class DialogValueType : public DialogValueBase{
	public:
		std::string mLabel;
		T mValue;
		T mDefault;
		T *mTarget;
		virtual void apply(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} *mTarget = mValue;}
		virtual void cancel(){mValue = mDefault;}
		virtual void setdefaults(){if(mTarget){*mTarget = mDefault;}}
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
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} DialogElement::render(); if(ImGui::Checkbox(mLabel.c_str(), &mValue)){mParent->bUpdateWinPos = true;}; if(mValue){ mParent->setCurrentCondition(mTargetCondition); } else { mParent->restoreLastContition(); } }		
		virtual void apply(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} if(mTarget){*mTarget = mValue;} if(mValue){ mParent->setCurrentCondition(mTargetCondition); } else { mParent->restoreLastContition(); } }
};

class DialogValueInt : public DialogValueType<int>{	
	public:
		int mMin = 0;
		int mMax = 0;
		DialogValueInt(DTDialog *cParent, int cCond, std::string cLabel, int cDefault, int* cTarget, int cMin, int cMax, bool cSameline){mParent = cParent; mLabel = cLabel; mDefault = cDefault; mValue = mDefault; mTarget = cTarget; mMin = cMin; mMax = cMax; bSameLine = cSameline; mCondition = cCond;}
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} DialogElement::render(); ImGui::SliderInt(mLabel.c_str(), &mValue, mMin, mMax);}		
};

class DialogValueIntActive : public DialogValueType<int>{
	public:		
		int mBaseValue;
		int mType = 0;
		DialogValueIntActive(DTDialog *cParent, int cCond, std::string cLabel, int cBase, int* cTarget, int cType){mParent = cParent; mBaseValue = cBase; mLabel = cLabel; mTarget = cTarget; mType = cType; bSameLine = false; mCondition = cCond;}
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} if(mType == 0){ mValue = mBaseValue - *mTarget;} if(mType == 1){ mValue = mBaseValue + *mTarget;} }
		virtual void apply(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} if(mType == 0){ mValue = mBaseValue - *mTarget;} if(mType == 1){ mValue = mBaseValue + *mTarget;} }
		virtual void cancel(){};
		virtual void update(){};
		virtual void setdefaults(){}
};

class DialogValueColor : public DialogValueType<ImU32>{	
	public:
		ImVec4 mColor = {0,0,0,1};
		ImU32 *mDefaultColor;
		DialogValueColor(DTDialog *cParent, int cCond, std::string cLabel, ImU32 *cDefaultColor,ImU32 *cTarget, bool cSameline){mParent = cParent; mLabel = cLabel; mDefaultColor = cDefaultColor; mTarget = cTarget; bSameLine = cSameline; mCondition = cCond;}
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} ImGui::ColorPicker3(mLabel.c_str(), (float*)&mColor, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoInputs);}		
		virtual void apply(){*mTarget = 0xFF; *mTarget = *mTarget << 8; *mTarget += (int)(mColor.z * 255.0); *mTarget = *mTarget << 8;*mTarget += (int)(mColor.y * 255.0); *mTarget = *mTarget << 8;*mTarget += (int)(mColor.x * 255.0);}
		virtual void cancel(){mColor = {0,0,0,1};}
		virtual void update(){mColor.x = ((*mDefaultColor & 0x000000ff) / 255.0); mColor.y = (((*mDefaultColor & 0x0000ff00) >> 8)/ 255.0); mColor.z = (((*mDefaultColor & 0x00ff0000) >> 16)/ 255.0);}
		virtual void setdefaults(){}
};


class DialogValueIntMinMax : public DialogValueType<int>{	
	public:
		int *mMin = NULL;
		int *mMax = NULL;
		DialogValueIntMinMax(DTDialog *cParent, int cCond, std::string cLabel, int cDefault, int* cTarget, int *cMin, int *cMax, bool cSameline){mParent = cParent; mLabel = cLabel; mDefault = cDefault; mValue = mDefault; mTarget = cTarget; mMin = cMin; mMax = cMax; bSameLine = cSameline; mCondition = cCond;}
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} DialogElement::render(); if(mValue > *mMax){mValue = *mMax;} if(mValue < *mMin){mValue = *mMin;} ImGui::SliderInt(mLabel.c_str(), &mValue, *mMin, *mMax);}		
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
		virtual void render(){DialogElement::render(); ImGui::RadioButton(mLabel.c_str(), mTarget, mDefault);}
};

class DialogValueRadioGroup : public DialogValueType<int>{
	public:
		DialogValueRadioGroup(){};
		DialogValueRadioGroup(DTDialog *cParent, int cCond, int cDefault, int* cTarget){mParent = cParent; mDefault = cDefault; mValue = mDefault; mTarget = cTarget; mCondition = cCond;};
		std::vector<DialogValueRadioButton*> mButtons;
		virtual void render();
};

class DialogValueRadioGroupCondition : public DialogValueRadioGroup{
	public:		
		DialogValueRadioGroupCondition(DTDialog *cParent, int cCond, int cDefault, int* cTarget){mParent = cParent; mDefault = cDefault; mValue = mDefault; mTarget = cTarget; mCondition = cCond;};
		virtual void render();
		virtual void apply(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} if(mTarget){*mTarget = mValue;} mParent->setCurrentCondition(mValue);}
};

class DialogValueIntTarget : public DialogValueType<int>{	
	public:		
		DialogValueIntTarget(DTDialog *cParent, int cCond, int cDefault, int* cTarget){mParent = cParent; mDefault = cDefault; mValue = mDefault; mTarget = cTarget; mCondition = cCond;}
		virtual void render(){};		
};

class DialogConditionRestore : public DialogValueBase{
	public:
		DialogConditionRestore(DTDialog *cParent){mParent = cParent;};		
		virtual void render(){mParent->restoreContition();};
		virtual void apply(){mParent->restoreContition();};		
};

class DialogConditionPush : public DialogValueBase{
	public:
		DialogConditionPush(DTDialog *cParent){mParent = cParent;};		
		virtual void render(){mParent->mConditionStack.push_back(mParent->mCondition);};
		virtual void apply(){mParent->mConditionStack.push_back(mParent->mCondition);};		
};

class DialogConditionPop : public DialogValueBase{
	public:
		DialogConditionPop(DTDialog *cParent){mParent = cParent;};		
		virtual void render(){if(mParent->mConditionStack.size()){mParent->mCondition = mParent->mConditionStack[mParent->mConditionStack.size()-1]; mParent->mConditionStack.pop_back();}};
		virtual void apply(){if(mParent->mConditionStack.size()){mParent->mCondition = mParent->mConditionStack[mParent->mConditionStack.size()-1]; mParent->mConditionStack.pop_back();}};
};


class DialogConditionSetIf : public DialogValueBase{
	public:
		int mTargetCondifion;
		int mCondifionState;
		DialogConditionSetIf(DTDialog *cParent, int cTarget, int cState){mParent = cParent; mTargetCondifion = cTarget; mCondifionState = cState;};		
		virtual void render(){if(mParent->mCondition == mCondifionState){mParent->setCurrentCondition(mTargetCondifion);}};
		virtual void apply(){if(mParent->mCondition == mCondifionState){mParent->setCurrentCondition(mTargetCondifion);}};		
};

class DialogConditionSetIfInt : public DialogValueBase{
	public:
		int *mTarget;
		int mTargetValue;	
		int mTargetCondifion;	
		DialogConditionSetIfInt(DTDialog *cParent, int cCond, int *cTarget, int cValue, int cTargetCond){mParent = cParent; mTarget = cTarget; mTargetValue = cValue; mTargetCondifion = cTargetCond; mCondition = cCond;};		
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} if(*mTarget == mTargetValue){mParent->setCurrentCondition(mTargetCondifion);}};
		virtual void apply(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} if(*mTarget == mTargetValue){mParent->setCurrentCondition(mTargetCondifion);}};			
};

class DialogSetLocalValue : public DialogValueBase{
	public:
		int mTarget;
		int mTargetValue;			
		DialogSetLocalValue(DTDialog *cParent, int cCond, int cTarget, int cValue){mParent = cParent; mTarget = cTarget; mTargetValue = cValue; mCondition = cCond;};		
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} mParent->setLocalValue(mTarget, mTargetValue);};
		virtual void apply(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} mParent->setLocalValue(mTarget, mTargetValue);};			
};


class DialogConditionSetIfBool : public DialogValueBase{
	public:
		bool *mTarget;
		bool mTargetValue;	
		int mTargetCondifion;	
		DialogConditionSetIfBool(DTDialog *cParent, int cCond, bool *cTarget, bool cValue, int cTargetCond){mParent = cParent; mTarget = cTarget; mTargetValue = cValue; mTargetCondifion = cTargetCond; mCondition = cCond;};		
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} if(*mTarget == mTargetValue){mParent->setCurrentCondition(mTargetCondifion);}};
		virtual void apply(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} if(*mTarget == mTargetValue){mParent->setCurrentCondition(mTargetCondifion);}};			
};


class DialogDisplayBase : public DialogElement{
	public:
		std::string mLabel;
		std::string mOutput;
		std::stringstream mConv;
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} DialogElement::render(); ImGui::Text("%s", mOutput.c_str());};
		virtual void update(){};
};

template <typename T> class DialogDisplayType : public DialogDisplayBase{
	public:
		T* mTarget;
		virtual void update(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} std::string tStr; mConv << *mTarget << std::endl; mConv >> tStr;  mOutput = mLabel + tStr;};
};

template <typename T> class DialogDisplayTypeDual : public DialogDisplayBase{
	public:
		T* mTarget1;
		T* mTarget2;
		std::string mCenter;
		std::string mEnd;
		virtual void update(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} std::string tStr1, tStr2; mConv << *mTarget1 << std::endl; mConv >> tStr1;mConv << *mTarget2 << std::endl; mConv >> tStr2;  mOutput = mLabel + tStr1 + mCenter + tStr2 + mEnd;};
};


template <class T> class DialogDisplayClassGetSize : public DialogDisplayBase{
	public:
		T** mTarget;		
		virtual void update(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} mOutput = mLabel + (*mTarget)->getSize();};
};

template <class T> class DialogDisplayVectorSize : public DialogDisplayBase{
	public:
		std::vector<T*> *mTarget;		
		virtual void update(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} std::string tStr; mConv << mTarget->size() << std::endl; mConv >> tStr;  mOutput = mLabel + tStr;};
};


class DialogDisplayInt : public DialogDisplayType<int>{
	public:
		DialogDisplayInt(DTDialog *cParent, int cCond, std::string cLabel, int* cTarget, bool cSameline){mParent = cParent; mLabel = cLabel; mTarget = cTarget; mCondition = cCond; bSameLine = cSameline;};
};

class DialogDisplayString : public DialogDisplayType<std::string>{
	public:
		DialogDisplayString(DTDialog *cParent, int cCond, std::string cLabel, std::string *cTarget, bool cSameline){mParent = cParent; mLabel = cLabel; mTarget = cTarget; mCondition = cCond; bSameLine = cSameline;};		
		virtual void update(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} mOutput = mLabel + " "; mOutput += *mTarget;}
};

class DialogDisplayColor : public DialogDisplayBase{
	public:
		SDL_Color *mTarget;
		virtual void update(){}
		DialogDisplayColor(DTDialog *cParent, int cCond, std::string cLabel, SDL_Color *cTarget, bool cSameline){mParent = cParent; mLabel = cLabel; mTarget = cTarget; mCondition = cCond; bSameLine = cSameline;};
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} SDL_Color lCol; lCol = *mTarget; ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(lCol.r, lCol.g, lCol.b, 255)); mParent->bHasColor = true;}		
		virtual void apply(){}
		virtual void cancel(){}		
};

class DialogDisplayColorRestore : public DialogDisplayBase{
	public:
		DialogDisplayColorRestore(DTDialog *cParent, int cCond, std::string cLabel, bool cSameline){mParent = cParent; mLabel = cLabel; mCondition = cCond; bSameLine = cSameline;};
		virtual void render(){if(mCondition > -1){if(mParent->mCondition != mCondition){return;}} if(mParent->bHasColor){ImGui::PopStyleColor(); mParent->bHasColor = false;}}
		virtual void update(){}
		virtual void apply(){}
		virtual void cancel(){}		

};

class DialogDisplayIntDual : public DialogDisplayTypeDual<int>{
	public:
		DialogDisplayIntDual(DTDialog *cParent, int cCond, std::string cLabel,std::string cCenter,std::string cEnd, int* cTarget1, int* cTarget2, bool cSameline){mParent = cParent; mLabel = cLabel; mCenter = cCenter; mEnd = cEnd; mTarget1 = cTarget1; mTarget2 = cTarget2; mCondition = cCond; bSameLine = cSameline;};
};

class DialogDisplaySpriteGetSize : public DialogDisplayClassGetSize<TSprite>{
	public:
		DialogDisplaySpriteGetSize(DTDialog *cParent, int cCond, std::string cLabel, TSprite** cTarget, bool cSameline){mParent = cParent; mLabel = cLabel; mTarget = cTarget; mCondition = cCond; bSameLine = cSameline;};		
};

class DialogDisplayTileMapGetSize : public DialogDisplayClassGetSize<TileMap>{
	public:
		DialogDisplayTileMapGetSize(DTDialog *cParent, int cCond, std::string cLabel, TileMap** cTarget, bool cSameline){mParent = cParent; mLabel = cLabel; mTarget = cTarget; mCondition = cCond; bSameLine = cSameline;};		
};

class DialogDisplayTileMapCount : public DialogDisplayVectorSize<TileMap>{
	public:
		DialogDisplayTileMapCount(DTDialog *cParent, int cCond, std::string cLabel, std::vector<TileMap*> *cTarget, bool cSameline){mParent = cParent; mLabel = cLabel; mTarget = cTarget; mCondition = cCond; bSameLine = cSameline;};		
};

class DialogDisplayTileCount : public DialogDisplayVectorSize<Tile>{
	public:
		DialogDisplayTileCount(DTDialog *cParent, int cCond, std::string cLabel, std::vector<Tile*> *cTarget, bool cSameline){mParent = cParent; mLabel = cLabel; mTarget = cTarget; mCondition = cCond; bSameLine = cSameline;};		
};

class DialogDisplaySpriteCount : public DialogDisplayVectorSize<TSprite>{
	public:
		DialogDisplaySpriteCount(DTDialog *cParent, int cCond, std::string cLabel, std::vector<TSprite*> *cTarget, bool cSameline){mParent = cParent; mLabel = cLabel; mTarget = cTarget; mCondition = cCond; bSameLine = cSameline;};
};

class TIDialog: public Dialog{
	public:		
		bool bIsInputActive=false;
		bool bInputIsAccepted=true;
		bool bIsNumeric=false;
		bool bAutoComplete=false;
		int mCompleteIndex=0;				
		static int MyCallback(ImGuiInputTextCallbackData* data);
		std::string mCompleteText;		
		std::vector<int> mAllowedValues;		
		int mMaxRange=0;
		int mMinRange=0;
		bool bMustExist=false;
		bool bMustBeFile=false;
		bool bMustBeFolder=false;
		bool bMustNotBeFile=false;
		bool bMustNotBeFolder=false;
		bool bMustNotExist=false;
		bool bMustBeProject=false;
		int checkCurrentText();
		void autoComplete();
		virtual void dropLastInputChar();		
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
		
		DialogValueFile(){};
		DialogValueFile(DTDialog *cParent, int cCond, std::string cLabel, std::string cInputLabel, std::string cFileExt, std::string cFileKey, std::string cDefault, std::string* cTarget, bool cMustExist, bool cMustBeFile, bool cMustBeFolder, bool cMustNotBeFile, bool cMustNotExist, bool cMustBeProject){mParent = cParent; mDefault = cDefault; mValue = mDefault; mTextInput.mInputLabel = cInputLabel; mTextInput.mDialogTextMain = mDefault; mTarget = cTarget; mCondition = cCond; mTextInput.bMustExist = cMustExist; mTextInput.bMustBeFile = cMustBeFile; mTextInput.bMustBeFolder = cMustBeFolder; mTextInput.bMustNotBeFile = cMustNotBeFile; mTextInput.bMustNotExist = cMustNotExist; mTextInput.bMustBeProject = cMustBeProject; mLabel = cLabel; mFileKey = "ChooseFileDlgKey" + cFileKey; mFileExt = cFileExt; mTextInput.bIsInputActive = true; mTextInput.bAutoComplete = true;}	
		virtual void render();
		virtual void cancel(){mValue = mDefault; mTextInput.mDialogTextMain = mDefault; bIsValid = false;}
		void close();
		virtual void setdefaults(){}
};

class DialogValueFileDefault : public DialogValueFile{
	public:
		std::string *mDefaultPath = NULL;
		DialogValueFileDefault(DTDialog *cParent, int cCond, std::string cLabel, std::string cInputLabel, std::string cFileExt, std::string cFileKey, std::string *cDefaultPath, std::string* cTarget, bool cMustExist, bool cMustBeFile, bool cMustBeFolder, bool cMustNotBeFile, bool cMustNotExist, bool cMustBeProject){mParent = cParent; mDefaultPath = cDefaultPath; mValue = *mDefaultPath; mTextInput.mInputLabel = cInputLabel; mTextInput.mDialogTextMain = *mDefaultPath; mTarget = cTarget; mCondition = cCond; mTextInput.bMustExist = cMustExist; mTextInput.bMustBeFile = cMustBeFile; mTextInput.bMustBeFolder = cMustBeFolder; mTextInput.bMustNotBeFile = cMustNotBeFile; mTextInput.bMustNotExist = cMustNotExist; mTextInput.bMustBeProject = cMustBeProject; mLabel = cLabel; mFileKey = "ChooseFileDlgKey" + cFileKey; mFileExt = cFileExt; mTextInput.bIsInputActive = true; mTextInput.bAutoComplete = true;}	
		virtual void update(){mValue = *mDefaultPath; mTextInput.mDialogTextMain = *mDefaultPath;};
		virtual void cancel(){mValue = *mDefaultPath; mTextInput.mDialogTextMain = *mDefaultPath; bIsValid = false;}
};

class OCDialog: public Dialog{
	public:		
		virtual int render();
};

class HDialog: public DTDialog{
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
		std::vector<std::string> mHelpTextAbout;
		std::vector<std::string> mHelpTextLicense;
		virtual void init();
		virtual void recieveInput(int mKey);		
		virtual int render();
};

class PODialog: public Dialog{
	public:		
		virtual int render(int xpos, int ypos, TextureParameters *mTexParam);
};

class TBDialog: public Dialog{
	public:		
		std::string mDialogTextWindow;		
		TEditor* mEditor;		
		virtual int render();
		bool bIsMenuOpen = false;
};

#endif