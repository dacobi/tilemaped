#include "TSettings.h"
#include "TileMap.h"
#include "cx16_palette.h"
#include "TEditor.h"
#include <SDL.h>
#include "ImGuiFileDialog.h"

extern TSettings mGlobalSettings;

TTexture::~TTexture(){
	if(TileTex){
		SDL_DestroyTexture(TileTex);
		std::cout << "SDL_DestroyTexture(TTexture::TileTex)" << std::endl;
		TileTex = NULL;
	}
}

int TTexture::loadFromFile(std::string filename,TPalette* tpal){
    initTexture();
    std::ifstream infile(filename, std::ios::binary );

    std::vector<unsigned char> tbuffer(std::istreambuf_iterator<char>(infile), {});
    FileData = tbuffer;

    updateTexture(tpal);
    return 0;
}

int TTexture::loadFromBuffer(std::vector<unsigned char> &cTileBuf,TPalette* tpal){
	initTexture();

	FileData = cTileBuf;
	updateTexture(tpal);
	return 0;
}

int TTexture::initTexture(){
	TileTex = SDL_CreateTexture(mGlobalSettings.TRenderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, mTexParam->TileSizeX,mTexParam->TileSizeY);
	SDL_SetTextureBlendMode(TileTex, SDL_BLENDMODE_BLEND);
	return 0;
}

int TTexture::updateTexture(TPalette* tpal){
	if(PixelData.size()){		
		PixelData.erase(PixelData.begin(),PixelData.end());
	}

	for(int i = 0; i < (mTexParam->TileSizeX * mTexParam->TileSizeY); i++){
    		PixelData.push_back(tpal->mapPaletteColor(getPixel(i)));
	}

    SDL_UpdateTexture(TileTex, NULL, PixelData.data(), mTexParam->TileSizeX * sizeof(Uint32));
	return 0;
}

/*
SDL_Rect TTexture::render(int xpos, int ypos, int tscale, bool updateRect ,bool drawGrid){
	SDL_Rect renderQuad = { xpos, ypos, mGlobalSettings.mTexParam->TileSizeX*tscale, mGlobalSettings.mTexParam->TileSizeY*tscale};
    SDL_RenderCopy(mGlobalSettings.TRenderer, TileTex, NULL, &renderQuad);	
    if(drawGrid){
		SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, mGlobalSettings.PixelGridColor.r ,mGlobalSettings.PixelGridColor.b ,mGlobalSettings.PixelGridColor.b ,0xff);
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &renderQuad);
    }
    if(bPixelSelected){
		SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, mGlobalSettings.AltHighlightColor.r, mGlobalSettings.AltHighlightColor.r, mGlobalSettings.AltHighlightColor.r, 0xff);
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &renderQuad);
		SDL_Rect sndRect = renderQuad;
		sndRect.x = sndRect.x-1;
		sndRect.y = sndRect.y-1;
		sndRect.w = sndRect.w+2;
		sndRect.h = sndRect.h+2;

		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &sndRect);
    }
    if(updateRect){
        	CurrentArea = renderQuad;
    }
    return renderQuad;
}
*/

int TPixel::setPixelColor(unsigned char tcolor, TPalette* tpal){
	PixelIndex = tcolor;
	PixelColor = tpal->TPalette[tcolor];
	if(tcolor == 0){
		PixelColor.a = 0x00;
	}
	return 0;
}

SDL_Rect TPixel::renderIm(int xpos, int ypos, int tscale, bool updateRect ,bool drawGrid){
		CurrentArea = { xpos, ypos, mGlobalSettings.mGlobalTexParam.TileRenderSize*tscale, mGlobalSettings.mGlobalTexParam.TileRenderSize*tscale};

		ImVec2 elmin;
		ImVec2 elmax;

		elmin.x = xpos;
		elmin.y = ypos;
		elmax.x = xpos + mGlobalSettings.mGlobalTexParam.TileRenderSize*tscale;
		elmax.y = ypos + mGlobalSettings.mGlobalTexParam.TileRenderSize*tscale;

		ImDrawList *tList = ImGui::GetWindowDrawList();

		tList->AddRectFilled(elmin, elmax, mGlobalSettings.CurrentEditor->mPalette.getImColor(PixelColor));

		if(bPixelSelected){
			ImVec2 exmin,exmax;
			exmin.x = elmin.x -1;
			exmin.y = elmin.y -1;
			exmax.x = elmax.x +1;
			exmax.y = elmax.y +1;			
			tList->AddRect(elmin, elmax, mGlobalSettings.ImAltHighLightColor);
			tList->AddRect(exmin, exmax, mGlobalSettings.ImAltHighLightColor);
		} else {
			if(drawGrid){
				tList->AddRect(elmin, elmax, mGlobalSettings.CurrentEditor->mPalette.getImColor(mGlobalSettings.PixelGridColor));
			}	
		}

    	

		return CurrentArea;
}

SDL_Rect TPixel::renderImDisabled(int xpos, int ypos, int tscale, bool updateRect ,bool drawGrid){
		CurrentArea = { 0,0,0,0};

		ImVec2 elmin;
		ImVec2 elmax;

		elmin.x = xpos;
		elmin.y = ypos;
		elmax.x = xpos + mGlobalSettings.mGlobalTexParam.TileRenderSize*tscale;
		elmax.y = ypos + mGlobalSettings.mGlobalTexParam.TileRenderSize*tscale;

		ImDrawList *tList = ImGui::GetWindowDrawList();

		int dCol = PixelColor.r + PixelColor.g + PixelColor.b;
		dCol /= 0xf;
		ImU32 tCol = 0xff;
		tCol = (tCol << 8) + dCol;
		tCol = (tCol << 8) + dCol;
		tCol = (tCol << 8) + dCol;

		tList->AddRectFilled(elmin, elmax, tCol);

		if(drawGrid){
			tList->AddRect(elmin, elmax, 0xff000000);
		}	
		 
		return CurrentArea;
}


SDL_Rect TPixel::renderEditor(int xpos, int ypos, int tscale, bool updateRect ,bool drawGrid){
		CurrentArea = { xpos, ypos, mGlobalSettings.mGlobalTexParam.TileRenderSize*tscale, mGlobalSettings.mGlobalTexParam.TileRenderSize*tscale};

		ImVec2 elmin;
		ImVec2 elmax;

		elmin.x = xpos;
		elmin.y = ypos;
		elmax.x = xpos + mGlobalSettings.mGlobalTexParam.TileRenderSize*tscale;
		elmax.y = ypos + mGlobalSettings.mGlobalTexParam.TileRenderSize*tscale;

		ImDrawList *tList = ImGui::GetWindowDrawList();

		tList->AddRectFilled(elmin, elmax, mGlobalSettings.CurrentEditor->mPalette.getImColor(mGlobalSettings.CurrentEditor->mPalette.TPaletteEdit[PixelIndex]));

		if(bPixelSelectedEdit){
			ImVec2 exmin,exmax;
			exmin.x = elmin.x -1;
			exmin.y = elmin.y -1;
			exmax.x = elmax.x +2;
			exmax.y = elmax.y +2;			
			tList->AddRect(elmin, elmax, mGlobalSettings.ImAltHighLightColor);
			tList->AddRect(exmin, exmax, mGlobalSettings.ImAltHighLightColor);
		} else {
			if(drawGrid){
				tList->AddRect(elmin, elmax, mGlobalSettings.CurrentEditor->mPalette.getImColor(mGlobalSettings.PixelGridColor));
			}	
		}

    	

		return CurrentArea;
}

/*
SDL_Rect TPixel::render(int xpos, int ypos, int tscale, bool updateRect ,bool drawGrid){
	CurrentArea = { xpos, ypos, mGlobalSettings.mTexParam->TileRenderSize*tscale, mGlobalSettings.mTexParam->TileRenderSize*tscale};

	SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, PixelColor.r,PixelColor.g,PixelColor.b,PixelColor.a);
	
	SDL_RenderFillRect(mGlobalSettings.TRenderer, &CurrentArea);
    
	if(drawGrid){
		SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, mGlobalSettings.PixelGridColor.r ,mGlobalSettings.PixelGridColor.g ,mGlobalSettings.PixelGridColor.b ,0xff);
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &CurrentArea);
	}

    if(bPixelSelected){
		SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, mGlobalSettings.AltHighlightColor.r, mGlobalSettings.AltHighlightColor.g, mGlobalSettings.AltHighlightColor.b, 0xff); 
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &CurrentArea);
		SDL_Rect sndRect = CurrentArea;
		sndRect.x = sndRect.x-1;
		sndRect.y = sndRect.y-1;
		sndRect.w = sndRect.w+2;
		sndRect.h = sndRect.h+2;

		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &sndRect);
    }
    return CurrentArea;
}
*/

SDL_Rect TPixel::renderEdSel(int xpos, int ypos, int tscale, bool drawGrid){
	CurrentArea = { xpos, ypos, tscale, tscale};

	SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, PixelColor.r,PixelColor.g,PixelColor.b,PixelColor.a);
	
	SDL_RenderFillRect(mGlobalSettings.TRenderer, &CurrentArea);
    
	if(drawGrid){
		SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, mGlobalSettings.PixelGridColor.r ,mGlobalSettings.PixelGridColor.g ,mGlobalSettings.PixelGridColor.b ,0xff);
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &CurrentArea);
	}

	if(bPixelSelected){
		SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, mGlobalSettings.AltHighlightColor.r, mGlobalSettings.AltHighlightColor.g, mGlobalSettings.AltHighlightColor.b, 0xff); 
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &CurrentArea);
		SDL_Rect sndRect = CurrentArea;
		sndRect.x = sndRect.x-1;
		sndRect.y = sndRect.y-1;
		sndRect.w = sndRect.w+2;
		sndRect.h = sndRect.h+2;

		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &sndRect);
    }

    return CurrentArea;
}


SDL_Rect TPixel::renderEd(int xpos, int ypos, TextureParameters *mTexParam, int tscale, bool updateRect ,bool drawGrid){
	CurrentArea = { xpos, ypos, mTexParam->TilePixelSize*tscale, mTexParam->TilePixelSize*tscale};

	SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, PixelColor.r,PixelColor.g,PixelColor.b,PixelColor.a);
	
	SDL_RenderFillRect(mGlobalSettings.TRenderer, &CurrentArea);
    
	if(drawGrid){
		SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, mGlobalSettings.PixelGridColor.r ,mGlobalSettings.PixelGridColor.g ,mGlobalSettings.PixelGridColor.b ,0xff);
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &CurrentArea);
	}

    if(bPixelSelected){
		SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, mGlobalSettings.AltHighlightColor.r, mGlobalSettings.AltHighlightColor.g, mGlobalSettings.AltHighlightColor.b, 0xff); 
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &CurrentArea);
		SDL_Rect sndRect = CurrentArea;
		sndRect.x = sndRect.x-1;
		sndRect.y = sndRect.y-1;
		sndRect.w = sndRect.w+2;
		sndRect.h = sndRect.h+2;

		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &sndRect);
    }
    return CurrentArea;
}


SDL_Rect TTexture::renderEx(int xpos, int ypos, int tscale, SDL_RendererFlip flip){
	SDL_Rect renderQuad = { xpos, ypos, mTexParam->TileSizeX*tscale, mTexParam->TileSizeY*tscale};
        SDL_RenderCopyEx(mGlobalSettings.TRenderer, TileTex, NULL, &renderQuad, 0, NULL, flip);	
        return renderQuad;
}

void TTexture::renderEd(int xpos, int ypos, TPalette* tpal){
	for(int i=0; i < mTexParam->TileSizeY; i++){
		for(int j=0; j < mTexParam->TileSizeX; j++){
			tpal->renderTileEd(xpos + mTexParam->TileSizeX*j, ypos + mTexParam->TileSizeY*i, getPixel(j+(i*mTexParam->TileSizeX)), mTexParam); //FileData[j+(i*mGlobalSettings.TileSizeX)]);			
		}
	}

}

SDL_Color TPalette::getSDLColor4Bit(int cR, int cG, int cB){
	SDL_Color nCol;
	nCol.a = 0xff;

	int tColR =  mMapColorIn[cR];
	int tColG =  mMapColorIn[cG];
	int tColB =  mMapColorIn[cB];

	nCol.r = tColR;
	nCol.g = tColG;
	nCol.b = tColB;

	return nCol;
}

SDL_Color TPalette::getSDLColor(ImVec4 cCol){
	SDL_Color nCol;
	nCol.a = 0xff;

	int tColR =  mMapColorIn[((int)(cCol.x * 255.0f))/16];
	int tColG =  mMapColorIn[((int)(cCol.y * 255.0f))/16];
	int tColB =  mMapColorIn[((int)(cCol.z * 255.0f))/16];

	nCol.r = tColR;
	nCol.g = tColG;
	nCol.b = tColB;

	return nCol;
}


ImVec4 TPalette::getIm4Color(SDL_Color cColor){
	ImVec4 tColor;
	tColor.w = 1.0f;
	tColor.x = cColor.r / 255.0f;
	tColor.y = cColor.g / 255.0f;
	tColor.z = cColor.b / 255.0f;

	return tColor;
}


ImU32 TPalette::getImColor(SDL_Color cColor){
	ImU32 tColor;
	tColor = cColor.a;
	tColor = tColor << 8;
	tColor += cColor.b;
	tColor = tColor << 8;
	tColor += cColor.g;
	tColor = tColor << 8;
	tColor += cColor.r;

	return tColor;
}

Uint32 TPalette::mapPaletteColor(int tcolor){
	Uint32 tmpcol = TPalette[tcolor].r;
	tmpcol = tmpcol << 8;
	tmpcol += TPalette[tcolor].g;
	tmpcol = tmpcol << 8;
	tmpcol += TPalette[tcolor].b;
	tmpcol = tmpcol << 8;
	if(tcolor == 0){		
		tmpcol += 0;
 	} else {
		tmpcol += 255;
	}
	

	return tmpcol;
}

int TPalette::importPaletteEdit(std::string palPath){

	int pType = mGlobalSettings.testPaletteFile(palPath);

	if(pType == 0){
		mGlobalSettings.CurrentEditor->showMessage("Error: Not Palette File!", true);
		return 1;
	}

	if(pType == 2){		
		std::ifstream input( palPath, std::ios::in );
		unsigned char tpalette[256][3];

		std::string nline;
    	std::string ntmp;    
    
    	std::getline(input, nline);
    	std::getline(input, nline);
    	std::getline(input, nline);
    	std::getline(input, nline);            
        
    	std::stringstream convert;
    	int mr, mg, mb;
    
    	for(int i = 0; i < 256; i++){
 			std::getline(input, nline);       	
    		
			convert << nline << std::endl;
    		convert >> mr >> mg >> mb >> ntmp;
    	
	    	tpalette[i][0] = mr;
    		tpalette[i][1] = mg;
    		tpalette[i][2] = mb;    	
    	}

		if(TPaletteEdit.size()){
			TPaletteEdit.erase(TPaletteEdit.begin(), TPaletteEdit.end());
		}

		for(int i = 0; i < 256; i++){
			SDL_Color tmpcol;
			
			tmpcol.r = tpalette[i][0];
			tmpcol.g = tpalette[i][1];
			tmpcol.b = tpalette[i][2];
						
			if((i  == 0) ) {				
				tmpcol.a = 0;
			} else {
				tmpcol.a = 255;
			}
			
			TPaletteEdit.push_back(tmpcol);
		}
		mGlobalSettings.CurrentEditor->showMessage("Palette Import Succesful");
		return 0;
	}

	if(pType == 1){
		if(fs::exists(fs::status(palPath))){
			if(fs::is_directory(fs::status(palPath))){
				std::cout << "Palette path is directory: " << palPath << std::endl;
				return 1;
			}
		}

	std::ifstream infile(palPath, std::ios::binary );
    std::vector<unsigned char> tbuffer(std::istreambuf_iterator<char>(infile), {});

	int magic1,magic2;

	magic1 = tbuffer[0];
	magic2 = tbuffer[1];

	tbuffer.erase(tbuffer.begin());
	tbuffer.erase(tbuffer.begin());

	
	if((magic1 == 16) && (magic2 == 42) && (tbuffer.size() == 512)){

		if(TPaletteEdit.size()){
			TPaletteEdit.erase(TPaletteEdit.begin(), TPaletteEdit.end());
		}
	
		for(int i = 0; i < 512; i+=2){
			SDL_Color tmpcol;
			
			tmpcol.r = mMapColorIn[tbuffer[i+1]];
			tmpcol.g = mMapColorIn[tbuffer[i] >> 4];
			tmpcol.b = mMapColorIn[tbuffer[i] & 0xf];
			if((i  == 0) ) {				
				tmpcol.a = 0;
			} else {
				tmpcol.a = 255;
			}
			
			TPaletteEdit.push_back(tmpcol);
		}
		
		mGlobalSettings.CurrentEditor->showMessage("Palette Import Succesful");		
		return 0;
	}

	std::cout << "Error Loading Palette: " << palPath << std::endl;
	return 1;
	}

	return 0;
}

int TPalette::importGimpPalette(std::string palPath){
	if(mGlobalSettings.testPaletteFile(palPath) == 2){
		std::cout << "Importing Gimp Palette: " << palPath << std::endl;
		std::ifstream input( palPath, std::ios::in );
		unsigned char tpalette[256][3];

		std::string nline;
    	std::string ntmp;    
    
    	std::getline(input, nline);
    	std::getline(input, nline);
    	std::getline(input, nline);
    	std::getline(input, nline);            
        
    	std::stringstream convert;
    	int mr, mg, mb;
    
    	for(int i = 0; i < 256; i++){
 			std::getline(input, nline);       	
    		
			convert << nline << std::endl;
    		convert >> mr >> mg >> mb >> ntmp;
    	
	    	tpalette[i][0] = mr;
    		tpalette[i][1] = mg;
    		tpalette[i][2] = mb;    	
    	}

		if(TPalette.size()){
			TPalette.erase(TPalette.begin(), TPalette.end());
		}

		if(PixelAreas.size()){
			PixelAreas.erase(PixelAreas.begin(), PixelAreas.end());
		}

		for(int i = 0; i < 256; i++){
			SDL_Color tmpcol;
			
			tmpcol.r = tpalette[i][0];
			tmpcol.g = tpalette[i][1];
			tmpcol.b = tpalette[i][2];
						
			if((i  == 0) ) {				
				tmpcol.a = 0;
			} else {
				tmpcol.a = 255;
			}
			
			TPalette.push_back(tmpcol);
		}

		PixelAreas.resize(256);	

		std::vector<unsigned char> tbuffer;
		tbuffer.push_back(16);
		tbuffer.push_back(42);
		unsigned char tmpChar;
		
		for(int i = 0; i < 256; i++){
    		tmpChar = (tpalette[i][1])/16;
			tmpChar = tmpChar << 4;
        	tmpChar += (tpalette[i][2])/16;

			tbuffer.push_back(tmpChar);

			tmpChar = (tpalette[i][0])/16;
        	tbuffer.push_back(tmpChar);
    	}
		
		mGlobalSettings.ProjectPalette = tbuffer;
		TPaletteEdit = TPalette;
		
		return 0;
	}
	std::cout << "Not Gimp palette!" << std::endl;
	return 1;
}

int TPalette::saveToFile(std::string cpath){
	if(mGlobalSettings.ProjectPalette.size() == 514){		
		std::ofstream outfile(cpath, std::ios::binary );
		outfile.write((const char*)mGlobalSettings.ProjectPalette.data(), mGlobalSettings.ProjectPalette.size());
		outfile.close();
		return 1;
	}
	return 0;
}

int TPalette::saveToFolder(std::string cpath){
	if(mGlobalSettings.ProjectPalette.size() == 514){		
		std::ofstream outfile(cpath + DIRDEL + "pal.bin", std::ios::binary );
		outfile.write((const char*)mGlobalSettings.ProjectPalette.data(), mGlobalSettings.ProjectPalette.size());
		outfile.close();
		return 1;
	}
	return 0;
}

int TPalette::updatePalette(){
	
	TPalette = TPaletteEdit;
	updateTPixels();

	if(!mGlobalSettings.ProjectPalette.size()){
		mGlobalSettings.ProjectPalette.resize(514);
		mGlobalSettings.ProjectPalette[0] = 16;
		mGlobalSettings.ProjectPalette[1] = 42;
		mGlobalSettings.bProjectHasPalette = true;
	}

	int pindex=0;
	for(int i = 2; i < 514; i+=2){			
		mGlobalSettings.ProjectPalette[i] = mMapColorOut[TPalette[pindex].g] << 4;
		mGlobalSettings.ProjectPalette[i] += mMapColorOut[TPalette[pindex].b];
		mGlobalSettings.ProjectPalette[i+1] = mMapColorOut[TPalette[pindex].r];
		pindex++;
	}

	return 0;
}

int TPalette::loadFromFile(std::string filename){

	if(fs::exists(fs::status(filename))){
		if(fs::is_directory(fs::status(filename))){
			std::cout << "Palette path is directory: " << filename << std::endl;
			return 1;
		}
	}

	if(TPalette.size()){
		TPalette.clear();
	}

	std::ifstream infile(filename, std::ios::binary );
    std::vector<unsigned char> tbuffer(std::istreambuf_iterator<char>(infile), {});

	mGlobalSettings.ProjectPalette = tbuffer;

	int magic1,magic2;

	magic1 = tbuffer[0];
	magic2 = tbuffer[1];

	tbuffer.erase(tbuffer.begin());
	tbuffer.erase(tbuffer.begin());
	
	if((magic1 == 16) && (magic2 == 42) && (tbuffer.size() == 512)){
		for(int i = 0; i < 512; i+=2){
			SDL_Color tmpcol;
			
			tmpcol.r = mMapColorIn[tbuffer[i+1]];
			tmpcol.g = mMapColorIn[tbuffer[i] >> 4];
			tmpcol.b = mMapColorIn[tbuffer[i] & 0xf];
			if((i  == 0) ) {				
				tmpcol.a = 0;
			} else {
				tmpcol.a = 255;
			}
			
			TPalette.push_back(tmpcol);
		}
		
		PixelAreas.resize(256);	

		TPaletteEdit = TPalette;

		std::cout << "Palette loaded from: " << filename << std::endl;
		return 0;
	}
	std::cout << "Error Loading Palette: " << filename << std::endl;
	return 1;
}

void TPalette::close(){

	if(TPalette.size()){
		TPalette.clear();
		TPaletteEdit.clear();
	}

	if(TPixels.size()){
		for(int i = 0; i < TPixels.size(); i++){
			delete TPixels[i];
		}
		TPixels.clear();
	}
}

int TPalette::initPalette(){
	if(TPalette.size()){
		TPalette.clear();
	}

	for(int i = 0; i < 256; i++){
		SDL_Color tmpcol;
		tmpcol.r = palette[i][0];
		tmpcol.g = palette[i][1];
		tmpcol.b = palette[i][2];
		if(i == 0) {			
			tmpcol.a = 0;
		} else {
			tmpcol.a = 255;
		}		
		TPalette.push_back(tmpcol);
	}
	PixelAreas.resize(256);
	
	TPaletteEdit = TPalette;
	
	return 0;
}

int TPalette::updateTPixels(){
	for(int i = 0; i < 256; i++){				
		TPixels[i]->setPixelColor(i, this);
	}
	return 0;
}

int TPalette::initTPixels(){
	if(TPixels.size()){
		for(int i = 0; i < TPixels.size(); i++){
			delete TPixels[i];
		}
		TPixels.clear();
	}

	TPixel *tmptpix;
	for(int i = 0; i < 256; i++){
		tmptpix = new TPixel;
		tmptpix->setPixelColor(i, this);
		TPixels.push_back(tmptpix);		
	}
	return 0;
}

SDL_Rect TPalette::renderSelEd(int xpos,int ypos, int tcolor, int cScale){
	SDL_SetRenderDrawBlendMode(mGlobalSettings.TRenderer, SDL_BLENDMODE_BLEND);
	int ccolor=0;
	if(tcolor != 0) ccolor = tcolor;
	return TPixels[ccolor]->renderEdSel(xpos, ypos, cScale,mGlobalSettings.bShowTilePixelSelGrid);	
}

SDL_Rect TPalette::renderTileEd(int xpos,int ypos, int tcolor, int cScale, TextureParameters *mTexParam){
	SDL_SetRenderDrawBlendMode(mGlobalSettings.TRenderer, SDL_BLENDMODE_BLEND);
	int ccolor=0;
	if(tcolor != 0) ccolor = tcolor + (mTexParam->PaletteOffset*16);
	return TPixels[ccolor]->renderEdSel(xpos, ypos, cScale,mGlobalSettings.bShowTilePixelGrid);	
}

SDL_Rect TPalette::renderTileEd(int xpos,int ypos, int tcolor, TextureParameters *mTexParam){
	SDL_SetRenderDrawBlendMode(mGlobalSettings.TRenderer, SDL_BLENDMODE_BLEND);
	int ccolor=0;
	if(tcolor != 0) ccolor = tcolor + (mTexParam->PaletteOffset*16);
	return TPixels[ccolor]->renderEd(xpos, ypos, mTexParam, mTexParam->mTileEdScale,false,mGlobalSettings.bShowPixelGrid);	
}

SDL_Rect TPalette::renderSpriteEd(int xpos,int ypos, int tcolor, TextureParameters *mTexParam){
	SDL_SetRenderDrawBlendMode(mGlobalSettings.TRenderer, SDL_BLENDMODE_BLEND);
	int ccolor=0;
	if(tcolor != 0) ccolor = tcolor + (mTexParam->PaletteOffset*16);
	return TPixels[ccolor]->renderEd(xpos, ypos, mTexParam, mTexParam->mTileEdScale,false,mGlobalSettings.bShowPixelGridSprite);	
}


/*
int TPalette::render(int xpos,int ypos){
	SDL_SetRenderDrawBlendMode(mGlobalSettings.TRenderer, SDL_BLENDMODE_NONE);
	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j++){
			PixelAreas[(i*16)+j] = TPixels[(i*16)+j]->render(
					xpos + ((mGlobalSettings.mTexParam->TileRenderSize*mGlobalSettings.PaletteScale+2)*j),
					ypos + ((mGlobalSettings.mTexParam->TileRenderSize*mGlobalSettings.PaletteScale+2)*i),
					mGlobalSettings.PaletteScale,true,true);
		}
	}

return 0;
}
*/

int TPalette::renderIm(int xpos,int ypos, TextureParameters *mTexParam){
	
	Dialog::render(xpos, ypos);

	ImGui::Begin("Palette", NULL, ImGuiWindowFlags_NoNav);

	ImVec2 cSize;
	if(mTexParam->TileSetBPP < 0x8){
		cSize.x = 90 + ((mTexParam->TileRenderSize*mGlobalSettings.PaletteScale+4) * 16);
		cSize.y = 50 + ((mTexParam->TileRenderSize*mGlobalSettings.PaletteScale+4) * 16);
	} else {
		cSize.x = 20 + ((mTexParam->TileRenderSize*mGlobalSettings.PaletteScale+4) * 16);
		cSize.y = 50 + ((mTexParam->TileRenderSize*mGlobalSettings.PaletteScale+4) * 16);
	}

	ImGui::SetWindowSize(cSize, ImGuiCond_Once);

	ImVec2 cPos = ImGui::GetWindowPos();
	cPos.x += 10;
	cPos.y += 40;

	if(mTexParam->TileSetBPP < 0x8){

	cPos.x += 70;

	ImGui::RadioButton("0", &mTexParam->PaletteOffset, 0);
	ImGui::RadioButton("1", &mTexParam->PaletteOffset, 1);	
	ImGui::RadioButton("2", &mTexParam->PaletteOffset, 2);
	ImGui::RadioButton("3", &mTexParam->PaletteOffset, 3);
	ImGui::RadioButton("4", &mTexParam->PaletteOffset, 4);
	ImGui::RadioButton("5", &mTexParam->PaletteOffset, 5);
	ImGui::RadioButton("6", &mTexParam->PaletteOffset, 6);	
	ImGui::RadioButton("7", &mTexParam->PaletteOffset, 7);
	ImGui::RadioButton("8", &mTexParam->PaletteOffset, 8);
	ImGui::RadioButton("9", &mTexParam->PaletteOffset, 9);
	ImGui::RadioButton("10", &mTexParam->PaletteOffset, 10);
	ImGui::RadioButton("11", &mTexParam->PaletteOffset, 11);
	ImGui::RadioButton("12", &mTexParam->PaletteOffset, 12);
	ImGui::RadioButton("13", &mTexParam->PaletteOffset, 13);
	ImGui::RadioButton("14", &mTexParam->PaletteOffset, 14);
	ImGui::RadioButton("15", &mTexParam->PaletteOffset, 15);


		for(int i = 0; i < 16; i++){
			if(i == mTexParam->PaletteOffset){
				if(mTexParam->TileSetBPP == 0x4){
					for(int j = 0; j < 16; j++){
						PixelAreas[(i*16)+j] = TPixels[(i*16)+j]->renderIm(
						cPos.x + ((mTexParam->TileRenderSize*mGlobalSettings.PaletteScale+3)*j),
						cPos.y + ((mTexParam->TileRenderSize*mGlobalSettings.PaletteScale+3)*i),
						mGlobalSettings.PaletteScale,true,true);
					}
				} else  {
					for(int j = 0; j < 4; j++){
						PixelAreas[(i*16)+j] = TPixels[(i*16)+j]->renderIm(
						cPos.x + ((mTexParam->TileRenderSize*mGlobalSettings.PaletteScale+3)*j),
						cPos.y + ((mTexParam->TileRenderSize*mGlobalSettings.PaletteScale+3)*i),
						mGlobalSettings.PaletteScale,true,true);
					}
					for(int j = 4; j < 16; j++){
						PixelAreas[(i*16)+j] = TPixels[(i*16)+j]->renderImDisabled(
						cPos.x + ((mTexParam->TileRenderSize*mGlobalSettings.PaletteScale+3)*j),
						cPos.y + ((mTexParam->TileRenderSize*mGlobalSettings.PaletteScale+3)*i),
						mGlobalSettings.PaletteScale,true,true);
					}
				}
			} else {
				for(int j = 0; j < 16; j++){
					PixelAreas[(i*16)+j] = TPixels[(i*16)+j]->renderImDisabled(
					cPos.x + ((mTexParam->TileRenderSize*mGlobalSettings.PaletteScale+3)*j),
					cPos.y + ((mTexParam->TileRenderSize*mGlobalSettings.PaletteScale+3)*i),
					mGlobalSettings.PaletteScale,true,true);
				}
			}
		}

	} else {

	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j++){
			PixelAreas[(i*16)+j] = TPixels[(i*16)+j]->renderIm(
					cPos.x + ((mTexParam->TileRenderSize*mGlobalSettings.PaletteScale+3)*j),
					cPos.y + ((mTexParam->TileRenderSize*mGlobalSettings.PaletteScale+3)*i),
					mGlobalSettings.PaletteScale,true,true);
		}
	}
	}

	mGlobalSettings.CurrentEditor->ImButtonsPalette.updateButtonStates();

	ImGui::End();

return 0;
}

void TPalette::setEditColor(){
	mR = (mEditColor.x * 255.0f) / 16;
	mG = (mEditColor.y * 255.0f) / 16;
	mB = (mEditColor.z * 255.0f) / 16;
}

int TPalette::renderEditor(int xpos,int ypos){
	
	Dialog::render(xpos, ypos);

	ImGui::Begin("Palette: Edit Mode", NULL, ImGuiWindowFlags_NoNav);

	if(bImportingPalette){
		Dialog::render();
		ImGui::SetNextWindowSize(ImVec2(800, 600));
    	ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKeyPal", "Choose Palette File", ".gpl,.bin", ".");	
		bImportingPalette = false;
	}

	// display
  	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKeyPal")){
    	// action if OK
    	if (ImGuiFileDialog::Instance()->IsOk()){
      		std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      		std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
	  		importPaletteEdit(filePathName);
      		// action
    	}    
    // close
    	ImGuiFileDialog::Instance()->Close();
  	}

	static ImVec4 backup_color;

    ImGuiColorEditFlags misc_flags = ( 0 ); //: ImGuiColorEditFlags_NoOptions);

	ImGui::Text("Change Selected Color");     

	const char *mRval = mMapColorVals[mR].c_str();
	const char *mGval = mMapColorVals[mG].c_str();
	const char *mBval = mMapColorVals[mB].c_str();

	ImGui::SliderInt("Red", &mR, 0, 15, mRval, ImGuiSliderFlags_AlwaysClamp);
	ImGui::SliderInt("Green", &mG, 0, 15, mGval, ImGuiSliderFlags_AlwaysClamp);
	ImGui::SliderInt("Blue", &mB, 0, 15, mBval, ImGuiSliderFlags_AlwaysClamp);
	
	bool bColorPicker = ImGui::ColorButton("Selected Color##3b", mEditColor, misc_flags);
	ImGui::SameLine();
	ImGui::Text("Pick Selected Color");

	if(bColorPicker){
	    ImGui::OpenPopup("mypicker");
		backup_color = mEditColor;
		mEditColorPick = mEditColor;
	}

	ImVec2 sPos = ImGui::GetWindowPos();

	ImGui::SetNextWindowPos(ImVec2(sPos.x+600, sPos.y), ImGuiCond_Always);

	bIsPickerOpen = false;

	if (ImGui::BeginPopup("mypicker", ImGuiWindowFlags_NoNav))
        {
			bIsPickerOpen = true;

            ImGui::Text("Select Color");
            ImGui::Separator();
            ImGui::ColorPicker3("##picker", (float*)&mEditColorPick, misc_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoInputs);
            //ImGui::SameLine();            
            ImGui::Text("Current");
			ImGui::SameLine();            
            ImGui::ColorButton("##current", mEditColorPick, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));
			ImGui::SameLine();            
            ImGui::Text("Previous");
			ImGui::SameLine();            	
            if (ImGui::ColorButton("##previous", backup_color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40))){
                	mEditColorPick = backup_color;
			}

			if(ImGui::Button("Choose")){
				mR = (mEditColorPick.x * 255.0f) / 16;
				mG = (mEditColorPick.y * 255.0f) / 16;
				mB = (mEditColorPick.z * 255.0f) / 16;

				bColorPicker = false;				
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if(ImGui::Button("Close")){
				bColorPicker = false;
				ImGui::CloseCurrentPopup();
			}

            ImGui::EndPopup();
        }


	ImVec2 cSize;
	cSize.x = 20 + ((mGlobalSettings.mGlobalTexParam.TileRenderSize*mGlobalSettings.PaletteScale+4) * 16);
	cSize.y = 270 + ((mGlobalSettings.mGlobalTexParam.TileRenderSize*mGlobalSettings.PaletteScale+4) * 16);

	ImGui::SetWindowSize(cSize, ImGuiCond_Once);

	ImVec2 cPos = ImGui::GetWindowPos();
	cPos.x += 10;
	cPos.y += 255;

	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j++){
			PixelAreas[(i*16)+j] = TPixels[(i*16)+j]->renderEditor(
					cPos.x + ((mGlobalSettings.mGlobalTexParam.TileRenderSize*mGlobalSettings.PaletteScale+4)*j),
					cPos.y + ((mGlobalSettings.mGlobalTexParam.TileRenderSize*mGlobalSettings.PaletteScale+4)*i),
					mGlobalSettings.PaletteScale,true,true);
		}
	}

	mGlobalSettings.CurrentEditor->ImButtonsPalette.updateButtonStates();


	TPaletteEdit[mGlobalSettings.CurrentEditor->mColorSelectedEdit] = getSDLColor4Bit(mR, mG, mB);
	mEditColor = getIm4Color(TPaletteEdit[mGlobalSettings.CurrentEditor->mColorSelectedEdit]);

	//TPaletteEdit[mGlobalSettings.CurrentEditor->mColorSelectedEdit] = getSDLColor(mEditColor);

	if(ImGui::Button("Apply Changes")){
		mGlobalSettings.CurrentEditor->activatePaletteUpdate();
	}

	ImGui::SameLine();

	if(ImGui::Button("Cancel Changes")){
		TPaletteEdit = TPalette;
		mEditColor = getIm4Color(TPalette[mGlobalSettings.CurrentEditor->mColorSelectedEdit]);
		setEditColor();	
	}

	ImGui::End();

return 0;
}


Tile::~Tile(){	
	freeTexture();
}

void Tile::freeTexture(){
	if(mTexParam->TileSetBPP < 8){
		std::cout << "SDL_DestroyTexture(Tile::TPOffset)" << std::endl;
		for(auto * cTex : TPOffset){
			if(cTex != NULL){				
				SDL_DestroyTexture(cTex);		
				cTex = NULL;
			}	
		}
	} else {
		if(TileTex != NULL){
			std::cout << "SDL_DestroyTexture(Tile::TileTex)" << std::endl;
			SDL_DestroyTexture(TileTex);		
			TileTex = NULL;
		}
	}
}

int TTexture::setPixel(int pindex, unsigned char pcolor, std::vector<unsigned char> &tBuf){
	if(mTexParam->TileSetBPP == 0x8){
		tBuf[pindex] = pcolor;
	}

	if(mTexParam->TileSetBPP == 0x4){
		int cindex = pindex / 2;
		int crem = pindex % 2;
		unsigned char ccolor = 0;

		int tmppix = tBuf[cindex];

		if(crem){
			ccolor = (tmppix & 0xf0) + (pcolor%16);
		} else {
			ccolor = (tmppix & 0x0f) + ((pcolor%16) << 4);
		}

		tBuf[cindex] = ccolor;
	}

	if(mTexParam->TileSetBPP == 0x2){
		int cindex = pindex / 4;
		int crem = pindex % 4;
		unsigned char ccolor = 0;

		int tmppix = tBuf[cindex];

		if(crem){
			switch(crem){
				case 1:
					ccolor = (tmppix & 0xCF) + (((pcolor%16)%4) << 4);
				break;
				case 2:
					ccolor = (tmppix & 0xF3) + (((pcolor%16)%4) << 2);
				break;
				case 3:
					ccolor = (tmppix & 0xFC) + (((pcolor%16)%4));
				break;
			};
		} else {
			ccolor = (tmppix & 0x3f) + (((pcolor%16)%4) << 6);
		}

		tBuf[cindex] = ccolor;
	}

	return 0;
}

int TTexture::setPixel(int pindex, unsigned char pcolor){
	if(mTexParam->TileSetBPP == 0x8){
		FileData[pindex] = pcolor;
	}
	if(mTexParam->TileSetBPP == 0x4){
		int cindex = pindex / 2;
		int crem = pindex % 2;
		unsigned char ccolor = 0;

		int tmppix = FileData[cindex];

		if(crem){
			ccolor = (tmppix & 0xf0) + (pcolor%16);
		} else {
			ccolor = (tmppix & 0x0f) + ((pcolor%16) << 4);
		}

		FileData[cindex] = ccolor;
	}
	if(mTexParam->TileSetBPP == 0x2){
		int cindex = pindex / 4;
		int crem = pindex % 4;
		unsigned char ccolor = 0;

		int tmppix = FileData[cindex];

		if(crem){
			switch(crem){
				case 1:
					ccolor = (tmppix & 0xCF) + (((pcolor%16)%4) << 4);
				break;
				case 2:
					ccolor = (tmppix & 0xF3) + (((pcolor%16)%4) << 2);
				break;
				case 3:
					ccolor = (tmppix & 0xFC) + (((pcolor%16)%4));
				break;
			};
		} else {
			ccolor = (tmppix & 0x3f) + (((pcolor%16)%4) << 6);
		}

		FileData[cindex] = ccolor;
	}
	return 0;
}

int TTexture::getFlipIndex(int pindex, int tflip){
	int nindex = pindex;

	if(tflip == 1){		
		nindex = pindex - 2 * (pindex % mTexParam->TileSizeX ) + mTexParam->TileSizeX  - 1;	
	}

	if(tflip == 2){				
		nindex = (mTexParam->TileSizeY - 1 - pindex / mTexParam->TileSizeX) * mTexParam->TileSizeX + pindex % mTexParam->TileSizeX;		
	}

	if(tflip == 3){
		nindex =  (mTexParam->TileSizeX *mTexParam->TileSizeY ) - pindex - 1;		
	}

	return nindex;
}

unsigned char TTexture::getPixel(int pindex, int poffset, int tflip){
	unsigned char tpix = 0;

	//int pIndexX;
	//int pIndexY;
	int nindex;

	//pIndexX = pindex % mGlobalSettings.TileSizeX;
	//pIndexY = pindex / mGlobalSettings.TileSizeX;

	if(tflip == 0){
		tpix = getPixel(pindex);
	}

	if(tflip == 1){		
		nindex = pindex - 2 * (pindex % mTexParam->TileSizeX ) + mTexParam->TileSizeX  - 1;
		tpix = getPixel(nindex);
	}

	if(tflip == 2){				
		nindex = (mTexParam->TileSizeY - 1 - pindex / mTexParam->TileSizeX) * mTexParam->TileSizeX + pindex % mTexParam->TileSizeX;
		tpix = getPixel(nindex);
	}

	if(tflip == 3){
		nindex =  (mTexParam->TileSizeX *mTexParam->TileSizeY ) - pindex - 1;
		tpix = getPixel(nindex);
	}

	if(tpix == 0) return tpix;
	tpix += (poffset*16);
	return tpix;

}

unsigned char TTexture::getPixel(int pindex, int poffset){
	unsigned char tpix = getPixel(pindex);
	if(tpix == 0) return tpix;
	tpix += (poffset*16);
	return tpix;
}

unsigned char TTexture::getPixel(int pindex){
	if(mTexParam->TileSetBPP == 0x8){
		 return FileData[pindex];
	}
	if(mTexParam->TileSetBPP == 0x4){
		int cindex = pindex / 2;
		int crem = pindex % 2;
		unsigned char ccolor = 0;

		int tmppix = FileData[cindex];

		if(crem){
			ccolor = (tmppix & 0x0f);
		} else {
			ccolor = ((tmppix & 0xf0) >> 4);			
		}

		return ccolor;
	}
	if(mTexParam->TileSetBPP == 0x2){
		int cindex = pindex / 4;
		int crem = pindex % 4;
		unsigned char ccolor = 0;

		int tmppix = FileData[cindex];

		if(crem){
			switch(crem){
				case 1:
					ccolor = ((tmppix & 0x30) >> 4);
				break;
				case 2:
					ccolor = ((tmppix & 0x0C) >> 2);
				break;
				case 3:
					ccolor = (tmppix & 0x03);
				break;
			};
		} else {
			ccolor = ((tmppix & 0xC0) >> 6);
		}
		return ccolor;
	}
	return 0;
}

int Tile::rotater(){
	if(mTexParam->TileSizeX != mTexParam->TileSizeY){return 1;}

	std::vector<unsigned char> tmpData;
	tmpData.resize(FileData.size());

	int index;
	int lsize = mTexParam->TileSizeX;
	unsigned char tmpPix;

	
	for(int i = 0; i < mTexParam->TileSizeY; i++){
		for(int j = 0; j < mTexParam->TileSizeX; j++){
			index = (lsize*(lsize-j-1))+i;
			tmpPix = getPixel(index);		
			setPixel((i*(lsize))+j, tmpPix, tmpData);
		}
	}

	FileData = tmpData;

	updateTexture(&mGlobalSettings.CurrentEditor->mPalette);

	return 0;
}

int Tile::rotatel(){
	if(mTexParam->TileSizeX != mTexParam->TileSizeY){return 1;}

	std::vector<unsigned char> tmpData;
	tmpData.resize(FileData.size());

	int index;
	int lsize = mTexParam->TileSizeX;
	unsigned char tmpPix;

	
	for(int i = 0; i < mTexParam->TileSizeY; i++){
		for(int j = 0; j < mTexParam->TileSizeX; j++){			
			index = (i*(lsize))+j;
			tmpPix = getPixel(index);		
			setPixel((lsize*(lsize-j-1))+i, tmpPix, tmpData);
		}
	}

	FileData = tmpData;

	updateTexture(&mGlobalSettings.CurrentEditor->mPalette);

	return 0;
}


int Tile::updateTexture(TPalette* tpal){
	if(mTexParam->TileSetBPP < 0x8){
		for(int j = 0; j < 16; j++){
			if(PixelData.size()){		
				PixelData.erase(PixelData.begin(),PixelData.end());
			}

			for(int i = 0; i < (mTexParam->TileSizeX * mTexParam->TileSizeY); i++){
    			PixelData.push_back(tpal->mapPaletteColor(getPixel(i, j)));
			}			
    		SDL_UpdateTexture(TPOffset[j], NULL, PixelData.data(), mTexParam->TileSizeX * sizeof(Uint32));		
		}
	} else {
		TTexture::updateTexture(tpal);
	}

	return 0;
}


int Tile::createNew(TPalette* tpal){
	
	initTile();
	initTexture();
    FileData.resize(((mTexParam->TileSizeX * mTexParam->TileSizeY)/mGlobalSettings.mTileBPPSize[mTexParam->TileSetBPP]), 0);	
	updateTexture(tpal);
	
	return 0;
}

int Tile::initTexture(){
	if(mTexParam->TileSetBPP < 0x8){
		for(int i = 0; i < 16; i++){			
			TPOffset[i] = SDL_CreateTexture(mGlobalSettings.TRenderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, mTexParam->TileSizeX,mTexParam->TileSizeY);
			SDL_SetTextureBlendMode(TPOffset[i], SDL_BLENDMODE_BLEND);
		}

	} else {
		TTexture::initTexture();
	}
	
	return 0;
}



int Tile::initTile(){
	SDL_Rect eRect = {0,0,1,1};
	PixelAreas.resize(mTexParam->TileSizeX*mTexParam->TileSizeY,eRect);
	if(mTexParam->TileSetBPP < 0x8){
		TPOffset.resize(16, 0);
	}
	mSelection.init(mTexParam->TileSizeX, mTexParam->TileSizeY,mTexParam->TilePixelSize, mTexParam->TilePixelSize, &mTexParam->mTileEdScale);
	return 0;
}

/*
SDL_Rect Tile::render(int xpos, int ypos, int tscale, bool updateRect ,bool drawGrid){
	

	SDL_Rect tmpRect = TTexture::render(xpos, ypos, tscale, updateRect, drawGrid);
	if(bIsSelected){
		SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, mGlobalSettings.AltHighlightColor.r, mGlobalSettings.AltHighlightColor.g, mGlobalSettings.AltHighlightColor.b, 0xff); 
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &tmpRect);
		SDL_Rect sndRect = tmpRect;
		sndRect.x = sndRect.x-1;
		sndRect.y = sndRect.y-1;
		sndRect.w = sndRect.w+2;
		sndRect.h = sndRect.h+2;
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &sndRect);
	}
	return tmpRect;
}
*/

SDL_Rect Tile::renderImCol(int xpos, int ypos, int mIndex, int tscale, bool bColEditSelected){
SDL_Rect tmpRect;

	//ImGui::Image

	if(mTexParam->TileSetBPP < 0x8){
		ImGui::ImageButton((ImTextureID)(intptr_t)TPOffset[mTexParam->PaletteOffset], ImVec2(mTexParam->TileSizeX * tscale, mTexParam->TileSizeY * tscale) , ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1));	
	} else {
		ImGui::ImageButton((ImTextureID)(intptr_t)TileTex, ImVec2(mTexParam->TileSizeX * tscale, mTexParam->TileSizeY * tscale), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1));
	}

	ImVec2 elmin = ImGui::GetItemRectMin();
	ImVec2 elmax = ImGui::GetItemRectMax();

	tmpRect.x = elmin.x;
	tmpRect.y = elmin.y;
	tmpRect.w = elmax.x - elmin.x;
	tmpRect.h = elmax.y - elmin.y;
	
	ImDrawList *tList = ImGui::GetWindowDrawList();

	if(bColEditSelected){		
		tList->AddRect(elmin, elmax, mGlobalSettings.ImAltHighLightColor);
	} else {
		tList->AddRect(elmin, elmax, mGlobalSettings.ImHighLightColor);
	}

	return tmpRect;
}

SDL_Rect Tile::renderIm(int xpos, int ypos, int mIndex, int &mDragAndDropped, int tscale, bool updateRect ,bool drawGrid){
	SDL_Rect tmpRect;

	//ImGui::Image

	if(mTexParam->TileSetBPP < 0x8){
		ImGui::ImageButton((ImTextureID)(intptr_t)TPOffset[mTexParam->PaletteOffset], ImVec2(mTexParam->TileSizeX * tscale, mTexParam->TileSizeY * tscale) , ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1));	
	} else {
		ImGui::ImageButton((ImTextureID)(intptr_t)TileTex, ImVec2(mTexParam->TileSizeX * tscale, mTexParam->TileSizeY * tscale), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1));
	}

	int mode = 0;
	int n = mIndex;

	if(mGlobalSettings.CurrentEditor->bLCTRLisDown){
		mode = 1;
	}

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)){                    
        ImGui::SetDragDropPayload("DND_TILE", &n, sizeof(int));
        if (mode == 1) { ImGui::Text("Replace"); }                    
        if (mode == 0) { ImGui::Text("Swap"); }
        ImGui::EndDragDropSource();
    }

 	if (ImGui::BeginDragDropTarget()){
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_TILE")){
            IM_ASSERT(payload->DataSize == sizeof(int));
            int payload_n = *(const int*)payload->Data;			
			mDragAndDropped = payload_n;                     
        }				
	}

	ImVec2 elmin = ImGui::GetItemRectMin();
	ImVec2 elmax = ImGui::GetItemRectMax();

	tmpRect.x = elmin.x;
	tmpRect.y = elmin.y;
	tmpRect.w = elmax.x - elmin.x;
	tmpRect.h = elmax.y - elmin.y;
	
	ImDrawList *tList = ImGui::GetWindowDrawList();

	if(bIsSelected){		
		tList->AddRect(elmin, elmax, mGlobalSettings.ImAltHighLightColor);
	} else {
		tList->AddRect(elmin, elmax, mGlobalSettings.ImHighLightColor);
	}

	return tmpRect;
}

SDL_Rect Tile::render(int xpos, int ypos, int tscale,TileProperties tProps){
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	if((tProps.bFlipX) && (tProps.bFlipY)){
			flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
	} else {
	        if(tProps.bFlipX){
			flip = SDL_FLIP_HORIZONTAL;
		} 
		if(tProps.bFlipY){
			flip = SDL_FLIP_VERTICAL;
		}
	}

	SDL_Rect tmpRect;

	if(mTexParam->TileSetBPP < 0x8){		
		tmpRect = { xpos, ypos, mTexParam->TileSizeX*tscale, mTexParam->TileSizeY*tscale};
        SDL_RenderCopyEx(mGlobalSettings.TRenderer, TPOffset[tProps.mPaletteOffset], NULL, &tmpRect, 0, NULL, flip);	        
	} else {
		tmpRect = TTexture::renderEx(xpos, ypos, tscale, flip);
	}
	
	if(bIsSelected){
		SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, mGlobalSettings.AltHighlightColor.r, mGlobalSettings.AltHighlightColor.g, mGlobalSettings.AltHighlightColor.b, 0xff);
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &tmpRect);
		SDL_Rect sndRect = tmpRect;
		sndRect.x = sndRect.x-1;
		sndRect.y = sndRect.y-1;
		sndRect.w = sndRect.w+2;
		sndRect.h = sndRect.h+2;
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &sndRect);
	}

	return tmpRect;
}

int Tile::renderSelection(SDL_Rect &sRect, SDL_Color sColor){
	SDL_SetRenderDrawColor(mGlobalSettings.TRenderer,sColor.r,sColor.g,sColor.b, 0xff);
	SDL_RenderDrawRect(mGlobalSettings.TRenderer,&sRect);
	/*SDL_Rect tmpBorder;
	tmpBorder.x = sRect.x -1;
	tmpBorder.y = sRect.y -1;
	tmpBorder.w = sRect.w +2;
	tmpBorder.h = sRect.h +2;
	SDL_RenderDrawRect(mGlobalSettings.TRenderer,&tmpBorder);*/
	/*tmpBorder.x--;
	tmpBorder.y--;
	tmpBorder.w+=2;
	tmpBorder.h+=2;
	SDL_RenderDrawRect(mGlobalSettings.TRenderer,&tmpBorder);*/
	return 0;
}

void Tile::renderEdSel(int xpos, int ypos, TPalette* tpal, int cScale){
	for(int i=0; i < mTexParam->TileSizeY; i++){
		for(int j=0; j < mTexParam->TileSizeX; j++){
			PixelAreas[j+(mTexParam->TileSizeX*i)] = tpal->renderTileEd(xpos + (cScale)*j, ypos + (cScale)*i, getPixel(j+(i*mTexParam->TileSizeX)), cScale, mTexParam); 			//mGlobalSettings.TilePixelSize * 
		}
	}

}

void Tile::renderEd(int xpos, int ypos, TPalette* tpal){
	for(int i=0; i < mTexParam->TileSizeY; i++){
		for(int j=0; j < mTexParam->TileSizeX; j++){
			PixelAreas[j+(mTexParam->TileSizeX*i)] = tpal->renderTileEd(xpos + (mTexParam->TilePixelSize * mTexParam->mTileEdScale)*j, ypos + (mTexParam->TilePixelSize * mTexParam->mTileEdScale)*i, getPixel(j+(i*mTexParam->TileSizeX)), mTexParam); 			
		}
	}
	
	if(mGlobalSettings.CurrentEditor->mCurrentBrushPixel && !mGlobalSettings.CurrentEditor->mBrushesPixel.bIsEditing){
		if(mGlobalSettings.CurrentEditor->mCurrentBrushPixel->mSelected.size()){
			for(int i=0; i < mTexParam->TileSizeY; i++){
				for(int j=0; j < mTexParam->TileSizeX; j++){	
					if(mGlobalSettings.CurrentEditor->mCurrentBrushPixel->findInSelection((j+(i*mTexParam->TileSizeX))) != -1){
						int findex = mGlobalSettings.CurrentEditor->mCurrentBrushPixel->findInSelection((j+(i*mTexParam->TileSizeX)));
						if(mGlobalSettings.CurrentEditor->mCurrentBrushPixel->mBrushElements[findex] != -1){
							tpal->renderTileEd(xpos + (mTexParam->TilePixelSize * mTexParam->mTileEdScale)*j, ypos + (mTexParam->TilePixelSize * mTexParam->mTileEdScale)*i, mGlobalSettings.CurrentEditor->mCurrentBrushPixel->mBrushElements[findex], mTexParam); 							
						}
					}
				}
			}
		}
	}
	
	if(mSelection.bHasSelection){
		mSelection.renderSelection(xpos, ypos);
	}

}

int Tile::loadFromFile(std::string filename,TPalette* tpal){ 
	initTile();	
	return TTexture::loadFromFile(filename,tpal);
}

int Tile::replaceWithBuffer(std::vector<unsigned char> &cTileBuf,TPalette* tpal){
	FileData = cTileBuf;
	updateTexture(tpal);
	return 0;
}

int Tile::loadFromBuffer(std::vector<unsigned char> &cTileBuf,TPalette* tpal){ 
	initTile();	
	TTexture::loadFromBuffer(cTileBuf,tpal);
	initTexture();
	updateTexture(tpal);
	return 0;
}

void TileSet::close(){
	for(auto *dTile: TTiles){
		dTile->freeTexture();
	}
	TTiles.clear();
}

void TileSet::shutdown(){
	for(auto *dTile: TTiles){
		dTile->freeTexture();
	}
}

Tile* TileSet::createNewFromFile(std::string newPath, TPalette* tpal){
	if(fs::exists(fs::status(newPath))){
		if(fs::is_directory(fs::status(newPath))){
			return NULL;
		}

		SDL_Surface *newSurf = IMG_Load(newPath.c_str());
		if(newSurf && (mGlobalSettings.mGlobalTexParam.TileSetBPP > 0x2)){
			if(newSurf->format->BitsPerPixel == 8){
				if((newSurf->w == mGlobalSettings.mGlobalTexParam.TileSizeX) && (newSurf->h == mGlobalSettings.mGlobalTexParam.TileSizeY)){
					std::vector<unsigned char> tbuffer;
					if(mGlobalSettings.mGlobalTexParam.TileSetBPP == 0x4){
						unsigned char tmpChar;
						bool sndPix=false;
						for(int i = 0; i < (mGlobalSettings.mGlobalTexParam.TileSizeX*mGlobalSettings.mGlobalTexParam.TileSizeY); i++){
							if(sndPix){
								tmpChar = (tmpChar << 4) + ((unsigned char*)(newSurf->pixels))[i]%16;
								tbuffer.push_back(tmpChar);
								sndPix = false;
							} else {
								tmpChar = ((unsigned char*)(newSurf->pixels))[i]%16;
								sndPix = true;
							}
							
						}
					} else {
						for(int i = 0; i < (mGlobalSettings.mGlobalTexParam.TileSizeX*mGlobalSettings.mGlobalTexParam.TileSizeY); i++){
							tbuffer.push_back(((unsigned char*)(newSurf->pixels))[i]);
						}
					}
					SDL_FreeSurface(newSurf);
					return createNewFromBuffer(tbuffer, tpal);
				}
			}
			SDL_FreeSurface(newSurf);
			return NULL;

		} else {
			std::ifstream infile(newPath, std::ios::binary );

			std::vector<unsigned char> tbuffer(std::istreambuf_iterator<char>(infile), {});

			if(tbuffer.size() == ((mGlobalSettings.mGlobalTexParam.TileSizeX * mGlobalSettings.mGlobalTexParam.TileSizeY)/mGlobalSettings.mTileBPPSize[mGlobalSettings.mGlobalTexParam.TileSetBPP])){
				return createNewFromBuffer(tbuffer, tpal);
			} else {
				return NULL;				
			}
		}
	}
	return NULL;
}

int TileSet::importTileSet(std::string cTilePath, std::vector<Tile*> &cNewTiles){
	if(fs::exists(fs::status(cTilePath))){
		if(fs::is_directory(fs::status(cTilePath))){
			return 1;
		}

		int mTileNum = TTiles.size();

		SDL_Surface *newSurf = IMG_Load(cTilePath.c_str());
		if(newSurf && (mGlobalSettings.mGlobalTexParam.TileSetBPP > 0x2)){
			if(importPNG(newSurf,  &mGlobalSettings.CurrentEditor->mPalette)){
				std::cout << "Error Importing TileSet: " << cTilePath << std::endl;
				return 1;
			} else {
				for(int i = mTileNum; i < TTiles.size(); i++){
					cNewTiles.push_back(TTiles[i]);
				}
			}
		} else {
			fs::path cNewPath = cTilePath;			
			if(importFile(cNewPath.parent_path().string(), cNewPath.filename().string(), &mGlobalSettings.CurrentEditor->mPalette)){
				std::cout << "Error Importing TileSet: " << cTilePath << std::endl;
				return 1;
			} else {
				for(int i = mTileNum; i < TTiles.size(); i++){
					cNewTiles.push_back(TTiles[i]);
				}
			}
		}
	}

	return 0;
}

int TileSet::importPNG(SDL_Surface *newSurf, TPalette* tpal){
	if(newSurf->format->BitsPerPixel == 8){
		if(!(newSurf->w % mGlobalSettings.mGlobalTexParam.TileSizeX) && !(newSurf->h % mGlobalSettings.mGlobalTexParam.TileSizeY)){
			int cTilesX,cTilesY;
			cTilesX = newSurf->w / mGlobalSettings.mGlobalTexParam.TileSizeX;
			cTilesY = newSurf->h / mGlobalSettings.mGlobalTexParam.TileSizeY;
			
			std::cout << "Import PNG: " << cTilesX << "," <<  cTilesY << std::endl;

			std::vector<std::vector<unsigned char>> tbuffers;

			tbuffers.resize(cTilesX*cTilesY);

			for(auto &tbuf : tbuffers){
				tbuf.resize((mGlobalSettings.mGlobalTexParam.TileSizeX*mGlobalSettings.mGlobalTexParam.TileSizeY) / mGlobalSettings.mTileBPPSize[mGlobalSettings.mGlobalTexParam.TileSetBPP],0);
			}

			Tile tmpTile;
			int cBufIndex;
			int cPixIndex;

			for(int ii=0; ii < newSurf->h; ii++){
				for(int jj=0; jj < newSurf->w; jj++){
					cBufIndex = mSelection.getTileIndex((ii*newSurf->w)+jj, newSurf->w, newSurf->h, cPixIndex, &mGlobalSettings.mGlobalTexParam);					
					tmpTile.setPixel(cPixIndex, ((unsigned char*)(newSurf->pixels))[(ii*newSurf->w)+jj], tbuffers[cBufIndex]);
				}
			}

			for(auto &tbufl : tbuffers){
				createNewFromBuffer(tbufl, tpal);
			}

			TileAreas.resize(TTiles.size());

			resizeEdit();
			
			SDL_FreeSurface(newSurf);
			return 0;
		}
	}
			
	SDL_FreeSurface(newSurf);
	return 1;
}

Tile* TileSet::createNewFromBuffer(std::vector<unsigned char> &newBuf, TPalette* tpal){

	Tile *newTile = new Tile();
	SDL_Rect newRect;

	newTile->loadFromBuffer(newBuf, tpal);
	TileAreas.push_back(newRect);
	TTiles.push_back(newTile);
	resizeEdit();
	return newTile;

}

Tile* TileSet::createNewCopy(Tile* cCopyTile, TPalette* tpal){	
	return createNewFromBuffer(cCopyTile->FileData, tpal);
}

Tile* TileSet::createNew(TPalette* tpal){	
	std::vector<unsigned char> tbuf;	
	tbuf.resize(((mGlobalSettings.mGlobalTexParam.TileSizeX * mGlobalSettings.mGlobalTexParam.TileSizeY)/mGlobalSettings.mTileBPPSize[mGlobalSettings.mGlobalTexParam.TileSetBPP]),0);
	return createNewFromBuffer(tbuf, tpal);
}

int TileSet::deleteTile(int cDropTile){
	Tile* dTile = *(TTiles.begin() +  cDropTile); 
	TTiles.erase(TTiles.begin() +  cDropTile);
	TileAreas.erase(TileAreas.begin() + cDropTile);
	delete dTile;
	resizeEdit();
	return 0;
}

int TileSet::removeTile(int cDropTile){
	Tile* dTile = *(TTiles.begin() +  cDropTile); 
	TTiles.erase(TTiles.begin() +  cDropTile);
	TileAreas.erase(TileAreas.begin() + cDropTile);	
	resizeEdit();
	return 0;
}

void TileSet::dropLastTile(){
	(*(TTiles.end()-1))->bIsSelected = false;	
	TTiles.pop_back();
	TileAreas.pop_back();
	reCalculateScale();
	resizeEdit();
}

void TileSet::appendTile(Tile* addTile){
	TTiles.push_back(addTile);
	SDL_Rect newRect;
	TileAreas.push_back(newRect);
	reCalculateScale();
	resizeEdit();
}

int TileSet::importFile(std::string tpath,std::string tfile, TPalette* tpal){
	fs::path cTileFile;
	int ntiles=0;

	if(fs::is_directory(fs::status(tpath))){
		cTileFile = tpath + DIRDEL + tfile;
	} else {
		std::cout << "Folder Not Found: " << tpath << std::endl;
		return 1;		
	}

	if(!fs::exists(fs::status(cTileFile))){
		std::cout << "File Not Found: " << cTileFile << std::endl;
		return 1;
	}

	std::ifstream infile(cTileFile, std::ios::binary );

	std::vector<unsigned char> tbuffer(std::istreambuf_iterator<char>(infile), {});

	int magic1 = tbuffer[0];
    int magic2 = tbuffer[1];

	int twidth;
    int theight;

	if( (magic1 == 0x0) ||(magic1 == 0x1) ||(magic1 == 0x10) ||(magic1 == 0x11)  ){
		twidth = ((magic1 & 0xf0) >> 4);
		theight = (magic1 & 0x0f);
	} else {
		std::cout << "Error in Tile File: " << cTileFile << std::endl;
		return 1;	
	}

	if((mTileSizeIn[twidth] != mGlobalSettings.mGlobalTexParam.TileSizeX) || (mTileSizeIn[theight] != mGlobalSettings.mGlobalTexParam.TileSizeY)){
		std::cout << "Wrong Tile Size: " << cTileFile << std::endl;
		return 1;	
	}

	if((magic2 != mGlobalSettings.mGlobalTexParam.TileSetBPP) ){
		std::cout << "Wrong Tile BPP: " << cTileFile << std::endl;
		return 1;	
	}

	int tmpTileSize = ((mGlobalSettings.mGlobalTexParam.TileSizeX * mGlobalSettings.mGlobalTexParam.TileSizeY)/mGlobalSettings.mTileBPPSize[mGlobalSettings.mGlobalTexParam.TileSetBPP]);

	tbuffer.erase(tbuffer.begin());
	tbuffer.erase(tbuffer.begin());

	if(tbuffer.size() % tmpTileSize){
		std::cout << "Error in Tile File: " << cTileFile << std::endl;
		return 1;
	}

	ntiles = tbuffer.size() / tmpTileSize;

	Tile *mTile;

	for(int tCount = 0; tCount < ntiles; tCount++){
		mTile = new Tile();
		std::vector<unsigned char>::const_iterator first = tbuffer.begin() + (tCount * tmpTileSize);
		std::vector<unsigned char>::const_iterator last = tbuffer.begin() + ((tCount * tmpTileSize) + (tmpTileSize));
		std::vector<unsigned char> tbuffer2(first, last);
		mTile->loadFromBuffer(tbuffer2 ,tpal);
		TTiles.push_back(mTile);
	}

	TileAreas.resize(TTiles.size());
	resizeEdit();

	return 0;
}

int TileSet::loadFromFile(std::string path,std::string filename, TPalette* tpal){ 

	fs::path cTileFile;
	int ntiles=0;

	if(fs::is_directory(fs::status(path))){
		cTileFile = path + DIRDEL + filename;
	} else {
		std::cout << "Folder Not Found: " << path << std::endl;
		return 1;		
	}

	if(!fs::exists(fs::status(cTileFile))){
		std::cout << "File Not Found: " << cTileFile << std::endl;
		return 1;
	}

	std::ifstream infile(cTileFile, std::ios::binary );

	std::vector<unsigned char> tbuffer(std::istreambuf_iterator<char>(infile), {});

	int magic1 = tbuffer[0];
    int magic2 = tbuffer[1];

	int twidth;
    int theight;

	if( (magic1 == 0x0) ||(magic1 == 0x1) ||(magic1 == 0x10) ||(magic1 == 0x11)  ){
		twidth = ((magic1 & 0xf0) >> 4);
		theight = (magic1 & 0x0f);
	} else {
		std::cout << "Error in Tile File: " << cTileFile << std::endl;
		return 1;	
	}

	mGlobalSettings.mGlobalTexParam.TileSizeX = mTileSizeIn[twidth];
	mGlobalSettings.mGlobalTexParam.TileSizeY = mTileSizeIn[theight];

	if( (magic2 == 0x8) ||(magic2 == 0x4) ||(magic2 == 0x2) ){
		mGlobalSettings.mGlobalTexParam.TileSetBPP = magic2;
	} else {
		std::cout << "Error in Tile File: " << cTileFile << std::endl;
		return 1;	
	}

	int tmpTileSize = ((mGlobalSettings.mGlobalTexParam.TileSizeX * mGlobalSettings.mGlobalTexParam.TileSizeY)/mGlobalSettings.mTileBPPSize[mGlobalSettings.mGlobalTexParam.TileSetBPP]);

	tbuffer.erase(tbuffer.begin());
	tbuffer.erase(tbuffer.begin());

	if(tbuffer.size() % tmpTileSize){
		std::cout << "Error in Tile File: " << cTileFile << std::endl;
		return 1;
	}

	ntiles = tbuffer.size() / tmpTileSize;
	TileAreas.resize(ntiles);

	Tile *mTile;

	for(int tCount = 0; tCount < ntiles; tCount++){
		mTile = new Tile();
		std::vector<unsigned char>::const_iterator first = tbuffer.begin() + (tCount * tmpTileSize);
		std::vector<unsigned char>::const_iterator last = tbuffer.begin() + ((tCount * tmpTileSize) + (tmpTileSize));
		std::vector<unsigned char> tbuffer2(first, last);
		mTile->loadFromBuffer(tbuffer2 ,tpal);
		TTiles.push_back(mTile);
	}

	resizeEdit();

	return 0;
}

int TileSet::saveToFolder(std::string tpath){
	std::ofstream outfile(tpath+DIRDEL+"tiles.bin", std::ios::binary );

	std::vector<unsigned char> obuffer;

	int magic1, magic2;

	magic1 = mTileSizeOut[mGlobalSettings.mGlobalTexParam.TileSizeX];
	magic1 = (magic1 << 4) + mTileSizeOut[mGlobalSettings.mGlobalTexParam.TileSizeY];

	magic2 = mGlobalSettings.mGlobalTexParam.TileSetBPP;

	obuffer.push_back(magic1);
	obuffer.push_back(magic2);

	for(auto &mTile : TTiles){		
		obuffer.insert(obuffer.end(), mTile->FileData.begin(), mTile->FileData.end());
	}

	outfile.write((char*)obuffer.data(),obuffer.size());
	outfile.close();

	return 0;
}

int TileSet::reCalculateScale(){
	if(mCurColumns > 1){
		mTileSetBackGround.h = mGlobalSettings.WindowHeight-mGlobalSettings.TopBarHeight;
		if( (int)( (float)( ( ( ((mCurTileScale+1)*mGlobalSettings.mGlobalTexParam.TileSizeX )+ mColSpace ) * TTiles.size() )  / ( mCurColumns - 1 ) ) ) < mTileSetBackGround.h ){
			mCurTileScale++;
			mCurColumns--;
			return 1;		
		}
	}
	return 0;
}

std::vector<int> TileSet::getPadding(){
	int isOdd = TTiles.size() % mSelEdWidth;
	int cRowNum = TTiles.size() / mSelEdWidth;

	std::vector<int> cPadding;
	
	if(isOdd){		
		for(int i = 0 ; i < (mSelEdWidth-isOdd); i++){
			for (int yy = 0; yy < mGlobalSettings.mGlobalTexParam.TileSizeY; yy++){
				for (int xx = 0; xx < mGlobalSettings.mGlobalTexParam.TileSizeX; xx++){				
					cPadding.push_back(getXY(yy, xx,  i + isOdd, cRowNum));					
				}				
			}			
		}
	}
	return cPadding;	
}


void TileSet::resizeEdit(){
	int isOdd = TTiles.size() % mSelEdWidth;
	int cRowNum = TTiles.size() / mSelEdWidth;

	int cPad = 0;

	int dummy = 1;

	if(isOdd){cPad = 1;}

	SDL_Rect rEmpty;

	rEmpty.x = 0;
	rEmpty.y = 0;
	rEmpty.w = 0;
	rEmpty.h = 0;

	mSelectionAreaX = mSelEdWidth * mGlobalSettings.mGlobalTexParam.TileSizeX;
	mSelectionAreaY = mGlobalSettings.mGlobalTexParam.TileSizeY * (cRowNum + cPad);
	
	EditPixelAreas.resize(mSelectionAreaX*mSelectionAreaY);

	for(int i = 0; i <  EditPixelAreas.size(); i++){
		EditPixelAreas[i].x = rEmpty.x;
		EditPixelAreas[i].y = rEmpty.y;
		EditPixelAreas[i].w = rEmpty.w;
		EditPixelAreas[i].h = rEmpty.h;
	}
	
	mSelection.clearSelection();	
	mSelection.init(mSelectionAreaX , mSelectionAreaY, 1, 1, &mCurEdScale);	
}

int TileSet::getXY(int xpos, int ypos, int cxpos, int cypos){
	int index;
	int lineL = mSelectionAreaX; 
	int fullL = lineL * mGlobalSettings.mGlobalTexParam.TileSizeY;
	index = xpos + (ypos * lineL) + (cxpos * mGlobalSettings.mGlobalTexParam.TileSizeX) + (cypos * fullL);		
	return index;
}

void TileSet::updateEditAreas(std::vector<SDL_Rect> &cTile, int xpos, int ypos){
	for(int i = 0; i < mGlobalSettings.mGlobalTexParam.TileSizeY; i++){
		for(int j = 0; j < mGlobalSettings.mGlobalTexParam.TileSizeX; j++){
			EditPixelAreas[getXY(j,i, xpos, ypos)] = cTile[(i*mGlobalSettings.mGlobalTexParam.TileSizeX)+j];
		}
	}
}


int TileSet::renderEd(int xpos, int ypos){

	if(bUpdateEditSelectionScale){
		mCurEdScale = mGlobalSettings.mTileSetEditScale;
	}

	if(bUpdateEditSelection){
		
		bUpdateEditSelection = false;

		if(mGlobalSettings.mTileSetEditWidth > TTiles.size()){
			mSelEdWidth = TTiles.size();
		} else {
			mSelEdWidth = mGlobalSettings.mTileSetEditWidth;
		}

		resizeEdit();

		mSelection.resize(mSelectionAreaX, mSelectionAreaY, 1, 1, &mCurEdScale);

	} 

	int isOdd = TTiles.size() % mSelEdWidth;
	int cRowNum = TTiles.size() / mSelEdWidth;
	
	SDL_Rect cBorder;
		for(int i = 0; i < mSelEdWidth; i++){
			for(int j = 0; j <  cRowNum; j++){
				int cxpos = xpos +  (mCurEdScale*mGlobalSettings.mGlobalTexParam.TileSizeX)*i;
				int cypos = ypos + (mGlobalSettings.mGlobalTexParam.TileSizeY*mCurEdScale)*j;

				for(int ii=0; ii < mGlobalSettings.mGlobalTexParam.TileSizeY; ii++){
					for(int jj=0; jj < mGlobalSettings.mGlobalTexParam.TileSizeX; jj++){
						EditPixelAreas[getXY(jj,ii, i, j)] = mGlobalSettings.CurrentEditor->mPalette.renderTileEd(cxpos + (mCurEdScale)*jj, cypos + (mCurEdScale)*ii, TTiles[(j*mSelEdWidth)+i]->getPixel(jj+(ii*mGlobalSettings.mGlobalTexParam.TileSizeX)), mCurEdScale, &mGlobalSettings.mGlobalTexParam); 			//mGlobalSettings.TilePixelSize * 
					}
				}
			
				if(mGlobalSettings.bShowTileGrid){				
					cBorder.x = xpos + ((mCurEdScale*mGlobalSettings.mGlobalTexParam.TileSizeX)*i);
					cBorder.y = ypos + ((mGlobalSettings.mGlobalTexParam.TileSizeY*mCurEdScale)*j);
					cBorder.w = (mCurEdScale*mGlobalSettings.mGlobalTexParam.TileSizeX);
					cBorder.h = (mCurEdScale*mGlobalSettings.mGlobalTexParam.TileSizeY);
					Tile::renderSelection(cBorder, mGlobalSettings.DefaultHighlightColor);
				}				
			}								
		}	
		if(isOdd){			
			int i = mSelEdWidth;
			for(int j = 0; j < isOdd; j++){			
				int cxpos = xpos +  (mCurEdScale*mGlobalSettings.mGlobalTexParam.TileSizeX)*j;
				int cypos = ypos + (mGlobalSettings.mGlobalTexParam.TileSizeY*mCurEdScale)*cRowNum;

				for(int ii=0; ii < mGlobalSettings.mGlobalTexParam.TileSizeY; ii++){
					for(int jj=0; jj < mGlobalSettings.mGlobalTexParam.TileSizeX; jj++){
						EditPixelAreas[getXY(jj,ii, j, cRowNum)] = mGlobalSettings.CurrentEditor->mPalette.renderTileEd(cxpos + (mCurEdScale)*jj, cypos + (mCurEdScale)*ii, TTiles[(i*cRowNum)+j]->getPixel(jj+(ii*mGlobalSettings.mGlobalTexParam.TileSizeX)), mCurEdScale, &mGlobalSettings.mGlobalTexParam); 			//mGlobalSettings.TilePixelSize * 
					}
				}

				if(mGlobalSettings.bShowTileGrid){	
					cBorder.x = xpos + ((mCurEdScale*mGlobalSettings.mGlobalTexParam.TileSizeX)*j);
					cBorder.y = ypos + ((mGlobalSettings.mGlobalTexParam.TileSizeY*mCurEdScale)*cRowNum);
					cBorder.w = (mCurEdScale*mGlobalSettings.mGlobalTexParam.TileSizeX);
					cBorder.h = (mCurEdScale*mGlobalSettings.mGlobalTexParam.TileSizeY);
					Tile::renderSelection(cBorder, mGlobalSettings.DefaultHighlightColor);
				}
			}
		}

		if(mGlobalSettings.CurrentEditor->mCurrentBrushPixelTileSet && !mGlobalSettings.CurrentEditor->mBrushesPixel.bIsEditing){
			if(mGlobalSettings.CurrentEditor->mCurrentBrushPixelTileSet->mSelected.size()){
				for(int i = 0; i < mSelEdWidth; i++){
					for(int j = 0; j <  cRowNum; j++){
						int cxpos = xpos +  (mCurEdScale*mGlobalSettings.mGlobalTexParam.TileSizeX)*i;
						int cypos = ypos + (mGlobalSettings.mGlobalTexParam.TileSizeY*mCurEdScale)*j;

						for(int ii=0; ii < mGlobalSettings.mGlobalTexParam.TileSizeY; ii++){
							for(int jj=0; jj < mGlobalSettings.mGlobalTexParam.TileSizeX; jj++){
								if(mGlobalSettings.CurrentEditor->mCurrentBrushPixelTileSet->findInSelection(getXY(jj,ii, i, j)) != -1){
									int findex = mGlobalSettings.CurrentEditor->mCurrentBrushPixelTileSet->findInSelection(getXY(jj,ii, i, j));
									if(mGlobalSettings.CurrentEditor->mCurrentBrushPixelTileSet->mBrushElements[findex] != -1){
										mGlobalSettings.CurrentEditor->mPalette.renderTileEd(cxpos + (mCurEdScale)*jj, cypos + (mCurEdScale)*ii, mGlobalSettings.CurrentEditor->mCurrentBrushPixelTileSet->mBrushElements[findex], mCurEdScale, &mGlobalSettings.mGlobalTexParam); 
									}
								}
							}					
						}								
					}	
				}
		
				if(isOdd){			
					int i = mSelEdWidth;
					for(int j = 0; j < isOdd; j++){			
						int cxpos = xpos +  (mCurEdScale*mGlobalSettings.mGlobalTexParam.TileSizeX)*j;
						int cypos = ypos + (mGlobalSettings.mGlobalTexParam.TileSizeY*mCurEdScale)*cRowNum;

						for(int ii=0; ii < mGlobalSettings.mGlobalTexParam.TileSizeY; ii++){
							for(int jj=0; jj < mGlobalSettings.mGlobalTexParam.TileSizeX; jj++){
								if(mGlobalSettings.CurrentEditor->mCurrentBrushPixelTileSet->findInSelection(getXY(jj,ii, j, cRowNum)) != -1){
									int findex = mGlobalSettings.CurrentEditor->mCurrentBrushPixelTileSet->findInSelection(getXY(jj,ii, j, cRowNum));
									if(mGlobalSettings.CurrentEditor->mCurrentBrushPixelTileSet->mBrushElements[findex] != -1){
										mGlobalSettings.CurrentEditor->mPalette.renderTileEd(cxpos + (mCurEdScale)*jj, cypos + (mCurEdScale)*ii, mGlobalSettings.CurrentEditor->mCurrentBrushPixelTileSet->mBrushElements[findex], mCurEdScale, &mGlobalSettings.mGlobalTexParam); 
									}
								}							
							}
						}					
					}
				}			
			}
		}
	

	if(mSelection.bHasSelection){
		mSelection.renderSelection(xpos, ypos);
	}		

	return 0;
}


/*
int TileSet::render(int ypos, int mScroll){
	
	mTileSetBackGround.h = mGlobalSettings.WindowHeight- mGlobalSettings.TopBarHeight;
	
	if(mCurColumns < 4){
		if( (int)( (float)( ( ( (mCurTileScale*mGlobalSettings.mGlobalTexParam.TileSizeX ) +mColSpace ) * TTiles.size() )  / mCurColumns ) ) > mTileSetBackGround.h ){	
			mCurTileScale--;
		
			if(mCurTileScale < 5){
				mCurColumns++;		
			}			
		}
	}

	mGlobalSettings.TileSetWidth = (((mCurTileScale*mGlobalSettings.mGlobalTexParam.TileSizeX)+mColSpace)*mCurColumns)+(mColSpace*3);

	int isOdd = TTiles.size() % mCurColumns;
	int cRowNum = TTiles.size() / mCurColumns;

	mTileSetBackGround.x = mGlobalSettings.WindowWidth - mGlobalSettings.TileSetWidth;
	mTileSetBackGround.y = ypos;
	mTileSetBackGround.w = mGlobalSettings.TileSetWidth; 

	SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, mGlobalSettings.DefaultBGColor.r,  mGlobalSettings.DefaultBGColor.g,  mGlobalSettings.DefaultBGColor.b, 0xff);
	SDL_RenderFillRect(mGlobalSettings.TRenderer, &mTileSetBackGround);
	
	
	if(mCurColumns > 0){
		for(int i = 0; i < mCurColumns; i++){
			for(int j = 0; j < cRowNum; j++){
				TileAreas[(i * cRowNum) + j] = TTiles[(i*cRowNum)+j]->render((mTileSetBackGround.x+ (mColSpace*2) +  ((mCurTileScale*mGlobalSettings.mGlobalTexParam.TileSizeX)+mColSpace)*i),mTileSetBackGround.y + mScroll + (mColSpace*2) + (((mGlobalSettings.mGlobalTexParam.TileSizeY*mCurTileScale)+mColSpace)*j), mCurTileScale,true,true);
			}								
		}	
		if(isOdd){			
			int i = mCurColumns;
			for(int j = 0; j < isOdd; j++){
				TileAreas[(i * cRowNum) + j] = TTiles[(i*cRowNum)+j]->render((mTileSetBackGround.x+ (mColSpace*2) +  ((mCurTileScale*mGlobalSettings.mGlobalTexParam.TileSizeX)+mColSpace)*j),mTileSetBackGround.y + mScroll + (mColSpace*2) + (((mGlobalSettings.mGlobalTexParam.TileSizeY*mCurTileScale)+mColSpace)*cRowNum), mCurTileScale,true,true);
			}
		}
	}


	
	int cMax = (int)( (float)( ( ( (mCurTileScale*mGlobalSettings.mGlobalTexParam.TileSizeY ) +mColSpace ) * TTiles.size() )  / mCurColumns )) + (4 * mGlobalSettings.mGlobalTexParam.TileSizeY);
	if((cMax - mTileSetBackGround.h) > 0 ){
		mMaxScrollY = -(cMax - mTileSetBackGround.h);
	} else {
		mMaxScrollY = 0;
	}
	
	SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, mGlobalSettings.DefaultGUIBorderColor.r,mGlobalSettings.DefaultGUIBorderColor.g,mGlobalSettings.DefaultGUIBorderColor.b,0xff);
	SDL_RenderDrawRect(mGlobalSettings.TRenderer, &mTileSetBackGround);
	
	mTileSetBackGround.x = mTileSetBackGround.x + 1;
	mTileSetBackGround.y = mTileSetBackGround.y + 1;
	mTileSetBackGround.w = mTileSetBackGround.w - 2; 
	mTileSetBackGround.h = mTileSetBackGround.h - 2;

	SDL_RenderDrawRect(mGlobalSettings.TRenderer, &mTileSetBackGround);

	mTileSetBackGround.x = mTileSetBackGround.x + 1;
	mTileSetBackGround.y = mTileSetBackGround.y + 1;
	mTileSetBackGround.w = mTileSetBackGround.w - 2; 
	mTileSetBackGround.h = mTileSetBackGround.h - 2;

	SDL_RenderDrawRect(mGlobalSettings.TRenderer, &mTileSetBackGround);


return 0;
}
*/

int TileSet::renderIm(int ypos, int mScroll){
	
	mTileSetBackGround.h = mGlobalSettings.WindowHeight- mGlobalSettings.TopBarHeight;
	
	if(mCurColumns < 4){
		if( (int)( (float)( ( ( (mCurTileScale*mGlobalSettings.mGlobalTexParam.TileSizeX ) +mColSpace ) * TTiles.size() )  / mCurColumns ) ) > mTileSetBackGround.h ){	
			mCurTileScale--;
		
			if(mCurTileScale < 5){
				mCurColumns++;
				updateWinPos = true;
			}			
		}
	}

	mGlobalSettings.TileSetWidth = (((mCurTileScale*mGlobalSettings.mGlobalTexParam.TileSizeX)+mColSpace)*mCurColumns)+(mColSpace*3);

	int isOdd = TTiles.size() % mCurColumns;
	int cRowNum = TTiles.size() / mCurColumns;

	mTileSetBackGround.x = mGlobalSettings.WindowWidth - mGlobalSettings.TileSetWidth;
	mTileSetBackGround.y = ypos;
	mTileSetBackGround.w = mGlobalSettings.TileSetWidth; 

	ImVec2 cWinPos;
	cWinPos.x = mTileSetBackGround.x;
	cWinPos.y = mTileSetBackGround.y;
	

	ImVec2 cWinSize;
	cWinSize.x = mTileSetBackGround.w;
	cWinSize.y = mTileSetBackGround.h;

	if(updateWinPos){
		ImGui::SetNextWindowPos(cWinPos, ImGuiCond_Always);
		ImGui::SetNextWindowSize(cWinSize, ImGuiCond_Always);
		updateWinPos = false;
	} else {
		ImGui::SetNextWindowPos(cWinPos, ImGuiCond_Once);
		ImGui::SetNextWindowSize(cWinSize, ImGuiCond_Once);
	}

	ImGui::Begin("TileSet", NULL, ImGuiWindowFlags_NoNav);    

	if(ImGui::Button("Move Up")){
		mGlobalSettings.CurrentEditor->moveTileUp();
	}

	if(ImGui::Button("Move Down")){
		mGlobalSettings.CurrentEditor->moveTileDown();
	}

	bool bIsDragged = false;
	int mDragged = -1;
	int mDragSource = 0;
	int mDragTarget = 0;

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(mGlobalSettings.DefaultBGColor.r,mGlobalSettings.DefaultBGColor.g,mGlobalSettings.DefaultBGColor.b)));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(mGlobalSettings.DefaultBGColor.r,mGlobalSettings.DefaultBGColor.g,mGlobalSettings.DefaultBGColor.b)));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(mGlobalSettings.DefaultBGColor.r,mGlobalSettings.DefaultBGColor.g,mGlobalSettings.DefaultBGColor.b)));
	
	if(mCurColumns > 0){
		for(int i = 0; i < cRowNum; i++){
			for(int j = 0; j < mCurColumns; j++){
				TileAreas[(i * mCurColumns) + j] = TTiles[(i*mCurColumns) + j]->renderIm((mTileSetBackGround.x+ (mColSpace*2) +  ((mCurTileScale*mGlobalSettings.mGlobalTexParam.TileSizeX)+mColSpace)*j),mTileSetBackGround.y + mScroll + (mColSpace*2) + (((mGlobalSettings.mGlobalTexParam.TileSizeY*mCurTileScale)+mColSpace)*i), (i*mCurColumns) + j, mDragged, mCurTileScale,true,true);								
				if((mDragged > -1) && !bIsDragged){
					bIsDragged = true;
					mDragSource = mDragged;
					mDragTarget = (i*mCurColumns) + j;
				}
				if((mCurColumns > 1) && (j < (mCurColumns-1))){					
					ImGui::SameLine();
				} 
			}										
		}	
		
		if(isOdd){			
			int i = mCurColumns;
			for(int j = 0; j < isOdd; j++){
				TileAreas[(i * cRowNum) + j] = TTiles[(i*cRowNum)+j]->renderIm((mTileSetBackGround.x+ (mColSpace*2) +  ((mCurTileScale*mGlobalSettings.mGlobalTexParam.TileSizeX)+mColSpace)*j),mTileSetBackGround.y + mScroll + (mColSpace*2) + (((mGlobalSettings.mGlobalTexParam.TileSizeY*mCurTileScale)+mColSpace)*cRowNum), (i*cRowNum)+j, mDragged,  mCurTileScale,true,true);				
				if((mDragged > -1) && !bIsDragged){
					bIsDragged = true;
					mDragSource = mDragged;
					mDragTarget = (i*cRowNum)+j;
				}				
				if((j < (isOdd-1))){
					ImGui::SameLine();
				}
			}

		}		
	}

	ImGui::PopStyleColor(3);

	if(bIsDragged){		
		mGlobalSettings.CurrentEditor->swapTiles(mDragSource, mDragTarget, true);
	}

	mGlobalSettings.CurrentEditor->ImButtonsTileSet.updateButtonStates();
	
    ImGui::End();


	int cMax = (int)( (float)( ( ( (mCurTileScale*mGlobalSettings.mGlobalTexParam.TileSizeY ) +mColSpace ) * TTiles.size() )  / mCurColumns )) + (4 * mGlobalSettings.mGlobalTexParam.TileSizeY);
	if((cMax - mTileSetBackGround.h) > 0 ){
		mMaxScrollY = -(cMax - mTileSetBackGround.h);
	} else {
		mMaxScrollY = 0;
	}

return 0;
}

void TileMap::init(){
	mSelection.init(TileMapWidth, TileMapHeight, mGlobalSettings.mGlobalTexParam.TileSizeX, mGlobalSettings.mGlobalTexParam.TileSizeY, &mGlobalSettings.TileMapScale);
}


int TileMap::createNew(int nWidth, int nHeight, int nSValue, int cPaletteOffset){

	createNew(nWidth, nHeight, nSValue);

	for(int i = 0; i < TileMapHeight; i++){
		for(int j = 0; j < TileMapWidth; j++){
			setOffset((i * TileMapWidth) + j, cPaletteOffset);
		}
	}

	return 0;
}

int TileMap::createNew(int nWidth, int nHeight, int nSValue){
	FileData.resize(nWidth * nHeight * 2, 0);
	TileAreas.resize(nWidth * nHeight);
	TileMapWidth = nWidth;
	TileMapHeight = nHeight;

	init();

	for(int i = 0; i < TileMapHeight; i++){
		for(int j = 0; j < TileMapWidth; j++){
			setTile((i * TileMapWidth) + j, nSValue);
		}
	}

	return 0;
}

int TileMap::createNew(){

	FileData.resize(mGlobalSettings.TileMapWidth * mGlobalSettings.TileMapHeight * 2, 0);
	TileAreas.resize(mGlobalSettings.TileMapWidth * mGlobalSettings.TileMapHeight);
	TileMapHeight = mGlobalSettings.TileMapHeight;
	TileMapWidth = mGlobalSettings.TileMapWidth;
	
	init();

	return 0;
}

int TileMap::createCollisionMap(){
	if(bHasCollisionMap){
		return 1;
	}
	
	bHasCollisionMap = true;
	mColMap.createNew(this);

	return 0;
}
int TileMap::removeCollisionMap(){
	if(bHasCollisionMap){
		bHasCollisionMap = false;
		mColMap.MapData.clear();
		return 0;
	}
	return 1;
}

int TileMap::loadFromFileOffset(std::string path, std::string filename, int cTileOffset, int cPaletteOffset){
	int retval = loadFromFileOffset(path, filename, cTileOffset);

	if(retval){
		return 1;
	}

	for(int i = 0; i < TileMapHeight; i++){
		for(int j = 0; j < TileMapWidth; j++){
			setOffset((i * TileMapWidth) + j, cPaletteOffset);
		}
	}

	return 0;
}

int TileMap::loadFromFileOffset(std::string path, std::string filename, int cTileOffset){
	int retval = loadFromFile(path, filename);

	if(retval){
		return 1;
	}

	for(int i = 0; i < TileMapHeight; i++){
		for(int j = 0; j < TileMapWidth; j++){
			setTile((i * TileMapWidth) + j, cTileOffset + getTile((i * TileMapWidth) + j));
		}
	}

	return 0;
}

void TileMap::swapTileValues(int tVal1, int tVal2){
	for(int i = 0; i < TileMapHeight; i++){
		for(int j = 0; j < TileMapWidth; j++){
			int tVal = getTile((i * TileMapWidth) + j);
			if(tVal == tVal1){
				setTile((i * TileMapWidth) + j,  tVal2);
			} else if(tVal == tVal2) {
				setTile((i * TileMapWidth) + j,  tVal1);
			}			
		}
	}

	if(bHasCollisionMap){
		mColMap.swapTileValues(tVal1, tVal2);
	}
}

int TileMap::loadFromFile(std::string path, std::string filename, bool bLoadColMap){
    	DataPath = path; 	
    	DataFile = filename;
		
		bIsSavedToFile = true;

    	std::ifstream infile(path+DIRDEL+filename, std::ios::binary );
    	
    	if(!infile){
		std::cout << "TileMap Not Found!" << std::endl;
		return 1;    	
    	}
    	
    	std::vector<unsigned char> tbuffer(std::istreambuf_iterator<char>(infile), {});

    	int twidth = tbuffer[0];
    	int theight = tbuffer[1];
        
		if((twidth >= 0) && (twidth <= 3)){
			twidth = mTilemapSizesIn[twidth];
		} else {
			std::cout << "TileMap File Error!" << std::endl;
			return 1;
		}


		if((theight >= 0) && (theight <= 3)){
			theight = mTilemapSizesIn[theight];
		} else {
			std::cout << "TileMap File Error!" << std::endl;
			return 1;
		}


	tbuffer.erase(tbuffer.begin());
	tbuffer.erase(tbuffer.begin());

	if((twidth * theight * 2) == tbuffer.size()){
		FileData.resize(twidth * theight * 2);
		FileData = tbuffer;
		//TODO maybe		
		TileMapHeight = theight;
		TileMapWidth = twidth;
		TileAreas.resize(twidth * theight);
		
	} else {
		std::cout << "TileMap File Error!" << std::endl;
		return 1;
	}

	init();

	if(bLoadColMap){
		
		std::string cColMapPath;
		cColMapPath = path + DIRDEL + "col" + filename.substr(0,filename.find(".")) + ".dat";		
		if(fs::exists(fs::status(cColMapPath))){	
			std::cout << "CollisionMap Found" << std::endl;
			if(mColMap.loadFromFile(cColMapPath, this)){
				std::cout << "Error Loading CollisionMap: " << cColMapPath << std::endl;
				return 1;
			} else {
				bHasCollisionMap = true;				
			}
		}
	}

    return 0;
}

int TileMap::saveToFolder(std::string tpath, std::string tfile){
   	std::ofstream outfile(tpath+DIRDEL+tfile, std::ios::binary );

	std::vector<unsigned char> obuffer;

	obuffer.push_back(mTilemapSizesOut[TileMapWidth]);
	obuffer.push_back(mTilemapSizesOut[TileMapHeight]);

	obuffer.insert(obuffer.end(), FileData.begin(), FileData.end());

	outfile.write((char*)obuffer.data(),obuffer.size());
	outfile.close();

	bIsSavedToFile = true;

	
	std::string cColMapPrefix;
	cColMapPrefix = tfile.substr(0,tfile.find("."));

	if(bHasCollisionMap){				
		mColMap.saveToFolder(tpath, cColMapPrefix);		
	} else {
		fs::path cColDat = tpath + DIRDEL + "col" + cColMapPrefix + ".dat";
		if(fs::exists(fs::status(cColDat))){
			fs::remove(cColDat);
		}
		fs::path cColBin = tpath + DIRDEL + "col" + cColMapPrefix + ".bin";
		if(fs::exists(fs::status(cColBin))){
			fs::remove(cColBin);
		}
	}
	return 0;
}

std::string TileMap::getMapSize(){
	std::string cMapSize;
	std::stringstream convert;
	std::string cMapWidth;
	std::string cMapHeight;

	convert << TileMapWidth << std::endl;
	convert >> cMapWidth;

	convert << TileMapHeight << std::endl;
	convert >> cMapHeight;
	
	cMapSize = cMapWidth + "x" + cMapHeight;

	return cMapSize;
}


int TileMap::removeTile(int cDropTile){
	for(int i = 0; i < (mGlobalSettings.TileMapHeight*mGlobalSettings.TileMapWidth); i++){
		int cTile = getTile(i);
		if(cTile >= cDropTile){
			setTile(i, cTile-1);
		}
	}

	if(bHasCollisionMap){
		mColMap.removeTile(cDropTile);
	}

	return 0;
}

int TileMap::getTile(int cTile){
	if((cTile > -1) && (cTile < TileAreas.size())){
		int cTileVal = (FileData[(cTile*2)+1] & 0x3) << 8;
		cTileVal += FileData[(cTile*2)];
		return cTileVal;
	}
	return -1;
}

int TileMap::setTile(int cTile, int cTileVal){
	if((cTile > -1) && (cTile < TileAreas.size())){
		FileData[(cTile*2)+1] = (FileData[(cTile*2)+1] & 0xfc) + ((cTileVal >> 8) & 0x3);
		FileData[(cTile*2)] = cTileVal & 0xff;		
		return 0;
	}	
	return 1;
}

int TileMap::getOffset(int cTile){
	if((cTile > -1) && (cTile < TileAreas.size())){
		int cTileFlip = ((FileData[(cTile*2)+1]  >> 4));
		return cTileFlip;
	}
	return -1;
}

int TileMap::setOffset(int cTile, int cOffset){
	if((cTile > -1) && (cTile < TileAreas.size())){
		FileData[(cTile*2)+1] = (FileData[(cTile*2)+1] & 0x0f) + (cOffset << 4);
		return 0;
	}
	return 1;
}

int TileMap::getFlip(int cTile){
	if((cTile > -1) && (cTile < TileAreas.size())){
		int cTileFlip = ((FileData[(cTile*2)+1]  >> 2) & 0x3);
		return cTileFlip;
	}
	return -1;
}

int TileMap::setFlip(int cTile, int cTileFlip){
	if((cTile > -1) && (cTile < TileAreas.size()) && (cTileFlip > -1) && (cTileFlip < 4) ){
		FileData[(cTile*2)+1] = (FileData[(cTile*2)+1] & 0xf3) + (cTileFlip << 2);
		return 0;
	}
	return 1;
}



TileProperties TileMap::getTileProp(int cTile){
	TileProperties mCurTileProps;
	if((cTile > -1) && (cTile < TileAreas.size())){
		int tFlip = getFlip(cTile);
		mCurTileProps.bFlipX = (tFlip & 0x1);
		mCurTileProps.bFlipY = (tFlip & 0x2);
				
		mCurTileProps.mPaletteOffset = getOffset(cTile);
	} else {
		mCurTileProps.bFlipX = 0;
		mCurTileProps.bFlipY = 0;
		mCurTileProps.mPaletteOffset = mGlobalSettings.mGlobalTexParam.PaletteOffset;
	}
	return mCurTileProps;
}


int TileMap::render(int xpos, int ypos, TileSet* mTiles){	
	for(int i=0; i < mGlobalSettings.TileMapHeight; i++){
		for(int j=0; j < mGlobalSettings.TileMapWidth; j++){
			int tIndex = getTile(j+(i*mGlobalSettings.TileMapWidth));			
			TileAreas[j+(i*mGlobalSettings.TileMapWidth)] = mTiles->TTiles[tIndex]->render(xpos + (mGlobalSettings.mGlobalTexParam.TileSizeX * j * mGlobalSettings.TileMapScale), ypos + (mGlobalSettings.mGlobalTexParam.TileSizeY * i * mGlobalSettings.TileMapScale), mGlobalSettings.TileMapScale, getTileProp(j+(i*mGlobalSettings.TileMapWidth)));			
			if(mGlobalSettings.bShowSelectedTile){
				if((mGlobalSettings.mSelectedTile == (j+(i*mGlobalSettings.TileMapWidth)))){
					SDL_SetRenderDrawColor(mGlobalSettings.TRenderer,mGlobalSettings.DefaultHighlightColor.r,mGlobalSettings.DefaultHighlightColor.g,mGlobalSettings.DefaultHighlightColor.b, 0xff);
					SDL_RenderDrawRect(mGlobalSettings.TRenderer, &TileAreas[j+(i*mGlobalSettings.TileMapWidth)]);
				}
			}
		}
	}
	
	if(mGlobalSettings.CurrentEditor->mCurrentBrushTile && !mGlobalSettings.CurrentEditor->mBrushesTile.bIsEditing){
		if(mGlobalSettings.CurrentEditor->mCurrentBrushTile->mSelected.size()){
			for(int i=0; i < mGlobalSettings.TileMapHeight; i++){
				for(int j=0; j < mGlobalSettings.TileMapWidth; j++){					
					if(mGlobalSettings.CurrentEditor->mCurrentBrushTile->isInSelection((j+(i*mGlobalSettings.TileMapWidth)))){						
						int findex = mGlobalSettings.CurrentEditor->mCurrentBrushTile->findInSelection((j+(i*mGlobalSettings.TileMapWidth)));
						if(mGlobalSettings.CurrentEditor->mCurrentBrushTile->mBrushElements[findex] != -1){
							mTiles->TTiles[mGlobalSettings.CurrentEditor->mCurrentBrushTile->mBrushElements[findex]]->render(xpos + (mGlobalSettings.mGlobalTexParam.TileSizeX * j * mGlobalSettings.TileMapScale), ypos + (mGlobalSettings.mGlobalTexParam.TileSizeY * i * mGlobalSettings.TileMapScale), mGlobalSettings.TileMapScale, mGlobalSettings.CurrentEditor->mCurrentBrushTile->getElementProps(findex));			
						}						
					}
				}
			}
		}
	}


	mBorder.x = TileAreas[0].x;
	mBorder.y = TileAreas[0].y;
	mBorder.w = mGlobalSettings.TileMapWidth * mGlobalSettings.mGlobalTexParam.TileSizeX * mGlobalSettings.TileMapScale;
	mBorder.h = mGlobalSettings.TileMapHeight * mGlobalSettings.mGlobalTexParam.TileSizeY * mGlobalSettings.TileMapScale;

	SDL_SetRenderDrawColor(mGlobalSettings.TRenderer,mGlobalSettings.DefaultGUIBorderColor.r,mGlobalSettings.DefaultGUIBorderColor.g,mGlobalSettings.DefaultGUIBorderColor.b, 0xff);
	SDL_RenderDrawRect(mGlobalSettings.TRenderer, &mBorder);

	if(mGlobalSettings.CurrentEditor->mTileMap->mSelection.bHasSelection){
		mGlobalSettings.CurrentEditor->mTileMap->mSelection.renderSelection(xpos, ypos);
	}

	return 0;
}
