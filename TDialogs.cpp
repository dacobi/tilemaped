#include "TSettings.h"
#include "TDialogs.h"
#include "TEditor.h"
#include "imgui_stdlib.h"
#include "ImGuiFileDialog.h"

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

int Dialog::render(){
	if(bUpdateWinPos){
		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		bUpdateWinPos = false;
	} else {
		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
	}
	ImVec2 cWinSize = ImGui::GetWindowSize();
	mDialogWidth = cWinSize.x;
	mDialogHeight = cWinSize.y;
	return 0;
}

int Dialog::render(int ypos){
	ImVec2 cCenter = ImGui::GetMainViewport()->GetCenter();
	cCenter.y = ypos;
	if(bUpdateWinPos){
		ImGui::SetNextWindowPos(cCenter, ImGuiCond_Always, ImVec2(0.5f, 0.0f));
		bUpdateWinPos = false;
	} else {
		ImGui::SetNextWindowPos(cCenter, ImGuiCond_Once, ImVec2(0.5f, 0.0f));
	}
	ImVec2 cWinSize = ImGui::GetWindowSize();
	mDialogWidth = cWinSize.x;
	mDialogHeight = cWinSize.y;
	return 0;
}


int Dialog::render(int xpos, int ypos){
	if(bUpdateWinPos){
		ImGui::SetNextWindowPos(ImVec2(xpos, ypos), ImGuiCond_Always);
		bUpdateWinPos = false;
	} else {
		ImGui::SetNextWindowPos(ImVec2(xpos, ypos), ImGuiCond_Once);
	}
	ImVec2 cWinSize = ImGui::GetWindowSize();
	mDialogWidth = cWinSize.x;
	mDialogHeight = cWinSize.y;
	return 0;
}


void TBDialog::init(){
	mDialogWidth = mGlobalSettings.WindowWidth;
	mDialogHeight =  mGlobalSettings.TopBarHeight;
	mDialogTextActions = mGlobalSettings.mFloppy +" Save/Save As: F12/F11    " + mGlobalSettings.mBook + " Help: F1";	
	
	mDialogTextWindow = mGlobalSettings.mWindow;
	switch(mEditor->mCurMode){
		case EMODE_MAP:
			mDialogTextWindow += " TileMap Editor";
		break;
		case EMODE_TILE:
			mDialogTextWindow += " Tile Editor";
		break;
		case EMODE_PALED:
			mDialogTextWindow += " Palette Editor";
		break;
	};

	mDialogTextProject = mGlobalSettings.mFile + " Project: " + mGlobalSettings.ProjectPath + "  " + mGlobalSettings.mInfo + " Info: F2";
	
}

int TBDialog::render(){
	mDialogWidth = mGlobalSettings.WindowWidth;
		
	mDialogTextWindow = mGlobalSettings.mWindow;
	switch(mEditor->mCurMode){
		case EMODE_MAP:
			mDialogTextWindow += " TileMap Editor";
		break;
		case EMODE_TILE:
			mDialogTextWindow += " Tile Editor";
		break;
		case EMODE_PALED:
			mDialogTextWindow += " Palette Editor";
		break;
	};

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
				//mGlobalSettings.CurrentEditor->mCurMode = EMODE_MAP;
				mGlobalSettings.CurrentEditor->setMode(EMODE_MAP);
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mWindow + " Tile")).c_str())){
				//mGlobalSettings.CurrentEditor->mCurMode = EMODE_TILE;
				mGlobalSettings.CurrentEditor->setMode(EMODE_TILE);
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mWindow + " Palette")).c_str())){
				//mGlobalSettings.CurrentEditor->activatePaletteEdit();
				mGlobalSettings.CurrentEditor->setMode(EMODE_PALED);
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mInfo + " Help Dialog (F1)")).c_str())){
				mGlobalSettings.CurrentEditor->activateHelpDialog();
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mInfo + " Project Info (F2)")).c_str() ,NULL,  &mGlobalSettings.bShowProjectInfo)){
				//mGlobalSettings.CurrentEditor->activateProjectInfo();
			}
			
			
			if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_MAP){
				if(mGlobalSettings.TileSetBPP < 0x8){
					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Palette Offset")).c_str(), NULL, &mGlobalSettings.bShowPaletteOffset) ){ 
					}
				}
				if(ImGui::MenuItem("Brushes (F9)", NULL , &mGlobalSettings.CurrentEditor->bShowBrushesTile)){										
				}
				if(ImGui::MenuItem("Selected Tile Type (S)", NULL , &mGlobalSettings.bShowTypeSelection)){										
				}
				if(ImGui::MenuItem("Selected Tile (T)", NULL , &mGlobalSettings.bShowSelectedTile)){										
				}

				
			}
			if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_TILE){
				if(ImGui::MenuItem("Brushes (F9)", NULL , &mGlobalSettings.CurrentEditor->bShowBrushesPixel)){										
				}
				if(ImGui::MenuItem("Selected Color (S)", NULL , &mGlobalSettings.bShowPixelType)){										
				}
				if(ImGui::MenuItem("Pixel Grid (P)", NULL , &mGlobalSettings.bShowPixelGrip)){										
				}
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_MAP){
				if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Flip Tile (F)")).c_str())){
					mGlobalSettings.CurrentEditor->flipSelectedTile();
				}
				if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " New Tile (F3)")).c_str())){
					mGlobalSettings.CurrentEditor->createNewTile();
				}
				if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Import Tile (F4)")).c_str())){
					mGlobalSettings.CurrentEditor->activateOpenTileDialog();		  			
				}
				if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Copy Tile (F5)")).c_str())){
					mGlobalSettings.CurrentEditor->createNewTileCopy(mGlobalSettings.CurrentEditor->mTileSelectedTile);
				}
				if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Rotate Tile (F6)")).c_str())){
					mGlobalSettings.CurrentEditor->rotateTile();
				}
				if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Remove Unused Tiles (F7)")).c_str())){
					mGlobalSettings.CurrentEditor->activateDropUnusedTiles();
				}
				
			}

			if(mGlobalSettings.CurrentEditor->mCurMode != EMODE_PALED){
				if(ImGui::MenuItem("Select All (A)")){
					mGlobalSettings.CurrentEditor->handleSelection(SELMODE_ALL);
				}
				if(ImGui::MenuItem("Select None (N)")){
					mGlobalSettings.CurrentEditor->handleSelection(SELMODE_NONE);
				}
				if(ImGui::MenuItem("Invert Selection (I)")){
					mGlobalSettings.CurrentEditor->handleSelection(SELMODE_INVERT);
				}
			}

			if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_PALED){
				if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Import Palette")).c_str())){
					mGlobalSettings.CurrentEditor->mPalette.bImportingPalette = true;
				}
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

	std::string cProjPath = mGlobalSettings.mFile + " Project: " + mGlobalSettings.ProjectPath;
	auto textWidthP = ImGui::CalcTextSize(cProjPath.c_str()).x;
	textWidthP += 20;

	if(textWidthP > ((mGlobalSettings.WindowWidth/2)-textWidth)){
		std::string newProjectPath = mGlobalSettings.ProjectPath.substr(mGlobalSettings.ProjectPath.size()/2, mGlobalSettings.ProjectPath.size()-(mGlobalSettings.ProjectPath.size()/2));
		newProjectPath.insert(0, ".");
		newProjectPath.insert(0, ".");
		newProjectPath.insert(0, ".");
		cProjPath = mGlobalSettings.mFile + " Project: " + newProjectPath;
		textWidthP =  ImGui::CalcTextSize(cProjPath.c_str()).x;
		textWidthP +=20;
	}

	ImGui::SetCursorPosX((mGlobalSettings.WindowWidth - textWidthP));
	ImGui::Text("%s", cProjPath.c_str());               

	ImGui::EndMainMenuBar();
	
	
	return 0;
}

void RTDialog::init(){
	mDialogTextMain = mGlobalSettings.mInfo +" Remove Unused Tiles? Undo Stack will be cleared";
	mDialogTextTitle = "Remove Unused Tiles";
	mDialogButtonAccept = "Remove";
	mDialogButtonCancel = "Cancel";
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

void PUDialog::init(){
	mDialogTextMain = mGlobalSettings.mInfo +" Apply Palette Changes? Undo Stack will be cleared";
	mDialogTextTitle = "Apply Palette Changes";
	mDialogButtonAccept = "Apply";
	mDialogButtonCancel = "Cancel";
}

void PUDialog::recieveInput(int mKey){
	
	if(mKey == SDLK_y){
		bInputIsAccept=true;
		mGlobalSettings.mPaletteUpdateState = 1;
	}
	if(mKey == SDLK_n){
		bInputIsCancel=true;
	}
}

void SDialog::init(){
	mDialogTextMain = mGlobalSettings.mFloppy +" Overwrite Project On Disk?";
	mDialogTextTitle = "Save Project";
	mDialogButtonAccept = "Save";
	mDialogButtonCancel = "Cancel";
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

int SDialog::render(){
    
	Dialog::render();

	ImGui::Begin(mDialogTextTitle.c_str(),NULL, ImGuiWindowFlags_AlwaysAutoResize);                         
    		
		ImGui::Text("%s", mDialogTextMain.c_str()); 

        if ( ImGui::Button(mDialogButtonAccept.c_str() )){ 
			recieveInput(SDLK_y);				
		}

		ImGui::SameLine();

		if (ImGui::Button(mDialogButtonCancel.c_str() )){ 
			recieveInput(SDLK_n);				
		}
			            
    ImGui::End();

	return 0;
}

void SADialog::cancel(){
	Dialog::cancel();
	mSubDialog->cancel();
	bSubDialogActive = false;
}

void SADialog::init(){
	mDialogTextMain = mGlobalSettings.mFloppy + " Save Project As Folder         ";
	mDialogTextTitle = "Save Project As";
	mDialogButtonAccept = "Save";
	mDialogButtonCancel = "Cancel";

	mTextInput.bIsInputActive = true;
	mTextInput.bMustNotBeFile = true;
	mTextInput.bAutoComplete = true;
	mTextInput.mDialogTextMain = mGlobalSettings.ProjectPath;
	mTextInput.init();	
}

int SADialog::render(){


	Dialog::render();


	ImGui::Begin(mDialogTextTitle.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize);                         
    		
	ImGui::Text("%s", mDialogTextMain.c_str());

	mTextInput.render();

	if (ImGui::Button("Choose Folder")){
		Dialog::render();
    	ImGui::SetNextWindowSize(ImVec2(800, 600));
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKeySave", "Choose Folder", NULL, ".");
	}

  	// display
  	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKeySave")){
    	// action if OK
    	if (ImGuiFileDialog::Instance()->IsOk()){
      		std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      		std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
	  		mTextInput.mDialogTextMain = filePathName;
			mTextInput.checkCurrentText();
			if(mTextInput.bInputIsAccepted){
				recieveInput(SDLK_y);
			}
      		// action
    	}
    
    	// close
    	ImGuiFileDialog::Instance()->Close();
  	}
  	
    if (ImGui::Button(mDialogButtonAccept.c_str())){
		if(mTextInput.bInputIsAccepted){
			recieveInput(SDLK_y);					
		}				
	}

	ImGui::SameLine();

	if (ImGui::Button(mDialogButtonCancel.c_str())){
		bInputIsCancel=true;
	}
            
    ImGui::End();

	if(bSubDialogActive){
		mSubDialog->bUpdateWinPos = true;
		mSubDialog->render();
		if(mSubDialog->bInputIsAccept){
			recieveInput(SDLK_y);
		}
		if(mSubDialog->bInputIsCancel){
			recieveInput(SDLK_n);
			mSubDialog->bInputIsCancel = false;
		}
	}
	
	return 0;
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
		}
		if(mKey == SDLK_n){
			bInputIsCancel=true;
		}
		if(mKey == SDLK_TAB){		
			mTextInput.autoComplete();			
		}
	}
}

void SADialog::dropLastInputChar(){	
	mTextInput.dropLastInputChar();	
}

int OPDialog::render(){


	Dialog::render();

	ImGui::Begin("Open Project",NULL, ImGuiWindowFlags_AlwaysAutoResize);                         
		
	ImGui::Text("Open Project from Folder");

	mTextInput.render();

	if (ImGui::Button("Choose Project Folder")){
		Dialog::render();
		ImGui::SetNextWindowSize(ImVec2(800, 600));
    	ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKeyPfold", "Choose Folder", NULL, ".");
	}

  	// display
  	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKeyPfold")){
    	// action if OK
    	if (ImGuiFileDialog::Instance()->IsOk()){
      		std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      		std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
	  		mTextInput.mDialogTextMain = filePathName;
			mTextInput.checkCurrentText();
			if(mTextInput.bInputIsAccepted){
				recieveInput(SDLK_y);
			}
      		// action
    	}
    
    	// close
    	ImGuiFileDialog::Instance()->Close();
  	}

    if (ImGui::Button("Open")){
		if(mTextInput.bInputIsAccepted){
			recieveInput(SDLK_y);					
		}				
	}

	ImGui::SameLine();

	if (ImGui::Button("Cancel")){
		bInputIsCancel=true;
	}		
    
    ImGui::End();
	
	return 0;
}

void OPDialog::recieveInput(int mKey){
	if(mKey == SDLK_y){
		if(mTextInput.bInputIsAccepted){
			bInputIsAccept=true;	
			bDialogIsWatingForText = false;

		}		
	}

	if(mKey == SDLK_n){
		bInputIsCancel=true;
	}		
	if(mKey == SDLK_TAB){
		mTextInput.autoComplete();		
	}
}

void OPDialog::init(){
	mDialogTextMain = mGlobalSettings.mFile + " Open Project From Folder";
	
	mTextInput.mDialogTextMain = "";
	mTextInput.mInputLabel = "Folder";
	mTextInput.bIsInputActive = true;
	mTextInput.bAutoComplete = true;
	mTextInput.init();

	bSubDialogActive = false;
	mTextInput.bInputIsAccepted = false;
}


void OCDialog::init(){	
	mOpenProject.init();
	mOpenProject.mTextInput.bMustBeFolder = true;
	mOpenProject.mTextInput.bMustBeProject = true;
	
	mCreateProject.init();
	
}

int OCDialog::render(){
	
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
		mOpenProject.render();
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
		mCreateProject.render();
		if(mCreateProject.bInputIsCancel){
			bSubDialogActive = false;
			bSubDialogIsCreate = false;
			mCreateProject.bInputIsCancel = false;
		}
		if(mCreateProject.bInputIsAccept){
			recieveInput(SDLK_y);
		}
	}

	return 0;
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
				mGlobalSettings.TileSetBPP = mCreateProject.tbpp;
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

int CPDialog::render(){
	
	Dialog::render();


	ImGui::Begin("Create New Project", NULL, ImGuiWindowFlags_AlwaysAutoResize);                         
    		
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

		ImGui::Text("Tile BPP");  
		ImGui::RadioButton("BPP: 8", &tbpp, 8);
		ImGui::SameLine();
		ImGui::RadioButton("BPP: 4", &tbpp, 4);
		ImGui::SameLine();
		ImGui::RadioButton("BPP: 2", &tbpp, 2);


		mReadPath.render();
		if(mReadPath.bIsActive){
			mActiveInput = &mReadPath;
		}

		if (ImGui::Button("New Project Folder")){
			Dialog::render();
			ImGui::SetNextWindowSize(ImVec2(800, 600));
    		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKeyNewf", "Choose Folder", NULL, ".");
		}

  		// display
  		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKeyNewf")){
    		// action if OK
    		if (ImGuiFileDialog::Instance()->IsOk()){
      			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
	  			mReadPath.mDialogTextMain = filePathName;
      			// action
    		}
    
    		// close
    		ImGuiFileDialog::Instance()->Close();
  		}

		ImGui::Checkbox("Use Palette", &bHasPalette);

		if(bHasPalette){
			mReadPal.render();	
			if(mReadPal.bIsActive){
				mActiveInput = &mReadPal;
			}

		if (ImGui::Button("Choose Palette File")){
			Dialog::render();
			ImGui::SetNextWindowSize(ImVec2(800, 600));
    		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKeyNewp", "Choose Palette", ".gpl,.bin", ".");
		}

  		// display
  		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKeyNewp")){
    		// action if OK
    		if (ImGuiFileDialog::Instance()->IsOk()){
      			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
	  			mReadPal.mDialogTextMain = filePathName;
      			// action
    		}
    
    		// close
    		ImGuiFileDialog::Instance()->Close();
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

	return 0;
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
		return;
	}

	if(mKey == SDLK_n){
		bInputIsCancel=true;
	}		
}

int RNDialog::render(int xpos, int ypos){	
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

	return 0;
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


int ITDialog::render(){	
	
	Dialog::render();

	ImGui::Begin("Import Tile", NULL, ImGuiWindowFlags_AlwaysAutoResize);                         
    		
	ImGui::Text("Import Tile from bitmap or RAW");

	mTextInput.render();

	if (ImGui::Button("Choose Tile File")){
		Dialog::render();
		ImGui::SetNextWindowSize(ImVec2(800, 600));
    	ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKeyTile", "Choose File", ".png,.data,.raw,.bin", ".");
	}
  
  	// display
  	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKeyTile")){
    	// action if OK
    	if (ImGuiFileDialog::Instance()->IsOk()){
      		std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      		std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
	  		mTextInput.mDialogTextMain = filePathName;
			mTextInput.checkCurrentText();
			if(mTextInput.bInputIsAccepted){
				recieveInput(SDLK_y);
			}
      		// action
    	}    
    // close
    	ImGuiFileDialog::Instance()->Close();
  	}

    if (ImGui::Button("Import")){
		if(mTextInput.bInputIsAccepted){
			recieveInput(SDLK_y);					
		}				
	}

	ImGui::SameLine();

	if (ImGui::Button("Cancel")){
		bInputIsCancel=true;
	}		
	        
    ImGui::End();
	
	return 0;
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
		}		
	}

	if(mKey == SDLK_n){
		bInputIsCancel=true;
	}		
	if(mKey == SDLK_TAB){
		mTextInput.autoComplete();		
	}
}


void TIDialog::init(){
	
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

int TIDialog::render(){
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

	return 0;
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
			mDir = fs::current_path().string();
		} else {
			mDir = cPath.parent_path().string();
		}

		mCompleteText = "";
		mFile = cPath.filename().string();			 

		if(fs::is_directory(fs::status(mDir))){
			for (const auto & entry : fs::directory_iterator(mDir)){
				std::string tStr =   (entry.path()).filename().string(); 
				if(mFile.length()){
					std::size_t subpos = tStr.find(mFile);						
					if((subpos != std::string::npos) && (subpos == 0)){												
						mCompleteText = tStr.substr(mFile.length());						
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
		if(bMustBeProject){
			int isProj = mGlobalSettings.testProjectFolder(mDialogTextMain);
			if(!cIsFolder || !isProj){						
				bInputIsAccepted=false;
				return 1;				
			}
		}
	}

	bInputIsAccepted=true;	
	return 0;
}

void HDialog::init(){
	mHelpTextMap = mGlobalSettings.mHelpTextMap;
	mHelpTextTile = mGlobalSettings.mHelpTextTile;
	mHelpTextGeneral = mGlobalSettings.mHelpText;
	mHelpTextPalette = mGlobalSettings.mHelpTextPalette;
}

void HDialog::recieveInput(int mKey){
	bInputIsCancel=true;
}


int HDialog::render(){

	Dialog::render();

	ImGui::Begin("Help", &mGlobalSettings.bShowHelpDialog);

	ImGui::SetWindowSize(ImVec2(950, 900), ImGuiCond_Once);

    ImGui::Separator();

	
	

	if(ImGui::CollapsingHeader("General")){
		ImGui::PushFont(mGlobalSettings.SFont);
		for(int i = 0; i < mHelpTextGeneral.size(); i++){
			ImGui::BulletText("%s", mHelpTextGeneral[i].c_str());
		}
		ImGui::PopFont();
	}

	ImGui::Separator();


	if(ImGui::CollapsingHeader("TileMap")){
		ImGui::PushFont(mGlobalSettings.SFont);
		for(int i = 0; i < mHelpTextMap.size(); i++){
			ImGui::BulletText("%s", mHelpTextMap[i].c_str());
		}
		ImGui::PopFont();
	}

	ImGui::Separator();

	if(ImGui::CollapsingHeader("Tile")){
		ImGui::PushFont(mGlobalSettings.SFont);
		for(int i = 0; i < mHelpTextTile.size(); i++){
			ImGui::BulletText("%s", mHelpTextTile[i].c_str());
		}
		ImGui::PopFont();
	}

	ImGui::Separator();


	if(ImGui::CollapsingHeader("Palette")){
		ImGui::PushFont(mGlobalSettings.SFont);
		for(int i = 0; i < mHelpTextPalette.size(); i++){
			ImGui::BulletText("%s", mHelpTextPalette[i].c_str());
		}
		ImGui::PopFont();
	}

	ImGui::Separator();


	 ImGuiStyle& style = ImGui::GetStyle();

    float size = ImGui::CalcTextSize("   Close   ").x + style.FramePadding.x * 2.0f;
    float avail = ImGui::GetContentRegionAvail().x;

    float off = (avail - size) * 0.5f;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
	

	if(ImGui::Button("   Close   ")){
		mGlobalSettings.bShowHelpDialog = false;
		recieveInput(SDLK_n);
	}

	if(!mGlobalSettings.bShowHelpDialog){
		recieveInput(SDLK_n);
	}

	ImGui::End();

	return 0;
}

void MEDialog::init(){
	mDialogTextMain = mGlobalSettings.mInfo + " ";
}
void MEDialog::setColorScheme(int nScheme){
	if(nScheme == 1){
		//mDialogBorderColor = mGlobalSettings.ErrorBorderColor;
		mTextColor = mGlobalSettings.ErrorTextColor;		
	} else {
		//mDialogBorderColor = mGlobalSettings.DefaultBorderColor;
		mTextColor = mGlobalSettings.DefaultTextColor;		
	}
}


void MEDialog::update(){

}


int MEDialog::render(){

	Dialog::render();

	ImGui::Begin("Message");                         
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(mTextColor.r, mTextColor.g, mTextColor.b, 255));
		ImGui::Text("%s", mDialogTextMain.c_str());
		ImGui::PopStyleColor();
		    
		if (ImGui::Button("Close")){
			bInputIsCancel=true;
		}
        
    ImGui::End();

	return 0;
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
	
	mDialogTextMain = mGlobalSettings.mInfo + " Project Info";

		
}

int PIDialog::render(int xpos, int ypos){

	Dialog::render(xpos, ypos);

	ImGui::PushFont(mGlobalSettings.SFont);

	ImGui::Begin(mDialogTextMain.c_str(), &mGlobalSettings.bShowProjectInfo, ImGuiWindowFlags_AlwaysAutoResize);
	
	ImGui::Text("%s", TileMap.c_str());
	ImGui::Text("%s", TileSizeX.c_str());
	ImGui::Text("%s", TileSizeY.c_str());

	ImGui::Text("%s", TilesInSet.c_str());
	ImGui::Text("%s", SelectedTile.c_str());

	ImGui::Text("%s", CurrentTile.c_str());
	ImGui::Text("%s", Flip.c_str());
	
	ImGui::End();

	ImGui::PopFont();

	return 0;
}

void PODialog::init(){
	mDialogTextMain = mGlobalSettings.mImage + " Palette Offset";
}

int PODialog::render(int xpos, int ypos){

	Dialog::render(ypos);

	ImGui::PushFont(mGlobalSettings.SFont);


	ImGui::Begin(mDialogTextMain.c_str(), &mGlobalSettings.bShowPaletteOffset);

	ImGui::RadioButton("0", &mGlobalSettings.PaletteOffset, 0);
	ImGui::SameLine();
	ImGui::RadioButton("1", &mGlobalSettings.PaletteOffset, 1);
	ImGui::SameLine();
	ImGui::RadioButton("2", &mGlobalSettings.PaletteOffset, 2);
	ImGui::SameLine();
	ImGui::RadioButton("3", &mGlobalSettings.PaletteOffset, 3);
	ImGui::SameLine();
	ImGui::RadioButton("4", &mGlobalSettings.PaletteOffset, 4);
	ImGui::SameLine();
	ImGui::RadioButton("5", &mGlobalSettings.PaletteOffset, 5);
	ImGui::SameLine();
	ImGui::RadioButton("6", &mGlobalSettings.PaletteOffset, 6);
	ImGui::SameLine();
	ImGui::RadioButton("7", &mGlobalSettings.PaletteOffset, 7);
	ImGui::SameLine();
	ImGui::RadioButton("8", &mGlobalSettings.PaletteOffset, 8);
	ImGui::SameLine();
	ImGui::RadioButton("9", &mGlobalSettings.PaletteOffset, 9);
	ImGui::SameLine();
	ImGui::RadioButton("10", &mGlobalSettings.PaletteOffset, 10);
	ImGui::SameLine();
	ImGui::RadioButton("11", &mGlobalSettings.PaletteOffset, 11);
	ImGui::SameLine();
	ImGui::RadioButton("12", &mGlobalSettings.PaletteOffset, 12);
	ImGui::SameLine();
	ImGui::RadioButton("13", &mGlobalSettings.PaletteOffset, 13);
	ImGui::SameLine();
	ImGui::RadioButton("14", &mGlobalSettings.PaletteOffset, 14);
	ImGui::SameLine();
	ImGui::RadioButton("15", &mGlobalSettings.PaletteOffset, 15);
	
	ImGui::PopFont();

	ImGui::End();
	return 0;
}

void QDialog::recieveInput(int mKey){
	
	if(mKey == SDLK_y){
		std::cout << "Shutting Down..." << std::endl;
		bInputIsAccept=true;
		mGlobalSettings.CurrentEditor->bEditorRunning = false;
	}
	if(mKey == SDLK_n){
		bInputIsCancel=true;
	}
}

int QDialog::render(){
    
	Dialog::render();

	ImGui::Begin("Quit Program?", NULL, ImGuiWindowFlags_AlwaysAutoResize);                         
    		
			ImGui::Text("Any unsaved progress will be lost   "); 

            if (ImGui::Button("Quit")){
				recieveInput(SDLK_y);
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel")){
				recieveInput(SDLK_n);
			}
            
        ImGui::End();
	
	return 0;
}