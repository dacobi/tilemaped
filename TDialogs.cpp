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
				mGlobalSettings.CurrentEditor->activateSaveDialog();
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mFloppy + " Save As (F11)")).c_str())){
				mGlobalSettings.CurrentEditor->activateSaveAsDialog();
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Open")).c_str())){
				mGlobalSettings.CurrentEditor->activateOpenCreateDialog(ESTATE_PROJECTOPEN);
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Create")).c_str())){
				mGlobalSettings.CurrentEditor->activateOpenCreateDialog(ESTATE_PROJECTCREATE);
			}


			if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_MAP){
				if(ImGui::BeginMenu((std::string(mGlobalSettings.mFile + " Import")).c_str())){
					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Import Tile")).c_str())){
						mGlobalSettings.CurrentEditor->activateOpenTileDialog();		  			
					}
					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Import TileSet")).c_str())){
						mGlobalSettings.CurrentEditor->activateOpenTileSetDialog();		  			
					}
					if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Import TileMap")).c_str())){
						mGlobalSettings.CurrentEditor->activateOpenTileMapDialog();		  			
					}
					if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Import Sprite")).c_str())){
						mGlobalSettings.CurrentEditor->activateOpenSpriteDialog();		  			
					}

					ImGui::EndMenu();
				}
			}
			if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_SPRITE){
				if(ImGui::BeginMenu((std::string(mGlobalSettings.mFile + " Import")).c_str())){

					if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Import Sprite")).c_str())){
						mGlobalSettings.CurrentEditor->activateOpenSpriteDialog();		  			
					}

					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Import Sprite Frame")).c_str())){
						mGlobalSettings.CurrentEditor->activateOpenFrameDialog();		  			
					}

					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Import Sprite Frame(s)")).c_str())){
						mGlobalSettings.CurrentEditor->activateOpenFramesDialog();		  			
					}

					ImGui::EndMenu();
				}
			}

			ImGui::Separator();

			mGlobalSettings.settingsMenu();

			ImGui::Separator();

			if(ImGui::MenuItem((std::string(mGlobalSettings.mExit + " Quit")).c_str())){
				mGlobalSettings.CurrentEditor->activateQuitDialog();					
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			bIsMenuOpen = true;

			if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_MAP){
				
					if(ImGui::BeginMenu((std::string(mGlobalSettings.mWindow + " Tilemaps")).c_str())){
						int nMap=0;
						for(auto *cMap : mGlobalSettings.CurrentEditor->mTileMaps){
							if(ImGui::MenuItem(cMap->getMapSize().c_str())){				
								mGlobalSettings.CurrentEditor->switchTileMap(nMap);
							}
							nMap++;
						}
 
						if(ImGui::MenuItem((std::string(mGlobalSettings.mFile+ " Create Tilemap")).c_str())){				
							mGlobalSettings.CurrentEditor->activateNewTileMapDialog();
						}		

						if(mGlobalSettings.CurrentEditor->mTileMaps.size() > 1){
							if(ImGui::MenuItem((std::string(mGlobalSettings.mFile+ " Remove Tilemap")).c_str())){				
								mGlobalSettings.CurrentEditor->activateRemoveTileMapDialog();
							}	
						}
						
						ImGui::EndMenu();
					}
				//} else {
				//	if(ImGui::MenuItem((std::string(mGlobalSettings.mWindow + " Tilemap")).c_str())){				
				//		mGlobalSettings.CurrentEditor->setMode(EMODE_MAP);
				//	}
				//}
			} else {
				if(ImGui::MenuItem((std::string(mGlobalSettings.mWindow + " Tilemap")).c_str())){				
					mGlobalSettings.CurrentEditor->setMode(EMODE_MAP);
				}
			}

			
			if(ImGui::MenuItem((std::string(mGlobalSettings.mWindow + " Tile")).c_str())){
				//mGlobalSettings.CurrentEditor->mCurMode = EMODE_TILE;
				mGlobalSettings.CurrentEditor->setMode(EMODE_TILE);
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mWindow + " Palette")).c_str())){
				//mGlobalSettings.CurrentEditor->activatePaletteEdit();
				mGlobalSettings.CurrentEditor->setMode(EMODE_PALED);
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mWindow + " TileSet")).c_str())){
				//mGlobalSettings.CurrentEditor->activatePaletteEdit();
				mGlobalSettings.CurrentEditor->setMode(EMODE_TILESET);
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mWindow + " Selection")).c_str())){
				//mGlobalSettings.CurrentEditor->activatePaletteEdit();
				mGlobalSettings.CurrentEditor->setMode(EMODE_SELEDIT);
			}

			if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_SPRITE){
				
					if(ImGui::BeginMenu((std::string(mGlobalSettings.mWindow + " Sprites")).c_str())){
						int nSprite=0;
						std::stringstream fconv;
						std::string cSFrame;
						std::string snSprite;
						std::string cSPriteStart;
						bool bCFrame;
						bool bCSprite;												
						for(auto *cSprite : mGlobalSettings.CurrentEditor->mSprites){							
							fconv << nSprite << std::endl;									
							fconv >> snSprite;
							bCSprite = (cSprite == mGlobalSettings.CurrentEditor->mSprite);

							if(bCSprite){
								cSPriteStart = mGlobalSettings.mBall + " Sprite ";
							} else {
								cSPriteStart = "  Sprite ";
							}

							if(ImGui::BeginMenu(std::string(cSPriteStart + snSprite + ": "+ cSprite->getSpriteSize()).c_str())){
								if(ImGui::IsItemClicked()){
									mGlobalSettings.CurrentEditor->switchSprite(nSprite);								
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
										mGlobalSettings.CurrentEditor->switchSprite(nSprite, nf);								
									}								
								}								
								ImGui::EndMenu();
							}
							nSprite++;
						}
 
						if(ImGui::MenuItem((std::string(mGlobalSettings.mFile+ " Create Sprite")).c_str())){				
							mGlobalSettings.CurrentEditor->activateNewSpriteDialog();
						}		

						
						if(ImGui::MenuItem((std::string(mGlobalSettings.mFile+ " Remove Sprite")).c_str())){				
							mGlobalSettings.CurrentEditor->activateRemoveSpriteDialog();
						}	

						/*
						if(ImGui::BeginMenu("Sprite Create")){
						
							//if(mGlobalSettings.CurrentEditor->mSprites.size() > 0){
							bool bFrameCopyPosible = false;

							for(auto *cSprt : mGlobalSettings.CurrentEditor->mSprites){
								if(cSprt != mGlobalSettings.CurrentEditor->mSprite){
									if((cSprt->mTexParam.TileSizeX == mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeX) && (cSprt->mTexParam.TileSizeY == mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeY) && (cSprt->mTexParam.TileSetBPP == mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSetBPP)){
										bFrameCopyPosible = true;
									}
								}
							}

							if(ImGui::BeginMenu((std::string(mGlobalSettings.mFile + " Copy Frame")).c_str(), bFrameCopyPosible)){
									int fSCount = 0;
									std::string sfSCount;
									std::string cCSFrame;
									for(auto *cSprt : mGlobalSettings.CurrentEditor->mSprites){
										if(cSprt != mGlobalSettings.CurrentEditor->mSprite){
											if((cSprt->mTexParam.TileSizeX == mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeX) && (cSprt->mTexParam.TileSizeY == mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeY) && (cSprt->mTexParam.TileSetBPP == mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSetBPP)){
											
												fconv << fSCount << std::endl;
												fconv >> sfSCount;

												if(ImGui::BeginMenu(std::string("Sprite " + sfSCount + ": " +  cSprt->getSpriteSize()).c_str())){

													for(int nf = 0; nf < cSprt->mFrames.size(); nf++){
														fconv << nf << std::endl;									
														fconv >> cCSFrame;
																								
														if(ImGui::MenuItem((std::string("Frame " + cCSFrame).c_str()))){																					
															mGlobalSettings.CurrentEditor->createNewFrameCopy(cSprt->mFrames[nf]);
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
										mGlobalSettings.CurrentEditor->createNewSpriteCopy(mGlobalSettings.CurrentEditor->mSprite);
										mGlobalSettings.CurrentEditor->showMessage("Sprite Copied Successfully");
								}

								if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Scaled Copy")).c_str())){
										mGlobalSettings.CurrentEditor->activateNewScaledSpriteDialog();
								}

								bool bAllowUpscale = false;
								int cAllowedScale = 2;

								if( (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeX < 64) && (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeY < 64) ){
									bAllowUpscale = true;

									if( (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeX < 32) && (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeY < 32) ){
										cAllowedScale = 4;
									}

									if( (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeX < 16) && (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeY < 16) ){
										cAllowedScale = 8;
									}																		
								}

								if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " UpScaled Copy")).c_str(), NULL, false, bAllowUpscale)){
									mGlobalSettings.CurrentEditor->activateNewUpscaledSpriteDialog(cAllowedScale);
								}


								bool bAllowDownscale = false;
								int cAllowedDownScale = 2;

								if( (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeX > 8) && (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeY > 8) ){
									bAllowDownscale = true;

									if( (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeX > 16) && (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeY > 16) ){
										cAllowedDownScale = 4;
									}

									if( (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeX > 32) && (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeY > 32) ){
										cAllowedDownScale = 8;
									}																		
								}

								if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " DownScaled Copy")).c_str(), NULL, false, bAllowDownscale)){
									mGlobalSettings.CurrentEditor->activateNewDownscaledSpriteDialog(cAllowedDownScale);
								}

								bool bAllowRange = false;

								if( mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeX == mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeY){
									bAllowRange = true;
								}


								if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Rotation Range")).c_str(), NULL, false, bAllowRange)){
									mGlobalSettings.CurrentEditor->activateSpriteRotationRangeDialog();
								}

								if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Frame Rotations")).c_str(), NULL, false, bAllowRange)){
									mGlobalSettings.CurrentEditor->activateSpriteRotationsDialog();
								}
															
								ImGui::EndMenu();		
							}
							*/
						//}
						
						ImGui::EndMenu();
					}

					if(ImGui::MenuItem((std::string(mGlobalSettings.mWindow + " Sprite Frames")).c_str(), NULL, &mGlobalSettings.CurrentEditor->mSprite->bShowSpriteFrames)){
						
					}

				//} else {
				//	if(ImGui::MenuItem((std::string(mGlobalSettings.mWindow + " Tilemap")).c_str())){				
				//		mGlobalSettings.CurrentEditor->setMode(EMODE_MAP);
				//	}
				//}
			} else {
				if(ImGui::MenuItem((std::string(mGlobalSettings.mWindow + " Sprite")).c_str())){				
					if(mGlobalSettings.CurrentEditor->mSprites.size()){
						mGlobalSettings.CurrentEditor->setMode(EMODE_SPRITE);
					} else {
						mGlobalSettings.CurrentEditor->activateNewSpriteDialog();
					}
					
					
				}
			}

			if(ImGui::MenuItem((std::string(mGlobalSettings.mInfo + " Help Dialog (F1)")).c_str())){
				mGlobalSettings.CurrentEditor->activateHelpDialog();
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mInfo + " Project Info (F2)")).c_str() ,NULL,  &mGlobalSettings.bShowProjectInfo)){
				//mGlobalSettings.CurrentEditor->activateProjectInfo();
			}
			
			
			if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_MAP){
				if(mGlobalSettings.mGlobalTexParam.TileSetBPP < 0x8){
					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Palette Offset")).c_str(), NULL, &mGlobalSettings.bShowPaletteOffset) ){ 
					}
				}
				if(ImGui::MenuItem("Brushes (F8)", NULL , &mGlobalSettings.CurrentEditor->bShowBrushesTile)){										
				}
				if(ImGui::MenuItem("Selected Tile Type (S)", NULL , &mGlobalSettings.bShowTypeSelection)){										
				}
				if(ImGui::MenuItem("Selected Tile (T)", NULL , &mGlobalSettings.bShowSelectedTile)){										
				}

				
			}
			if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_TILE){
				if(ImGui::MenuItem("Brushes (F8)", NULL , &mGlobalSettings.CurrentEditor->bShowBrushesPixel)){										
				}
				if(ImGui::MenuItem("Selected Color (S)", NULL , &mGlobalSettings.bShowPixelType)){										
				}
				if(ImGui::MenuItem("Pixel Grid (P)", NULL , &mGlobalSettings.bShowPixelGrid)){										
				}
			}
			if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_SPRITE){
				if(ImGui::MenuItem("Brushes (F8)", NULL , &mGlobalSettings.CurrentEditor->mSprite->bShowBrushesPixel)){										
				}
				if(ImGui::MenuItem("Selected Color (S)", NULL , &mGlobalSettings.bShowPixelTypeSprite)){										
				}
				if(ImGui::MenuItem("Pixel Grid (P)", NULL , &mGlobalSettings.bShowPixelGridSprite)){										
				}
			}

			if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_TILESET){	
				if(ImGui::MenuItem("Brushes (F8)", NULL , &mGlobalSettings.CurrentEditor->bShowBrushesPixelTileSet)){                                                                         
                }			
				if(ImGui::MenuItem("Tile Grid (T)", NULL , &mGlobalSettings.bShowTileGrid)){										
				}
				if(ImGui::MenuItem("Pixel Grid (P)", NULL , &mGlobalSettings.bShowTilePixelGrid)){										
				}
				if(ImGui::MenuItem("Selected Color (S)", NULL , &mGlobalSettings.bShowPixelType)){										
				}
			}

			if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_SELEDIT){	
				if(ImGui::MenuItem("Brushes (F8)", NULL , &mGlobalSettings.CurrentEditor->bShowBrushesPixelSelEdit)){                                                                         
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

			if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_SPRITE){				
				if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " New Sprite Frame (F3)")).c_str())){
					mGlobalSettings.CurrentEditor->createNewFrame();
				}
				if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Copy Sprite Frame (F4)")).c_str())){
					mGlobalSettings.CurrentEditor->createNewFrameCopy(mGlobalSettings.CurrentEditor->mSprite->mFrame);
				}
				if(mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeX == mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeY){					
					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Rotate Frame Left (F5)")).c_str())){
						mGlobalSettings.CurrentEditor->rotateFrameLeft();
					}
					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Rotate Frame Right (F6)")).c_str())){
						mGlobalSettings.CurrentEditor->rotateFrameRight();
					}
					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Rotate Selected Frame")).c_str())){
						mGlobalSettings.CurrentEditor->activateRotateFrameDialog();
					}
					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Scale Selected Frame")).c_str())){
						mGlobalSettings.CurrentEditor->activateScaleFrameDialog();
					}
				}				
				if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Remove Selected Frame (DEL)")).c_str())){
					if(mGlobalSettings.CurrentEditor->mSprite->mFrames.size() > 1){
						mGlobalSettings.CurrentEditor->activateRemoveFrame();
					} else {
						mGlobalSettings.CurrentEditor->activateRemoveSpriteDialog();
					}
				}
				

				if(mGlobalSettings.mUseTextureFiltering > 1){
					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Filter Selected Frame")).c_str())){
						mGlobalSettings.CurrentEditor->filterFrame();
					}
				}

				/*  */

				if(ImGui::BeginMenu(std::string(mGlobalSettings.mTool + " Sprite Tools").c_str())){
						
							//if(mGlobalSettings.CurrentEditor->mSprites.size() > 0){
							bool bFrameCopyPosible = false;

							for(auto *cSprt : mGlobalSettings.CurrentEditor->mSprites){
								if(cSprt != mGlobalSettings.CurrentEditor->mSprite){
									if((cSprt->mTexParam.TileSizeX == mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeX) && (cSprt->mTexParam.TileSizeY == mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeY) && (cSprt->mTexParam.TileSetBPP == mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSetBPP)){
										bFrameCopyPosible = true;
									}
								}
							}

							if(ImGui::BeginMenu((std::string(mGlobalSettings.mImage + " Copy Frame")).c_str(), bFrameCopyPosible)){
									int fSCount = 0;
									std::string sfSCount;
									std::string cCSFrame;
									std::stringstream fconv;
									for(auto *cSprt : mGlobalSettings.CurrentEditor->mSprites){
										if(cSprt != mGlobalSettings.CurrentEditor->mSprite){
											if((cSprt->mTexParam.TileSizeX == mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeX) && (cSprt->mTexParam.TileSizeY == mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeY) && (cSprt->mTexParam.TileSetBPP == mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSetBPP)){
											
												fconv << fSCount << std::endl;
												fconv >> sfSCount;

												if(ImGui::BeginMenu(std::string("Sprite " + sfSCount + ": " +  cSprt->getSpriteSize()).c_str())){

													for(int nf = 0; nf < cSprt->mFrames.size(); nf++){
														fconv << nf << std::endl;									
														fconv >> cCSFrame;
																								
														if(ImGui::MenuItem((std::string("Frame " + cCSFrame).c_str()))){																					
															mGlobalSettings.CurrentEditor->createNewFrameCopy(cSprt->mFrames[nf]);
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
										mGlobalSettings.CurrentEditor->createNewSpriteCopy(mGlobalSettings.CurrentEditor->mSprite);
										mGlobalSettings.CurrentEditor->showMessage("Sprite Copied Successfully");
								}

								if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Scaled Copy")).c_str())){
										mGlobalSettings.CurrentEditor->activateNewScaledSpriteDialog();
								}

								bool bAllowUpscale = false;
								int cAllowedScale = 2;

								if( (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeX < 64) && (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeY < 64) ){
									bAllowUpscale = true;

									if( (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeX < 32) && (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeY < 32) ){
										cAllowedScale = 4;
									}

									if( (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeX < 16) && (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeY < 16) ){
										cAllowedScale = 8;
									}																		
								}

								if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Upscaled Copy")).c_str(), NULL, false, bAllowUpscale)){
									mGlobalSettings.CurrentEditor->activateNewUpscaledSpriteDialog(cAllowedScale);
								}


								bool bAllowDownscale = false;
								int cAllowedDownScale = 2;

								if( (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeX > 8) && (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeY > 8) ){
									bAllowDownscale = true;

									if( (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeX > 16) && (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeY > 16) ){
										cAllowedDownScale = 4;
									}

									if( (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeX > 32) && (mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeY > 32) ){
										cAllowedDownScale = 8;
									}																		
								}

								if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Downscaled Copy")).c_str(), NULL, false, bAllowDownscale)){
									mGlobalSettings.CurrentEditor->activateNewDownscaledSpriteDialog(cAllowedDownScale);
								}

								bool bAllowRange = false;

								if( mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeX == mGlobalSettings.CurrentEditor->mSprite->mTexParam.TileSizeY){
									bAllowRange = true;
								}


								if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Rotation Range")).c_str(), NULL, false, bAllowRange)){
									mGlobalSettings.CurrentEditor->activateSpriteRotationRangeDialog();
								}

								if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Frame Rotations")).c_str(), NULL, false, bAllowRange)){
									mGlobalSettings.CurrentEditor->activateSpriteRotationsDialog();
								}
															
								ImGui::EndMenu();		
							}

				/*  */	

				if(ImGui::BeginMenu((std::string(mGlobalSettings.mGear + " Sprite Frame Reordering").c_str()))){										
					if(ImGui::MenuItem("Warn Before Frame Removal", NULL, &mGlobalSettings.bSpriteWarnBeforeDelete)){
						
					}
					ImGui::EndMenu();
				}
				if(ImGui::BeginMenu((std::string(mGlobalSettings.mGear + " Sprite Edit").c_str()))){					
					if(ImGui::SliderInt("Frame Scale", &mGlobalSettings.CurrentEditor->mSprite->mTexParam.TilePixelSize, TSprite::MinScale, TSprite::MaxScale,"%d", ImGuiSliderFlags_NoInput)){						
						mGlobalSettings.CurrentEditor->setSpriteBrushes();						
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

			if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_MAP){
				if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Flip Tile (F) (X) (Y)")).c_str())){
					mGlobalSettings.CurrentEditor->flipSelectedTile(0);
				}
				if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " New Tile (F3)")).c_str())){
					mGlobalSettings.CurrentEditor->createNewTile();
				}				
				if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Copy Tile (F4)")).c_str())){
					mGlobalSettings.CurrentEditor->createNewTileCopy(mGlobalSettings.CurrentEditor->mTileSelectedTile);
				}

				if(mGlobalSettings.mGlobalTexParam.TileSizeX == mGlobalSettings.mGlobalTexParam.TileSizeY){					
					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Rotate Tile Left (F5)")).c_str())){
						mGlobalSettings.CurrentEditor->rotateTileLeft();
					}
					if(ImGui::MenuItem((std::string(mGlobalSettings.mImage + " Rotate Tile Right (F6)")).c_str())){
						mGlobalSettings.CurrentEditor->rotateTile();
					}
				}

				if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Remove Selected Tile (DEL)")).c_str())){
					mGlobalSettings.CurrentEditor->activateDropUnusedTile();
				}

				if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Remove Unused Tiles")).c_str())){
					mGlobalSettings.CurrentEditor->activateDropUnusedTiles();
				}
				
				/*
				if(ImGui::BeginMenu((std::string(mGlobalSettings.mImage + " Texture Filtering")).c_str())){
					ImGui::RadioButton("None", &mGlobalSettings.mUseTextureFiltering, 0);
					ImGui::RadioButton("Some", &mGlobalSettings.mUseTextureFiltering, 1);
					ImGui::RadioButton("All", &mGlobalSettings.mUseTextureFiltering, 2);

					ImGui::EndMenu();
				}*/				

				if(ImGui::BeginMenu((std::string(mGlobalSettings.mFile + " CollisionMap")).c_str())){
				
					if(mGlobalSettings.CurrentEditor->mTileMap->bHasCollisionMap){
						if(ImGui::MenuItem((std::string(mGlobalSettings.mFile+ " Edit")).c_str())){				
								mGlobalSettings.CurrentEditor->activateColMapDialog();
							}
						if(ImGui::MenuItem((std::string(mGlobalSettings.mFile+ " Remove")).c_str())){											
								mGlobalSettings.CurrentEditor->removeColMapDialog();
							}
					} else {
						if(ImGui::MenuItem((std::string(mGlobalSettings.mFile+ " Create")).c_str())){				
								mGlobalSettings.CurrentEditor->activateColMapDialog(true);
						}
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
			}

			if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_TILESET){
				if(ImGui::BeginMenu((std::string(mGlobalSettings.mGear + " TileSet Edit").c_str()))){
					if(ImGui::SliderInt("Grid Width", &mGlobalSettings.mTileSetEditWidth, TileSet::MinGrid, TileSet::MaxGrid,"%d", ImGuiSliderFlags_NoInput)){
						mGlobalSettings.CurrentEditor->mTileSet.bUpdateEditSelection = true;
					}
					if(ImGui::SliderInt("Tile Scale", &mGlobalSettings.mTileSetEditScale, TileSet::MinScale, TileSet::MaxScale,"%d", ImGuiSliderFlags_NoInput)){
						mGlobalSettings.CurrentEditor->mTileSet.bUpdateEditSelectionScale = true;
					}
					ImGui::EndMenu();
				}
				
			}

			if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_SELEDIT){
				if(ImGui::BeginMenu((std::string(mGlobalSettings.mGear + " Selection Edit").c_str()))){					
					if(ImGui::SliderInt("Tile Scale", &mGlobalSettings.mSelectionEditScale, TSelectionEditor::MinScale, TSelectionEditor::MaxScale,"%d", ImGuiSliderFlags_NoInput)){
						mGlobalSettings.CurrentEditor->mSelEdit.bUpdateEditSelectionScale = true;
					}
					ImGui::EndMenu();
				}
				
			}

			if(mGlobalSettings.CurrentEditor->mCurMode == EMODE_TILE){
				if(ImGui::BeginMenu((std::string(mGlobalSettings.mGear + " Tile Edit").c_str()))){					
					if(ImGui::SliderInt("Tile Scale", &mGlobalSettings.mGlobalTexParam.TilePixelSize, Tile::MinScale, Tile::MaxScale,"%d", ImGuiSliderFlags_NoInput)){						
						mGlobalSettings.CurrentEditor->mBrushesPixel.setBrushDeltas(mGlobalSettings.mGlobalTexParam.TilePixelSize, mGlobalSettings.mGlobalTexParam.TilePixelSize, &mGlobalSettings.mGlobalTexParam.mTileEdScale, mGlobalSettings.mGlobalTexParam.mTileEdScale, &mGlobalSettings.mGlobalTexParam);
					}
											
					ImGui::EndMenu();
				}
			}

			if((mGlobalSettings.CurrentEditor->mCurMode != EMODE_PALED) && (mGlobalSettings.CurrentEditor->mCurMode != EMODE_SELEDIT)){
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
		
			if(ImGui::MenuItem("Undo (U) (CTRL + Z)")){
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
		mGlobalSettings.mEditorState = ESTATE_TILEDELETEALL;
	}
	if(mKey == SDLK_n){
		bInputIsCancel=true;
	}
}

void RTSDialog::init(){
	mDialogTextMain = mGlobalSettings.mInfo +" Remove Selected Tile? Undo Stack will be cleared."; 
	mDialogTextTitle = "Remove Selected Tile";
	mDialogButtonAccept = "Remove";
	mDialogButtonCancel = "Cancel";
}

void RTSDialog::recieveInput(int mKey){
	
	if(mKey == SDLK_y){
		bInputIsAccept=true;		
		mGlobalSettings.mEditorState = ESTATE_TILEDELETE;
	}
	if(mKey == SDLK_n){
		mGlobalSettings.bTileSetWarnBeforeDelete = true;
		bInputIsCancel=true;
	}
}

int RTSDialog::render(){
    
	Dialog::render();

	ImGui::Begin(mDialogTextTitle.c_str(),NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);                         
    		
		ImGui::Text("%s", mDialogTextMain.c_str()); 

		ImGui::Checkbox("Show this warning", &mGlobalSettings.bTileSetWarnBeforeDelete);

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

/* Rotate Frame */

void RTSFDialog::init(){
	mDialogTextMain = mGlobalSettings.mImage +" Rotate Selected Frame by angle in degrees"; 
	mDialogTextTitle = "Rotate Selected Frame";	
	frotateangle = 0;
}

void RTSFDialog::recieveInput(int mKey){
	
	if(mKey == SDLK_y){
		bInputIsAccept=true;		
		mGlobalSettings.mEditorState = ESTATE_FRAMEROTATE;
		mGlobalSettings.mRotateFrameAngle = frotateangle;
	}
	if(mKey == SDLK_n){
		frotateangle = 0;
		bInputIsCancel=true;
	}
}

int RTSFDialog::render(){
    
	Dialog::render();

	ImGui::Begin(mDialogTextTitle.c_str(),NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);                         
    		
		ImGui::Text("%s", mDialogTextMain.c_str()); 

		ImGui::SliderFloat("Angle", &frotateangle, 0, 360.0,"%.1f", ImGuiSliderFlags_AlwaysClamp);

        if ( ImGui::Button("Rotate")){ 
			recieveInput(SDLK_y);				
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")){ 
			recieveInput(SDLK_n);				
		}
			            
    ImGui::End();

	return 0;
}

void RTSFDialog::cancel(){
	Dialog::cancel();
	frotateangle = 0;	
}



/* End */

/* Frame Scale */



void SSFDialog::init(){
	mDialogTextMain = mGlobalSettings.mImage +" Scale Selected Frame by float factor"; 
	mDialogTextTitle = "Scale Selected Frame";	
	fscalefactor = 1;
}

void SSFDialog::recieveInput(int mKey){
	
	if(mKey == SDLK_y){
		bInputIsAccept=true;		
		mGlobalSettings.mEditorState = ESTATE_FRAMESCALE;
		mGlobalSettings.mScaleFrameFactor = fscalefactor;
	}
	if(mKey == SDLK_n){
		fscalefactor = 1;
		bInputIsCancel=true;
	}
}

int SSFDialog::render(){
    
	Dialog::render();

	ImGui::Begin(mDialogTextTitle.c_str(),NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);                         
    		
		ImGui::Text("%s", mDialogTextMain.c_str()); 

		ImGui::SliderFloat("Scale Factor", &fscalefactor, 0.5, 2.0,"%.2f", ImGuiSliderFlags_AlwaysClamp);

        if ( ImGui::Button("Scale")){ 
			recieveInput(SDLK_y);				
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")){ 
			recieveInput(SDLK_n);				
		}
			            
    ImGui::End();

	return 0;
}

void SSFDialog::cancel(){
	Dialog::cancel();
	fscalefactor = 1;	
}


/* End */


void RSFDialog::init(){
	mDialogTextMain = mGlobalSettings.mInfo +" Remove Selected Frame? Sprite Undo Stack will be cleared."; 
	mDialogTextTitle = "Remove Selected Frame";
	mDialogButtonAccept = "Remove";
	mDialogButtonCancel = "Cancel";
}

void RSFDialog::recieveInput(int mKey){
	
	if(mKey == SDLK_y){
		bInputIsAccept=true;		
		mGlobalSettings.mEditorState = ESTATE_FRAMEDELETE;
	}
	if(mKey == SDLK_n){
		mGlobalSettings.bSpriteWarnBeforeDelete = true;
		bInputIsCancel=true;
	}
}

int RSFDialog::render(){
    
	Dialog::render();

	ImGui::Begin(mDialogTextTitle.c_str(),NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);                         
    		
		ImGui::Text("%s", mDialogTextMain.c_str()); 

		ImGui::Checkbox("Show this warning", &mGlobalSettings.bSpriteWarnBeforeDelete);

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



void RTMDialog::init(){
	mDialogTextMain = mGlobalSettings.mFile +" Remove TileMap? Undo Stack will be cleared";
	mDialogTextTitle = "Remove TileMap";
	mDialogButtonAccept = "Remove";
	mDialogButtonCancel = "Cancel";
}

void RTMDialog::recieveInput(int mKey){
	
	if(mKey == SDLK_y){
		bInputIsAccept=true;		
		mGlobalSettings.mEditorState = ESTATE_TILEMAPDELETE;
	}
	if(mKey == SDLK_n){
		bInputIsCancel=true;
	}
}

void RSDialog::init(){
	mDialogTextMain = mGlobalSettings.mFile +" Remove Sprite? Can not be undone";
	mDialogTextTitle = "Remove Sprite";
	mDialogButtonAccept = "Remove";
	mDialogButtonCancel = "Cancel";
}

void RSDialog::recieveInput(int mKey){
	
	if(mKey == SDLK_y){
		bInputIsAccept=true;		
		mGlobalSettings.mEditorState = ESTATE_SPRITEDELETE;
	}
	if(mKey == SDLK_n){
		bInputIsCancel=true;
	}
}


void RCMDialog::init(){
	mDialogTextMain = mGlobalSettings.mFile +" Remove CollisionMap for current TileMap?";
	mDialogTextTitle = "Remove CollisionMap";
	mDialogButtonAccept = "Remove";
	mDialogButtonCancel = "Cancel";
}

void RCMDialog::recieveInput(int mKey){
	
	if(mKey == SDLK_y){
		bInputIsAccept=true;		
		mGlobalSettings.mEditorState = ESTATE_COLMAPREMOVE;
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
		mGlobalSettings.mEditorState = ESTATE_PALETTEUPDATE;
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
		mGlobalSettings.mEditorState = ESTATE_PROJECTSAVE;
	}
	if(mKey == SDLK_n){
		bInputIsCancel=true;
	}
}

int SDialog::render(){
    
	Dialog::render();

	ImGui::Begin(mDialogTextTitle.c_str(),NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);                         
    		
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


	ImGui::Begin(mDialogTextTitle.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);                         
    		
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
			mGlobalSettings.mEditorState = ESTATE_PROJECTSAVE;

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
					mGlobalSettings.mEditorState = ESTATE_PROJECTSAVE;

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

	ImGui::Begin("Open Project",NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);                         
		
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
			if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Open")).c_str())){
				bSubDialogActive = true;
				bSubDialogIsOpen = true;
				bDialogIsWatingForText = true;				
			}
			if(ImGui::MenuItem((std::string(mGlobalSettings.mFile + " Create")).c_str())){
				bSubDialogActive = true;
				bSubDialogIsCreate = true;
				bDialogIsWatingForText = true;				
			}

			ImGui::Separator();

			mGlobalSettings.settingsMenu();

			ImGui::Separator();


			if(ImGui::MenuItem((std::string(mGlobalSettings.mExit + " Quit")).c_str())){
				mGlobalSettings.bRunningOCD = false;
			}

			ImGui::EndMenu();
		}

		std::string mDialogTextWindow = "TilemapEd";

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
				mGlobalSettings.mEditorState = ESTATE_PROJECTOPEN;

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
				mGlobalSettings.mGlobalTexParam.TileSetBPP = mCreateProject.tbpp;
				mGlobalSettings.mGlobalTexParam.TileSizeX = mCreateProject.tilex;
				mGlobalSettings.mGlobalTexParam.TileSizeY = mCreateProject.tiley;
				mGlobalSettings.ProjectPath = mCreateProject.mReadPath.mDialogTextMain;

				if(mCreateProject.bHasTileSet){
					switch (mCreateProject.tilesettype){
						case 0:
							mGlobalSettings.mNewTilePath = mCreateProject.mReadTileSet.mDialogTextMain;	
						break;
						case 1:
							mGlobalSettings.mNewTilePath = mCreateProject.mReadTileSet.mDialogTextMain;	
						break;
						case 2:
							mGlobalSettings.mNewTileSize = mCreateProject.tilesetsize;
						break;				
						default:
						break;
					}
				}

				if(mCreateProject.bHasPalette){
					if(mCreateProject.mReadPal.bInputIsAccepted){
						if(mGlobalSettings.testPaletteFile(mCreateProject.mReadPal.mDialogTextMain)){
							mGlobalSettings.bProjectHasPalette = true;
							mGlobalSettings.ProjectPalettePath = mCreateProject.mReadPal.mDialogTextMain;
						}
					}
				}
				
				mGlobalSettings.mEditorState = ESTATE_PROJECTCREATE;

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


void CTMDialog::init(){
	mDialogTextMain = mGlobalSettings.mFile + " Create New TileMap";	
}

void CTMDialog::cancel(){
		Dialog::cancel();
		tmapx = 32;
		tmapy = 32;
		toffset = 1;
		mPaletteOffset = 0;
}

void CSDialog::init(){
	mDialogTextMain = mGlobalSettings.mFile + " Create New Sprite";	
}

void CSDialog::cancel(){
		Dialog::cancel();
		spritex=32;
		spritey=32;
		spritebpp=8;				
}


void CPDialog::init(){
	mDialogTextMain = mGlobalSettings.mFile + " Create New Project";	
		
	mReadPath.mDialogTextMain = "newfolder";
	mReadPal.mDialogTextMain = "";
	mReadTileSet.mDialogTextMain = "";

	mReadPath.mInputLabel = "Project Folder";
	mReadPal.mInputLabel = "Palette File";
	mReadTileSet.mInputLabel = "TileSet File";

	mReadPath.bMustNotExist = true;
	mReadPath.bAutoComplete = true;

	mReadPal.bMustBeFile = true;
	mReadPal.bAutoComplete = true;

	mReadTileSet.bMustBeFile = true;
	mReadTileSet.bAutoComplete = true;

	mReadPath.bInputIsAccepted = true;
	mReadPal.bInputIsAccepted = true;


	mReadPath.init();
	mReadPal.init();
	mReadTileSet.init();

	mActiveInput = &mReadPath;
	mActiveInput->bIsInputActive = true;

}

int CTMDialog::render(){
	
	Dialog::render();


	ImGui::Begin(mDialogTextMain.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);                         
    		
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

		int mMax = mGlobalSettings.CurrentEditor->mTileSet.TTiles.size();
		ImGui::SliderInt("Initial Tile Value", &toffset, 1, mMax,"%d", ImGuiSliderFlags_AlwaysClamp);

		if(mGlobalSettings.mGlobalTexParam.TileSetBPP < 0x8){
			ImGui::SliderInt("Initial Palette Offset", &mPaletteOffset, 0, 15,"%d", ImGuiSliderFlags_AlwaysClamp);
		}

		ImGui::Text("Undo Stack will be cleared");  

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

void CTMDialog::recieveInput(int mKey){
	
	if(mKey == SDLK_y){		
		bInputIsAccept=true;	
		bDialogIsWatingForText = false;				
		mGlobalSettings.mEditorState = ESTATE_TILEMAPCREATE;

		mGlobalSettings.mNewTileMapOffset = toffset - 1;
		mGlobalSettings.mNewTileMapX = tmapx;
		mGlobalSettings.mNewTileMapY = tmapy;
		mGlobalSettings.mNewTileMapPaletteOffset = mPaletteOffset;
	}		
	
	if(mKey == SDLK_n){
		bInputIsCancel=true;
	}			
}

int CSDialog::render(){
	
	Dialog::render();


	ImGui::Begin(mDialogTextMain.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);                         
    		
		ImGui::Text("Sprite Width & Height");  
			
		ImGui::RadioButton("W: 8", &spritex, 8);
		ImGui::SameLine();
		ImGui::RadioButton("W: 16", &spritex, 16);
		ImGui::SameLine();
		ImGui::RadioButton("W: 32", &spritex, 32);
		ImGui::SameLine();
		ImGui::RadioButton("W: 64", &spritex, 64);

		ImGui::RadioButton("H: 8", &spritey, 8);
		ImGui::SameLine();
		ImGui::RadioButton("H: 16", &spritey, 16);
		ImGui::SameLine();
		ImGui::RadioButton("H: 32", &spritey, 32);
		ImGui::SameLine();
		ImGui::RadioButton("H: 64", &spritey, 64);

		
		ImGui::Text("Sprite BPP");  
			
		ImGui::RadioButton("BPP: 4", &spritebpp, 4);
		ImGui::SameLine();
		ImGui::RadioButton("BPP: 8", &spritebpp, 8);
		
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

void CSDialog::recieveInput(int mKey){
	
	if(mKey == SDLK_y){		
		bInputIsAccept=true;	
		bDialogIsWatingForText = false;		

		mGlobalSettings.mEditorState = ESTATE_SPRITECREATE;
		
		mGlobalSettings.mNewSpriteX = spritex;
		mGlobalSettings.mNewSpriteY = spritey;
		mGlobalSettings.mNewSpriteBPP = spritebpp;
	}		
	
	if(mKey == SDLK_n){
		bInputIsCancel=true;
	}			
}


int CPDialog::render(){
	
	Dialog::render();


	ImGui::Begin("Create New Project", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);                         
    		
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

		ImGui::Separator();

		ImGui::Text("TileSize X/Y");  
			
		ImGui::RadioButton("X: 8", &tilex, 8);
		ImGui::SameLine();
		ImGui::RadioButton("X: 16", &tilex, 16);
		ImGui::SameLine();
		ImGui::RadioButton("Y: 8", &tiley, 8);
		ImGui::SameLine();
		ImGui::RadioButton("Y: 16", &tiley, 16);

		ImGui::Separator();

		if(ImGui::Checkbox("Create/Import TileSet", &bHasTileSet)){
			tbpp = 8;
			if(!bHasTileSet){
				tilesetsize = 0;
			}	
		}

		if(bHasTileSet){
			ImGui::Text("TileSet Type");  
			ImGui::RadioButton("Project File", &tilesettype, 0);
			ImGui::SameLine();
			ImGui::RadioButton("Import PNG", &tilesettype, 1);
			ImGui::SameLine();
			ImGui::RadioButton("Empty TileSet", &tilesettype, 2);

			ImGui::Separator();

			switch (tilesettype)
			{
			case 0:
				ImGui::Text("TileSet File must match Project BPP and TileSizeX/Y");  

				mReadTileSet.render();
				if(mReadTileSet.bIsActive){
					mActiveInput = &mReadTileSet;
				}

				if (ImGui::Button("Choose TileSet File")){
					Dialog::render();
					ImGui::SetNextWindowSize(ImVec2(800, 600));
    				ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKeyNewfts", "Choose TileSet Project File", ".bin", ".");
				}

  				// display
  				if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKeyNewfts")){
    				// action if OK
    				if (ImGuiFileDialog::Instance()->IsOk()){
      						std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      						std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
	  						mReadTileSet.mDialogTextMain = filePathName;
			      			// action
    				}    
    			// close
    			ImGuiFileDialog::Instance()->Close();
	  			}

				ImGui::Text("TileSet BPP");  
				ImGui::RadioButton("BPP: 8", &tbpp, 8);
				ImGui::SameLine();
				ImGui::RadioButton("BPP: 4", &tbpp, 4);
				ImGui::SameLine();
				ImGui::RadioButton("BPP: 2", &tbpp, 2);
				break;
			case 1:
				mReadTileSet.render();
				if(mReadTileSet.bIsActive){
					mActiveInput = &mReadTileSet;
				}

					

				if (ImGui::Button("Choose TileSet PNG")){
					Dialog::render();
					ImGui::SetNextWindowSize(ImVec2(800, 600));
    				ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKeyNewftspng", "Choose TileSet PNG", ".png", ".");
				}

  				// display
  				if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKeyNewftspng")){
    				// action if OK
    				if (ImGuiFileDialog::Instance()->IsOk()){
      						std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      						std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
	  						mReadTileSet.mDialogTextMain = filePathName;
			      			// action
    				}    
    			// close
    			ImGuiFileDialog::Instance()->Close();
	  			}
				ImGui::Text("Imported Tile BPP");  
					ImGui::RadioButton("BPP: 8", &tbpp, 8);
					ImGui::SameLine();
					ImGui::RadioButton("BPP: 4", &tbpp, 4);
				
				break;
			case 2:
				ImGui::Text("TileSet Initial Size");  
				ImGui::RadioButton("4x4", &tilesetsize, 4);
				ImGui::SameLine();
				ImGui::RadioButton("8x8", &tilesetsize, 8);
				ImGui::SameLine();
				ImGui::RadioButton("16x16", &tilesetsize, 16);

				ImGui::Text("Tile BPP");  
				ImGui::RadioButton("BPP: 8", &tbpp, 8);
				ImGui::SameLine();
				ImGui::RadioButton("BPP: 4", &tbpp, 4);
				ImGui::SameLine();
				ImGui::RadioButton("BPP: 2", &tbpp, 2);

				break;
			
			default:
				break;
			}

		} else {
			ImGui::Text("Tile BPP");  
			ImGui::RadioButton("BPP: 8", &tbpp, 8);
			ImGui::SameLine();
			ImGui::RadioButton("BPP: 4", &tbpp, 4);
			ImGui::SameLine();
			ImGui::RadioButton("BPP: 2", &tbpp, 2);
		}

		ImGui::Separator();


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

		ImGui::Separator();

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

		ImGui::Separator();

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

/*
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
	ImGui::Begin("Hello, world!", &isShown, ImGuiWindowFlags_NoNav);                          // Create a window called "Hello, world!" and append into it.

   		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
   		
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
*/

int ITDialog::render(){	
	
	Dialog::render();

	ImGui::Begin("Import Tile", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);                         
    		
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
			mGlobalSettings.mEditorState = ESTATE_TILEIMPORT;
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


/* Frame Import*/

int ISFDialog::render(){	
	
	Dialog::render();

	ImGui::Begin("Import Sprite Frame", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);                         
    		
	ImGui::Text("Import Sprite Frame from PNG or RAW");
	

	mTextInput.render();

	if (ImGui::Button("Choose Frame File")){
		Dialog::render();
		ImGui::SetNextWindowSize(ImVec2(800, 600));
    	ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKeyFrame", "Choose File", ".png,.data,.raw,.bin", ".");
	}
  
  	// display
  	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKeyFrame")){
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


void ISFDialog::init(){
	mTextInput.bIsInputActive = true;
	mTextInput.bAutoComplete = true;
	mTextInput.bMustBeFile = true;	
}

void ISFDialog::cancel(){
		Dialog::cancel();
		mTextInput.mTextColor =  mGlobalSettings.DefaultTextColor;
		mTextInput.mDialogTextMain = "";
}

void ISFDialog::recieveInput(int mKey){
	if(mKey == SDLK_y){
		if(mTextInput.bInputIsAccepted){
			bInputIsAccept=true;	
			bDialogIsWatingForText = false;			
			mGlobalSettings.mEditorState = ESTATE_FRAMEIMPORT;

			mGlobalSettings.mNewFramePath = mTextInput.mDialogTextMain;
		}		
	}

	if(mKey == SDLK_n){
		bInputIsCancel=true;
	}		
	if(mKey == SDLK_TAB){
		mTextInput.autoComplete();		
	}
}


void ISFSDialog::recieveInput(int mKey){
	if(mKey == SDLK_y){
		if(mTextInput.bInputIsAccepted){
			bInputIsAccept=true;	
			bDialogIsWatingForText = false;
			
			mGlobalSettings.mEditorState = ESTATE_FRAMESIMPORT;

			mGlobalSettings.mNewFramesPath = mTextInput.mDialogTextMain;
		}		
	}

	if(mKey == SDLK_n){
		bInputIsCancel=true;
	}		
	if(mKey == SDLK_TAB){
		mTextInput.autoComplete();		
	}
}

int ISFSDialog::render(){	
	
	Dialog::render();

	ImGui::Begin("Import Sprite Frame(s)", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);                         
    		
	ImGui::Text("Import Sprite Frame(s) from Projet File, PNG or RAW");
	//ImGui::Text("Undo Stack will be cleared");
	

	mTextInput.render();

	if (ImGui::Button("Choose Frame(s) File")){
		Dialog::render();
		ImGui::SetNextWindowSize(ImVec2(800, 600));
    	ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKeyFrames", "Choose File", ".png,.bin,.data,.raw", ".");
	}
  
  	// display
  	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKeyFrames")){
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


/* End*/

/* Sprite*/

int ISDialog::render(){	
	
	Dialog::render();

	ImGui::Begin("Import Sprite", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);                         
    		
	ImGui::Text("Import Sprite from Project file, PNG or RAW");

	ImGui::RadioButton("Project File", &simporttype, 0);
	ImGui::SameLine();
	ImGui::RadioButton("PNG", &simporttype, 1);
	ImGui::SameLine();
	ImGui::RadioButton("RAW", &simporttype, 2);

	if(simporttype != 0){

		ImGui::Text("Sprite Width & Height");  
			
		ImGui::RadioButton("W: 8", &spritex, 8);
		ImGui::SameLine();
		ImGui::RadioButton("W: 16", &spritex, 16);
		ImGui::SameLine();
		ImGui::RadioButton("W: 32", &spritex, 32);
		ImGui::SameLine();
		ImGui::RadioButton("W: 64", &spritex, 64);

		ImGui::RadioButton("H: 8", &spritey, 8);
		ImGui::SameLine();
		ImGui::RadioButton("H: 16", &spritey, 16);
		ImGui::SameLine();
		ImGui::RadioButton("H: 32", &spritey, 32);
		ImGui::SameLine();	
		ImGui::RadioButton("H: 64", &spritey, 64);

		
		ImGui::Text("Sprite BPP");  
			
		ImGui::RadioButton("BPP: 4", &spritebpp, 4);
		ImGui::SameLine();
		ImGui::RadioButton("BPP: 8", &spritebpp, 8);

	}
	

	mTextInput.render();

	switch (simporttype)
	{
	case 0:
		if (ImGui::Button("Choose Sprite File")){
			Dialog::render();
			ImGui::SetNextWindowSize(ImVec2(800, 600));
    		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKeySprite", "Choose File", ".bin", ".");
		}
	break;
	case 1:
		if (ImGui::Button("Choose Sprite File")){
			Dialog::render();
			ImGui::SetNextWindowSize(ImVec2(800, 600));
    		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKeySprite", "Choose File", ".png", ".");
		}
	break;
	case 2:
		if (ImGui::Button("Choose Sprite File")){
			Dialog::render();
			ImGui::SetNextWindowSize(ImVec2(800, 600));
    		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKeySprite", "Choose File", ".bin,.raw,.data", ".");
		}
	break;
	
	default:
		break;
	}

  
  	// display
  	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKeySprite")){
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


void ISDialog::init(){
	mTextInput.bIsInputActive = true;
	mTextInput.bAutoComplete = true;
	mTextInput.bMustBeFile = true;	
}

void ISDialog::cancel(){
		Dialog::cancel();
		mTextInput.mTextColor =  mGlobalSettings.DefaultTextColor;
		mTextInput.mDialogTextMain = "";
		spritex=32;
		spritey=32;
		spritebpp=8;
		simporttype = 0;			
}

void ISDialog::recieveInput(int mKey){
	if(mKey == SDLK_y){
		if(mTextInput.bInputIsAccepted){
			bInputIsAccept=true;	
			bDialogIsWatingForText = false;
			
			mGlobalSettings.mEditorState = ESTATE_SPRITEIMPORT;

			mGlobalSettings.mNewSpritePath = mTextInput.mDialogTextMain;

			
			mGlobalSettings.mNewSpriteType = simporttype;
			mGlobalSettings.mNewSpriteX = spritex;
			mGlobalSettings.mNewSpriteY = spritey;
			mGlobalSettings.mNewSpriteBPP = spritebpp;

		}		
	}

	if(mKey == SDLK_n){
		bInputIsCancel=true;
	}		
	if(mKey == SDLK_TAB){
		mTextInput.autoComplete();		
	}
}


/* End*/


void ITSDialog::recieveInput(int mKey){
	if(mKey == SDLK_y){
		if(mTextInput.bInputIsAccepted){
			bInputIsAccept=true;	
			bDialogIsWatingForText = false;			
			mGlobalSettings.mEditorState = ESTATE_TILESETIMPORT;
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


int ITSDialog::render(){	
	
	Dialog::render();

	ImGui::Begin("Import TileSet", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);                         
    		
	ImGui::Text("Import TileSet from Project file or PNG");
	
	mTextInput.render();

	if (ImGui::Button("Choose TileSet File")){
		Dialog::render();
		ImGui::SetNextWindowSize(ImVec2(800, 600));
    	ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKeyTileSet", "Choose TileSet File", ".bin,.png", ".");
	}
  
  	// display
  	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKeyTileSet")){
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

void ITMDialog::cancel(){
		Dialog::cancel();
		mTextInput.mTextColor =  mGlobalSettings.DefaultTextColor;
		mTextInput.mDialogTextMain = "";
		bUseOffset = false;
		mTileOffset = 1;
		mPaletteOffset = 0;
}

void ITMDialog::recieveInput(int mKey){
	if(mKey == SDLK_y){
		if(mTextInput.bInputIsAccepted){
			bInputIsAccept=true;	
			bDialogIsWatingForText = false;
			mGlobalSettings.mNewTileMapPaletteOffset = mPaletteOffset;
			if(bUseOffset && (mTileOffset > 1)){				
				mGlobalSettings.mEditorState = ESTATE_TILEMAPIMPORTOFFSET;
				mGlobalSettings.mNewTileMapOffset = mTileOffset - 1;				
			} else {				
				mGlobalSettings.mEditorState = ESTATE_TILEMAPIMPORT;
			}
			mGlobalSettings.mNewTileMapPath = mTextInput.mDialogTextMain;
		}		
	}

	if(mKey == SDLK_n){
		bInputIsCancel=true;
	}		
	if(mKey == SDLK_TAB){
		mTextInput.autoComplete();		
	}
}


int ITMDialog::render(){	
	
	Dialog::render();

	ImGui::Begin("Import TileMap", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);                         
    		
	ImGui::Text("Import TileMap from Project file");
	ImGui::Text("Undo Stack will be cleared!");

	ImGui::Checkbox("Use Tile Offset", &bUseOffset);

	if(bUseOffset){
		int mMax = mGlobalSettings.CurrentEditor->mTileSet.TTiles.size();
		ImGui::SliderInt("Tile Offset", &mTileOffset, 1, mMax,"%d", ImGuiSliderFlags_AlwaysClamp);
	}

	if(mGlobalSettings.mGlobalTexParam.TileSetBPP < 0x8){
			ImGui::SliderInt("Palette Offset", &mPaletteOffset, 0, 15,"%d", ImGuiSliderFlags_AlwaysClamp);
	}
	
	mTextInput.render();

	if (ImGui::Button("Choose TileMap File")){
		Dialog::render();
		ImGui::SetNextWindowSize(ImVec2(800, 600));
    	ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKeyTileMap", "Choose TileMap File", ".bin", ".");
	}
  
  	// display
  	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKeyTileMap")){
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
	mHelpTextTileSet = mGlobalSettings.mHelpTextTileSet;
	mHelpTextSelection = mGlobalSettings.mHelpTextSelection;
	mHelpTextGeneral = mGlobalSettings.mHelpText;
	mHelpTextPalette = mGlobalSettings.mHelpTextPalette;
	mHelpTextImport = mGlobalSettings.mHelpTextImport;
	mHelpTextColMapEditor = mGlobalSettings.mHelpTextColMapEditor;
}

void HDialog::recieveInput(int mKey){
	bInputIsCancel=true;
}


int HDialog::render(){

	Dialog::render();

	ImGui::Begin("Help", &mGlobalSettings.bShowHelpDialog, ImGuiWindowFlags_NoNav);

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


	ImGuiStyle& style = ImGui::GetStyle();

    float size = ImGui::CalcTextSize("   Close   ").x + style.FramePadding.x * 2.0f;
    float avail = ImGui::GetContentRegionAvail().x;

    float off = (avail - size) * 0.5f;
    if(off > 0.0f){
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
	}

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
		mTextColor = mGlobalSettings.ErrorTextColor;		
	} else {		
		mTextColor = mGlobalSettings.DefaultTextColor;		
	}
}


void MEDialog::update(){

}


int MEDialog::render(){

	Dialog::render();

	ImGui::Begin("Message", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);                         
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
	
	int cTileFlip = mEditor->mTileMap->getFlip(mGlobalSettings.mSelectedTile);

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
	
	convert << mGlobalSettings.TileMapWidth << std::endl;
	convert >> cMapWidth;

	convert << mGlobalSettings.TileMapHeight << std::endl;
	convert >> cMapHeight;

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


	convert << mGlobalSettings.mGlobalTexParam.TileSizeX << std::endl;
	convert >> cTileSizeX;

	convert << mGlobalSettings.mGlobalTexParam.TileSizeY << std::endl;
	convert >> cTileSizeY;

	fliph=0;
	flipv=0;	

	update();
	
	mDialogTextMain = mGlobalSettings.mInfo + " Project Info";

		
}

int PIDialog::render(int xpos, int ypos){

	Dialog::render(xpos, ypos);

	ImGui::PushFont(mGlobalSettings.SFont);

	ImGui::Begin(mDialogTextMain.c_str(), &mGlobalSettings.bShowProjectInfo, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);
	
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

int PODialog::render(int xpos, int ypos, TextureParameters *mTexParam){

	Dialog::render(ypos);

	ImGui::PushFont(mGlobalSettings.SFont);


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

	ImGui::Begin("Quit Program?", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);                         
    		
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

void CCPDialog::recieveInput(int mKey){
	
	if(mKey == SDLK_y){
		std::cout << "Closing Project" << std::endl;
		bInputIsAccept=true;		
		mGlobalSettings.CurrentEditor->bEditorRunning = false;
		mGlobalSettings.bRunningOCD = true;
	}
	if(mKey == SDLK_n){
		mGlobalSettings.mOpenCreateProjectState = ESTATE_NONE;		
		bInputIsCancel=true;
	}
}

int CCPDialog::render(){
    
	Dialog::render();

	ImGui::Begin("Close Project?", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);                         
    		
			ImGui::Text("Close Current Project? Any unsaved progress will be lost"); 

            if (ImGui::Button("Close")){
				recieveInput(SDLK_y);
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel")){
				recieveInput(SDLK_n);
			}
            
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


void DTDialog::init(){

}

int DTDialog::render(){

	Dialog::render();

	ImGui::Begin(mDialogTextTitle.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);

	for(auto *cElem: mElements){
		cElem->render();
	}

	ImGui::End();

	return 0;
}

void DTDialog::recieveInput(int mKey){
	if(mKey == SDLK_y){	
		bInputIsAccept=true;	
		for(auto cVal : mValues){
			cVal->apply();			
		}
		mGlobalSettings.mEditorState = mTargetState;
	}		
	
	if(mKey == SDLK_n){
		bInputIsCancel=true;
		for(auto cVal : mValues){
			cVal->cancel();
		}
	}	

	if(mKey == SDLK_TAB){
		//mTextInput.autoComplete();		
	}

}

void DTDialog::cancel(){
	Dialog::cancel();

	for(auto cVal : mValues){
			cVal->cancel();
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

void DTDialog::addBool(std::string cLabel, bool cDefault, bool *cTarget, bool bSameline){
	DialogValueBool *nBool = new DialogValueBool(this, mRequiredCondition, cLabel, cDefault, cTarget, bSameline);

	mElements.push_back(nBool);
	mValues.push_back(nBool);

}

void DTDialog::addInt(std::string cLabel, int cDefault, int *cTarget, int cMin, int cMax, bool bSameline){
	DialogValueInt *nInt = new DialogValueInt(this, mRequiredCondition, cLabel, cDefault, cTarget, cMin, cMax, bSameline);

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

void DTDialog::addRadioButton(std::string cLabel, int cDefault, bool cSameline){
	DialogValueRadioGroup* cGroup = dynamic_cast<DialogValueRadioGroup*>(mValues[mValues.size()-1]);

	if(cGroup){
		DialogValueRadioButton* nGBut = new DialogValueRadioButton(cLabel, cDefault, &cGroup->mValue, cSameline);
		cGroup->mButtons.push_back(nGBut);
	}
}

void DTDialog::addIntTarget(int cDefault, int *cTarget){
	DialogValueIntTarget *nInt = new DialogValueIntTarget(this, mRequiredCondition, cDefault, cTarget);
	
	mValues.push_back(nInt);
}

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

/* Dialog Template End */