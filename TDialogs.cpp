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

void OPDialog::init(){
	mDialogTextMain = mTexDialogTextMain.mFile + " Open Project From Folder";

	mAcceptButton.mDialogTextMain = "Open";
	mCancelButton.mDialogTextMain = "Cancel";	
	
	mAcceptButton.init();	
	mCancelButton.init();

		
	mTexDialogTextMain.loadTTFFromUTF8(mDialogTextMain, mTextColor);
	
	mTextInput.mDialogTextMain = "";
	mTextInput.bIsInputActive = true;
	mTextInput.init();

	bSubDialogActive = false;
	mTextInput.bInputIsAccepted = false;

	mDialogWidth = mTexDialogTextMain.mTexWidth + (mDialogBorder*6);
	mDialogHeight = mTexDialogTextMain.mTexHeight * 6;
}


void OCDialog::init(){
	mDialogTextMain = mTexDialogTextMain.mInfo + " Open or Create New Project";
	mTexDialogTextMain.loadTTFFromUTF8(mDialogTextMain, mTextColor);
	mOpenButton.mDialogTextMain = "Open";
	mCreateButton.mDialogTextMain = "Create";
	mQuitButton.mDialogTextMain = "Quit";

	mOpenButton.init();
	mCreateButton.init();
	mQuitButton.init();

	mDialogWidth =  (mCreateButton.mDialogWidth * 4) + (mDialogBorder *2);
	mDialogHeight = (mCreateButton.mDialogHeight * 3) + (mDialogBorder *2);
	
	
	mOpenProject.init();
	mOpenProject.mTextInput.bMustBeFolder = true;
	
	mCreateProject.init();

	//mDialogHeight *=6;	
	//mDialogWidth += mDialogBorder * 5;	
}

SDL_Rect OCDialog::render(int xpos, int ypos){
	SDL_Rect tmpBorder = Dialog::render(xpos, ypos);

	mTexDialogTextMain.render(tmpBorder.x+(mDialogWidth/2)-(mTexDialogTextMain.mTexWidth/2),tmpBorder.y+mDialogBorder*2);

	mOpenButton.render(tmpBorder.x + (((mDialogWidth / 4))  - (mOpenButton.mDialogWidth/2)) - (mDialogBorder * 2), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight + mDialogBorder*3);	
	mCreateButton.render(tmpBorder.x + (((mDialogWidth / 4)*2) - (mCreateButton.mDialogWidth/2)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight + mDialogBorder*3);
	mQuitButton.render(tmpBorder.x + (((mDialogWidth / 4)*3) - (mQuitButton.mDialogWidth/2)) + (mDialogBorder * 2), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight + mDialogBorder*3);		

	if(bSubDialogActive && bSubDialogIsOpen){
		mOpenProject.render(xpos + 50, ypos + 50);
	}

	if(bSubDialogActive && bSubDialogIsCreate){
		mCreateProject.render(xpos + 50, ypos + 50);
	}

	return tmpBorder;
}

int OCDialog::recieveInput(int mx, int my){
	if(bSubDialogActive && bSubDialogIsOpen){
		if(mOpenProject.mAcceptButton.recieveInput(mx, my)){
			recieveInput(SDLK_y);
			return 0;
		}
		if(mOpenProject.mCancelButton.recieveInput(mx, my)){
			recieveInput(SDLK_n);
			return 0;
		}		
	}
	if(bSubDialogActive && bSubDialogIsCreate){
		mCreateProject.recieveInput(mx, my);
			//recieveInput(SDLK_y);
			return 0;
		//}
		//if(mOpenProject.mCancelButton.recieveInput(mx, my)){
		//	recieveInput(SDLK_n);
		//	return 0;
		//}		
	}
	if(mOpenButton.recieveInput(mx,my)){		
		bSubDialogActive = true;
		bSubDialogIsOpen = true;
		bDialogIsWatingForText = true;
		SDL_StartTextInput();
	}
	if(mCreateButton.recieveInput(mx,my)){
		bSubDialogActive = true;
		bSubDialogIsCreate = true;
		bDialogIsWatingForText = true;
		SDL_StartTextInput();
	}
	if(mQuitButton.recieveInput(mx,my)){
		recieveInput(SDLK_n);		
	}
	return 0;
}

void OCDialog::resize(){
	mOpenProject.mDialogWidth = mOpenProject.mTexDialogTextMain.mTexWidth > mOpenProject.mTextInput.mTexDialogTextMain.mTexWidth ? mOpenProject.mTexDialogTextMain.mTexWidth : mOpenProject.mTextInput.mTexDialogTextMain.mTexWidth;
	mOpenProject.mDialogWidth += mDialogBorder * 5;
}


void OCDialog::recieveInput(std::string cStr){
	if(bSubDialogActive && bSubDialogIsOpen){
		mOpenProject.mTextInput.recieveInput(cStr);		
		resize();
	}
	if(bSubDialogActive && bSubDialogIsCreate){
		mCreateProject.recieveInput(cStr);		
		//resize();
	}
}

void OCDialog::dropLastInputChar(){
	if(bSubDialogActive && bSubDialogIsOpen){
		mOpenProject.mTextInput.dropLastInputChar();
	}
	if(bSubDialogActive && bSubDialogIsCreate){
		mCreateProject.dropLastInputChar();
	}
}


void OCDialog::recieveInput(int mKey){
	
	if(mKey == SDLK_y){
		if(bSubDialogActive && bSubDialogIsOpen){
			if(mOpenProject.mTextInput.bInputIsAccepted){
				mGlobalSettings.ProjectPath = mOpenProject.mTextInput.mDialogTextMain;
				mGlobalSettings.mProjectOpenState = 1;
				SDL_StopTextInput();
				bInputIsAccept=true;
				bDialogIsWatingForText = false;
			}
		}
	}
	if(mKey == SDLK_n){
		if(bSubDialogActive && bSubDialogIsOpen){
			bSubDialogActive = false;
			bSubDialogIsOpen = false;
			bDialogIsWatingForText = false;
			SDL_StopTextInput();
			return;
		}

		bInputIsCancel=true;
	}
}

void CPDialog::init(){
	mDialogTextMain = mTexDialogTextMain.mFile + " Create New Project";
	mTexDialogTextMain.loadTTFFromUTF8(mDialogTextMain, mTextColor);

	mDialogInputWidth = "Map Width";		
	mTexDialogWidth.loadTTFFromUTF8(mDialogInputWidth, mTextColor);
	mDialogInputHeight = "Map Height";		
	mTexDialogHeight.loadTTFFromUTF8(mDialogInputHeight, mTextColor);
	mDialogInputSizeX = "Tile Size X";		
	mTexDialogSizeX.loadTTFFromUTF8(mDialogInputSizeX, mTextColor);
	mDialogInputSizeY = "Tile Size Y";		
	mTexDialogSizeY.loadTTFFromUTF8(mDialogInputSizeY, mTextColor);
	mDialogInputPath = "New Folder";		
	mTexDialogPath.loadTTFFromUTF8(mDialogInputPath, mTextColor);

	mCreateButton.mDialogTextMain = "Create";
	mCancelButton.mDialogTextMain = "Cancel";	
	
	mCreateButton.init();	
	mCancelButton.init();

	mReadWidth.mAllowedValues.push_back(32);
	mReadWidth.mAllowedValues.push_back(64);
	mReadWidth.mAllowedValues.push_back(128);
	mReadWidth.mAllowedValues.push_back(256);

	mReadHeight.mAllowedValues = mReadWidth.mAllowedValues;

	mReadSizeX.mAllowedValues.push_back(8);
	mReadSizeX.mAllowedValues.push_back(16);

	mReadSizeY.mAllowedValues = mReadSizeX.mAllowedValues;

	mReadWidth.mDialogTextMain = "32";
	mReadHeight.mDialogTextMain = "32";
	mReadSizeX.mDialogTextMain = "16";
	mReadSizeY.mDialogTextMain = "16";
	mReadPath.mDialogTextMain = "newfolder";

	mReadWidth.bIsNumeric= true;
	mReadHeight.bIsNumeric = true;
	mReadSizeX.bIsNumeric = true;
	mReadSizeY.bIsNumeric = true;
	mReadPath.bMustNotExist = true;

	mReadWidth.bInputIsAccepted = true;
	mReadHeight.bInputIsAccepted = true;
	mReadSizeX.bInputIsAccepted = true;
	mReadSizeY.bInputIsAccepted = true;
	mReadPath.bInputIsAccepted = true;

	mReadWidth.init();
	mReadHeight.init();
	mReadSizeX.init();
	mReadSizeY.init();
	mReadPath.init();

	mActiveInput = &mReadWidth;
	mActiveInput->bIsInputActive = true;


	mDialogWidth =  (mCreateButton.mDialogWidth * 4) + (mDialogBorder *2);
	mDialogHeight = (mCreateButton.mDialogHeight * 8) + (mDialogBorder);
}

SDL_Rect CPDialog::render(int xpos, int ypos){
	SDL_Rect tmpBorder = Dialog::render(xpos, ypos);

	int offset = 8;

	mTexDialogTextMain.render(tmpBorder.x+(mDialogWidth/2)-(mTexDialogTextMain.mTexWidth/2),tmpBorder.y+mDialogBorder*2);
	mTexDialogWidth.render(tmpBorder.x+(mDialogBorder*2),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * 1.2) + offset);
	mReadWidth.render(tmpBorder.x+(mDialogBorder*2) + (mTexDialogWidth.mTexWidth*1.4),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * 1.2)+ offset);

	mTexDialogHeight.render(tmpBorder.x+(mDialogBorder*2),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * 2.4)+ offset);
	mReadHeight.render(tmpBorder.x+(mDialogBorder*2) + (mTexDialogWidth.mTexWidth*1.4),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * 2.4)+ offset);

	mTexDialogSizeX.render(tmpBorder.x+(mDialogBorder*2),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * 3.6)+ offset);
	mReadSizeX.render(tmpBorder.x+(mDialogBorder*2) + (mTexDialogWidth.mTexWidth*1.4),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * 3.6)+ offset);

	mTexDialogSizeY.render(tmpBorder.x+(mDialogBorder*2),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * 4.8)+ offset);
	mReadSizeY.render(tmpBorder.x+(mDialogBorder*2) + (mTexDialogWidth.mTexWidth*1.4),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * 4.8)+ offset);

	mTexDialogPath.render(tmpBorder.x+(mDialogBorder*2),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * 6)+ offset);
	mReadPath.render(tmpBorder.x+(mDialogBorder*2) + (mTexDialogWidth.mTexWidth*1.4),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * 6)+ offset);

	mCreateButton.render(tmpBorder.x + (((mDialogWidth / 4)) - (mCreateButton.mDialogWidth/2)), tmpBorder.y+(mTexDialogTextMain.mTexHeight * 8.4)+ offset);
	mCancelButton.render(tmpBorder.x + (((mDialogWidth / 4)*3) - (mCancelButton.mDialogWidth/2)) + (mDialogBorder * 2), tmpBorder.y+(mTexDialogTextMain.mTexHeight * 8.4)+ offset);

	return tmpBorder;
}


void CPDialog::dropLastInputChar(){
	mActiveInput->dropLastInputChar();
}

void CPDialog::recieveInput(std::string cTextInput){
	mActiveInput->recieveInput(cTextInput);
}

void CPDialog::recieveInput(int mKey){}	

int CPDialog::recieveInput(int mx, int my){
	if(mReadWidth.recieveInput(mx, my)){
		mReadWidth.bIsInputActive = true;
		mReadHeight.bIsInputActive = false;
		mReadSizeX.bIsInputActive = false;
		mReadSizeY.bIsInputActive = false;
		mReadPath.bIsInputActive = false;
		mActiveInput = &mReadWidth;
	}
	if(mReadHeight.recieveInput(mx, my)){
		mReadWidth.bIsInputActive = false;
		mReadHeight.bIsInputActive = true;
		mReadSizeX.bIsInputActive = false;
		mReadSizeY.bIsInputActive = false;
		mReadPath.bIsInputActive = false;
		mActiveInput = &mReadHeight;
	}
	if(mReadSizeX.recieveInput(mx, my)){
		mReadWidth.bIsInputActive = false;
		mReadHeight.bIsInputActive = false;
		mReadSizeX.bIsInputActive = true;
		mReadSizeY.bIsInputActive = false;
		mReadPath.bIsInputActive = false;
		mActiveInput = &mReadSizeX;
	}
	if(mReadSizeY.recieveInput(mx, my)){
		mReadWidth.bIsInputActive = false;
		mReadHeight.bIsInputActive = false;
		mReadSizeX.bIsInputActive = false;
		mReadSizeY.bIsInputActive = true;
		mReadPath.bIsInputActive = false;
		mActiveInput = &mReadSizeY;
	}
	if(mReadPath.recieveInput(mx, my)){
		mReadWidth.bIsInputActive = false;
		mReadHeight.bIsInputActive = false;
		mReadSizeX.bIsInputActive = false;
		mReadSizeY.bIsInputActive = false;
		mReadPath.bIsInputActive = true;
		mActiveInput = &mReadPath;
	}
	return 0;
}

void RNDialog::dropLastInputChar(){
	mTextInput.dropLastInputChar();
}


void RNDialog::recieveInput(std::string mStr){	
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
		if(mAllowedValues.size()){
			std::stringstream convert;
			convert << mDialogTextMain << std::endl;
			int tmpNum=0;
			convert >> tmpNum;
			bool bValIsFound = false;
			for(auto &cVal : mAllowedValues){
				if(cVal == tmpNum){
					bValIsFound = true;				
				}
			}
			if(!bValIsFound){
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

	std::string cTileSizeX;
	convert << mGlobalSettings.TileSizeX << std::endl;
	convert >> cTileSizeX;

	std::string cTileSizeY;
	convert << mGlobalSettings.TileSizeY << std::endl;
	convert >> cTileSizeY;

	mNewText = new TTFTexture();
	tmpStr = "TileSizeX: " + cTileSizeX;
	mNewText->loadTTFFromUTF8(tmpStr, mTextColor);
	mHelpText.push_back(mNewText);

	mNewText = new TTFTexture();
	tmpStr = "TileSizeY: " + cTileSizeY;
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
