#include "TSettings.h"
#include "TDialogs.h"
#include "TEditor.h"
#include "imgui_stdlib.h"

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
	//SDL_Rect tmpRect = Dialog::render(xpos, ypos);
	
	mDialogTextWindow = mEditor->mCurMode == EMODE_MAP ? mGlobalSettings.mWindow+ " Tilemap Editor" : mGlobalSettings.mWindow + " Tile Editor";
	/*mTexDialogTextWindow.loadTTFFromUTF8(mDialogTextWindow, mTextColor);
	mTexDialogTextActions.render(mDialogBorder*4,(mDialogHeight-mTexDialogTextActions.mTexHeight)/2);
	mTexDialogTextWindow.render((mGlobalSettings.WindowWidth/2)-(mTexDialogTextWindow.mTexWidth/2),(mDialogHeight-mTexDialogTextActions.mTexHeight)/2);
	mTexDialogTextProject.render(mGlobalSettings.WindowWidth -  mTexDialogTextProject.mTexWidth - mDialogBorder*4, (mDialogHeight-mTexDialogTextActions.mTexHeight)/2);
	*/
	ImGui::BeginMainMenuBar();
		if (ImGui::BeginMenu("File"))
		{
			if(ImGui::MenuItem((std::string(mGlobalSettings.mFloppy + " Save (F12)")).c_str())){
				mGlobalSettings.CurrentEditor->activateSaveDialog();
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mFloppy + " Save As (F11)")).c_str())){
				mGlobalSettings.CurrentEditor->activateSaveAsDialog();
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mExit + " Quit")).c_str())){
				mGlobalSettings.CurrentEditor->bEditorRunning = false;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if(ImGui::MenuItem((std::string(mGlobalSettings.mWindow + " Tilemap")).c_str())){
				mGlobalSettings.CurrentEditor->mCurMode = EMODE_MAP;
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mWindow + " Tile")).c_str())){
				mGlobalSettings.CurrentEditor->mCurMode = EMODE_TILE;
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mInfo + " Project Info (F2)")).c_str())){
				mGlobalSettings.CurrentEditor->activateProjectInfo();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Empty Tile (F3)")).c_str())){
				mGlobalSettings.CurrentEditor->createNewTile();
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Import Tile (F4)")).c_str())){
				mGlobalSettings.CurrentEditor->activateOpenTileDialog();		  			
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Copy Tile (F5)")).c_str())){
				mGlobalSettings.CurrentEditor->createNewTileCopy(mGlobalSettings.CurrentEditor->mTileSelectedTile);
			}
			if(ImGui::MenuItem("Undo (U)")){
				mGlobalSettings.CurrentEditor->undoLastActionGroup();	  		
			}
			if(ImGui::MenuItem("Redo (R)")){
				mGlobalSettings.CurrentEditor->redoLastActionGroup();	  		
			}
			


			
			ImGui::EndMenu();
		}

    auto textWidth   = ImGui::CalcTextSize(mDialogTextWindow.c_str()).x;
	ImGui::SetCursorPosX((mGlobalSettings.WindowWidth - textWidth) * 0.5f);
	ImGui::Text(mDialogTextWindow.c_str());               // Display some text (you can use a format strings too)

	mGlobalSettings.TopBarHeight = ImGui::GetWindowHeight();

	ImGui::EndMainMenuBar();
	
	SDL_Rect tmpRect;
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
        	
	ImGui::Begin("Save Project");                         
    		
			ImGui::Text("Replace Project On Disk?");

            if (ImGui::Button("Save")){
				bInputIsAccept=true;
				mGlobalSettings.mProjectSaveState = 1;
			}

			if (ImGui::Button("Cancel")){
				bInputIsCancel=true;
			}
            
        ImGui::End();

	//SDL_Rect tmpBorder = Dialog::render(xpos, ypos);
	SDL_Rect tmpBorder;	
	//mTexDialogTextMain.render(tmpBorder.x+mDialogBorder*2,tmpBorder.y+mDialogBorder*2);
	
	//mAcceptButton.render(tmpBorder.x + ((mDialogWidth / 4) - (mAcceptButton.mDialogWidth/2)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight+mDialogBorder*3);	
	//mCancelButton.render(tmpBorder.x + (((mDialogWidth / 4)*3) - (mAcceptButton.mDialogWidth/2)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight+mDialogBorder*3);	
		
	return tmpBorder;
}

int SADialog::recieveInput(int mx, int my){
	/*
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
	}*/
	return 0;
}

void SADialog::resize(){


	int mMinDia = 400;

	//mDialogWidth = mTexDialogTextMain.mTexWidth > mTextInput.mTexDialogTextMain.mTexWidth ? mTexDialogTextMain.mTexWidth : mTextInput.mTexDialogTextMain.mTexWidth;
	mDialogWidth = mDialogWidth < mTextInput.mDialogWidth  ? mDialogWidth + (mDialogBorder *2) : mTextInput.mDialogWidth  + (mDialogBorder *8);
	//mDialogWidth = mPathWidth > mDialogWidth ? mPathWidth + (mDialogBorder * 2) : mDialogWidth; 
	mDialogHeight = mTexDialogTextMain.mTexHeight;
	
	mDialogWidth = mDialogWidth <  mMinDia ? mMinDia : mDialogWidth; 

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
	mTextInput.bMustNotBeFile = true;
	mTextInput.bAutoComplete = true;
	mTextInput.init();
	
	resize();
}

SDL_Rect SADialog::render(int xpos, int ypos){

ImGui::Begin("Save Project As");                         
    		
			ImGui::Text("Save Project As Folder");

			mTextInput.render(xpos ,  ypos);

            if (ImGui::Button("Save")){
				if(mTextInput.bInputIsAccepted){
					recieveInput(SDLK_y);					
				}				
			}

			if (ImGui::Button("Cancel")){
				bInputIsCancel=true;
			}
            
        ImGui::End();

	SDL_Rect tmpBorder;// = Dialog::render(xpos, ypos);
/*
	mTexDialogTextMain.render(tmpBorder.x+mDialogBorder*2,tmpBorder.y+mDialogBorder*2);
	
	mAcceptButton.render(tmpBorder.x + ((mDialogWidth / 4) - (mAcceptButton.mDialogWidth/2)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight * 2 + mDialogBorder*4);	
	mCancelButton.render(tmpBorder.x + (((mDialogWidth / 4)*3) - (mAcceptButton.mDialogWidth/2)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight * 2+ mDialogBorder*4);	

	
	mTextInput.render(tmpBorder.x+((mDialogWidth/2)-(mTextInput.mDialogWidth/2)) ,tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight+mDialogBorder*2);
	*/
	if(bSubDialogActive){
		tmpBorder = mSubDialog->render(xpos + 50, ypos + 50);
		if(mSubDialog->bInputIsAccept){
			recieveInput(SDLK_y);
		}
		if(mSubDialog->bInputIsCancel){
			recieveInput(SDLK_n);
			mSubDialog->bInputIsCancel = false;
		}
	}
	
	return tmpBorder;
}

void SADialog::recieveInput(std::string mText){		
	//mTextInput.recieveInput(mText);
	//mTextInput.mDialogTextMain += mText;
	//mTextInput.mTextColor =  mGlobalSettings.DefaultTextColor;
	//mTextInput.init();
	//resize();	
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
				//if(fs::exists(fs::status(mTextInput.mDialogTextMain))){
				//	mTextInput.mTextColor = mGlobalSettings.ErrorTextColor;
				//	return;
				//}
				if(mTextInput.bInputIsAccepted){
					mGlobalSettings.mProjectSaveState = 1;
					mGlobalSettings.ProjectPath = mTextInput.mDialogTextMain;
					bInputIsAccept=true;
				}	
			}
			SDL_StopTextInput();
		}
		if(mKey == SDLK_n){
			bInputIsCancel=true;
			SDL_StopTextInput();			
		}
		if(mKey == SDLK_TAB){		
			mTextInput.autoComplete();
			resize();
		}
	}
}

void SADialog::dropLastInputChar(){
	
	/*
	if(mTextInput.mDialogTextMain.size()){
		mTextInput.mDialogTextMain.pop_back();
		mTextInput.mTextColor = mGlobalSettings.DefaultTextColor;
		mTextInput.init();
		resize();
	}*/
	mTextInput.dropLastInputChar();
	//resize();
}

SDL_Rect OPDialog::render(int xpos, int ypos){

ImGui::Begin("Open Project");                         
    		
			ImGui::Text("Open Project from Folder");

			mTextInput.render(xpos ,  ypos);

            if (ImGui::Button("Open")){
				if(mTextInput.bInputIsAccepted){
					recieveInput(SDLK_y);					
				}				
			}

			if (ImGui::Button("Cancel")){
				bInputIsCancel=true;
			}		
            
        ImGui::End();

	SDL_Rect tmpRect;
	return tmpRect;

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
	mTextInput.bAutoComplete = true;
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
	
}

SDL_Rect OCDialog::render(int xpos, int ypos){
	SDL_Rect tmpBorder;// = Dialog::render(xpos, ypos);


	ImGui::BeginMainMenuBar();
		if (ImGui::BeginMenu("File"))
		{
			if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Open Project")).c_str())){
				bSubDialogActive = true;
				bSubDialogIsOpen = true;
				bDialogIsWatingForText = true;				
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Create Project")).c_str())){
				bSubDialogActive = true;
				bSubDialogIsCreate = true;
				bDialogIsWatingForText = true;				
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mExit + " Quit")).c_str())){
				mGlobalSettings.bRunningOCD = false;
			}

			ImGui::EndMenu();
		}

		std::string mDialogTextWindow = "Tilemaped";

		auto textWidth   = ImGui::CalcTextSize(mDialogTextWindow.c_str()).x;
		ImGui::SetCursorPosX((mGlobalSettings.WindowWidth - textWidth) * 0.5f);
		ImGui::Text(mDialogTextWindow.c_str());

		ImGui::EndMainMenuBar();


	/*
	mTexDialogTextMain.render(tmpBorder.x+(mDialogWidth/2)-(mTexDialogTextMain.mTexWidth/2),tmpBorder.y+mDialogBorder*2);

	mOpenButton.render(tmpBorder.x + (((mDialogWidth / 4))  - (mOpenButton.mDialogWidth/2)) - (mDialogBorder * 2), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight + mDialogBorder*3);	
	mCreateButton.render(tmpBorder.x + (((mDialogWidth / 4)*2) - (mCreateButton.mDialogWidth/2)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight + mDialogBorder*3);
	mQuitButton.render(tmpBorder.x + (((mDialogWidth / 4)*3) - (mQuitButton.mDialogWidth/2)) + (mDialogBorder * 2), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight + mDialogBorder*3);		


	*/
	if(bSubDialogActive && bSubDialogIsOpen){
		mOpenProject.render(xpos + 50, ypos + 50);
		if(mOpenProject.bInputIsCancel){
			bSubDialogActive = false;
			bSubDialogIsOpen = false;
			mOpenProject.bInputIsCancel = false;
		}
		if(mOpenProject.bInputIsAccept){
			recieveInput(SDLK_y);
		}
	}

	if(bSubDialogActive && bSubDialogIsCreate){
		mCreateProject.render(xpos + 50, ypos + 50);
		if(mCreateProject.bInputIsCancel){
			bSubDialogActive = false;
			bSubDialogIsCreate = false;
			mCreateProject.bInputIsCancel = false;
		}
		if(mCreateProject.bInputIsAccept){
			recieveInput(SDLK_y);
		}
	}

	return tmpBorder;
}

int OCDialog::recieveInput(int mx, int my){
	return 0;
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
		if(mCreateProject.mCreateButton.bInputIsAccept &&  mCreateProject.bInputIsAccepted){		
			recieveInput(SDLK_y);
		}
		if(mCreateProject.bInputIsCancel){
			recieveInput(SDLK_n);
		}			
		return 0;				
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
	return;
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
		if(bSubDialogActive && bSubDialogIsCreate){
			mCreateProject.recieveInput(mKey);
			if(mCreateProject.bInputIsAccepted){				
				std::stringstream convert;
				int mapx, mapy, tilex, tiley;
				
				convert << mCreateProject.mReadWidth.mDialogTextMain << std::endl;
				convert >> mapx;
				
				convert << mCreateProject.mReadHeight.mDialogTextMain << std::endl;
				convert >> mapy;

				convert << mCreateProject.mReadSizeX.mDialogTextMain << std::endl;
				convert >> tilex;

				convert << mCreateProject.mReadSizeY.mDialogTextMain << std::endl;
				convert >> tiley;
				
				mGlobalSettings.TileMapWidth = mCreateProject.tmapx;
				mGlobalSettings.TileMapHeight = mCreateProject.tmapy;
				mGlobalSettings.TileSizeX = mCreateProject.tilex;
				mGlobalSettings.TileSizeY = mCreateProject.tiley;
				mGlobalSettings.ProjectPath = mCreateProject.mReadPath.mDialogTextMain;

				if(mCreateProject.bHasPalette){
					if(mCreateProject.mReadPal.bInputIsAccepted){
						if(mGlobalSettings.testPaletteFile(mCreateProject.mReadPal.mDialogTextMain)){
							mGlobalSettings.bProjectHasPalette = true;
							mGlobalSettings.ProjectPalettePath = mCreateProject.mReadPal.mDialogTextMain;
						}
					}
				}

				mGlobalSettings.mProjectOpenState = 2;
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
	if(bSubDialogActive && bSubDialogIsCreate){
			bSubDialogActive = false;
			bSubDialogIsCreate = false;
			bDialogIsWatingForText = false;
			SDL_StopTextInput();
	 		return;
		}

		bInputIsCancel=true;
	}
	if(mKey == SDLK_TAB){
		if(bSubDialogActive && bSubDialogIsCreate){
			mCreateProject.recieveInput(SDLK_TAB);
		}
		if(bSubDialogActive && bSubDialogIsOpen){
			mOpenProject.mTextInput.autoComplete();
		}
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
	mDialogInputPath = "Folder";		
	mTexDialogPath.loadTTFFromUTF8(mDialogInputPath, mTextColor);
	mDialogInputPal = "Palette";		
	mTexDialogPal.loadTTFFromUTF8(mDialogInputPal, mTextColor);

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
	mReadPal.mDialogTextMain = "";

	mReadPath.mInputLabel = "Project Folder";
	mReadPal.mInputLabel = "Palette File";

	mReadWidth.bIsNumeric= true;
	mReadHeight.bIsNumeric = true;
	mReadSizeX.bIsNumeric = true;
	mReadSizeY.bIsNumeric = true;

	mReadPath.bMustNotExist = true;
	mReadPath.bAutoComplete = true;

	mReadPal.bMustBeFile = true;
	mReadPal.bAutoComplete = true;

	mReadWidth.bInputIsAccepted = true;
	mReadHeight.bInputIsAccepted = true;
	mReadSizeX.bInputIsAccepted = true;
	mReadSizeY.bInputIsAccepted = true;
	mReadPath.bInputIsAccepted = true;
	mReadPal.bInputIsAccepted = true;

	mReadPath.mMinDialogWidth = 200;
	mReadPal.mMinDialogWidth = 200;

	mReadWidth.init();
	mReadHeight.init();
	mReadSizeX.init();
	mReadSizeY.init();
	mReadPath.init();
	mReadPal.init();

	mActiveInput = &mReadWidth;
	mActiveInput->bIsInputActive = true;


	mDialogWidth =  (mCreateButton.mDialogWidth * 6) + (mDialogBorder *2);
	mDialogHeight = (mCreateButton.mDialogHeight * 9) + (mDialogBorder * 2);
}

void CPDialog::resize(){

	int mPalWidth,mPathWidth, mTestWidth;

	mPalWidth = (mReadPal.mDialogWidth + (mDialogBorder*4) + (mTexDialogWidth.mTexWidth*1.4));
	mPathWidth = (mReadPath.mDialogWidth + (mDialogBorder*4) + (mTexDialogWidth.mTexWidth*1.4));

	mTestWidth = mPalWidth < mPathWidth ? mPathWidth : mPalWidth;

	int mNewWidth = mTestWidth + (mDialogBorder * 2);


	if(mDialogWidth < mTestWidth){
		mDialogWidth = mTestWidth;
	}

	if(mDialogWidth > mTestWidth){
		mDialogWidth = mNewWidth  < (mCreateButton.mDialogWidth * 6) + (mDialogBorder *2) ? (mCreateButton.mDialogWidth * 6) + (mDialogBorder *2) : mNewWidth;
	}

}


SDL_Rect CPDialog::render(int xpos, int ypos){
	SDL_Rect tmpBorder;// = Dialog::render(xpos, ypos);

	

	ImGui::Begin("Create New Project");                         
    		
			ImGui::Text("TileMap Width & Height");  
			
			ImGui::RadioButton("W: 32", &tmapx, 32);
			ImGui::SameLine();
			ImGui::RadioButton("W: 64", &tmapx, 64);
			ImGui::SameLine();
			ImGui::RadioButton("W: 128", &tmapx, 128);
			ImGui::SameLine();
			ImGui::RadioButton("W: 256", &tmapx, 256);

			ImGui::RadioButton("H: 32", &tmapy, 32);
			ImGui::SameLine();
			ImGui::RadioButton("H: 64", &tmapy, 64);
			ImGui::SameLine();
			ImGui::RadioButton("H: 128", &tmapy, 128);
			ImGui::SameLine();
			ImGui::RadioButton("H: 256", &tmapy, 256);


			ImGui::Text("TileSize X/Y");  
			
			ImGui::RadioButton("X: 8", &tilex, 8);
			ImGui::SameLine();
			ImGui::RadioButton("X: 16", &tilex, 16);
			ImGui::SameLine();
			ImGui::RadioButton("Y: 8", &tiley, 8);
			ImGui::SameLine();
			ImGui::RadioButton("Y: 16", &tiley, 16);

			mReadPath.render(0,0);
			if(mReadPath.bIsActive){
				mActiveInput = &mReadPath;
			}

			ImGui::Checkbox("Use Palette", &bHasPalette);

			if(bHasPalette){
				mReadPal.render(0,0);	
				if(mReadPal.bIsActive){
				mActiveInput = &mReadPal;
			}
			}

			

            if (ImGui::Button("Create")){
				recieveInput(SDLK_y);
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel")){
				recieveInput(SDLK_n);
			}		
            
     ImGui::End();


/*
	int offset = 8;
	float multip = 1.3;


	mTexDialogTextMain.render(tmpBorder.x+(mDialogWidth/2)-(mTexDialogTextMain.mTexWidth/2),tmpBorder.y+mDialogBorder*2);
	mTexDialogWidth.render(tmpBorder.x+(mDialogBorder*2),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * multip) + offset);
	mReadWidth.render(tmpBorder.x+(mDialogBorder*2) + (mTexDialogWidth.mTexWidth*1.4),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * multip)+ offset);

	mTexDialogHeight.render(tmpBorder.x+(mDialogBorder*2),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * 2 * multip)+ offset);
	mReadHeight.render(tmpBorder.x+(mDialogBorder*2) + (mTexDialogWidth.mTexWidth*1.4),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * 2 * multip)+ offset);

	mTexDialogSizeX.render(tmpBorder.x+(mDialogBorder*2),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * 3 * multip)+ offset);
	mReadSizeX.render(tmpBorder.x+(mDialogBorder*2) + (mTexDialogWidth.mTexWidth*1.4),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * 3 * multip)+ offset);

	mTexDialogSizeY.render(tmpBorder.x+(mDialogBorder*2),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * 4 * multip)+ offset);
	mReadSizeY.render(tmpBorder.x+(mDialogBorder*2) + (mTexDialogWidth.mTexWidth*1.4),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * 4 * multip)+ offset);

	mTexDialogPath.render(tmpBorder.x+(mDialogBorder*2),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * 5 * multip)+ offset);
	mReadPath.render(tmpBorder.x+(mDialogBorder*2) + (mTexDialogWidth.mTexWidth*1.4),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * 5 * multip)+ offset);

	mTexDialogPal.render(tmpBorder.x+(mDialogBorder*2),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * 6 * multip)+ offset);
	mReadPal.render(tmpBorder.x+(mDialogBorder*2) + (mTexDialogWidth.mTexWidth*1.4),tmpBorder.y+ (mDialogBorder*2) + (mTexDialogTextMain.mTexHeight * 6 * multip)+ offset);

	mCreateButton.render(tmpBorder.x + (((mDialogWidth / 4)) - (mCreateButton.mDialogWidth/2)), tmpBorder.y+(mTexDialogTextMain.mTexHeight * 8 * multip)+ offset);
	mCancelButton.render(tmpBorder.x + (((mDialogWidth / 4)*3) - (mCancelButton.mDialogWidth/2)) + (mDialogBorder * 2), tmpBorder.y+(mTexDialogTextMain.mTexHeight * 8 * multip)+ offset);
*/
	return tmpBorder;
}


void CPDialog::dropLastInputChar(){
	mActiveInput->dropLastInputChar();
	resize();
}

void CPDialog::recieveInput(std::string cTextInput){
	mActiveInput->recieveInput(cTextInput);
	resize();
}

void CPDialog::recieveInput(int mKey){
	if(mKey == SDLK_n){
		bInputIsCancel = true;
	}
	if(mKey == SDLK_y){
		if( (mReadPath.bInputIsAccepted) ){
			bInputIsAccept=true;
			bInputIsAccepted = true;
		} else {
			bInputIsAccepted = false;
		}

	}
	if(mKey == SDLK_TAB){
		mActiveInput->autoComplete();
		resize();
	}
}	

int CPDialog::recieveInput(int mx, int my){
	return 0;
	if(mReadWidth.recieveInput(mx, my)){
		mReadWidth.bIsInputActive = true;
		mReadHeight.bIsInputActive = false;
		mReadSizeX.bIsInputActive = false;
		mReadSizeY.bIsInputActive = false;
		mReadPath.bIsInputActive = false;
		mReadPal.bIsInputActive = false;
		mActiveInput = &mReadWidth;
	}
	if(mReadHeight.recieveInput(mx, my)){
		mReadWidth.bIsInputActive = false;
		mReadHeight.bIsInputActive = true;
		mReadSizeX.bIsInputActive = false;
		mReadSizeY.bIsInputActive = false;
		mReadPath.bIsInputActive = false;
		mReadPal.bIsInputActive = false;
		mActiveInput = &mReadHeight;
	}
	if(mReadSizeX.recieveInput(mx, my)){
		mReadWidth.bIsInputActive = false;
		mReadHeight.bIsInputActive = false;
		mReadSizeX.bIsInputActive = true;
		mReadSizeY.bIsInputActive = false;
		mReadPath.bIsInputActive = false;
		mReadPal.bIsInputActive = false;
		mActiveInput = &mReadSizeX;
	}
	if(mReadSizeY.recieveInput(mx, my)){
		mReadWidth.bIsInputActive = false;
		mReadHeight.bIsInputActive = false;
		mReadSizeX.bIsInputActive = false;
		mReadSizeY.bIsInputActive = true;
		mReadPath.bIsInputActive = false;
		mReadPal.bIsInputActive = false;
		mActiveInput = &mReadSizeY;
	}
	if(mReadPath.recieveInput(mx, my)){
		mReadWidth.bIsInputActive = false;
		mReadHeight.bIsInputActive = false;
		mReadSizeX.bIsInputActive = false;
		mReadSizeY.bIsInputActive = false;
		mReadPath.bIsInputActive = true;
		mReadPal.bIsInputActive = false;
		mActiveInput = &mReadPath;
	}
	if(mReadPal.recieveInput(mx, my)){
		mReadWidth.bIsInputActive = false;
		mReadHeight.bIsInputActive = false;
		mReadSizeX.bIsInputActive = false;
		mReadSizeY.bIsInputActive = false;
		mReadPath.bIsInputActive = false;
		mReadPal.bIsInputActive = true;
		mActiveInput = &mReadPal;
	}

	if(mCreateButton.recieveInput(mx, my)){
		mCreateButton.bInputIsAccept = true;
		recieveInput(SDLK_y);
	}
	if(mCancelButton.recieveInput(mx, my)){
		recieveInput(SDLK_n);
	}
	return 0;
}

void RNDialog::dropLastInputChar(){
	mTextInput.dropLastInputChar();
}


int RNDialog::recieveInput(int mx, int my){
	return 1;
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
	
	
	 
		
			
			
		/*
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
		*/
		

    		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    		ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            //ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();

    

	/*
	ImGui::Begin("Another Window", &show_another_window);  
            ImGui::Text("Hello from another window!");
    ImGui::End();*/
	
	/*SDL_Rect tmpBorder = Dialog::render(xpos, ypos);

	mTexDialogTextMain.render(tmpBorder.x+mDialogBorder*2,tmpBorder.y+mDialogBorder*2);
	
	mAcceptButton.render(tmpBorder.x + ((mDialogWidth/4)-(mAcceptButton.mDialogWidth/2)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight * 2 + mDialogBorder*4);	
	mCancelButton.render(tmpBorder.x + (((mDialogWidth / 4)*3) - (mCancelButton.mDialogWidth/2)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight * 2+ mDialogBorder*4);	
		
	mTextInput.render(tmpBorder.x+((mDialogWidth/2)-(mTextInput.mDialogWidth/2)) ,tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight+mDialogBorder*2);
*/
	SDL_Rect tmpBorder;
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
	SDL_Rect tmpBorder; // = Dialog::render(xpos, ypos);

	//mTexDialogTextMain.render(tmpBorder.x+mDialogBorder*2,tmpBorder.y+mDialogBorder*2);
	
	//mAcceptButton.render(tmpBorder.x + ((tmpBorder.w / 4) - (mAcceptButton.mTexDialogTextMain.mTexWidth)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight * 2 + mDialogBorder*4);	
	//mCancelButton.render(tmpBorder.x + (((tmpBorder.w / 4)*3) - (mCancelButton.mTexDialogTextMain.mTexWidth)), tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight * 2+ mDialogBorder*4);	


ImGui::Begin("Import Tile");                         
    		
			ImGui::Text("Import Tile from bitmap or RAW");

			mTextInput.render(xpos ,  ypos);

            if (ImGui::Button("Import")){
				if(mTextInput.bInputIsAccepted){
					recieveInput(SDLK_y);					
				}				
			}

			if (ImGui::Button("Cancel")){
				bInputIsCancel=true;
			}		
            
        ImGui::End();


	//mTextInput.render(tmpBorder.x+((mDialogWidth/2)-(mTextInput.mDialogWidth/2)) ,tmpBorder.y+mDialogBorder+mTexDialogTextMain.mTexHeight+mDialogBorder*2);
	
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
	mTextInput.bAutoComplete = true;
	mTextInput.bMustBeFile = true;
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
		if(mTextInput.bInputIsAccepted){
			bInputIsAccept=true;	
			bDialogIsWatingForText = false;
			mGlobalSettings.mOpenTileState = 1;
			mGlobalSettings.mNewTilePath = mTextInput.mDialogTextMain;
			SDL_StopTextInput();
		}
		/*if(fs::exists(fs::status(mTextInput.mDialogTextMain))){
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
			} */
	}

	if(mKey == SDLK_n){
		bInputIsCancel=true;
		SDL_StopTextInput();		
	}		
	if(mKey == SDLK_TAB){
		mTextInput.autoComplete();
		resize();
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
	
	if(bAutoComplete){
		if(mCompleteText.length()){
			mCompleteCursor = mCompleteText.substr(0,1);
			mCompleteAfter = mCompleteText.substr(1,mCompleteText.length()-1);
		} else {
			mCompleteCursor = " ";
			mCompleteAfter = "";
		}
		
		mTexDialogTextMain.loadTTFFromUTF8(mTexDialogTextMain.mPrompt + " " + mDialogTextMain, mTextColor,mGlobalSettings.TFont);

		if(bShowCursor){
			mTexCompleteCursor.loadTTFFromUTF8(mCompleteCursor, mGlobalSettings.AltTextColor,mGlobalSettings.UFont);
		} else {
			mTexCompleteCursor.loadTTFFromUTF8(mCompleteCursor, mGlobalSettings.AltTextColor,mGlobalSettings.TFont);
		}

		mTexCompleteAfter.loadTTFFromUTF8(mCompleteAfter, mGlobalSettings.AltTextColor,mGlobalSettings.TFont);

		//mTexCompleteText.loadTTFFromUTF8(mCompleteText, mGlobalSettings.AltTextColor,mGlobalSettings.TFont);
	} else {
		if(bShowCursor){
			mTexDialogTextMain.loadTTFFromUTF8(mTexDialogTextMain.mPrompt + " " + mDialogTextMain +"_", mTextColor,mGlobalSettings.TFont);
		} else {	
			mTexDialogTextMain.loadTTFFromUTF8(mTexDialogTextMain.mPrompt + " " + mDialogTextMain + " ", mTextColor,mGlobalSettings.TFont);
		}
	}
	if(bAutoComplete && mCompleteText.length()){
		mDialogWidth = mTexDialogTextMain.mTexWidth + mTexCompleteCursor.mTexWidth +  (mDialogBorder * 2);
		//std::cout << "CompleteCursor: " <<  mTexCompleteCursor.mTexWidth << std::endl;
		if(mCompleteAfter.length()){
			//std::cout << "CompleteAfter: " << mTexCompleteAfter.mTexWidth << std::endl;
			mDialogWidth += mTexCompleteAfter.mTexWidth + (mDialogBorder * 2);
		}
	} else {
		mDialogWidth = mTexDialogTextMain.mTexWidth + (mDialogBorder * 2);
	}
	
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
		//init();
	}
}

int TIDialog::recieveInput(int mx, int my){
	return BDialog::recieveInput(mx, my);
}

SDL_Rect TIDialog::render(int xpos, int ypos){
	//ImGui::InputText("FileName", &mDialogTextMain);
	//strcpy((char*)mDialogTextMain.c_str(), tmpbuf);
	checkCurrentText();

	if(bInputIsAccepted){
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(mGlobalSettings.DefaultTextColor.r,mGlobalSettings.DefaultTextColor.g,mGlobalSettings.DefaultTextColor.b ,255));
	} else {
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(mGlobalSettings.ErrorTextColor.r,mGlobalSettings.ErrorTextColor.g,mGlobalSettings.ErrorTextColor.b ,255));
	}

	//ImGui::InputText("##text1", txt_green, sizeof(txt_green));
	

	if(bAutoComplete){
		ImGui::InputText(mInputLabel.c_str(), &mDialogTextMain, ImGuiInputTextFlags_CallbackCompletion, MyCallback, &mCompleteText);
		bIsActive = ImGui::IsItemActive();
	} else {
		ImGui::InputText(mInputLabel.c_str(), &mDialogTextMain);
		bIsActive = ImGui::IsItemActive();
	}

	ImGui::PopStyleColor();

	//init();
	/*mButtonRect = Dialog::render(xpos, ypos);
	
	int cTextX = xpos + (mDialogBorder *2);
	int cTextY = ypos + (mDialogBorder + (mDialogBorder/3));
	mTexDialogTextMain.render(cTextX, cTextY);
	if(bAutoComplete){
		mTexCompleteCursor.render(cTextX+mTexDialogTextMain.mTexWidth, cTextY);
		mTexCompleteAfter.render(cTextX+mTexDialogTextMain.mTexWidth+mTexCompleteCursor.mTexWidth, cTextY);
	}
	*/
	//mButtonRect = Dialog;
	return mButtonRect;


}


 int TIDialog::MyCallback(ImGuiInputTextCallbackData* data)
                {
                    if (data->EventFlag == ImGuiInputTextFlags_CallbackCompletion)
                    {
						
						//strcpy(mDialogTextMain.c_str(), newbuf);
						
						const char* tmp = ((std::string*)(data->UserData))->c_str();

                        data->InsertChars(data->CursorPos, tmp);
                   }
			}


void TIDialog::autoComplete(){
	if(bAutoComplete){
		/*
		mDialogTextMain += mCompleteText;
		mCompleteText = "";			
		checkCurrentText();*/
		fs::path cPath = mDialogTextMain;
			std::string mDir,mFile;

			if(cPath.parent_path() == ""){
				mDir = ".";
			} else {
				mDir = cPath.parent_path();
			}

			mCompleteText = "";
			mFile = cPath.filename();

			std::cout << "Complete Before" << std::endl;

			if(fs::is_directory(fs::status(mDir))){
				for (const auto & entry : fs::directory_iterator(mDir)){
					std::string tStr = (entry.path()).filename();
					std::cout << "Complete:" << mFile << std::endl;
					if(mFile.length()){
						std::size_t subpos = tStr.find(mFile);						
						if((subpos != std::string::npos) && (subpos == 0)){												
							mCompleteText = tStr.substr(mFile.length());
							std::cout << "Complete Subpos:" << mCompleteText << std::endl;
							//mDialogTextMain += mCompleteText;
							//std::cout << mDialogTextMain << std::endl;
							//strcpy((char*)mDialogTextMain.c_str(), tmpbuf);
							//mCompleteText = "";
							break;
						} 				
					}
				}
		}
		
	}
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

		//if(bMustExist || bMustNotExist || bMustBeFile || bMustNotBeFile){			
			if(fs::exists(fs::status(mDialogTextMain))){
				cExists = true;			
			}
		//}

		//if(bMustBeFolder || bMustBeFile || bMustNotBeFile){
			if(fs::is_directory(fs::status(mDialogTextMain))){
				cIsFolder = true;
			}
		//}

		/*
		if(bAutoComplete){
			fs::path cPath = mDialogTextMain;
			std::string mDir,mFile;

			if(cPath.parent_path() == ""){
				mDir = ".";
			} else {
				mDir = cPath.parent_path();
			}

			mCompleteText = "";
			mFile = cPath.filename();

			if(fs::is_directory(fs::status(mDir))){
				for (const auto & entry : fs::directory_iterator(mDir)){
					std::string tStr = (entry.path()).filename();
					if(mFile.length()){
						std::size_t subpos = tStr.find(mFile);						
						if((subpos != std::string::npos) && (subpos == 0)){					
							mCompleteText = tStr.substr(mFile.length());							
							break;
						} 				
					}
				}
			}
			init();        	
		}*/

		if(bMustBeFile){
			if(cIsFolder){					
				bInputIsAccepted=false;
				return 1;
			}
			if(!cExists){								
				bInputIsAccepted=false;
				return 1;
			} 			
		} 

		if(bMustNotBeFile){			
			if(cExists){
				if(!cIsFolder){					
					bInputIsAccepted=false;
					return 1;
				} 
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
		mCompleteText = "";
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

	SDL_Rect tmpBorder; // = Dialog::render(xpos, ypos);


ImGui::Begin("Message");                         
    		

			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(mTextColor.r, mTextColor.g, mTextColor.b, 255));
			ImGui::Text(mDialogTextMain.c_str());
			ImGui::PopStyleColor();
			//mTextInput.render(xpos ,  ypos);
            
			if (ImGui::Button("Close")){
				bInputIsCancel=true;
			}
            
        ImGui::End();
	//int cTextX = xpos + (3 * mDialogBorder);
	//int cTextY = ypos + (3 * mDialogBorder);
		
	//mHelpText[0]->render(cTextX, cTextY);	
	//cTextY += (mHelpText[0]->mTexHeight * 2);
	
	//mCloseButton.render(xpos + ((mDialogWidth/2)-(mCloseButton.mDialogWidth/2)), cTextY - (mDialogBorder*1.5));
	
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

	
	TileSizeX = "TileSizeX " + cTileSizeX;
	TileSizeY = "TileSizeY " + cTileSizeY;

	Flip = "FlipH: "+cFlipH + " FlipV: " + cFlipV;
	
	std::string cTileX,cTileY;
	int cx,cy;
	
	cx = mGlobalSettings.mSelectedTile % mGlobalSettings.TileMapWidth;
	cy = mGlobalSettings.mSelectedTile / mGlobalSettings.TileMapWidth;
	
	convert << (cx +1) << std::endl;
	convert >> cTileX;
	
	convert << (cy+1) << std::endl;
	convert >> cTileY;

	TileMap = "TileMap " + cMapWidth + "x" + cMapHeight;

	CurrentTile = "Current Tile: (" + cTileX + "," + cTileY + ")";
	
	
	SelectedTile = "Selected in TileSet: " + cCurTile;	


	TilesInSet = "Tiles in TileSet: " + cTileNum;
}

void PIDialog::init(){

	
	std::stringstream convert;
	
		
	convert << mGlobalSettings.TileMapWidth << std::endl;
	convert >> cMapWidth;

	convert << mGlobalSettings.TileMapHeight << std::endl;
	convert >> cMapHeight;


	convert << mGlobalSettings.TileSizeX << std::endl;
	convert >> cTileSizeX;

	convert << mGlobalSettings.TileSizeY << std::endl;
	convert >> cTileSizeY;

	fliph=0;
	flipv=0;	

	update();
		
		
}

SDL_Rect PIDialog::render(int xpos, int ypos){
	SDL_Rect tmpBorder; // = Dialog::render(xpos, ypos);

	
	ImGui::Begin("\uf449 Project Info", &mGlobalSettings.bShowProjectInfo);

	ImGui::Text(TileMap.c_str());
	ImGui::Text(TileSizeX.c_str());
	ImGui::Text(TileSizeY.c_str());

	ImGui::Text(TilesInSet.c_str());
	ImGui::Text(SelectedTile.c_str());

	ImGui::Text(CurrentTile.c_str());
	ImGui::Text(Flip.c_str());
	
	ImGui::End();
		
	return tmpBorder;
}
