#include "TSettings.h"
#include "TDialogs.h"
#include "TEditor.h"

void Dialog::setColorScheme(int nScheme){}

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

	SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, mDialogColor.r,mDialogColor.g,mDialogColor.b,0xff);
	SDL_RenderFillRect(mGlobalSettings.TRenderer, &tmpBorder2);
	return tmpBorder;
}

int Dialog::recieveInput(int mx, int my){
	return 0;
}

void TBDialog::init(){
	mDialogBorder=3;
	mDialogColor = mGlobalSettings.DefaultDarkBGColor;
	mDialogBorderColor=  mGlobalSettings.DefaultButtonBorderColor;
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

void RTDialog::init(){
	mDialogTextMain = mTexDialogTextMain.mInfo +" Remove Unused Tiles. Undo will be cleared!";

	mAcceptButton.mDialogTextMain = "Remove";
	mCancelButton.mDialogTextMain = "Cancel";	
	
	mAcceptButton.init();	
	mCancelButton.init();
		
	mTexDialogTextMain.loadTTFFromUTF8(mDialogTextMain, mTextColor);
	
	mDialogWidth = mTexDialogTextMain.mTexWidth > mTexDialogTextInput.mTexWidth ? mTexDialogTextMain.mTexWidth : mTexDialogTextInput.mTexWidth;
	mDialogHeight = mTexDialogTextMain.mTexHeight > mTexDialogTextInput.mTexHeight ? mTexDialogTextMain.mTexHeight : mTexDialogTextInput.mTexHeight;
	
	mDialogHeight *=5;
	
	mDialogWidth += mDialogBorder * 4;
}

void RTDialog::recieveInput(int mKey){
	
	if(mKey == SDLK_y){
		bInputIsAccept=true;
		mGlobalSettings.mDeleteUnusedTilesState = 1;
	}
	if(mKey == SDLK_n){
		bInputIsCancel=true;
	}
}

void SDialog::init(){
	mDialogTextMain = mTexDialogTextMain.mFloppy +" Overwrite Project On Disk?";

	mAcceptButton.mDialogTextMain = "Save";
	mCancelButton.mDialogTextMain = "Cancel";	
	
	mAcceptButton.init();	
	mCancelButton.init();
		
	mTexDialogTextMain.loadTTFFromUTF8(mDialogTextMain, mTextColor);
	
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
	
	if(mKey == SDLK_y){
		bInputIsAccept=true;
		mGlobalSettings.mProjectSaveState = 1;
	}
	if(mKey == SDLK_n){
		bInputIsCancel=true;
	}
}

SDL_Rect SDialog::render(int xpos, int ypos){
	SDL_Rect tmpBorder = Dialog::render(xpos, ypos);

	mTexDialogTextMain.render(tmpBorder.x+mDialogBorder*2,tmpBorder.y+mDialogBorder*2);
	
	mAcceptButton.render(tmpBorder.x + ((mDialogWidth / 4) - (mAcceptButton.mDialogWidth/2)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight+mDialogBorder*3);	
	mCancelButton.render(tmpBorder.x + (((mDialogWidth / 4)*3) - (mAcceptButton.mDialogWidth/2)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight+mDialogBorder*3);	
		
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
	mDialogHeight = mTexDialogTextMain.mTexHeight;
	
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
	SDL_Rect tmpBorder = Dialog::render(xpos, ypos);

	mTexDialogTextMain.render(tmpBorder.x+mDialogBorder*2,tmpBorder.y+mDialogBorder*2);
	
/*
((mDialogWidth/4)-(mAcceptButton.mDialogWidth/2))
*/
	mAcceptButton.render(tmpBorder.x + ((mDialogWidth / 4) - (mAcceptButton.mDialogWidth/2)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight * 2 + mDialogBorder*4);	
	mCancelButton.render(tmpBorder.x + (((mDialogWidth / 4)*3) - (mAcceptButton.mDialogWidth/2)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight * 2+ mDialogBorder*4);	

	/*
	mAcceptButton.render(tmpBorder.x + ((tmpBorder.w / 4) - (mAcceptButton.mTexDialogTextMain.mTexWidth)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight * 2 + mDialogBorder*4);	
	mCancelButton.render(tmpBorder.x + (((tmpBorder.w / 4)*3) - (mCancelButton.mTexDialogTextMain.mTexWidth)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight * 2+ mDialogBorder*4);	
	*/	
	mTextInput.render(tmpBorder.x+((mDialogWidth/2)-(mTextInput.mDialogWidth/2)) ,tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight+mDialogBorder*2);
	if(bSubDialogActive){
		tmpBorder = mSubDialog->render(xpos + 50, ypos + 50);
	}
	return tmpBorder;
}

void SADialog::recieveInput(std::string mText){		
	mTextInput.mDialogTextMain += mText;
	mTextInput.mTextColor =  mGlobalSettings.DefaultTextColor;
	mTextInput.init();
	resize();	
}

void SADialog::recieveInput(int mKey){
	
	if(bSubDialogActive){
			if(mKey == SDLK_y){
				bInputIsAccept=true;
				bSubDialogActive = false;
				mGlobalSettings.mProjectSaveState = 1;
				mGlobalSettings.ProjectPath = mTextInput.mDialogTextMain;
			}	
			if(mKey == SDLK_n){
				bSubDialogActive=false;
				bDialogIsWatingForText = true;
				SDL_StartTextInput();				
			}		
	} else {
		if(mKey == SDLK_y){
			if(fs::is_directory(fs::status(mTextInput.mDialogTextMain))){
				bSubDialogActive = true;
				bDialogIsWatingForText = false;
			} else {
				if(fs::exists(fs::status(mTextInput.mDialogTextMain))){
					mTextInput.mTextColor = mGlobalSettings.ErrorTextColor;
					return;
				}
				mGlobalSettings.mProjectSaveState = 1;
				mGlobalSettings.ProjectPath = mTextInput.mDialogTextMain;
				bInputIsAccept=true;				
			}
			SDL_StopTextInput();
		}
		if(mKey == SDLK_n){
			bInputIsCancel=true;
			SDL_StopTextInput();			
		}		
	}
}

void SADialog::dropLastInputChar(){
	if(mTextInput.mDialogTextMain.size()){
		mTextInput.mDialogTextMain.pop_back();
		mTextInput.mTextColor = mGlobalSettings.DefaultTextColor;
		mTextInput.init();
		resize();
	}
}

void RNDialog::dropLastInputChar(){
	mTextInput.dropLastInputChar();
}


void RNDialog::recieveInput(std::string mStr){
	/*if(mTextInput.mDialogTextMain.size() < 10){
		for (int i = 0; i < mStr.size(); i++) {
      		if(isdigit(mStr[i])){
				mTextInput.mDialogTextMain += mStr[i];
				mTextInput.mTextColor =  mGlobalSettings.DefaultTextColor;
				mTextInput.init();
				resize();
	  		}
		}
	} */
	mTextInput.recieveInput(mStr);
	resize();
}


void RNDialog::recieveInput(int mKey){
	if(mKey == SDLK_y){
		std::stringstream convert;
		int cNumber;

		convert << mTextInput.mDialogTextMain  << std::endl;
		convert >> cNumber;
		
		mNumber = cNumber;
		bInputIsAccept=true;	
		bDialogIsWatingForText = false;

		SDL_StopTextInput();
		return;
	}

	if(mKey == SDLK_n){
		bInputIsCancel=true;
		SDL_StopTextInput();		
	}		
}

SDL_Rect RNDialog::render(int xpos, int ypos){	
	SDL_Rect tmpBorder = Dialog::render(xpos, ypos);

	mTexDialogTextMain.render(tmpBorder.x+mDialogBorder*2,tmpBorder.y+mDialogBorder*2);
	
	mAcceptButton.render(tmpBorder.x + ((mDialogWidth/4)-(mAcceptButton.mDialogWidth/2)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight * 2 + mDialogBorder*4);	
	mCancelButton.render(tmpBorder.x + (((mDialogWidth / 4)*3) - (mCancelButton.mDialogWidth/2)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight * 2+ mDialogBorder*4);	
		
	mTextInput.render(tmpBorder.x+((mDialogWidth/2)-(mTextInput.mDialogWidth/2)) ,tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight+mDialogBorder*2);

	return tmpBorder;
}


void RNDialog::cancel(){
		mTextInput.mTextColor = mGlobalSettings.DefaultTextColor;
		mTextInput.mDialogTextMain = "";
		bInputIsAccept=false;	
		bInputIsCancel=false;	
		bDialogIsWatingForText = false;
}

void RNDialog::init(){
	mDialogTextMain = mTexDialogTextMain.mInfo + " Enter Number         ";

	mAcceptButton.mDialogTextMain = "Accept";
	mCancelButton.mDialogTextMain = "Cancel";	
	
	mAcceptButton.init();	
	mCancelButton.init();

	mTexDialogTextMain.loadTTFFromUTF8(mDialogTextMain, mTextColor);
	
	mTextInput.mDialogTextMain = "";
	mTextInput.bIsInputActive = true;
	mTextInput.init();

	bSubDialogActive = false;

	mDialogWidth = (mAcceptButton.mTexDialogTextMain.mTexWidth  + mCancelButton.mTexDialogTextMain.mTexWidth) * 2  + (mDialogBorder*6);
	mDialogHeight = mTexDialogTextMain.mTexHeight * 6;
}

SDL_Rect ITDialog::render(int xpos, int ypos){	
	SDL_Rect tmpBorder = Dialog::render(xpos, ypos);

	mTexDialogTextMain.render(tmpBorder.x+mDialogBorder*2,tmpBorder.y+mDialogBorder*2);
	
	mAcceptButton.render(tmpBorder.x + ((tmpBorder.w / 4) - (mAcceptButton.mTexDialogTextMain.mTexWidth)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight * 2 + mDialogBorder*4);	
	mCancelButton.render(tmpBorder.x + (((tmpBorder.w / 4)*3) - (mCancelButton.mTexDialogTextMain.mTexWidth)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight * 2+ mDialogBorder*4);	
		
	mTextInput.render(tmpBorder.x+((mDialogWidth/2)-(mTextInput.mDialogWidth/2)) ,tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight+mDialogBorder*2);
	
	return tmpBorder;
}

void ITDialog::init(){
	mDialogTextMain = mTexDialogTextMain.mFile + " Import Tile from file";

	mAcceptButton.mDialogTextMain = "Open";
	mCancelButton.mDialogTextMain = "Cancel";	
	
	mAcceptButton.init();	
	mCancelButton.init();

		
	mTexDialogTextMain.loadTTFFromUTF8(mDialogTextMain, mTextColor);
	
	mTextInput.mDialogTextMain = "";
	mTextInput.bIsInputActive = true;
	mTextInput.init();

	bSubDialogActive = false;

	mDialogWidth = mTexDialogTextMain.mTexWidth + (mDialogBorder*6);
	mDialogHeight = mTexDialogTextMain.mTexHeight * 6;
}

void ITDialog::cancel(){
		Dialog::cancel();
		mTextInput.mTextColor =  mGlobalSettings.DefaultTextColor;
		mTextInput.mDialogTextMain = "";
}

void ITDialog::recieveInput(int mKey){
	if(mKey == SDLK_y){
		if(fs::exists(fs::status(mTextInput.mDialogTextMain))){
			if(fs::is_directory(fs::status(mTextInput.mDialogTextMain))){
				mTextInput.mTextColor = mGlobalSettings.ErrorTextColor;
				return;
			}
			bInputIsAccept=true;	
			bDialogIsWatingForText = false;
			mGlobalSettings.mOpenTileState = 1;
			mGlobalSettings.mNewTilePath = mTextInput.mDialogTextMain;
			SDL_StopTextInput();
			return;
		} else {
				mTextInput.mTextColor =  mGlobalSettings.ErrorTextColor;
				return;
			}
		}

	if(mKey == SDLK_n){
		bInputIsCancel=true;
		SDL_StopTextInput();		
	}		
}


int ITDialog::recieveInput(int mx, int my){
	return SDialog::recieveInput(mx, my);
}

void BDialog::init(){
	mDialogBorder = 3;
	mDialogBorderColor = mGlobalSettings.DefaultButtonBorderColor;
	mDialogColor = mGlobalSettings.DefaultButtonColor;

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
	
	int cTextX = xpos + (mDialogWidth-mTexDialogTextMain.mTexWidth)/2;
	int cTextY = ypos + (mDialogBorder + (mDialogBorder/3));
	mTexDialogTextMain.render(cTextX, cTextY);
	
	return mButtonRect;
}

void BDialog::setColorScheme(int nScheme){

	if(nScheme == 1){
		mDialogBorderColor = mGlobalSettings.ErrorBorderColor;
		mTextColor = mGlobalSettings.ErrorTextColor;
	} else {
		mDialogBorderColor = mGlobalSettings.DefaultBorderColor;
		mTextColor = mGlobalSettings.DefaultTextColor;
	}
}


void TIDialog::init(){

	mDialogBorder=3;
	mDialogBorderColor= mGlobalSettings.DefaultTextColor;
	mCursorTime += mGlobalSettings.getTicks();

	if(bInputIsAccepted){
		mTextColor = mGlobalSettings.DefaultTextColor;
	} else {
		mTextColor = mGlobalSettings.ErrorTextColor;
	}

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

void TIDialog::dropLastInputChar(){
	if(mDialogTextMain.size()){
		mDialogTextMain.pop_back();
		checkCurrentText();
		init();
	}
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

int TIDialog::checkCurrentText(){

	if(bIsNumeric){
		if(mMaxRange){		
			std::stringstream convert;
			convert << mDialogTextMain << std::endl;
			int tmpNum=0;
			convert >> tmpNum;
			if((tmpNum < mMinRange) || (tmpNum > mMaxRange)){
				
				bInputIsAccepted=false;
				return 1;
			}		
		}
	} else {
		bool cExists = false;
		bool cIsFolder = false;

		if(bMustExist || bMustNotExist){			
			if(fs::exists(fs::status(mDialogTextMain))){
				cExists = true;			
			}
		}

		if(bMustBeFolder){
			if(fs::is_directory(fs::status(mDialogTextMain))){
				cIsFolder = true;
			}
		}

		if(bMustExist){
				if(!cExists){				
				bInputIsAccepted=false;
				return 1;
			}
		} 

		if(bMustBeFolder){
				if(!cIsFolder){					
					bInputIsAccepted=false;
					return 1;
				}
		}
		
		if(bMustNotExist){
			if(cExists){				
				bInputIsAccepted=false;
				return 1;
			}
		}
	}

	bInputIsAccepted=true;	
	return 0;
}

void TIDialog::recieveInput(std::string cTextInput){
	
	if(bIsNumeric){		
		std::string tmpStr, tmpStr2;
		for (int i = 0; i < cTextInput.size(); i++) {
      		if(isdigit(cTextInput[i])){
				tmpStr += cTextInput[i];				
	  		}
		}

		tmpStr2 = mDialogTextMain + tmpStr;
		
		if(mMaxLength){
			if(tmpStr2.size() < mMaxLength){
				mDialogTextMain = tmpStr2;	
			}
		} else {
			mDialogTextMain = tmpStr2;
		}

		checkCurrentText();

	} else {
		std::string tmpStr = mDialogTextMain + cTextInput;

		if(mMaxLength){
			if(tmpStr.size() < mMaxLength){
				mDialogTextMain += cTextInput;
			}
		} else {
			mDialogTextMain += cTextInput;
		}

		checkCurrentText();
	}	
}

void HDialog::init(){
	
	TTFTexture* mNewText = new TTFTexture();
	std::string tmpStr = mNewText->mBook + " UI Guide: " + mCurModeHead;
	mNewText->loadTTFFromUTF8(tmpStr, mTextColor, mGlobalSettings.LFont);
	mHelpText.push_back(mNewText);
	
	for(const auto& mStr : mCurModeText){
		mNewText = new TTFTexture();
		mNewText->loadTTFFromString(mStr, mTextColor);
		mHelpText.push_back(mNewText);
	}
	
		mNewText = new TTFTexture();
		mNewText->loadTTFFromString(" ", mTextColor);
		mHelpText.push_back(mNewText);
	
		mNewText = new TTFTexture();
		mNewText->loadTTFFromString("General:", mTextColor);
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
	mDialogHeight = mHelpText[1]->mTexHeight * mHelpText.size()*1.5 + mHelpText[1]->mTexHeight*4;
		
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
		cTextY += (hTex->mTexHeight * 1.5);
	}
	
	mCloseButton.render(xpos + ((mDialogWidth/2)-(mCloseButton.mDialogWidth/2)), cTextY - (mDialogBorder*1.5));
	
	return tmpBorder;
}

void MEDialog::init(){

	TTFTexture* mNewText = new TTFTexture();
	mDialogTextMain = mNewText->mInfo + " ";
	mNewText->loadTTFFromUTF8(mDialogTextMain, mTextColor);
	mHelpText.push_back(mNewText);
		
	mCloseButton.mDialogTextMain = "Close";
	mCloseButton.init();

}
void MEDialog::setColorScheme(int nScheme){
	if(nScheme == 1){
		mDialogBorderColor = mGlobalSettings.ErrorBorderColor;
		mTextColor = mGlobalSettings.ErrorTextColor;		
	} else {
		mDialogBorderColor = mGlobalSettings.DefaultBorderColor;
		mTextColor = mGlobalSettings.DefaultTextColor;		
	}
}


void MEDialog::update(){

	mHelpText[0]->loadTTFFromUTF8(std::string(mHelpText[0]->mInfo + " " + mDialogTextMain), mTextColor);

	mDialogWidth = mHelpText[0]->mTexWidth + (mDialogBorder * 5);
	mDialogHeight = mHelpText[0]->mTexHeight * 5;
}


SDL_Rect MEDialog::render(int xpos, int ypos){

	SDL_Rect tmpBorder = Dialog::render(xpos, ypos);

	int cTextX = xpos + (3 * mDialogBorder);
	int cTextY = ypos + (3 * mDialogBorder);
		
	mHelpText[0]->render(cTextX, cTextY);	
	cTextY += (mHelpText[0]->mTexHeight * 2);
	
	mCloseButton.render(xpos + ((mDialogWidth/2)-(mCloseButton.mDialogWidth/2)), cTextY - (mDialogBorder*1.5));
	
	return tmpBorder;
}

void PIDialog::update(){
	std::stringstream convert;
	
	std::string cFlipH;
	std::string cFlipV;
	std::string cTileNum;
	std::string cCurTile;
	
	curtile = mEditor->mMapSelectedTile;
	tilenum = mEditor->mTileSet.TTiles.size();
	
	int cTileFlip = mEditor->mTileMap.getFlip(mGlobalSettings.mSelectedTile);

	fliph = (cTileFlip & 0x1) ? 1 : 0;
	flipv = (cTileFlip & 0x2) ? 1 : 0;

	convert << (curtile + 1) << std::endl;
	convert >> cCurTile;

	convert << tilenum << std::endl;
	convert >> cTileNum;

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
	
	convert << (cx +1) << std::endl;
	convert >> cTileX;
	
	convert << (cy+1) << std::endl;
	convert >> cTileY;

	tmpStr = "Current Tile: (" + cTileX + "," + cTileY + ")";
	
	mHelpText[mHelpText.size()-2]->loadTTFFromUTF8(tmpStr, mTextColor);

	tmpStr = "Selected in TileSet: " + cCurTile;	

	mHelpText[mHelpText.size()-3]->loadTTFFromUTF8(tmpStr, mTextColor);

	tmpStr = "Tiles in TileSet: " + cTileNum;

	mHelpText[mHelpText.size()-4]->loadTTFFromUTF8(tmpStr, mTextColor);

	int cMaxWidth = mHelpText[0]->mTexWidth;

	for(const auto* hTex : mHelpText){
		if(hTex->mTexWidth > cMaxWidth){cMaxWidth = hTex->mTexWidth;}
	}
	
	mDialogWidth = cMaxWidth + (mDialogBorder * 8);
}

void PIDialog::init(){

	mDialogBorder=3;
	mDialogColor = mGlobalSettings.DefaultDarkBGColor;
	mDialogBorderColor=  mGlobalSettings.DefaultButtonBorderColor;

	TTFTexture* mNewText;
	std::string tmpStr;
	std::stringstream convert;
	
	std::string cMapWidth;
	std::string cMapHeight;
	
	convert << mGlobalSettings.TileMapWidth << std::endl;
	convert >> cMapWidth;

	convert << mGlobalSettings.TileMapHeight << std::endl;
	convert >> cMapHeight;

	mNewText = new TTFTexture();
	tmpStr = "TileMap: " + cMapWidth + "x" + cMapHeight;
	mNewText->loadTTFFromUTF8(tmpStr, mTextColor);
	mHelpText.push_back(mNewText);

	std::string cTileSize;
	convert << mGlobalSettings.TileSize << std::endl;
	convert >> cTileSize;

	mNewText = new TTFTexture();
	tmpStr = "TileSize: " + cTileSize;
	mNewText->loadTTFFromUTF8(tmpStr, mTextColor);
	mHelpText.push_back(mNewText);

	mNewText = new TTFTexture();
	tmpStr = "Tiles in TileSet: ";
	mNewText->loadTTFFromUTF8(tmpStr, mTextColor);
	mHelpText.push_back(mNewText);

	mNewText = new TTFTexture();
	tmpStr = "Selected in TileSet: ";
	mNewText->loadTTFFromUTF8(tmpStr, mTextColor);
	mHelpText.push_back(mNewText);


	mNewText = new TTFTexture();
	tmpStr = "Current Tile: (0,0)";
	mNewText->loadTTFFromUTF8(tmpStr, mTextColor);
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
	mDialogHeight = mHelpText.size()*(mHelpText[0]->mTexHeight  + (mHelpText[0]->mTexHeight/2)) + (mDialogBorder * 4);
	mDialogWidth += mDialogBorder * 8;	
		
}

SDL_Rect PIDialog::render(int xpos, int ypos){
	SDL_Rect tmpBorder = Dialog::render(xpos, ypos);

	int cTextX = xpos + (3 * mDialogBorder);
	int cTextY = ypos + (3 * mDialogBorder);
		
	for(auto* hTex : mHelpText){
		hTex->render(cTextX, cTextY);	
		cTextY += (hTex->mTexHeight*1.5);
	}
		
	return tmpBorder;
}
