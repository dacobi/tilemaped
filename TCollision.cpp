#include "TCollision.h"
#include "TileMap.h"
#include "TEditor.h"

extern TSettings mGlobalSettings;

int TCollisionMap::createNew(TileMap *cTileMap){
    mTileMap = cTileMap;
    MapData.resize(mGlobalSettings.CurrentEditor->mTileSet.TTiles.size(), 0);
    return 0;
}

int TCollisionMap::checkSize(){
    int retval = 0;

    while(MapData.size() < mGlobalSettings.CurrentEditor->mTileSet.TTiles.size()){
        MapData.push_back(0);
        retval++;
    }
    
    while(MapData.size() > mGlobalSettings.CurrentEditor->mTileSet.TTiles.size()){
        MapData.pop_back();
        retval--;
    }

    return retval;
}



int TCollisionMap::loadFromFile(std::string filename, TileMap *cTileMap){

    std::string tmpStr,tmpStr2;
    int tmpInt,tmpInt2,tmpInt3;
    std::ifstream input(filename, std::ios::in );
    std::stringstream convert;

    std::getline(input, tmpStr);

    convert << tmpStr << std::endl;

    convert >> tmpStr;
    convert >> tmpInt;

    if(tmpStr == "CollisionMap"){
        std::getline(input, tmpStr);        
        convert << tmpStr << std::endl;
        for(int i = 0; i < tmpInt; i++){                                    
            convert >> tmpInt2;
            MapData.push_back(tmpInt2);
        }        
        mTileMap = cTileMap;
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
    
    for(int ii = 0; ii < mTileMap->TileMapHeight; ii++){
        for(int jj = 0; jj < mTileMap->TileMapWidth; jj++){
            unsigned char tVal = MapData[mTileMap->getTile((ii * mTileMap->TileMapWidth) + jj)];
            FileData.push_back(tVal);
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
	TileAreas.resize(mGlobalSettings.CurrentEditor->mTileSet.TTiles.size());
	mCollisionValue = mTileMap->mColMap.MapData[mSelectedTile];

    return 0;
}
 
    
int TCollisionMapEditor::render(){

    if(mTileMap->mColMap.checkSize() != 0){
        mSelectedTile = 0;
        mCollisionValue = mTileMap->mColMap.MapData[mSelectedTile];
        TileAreas.resize(mGlobalSettings.CurrentEditor->mTileSet.TTiles.size());
    }

    int mCurColumns = 4;

    if(mGlobalSettings.CurrentEditor->mTileSet.TTiles.size() > 63){
        mCurColumns = 8;
    }

    //if(mGlobalSettings.CurrentEditor->mTileSet.TTiles.size() > 255){
    //    mCurColumns = 16;
    //}

    int mColSpace = 2;
    int mCurTileScale = 4;
    int mScroll = 0;

    int isOdd = mGlobalSettings.CurrentEditor->mTileSet.TTiles.size() % mCurColumns;
	int cRowNum = mGlobalSettings.CurrentEditor->mTileSet.TTiles.size() / mCurColumns;

    ImGui::SetNextWindowSize(ImVec2(700,900), ImGuiCond_Once);

    if(bUpdateWinPos){
		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		bUpdateWinPos = false;
	} else {
		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
	}

    ImGui::Begin("Collision Map", &mGlobalSettings.CurrentEditor->bShowCollisionEditor, ImGuiWindowFlags_NoNav); 

    std::stringstream convert;
    std::string sTile;
    convert <<  (mSelectedTile + 1) << std::endl;
    convert >> sTile;

    std::string cCurTile = "Selected Tile: " + sTile;

    ImGui::Text("%s",cCurTile.c_str());

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

    if(ImGui::SliderInt("Collision Value", &mCollisionValue, 0, 255,"%d", ImGuiSliderFlags_AlwaysClamp)){
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
    int cAreaY = cRowRen * ((mGlobalSettings.mGlobalTexParam.TexSizeY * mCurTileScale ) + (mColSpace * 2));
    
    ImGui::SetNextWindowContentSize(ImVec2(cWinSize.x-20,cAreaY+10));

    ImGui::BeginChild("Collision Tiles", ImVec2(0,0), false, ImGuiWindowFlags_NoNav);

    ImVec2 tSubWinPos = ImGui::GetWindowPos();

    mColMapOffset = tSubWinPos.y;

	if(mCurColumns > 0){
		for(int i = 0; i < cRowNum; i++){
			for(int j = 0; j < mCurColumns; j++){
				TileAreas[(i * mCurColumns) + j] = mGlobalSettings.CurrentEditor->mTileSet.TTiles[(i*mCurColumns) + j]->renderImCol((cWinPos.x+ (mColSpace*2) +  ((mCurTileScale*mGlobalSettings.mGlobalTexParam.TexSizeX)+mColSpace)*j),cWinPos.y + mScroll + (mColSpace*2) + (((mGlobalSettings.mGlobalTexParam.TexSizeY*mCurTileScale)+mColSpace)*i), (i*mCurColumns) + j,  mCurTileScale, (((i*mCurColumns) + j) == mSelectedTile));												
				if((mCurColumns > 1) && (j < (mCurColumns-1))){					
					ImGui::SameLine();
				} 
			}										
		}	
		
		if(isOdd){			
			int i = mCurColumns;
			for(int j = 0; j < isOdd; j++){
				TileAreas[(i * cRowNum) + j] = mGlobalSettings.CurrentEditor->mTileSet.TTiles[(i*cRowNum)+j]->renderImCol((cWinPos.x+ (mColSpace*2) +  ((mCurTileScale*mGlobalSettings.mGlobalTexParam.TexSizeX)+mColSpace)*j),cWinPos.y + mScroll + (mColSpace*2) + (((mGlobalSettings.mGlobalTexParam.TexSizeY*mCurTileScale)+mColSpace)*cRowNum), (i*cRowNum)+j,  mCurTileScale, (((i*mCurColumns) + j) == mSelectedTile));												
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
        mGlobalSettings.CurrentEditor->bShowCollisionEditor = false;
    }

    mGlobalSettings.CurrentEditor->ImButtonsColEdit.updateButtonStates();
    
    ImGui::End();

    return 0;
}