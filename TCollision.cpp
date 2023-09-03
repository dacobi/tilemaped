#include "TCollision.h"
#include "TileMap.h"
#include "TEditor.h"

extern TSettings mGlobalSettings;

void TCollisionMap::initoverlay(){
    mOverlay.setRects(&mTileMap->TileAreas);
	mOverlay.setGrid(mTileMap->TileMapWidth, mTileMap->TileMapHeight);
	mOverlay.setSize(mGlobalSettings.mGlobalTexParam.TexSizeX, mGlobalSettings.mGlobalTexParam.TexSizeY);
	mOverlay.setScale("C255", 1);

    mOverlay.mRender = [this]{
            int ccolval = MapData[mTileMap->getTile(mOverlay.mIndex)];

            if(ccolval > 0){
                mGlobalSettings.mOverlayText.renderText("C", mOverlay.mX , mOverlay.mY);
			    mGlobalSettings.mOverlayText.renderNum(ccolval, mOverlay.mX + mGlobalSettings.mOverlayText.mLastWidth, mOverlay.mY);
            }
    };
}

int TCollisionMap::createNew(TileMap *cTileMap, int cFormat){
    mTileMap = cTileMap;
    mMapFormat = cFormat;
    MapData.resize(mGlobalSettings.mEditor->mTileSet.TTiles.size(), 0);
    initoverlay();
    return 0;
}

int TCollisionMap::checkSize(){
    int retval = 0;

    while(MapData.size() < mGlobalSettings.mEditor->mTileSet.TTiles.size()){
        MapData.push_back(0);
        retval++;
    }
    
    while(MapData.size() > mGlobalSettings.mEditor->mTileSet.TTiles.size()){
        MapData.pop_back();
        retval--;
    }

    return retval;
}



int TCollisionMap::loadFromFile(std::string filename, TileMap *cTileMap){

    std::string tmpStr,tmpStr2;
    int tmpInt,tmpIntF,tmpInt2,tmpInt3;
    std::ifstream input(filename, std::ios::in );
    std::stringstream convert;

    std::getline(input, tmpStr);

    convert << tmpStr << std::endl;

    convert >> tmpStr;
    convert >> tmpInt;
    convert >> tmpIntF;

    if(tmpStr == "CollisionMap"){
        std::getline(input, tmpStr);        
        convert << tmpStr << std::endl;
        for(int i = 0; i < tmpInt; i++){                                    
            convert >> tmpInt2;
            MapData.push_back(tmpInt2);
        }        
        mTileMap = cTileMap;
        mMapFormat = tmpIntF;
        initoverlay();
        return 0;
    }

    return 1;
}


int TCollisionMap::saveToFolder(std::string tpath, std::string tprefix){

    if(MapData.size() == 0){
        return 0;
    }

    checkSize();

    std::string cColMapPath = tpath + DIRDEL + "col" + tprefix + ".dat";

    std::string tmpStr,tmpStr2;
    std::ofstream output(cColMapPath, std::ios::out );
    std::stringstream convert;

    tmpStr = "CollisionMap ";
    convert << MapData.size() << std::endl;
    convert >> tmpStr2;
     
    tmpStr += tmpStr2 + " ";

    convert << mMapFormat << std::endl;
    convert >> tmpStr2;
     
    tmpStr += tmpStr2;

    output << tmpStr << std::endl;

    for(int i = 0; i < MapData.size(); i++){
        convert << (int)MapData[i] << std::endl;
        convert >> tmpStr;
        tmpStr += " ";
        output << tmpStr;
    }

    output << std::endl;

    output.close();

    std::vector<unsigned char> FileData;

   	std::ofstream outfile(tpath+DIRDEL+"col" + tprefix + ".bin", std::ios::binary );

    FileData.push_back(mTileMap->mTilemapSizesOut[mTileMap->TileMapWidth]);
	FileData.push_back(mTileMap->mTilemapSizesOut[mTileMap->TileMapHeight]);
    
    if(mMapFormat == 0){
        for(int ii = 0; ii < mTileMap->TileMapHeight; ii++){
            for(int jj = 0; jj < mTileMap->TileMapWidth; jj++){
                unsigned char tVal = MapData[mTileMap->getTile((ii * mTileMap->TileMapWidth) + jj)];
                FileData.push_back(tVal);
            }
        }
    }

    if(mMapFormat == 1){
        bool bNewByte = false;
        unsigned char tVal;
        for(int ii = 0; ii < mTileMap->TileMapHeight; ii++){
            for(int jj = 0; jj < mTileMap->TileMapWidth; jj++){
                if(bNewByte){
                    tVal += (MapData[mTileMap->getTile((ii * mTileMap->TileMapWidth) + jj)] & 0x0f);
                    FileData.push_back(tVal);
                    bNewByte = false;
                } else {
                    tVal = (MapData[mTileMap->getTile((ii * mTileMap->TileMapWidth) + jj)] & 0x0f) << 4;
                    bNewByte = true;                    
                }
            }
        }
    }

    if(mMapFormat == 2){        
        unsigned char tVal;
        for(int ii = 0; ii < mTileMap->TileMapHeight; ii++){
            for(int jj = 0; jj < mTileMap->TileMapWidth; jj++){                
                    tVal = mTileMap->getFlip((ii * mTileMap->TileMapWidth) + jj) << 6;
                    tVal += (MapData[mTileMap->getTile((ii * mTileMap->TileMapWidth) + jj)] & 0x3f);
                    FileData.push_back(tVal);                  
            }
        }
    }

    if(mMapFormat == 3){
        bool bNewByte = false;
        unsigned char tVal;
        for(int ii = 0; ii < mTileMap->TileMapHeight; ii++){
            for(int jj = 0; jj < mTileMap->TileMapWidth; jj++){
                if(bNewByte){                    
                    tVal += mTileMap->getFlip((ii * mTileMap->TileMapWidth) + jj) << 2;                    
                    tVal += (MapData[mTileMap->getTile((ii * mTileMap->TileMapWidth) + jj)] & 0x03);                    
                    FileData.push_back(tVal);
                    bNewByte = false;
                } else {
                    tVal = mTileMap->getFlip((ii * mTileMap->TileMapWidth) + jj) << 2;                    
                    tVal += (MapData[mTileMap->getTile((ii * mTileMap->TileMapWidth) + jj)] & 0x03);
                    tVal = tVal << 4;
                    bNewByte = true;                    
                }
            }
        }
    }

    if(mMapFormat == 4){        
        unsigned char tVal;
        for(int ii = 0; ii < mTileMap->TileMapHeight; ii++){
            for(int jj = 0; jj < mTileMap->TileMapWidth; jj++){                
                    tVal = mTileMap->getOffset((ii * mTileMap->TileMapWidth) + jj) << 4;
                    tVal += (MapData[mTileMap->getTile((ii * mTileMap->TileMapWidth) + jj)] & 0x0f);
                    FileData.push_back(tVal);                  
            }
        }
    }

    if(mMapFormat == 5){        
        unsigned char tVal;
        for(int ii = 0; ii < mTileMap->TileMapHeight; ii++){
            for(int jj = 0; jj < mTileMap->TileMapWidth; jj++){  
                    tVal = mTileMap->getFlip((ii * mTileMap->TileMapWidth) + jj) << 6;              
                    tVal += mTileMap->getOffset((ii * mTileMap->TileMapWidth) + jj) << 2;
                    tVal += (MapData[mTileMap->getTile((ii * mTileMap->TileMapWidth) + jj)] & 0x03);
                    FileData.push_back(tVal);                  
            }
        }
    }


    outfile.write((char*)FileData.data(),FileData.size());
	outfile.close();

    return 0;            
}


void TCollisionMap::swapTileValues(int tVal1, int tVal2){
    checkSize();
    std::swap(MapData[tVal1],MapData[tVal2]);    
}

int TCollisionMap::removeTile(int cDropTile){
    checkSize();
    MapData.erase(MapData.begin() + cDropTile);
    return 0;
}


int TCollisionMapEditor::startEdit(TileMap *cTileMap){

    mTileMap = cTileMap;

    mTileMap->mColMap.checkSize();
	
    mSelectedTile = 0;
	TileAreas.resize(mGlobalSettings.mEditor->mTileSet.TTiles.size());
	mCollisionValue = mTileMap->mColMap.MapData[mSelectedTile];

    return 0;
}
 
    
int TCollisionMapEditor::render(){

    if(mTileMap->mColMap.checkSize() != 0){
        mSelectedTile = 0;
        mCollisionValue = mTileMap->mColMap.MapData[mSelectedTile];
        TileAreas.resize(mGlobalSettings.mEditor->mTileSet.TTiles.size());
    }

    int mCurColumns = 4;

    if(mGlobalSettings.mEditor->mTileSet.TTiles.size() > 63){
        mCurColumns = 8;
    }

    //if(mGlobalSettings.CurrentEditor->mTileSet.TTiles.size() > 255){
    //    mCurColumns = 16;
    //}

    int mColSpace = 2;
    int mCurTileScale = 4;
    int mScroll = 0;

    int isOdd = mGlobalSettings.mEditor->mTileSet.TTiles.size() % mCurColumns;
	int cRowNum = mGlobalSettings.mEditor->mTileSet.TTiles.size() / mCurColumns;

    if(mGlobalSettings.bHighDPI){
        ImGui::SetNextWindowSize(ImVec2(700,900), ImGuiCond_Once);
    } else {
        ImGui::SetNextWindowSize(ImVec2(700,900), ImGuiCond_Once);
    }

    ImVec2 cNewPos = ImGui::GetMainViewport()->GetCenter();
	cNewPos.x /= mGlobalSettings.mUIScale;
	cNewPos.y /= mGlobalSettings.mUIScale;

    if(bUpdateWinPos){
		ImGui::SetNextWindowPos(cNewPos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		bUpdateWinPos = false;
	} else {
		ImGui::SetNextWindowPos(cNewPos, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
	}

    ImGui::Begin(std::string("Collision Map: " + mTileMap->mColMap.mFormatStrings[mTileMap->mColMap.mMapFormat]).c_str(), &mGlobalSettings.mEditor->bShowCollisionEditor, ImGuiWindowFlags_NoNav); 

    std::stringstream convert;
    std::string sTile;
    convert <<  (mSelectedTile) << std::endl;
    convert >> sTile;

    std::string cCurTile = "Selected Tile: " + sTile;

    ImGui::Text("%s",cCurTile.c_str());

    ImGui::SameLine();

    ImGuiStyle& style3 = ImGui::GetStyle();

    float size3 = ImGui::CalcTextSize("Show Overlay").x * 1.3; //+ style3.FramePadding.x * 4.0f;
    float avail3 = ImGui::GetContentRegionAvail().x / 2;

    float off3 = (avail3 - size3) - 10;

    if(off3 > 0.0f){
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off3);
	}
     
    ImGui::Checkbox("Show Overlay", &mGlobalSettings.mProjectSettings.ColMap_ShowOverlay->bvalue);

     
    ImGui::SameLine();


    ImGuiStyle& style2 = ImGui::GetStyle();

    float size2 = ImGui::CalcTextSize("Set All Values").x + style2.FramePadding.x * 2.0f;
    float avail2 = ImGui::GetContentRegionAvail().x;

    float off2 = (avail2 - size2) - 10;
    if(off2 > 0.0f){
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off2);
	} 

    if(ImGui::Button("Set All Values")){
        for(auto &cTVal : mTileMap->mColMap.MapData){
            cTVal = mCollisionValue;
        }
    }

    if(ImGui::SliderInt("Collision Value", &mCollisionValue, 0, mTileMap->mColMap.mFormatMax[mTileMap->mColMap.mMapFormat] ,"%d", ImGuiSliderFlags_AlwaysClamp)){
        mTileMap->mColMap.MapData[mSelectedTile] = mCollisionValue;
    }

    ImGui::Separator();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(mGlobalSettings.DefaultBGColor.r,mGlobalSettings.DefaultBGColor.g,mGlobalSettings.DefaultBGColor.b)));    
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(mGlobalSettings.DefaultBGColor.r,mGlobalSettings.DefaultBGColor.g,mGlobalSettings.DefaultBGColor.b)));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(mGlobalSettings.DefaultBGColor.r,mGlobalSettings.DefaultBGColor.g,mGlobalSettings.DefaultBGColor.b)));
	
    ImVec2 cWinPos = ImGui::GetWindowPos();
    ImVec2 cWinSize = ImGui::GetWindowSize();

    ImGui::SetNextWindowSizeConstraints(ImVec2(cWinSize.x-20,cWinSize.y - 160),ImVec2(cWinSize.x-20,cWinSize.y - 160));

    int cRowRen = cRowNum + (isOdd > 0);
    int cAreaY = (cRowRen * 5) + (cRowRen * ((mGlobalSettings.mGlobalTexParam.TexSizeY * mCurTileScale ) + (mColSpace * 2)));
    

    ImGui::SetNextWindowContentSize(ImVec2(cWinSize.x-20,cAreaY+10));

    ImGui::BeginChild("Collision Tiles", ImVec2(0,0), false, ImGuiWindowFlags_NoNav);

    ImVec2 tSubWinPos = ImGui::GetWindowPos();

    mColMapOffset = tSubWinPos.y;

	if(mCurColumns > 0){
		for(int i = 0; i < cRowNum; i++){
                        
            ImVec2 cpos = ImGui::GetCursorPos();

			cpos.x += 5;
			cpos.y += 5;

			ImGui::SetCursorPos(cpos);            

			for(int j = 0; j < mCurColumns; j++){
				TileAreas[(i * mCurColumns) + j] = mGlobalSettings.mEditor->mTileSet.TTiles[(i*mCurColumns) + j]->renderImCol((cWinPos.x+ (mColSpace*2) +  ((mCurTileScale*mGlobalSettings.mGlobalTexParam.TexSizeX)+mColSpace)*j),cWinPos.y + mScroll + (mColSpace*2) + (((mGlobalSettings.mGlobalTexParam.TexSizeY*mCurTileScale)+mColSpace)*i), (i*mCurColumns) + j, mTileMap->mColMap.MapData[(i*mCurColumns) + j],  mCurTileScale, (((i*mCurColumns) + j) == mSelectedTile));												
				if((mCurColumns > 1) && (j < (mCurColumns-1))){					
					ImGui::SameLine();
				} 
			}										
		}	
		
		if(isOdd){			
            
            ImVec2 cpos = ImGui::GetCursorPos();

			cpos.x += 5;
			cpos.y += 5;

			ImGui::SetCursorPos(cpos);
            
			int i = mCurColumns;
			for(int j = 0; j < isOdd; j++){
				TileAreas[(i * cRowNum) + j] = mGlobalSettings.mEditor->mTileSet.TTiles[(i*cRowNum)+j]->renderImCol((cWinPos.x+ (mColSpace*2) +  ((mCurTileScale*mGlobalSettings.mGlobalTexParam.TexSizeX)+mColSpace)*j),cWinPos.y + mScroll + (mColSpace*2) + (((mGlobalSettings.mGlobalTexParam.TexSizeY*mCurTileScale)+mColSpace)*cRowNum), (i*cRowNum)+j , mTileMap->mColMap.MapData[(i*cRowNum)+j] ,  mCurTileScale, (((i * cRowNum) + j) == mSelectedTile));															
                if((j < (isOdd-1))){
					ImGui::SameLine();
				}
			}

		}		
	}

	ImGui::PopStyleColor(3);


    ImGui::EndChild();

    ImGui::Separator();
    
    ImGuiStyle& style = ImGui::GetStyle();

    float size = ImGui::CalcTextSize("   Close   ").x + style.FramePadding.x * 2.0f;
    float avail = ImGui::GetContentRegionAvail().x;

    float off = (avail - size) * 0.5f;
    if(off > 0.0f){
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
	}

    if(ImGui::Button("   Close   ")){
        mGlobalSettings.mEditor->bShowCollisionEditor = false;
    }

    mGlobalSettings.mEditor->ImButtonsColEdit.updateButtonStates();
    
    ImGui::End();

    return 0;
}