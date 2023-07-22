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
		ImVec2 cNewPos = ImGui::GetMainViewport()->GetCenter();
		cNewPos.x /= mGlobalSettings.mUIScale;
		cNewPos.y /= mGlobalSettings.mUIScale;
		ImGui::SetNextWindowPos(cNewPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		bUpdateWinPos = false;		
	} else {
		ImVec2 cNewPos = ImGui::GetMainViewport()->GetCenter();
		cNewPos.x /= mGlobalSettings.mUIScale;
		cNewPos.y /= mGlobalSettings.mUIScale;
		ImGui::SetNextWindowPos(cNewPos, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
		//ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
	}
	ImVec2 cWinSize = ImGui::GetWindowSize();
	mDialogWidth = cWinSize.x;
	mDialogHeight = cWinSize.y;
	return 0;
}

int Dialog::render(int ypos){
	ImVec2 cCenter = ImGui::GetMainViewport()->GetCenter();
	cCenter.x /= mGlobalSettings.mUIScale;
	cCenter.y /= mGlobalSettings.mUIScale;
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
		case EMODE_TILESET:
			mDialogTextWindow += " TileSet Editor";
		break;
		case EMODE_SELEDIT:
			mDialogTextWindow += " Selection Editor";
		break;
		case EMODE_SPRITE:
			mDialogTextWindow += " Sprite Editor";
		break;
	};

	bIsMenuOpen = false;

	ImGui::BeginMainMenuBar();

		if (ImGui::BeginMenu("File"))
		{
			bIsMenuOpen = true;

			if(ImGui::MenuItem((std::string(mGlobalSettings.mFloppy + " Save (F12)")).c_str())){
				mEditor->activateSaveDialog();
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mFloppy + " Save As (F11)")).c_str())){				
				mEditor->activateDTDialog(EDIALOG_PROJECTSAVEAS);
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Open")).c_str())){
				mEditor->activateDTDialog(EDIALOG_PROJECTCLOSE, ESTATE_PROJECTOPEN);
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Create")).c_str())){
				mEditor->activateDTDialog(EDIALOG_PROJECTCLOSE, ESTATE_PROJECTCREATE);				
			}


			if(mEditor->mCurMode == EMODE_MAP){
				if(ImGui::BeginMenu((std::string(mGlobalSettings.mFile + " Import")).c_str())){
					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Import Tile")).c_str())){
						mEditor->activateDTDialog(EDIALOG_TILEIMPORT);
					}
					if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Import TileSet")).c_str())){
						mEditor->activateDTDialog(EDIALOG_TILESETIMPORT);		  			
					}
					if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Import TileMap")).c_str())){
						//mEditor->activateOpenTileMapDialog();	  			
						int cTileMapCond = -1;
						if(mGlobalSettings.mGlobalTexParam.TexBPP < 0x8){cTileMapCond = 8;}
						mEditor->activateDTDialog(EDIALOG_TILEMAPIMPORT, cTileMapCond, 0, mEditor->mTileSet.TTiles.size()-1);

					}
					if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Import Sprite")).c_str())){
						mEditor->activateDTDialog(EDIALOG_SPRITEIMPORT);
					}

					ImGui::EndMenu();
				}
			}
			if(mEditor->mCurMode == EMODE_SPRITE){
				if(ImGui::BeginMenu((std::string(mGlobalSettings.mFile + " Import")).c_str())){

					if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Import Sprite")).c_str())){
						mEditor->activateDTDialog(EDIALOG_SPRITEIMPORT);
					}

					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Import Sprite Frame")).c_str())){
						//mEditor->activateOpenFrameDialog();		  			
						mEditor->activateDTDialog(EDIALOG_SPRITEFRAMEIMPORT);
					}

					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Import Sprite Frame(s)")).c_str())){
						//mEditor->activateOpenFramesDialog();		  			
						mEditor->activateDTDialog(EDIALOG_SPRITEFRAMESIMPORT);
					}

					ImGui::EndMenu();
				}
			}

			ImGui::Separator();

			if(ImGui::BeginMenu((std::string(mGlobalSettings.mGear + " Project").c_str()))){

				if(ImGui::BeginMenu((std::string(mGlobalSettings.mGear + " Selection Mode").c_str()))){
					bool bIsAppend = mGlobalSettings.bSelectionMode;
					bool bIsReplace = !mGlobalSettings.bSelectionMode;

					if(ImGui::MenuItem("Replace", NULL, &bIsReplace)){
						mGlobalSettings.bSelectionMode = !bIsReplace;						
					}

					if(ImGui::MenuItem("Append", NULL, &bIsAppend)){
						mGlobalSettings.bSelectionMode = bIsAppend;						
					}
					
					ImGui::EndMenu();
				}

				if(ImGui::BeginMenu((std::string(mGlobalSettings.mFile + " Clipboards").c_str()))){

					ImGui::Checkbox("Save To Project", &mGlobalSettings.mProjectSettings.Clipboard_SaveToProject->bvalue);

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			mGlobalSettings.settingsMenu();

			ImGui::Separator();

			if(ImGui::MenuItem((std::string(mGlobalSettings.mExit + " Quit")).c_str())){
				mEditor->activateDTDialog(EDIALOG_PROGRAMQUIT);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			bIsMenuOpen = true;

			if(mEditor->mCurMode == EMODE_MAP){
				
					if(ImGui::BeginMenu((std::string(mGlobalSettings.mWindow + " Tilemaps")).c_str())){
						int nMap=0;
						for(auto *cMap : mEditor->mTileMaps){
							if(ImGui::MenuItem(cMap->getMapSize().c_str())){				
								mEditor->switchTileMap(nMap);
							}
							nMap++;
						}
 
						if(ImGui::MenuItem((std::string(mGlobalSettings.mFile+ " Create Tilemap")).c_str())){				
							mEditor->activateDTDialog(EDIALOG_TILEMAPCREATE, mGlobalSettings.mGlobalTexParam.TexBPP < 8 ? 1 : -1, 0, mEditor->mTileSet.TTiles.size()-1);
						}		

						if(mEditor->mTileMaps.size() > 1){
							if(ImGui::MenuItem((std::string(mGlobalSettings.mFile+ " Remove Tilemap")).c_str())){				
								mEditor->activateDTDialog(EDIALOG_TILEMAPDELETE);
							}	
						}
						
						ImGui::EndMenu();
					}				
			} else {
				if(ImGui::MenuItem((std::string(mGlobalSettings.mWindow + " Tilemap")).c_str())){				
					mEditor->setMode(EMODE_MAP);
				}
			}

			
			if(ImGui::MenuItem((std::string(mGlobalSettings.mWindow + " Tile")).c_str())){				
				mEditor->setMode(EMODE_TILE);
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mWindow + " Palette")).c_str())){				
				mEditor->setMode(EMODE_PALED);
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mWindow + " TileSet")).c_str())){				
				mEditor->setMode(EMODE_TILESET);
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mWindow + " Selection")).c_str())){				
				mEditor->setMode(EMODE_SELEDIT);
			}

			if(mEditor->mCurMode == EMODE_SPRITE){
				
					if(ImGui::BeginMenu((std::string(mGlobalSettings.mWindow + " Sprites")).c_str())){
						int nSprite=0;
						std::stringstream fconv;
						std::string cSFrame;
						std::string snSprite;
						std::string cSPriteStart;
						bool bCFrame;
						bool bCSprite;												
						for(auto *cSprite : mEditor->mSprites){							
							fconv << nSprite << std::endl;									
							fconv >> snSprite;
							bCSprite = (cSprite == mEditor->mSprite);

							if(bCSprite){
								cSPriteStart = mGlobalSettings.mBall + " Sprite ";
							} else {
								cSPriteStart = "  Sprite ";
							}

							if(ImGui::BeginMenu(std::string(cSPriteStart + snSprite + ": "+ cSprite->getSpriteSize()).c_str())){
								if(ImGui::IsItemClicked()){
									mEditor->switchSprite(nSprite);								
								}
								std::string cPreFrame;
								
								for(int nf = 0; nf < cSprite->mFrames.size(); nf++){
									fconv << nf << std::endl;									
									fconv >> cSFrame;
									bCFrame = (nf == cSprite->mSelectedFrame);

									if(bCFrame){
										cPreFrame = mGlobalSettings.mBall + " Frame ";
									} else {
										cPreFrame = "  Frame ";
									}		

									if(ImGui::MenuItem((std::string(cPreFrame + cSFrame).c_str()))){																					
										mEditor->switchSprite(nSprite, nf);								
									}								
								}								
								ImGui::EndMenu();
							}
							nSprite++;
						}
 
						if(ImGui::MenuItem((std::string(mGlobalSettings.mFile+ " Create Sprite")).c_str())){				
							mEditor->activateDTDialog(EDIALOG_SPRITECREATE);
						}		

						
						if(ImGui::MenuItem((std::string(mGlobalSettings.mFile+ " Remove Sprite")).c_str())){				
							mEditor->activateDTDialog(EDIALOG_SPRITEDELETE);
						}	

						
						
						ImGui::EndMenu();
					}

					if(ImGui::MenuItem((std::string(mGlobalSettings.mWindow + " Sprite Frames")).c_str(), NULL, &mEditor->mSprite->bShowSpriteFrames)){
						
					}				
			} else {
				if(ImGui::MenuItem((std::string(mGlobalSettings.mWindow + " Sprite")).c_str())){				
					if(mEditor->mSprites.size()){
						mEditor->setMode(EMODE_SPRITE);
					} else {
						mEditor->activateDTDialog(EDIALOG_SPRITECREATE);
					}
					
					
				}
			}

			if(ImGui::MenuItem((std::string(mGlobalSettings.mInfo + " Help Dialog (F1)")).c_str())){								
				mEditor->activateDTDialog(EDIALOG_HELPMENU);
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mInfo + " Project Info (F2)")).c_str() ,NULL,  &mGlobalSettings.bShowProjectInfo)){
				
			}
			
			
			if(mEditor->mCurMode == EMODE_MAP){
				if(mGlobalSettings.mGlobalTexParam.TexBPP < 0x8){
					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Palette Offset")).c_str(), NULL, &mGlobalSettings.bShowPaletteOffset) ){ 
					}
				}
				if(ImGui::MenuItem(std::string(mGlobalSettings.mClip + " Clipboard (F7)").c_str(), NULL , &mEditor->bShowClipboardMap)){		
					mEditor->activateClipboard(false);
				}
				if(ImGui::MenuItem(std::string(mGlobalSettings.mBrush + " Brushes (F8)").c_str(), NULL , &mEditor->bShowBrushesTile)){										
					mEditor->activateBrushes(false);
				}
				if(ImGui::MenuItem(std::string(mGlobalSettings.mBrush + " Next Brush (F9)").c_str())){     
					mEditor->activateBrush();
                }
				if(ImGui::MenuItem(std::string(mGlobalSettings.mBrush + " Drop Brush (F10) (ESC)").c_str())){     
					mEditor->dropBrush();
                }
				if(ImGui::MenuItem(std::string(mGlobalSettings.mPoint + " Next Brush Pos (B)").c_str())){     
					mEditor->nextBrushPos();
                }
				if(ImGui::MenuItem("Selected Tile Type (S)", NULL , &mGlobalSettings.bShowTypeSelection)){										
				}
				if(ImGui::MenuItem("Selected Tile (T)", NULL , &mGlobalSettings.bShowSelectedTile)){										
				}

				
			}
			if(mEditor->mCurMode == EMODE_TILE){
				if(ImGui::MenuItem(std::string(mGlobalSettings.mClip + " Clipboard (F7)").c_str(), NULL , &mEditor->mTileSet.bShowClipboardTiles)){		
					mEditor->activateClipboard(false);
				}
				if(ImGui::MenuItem(std::string(mGlobalSettings.mBrush + " Brushes (F8)").c_str(), NULL , &mEditor->bShowBrushesPixel)){										
					mEditor->activateBrushes(false);
				}
				if(ImGui::MenuItem(std::string(mGlobalSettings.mBrush + " Next Brush (F9)").c_str())){     
					mEditor->activateBrush();
                }
				if(ImGui::MenuItem(std::string(mGlobalSettings.mBrush + " Drop Brush (F10) (ESC)").c_str())){     
					mEditor->dropBrush();
                }
				if(ImGui::MenuItem(std::string(mGlobalSettings.mPoint + " Next Brush Pos (B)").c_str())){     
					mEditor->nextBrushPos();
                }
				if(ImGui::MenuItem("Selected Color (S)", NULL , &mGlobalSettings.bShowPixelType)){										
				}
				if(ImGui::MenuItem("Pixel Grid (P)", NULL , &mGlobalSettings.bShowPixelGrid)){										
				}
			}
			if(mEditor->mCurMode == EMODE_SPRITE){
				if(ImGui::MenuItem(std::string(mGlobalSettings.mClip + " Clipboard (F7)").c_str(), NULL , &mEditor->mSprite->bShowClipboard)){
					mEditor->activateClipboard(false);									
				}
				if(ImGui::MenuItem(std::string(mGlobalSettings.mBrush + " Brushes (F8)").c_str(), NULL , &mEditor->mSprite->bShowBrushesPixel)){										
					mEditor->activateBrushes(false);
				}
				if(ImGui::MenuItem(std::string(mGlobalSettings.mBrush + " Next Brush (F9)").c_str())){     
					mEditor->activateBrush();
                }
				if(ImGui::MenuItem(std::string(mGlobalSettings.mBrush + " Drop Brush (F10) (ESC)").c_str())){     
					mEditor->dropBrush();
                }
				if(ImGui::MenuItem(std::string(mGlobalSettings.mPoint + " Next Brush Pos (B)").c_str())){     
					mEditor->nextBrushPos();
                }				
				if(ImGui::MenuItem("Selected Color (S)", NULL , &mGlobalSettings.bShowPixelTypeSprite)){										
				}
				if(ImGui::MenuItem("Pixel Grid (P)", NULL , &mGlobalSettings.bShowPixelGridSprite)){										
				}
			}

			if(mEditor->mCurMode == EMODE_TILESET){
				if(ImGui::MenuItem(std::string(mGlobalSettings.mClip + " Clipboard (F7)").c_str(), NULL , &mEditor->mTileSet.bShowClipboardTileSet)){		
					mEditor->activateClipboard(false);
				}
				if(ImGui::MenuItem(std::string(mGlobalSettings.mBrush + " Brushes (F8)").c_str(), NULL , &mEditor->bShowBrushesPixelTileSet)){                                                                         
					mEditor->activateBrushes(false);
                }
				if(ImGui::MenuItem(std::string(mGlobalSettings.mBrush + " Next Brush (F9)").c_str())){     
					mEditor->activateBrush();
                }
				if(ImGui::MenuItem(std::string(mGlobalSettings.mBrush + " Drop Brush (F10) (ESC)").c_str())){     
					mEditor->dropBrush();
                }
				if(ImGui::MenuItem(std::string(mGlobalSettings.mPoint + " Next Brush Pos (B)").c_str())){     
					mEditor->nextBrushPos();
                }			
				if(ImGui::MenuItem("Tile Grid (T)", NULL , &mGlobalSettings.bShowTileGrid)){										
				}
				if(ImGui::MenuItem("Pixel Grid (P)", NULL , &mGlobalSettings.bShowTilePixelGrid)){										
				}
				if(ImGui::MenuItem("Selected Color (S)", NULL , &mGlobalSettings.bShowPixelType)){										
				}
			}

			if(mEditor->mCurMode == EMODE_SELEDIT){	
				if(ImGui::MenuItem(std::string(mGlobalSettings.mBrush + " Brushes (F8)").c_str(), NULL , &mEditor->bShowBrushesPixelSelEdit)){     
					mEditor->activateBrushes(false);
                }
				if(ImGui::MenuItem(std::string(mGlobalSettings.mBrush + " Next Brush (F9)").c_str())){     
					mEditor->activateBrush();
                }
				if(ImGui::MenuItem(std::string(mGlobalSettings.mBrush + " Drop Brush (F10) (ESC)").c_str())){     
					mEditor->dropBrush();
                }
				if(ImGui::MenuItem(std::string(mGlobalSettings.mPoint + " Next Brush Pos (B)").c_str())){     
					mEditor->nextBrushPos();
                }
				if(ImGui::MenuItem("Tile Grid (T)", NULL , &mGlobalSettings.bShowTileSelGrid)){										
				}
				if(ImGui::MenuItem("Pixel Grid (P)", NULL , &mGlobalSettings.bShowTilePixelSelGrid)){										
				}
				if(ImGui::MenuItem("Selected Color (S)", NULL , &mGlobalSettings.bShowPixelType)){										
				}
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			bIsMenuOpen = true;

			if(mEditor->mCurMode == EMODE_SPRITE){				
				if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " New Sprite Frame (F3)")).c_str())){
					mEditor->createNewFrame();
				}
				if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Copy Sprite Frame (F4)")).c_str())){
					mEditor->createNewFrameCopy(mEditor->mSprite->mFrame);
				}
				if(mEditor->mSprite->mTexParam.TexSizeX == mEditor->mSprite->mTexParam.TexSizeY){					
					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Rotate Frame Left (F5)")).c_str())){
						mEditor->rotateFrameLeft();
					}
					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Rotate Frame Right (F6)")).c_str())){
						mEditor->rotateFrameRight();
					}
					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Rotate Selected Frame")).c_str())){
						mEditor->activateDTDialog(EDIALOG_FRAMEROTATE);
					}
					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Scale Selected Frame")).c_str())){
						mEditor->activateDTDialog(EDIALOG_FRAMESCALE);
					}
				}				
				if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Remove Selected Frame (DEL)")).c_str())){
					if(mEditor->mSprite->mFrames.size() > 1){
						mEditor->activateRemoveFrame();
					} else {
						mEditor->activateDTDialog(EDIALOG_SPRITEDELETE);
					}
				}
				

				if(mGlobalSettings.mUseTextureFiltering > 1){
					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Filter Selected Frame")).c_str())){
						mEditor->filterFrame();
					}
				}

				/*  */

				if(ImGui::BeginMenu(std::string(mGlobalSettings.mTool + " Sprite Tools").c_str())){
													
					bool bFrameCopyPosible = false;

					for(auto *cSprt : mEditor->mSprites){
						if(cSprt != mEditor->mSprite){
							if((cSprt->mTexParam.TexSizeX == mEditor->mSprite->mTexParam.TexSizeX) && (cSprt->mTexParam.TexSizeY == mEditor->mSprite->mTexParam.TexSizeY) && (cSprt->mTexParam.TexBPP == mEditor->mSprite->mTexParam.TexBPP)){
								bFrameCopyPosible = true;
							}
						}
					}

					if(ImGui::BeginMenu((std::string(mGlobalSettings.mImage + " Copy Frame")).c_str(), bFrameCopyPosible)){
						int fSCount = 0;
						std::string sfSCount;
						std::string cCSFrame;
						std::stringstream fconv;
						
						for(auto *cSprt : mEditor->mSprites){
							if(cSprt != mEditor->mSprite){
								if((cSprt->mTexParam.TexSizeX == mEditor->mSprite->mTexParam.TexSizeX) && (cSprt->mTexParam.TexSizeY == mEditor->mSprite->mTexParam.TexSizeY) && (cSprt->mTexParam.TexBPP == mEditor->mSprite->mTexParam.TexBPP)){
											
									fconv << fSCount << std::endl;
									fconv >> sfSCount;

									if(ImGui::BeginMenu(std::string("Sprite " + sfSCount + ": " +  cSprt->getSpriteSize()).c_str())){

										for(int nf = 0; nf < cSprt->mFrames.size(); nf++){
											fconv << nf << std::endl;									
											fconv >> cCSFrame;
																								
											if(ImGui::MenuItem((std::string("Frame " + cCSFrame).c_str()))){																					
												mEditor->createNewFrameCopy(cSprt->mFrames[nf]);
											}								
										}	
										
										ImGui::EndMenu();
									}											
								}	
							}
							
							fSCount++;
						}
						
						ImGui::EndMenu();		
					}

							

					if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Copy")).c_str())){
						mEditor->activateDTDialog(EDIALOG_SPRITECREATECOPY);										
					}

					if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Scaled Copy")).c_str())){										
						mEditor->activateDTDialog(EDIALOG_SPRITECREATESCALEDCOPY);
					}

					bool bAllowUpscale = false;
					int cAllowedScale = 2;

					if( (mEditor->mSprite->mTexParam.TexSizeX < 64) && (mEditor->mSprite->mTexParam.TexSizeY < 64) ){
						bAllowUpscale = true;

						if( (mEditor->mSprite->mTexParam.TexSizeX < 32) && (mEditor->mSprite->mTexParam.TexSizeY < 32) ){
							cAllowedScale = 4;
						}

						if( (mEditor->mSprite->mTexParam.TexSizeX < 16) && (mEditor->mSprite->mTexParam.TexSizeY < 16) ){
							cAllowedScale = 8;
						}																		
					}

					if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Upscaled Copy")).c_str(), NULL, false, bAllowUpscale)){									
						mEditor->activateDTDialog(EDIALOG_SPRITECREATEUPSCALEDCOPY, cAllowedScale);
					}


					bool bAllowDownscale = false;
					int cAllowedDownScale = 2;

					if( (mEditor->mSprite->mTexParam.TexSizeX > 8) && (mEditor->mSprite->mTexParam.TexSizeY > 8) ){
						bAllowDownscale = true;

						if( (mEditor->mSprite->mTexParam.TexSizeX > 16) && (mEditor->mSprite->mTexParam.TexSizeY > 16) ){
							cAllowedDownScale = 4;
						}

						if( (mEditor->mSprite->mTexParam.TexSizeX > 32) && (mEditor->mSprite->mTexParam.TexSizeY > 32) ){
							cAllowedDownScale = 8;
						}																		
					}

					if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Downscaled Copy")).c_str(), NULL, false, bAllowDownscale)){									
						mEditor->activateDTDialog(EDIALOG_SPRITECREATEDOWNSCALEDCOPY, cAllowedDownScale);
					}

					bool bAllowRange = false;

					if( mEditor->mSprite->mTexParam.TexSizeX == mEditor->mSprite->mTexParam.TexSizeY){
						bAllowRange = true;
					}

					if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Convert BPP")).c_str())){										
						mEditor->activateDTDialog(EDIALOG_SPRITECONVERTBPP, mEditor->mSprite->mTexParam.TexBPP);
					}

					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Rotation Range")).c_str(), NULL, false, bAllowRange)){									
						mEditor->activateDTDialog(EDIALOG_SPRITECREATEROTATIONRANGE);
					}

					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Frame Rotations")).c_str(), NULL, false, bAllowRange)){									
						mEditor->activateDTDialog(EDIALOG_SPRITECREATEFRAMEROTATIONS);
					}
															
					ImGui::EndMenu();		
				}

				if(ImGui::BeginMenu((std::string(mGlobalSettings.mGear + " Sprite Frame Reordering").c_str()))){										
					if(ImGui::MenuItem("Warn Before Frame Removal", NULL, &mGlobalSettings.bSpriteWarnBeforeDelete)){
						
					}
					ImGui::EndMenu();
				}
				if(ImGui::BeginMenu((std::string(mGlobalSettings.mGear + " Sprite Edit").c_str()))){					
					if(ImGui::SliderInt("Frame Scale", &mEditor->mSprite->mTexParam.TexPixelSize, TSprite::MinScale, TSprite::MaxScale,"%d", ImGuiSliderFlags_NoInput)){						
						mEditor->setSpriteBrushes();						
					}
					if(ImGui::SliderInt("Max Grid Width", &mEditor->mSprite->mMaxColumns, TSprite::MinCol, TSprite::MaxCol,"%d", ImGuiSliderFlags_NoInput)){
						mEditor->mSprite->resizeScale();						
					}
					if(ImGui::SliderInt("Min Frame Scale", &mEditor->mSprite->mMinFrameScale, TSprite::MinFrame, mEditor->mSprite->MaxFrame,"%d", ImGuiSliderFlags_NoInput)){
						mEditor->mSprite->resizeScale();						
					}
											
					ImGui::EndMenu();
				}
			
				if(ImGui::BeginMenu((std::string(mGlobalSettings.mGear + " Texture Filtering")).c_str())){
					ImGui::RadioButton("None", &mGlobalSettings.mUseTextureFiltering, 0);
					ImGui::RadioButton("Some", &mGlobalSettings.mUseTextureFiltering, 1);
					ImGui::RadioButton("All", &mGlobalSettings.mUseTextureFiltering, 2);

					ImGui::EndMenu();

				}		
			}

			if(mEditor->mCurMode == EMODE_MAP){
				if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Flip Tile (F) (X) (Y)")).c_str())){
					mEditor->flipSelectedTile(0);
				}
				if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " New Tile (F3)")).c_str())){
					mEditor->createNewTile();
				}				
				if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Copy Tile (F4)")).c_str())){
					mEditor->createNewTileCopy(mEditor->mTileSelectedTile);
				}

				if(mGlobalSettings.mGlobalTexParam.TexSizeX == mGlobalSettings.mGlobalTexParam.TexSizeY){					
					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Rotate Tile Left (F5)")).c_str())){
						mEditor->rotateTileLeft();
					}
					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Rotate Tile Right (F6)")).c_str())){
						mEditor->rotateTile();
					}
				}

				if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Remove Selected Tile (DEL)")).c_str())){
					mEditor->activateDropUnusedTile();
				}

				if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Remove Unused Tiles")).c_str())){
					mEditor->activateDTDialog(EDIALOG_TILEDELETEALL);
				}
											
				if(ImGui::BeginMenu((std::string(mGlobalSettings.mFile + " CollisionMap")).c_str())){
				
					if(mEditor->mTileMap->bHasCollisionMap){
						if(ImGui::MenuItem((std::string(mGlobalSettings.mFile+ " Edit")).c_str())){				
							mEditor->activateColMapDialog();
						}
						
						if(ImGui::MenuItem((std::string(mGlobalSettings.mFile+ " Remove")).c_str())){											
							mEditor->activateDTDialog(EDIALOG_COLMAPREMOVE);
						}
					} else {
						if(ImGui::MenuItem((std::string(mGlobalSettings.mFile+ " Create")).c_str())){				
							mEditor->activateColMapDialog(true);
						}
					}

					ImGui::EndMenu();
				}

				if(ImGui::BeginMenu((std::string(mGlobalSettings.mGear + " TileSet Edit").c_str()))){
					if(ImGui::SliderInt("Max Grid Width", &mEditor->mTileSet.mMaxColumns, TileSet::MinCol, TileSet::MaxCol,"%d", ImGuiSliderFlags_NoInput)){
						mEditor->mTileSet.resizeScale();						
					}
					if(ImGui::SliderInt("Min Tile Scale", &mEditor->mTileSet.mMinTileScale, TileSet::MinTile, TileSet::MaxTile, "%d", ImGuiSliderFlags_NoInput)){
						mEditor->mTileSet.resizeScale();						
					}
					ImGui::EndMenu();
				}

				if(ImGui::BeginMenu((std::string(mGlobalSettings.mGear + " TileSet Reordering").c_str()))){
					
					if(ImGui::MenuItem("Update TileMap(s)", NULL, &mGlobalSettings.bTileSetOrderUpdateTileMap)){
						
					}
					if(ImGui::MenuItem("Warn before Tile Removal", NULL, &mGlobalSettings.bTileSetWarnBeforeDelete)){
						
					}
					ImGui::EndMenu();
				}

				if(ImGui::BeginMenu((std::string(mGlobalSettings.mGear + " TileMap Clipboard").c_str()))){
					
					if(ImGui::MenuItem("Tile Zero Is Transparent", NULL, &mEditor->mTileMap->bIsTileZeroTransparent)){
						
					}
					
					ImGui::EndMenu();
				}								
			}

			if(mEditor->mCurMode == EMODE_TILESET){
				if(ImGui::BeginMenu((std::string(mGlobalSettings.mGear + " TileSet Edit").c_str()))){
					if(ImGui::SliderInt("Grid Width", &mGlobalSettings.mTileSetEditWidth, TileSet::MinGrid, TileSet::MaxGrid,"%d", ImGuiSliderFlags_NoInput)){
						mEditor->mTileSet.bUpdateEditSelection = true;
					}
					if(ImGui::SliderInt("Tile Scale", &mGlobalSettings.mTileSetEditScale, TileSet::MinScale, TileSet::MaxScale,"%d", ImGuiSliderFlags_NoInput)){
						mEditor->mTileSet.bUpdateEditSelectionScale = true;
					}
					ImGui::EndMenu();
				}
				
			}

			if(mEditor->mCurMode == EMODE_SELEDIT){
				if(ImGui::BeginMenu((std::string(mGlobalSettings.mGear + " Selection Edit").c_str()))){					
					if(ImGui::SliderInt("Tile Scale", &mGlobalSettings.mSelectionEditScale, TSelectionEditor::MinScale, TSelectionEditor::MaxScale,"%d", ImGuiSliderFlags_NoInput)){
						mEditor->mSelEdit.bUpdateEditSelectionScale = true;
					}
					ImGui::EndMenu();
				}
				
			}

			if(mEditor->mCurMode == EMODE_TILE){
				if(ImGui::BeginMenu((std::string(mGlobalSettings.mGear + " Tile Edit").c_str()))){					
					if(ImGui::SliderInt("Tile Scale", &mGlobalSettings.mGlobalTexParam.TexPixelSize, Tile::MinScale, Tile::MaxScale,"%d", ImGuiSliderFlags_NoInput)){						
						mEditor->mBrushesPixel.setBrushDeltas(mGlobalSettings.mGlobalTexParam.TexPixelSize, mGlobalSettings.mGlobalTexParam.TexPixelSize, &mGlobalSettings.mGlobalTexParam.TexEditScale, mGlobalSettings.mGlobalTexParam.TexEditScale, &mGlobalSettings.mGlobalTexParam);
						mEditor->mTileSet.mClipboardTiles.setBrushDeltas(mGlobalSettings.mGlobalTexParam.TexPixelSize, mGlobalSettings.mGlobalTexParam.TexPixelSize, &mGlobalSettings.mGlobalTexParam.TexEditScale, mGlobalSettings.mGlobalTexParam.TexEditScale, &mGlobalSettings.mGlobalTexParam);
					}
											
					ImGui::EndMenu();
				}
			}

			if((mEditor->mCurMode != EMODE_PALED) && (mEditor->mCurMode != EMODE_SELEDIT)){
				
				if( (mEditor->mCurMode == EMODE_SPRITE) || (mEditor->mCurMode == EMODE_TILE) ||(mEditor->mCurMode == EMODE_TILESET) || (mEditor->mCurMode == EMODE_MAP) ){
					if(ImGui::MenuItem(std::string(mGlobalSettings.mCopy + " Copy Selection (C)").c_str())){						
						mEditor->handleCopyPaste();		
					}
					if(ImGui::MenuItem(std::string(mGlobalSettings.mCut + " Move/Cut Selection (M)").c_str())){						
						mEditor->handleCopyPaste(true);		
					}
					if(ImGui::MenuItem(std::string(mGlobalSettings.mClip + " Paste Selection (V)").c_str())){						
						mEditor->handleClipboardPaste();
					}
					if(ImGui::MenuItem(std::string(mGlobalSettings.mClip + " Next Selection (CTRL + V)").c_str())){						
						mEditor->handleClipboardPaste(true);
					}
					if(ImGui::MenuItem(std::string(mGlobalSettings.mClip + " Drop Selection (F10) (ESC)").c_str())){						
						mEditor->dropBrush();
					}
				}				

				if(ImGui::MenuItem("Select All (A)")){
					mEditor->handleSelection(SELMODE_ALL);
				}
				if(ImGui::MenuItem("Select None (N)")){
					mEditor->handleSelection(SELMODE_NONE);
				}
				if(ImGui::MenuItem("Invert Selection (I)")){
					mEditor->handleSelection(SELMODE_INVERT);
				}
			}

			if(mEditor->mCurMode == EMODE_PALED){
				if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Import Palette")).c_str())){
					mEditor->mPalette.bImportingPalette = true;
				}
				if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Import Palette Sub Range")).c_str())){
					mEditor->activateDTDialog(EDIALOG_PALETTEIMPORT);
				}
				if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Export Palette")).c_str())){
					mEditor->activateDTDialog(EDIALOG_PALETTEEXPORT);
				}
				if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Load Default Palette")).c_str())){
					mEditor->mPalette.initPaletteEdit();
					mEditor->showMessage("Default X16 Palette Loaded\n  Click <Cancel Changes> to revert to previous Palette");
				}
			}
		
			if(ImGui::MenuItem("Undo (U) (CTRL + Z)")){
				mEditor->undoLastActionGroup();	  		
			}
			if(ImGui::MenuItem("Redo (R)")){
				mEditor->redoLastActionGroup();	  		
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

int OCDialog::render(){
	
	ImGui::BeginMainMenuBar();
		if (ImGui::BeginMenu("File"))
		{
			if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Open")).c_str())){				
				mGlobalSettings.mEditor->activateDTDialog(EDIALOG_PROJECTOPEN);				
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Create")).c_str())){				
				mGlobalSettings.mEditor->activateDTDialog(EDIALOG_PROJECTCREATE);		
			}

			ImGui::Separator();

			mGlobalSettings.settingsMenu();

			ImGui::Separator();


			if(ImGui::MenuItem((std::string(mGlobalSettings.mExit + " Quit")).c_str())){
				mGlobalSettings.bRunningOCD = false;
				mGlobalSettings.mEditorState = ESTATE_NONE;
			}

			ImGui::EndMenu();
		}

		std::string mDialogTextWindow = "TilemapEd";

		auto textWidth   = ImGui::CalcTextSize(mDialogTextWindow.c_str()).x;
		ImGui::SetCursorPosX((mGlobalSettings.WindowWidth - textWidth) * 0.5f);
		ImGui::Text("%s", mDialogTextWindow.c_str());

		ImGui::EndMainMenuBar();
	
	return 0;
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

	if(mDialogTextMain.size() == 0){					
		bInputIsAccepted=false;
		return 1;
	}

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

		if(bMustNotBeFolder){
				if(cIsFolder){					
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
	mHelpTextTileSet = mGlobalSettings.mHelpTextTileSet;
	mHelpTextSelection = mGlobalSettings.mHelpTextSelection;
	mHelpTextGeneral = mGlobalSettings.mHelpText;
	mHelpTextPalette = mGlobalSettings.mHelpTextPalette;
	mHelpTextImport = mGlobalSettings.mHelpTextImport;
	mHelpTextColMapEditor = mGlobalSettings.mHelpTextColMapEditor;
	mHelpTextSprite = mGlobalSettings.mHelpTextSprite;
	mHelpTextAbout = mGlobalSettings.mHelpTextAbout;
	mHelpTextLicense = mGlobalSettings.mHelpTextLicense;
	bCloseBool = &mGlobalSettings.bShowHelpDialog;
}

void HDialog::recieveInput(int mKey){
	bInputIsCancel=true;
}


int HDialog::render(){

	Dialog::render();

	ImGui::Begin("Help", bCloseBool, ImGuiWindowFlags_NoNav);

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

	if(ImGui::CollapsingHeader("TileSet")){
		ImGui::PushFont(mGlobalSettings.SFont);
		for(int i = 0; i < mHelpTextTileSet.size(); i++){
			ImGui::BulletText("%s", mHelpTextTileSet[i].c_str());
		}
		ImGui::PopFont();
	}

	ImGui::Separator();

	if(ImGui::CollapsingHeader("Selection")){
		ImGui::PushFont(mGlobalSettings.SFont);
		for(int i = 0; i < mHelpTextSelection.size(); i++){
			ImGui::BulletText("%s", mHelpTextSelection[i].c_str());
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

	if(ImGui::CollapsingHeader("Sprite")){
		ImGui::PushFont(mGlobalSettings.SFont);
		for(int i = 0; i < mHelpTextSprite.size(); i++){
			ImGui::BulletText("%s", mHelpTextSprite[i].c_str());
		}
		ImGui::PopFont();
	}

	ImGui::Separator();

	if(ImGui::CollapsingHeader("CollisionMap")){
		ImGui::PushFont(mGlobalSettings.SFont);
		for(int i = 0; i < mHelpTextColMapEditor.size(); i++){
			ImGui::BulletText("%s", mHelpTextColMapEditor[i].c_str());
		}
		ImGui::PopFont();
	}

	ImGui::Separator();

	if(ImGui::CollapsingHeader("Import")){
		ImGui::PushFont(mGlobalSettings.SFont);
		for(int i = 0; i < mHelpTextImport.size(); i++){
			ImGui::BulletText("%s", mHelpTextImport[i].c_str());
		}
		ImGui::PopFont();
	}

	ImGui::Separator();

	if(ImGui::CollapsingHeader("About")){
		ImGui::PushFont(mGlobalSettings.SFont);
		for(int i = 0; i < mHelpTextAbout.size(); i++){
			ImGui::BulletText("%s", mHelpTextAbout[i].c_str());
		}
		ImGui::PopFont();
	}

	ImGui::Separator();

	if(ImGui::CollapsingHeader("License")){
		ImGui::PushFont(mGlobalSettings.SFont);
		for(int i = 0; i < mHelpTextLicense .size(); i++){
			ImGui::BulletText("%s", mHelpTextLicense[i].c_str());
		}
		ImGui::PopFont();
	}

	ImGui::Separator();


	ImGuiStyle& style = ImGui::GetStyle();

    float size = ImGui::CalcTextSize("   Close   ").x + style.FramePadding.x * 2.0f;
    float avail = ImGui::GetContentRegionAvail().x;

    float off = (avail - size) * 0.5f;
    if(off > 0.0f){
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
	}

	if(ImGui::Button("   Close   ")){
		*bCloseBool = false;
		recieveInput(SDLK_n);
	}

	if(!*bCloseBool){
		recieveInput(SDLK_n);
	}

	ImGui::End();

	return 0;
}

int PODialog::render(int xpos, int ypos, TextureParameters *mTexParam){

	Dialog::render(ypos);

	ImGui::PushFont(mGlobalSettings.SFont);

	mDialogTextMain = "Palette Offset"; //mGlobalSettings.mImage + 

	ImGui::Begin(mDialogTextMain.c_str(), &mGlobalSettings.bShowPaletteOffset, ImGuiWindowFlags_NoNav);

	ImGui::RadioButton("0", &mTexParam->PaletteOffset, 0);
	ImGui::SameLine();
	ImGui::RadioButton("1", &mTexParam->PaletteOffset, 1);
	ImGui::SameLine();
	ImGui::RadioButton("2", &mTexParam->PaletteOffset, 2);
	ImGui::SameLine();
	ImGui::RadioButton("3", &mTexParam->PaletteOffset, 3);
	ImGui::SameLine();
	ImGui::RadioButton("4", &mTexParam->PaletteOffset, 4);
	ImGui::SameLine();
	ImGui::RadioButton("5", &mTexParam->PaletteOffset, 5);
	ImGui::SameLine();
	ImGui::RadioButton("6", &mTexParam->PaletteOffset, 6);
	ImGui::SameLine();
	ImGui::RadioButton("7", &mTexParam->PaletteOffset, 7);
	ImGui::SameLine();
	ImGui::RadioButton("8", &mTexParam->PaletteOffset, 8);
	ImGui::SameLine();
	ImGui::RadioButton("9", &mTexParam->PaletteOffset, 9);
	ImGui::SameLine();
	ImGui::RadioButton("10", &mTexParam->PaletteOffset, 10);
	ImGui::SameLine();
	ImGui::RadioButton("11", &mTexParam->PaletteOffset, 11);
	ImGui::SameLine();
	ImGui::RadioButton("12", &mTexParam->PaletteOffset, 12);
	ImGui::SameLine();
	ImGui::RadioButton("13", &mTexParam->PaletteOffset, 13);
	ImGui::SameLine();
	ImGui::RadioButton("14", &mTexParam->PaletteOffset, 14);
	ImGui::SameLine();
	ImGui::RadioButton("15", &mTexParam->PaletteOffset, 15);
	
	ImGui::PopFont();

	ImGui::End();
	return 0;
}

/* Dialog Template */

void DialogButton::render(){
	if(mCondition > -1){if(mParent->mCondition != mCondition){return;}}

	DialogElement::render();

	if(ImGui::Button(mLabel.c_str())){		
		mParent->recieveInput(mAction);
	}
}

void DialogValueRadioGroup::render(){
	if(mCondition > -1){if(mParent->mCondition != mCondition){return;}}

	for(auto *cBut : mButtons){
		cBut->render();
	}
}

void DialogValueRadioGroupCondition::render(){
	if(mCondition > -1){if(mParent->mCondition != mCondition){return;}}


	mParent->setCurrentCondition(mValue);
	
	for(auto *cBut : mButtons){		 
		cBut->DialogElement::render(); 
		if(ImGui::RadioButton(cBut->mLabel.c_str(), cBut->mTarget, cBut->mDefault)){
			mParent->setCurrentCondition(*cBut->mTarget);
			mParent->bUpdateWinPos = true;
		}
	}
}

void DialogValueFile::close(){
	ImGuiFileDialog::Instance()->Close();
}

void DialogValueFile::render(){
	if(mCondition > -1){
		if(mParent->mCondition != mCondition){
			bIsValid = true;
			mTextInput.bIsActive = false;
			return;
		}
	}

	mTextInput.render();

	if(mTextInput.bIsActive){
		mParent->mActiveInput = &mTextInput;
	}


	if(mTextInput.bInputIsAccepted){
		bIsValid = true;
		mValue = mTextInput.mDialogTextMain;
	} else {
		bIsValid = false;
	}

	DialogElement::render();

	if(ImGui::Button(mLabel.c_str())){
		ImVec2 cNewPos = ImGui::GetMainViewport()->GetCenter();
		cNewPos.x /= mGlobalSettings.mUIScale;
		cNewPos.y /= mGlobalSettings.mUIScale;

		ImGui::SetNextWindowPos(cNewPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(800, 600));
		if(mFileExt.size() == 0){
			ImGuiFileDialog::Instance()->OpenDialog(mFileKey.c_str(), mLabel.c_str(), NULL, ".");
		} else {
    		ImGuiFileDialog::Instance()->OpenDialog(mFileKey.c_str(), mLabel.c_str(), mFileExt.c_str(), ".");
		}
	}

	if (ImGuiFileDialog::Instance()->Display(mFileKey.c_str())){
    	
    	if (ImGuiFileDialog::Instance()->IsOk()){
      		std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      		std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
	  		mTextInput.mDialogTextMain = filePathName;
			mTextInput.checkCurrentText();
			if(mTextInput.bInputIsAccepted){
				bIsValid = true;
				mValue = mTextInput.mDialogTextMain;
			} else {
				bIsValid = false;
			}
      	
    	}    
    
    	ImGuiFileDialog::Instance()->Close();
  	}

}

void DTDialog::init(){

}

int DTDialog::render(){

	Dialog::render();

	ImGui::Begin(mDialogTextTitle.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);

	if(ImGui::IsWindowFocused()){
		if(mFiles.size()){
			ImGuiFileDialog::Instance()->Close();
		}
	}

	for(auto *cElem: mElements){
		cElem->render();
	}

	ImGui::End();

	return 0;
}

void DTDialog::update(){

	for(auto *cVals: mValues){
		cVals->update();
	}
}

void DTDialog::recieveInput(int mKey){
	if(mKey == SDLK_y){

		for(auto *cInput : mFiles){
			if(!cInput->bIsValid){
				return;
			}
		}

		bInputIsAccept=true;	
		bDialogIsWatingForText = false;

		for(auto cVal : mValues){
			cVal->apply();			
		}

		if(mTargetState != -1){
			mGlobalSettings.mEditorState = mTargetState;
		}
	}		
	
	if(mKey == SDLK_n){
		bInputIsCancel=true;
		bDialogIsWatingForText = false;
		for(auto cVal : mValues){
			cVal->cancel();
		}
	}	

	if(mKey == SDLK_TAB){
		if(mActiveInput){
			if(mActiveInput->bIsActive){
				mActiveInput->autoComplete();
			}
		}		 
	}
}

void DTDialog::dropLastInputChar(){	
	if(mActiveInput){
		if(mActiveInput->bIsActive){
			mActiveInput->dropLastInputChar();
		}
	}	
}

void DTDialog::cancel(){
	Dialog::cancel();

	for(auto cVal : mValues){
		cVal->cancel();
	}

	if(bCloseBool){
		*bCloseBool = false;
	}
}

void DTDialog::addText(std::string cText, bool bSameline){
	DialogElementText *nText = new DialogElementText(this, mRequiredCondition, cText, bSameline);

	mElements.push_back(nText);
	mBasicElements.push_back(nText);
}

void DTDialog::addSeperator(){
	DialogElementSeperator *nSep = new DialogElementSeperator();

	nSep->mParent = this;
	nSep->mCondition = mRequiredCondition;

	mElements.push_back(nSep);
	mBasicElements.push_back(nSep);
}

void DTDialog::addSameLine(){
	DialogElement *nSame = new DialogElement();
	nSame->bSameLine = true;
	nSame->mParent = this;
	nSame->mCondition = mRequiredCondition;
	mElements.push_back(nSame);
}

void DTDialog::addConditionRestore(){
	DialogConditionRestore *nCondRes = new DialogConditionRestore(this);
	mElements.push_back(nCondRes);
	mValues.push_back(nCondRes);
}

void DTDialog::addConditionSetIf(int cTarget, int cState){
	DialogConditionSetIf *nCondSet = new DialogConditionSetIf(this, cTarget, cState);
	mElements.push_back(nCondSet);
	mValues.push_back(nCondSet);
}

void DTDialog::addConditionSetIfInt(int *cTarget, int cValue, int cCond){
	DialogConditionSetIfInt *nCondSet = new DialogConditionSetIfInt(this, mRequiredCondition, cTarget, cValue, cCond);
	mElements.push_back(nCondSet);
	mValues.push_back(nCondSet);
}

void DTDialog::addConditionSetIfBool(bool *cTarget, bool cValue, int cCond){
	DialogConditionSetIfBool *nCondSet = new DialogConditionSetIfBool(this, mRequiredCondition, cTarget, cValue, cCond);
	mElements.push_back(nCondSet);
	mValues.push_back(nCondSet);
}

void DTDialog::addBool(std::string cLabel, bool cDefault, bool *cTarget, bool bSameline){
	DialogValueBool *nBool = new DialogValueBool(this, mRequiredCondition, cLabel, cDefault, cTarget, bSameline);

	mElements.push_back(nBool);
	mValues.push_back(nBool);
}

void DTDialog::addBoolCondition(std::string cLabel, bool cDefault, bool *cTarget, int cTargCond, bool bSameline){
	DialogValueBoolCondition *nBool = new DialogValueBoolCondition(this, mRequiredCondition, cLabel, cDefault, cTarget, cTargCond, bSameline);

	mElements.push_back(nBool);
	mValues.push_back(nBool);
}

void DTDialog::addInt(std::string cLabel, int cDefault, int *cTarget, int cMin, int cMax, bool bSameline){
	DialogValueInt *nInt = new DialogValueInt(this, mRequiredCondition, cLabel, cDefault, cTarget, cMin, cMax, bSameline);

	mElements.push_back(nInt);
	mValues.push_back(nInt);

}

void DTDialog::addColor(std::string cLabel, ImU32 *cDefaultColor, ImU32 *cTarget, bool bSameline){
	DialogValueColor *nCol = new DialogValueColor(this, mRequiredCondition, cLabel, cDefaultColor, cTarget, bSameline);

	mElements.push_back(nCol);
	mValues.push_back(nCol);
}

void DTDialog::addIntMinMax(std::string cLabel, int cDefault, int *cTarget, int *cMin, int *cMax, bool bSameline){
	DialogValueIntMinMax *nInt = new DialogValueIntMinMax(this, mRequiredCondition, cLabel, cDefault, cTarget, cMin, cMax, bSameline);

	mElements.push_back(nInt);
	mValues.push_back(nInt);
}


void DTDialog::addIntStrings(std::string cLabel, int cDefault, int *cTarget, std::vector<std::string> &cStrings, bool bSameline){
	DialogValueIntStrings *nInt = new DialogValueIntStrings(this, mRequiredCondition, cLabel, cDefault, cTarget, cStrings, bSameline);

	mElements.push_back(nInt);
	mValues.push_back(nInt);
}

void DTDialog::addFloat(std::string cLabel, float cDefault, float *cTarget, float cMin, float cMax,std::string cFormat, bool bSameline){
	
	DialogValueFloat *nFloat = new DialogValueFloat(this, mRequiredCondition, cLabel, cDefault, cTarget, cMin, cMax, cFormat, bSameline);

	mElements.push_back(nFloat);
	mValues.push_back(nFloat);
}

void DTDialog::addButton(std::string cLabel, int cAction, bool cSameline){
	DialogButton * nBut = new DialogButton(this, mRequiredCondition, cLabel, cAction, cSameline);

	mElements.push_back(nBut);
	mButtons.push_back(nBut);
}

void DTDialog::addRadioGroup(int cDefault, int* cTarget){
	DialogValueRadioGroup* nGroup = new DialogValueRadioGroup(this, mRequiredCondition, cDefault, cTarget);

	mElements.push_back(nGroup);
	mValues.push_back(nGroup);
}

void DTDialog::addRadioGroupCondition(int cDefault, int* cTarget){
	DialogValueRadioGroupCondition* nGroup = new DialogValueRadioGroupCondition(this, mRequiredCondition, cDefault, cTarget);

	mElements.push_back(nGroup);
	mValues.push_back(nGroup);
}

void DTDialog::addRadioButton(std::string cLabel, int cDefault, bool cSameline){
	DialogValueRadioGroup* cGroup = dynamic_cast<DialogValueRadioGroup*>(mValues[mValues.size()-1]);

	if(cGroup){
		DialogValueRadioButton* nGBut = new DialogValueRadioButton(cLabel, cDefault, &cGroup->mValue, cSameline);
		cGroup->mButtons.push_back(nGBut);
	} else {
		DialogValueRadioGroupCondition* cGroupCond = dynamic_cast<DialogValueRadioGroupCondition*>(mValues[mValues.size()-1]);
		if(cGroupCond){
			DialogValueRadioButton* nGBut = new DialogValueRadioButton(cLabel, cDefault, &cGroupCond->mValue, cSameline);
			cGroupCond->mButtons.push_back(nGBut);
		}
	}
}

void DTDialog::addIntTarget(int cDefault, int *cTarget){
	DialogValueIntTarget *nInt = new DialogValueIntTarget(this, mRequiredCondition, cDefault, cTarget);
	
	mValues.push_back(nInt);
}

void DTDialog::addFile(std::string cLabel,std::string cInputLabel, std::string cFileExt, std::string cFileKey, std::string cDefault, std::string* cTarget, bool cMustExist, bool cMustBeFile, bool cMustBeFolder, bool cMustNotBeFile, bool cMustNotExist, bool cMustBeProject, bool cMustNotBeFolder, bool cSameline){

	DialogValueFile *nFile = new DialogValueFile(this, mRequiredCondition, cLabel, cInputLabel, cFileExt, cFileKey, cDefault, cTarget, cMustExist, cMustBeFile, cMustBeFolder, cMustNotBeFile, cMustNotExist, cMustBeProject);

	nFile->mTextInput.bMustNotBeFolder = cMustNotBeFolder;

	nFile->bSameLine = cSameline;

	mElements.push_back(nFile);
	mValues.push_back(nFile);
	mFiles.push_back(nFile);

	mActiveInput = &nFile->mTextInput;

	bDialogIsWatingForText = true;
}

void DTDialog::addFileDefault(std::string cLabel,std::string cInputLabel, std::string cFileExt, std::string cFileKey, std::string *cDefaultPath, std::string* cTarget, bool cMustExist, bool cMustBeFile, bool cMustBeFolder, bool cMustNotBeFile, bool cMustNotExist, bool cMustBeProject, bool cSameline){

	DialogValueFileDefault *nFile = new DialogValueFileDefault(this, mRequiredCondition, cLabel, cInputLabel, cFileExt, cFileKey, cDefaultPath, cTarget, cMustExist, cMustBeFile, cMustBeFolder, cMustNotBeFile, cMustNotExist, cMustBeProject);

	nFile->bSameLine = cSameline;

	mElements.push_back(nFile);
	mValues.push_back(nFile);
	mFiles.push_back(nFile);

	mActiveInput = &nFile->mTextInput;

	bDialogIsWatingForText = true;
}

void DTDialog::addSetFileExt(std::string cKey, std::string cExt){

	DialogValueSetFileExt *nFileExt = new DialogValueSetFileExt(this, mRequiredCondition, cKey, cExt);

	mElements.push_back(nFileExt);
}

/* DTDCDialog */

int DTDCDialog::render(){

	if(bConfirmIsActive && !bFirstConfirm && !bStillConfirm){
		bFirstConfirm = true;
	}

	Dialog::render();

	ImGui::Begin(mDialogTextTitle.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);

	if(ImGui::IsWindowFocused()){
		if(mFiles.size()){
			ImGuiFileDialog::Instance()->Close();
		}

		if(bFirstConfirm){
			bFirstConfirm = false;
			bStillConfirm = true;
		} else {
			if(bConfirmIsActive){
				bConfirmIsActive = false;
				bStillConfirm = false;
				bFirstConfirm = false;
				mConfirmDialog.cancel();
			}
		}
	}

	for(auto *cElem: mElements){
		cElem->render();
	}

	ImGui::End();

	if(bConfirmIsActive){				
		mConfirmDialog.render();
		if(mConfirmDialog.bInputIsAccept){
			recieveInput(SDLK_y);
		}
		if(mConfirmDialog.bInputIsCancel){
			recieveInput(SDLK_n);
		}
	}

	return 0;
}

void DTDCDialog::recieveInput(int mKey){
	if(mKey == SDLK_y){

		for(auto *cInput : mFiles){
			if(!cInput->bIsValid){
				return;
			}
		}

		if(bConfirmIsActive){
			mConfirmDialog.recieveInput(SDLK_y);
		}
		
		if(mConditionPath){			
			if(fs::exists(fs::status(*mConditionPath)) == bConditionState){				
				bConfirmIsActive = true;					
			}
		}

		bool bConfirmIsGo = false;

		if(bConfirmIsActive){			
			if(mConfirmDialog.bInputIsAccept){				
				bConfirmIsGo = true;
			}
		} else {
			bConfirmIsGo = true;
		}

		if(bConfirmIsGo){
			bInputIsAccept=true;	
			bDialogIsWatingForText = false;

			for(auto cVal : mValues){
				cVal->apply();			
			}

			if(mTargetState != -1){
				mGlobalSettings.mEditorState = mTargetState;
			}
		}

		return;		
	}		
	
	if(mKey == SDLK_n){
		if(bConfirmIsActive){
			bConfirmIsActive = false;
			mConfirmDialog.cancel();
			bStillConfirm = false;
			bFirstConfirm = false;			
		} else {
			bInputIsCancel=true;
			bDialogIsWatingForText = false;
			for(auto cVal : mValues){
				cVal->cancel();
			}
			mConfirmDialog.cancel();
			bStillConfirm = false;
			bFirstConfirm = false;
		}
		return;
	}	

	if(mKey == SDLK_TAB){
		if(mActiveInput && !bConfirmIsActive){			
			if(mActiveInput->bIsActive){
				mActiveInput->autoComplete();
			}
		}		 
	}
}

void DTDCDialog::cancel(){
	Dialog::cancel();

	for(auto cVal : mValues){
		cVal->cancel();
	}

	bConfirmIsActive = false;
	bStillConfirm = false;
	bFirstConfirm = false;
		
	mConfirmDialog.cancel();

	if(bCloseBool){
		*bCloseBool = false;
	}
}

void DTDCDialog::addConfirmText(std::string cText, bool bSameline){
	mConfirmDialog.addText(cText, bSameline);
}

void DTDCDialog::setConfirmButtons(std::string cConfirm, std::string cCancel){
	mConfirmDialog.addSeperator();
	mConfirmDialog.addButton(cConfirm, SDLK_y);
	mConfirmDialog.addButton(cCancel, SDLK_n, true);
}

void DTDCDialog::setConfirmConditionExists(std::string *cPath, bool cState){
	mConditionPath = cPath;
	bConditionState = cState;
}

void DTDialog::addIntActiveMinus(std::string cLabel, int cBase, int *cTarget){
	DialogValueIntActive *nInt = new DialogValueIntActive(this, mRequiredCondition, cLabel, cBase, cTarget, 0);

	mElements.push_back(nInt);
	mValues.push_back(nInt);
}

bool* DTDialog::getBoolValue(std::string cBoolLabel){

	for(auto *cInput : mValues){

		DialogValueBool *cBool = dynamic_cast<DialogValueBool*>(cInput);

		if(cBool){
			if(cBool->mLabel == cBoolLabel){
				return &cBool->mValue;
			}
		}

		DialogValueBoolCondition *cBoolCond = dynamic_cast<DialogValueBoolCondition*>(cInput);

		if(cBoolCond){
			if(cBoolCond->mLabel == cBoolLabel){
				return &cBoolCond->mValue;
			}
		}
	}

	return NULL;
}

int* DTDialog::getIntValue(std::string cIntLabel){
	for(auto *cInput : mValues){

		DialogValueInt *cInt = dynamic_cast<DialogValueInt*>(cInput);

		if(cInt){
			if(cInt->mLabel == cIntLabel){
				return &cInt->mValue;
			}
		}

		DialogValueIntMinMax *cIntMinMax = dynamic_cast<DialogValueIntMinMax*>(cInput);

		if(cIntMinMax){
			if(cIntMinMax->mLabel == cIntLabel){
				return &cIntMinMax->mValue;
			}
		}		
	}
	return NULL;
}

int* DTDialog::getIntValueRadio(int cIntIndex){

	int cRadioCount = 0;

	for(auto *cInput : mValues){

		DialogValueRadioGroup *cRadioInt = dynamic_cast<DialogValueRadioGroup*>(cInput);

		if(cRadioInt){
			if(cRadioCount == cIntIndex){
				return &cRadioInt->mValue;
			}
			cRadioCount++;
		}
	}
	return NULL;
}

int* DTDialog::getIntActiveValue(std::string cIntLabel){
	
	for(auto *cInput : mValues){

		DialogValueIntActive *cInt = dynamic_cast<DialogValueIntActive*>(cInput);

		if(cInt){
			if(cInt->mLabel == cIntLabel){
				return &cInt->mValue;
			}
		}		
	}
	return NULL;
}

std::string* DTDialog::getFileExt(std::string cFileVal){
	for(auto *cInput : mFiles){
		if(cInput->mFileKey == "ChooseFileDlgKey"+cFileVal){
			return &cInput->mFileExt;
		}
	}
	return NULL;
}

std::string* DTDialog::getFilePath(std::string cFileVal){
	for(auto *cInput : mFiles){
		if(cInput->mFileKey == "ChooseFileDlgKey"+cFileVal){
			return &cInput->mTextInput.mDialogTextMain;
		}
	}
	return NULL;
}


/* DTDCDialog End*/

/* IDDialog */

int IDDialog::render(int xpos, int ypos){

	Dialog::render(xpos, ypos);

	if(bSmallFonts){
		ImGui::PushFont(mGlobalSettings.SFont);
	}

	ImGui::Begin(mDialogTextTitle.c_str(), bCloseBool, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);

	for(auto *cElem: mElements){
		cElem->render();
	}

	ImGui::End();


	if(bSmallFonts){
		ImGui::PopFont();
	}

	return 0;
}

void IDDialog::update(){

	for(auto *cDisps : mDisplays){
		cDisps->update();
	}

}

void IDDialog::setCloseBool(bool *cClose){bCloseBool = cClose;}

void IDDialog::setFontsSmall(bool cSmall){bSmallFonts = cSmall;}

void IDDialog::addDisplayInt(std::string cLabel, int* cTarget, bool cSameline){
	DialogDisplayInt *nInt = new DialogDisplayInt(this, mRequiredCondition, cLabel, cTarget, cSameline);

	mDisplays.push_back(nInt);
	mElements.push_back(nInt);
}

void IDDialog::addDisplayString(std::string cLabel, std::string* cTarget, bool cSameline){
	DialogDisplayString *nStr = new DialogDisplayString(this, mRequiredCondition, cLabel, cTarget, cSameline);

	mDisplays.push_back(nStr);
	mElements.push_back(nStr);
}

void IDDialog::addDisplayColor(SDL_Color *cTarget){
	DialogDisplayColor *nCol = new DialogDisplayColor(this, mRequiredCondition, "color", cTarget, false);
	
	mElements.push_back(nCol);
}

void IDDialog::addDisplayColorRestore(){
	DialogDisplayColorRestore *nCol = new DialogDisplayColorRestore(this, mRequiredCondition, "color", false);
	
	mElements.push_back(nCol);
}

void IDDialog::addDisplayIntDual(std::string cLabel,std::string cCenter,std::string cEnd, int* cTarget1, int* cTarget2, bool cSameline){
	DialogDisplayIntDual *nInt = new DialogDisplayIntDual(this, mRequiredCondition, cLabel, cCenter, cEnd, cTarget1, cTarget2, cSameline);

	mDisplays.push_back(nInt);
	mElements.push_back(nInt);
}
		
void IDDialog::addDisplayTileCount(std::string cLabel, std::vector<Tile*> *cTarget, bool cSameline){
	DialogDisplayTileCount *nTlC = new DialogDisplayTileCount(this, mRequiredCondition, cLabel, cTarget, cSameline);

	mDisplays.push_back(nTlC);
	mElements.push_back(nTlC);
}

void IDDialog::addDisplayTileMapCount(std::string cLabel, std::vector<TileMap*> *cTarget, bool cSameline){
	DialogDisplayTileMapCount *nTMC = new DialogDisplayTileMapCount(this, mRequiredCondition, cLabel, cTarget, cSameline);

	mDisplays.push_back(nTMC);
	mElements.push_back(nTMC);
}

void IDDialog::addDisplaySpriteCount(std::string cLabel, std::vector<TSprite*> *cTarget, bool cSameline){
	DialogDisplaySpriteCount *nSC = new DialogDisplaySpriteCount(this, mRequiredCondition, cLabel, cTarget, cSameline);

	mDisplays.push_back(nSC);
	mElements.push_back(nSC);
}

void IDDialog::addDisplayTileMapSize(std::string cLabel, TileMap **cTarget, bool cSameline){
	DialogDisplayTileMapGetSize *nTMGS = new DialogDisplayTileMapGetSize(this, mRequiredCondition, cLabel, cTarget, cSameline);

	mDisplays.push_back(nTMGS);
	mElements.push_back(nTMGS);
}

void IDDialog::addDisplaySpriteSize(std::string cLabel,  TSprite **cTarget, bool cSameline){
	DialogDisplaySpriteGetSize *nSGS = new DialogDisplaySpriteGetSize(this, mRequiredCondition, cLabel, cTarget, cSameline);

	mDisplays.push_back(nSGS);
	mElements.push_back(nSGS);

}

/* IDDialog create */

IDDialog* IDDialog::createMessageDialog(){
	IDDialog *newDialog = new IDDialog();

	newDialog->setLabel("Message");

	newDialog->setRequiredCondition(1);
	newDialog->addDisplayColor(&mGlobalSettings.ErrorTextColor);

	newDialog->clearRequiredCondition();
	newDialog->addDisplayString(mGlobalSettings.mInfo, &mGlobalSettings.mProgramMessage);

	newDialog->setRequiredCondition(1);
	newDialog->addDisplayColorRestore();

	newDialog->clearRequiredCondition();

	newDialog->addSeperator();

	newDialog->addButton("Close", SDLK_n);

	return newDialog;
}

IDDialog* IDDialog::createProjectInfoDialog(){
	IDDialog *newDialog = new IDDialog();

	newDialog->setLabel(mGlobalSettings.mInfo + " Project Info");

	newDialog->setCloseBool(&mGlobalSettings.bShowProjectInfo);

	newDialog->setFontsSmall(true);

	newDialog->createValues(5);

	newDialog->addDisplayTileMapSize("TileMap Size: ", &mGlobalSettings.mEditor->mTileMap);
	newDialog->addDisplayTileMapCount("TileMaps in Project: ", &mGlobalSettings.mEditor->mTileMaps);
	newDialog->addDisplayInt("Selected TileMap: ",  &mGlobalSettings.mEditor->mSelectedTileMap);

	newDialog->addDisplayTileCount("Tiles in TileSet: ", &mGlobalSettings.mEditor->mTileSet.TTiles);
	newDialog->addDisplayIntDual("Tile Size: ","x","",&mGlobalSettings.mGlobalTexParam.TexSizeX, &mGlobalSettings.mGlobalTexParam.TexSizeY);
	newDialog->addDisplayInt("Tile BPP: ",&mGlobalSettings.mGlobalTexParam.TexBPP);
	newDialog->addDisplayInt("Selected Tile: ", &mGlobalSettings.mEditor->mMapSelectedTile);
	newDialog->addDisplayIntDual("Selected Tile in TileMap: (", ",", ")", newDialog->getValue(0), newDialog->getValue(1));
	newDialog->addDisplayIntDual("Tile Flip (H/V): (", "/", ")", newDialog->getValue(2), newDialog->getValue(3));

	newDialog->setRequiredCondition(1);

	newDialog->addDisplaySpriteSize("Sprite Size: ", &mGlobalSettings.mEditor->mSprite);
	newDialog->addDisplaySpriteCount("Sprites in Project: ", &mGlobalSettings.mEditor->mSprites);
	newDialog->addDisplayInt("Selected Sprite: ",  &mGlobalSettings.mEditor->mSelectedSprite);
	newDialog->addDisplayInt("Sprite BPP: ",  newDialog->getValue(4));

	newDialog->clearRequiredCondition();
	
	return newDialog;
}

/* IDDialog create end */

/* DTDialog create */

DTDialog* DTDialog::createSpriteRotationsDialog(){
	
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Create Frame Rotations");

	newDialog->setTarget(ESTATE_SPRITEROTATIONS);

	newDialog->addText(mGlobalSettings.mImage + " Create Rotations of Current Frame?");

	newDialog->addText("Select Angle in Degrees between Frames:");

	newDialog->addFloat("Angle", 0.0, &mGlobalSettings.mNewSpriteRotationsAngle, 0, 45.0);

	newDialog->addText("Select Number of Rotations:");

	newDialog->addInt("Rotations", 1,  &mGlobalSettings.mNewSpriteRotations, 1, 90);

	newDialog->addSeperator();

	newDialog->addButton("Create", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;

}

DTDialog* DTDialog::createSpriteRotationRangeDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Create Rotation Range");

	newDialog->setTarget(ESTATE_SPRITEROTATIONRANGE);

	newDialog->addText(mGlobalSettings.mImage + " Create Rotation Range from Current Frame?");
	
	newDialog->addText("Select Range in Degrees:");

	newDialog->addRadioGroup(90, &mGlobalSettings.mNewSpriteRange);
	newDialog->addRadioButton("0-90", 90, false);	
	newDialog->addRadioButton("0-180", 180, true);

	newDialog->addText("Intervals between Start and End angle:");

	std::vector<std::string> cStrings = {"1", "3", "7", "15", "31", "63", "127"};
	newDialog->addIntStrings("Intervals", 0, &mGlobalSettings.mNewSpriteRangeIntervals, cStrings);

	newDialog->addSeperator();

	newDialog->addButton("Create", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);


	return newDialog;
}


DTDialog* DTDialog::createSpriteUpscaledCopyDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Create Upscaled Copy");

	newDialog->setTarget(ESTATE_SPRITEUPSCALEDCOPY);

	newDialog->setRequiredCondition(8);
	newDialog->addText(mGlobalSettings.mFile + " Create Upscaled Sprite Copy?");

	newDialog->setRequiredCondition(4);
	newDialog->addText(mGlobalSettings.mFile + " Create Upscaled Sprite Copy?");

	newDialog->setRequiredCondition(2);
	newDialog->addText(mGlobalSettings.mFile + " Create Upscaled Sprite Copy by 2X?");

	newDialog->setRequiredCondition(8);
	newDialog->addRadioGroup(2, &mGlobalSettings.mNewSpriteUpscale);

	newDialog->addRadioButton("2X", 2, false);
	newDialog->addRadioButton("4X", 4, true);
	newDialog->addRadioButton("8X", 8, true);

	newDialog->setRequiredCondition(4);
	newDialog->addRadioGroup(2, &mGlobalSettings.mNewSpriteUpscale);

	newDialog->addRadioButton("2X", 2, false);
	newDialog->addRadioButton("4X", 4, true);
	
	newDialog->setRequiredCondition(2);
	newDialog->addIntTarget(2, &mGlobalSettings.mNewSpriteUpscale);

	newDialog->clearRequiredCondition();

	newDialog->addSeperator();

	newDialog->addButton("Create", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);


	return newDialog;
}

DTDialog* DTDialog::createSpriteDownscaledCopyDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Create Downscaled Copy");

	newDialog->setTarget(ESTATE_SPRITEDOWNSCALEDCOPY);

	newDialog->setRequiredCondition(8);
	newDialog->addText(mGlobalSettings.mFile + " Create Downscaled Sprite Copy?");

	newDialog->setRequiredCondition(4);
	newDialog->addText(mGlobalSettings.mFile + " Create Downscaled Sprite Copy?");

	newDialog->setRequiredCondition(2);
	newDialog->addText(mGlobalSettings.mFile + " Create Downscaled Sprite Copy by 2X?");


	newDialog->setRequiredCondition(8);
	newDialog->addRadioGroup(2, &mGlobalSettings.mNewSpriteDownscale);

	newDialog->addRadioButton("2X", 2, false);
	newDialog->addRadioButton("4X", 4, true);
	newDialog->addRadioButton("8X", 8, true);

	newDialog->setRequiredCondition(4);
	newDialog->addRadioGroup(2, &mGlobalSettings.mNewSpriteDownscale);

	newDialog->addRadioButton("2X", 2, false);
	newDialog->addRadioButton("4X", 4, true);
	
	newDialog->setRequiredCondition(2);
	newDialog->addIntTarget(2, &mGlobalSettings.mNewSpriteDownscale);

	newDialog->clearRequiredCondition();

	newDialog->addSeperator();

	newDialog->addButton("Create", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}


DTDialog* DTDialog::createSpriteScaledCopyDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Create Scaled Copy");

	newDialog->setTarget(ESTATE_SPRITESCALEDCOPY);

	newDialog->addText(mGlobalSettings.mFile + " Create Scaled Sprite Copy?");

	newDialog->addFloat("Sprite Scale", 1.0, &mGlobalSettings.mNewSpriteScale, 0.5, 2.0);

	newDialog->addSeperator();

	newDialog->addButton("Create", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createFrameRotateDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Rotate Selected Frame");

	newDialog->setTarget(ESTATE_FRAMEROTATE);

	newDialog->addText(mGlobalSettings.mImage + " Rotate Selected Frame by Angle in Degrees?");

	newDialog->addFloat("Frame Angle", 0.0, &mGlobalSettings.mRotateFrameAngle, 0.0, 360.0);

	newDialog->addSeperator();

	newDialog->addButton("Rotate", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createFrameScaleDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Scale Selected Frame");

	newDialog->setTarget(ESTATE_FRAMESCALE);

	newDialog->addText(mGlobalSettings.mImage + " Scale Selected Frame by Float Factor?");

	newDialog->addFloat("Frame Scale", 1.0, &mGlobalSettings.mScaleFrameFactor, 0.5, 2.0);

	newDialog->addSeperator();

	newDialog->addButton("Scale", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createSpriteCopyDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Create Copy");

	newDialog->setTarget(ESTATE_SPRITECOPY);

	newDialog->addText(mGlobalSettings.mFile + " Create Copy of Current Sprite?");

	newDialog->addSeperator();

	newDialog->addButton("Create", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createTileMapRemoveDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Remove TileMap");

	newDialog->setTarget(ESTATE_TILEMAPDELETE);

	newDialog->addText(mGlobalSettings.mFile + " Remove Current TileMap?");
	newDialog->addText(mGlobalSettings.mInfo + " Undo Stack will be cleared");

	newDialog->addSeperator();

	newDialog->addButton("Remove", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createColMapRemoveDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Remove CollisionMap");

	newDialog->setTarget(ESTATE_COLMAPREMOVE);

	newDialog->addText(mGlobalSettings.mFile + " Remove CollisionMap for current TileMap?");
	newDialog->addText(mGlobalSettings.mInfo + " Can not be undone");

	newDialog->addSeperator();

	newDialog->addButton("Remove", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createTilesUnusedRemoveDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Remove Unused Tiles");

	newDialog->setTarget(ESTATE_TILEDELETEALL);

	newDialog->addText(mGlobalSettings.mImage + " Remove Unused Tiles?");
	newDialog->addText(mGlobalSettings.mInfo + " Undo Stack will be cleared");

	newDialog->addSeperator();

	newDialog->addButton("Remove", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createFrameRemoveDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Remove Selected Frame");

	newDialog->setTarget(ESTATE_FRAMEDELETE);

	newDialog->addText(mGlobalSettings.mImage + " Remove Selected Frame?");
	newDialog->addText(mGlobalSettings.mInfo + " Sprite Undo Stack will be cleared");
	newDialog->addBool("Show this warning", true, &mGlobalSettings.bSpriteWarnBeforeDelete);

	newDialog->addSeperator();

	newDialog->addButton("Remove", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createSpriteRemoveDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Remove Sprite");

	newDialog->setTarget(ESTATE_SPRITEDELETE);

	newDialog->addText(mGlobalSettings.mFile + " Remove Current Sprite?");
	newDialog->addText(mGlobalSettings.mInfo + " Can not be undone");

	newDialog->addSeperator();

	newDialog->addButton("Remove", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createTileRemoveDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Remove Selected Tile");

	newDialog->setTarget(ESTATE_TILEDELETE);

	newDialog->addText(mGlobalSettings.mImage + " Remove Selected Tile?");
	newDialog->addText(mGlobalSettings.mInfo + " Undo Stack will be cleared");

	newDialog->addBool("Show this warning", true, &mGlobalSettings.bTileSetWarnBeforeDelete);

	newDialog->addSeperator();

	newDialog->addButton("Remove", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createTileSetImportDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Import TileSet");

	newDialog->setTarget(ESTATE_TILESETIMPORT);

	newDialog->addText(mGlobalSettings.mFile + " Import TileSet from Project file or PNG?");

	newDialog->addFile("Choose TileSet File", "Filename", ".png,.bin", "ImpTileSet", "", &mGlobalSettings.mNewTilePath);

	newDialog->addSeperator();

	newDialog->addButton("Import", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createSpriteFrameImportDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Import Frame");

	newDialog->setTarget(ESTATE_FRAMEIMPORT);

	newDialog->addText(mGlobalSettings.mImage + " Import Sprite Frame from File?");

	newDialog->addFile("Choose Frame File", "Filename", ".png,.bin,.data,.raw", "SFrame", "", &mGlobalSettings.mNewFramePath);

	newDialog->addSeperator();

	newDialog->addButton("Import", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createSpriteFramesImportDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Import Frame(s)");

	newDialog->setTarget(ESTATE_FRAMESIMPORT);

	newDialog->addText(mGlobalSettings.mImage + " Import Sprite Frame(s) from File?");

	newDialog->addFile("Choose Frame(s) File", "Filename", ".png,.bin,.data,.raw", "SFrames", "", &mGlobalSettings.mNewFramesPath);

	newDialog->addSeperator();

	newDialog->addButton("Import", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createTileImportDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Import Tile");

	newDialog->setTarget(ESTATE_TILEIMPORT);

	newDialog->addText(mGlobalSettings.mImage + " Import Tile from PNG or RAW?");

	newDialog->addFile("Choose Tile File", "Filename", ".png,.bin,.data,.raw", "TileImp", "", &mGlobalSettings.mNewTilePath);

	newDialog->addSeperator();

	newDialog->addButton("Import", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createThemeColorDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Change Theme Color");

	newDialog->setTarget(ESTATE_THEMECOLOR);

	newDialog->setRequiredCondition(1);
	newDialog->addText(mGlobalSettings.mInfo + " Choose \"Selection Dark\" Color");

	newDialog->setRequiredCondition(2);
	newDialog->addText(mGlobalSettings.mInfo + " Choose \"Selection Light\" Color");
	
	newDialog->setRequiredCondition(3);
	newDialog->addText(mGlobalSettings.mInfo + " Choose \"Highlight Dark\" Color");

	newDialog->setRequiredCondition(4);
	newDialog->addText(mGlobalSettings.mInfo + " Choose \"Highlight Light\" Color");

	newDialog->setRequiredCondition(5);
	newDialog->addText(mGlobalSettings.mInfo + " Choose \"PixelGrid Dark\" Color");

	newDialog->setRequiredCondition(6);
	newDialog->addText(mGlobalSettings.mInfo + " Choose \"PixelGrid Light\" Color");

	newDialog->clearRequiredCondition();

	newDialog->addSeperator();

	newDialog->addColor("##themepick", &mGlobalSettings.mThemeColorNew, &mGlobalSettings.mThemeColorNew);

	newDialog->addSeperator();

	newDialog->addButton("Choose", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createSpriteConvertBPPDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Sprite Convert BPP");

	newDialog->setTarget(ESTATE_SPRITECONVERTBPP);

	newDialog->setRequiredCondition(4);
	newDialog->addText(mGlobalSettings.mFile + " Create Sprite Copy with BPP 8?");
	newDialog->clearRequiredCondition();

	newDialog->setRequiredCondition(8);
	newDialog->addText(mGlobalSettings.mFile + " Create Sprite Copy with BPP 4?");
	newDialog->clearRequiredCondition();

	newDialog->addSeperator();

	newDialog->addButton("Convert", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createProjectOpenDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Open Project");

	newDialog->setTarget(ESTATE_PROJECTOPEN);

	newDialog->addText(mGlobalSettings.mFile + " Open Project from Folder?");

	newDialog->addSeperator();

	newDialog->addFile("Choose Project Folder", "Folder", "", "Prfold", "", &mGlobalSettings.ProjectPath, true, false, true, false, false, true);

	newDialog->addSeperator();

	newDialog->addButton("Open", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createProjectCreateDialog(){

	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Create Project");

	newDialog->setTarget(ESTATE_PROJECTCREATE);

	newDialog->addText(mGlobalSettings.mFile + " Create New Project?");

	newDialog->addSeperator();

	newDialog->addText("TileMap Width & Height");

	newDialog->addRadioGroup(32, &mGlobalSettings.TileMapWidth);
	newDialog->addRadioButton("W: 32", 32);
	newDialog->addRadioButton("W: 64", 64, true);
	newDialog->addRadioButton("W: 128", 128, true);
	newDialog->addRadioButton("W: 256", 256, true);

	newDialog->addRadioGroup(32, &mGlobalSettings.TileMapHeight);
	newDialog->addRadioButton("H: 32", 32);
	newDialog->addRadioButton("H: 64", 64, true);
	newDialog->addRadioButton("H: 128", 128, true);
	newDialog->addRadioButton("H: 256", 256, true);

	newDialog->addSeperator();

	newDialog->addText("Tile Size X/Y");

	newDialog->addRadioGroup(16, &mGlobalSettings.mGlobalTexParam.TexSizeX);
	newDialog->addRadioButton("X: 8", 8);
	newDialog->addRadioButton("X: 16", 16, true);

	newDialog->addRadioGroup(16, &mGlobalSettings.mGlobalTexParam.TexSizeY);
	newDialog->addRadioButton("Y: 8", 8);
	newDialog->addRadioButton("Y: 16", 16, true);

	newDialog->addSeperator();

	newDialog->addBoolCondition("Create/Import TileSet?", false, NULL, 1);
	
	newDialog->setRequiredCondition(1);

	newDialog->addRadioGroupCondition(2, NULL);
	newDialog->addRadioButton("Project File", 2);
	newDialog->addRadioButton("Import PNG", 3);
	newDialog->addRadioButton("Empty TileSet", 4);
	
	newDialog->setRequiredCondition(2);
	newDialog->addSeperator();
	newDialog->addText(mGlobalSettings.mFile + " TileSet File must match Project BPP and TileSize X/Y");
	newDialog->addFile("Choose TileSet Project File", "TileSet File", ".bin", "NewFts", "", &mGlobalSettings.mNewTilePath);
	
	newDialog->addSeperator();
	newDialog->addText("TileSet BPP");
	newDialog->addRadioGroup(8, &mGlobalSettings.mGlobalTexParam.TexBPP);
	newDialog->addRadioButton("BPP: 8", 8);
	newDialog->addRadioButton("BPP: 4", 4, true);
	newDialog->addRadioButton("BPP: 2", 2, true);
	

	newDialog->setRequiredCondition(3);
	newDialog->addSeperator();
	newDialog->addText(mGlobalSettings.mFile + " Choose TileSet PNG File");
	newDialog->addFile("Choose TileSet PNG", "PNG File", ".png", "NewFtspng", "", &mGlobalSettings.mNewTilePath);
	
	newDialog->addSeperator();
	newDialog->addText("Imported Tile BPP");
	newDialog->addRadioGroup(8, &mGlobalSettings.mGlobalTexParam.TexBPP);
	newDialog->addRadioButton("BPP: 8", 8);
	newDialog->addRadioButton("BPP: 4", 4, true);

	newDialog->setRequiredCondition(4);
	newDialog->addSeperator();
	newDialog->addText("TileSet Initial Size");
	newDialog->addRadioGroup(4, &mGlobalSettings.mNewTileSize);
	newDialog->addRadioButton("4x4", 4);
	newDialog->addRadioButton("8x8", 8, true);
	newDialog->addRadioButton("16x16", 16, true);
	newDialog->addSeperator();
	newDialog->addText("Tile BPP");
	newDialog->addRadioGroup(8, &mGlobalSettings.mGlobalTexParam.TexBPP);
	newDialog->addRadioButton("BPP: 8", 8);
	newDialog->addRadioButton("BPP: 4", 4, true);
	newDialog->addRadioButton("BPP: 2", 2, true);

	newDialog->addConditionSetIf(0, -1);
	newDialog->setRequiredCondition(0);
	newDialog->addSeperator();
	newDialog->addText("Tile BPP");
	newDialog->addRadioGroup(8, &mGlobalSettings.mGlobalTexParam.TexBPP);
	newDialog->addRadioButton("BPP: 8", 8);
	newDialog->addRadioButton("BPP: 4", 4, true);
	newDialog->addRadioButton("BPP: 2", 2, true);

	
	newDialog->clearRequiredCondition();
	newDialog->addConditionRestore();

	newDialog->addSeperator();

	newDialog->addBoolCondition("Use Palette?", false, &mGlobalSettings.bProjectHasPalette, 5);
	
	newDialog->setRequiredCondition(5);
	newDialog->addText(mGlobalSettings.mFile + " Choose Palette Project File or Gimp Palette");
	newDialog->addFile("Choose Palette File", "Palette File", ".gpl,.bin", "NewPrPal", "", &mGlobalSettings.ProjectPalettePath);

	newDialog->clearRequiredCondition();
	newDialog->addConditionRestore();

	newDialog->addSeperator();

	newDialog->addText("Choose New Project Folder");
	newDialog->addFile("New Project Folder", "Folder", "", "NewPrf", "newfolder", &mGlobalSettings.ProjectPath, false, false, false, false, true, false);

	newDialog->addSeperator();

	newDialog->addButton("Create", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;

}

DTDialog* DTDialog::createTileMapCreateDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->createValues(2);

	newDialog->setLabel("Create TileMap");

	newDialog->setTarget(ESTATE_TILEMAPCREATE);

	newDialog->addText(mGlobalSettings.mFile + " Create New TileMap?");
	newDialog->addText(mGlobalSettings.mInfo + " Undo Stack will be cleared");

	newDialog->addSeperator();

	newDialog->addText("TileMap Width & Height");

	newDialog->addRadioGroup(32, &mGlobalSettings.mNewTileMapX);
	newDialog->addRadioButton("W: 32", 32);
	newDialog->addRadioButton("W: 64", 64, true);
	newDialog->addRadioButton("W: 128", 128, true);
	newDialog->addRadioButton("W: 256", 256, true);

	newDialog->addRadioGroup(32, &mGlobalSettings.mNewTileMapY);
	newDialog->addRadioButton("H: 32", 32);
	newDialog->addRadioButton("H: 64", 64, true);
	newDialog->addRadioButton("H: 128", 128, true);
	newDialog->addRadioButton("H: 256", 256, true);

	newDialog->addSeperator();

	newDialog->addIntMinMax("Initial Tile Value", 0, &mGlobalSettings.mNewTileMapOffset,newDialog->getValue(0), newDialog->getValue(1));

	newDialog->setRequiredCondition(1);
	newDialog->addSeperator();
	newDialog->addInt("Initial Palette Offset", 0, &mGlobalSettings.mNewTileMapPaletteOffset,0,15);

	newDialog->clearRequiredCondition();

	newDialog->addSeperator();
	newDialog->addButton("Create", SDLK_y);	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createSpriteCreateDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Create Sprite");

	newDialog->setTarget(ESTATE_SPRITECREATE);

	newDialog->addText(mGlobalSettings.mFile + " Create New Sprite?");
	
	newDialog->addSeperator();

	newDialog->addText("Sprite Width & Height");

	newDialog->addRadioGroup(32, &mGlobalSettings.mNewSpriteX);
	newDialog->addRadioButton("W: 8", 8);
	newDialog->addRadioButton("W: 16", 16, true);
	newDialog->addRadioButton("W: 32", 32, true);
	newDialog->addRadioButton("W: 64", 64, true);

	newDialog->addRadioGroup(32, &mGlobalSettings.mNewSpriteY);
	newDialog->addRadioButton("H: 8", 8);
	newDialog->addRadioButton("H: 16", 16, true);
	newDialog->addRadioButton("H: 32", 32, true);
	newDialog->addRadioButton("H: 64", 64, true);

	newDialog->addSeperator();
	newDialog->addText("Sprite BPP");
	newDialog->addRadioGroup(8, &mGlobalSettings.mNewSpriteBPP);
	newDialog->addRadioButton("BPP: 8", 8);
	newDialog->addRadioButton("BPP: 4", 4, true);
	

	newDialog->addSeperator();
	newDialog->addButton("Create", SDLK_y);	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createSpriteImportDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Import Sprite");

	newDialog->setTarget(ESTATE_SPRITEIMPORT);

	newDialog->addText(mGlobalSettings.mFile + " Import Sprite from Project file, PNG or RAW?");

	newDialog->addSeperator();

	newDialog->addRadioGroupCondition(0, &mGlobalSettings.mNewSpriteType);
	newDialog->addRadioButton("Project File", 0);
	newDialog->addRadioButton("PNG", 1);
	newDialog->addRadioButton("RAW", 2);

	newDialog->setRequiredCondition(0);
	newDialog->addSeperator();
	newDialog->addFile("Choose Sprite Project File", "Project File", ".bin", "SprImpPrjFile", "", &mGlobalSettings.mNewSpritePath);

	newDialog->setRequiredCondition(1);
	newDialog->addSeperator();
	newDialog->addText("Sprite Width & Height");

	newDialog->addRadioGroup(32, &mGlobalSettings.mNewSpriteX);
	newDialog->addRadioButton("W: 8", 8);
	newDialog->addRadioButton("W: 16", 16, true);
	newDialog->addRadioButton("W: 32", 32, true);
	newDialog->addRadioButton("W: 64", 64, true);

	newDialog->addRadioGroup(32, &mGlobalSettings.mNewSpriteY);
	newDialog->addRadioButton("H: 8", 8);
	newDialog->addRadioButton("H: 16", 16, true);
	newDialog->addRadioButton("H: 32", 32, true);
	newDialog->addRadioButton("H: 64", 64, true);

	newDialog->addSeperator();
	newDialog->addText("Sprite BPP");
	newDialog->addRadioGroup(8, &mGlobalSettings.mNewSpriteBPP);
	newDialog->addRadioButton("BPP: 8", 8);
	newDialog->addRadioButton("BPP: 4", 4, true);

	newDialog->addSeperator();
	newDialog->addFile("Choose Sprite PNG File", "PNG File", ".png", "SprImpPNGFile", "", &mGlobalSettings.mNewSpritePath);

	newDialog->setRequiredCondition(2);
	newDialog->addSeperator();
	newDialog->addText("Sprite Width & Height");

	newDialog->addRadioGroup(32, &mGlobalSettings.mNewSpriteX);
	newDialog->addRadioButton("W: 8", 8);
	newDialog->addRadioButton("W: 16", 16, true);
	newDialog->addRadioButton("W: 32", 32, true);
	newDialog->addRadioButton("W: 64", 64, true);

	newDialog->addRadioGroup(32, &mGlobalSettings.mNewSpriteY);
	newDialog->addRadioButton("H: 8", 8);
	newDialog->addRadioButton("H: 16", 16, true);
	newDialog->addRadioButton("H: 32", 32, true);
	newDialog->addRadioButton("H: 64", 64, true);

	newDialog->addSeperator();
	newDialog->addText("Sprite BPP");
	newDialog->addRadioGroup(8, &mGlobalSettings.mNewSpriteBPP);
	newDialog->addRadioButton("BPP: 8", 8);
	newDialog->addRadioButton("BPP: 4", 4, true);

	newDialog->addSeperator();
	newDialog->addFile("Choose Sprite RAW File", "RAW File", ".bin,.raw,.data", "SprImpRAWFile", "", &mGlobalSettings.mNewSpritePath);

	newDialog->clearRequiredCondition();
	newDialog->addConditionRestore();

	newDialog->addSeperator();

	newDialog->addButton("Import", SDLK_y);	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createTileMapImportDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Import TileMap");

	newDialog->setTarget(ESTATE_TILEMAPIMPORT);

	newDialog->createValues(2);

	newDialog->addText(mGlobalSettings.mFile + " Import TileMap from File?");
	newDialog->addText(mGlobalSettings.mInfo + " Undo Stack will be cleared!");

	newDialog->addSeperator();
	newDialog->addBoolCondition("Use Tile Offset", false, &mGlobalSettings.bNewTileMapOffset, 1);

	newDialog->setRequiredCondition(1);
	newDialog->addText(mGlobalSettings.mInfo + " Choose Tile Offset");
	newDialog->addIntMinMax("Tile Offset", 0, &mGlobalSettings.mNewTileMapOffset, newDialog->getValue(0), newDialog->getValue(1));	
	newDialog->clearRequiredCondition();
	
	newDialog->addSeperator();

	newDialog->addConditionRestore();

	newDialog->setRequiredCondition(8);
	newDialog->addText(mGlobalSettings.mInfo + " Choose Palette Offset");
	newDialog->addInt("Palette Offset", 0, &mGlobalSettings.mNewTileMapPaletteOffset, 0, 15);
	newDialog->addSeperator();
	newDialog->clearRequiredCondition();

	newDialog->addFile("Choose TileMap File", "Filename", ".bin", "ITileMap", "", &mGlobalSettings.mNewTileMapPath);

	newDialog->addSeperator();

	newDialog->addButton("Import", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;


}

DTDialog* DTDialog::createProjectCloseDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Close Project");

	newDialog->setTarget(ESTATE_PROJECTCLOSE);

	newDialog->addText(mGlobalSettings.mInfo + " Close Current Project?");
	newDialog->addText(mGlobalSettings.mFile + " Unsaved progress will be lost");

	newDialog->setRequiredCondition(ESTATE_PROJECTOPEN);
	newDialog->addIntTarget(ESTATE_PROJECTOPEN, &mGlobalSettings.mOpenCreateProjectState);

	newDialog->setRequiredCondition(ESTATE_PROJECTCREATE);
	newDialog->addIntTarget(ESTATE_PROJECTCREATE, &mGlobalSettings.mOpenCreateProjectState);

	newDialog->clearRequiredCondition();

	newDialog->addSeperator();

	newDialog->addButton("Close", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createPaletteImportDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->createLocalValues(1);
	newDialog->setLocalValue(0, 1);

	newDialog->setLabel("Import Palette Sub Range");

	newDialog->setTarget(ESTATE_PALETTEIMPORT);

	newDialog->addText(mGlobalSettings.mFile + " Import Palette Colors Sub Range?");
	
	newDialog->addSeperator();

	newDialog->addFile("Choose Palette File", "PAL File", ".gpl,.bin", "IMPalette", "", &mGlobalSettings.mNewPalettePath);

	newDialog->addSeperator();

	newDialog->addText(mGlobalSettings.mInfo + " Select Palette Colors Sub Range");

	newDialog->addInt("External Palette Start Color", 0, &mGlobalSettings.mNewPaletteStartExtern, 0, 255);

	newDialog->addIntActiveMinus("PALSTARTMIN", 256, newDialog->getIntValue("External Palette Start Color"));

	newDialog->addIntMinMax("External Palette Color Range", 256, &mGlobalSettings.mNewPaletteRangeExtern,newDialog->getLocalValue(0), newDialog->getIntActiveValue("PALSTARTMIN"));

	newDialog->addIntActiveMinus("PALRANGEMAX", 256, newDialog->getIntValue("External Palette Color Range"));

	newDialog->addIntMinMax("Internal Palette Start Color", 0, &mGlobalSettings.mNewPaletteStartIntern, newDialog->getValue(-1), newDialog->getIntActiveValue("PALRANGEMAX"));

	newDialog->addSeperator();

	newDialog->addButton("Import", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;


}

DTDialog* DTDialog::createPaletteUpdateDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Apply Palette Changes");

	newDialog->setTarget(ESTATE_PALETTEUPDATE);

	newDialog->addText(mGlobalSettings.mInfo + " Apply Palette Changes?");
	newDialog->addText(mGlobalSettings.mInfo + " Undo Stack will be cleared");

	newDialog->addSeperator();

	newDialog->addButton("Apply", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createProgramQuitDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Quit Program");

	newDialog->setTarget(ESTATE_PROGRAMQUIT);

	newDialog->addText(mGlobalSettings.mInfo + " Quit Program?");
	newDialog->addText(mGlobalSettings.mFile + " Unsaved progress will be lost");

	newDialog->addSeperator();

	newDialog->addButton("Quit", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

DTDialog* DTDialog::createProjectSaveDialog(){
	DTDialog* newDialog = new DTDialog();

	newDialog->setLabel("Save Project");

	newDialog->setTarget(ESTATE_PROJECTSAVE);

	newDialog->addText(mGlobalSettings.mFloppy + " Save Current Project?");
	newDialog->addText(mGlobalSettings.mInfo + " Existing Data on Disk will be Overwritten");

	newDialog->addSeperator();

	newDialog->addButton("Save", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	return newDialog;
}

/* DTDialog create end */ 

/* DTDCDialog create */ 

DTDCDialog* DTDCDialog::createProjectSaveAsDialog(){

	DTDCDialog* newDialog = new DTDCDialog();

	newDialog->setLabel("Save Project As");

	newDialog->setTarget(ESTATE_PROJECTSAVE);

	newDialog->addText(mGlobalSettings.mFloppy + " Save Current Project as Folder?");
	
	newDialog->addFileDefault("Choose Project Folder", "Folder", "", "PrjAsFold", &mGlobalSettings.ProjectPath, &mGlobalSettings.ProjectPath, false, false, false, true);

	newDialog->addSeperator();

	newDialog->addButton("Save As", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	newDialog->setConfirmLabel("Confirm Overwrite");

	newDialog->addConfirmText(mGlobalSettings.mFloppy + " Overwrite Folder on Disk?");
	newDialog->addConfirmText(mGlobalSettings.mInfo + " Existing Data will be Overwritten");

	newDialog->setConfirmButtons("Confirm","Cancel");

	newDialog->setConfirmConditionExists(newDialog->getFilePath("PrjAsFold"), true);

	return newDialog;

}

DTDCDialog* DTDCDialog::createPaletteExportDialog(){

	DTDCDialog* newDialog = new DTDCDialog();

	newDialog->createLocalValues(1);
	newDialog->setLocalValue(0, 1);

	newDialog->setLabel("Export Editor Palette");

	newDialog->setTarget(ESTATE_PALETTEEXPORT);

	newDialog->addText(mGlobalSettings.mFile + " Export Current Editor Palette?");
	newDialog->addText(mGlobalSettings.mInfo + " (Not Project Palette if <Apply Changes> has not been clicked)");
	
	newDialog->addSeperator();

	newDialog->addRadioGroupCondition(1, &mGlobalSettings.mExportPaletteType);
	newDialog->addRadioButton("GIMP Palette", 1);
	newDialog->addRadioButton("Project Palette", 2, true);
	
	newDialog->setRequiredCondition(1);
	newDialog->addSetFileExt("ExpPalFile", ".gpl");

	newDialog->setRequiredCondition(2);
	newDialog->addSetFileExt("ExpPalFile", ".bin");

	newDialog->clearRequiredCondition();
	newDialog->addConditionRestore();

	newDialog->addFile("Choose Palette File", "PAL File", ".gpl", "ExpPalFile", "", &mGlobalSettings.mExportPalettePath, false, false, false, false, false, false, true);

	newDialog->addSeperator();

	newDialog->addBoolCondition("Use Selection Range", false, &mGlobalSettings.bExportPaletteHasRange, 3);

	newDialog->setRequiredCondition(3);
	newDialog->addText(mGlobalSettings.mInfo + " Select Palette Colors Sub Range");

	newDialog->addConditionSetIfInt(newDialog->getIntValueRadio(0), 2, 4);

	newDialog->setRequiredCondition(4);
	newDialog->addText(mGlobalSettings.mInfo + " Project Palettes with less than 256 Colors\n  can not be loaded into TilemapEd");

	newDialog->clearRequiredCondition();
	newDialog->addConditionSetIfBool(newDialog->getBoolValue("Use Selection Range"), true, 3);

	newDialog->setRequiredCondition(3);
	newDialog->addInt("Editor Palette Start Color", 0, &mGlobalSettings.mExportPaletteStart, 0, 255);

	newDialog->addIntActiveMinus("EXPPALRANGEMAX", 256, newDialog->getIntValue("Editor Palette Start Color"));
	newDialog->addIntMinMax("Editor Palette Color Range", 256, &mGlobalSettings.mExportPaletteRange, newDialog->getLocalValue(0), newDialog->getIntActiveValue("EXPPALRANGEMAX"));

	newDialog->clearRequiredCondition();
	newDialog->addConditionRestore();

	newDialog->addSeperator();

	newDialog->addButton("Export", SDLK_y);
	
	newDialog->addButton("Cancel", SDLK_n, true);

	newDialog->setConfirmLabel("Confirm Overwrite");

	newDialog->addConfirmText(mGlobalSettings.mFile + " Overwrite File on Disk?");
	newDialog->addConfirmText(mGlobalSettings.mInfo + " Existing Data will be Overwritten");

	newDialog->setConfirmButtons("Confirm","Cancel");

	newDialog->setConfirmConditionExists(newDialog->getFilePath("ExpPalFile"), true);

	return newDialog;

}

/* DTDCDialog create end */ 

/* Dialog Template End */