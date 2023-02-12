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

void Dialog::dropLastInputChar(){

}

SDL_Rect Dialog::render(int xpos, int ypos){
	SDL_Rect tmpBorder;
	
	return tmpBorder;
}


void TBDialog::init(){
	mDialogColor = mGlobalSettings.DefaultDarkBGColor;
	mDialogBorderColor=  mGlobalSettings.DefaultButtonBorderColor;
	mDialogWidth = mGlobalSettings.WindowWidth;
	mDialogHeight =  mGlobalSettings.TopBarHeight;
	mDialogTextActions = mGlobalSettings.mFloppy +" Save/Save As: F12/F11    " + mGlobalSettings.mBook + " Help: F1";	
	
	mDialogTextWindow = mEditor->mCurMode == EMODE_MAP ? mGlobalSettings.mWindow + " TileMap Editor" : mGlobalSettings.mWindow + " Tile Editor";
	
	mDialogTextProject = mGlobalSettings.mFile + " Project: " + mGlobalSettings.ProjectPath + "  " + mGlobalSettings.mInfo + " Info: F2";
	
}

SDL_Rect TBDialog::render(int xpos, int ypos){
	mDialogWidth = mGlobalSettings.WindowWidth;
		
	mDialogTextWindow = mEditor->mCurMode == EMODE_MAP ? mGlobalSettings.mWindow+ " Tilemap Editor" : mGlobalSettings.mWindow + " Tile Editor";
	
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
			if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_MAP){
				if(ImGui::MenuItem("Selected Tile Type (S)", NULL , &mGlobalSettings.bShowTypeSelection)){										
				}
				if(ImGui::MenuItem("Selected Tile (T)", NULL , &mGlobalSettings.bShowSelectedTile)){										
				}		
				
			}
			if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_TILE){
				if(ImGui::MenuItem("Selected Color (S)", NULL , &mGlobalSettings.bShowPixelType)){										
				}
				if(ImGui::MenuItem("Pixel Grid (P)", NULL , &mGlobalSettings.bShowPixelGrip)){										
				}
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
			if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Remove Unused Tile (F6)")).c_str())){
				mGlobalSettings.CurrentEditor->activateDropUnusedTiles();
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
	ImGui::Text("%s", mDialogTextWindow.c_str());               

	mGlobalSettings.TopBarHeight = ImGui::GetWindowHeight();

	ImGui::EndMainMenuBar();
	
	SDL_Rect tmpRect;
	return tmpRect;
}

void RTDialog::init(){
	mDialogTextMain = mGlobalSettings.mInfo +" Remove Unused Tiles";	
}

SDL_Rect RTDialog::render(int xpos, int ypos){
        	
	ImGui::Begin(mDialogTextMain.c_str());                         
    		
			ImGui::Text("Remove Unused Tiles? Undo Stack will be cleared!");

            if (ImGui::Button("Remove")){
				recieveInput(SDLK_y);
			}

			if (ImGui::Button("Cancel")){
				recieveInput(SDLK_n);
			}
            
        ImGui::End();

	
	SDL_Rect tmpBorder;			
	return tmpBorder;
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
	mDialogTextMain = mGlobalSettings.mFloppy +" Overwrite Project On Disk?";
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
    		
			ImGui::Text("%s", mDialogTextMain.c_str()); //mDialogTextMain.c_str());

            if (ImGui::Button("Save")){
				bInputIsAccept=true;
				mGlobalSettings.mProjectSaveState = 1;
			}

			if (ImGui::Button("Cancel")){
				bInputIsCancel=true;
			}
            
        ImGui::End();

	
	SDL_Rect tmpBorder;			
	return tmpBorder;
}


void SADialog::cancel(){
	Dialog::cancel();
	mSubDialog->cancel();
	bSubDialogActive = false;
}

void SADialog::init(){
	mDialogTextMain = mGlobalSettings.mFloppy + " Save Project As Folder";
	
	mTextInput.bIsInputActive = true;
	mTextInput.bMustNotBeFile = true;
	mTextInput.bAutoComplete = true;
	mTextInput.init();	
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

	SDL_Rect tmpBorder;

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
		}
	}
}

void SADialog::dropLastInputChar(){	
	mTextInput.dropLastInputChar();	
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
	mDialogTextMain = mGlobalSettings.mFile + " Open Project From Folder";
	
	mTextInput.mDialogTextMain = "";
	mTextInput.bIsInputActive = true;
	mTextInput.bAutoComplete = true;
	mTextInput.init();

	bSubDialogActive = false;
	mTextInput.bInputIsAccepted = false;
}


void OCDialog::init(){	
	mOpenProject.init();
	mOpenProject.mTextInput.bMustBeFolder = true;
	
	mCreateProject.init();
	
}

SDL_Rect OCDialog::render(int xpos, int ypos){
	SDL_Rect tmpBorder;


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
		ImGui::Text("%s", mDialogTextWindow.c_str());

		ImGui::EndMainMenuBar();
	
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
	mDialogTextMain = mGlobalSettings.mFile + " Create New Project";	
		
	mReadPath.mDialogTextMain = "newfolder";
	mReadPal.mDialogTextMain = "";

	mReadPath.mInputLabel = "Project Folder";
	mReadPal.mInputLabel = "Palette File";

	mReadPath.bMustNotExist = true;
	mReadPath.bAutoComplete = true;

	mReadPal.bMustBeFile = true;
	mReadPal.bAutoComplete = true;

	mReadPath.bInputIsAccepted = true;
	mReadPal.bInputIsAccepted = true;


	mReadPath.init();
	mReadPal.init();

	mActiveInput = &mReadPath;
	mActiveInput->bIsInputActive = true;

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

	return tmpBorder;
}


void CPDialog::dropLastInputChar(){
	mActiveInput->dropLastInputChar();

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
	}
}	

void RNDialog::dropLastInputChar(){
	mTextInput.dropLastInputChar();
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
	ImGui::Begin("Hello, world!", &isShown);                          // Create a window called "Hello, world!" and append into it.

   		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
   		/*ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
   		ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
            //ImGui::SameLine();
	    ImGui::Text("counter = %d", counter);*/
		
		ImGui::Image((ImTextureID)(intptr_t)mGlobalSettings.CurrentEditor->mTileSet.TTiles[1]->TileTex, ImVec2(160,160));
		if(ImGui::IsMouseClicked(1)){
				int x = mGlobalSettings.mio->MousePos.x;
				int y = mGlobalSettings.mio->MousePos.y;
				wpos = ImGui::GetWindowPos();
				wsize = ImGui::GetWindowSize();
				elmin = ImGui::GetItemRectMin();
				elmax = ImGui::GetItemRectMax();
				esize = ImGui::GetItemRectSize();
				wx = ImGui::GetWindowContentRegionMin().x;
				wy = ImGui::GetWindowContentRegionMin().y;				
				float wxm = ImGui::GetWindowContentRegionMax().x;
				float wym = ImGui::GetWindowContentRegionMax().y;	
				
				ImGui::Text("%d", x); ImGui::SameLine(); ImGui::Text("%d", y);
				ImGui::Text("%f", wpos.x); ImGui::SameLine(); ImGui::Text("%f", wpos.y);
				ImGui::Text("%f", wsize.x); ImGui::SameLine(); ImGui::Text("%f", wsize.y);
				ImGui::Text("%f", elmin.x); ImGui::SameLine(); ImGui::Text("%f", elmin.y);
				ImGui::Text("%f", elmax.x); ImGui::SameLine(); ImGui::Text("%f", elmax.y);


				if( (x >= (elmin.x)) &&  (x <= (elmax.x)) &&  (y >= (elmin.y))  &&  (y <= (elmax.y))){
					isShown = false;
					mGlobalSettings.CurrentEditor->mActiveDialog = NULL;
				}
		}
		
		
    ImGui::End();

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

}


SDL_Rect ITDialog::render(int xpos, int ypos){	
	SDL_Rect tmpBorder; 

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
	
	return tmpBorder;
}


void ITDialog::init(){
	mTextInput.bIsInputActive = true;
	mTextInput.bAutoComplete = true;
	mTextInput.bMustBeFile = true;	
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
	}

	if(mKey == SDLK_n){
		bInputIsCancel=true;
		SDL_StopTextInput();		
	}		
	if(mKey == SDLK_TAB){
		mTextInput.autoComplete();		
	}
}


void BDialog::init(){
		
}

SDL_Rect BDialog::render(int xpos, int ypos){
	
	mButtonRect;
	
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
	
}

void TIDialog::dropLastInputChar(){
	if(mDialogTextMain.size()){
		mDialogTextMain.pop_back();
		checkCurrentText();		
	}
}

SDL_Rect TIDialog::render(int xpos, int ypos){
	checkCurrentText();

	if(bInputIsAccepted){
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(mGlobalSettings.DefaultTextColor.r,mGlobalSettings.DefaultTextColor.g,mGlobalSettings.DefaultTextColor.b ,255));
	} else {
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(mGlobalSettings.ErrorTextColor.r,mGlobalSettings.ErrorTextColor.g,mGlobalSettings.ErrorTextColor.b ,255));
	}

	if(bAutoComplete){
		ImGui::InputText(mInputLabel.c_str(), &mDialogTextMain, ImGuiInputTextFlags_CallbackCompletion, MyCallback, &mCompleteText);
		bIsActive = ImGui::IsItemActive();
	} else {
		ImGui::InputText(mInputLabel.c_str(), &mDialogTextMain);
		bIsActive = ImGui::IsItemActive();
	}

	ImGui::PopStyleColor();

	return mButtonRect;
}


 int TIDialog::MyCallback(ImGuiInputTextCallbackData* data){
	if (data->EventFlag == ImGuiInputTextFlags_CallbackCompletion){						
		const char* tmp = ((std::string*)(data->UserData))->c_str();
    	data->InsertChars(data->CursorPos, tmp);
    }

	return 0;
}


void TIDialog::autoComplete(){
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

		if(fs::exists(fs::status(mDialogTextMain))){
			cExists = true;			
		}

		if(fs::is_directory(fs::status(mDialogTextMain))){
			cIsFolder = true;
		}


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

void HDialog::init(){
	
}

void HDialog::recieveInput(int mKey){
	bInputIsCancel=true;
}


SDL_Rect HDialog::render(int xpos, int ypos){
	SDL_Rect tmpBorder;
	
	return tmpBorder;
}

void MEDialog::init(){
	mDialogTextMain = mGlobalSettings.mInfo + " ";
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

}


SDL_Rect MEDialog::render(int xpos, int ypos){

	SDL_Rect tmpBorder; 

	ImGui::Begin("Message");                         
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(mTextColor.r, mTextColor.g, mTextColor.b, 255));
		ImGui::Text("%s", mDialogTextMain.c_str());
		ImGui::PopStyleColor();
		    
		if (ImGui::Button("Close")){
			bInputIsCancel=true;
		}
            
    ImGui::End();

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
	SDL_Rect tmpBorder; 

	
	ImGui::Begin("\uf449 Project Info", &mGlobalSettings.bShowProjectInfo);

	ImGui::Text("%s", TileMap.c_str());
	ImGui::Text("%s", TileSizeX.c_str());
	ImGui::Text("%s", TileSizeY.c_str());

	ImGui::Text("%s", TilesInSet.c_str());
	ImGui::Text("%s", SelectedTile.c_str());

	ImGui::Text("%s", CurrentTile.c_str());
	ImGui::Text("%s", Flip.c_str());
	
	ImGui::End();
		
	return tmpBorder;
}
