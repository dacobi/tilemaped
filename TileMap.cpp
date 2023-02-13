#include "TSettings.h"
#include "TileMap.h"
#include "cx16_palette.h"
#include "TEditor.h"
#include <SDL.h>

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

    //FileData.resize(mGlobalSettings.TileSizeX * mGlobalSettings.TileSizeY);

    std::vector<unsigned char> tbuffer(std::istreambuf_iterator<char>(infile), {});
    FileData = tbuffer;

    updateTexture(tpal);
    return 0;
}

int TTexture::loadFromBuffer(std::vector<unsigned char> &cTileBuf,TPalette* tpal){
	initTexture();
    //FileData.resize(mGlobalSettings.TileSizeX * mGlobalSettings.TileSizeY);
	FileData = cTileBuf;
	updateTexture(tpal);
	return 0;
}

/*
int TTexture::setAllPixels(unsigned char tcolor, TPalette* tpal){
	FileData.resize(mGlobalSettings.TileSizeX * mGlobalSettings.TileSizeY, tcolor);
	updateTexture(tpal);
	return 0;
}
*/

int TTexture::initTexture(){
	TileTex = SDL_CreateTexture(mGlobalSettings.TRenderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, mGlobalSettings.TileSizeX,mGlobalSettings.TileSizeY);
	SDL_SetTextureBlendMode(TileTex, SDL_BLENDMODE_BLEND);
	return 0;
}

int TTexture::updateTexture(TPalette* tpal){
	if(PixelData.size()){		
		PixelData.erase(PixelData.begin(),PixelData.end());
	}

	for(int i = 0; i < (mGlobalSettings.TileSizeX * mGlobalSettings.TileSizeY); i++){
    		PixelData.push_back(tpal->mapPaletteColor(getPixel(i)));
	}

    SDL_UpdateTexture(TileTex, NULL, PixelData.data(), mGlobalSettings.TileSizeX * sizeof(Uint32));
	return 0;
}

SDL_Rect TTexture::render(int xpos, int ypos, int tscale, bool updateRect ,bool drawGrid){
	SDL_Rect renderQuad = { xpos, ypos, mGlobalSettings.TileSizeX*tscale, mGlobalSettings.TileSizeY*tscale};
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

int TPixel::setPixelColor(unsigned char tcolor, TPalette* tpal){
	PixelIndex = tcolor;
	PixelColor = tpal->TPalette[tcolor];
	return 0;
}

/*
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
*/


SDL_Rect TPixel::renderIm(int xpos, int ypos, int tscale, bool updateRect ,bool drawGrid){
		CurrentArea = { xpos, ypos, mGlobalSettings.TileRenderSize*tscale, mGlobalSettings.TileRenderSize*tscale};

		ImVec2 elmin;
		ImVec2 elmax;

		elmin.x = xpos;
		elmin.y = ypos;
		elmax.x = xpos + mGlobalSettings.TileRenderSize*tscale;
		elmax.y = ypos + mGlobalSettings.TileRenderSize*tscale;

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
		elmax.x = xpos + mGlobalSettings.TileRenderSize*tscale;
		elmax.y = ypos + mGlobalSettings.TileRenderSize*tscale;

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
		CurrentArea = { xpos, ypos, mGlobalSettings.TileRenderSize*tscale, mGlobalSettings.TileRenderSize*tscale};

		ImVec2 elmin;
		ImVec2 elmax;

		elmin.x = xpos;
		elmin.y = ypos;
		elmax.x = xpos + mGlobalSettings.TileRenderSize*tscale;
		elmax.y = ypos + mGlobalSettings.TileRenderSize*tscale;

		ImDrawList *tList = ImGui::GetWindowDrawList();

		tList->AddRectFilled(elmin, elmax, mGlobalSettings.CurrentEditor->mPalette.getImColor(mGlobalSettings.CurrentEditor->mPalette.TPaletteEdit[PixelIndex]));

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

SDL_Rect TPixel::render(int xpos, int ypos, int tscale, bool updateRect ,bool drawGrid){
	CurrentArea = { xpos, ypos, mGlobalSettings.TileRenderSize*tscale, mGlobalSettings.TileRenderSize*tscale};

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


SDL_Rect TPixel::renderEd(int xpos, int ypos, int tscale, bool updateRect ,bool drawGrid){
	CurrentArea = { xpos, ypos, mGlobalSettings.TileSizeX*tscale, mGlobalSettings.TileSizeY*tscale};

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
	SDL_Rect renderQuad = { xpos, ypos, mGlobalSettings.TileSizeX*tscale, mGlobalSettings.TileSizeY*tscale};
        SDL_RenderCopyEx(mGlobalSettings.TRenderer, TileTex, NULL, &renderQuad, 0, NULL, flip);	
        return renderQuad;
}

void TTexture::renderEd(int xpos, int ypos, TPalette* tpal){
	for(int i=0; i < mGlobalSettings.TileSizeY; i++){
		for(int j=0; j < mGlobalSettings.TileSizeX; j++){
			tpal->renderTileEd(xpos + mGlobalSettings.TileSizeX*j, ypos + mGlobalSettings.TileSizeY*i, getPixel(j+(i*mGlobalSettings.TileSizeX))); //FileData[j+(i*mGlobalSettings.TileSizeX)]);			
		}
	}

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
			
			//std::cout << "Color: R: " << (int)tmpcol.r << " G: " << (int)tmpcol.g << " B: " << (int)tmpcol.b << std::endl;
 
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

	int pindex=0;
	for(int i = 2; i < 514; i+=2){
			/*SDL_Color tmpcol;
			
			tmpcol.r = mMapColorIn[tbuffer[i+1]];
			tmpcol.g = mMapColorIn[tbuffer[i] >> 4];
			tmpcol.b = mMapColorIn[tbuffer[i] & 0xf];
			if((i  == 0) ) {				
				tmpcol.a = 0;
			} else {
				tmpcol.a = 255;
			}
			
			TPalette.push_back(tmpcol);
			*/
		//FileData[i]
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

int TPalette::initPalette(){
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
	TPixel *tmptpix;
	for(int i = 0; i < 256; i++){
		tmptpix = new TPixel;
		tmptpix->setPixelColor(i, this);
		TPixels.push_back(tmptpix);		
	}
	return 0;
}

SDL_Rect TPalette::renderTileEd(int xpos,int ypos, int tcolor){
	SDL_SetRenderDrawBlendMode(mGlobalSettings.TRenderer, SDL_BLENDMODE_BLEND);
	int ccolor=0;
	if(tcolor != 0) ccolor = tcolor + (mGlobalSettings.PaletteOffset*16);
	return TPixels[ccolor]->renderEd(xpos, ypos, mGlobalSettings.mTileEdScale,false,mGlobalSettings.bShowPixelGrip);	
}

int TPalette::render(int xpos,int ypos){
	SDL_SetRenderDrawBlendMode(mGlobalSettings.TRenderer, SDL_BLENDMODE_NONE);
	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j++){
			PixelAreas[(i*16)+j] = TPixels[(i*16)+j]->render(
					xpos + ((mGlobalSettings.TileRenderSize*mGlobalSettings.PaletteScale+2)*j),
					ypos + ((mGlobalSettings.TileRenderSize*mGlobalSettings.PaletteScale+2)*i),
					mGlobalSettings.PaletteScale,true,true);
		}
	}

return 0;
}

int TPalette::renderIm(int xpos,int ypos){
	
	Dialog::render(xpos, ypos);

	ImGui::Begin("Palette");

	ImVec2 cSize;
	cSize.x = 20 + ((mGlobalSettings.TileRenderSize*mGlobalSettings.PaletteScale+4) * 16);
	cSize.y = 50 + ((mGlobalSettings.TileRenderSize*mGlobalSettings.PaletteScale+4) * 16);

	ImGui::SetWindowSize(cSize, ImGuiCond_Once);

	ImVec2 cPos = ImGui::GetWindowPos();
	cPos.x += 10;
	cPos.y += 40;

	if(mGlobalSettings.TileSetBPP < 0x8){

		for(int i = 0; i < 16; i++){
			if(i == mGlobalSettings.PaletteOffset){
				for(int j = 0; j < 16; j++){
					PixelAreas[(i*16)+j] = TPixels[(i*16)+j]->renderIm(
					cPos.x + ((mGlobalSettings.TileRenderSize*mGlobalSettings.PaletteScale+4)*j),
					cPos.y + ((mGlobalSettings.TileRenderSize*mGlobalSettings.PaletteScale+4)*i),
					mGlobalSettings.PaletteScale,true,true);
				}
			} else {
				for(int j = 0; j < 16; j++){
					PixelAreas[(i*16)+j] = TPixels[(i*16)+j]->renderImDisabled(
					cPos.x + ((mGlobalSettings.TileRenderSize*mGlobalSettings.PaletteScale+4)*j),
					cPos.y + ((mGlobalSettings.TileRenderSize*mGlobalSettings.PaletteScale+4)*i),
					mGlobalSettings.PaletteScale,true,true);
				}
			}
		}

	} else {

	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j++){
			PixelAreas[(i*16)+j] = TPixels[(i*16)+j]->renderIm(
					cPos.x + ((mGlobalSettings.TileRenderSize*mGlobalSettings.PaletteScale+4)*j),
					cPos.y + ((mGlobalSettings.TileRenderSize*mGlobalSettings.PaletteScale+4)*i),
					mGlobalSettings.PaletteScale,true,true);
		}
	}
	}

	mGlobalSettings.CurrentEditor->ImButtonsPalette.updateButtonStates();

	ImGui::End();

return 0;
}

int TPalette::renderEditor(int xpos,int ypos){
	
	Dialog::render(xpos, ypos);

	ImGui::Begin("Palette: Edit Mode");

	static ImVec4 backup_color;

    ImGuiColorEditFlags misc_flags = ( 0 ); //: ImGuiColorEditFlags_NoOptions);

	ImGui::Text("Change Selected Color");     
	ImGui::ColorEdit3("Selected Color##1", (float*)&mEditColor, misc_flags);
	
	bool bColorPicker = ImGui::ColorButton("Selected Color##3b", mEditColor, misc_flags);
	ImGui::SameLine();
	ImGui::Text("Pick Selected Color");

	if(bColorPicker){
	    ImGui::OpenPopup("mypicker");
		backup_color = mEditColor;		
	}

	ImVec2 sPos = ImGui::GetWindowPos();

	ImGui::SetNextWindowPos(ImVec2(sPos.x+600, sPos.y), ImGuiCond_Always);

	if (ImGui::BeginPopup("mypicker"))
        {
            ImGui::Text("Select Color");
            ImGui::Separator();
            ImGui::ColorPicker3("##picker", (float*)&mEditColor, misc_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
            ImGui::SameLine();            
            ImGui::Text("Current");
            ImGui::ColorButton("##current", mEditColor, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));
            ImGui::Text("Previous");
            if (ImGui::ColorButton("##previous", backup_color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40)))
                	mEditColor = backup_color;
            
            ImGui::EndPopup();
        }


	ImVec2 cSize;
	cSize.x = 20 + ((mGlobalSettings.TileRenderSize*mGlobalSettings.PaletteScale+4) * 16);
	cSize.y = 210 + ((mGlobalSettings.TileRenderSize*mGlobalSettings.PaletteScale+4) * 16);

	ImGui::SetWindowSize(cSize, ImGuiCond_Once);

	ImVec2 cPos = ImGui::GetWindowPos();
	cPos.x += 10;
	cPos.y += 180;

	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j++){
			PixelAreas[(i*16)+j] = TPixels[(i*16)+j]->renderEditor(
					cPos.x + ((mGlobalSettings.TileRenderSize*mGlobalSettings.PaletteScale+4)*j),
					cPos.y + ((mGlobalSettings.TileRenderSize*mGlobalSettings.PaletteScale+4)*i),
					mGlobalSettings.PaletteScale,true,true);
		}
	}

	mGlobalSettings.CurrentEditor->ImButtonsPalette.updateButtonStates();


	TPaletteEdit[mGlobalSettings.CurrentEditor->mColorSelected] = getSDLColor(mEditColor);

	if(ImGui::Button("Apply Changes")){
		mGlobalSettings.CurrentEditor->activetePaletteUpdate();
	}

	ImGui::SameLine();

	if(ImGui::Button("Cancel Changes")){
		TPaletteEdit = TPalette;
		mEditColor = getIm4Color(TPalette[mGlobalSettings.CurrentEditor->mColorSelected]);
	}

	ImGui::End();

return 0;
}


Tile::~Tile(){
	
	if(TileTex){
		SDL_DestroyTexture(TileTex);
		std::cout << "SDL_DestroyTexture(Tile::TileTex)" << std::endl;
		TileTex = NULL;
	}
	
}

int TTexture::setPixel(int pindex, unsigned char pcolor){
	if(mGlobalSettings.TileSetBPP == 0x8){
		FileData[pindex] = pcolor;
	}
	if(mGlobalSettings.TileSetBPP == 0x4){
		int cindex = pindex / 2;
		int crem = pindex % 2;
		unsigned char ccolor;

		int tmppix = FileData[cindex];

		if(crem){
			ccolor = (tmppix & 0xf0) + (pcolor%16);
		} else {
			ccolor = (tmppix & 0x0f) + ((pcolor%16) << 4);
		}

		FileData[cindex] = ccolor;
	}
	return 0;
}

unsigned char TTexture::getPixel(int pindex, int poffset){
	unsigned char tpix = getPixel(pindex);
	if(tpix == 0) return tpix;
	tpix += (poffset*16);
	return tpix;
}

unsigned char TTexture::getPixel(int pindex){
	if(mGlobalSettings.TileSetBPP == 0x8){
		 return FileData[pindex];
	}
	if(mGlobalSettings.TileSetBPP == 0x4){
		int cindex = pindex / 2;
		int crem = pindex % 2;
		unsigned char ccolor;

		int tmppix = FileData[cindex];

		if(crem){
			ccolor = (tmppix & 0x0f);
		} else {
			ccolor = ((tmppix & 0xf0) >> 4);			
		}

		return ccolor;
	}
	return 0;
}

int Tile::updateTexture(TPalette* tpal){
	if(mGlobalSettings.TileSetBPP < 0x8){
		for(int j = 0; j < 16; j++){
			if(PixelData.size()){		
				PixelData.erase(PixelData.begin(),PixelData.end());
			}

			for(int i = 0; i < (mGlobalSettings.TileSizeX * mGlobalSettings.TileSizeY); i++){
    			PixelData.push_back(tpal->mapPaletteColor(getPixel(i, j)));
			}

			std::cout << "Update Ofs: " << j << std::endl;
    		SDL_UpdateTexture(TPOffset[j], NULL, PixelData.data(), mGlobalSettings.TileSizeX * sizeof(Uint32));		
		}
	} else {
		TTexture::updateTexture(tpal);
	}

/*
	if(PixelData.size()){		
		PixelData.erase(PixelData.begin(),PixelData.end());
	}

	for(int i = 0; i < (mGlobalSettings.TileSizeX * mGlobalSettings.TileSizeY); i++){
    		PixelData.push_back(tpal->mapPaletteColor(getPixel(i)));
	}

    SDL_UpdateTexture(TileTex, NULL, PixelData.data(), mGlobalSettings.TileSizeX * sizeof(Uint32));
	*/
	return 0;
}


int Tile::createNew(TPalette* tpal){
	std::cout << "initTile" << std::endl;
	initTile();
	std::cout << "initTexture" << std::endl;
	initTexture();

    FileData.resize(((mGlobalSettings.TileSizeX * mGlobalSettings.TileSizeY)/mGlobalSettings.mTileBPPSize[mGlobalSettings.TileSetBPP]), 0);
	std::cout << "setAllPixels" << std::endl;
	//setAllPixels(0, tpal);
	updateTexture(tpal);
	
	return 0;
}

int Tile::initTexture(){
	if(mGlobalSettings.TileSetBPP < 0x8){
		for(int i = 0; i < 16; i++){
			std::cout << "Create Ofs: " << i << std::endl;
			TPOffset[i] = SDL_CreateTexture(mGlobalSettings.TRenderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, mGlobalSettings.TileSizeX,mGlobalSettings.TileSizeY);
			SDL_SetTextureBlendMode(TPOffset[i], SDL_BLENDMODE_BLEND);
		}

	} else {
		TTexture::initTexture();
	}

	//TileTex = SDL_CreateTexture(mGlobalSettings.TRenderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, mGlobalSettings.TileSizeX,mGlobalSettings.TileSizeY);
	//SDL_SetTextureBlendMode(TileTex, SDL_BLENDMODE_BLEND);
	return 0;
}



int Tile::initTile(){
	SDL_Rect eRect = {0,0,1,1};
	PixelAreas.resize(mGlobalSettings.TileSizeX*mGlobalSettings.TileSizeY,eRect);
	if(mGlobalSettings.TileSetBPP < 0x8){
		TPOffset.resize(16, 0);
	}
	return 0;
}

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

SDL_Rect Tile::renderIm(int xpos, int ypos, int tscale, bool updateRect ,bool drawGrid){
	SDL_Rect tmpRect;// = TTexture::render(xpos, ypos, tscale, updateRect, drawGrid);

	if(mGlobalSettings.TileSetBPP < 0x8){
		ImGui::Image((ImTextureID)(intptr_t)TPOffset[mGlobalSettings.PaletteOffset], ImVec2(mGlobalSettings.TileSizeX * tscale, mGlobalSettings.TileSizeY * tscale));	
	} else {
		ImGui::Image((ImTextureID)(intptr_t)TileTex, ImVec2(mGlobalSettings.TileSizeX * tscale, mGlobalSettings.TileSizeY * tscale));
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

	/*
	if(bIsSelected){
		SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, mGlobalSettings.AltHighlightColor.r, mGlobalSettings.AltHighlightColor.g, mGlobalSettings.AltHighlightColor.b, 0xff); 
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &tmpRect);
		SDL_Rect sndRect = tmpRect;
		sndRect.x = sndRect.x-1;
		sndRect.y = sndRect.y-1;
		sndRect.w = sndRect.w+2;
		sndRect.h = sndRect.h+2;
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &sndRect);
	} */
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

	if(mGlobalSettings.TileSetBPP < 0x8){
		std::cout << "Render Ofs: " << tProps.mPaletteOffset << std::endl;
		tmpRect = { xpos, ypos, mGlobalSettings.TileSizeX*tscale, mGlobalSettings.TileSizeY*tscale};
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

void Tile::renderEd(int xpos, int ypos, TPalette* tpal){
	for(int i=0; i < mGlobalSettings.TileSizeY; i++){
		for(int j=0; j < mGlobalSettings.TileSizeX; j++){
			PixelAreas[j+(mGlobalSettings.TileSizeX*i)] = tpal->renderTileEd(xpos + (mGlobalSettings.TileSizeX * mGlobalSettings.mTileEdScale)*j, ypos + (mGlobalSettings.TileSizeY * mGlobalSettings.mTileEdScale)*i, getPixel(j+(i*mGlobalSettings.TileSizeX))); //FileData[j+(i*mGlobalSettings.TileSizeX)]);
		}
	}
}

int Tile::loadFromFile(std::string filename,TPalette* tpal){ 
	initTile();	
	return TTexture::loadFromFile(filename,tpal);
}

int Tile::loadFromBuffer(std::vector<unsigned char> &cTileBuf,TPalette* tpal){ 
	initTile();	
	TTexture::loadFromBuffer(cTileBuf,tpal);
	initTexture();
	updateTexture(tpal);
	return 0;
}

void TileSet::shutdown(){
	for(auto *dTile: TTiles){
		delete dTile;
	}
}

Tile* TileSet::createNewFromFile(std::string newPath, TPalette* tpal){
	if(fs::exists(fs::status(newPath))){
		if(fs::is_directory(fs::status(newPath))){
			return NULL;
		}

		SDL_Surface *newSurf = IMG_Load(newPath.c_str());
		if(newSurf){
			if(newSurf->format->BitsPerPixel == 8){
				if((newSurf->w == mGlobalSettings.TileSizeX) && (newSurf->h == mGlobalSettings.TileSizeY)){
					std::vector<unsigned char> tbuffer;
					for(int i = 0; i < (mGlobalSettings.TileSizeX*mGlobalSettings.TileSizeY); i++){
						tbuffer.push_back(((unsigned char*)(newSurf->pixels))[i]);
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

			if(tbuffer.size() == ((mGlobalSettings.TileSizeX * mGlobalSettings.TileSizeY)/mGlobalSettings.mTileBPPSize[mGlobalSettings.TileSetBPP])){
				return createNewFromBuffer(tbuffer, tpal);
			} else {
				return NULL;				
			}
		}
	}
	return NULL;
}

Tile* TileSet::createNewFromBuffer(std::vector<unsigned char> &newBuf, TPalette* tpal){

	Tile *newTile = new Tile();
	SDL_Rect newRect;

	newTile->loadFromBuffer(newBuf, tpal);
	TileAreas.push_back(newRect);
	TTiles.push_back(newTile);
	return newTile;

}

Tile* TileSet::createNewCopy(Tile* cCopyTile, TPalette* tpal){	
	return createNewFromBuffer(cCopyTile->FileData, tpal);
}

Tile* TileSet::createNew(TPalette* tpal){	
	std::vector<unsigned char> tbuf;
	//tbuf.resize(mGlobalSettings.TileSizeX * mGlobalSettings.TileSizeY,0);
	//((mGlobalSettings.TileSizeX * mGlobalSettings.TileSizeY)/mTileBPPSize[mGlobalSettings.TileSetBPP])
	tbuf.resize(((mGlobalSettings.TileSizeX * mGlobalSettings.TileSizeY)/mGlobalSettings.mTileBPPSize[mGlobalSettings.TileSetBPP]),0);
	return createNewFromBuffer(tbuf, tpal);
}

int TileSet::removeTile(int cDropTile){
	Tile* dTile = *(TTiles.begin() +  cDropTile); 
	TTiles.erase(TTiles.begin() +  cDropTile);
	TileAreas.erase(TileAreas.begin() + cDropTile);
	delete dTile;
	return 0;
}

void TileSet::dropLastTile(){
	(*(TTiles.end()-1))->bIsSelected = false;
	TTiles.pop_back();
	TileAreas.pop_back();
	reCalculateScale();
}

void TileSet::appendTile(Tile* addTile){
	TTiles.push_back(addTile);
	SDL_Rect newRect;
	TileAreas.push_back(newRect);
	reCalculateScale();
}

int TileSet::loadFromFolder(std::string path, TPalette* tpal){ 

	fs::path cTileFile;
	int ntiles=0;

	if(fs::is_directory(fs::status(path))){
		cTileFile = path + DIRDEL + "tiles.bin";
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

	mGlobalSettings.TileSizeX = mTileSizeIn[twidth];
	mGlobalSettings.TileSizeY = mTileSizeIn[theight];

	if( (magic2 == 0x8) ||(magic2 == 0x4) ||(magic2 == 0x2) ){
		mGlobalSettings.TileSetBPP = magic2;
	} else {
		std::cout << "Error in Tile File: " << cTileFile << std::endl;
		return 1;	
	}

/*
	if((twidth == 8) || (twidth == 16)){
		mGlobalSettings.TileSizeX = twidth;
		mGlobalSettings.TileSizeY = theight;
	} else {
		std::cout << "Error in Tile File: " << cTileFile << std::endl;
		return 1;		
	}
*/
	int tmpTileSize = ((mGlobalSettings.TileSizeX * mGlobalSettings.TileSizeY)/mGlobalSettings.mTileBPPSize[mGlobalSettings.TileSetBPP]);

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

	return 0;
}

int TileSet::saveToFolder(std::string tpath){
	std::ofstream outfile(tpath+DIRDEL+"tiles.bin", std::ios::binary );

	std::vector<unsigned char> obuffer;

	int magic1, magic2;

	magic1 = mTileSizeOut[mGlobalSettings.TileSizeX];
	magic1 = (magic1 << 4) + mTileSizeOut[mGlobalSettings.TileSizeY];

	magic2 = mGlobalSettings.TileSetBPP;

	obuffer.push_back(magic1);
	obuffer.push_back(magic2);

	//obuffer.push_back(mGlobalSettings.TileSizeX);
	//obuffer.push_back(mGlobalSettings.TileSizeY);

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
		if( (int)( (float)( ( ( ((mCurTileScale+1)*mGlobalSettings.TileSizeX )+ mColSpace ) * TTiles.size() )  / ( mCurColumns - 1 ) ) ) < mTileSetBackGround.h ){
			mCurTileScale++;
			mCurColumns--;
			return 1;		
		}
	}
	return 0;
}

int TileSet::render(int ypos, int mScroll){
	
	mTileSetBackGround.h = mGlobalSettings.WindowHeight- mGlobalSettings.TopBarHeight;
	
	if(mCurColumns < 4){
		if( (int)( (float)( ( ( (mCurTileScale*mGlobalSettings.TileSizeX ) +mColSpace ) * TTiles.size() )  / mCurColumns ) ) > mTileSetBackGround.h ){	
			mCurTileScale--;
		
			if(mCurTileScale < 5){
				mCurColumns++;		
			}			
		}
	}

	mGlobalSettings.TileSetWidth = (((mCurTileScale*mGlobalSettings.TileSizeX)+mColSpace)*mCurColumns)+(mColSpace*3);

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
				TileAreas[(i * cRowNum) + j] = TTiles[(i*cRowNum)+j]->render((mTileSetBackGround.x+ (mColSpace*2) +  ((mCurTileScale*mGlobalSettings.TileSizeX)+mColSpace)*i),mTileSetBackGround.y + mScroll + (mColSpace*2) + (((mGlobalSettings.TileSizeY*mCurTileScale)+mColSpace)*j), mCurTileScale,true,true);
			}								
		}	
		if(isOdd){			
			int i = mCurColumns;
			for(int j = 0; j < isOdd; j++){
				TileAreas[(i * cRowNum) + j] = TTiles[(i*cRowNum)+j]->render((mTileSetBackGround.x+ (mColSpace*2) +  ((mCurTileScale*mGlobalSettings.TileSizeX)+mColSpace)*j),mTileSetBackGround.y + mScroll + (mColSpace*2) + (((mGlobalSettings.TileSizeY*mCurTileScale)+mColSpace)*cRowNum), mCurTileScale,true,true);
			}
		}
	}


	
	int cMax = (int)( (float)( ( ( (mCurTileScale*mGlobalSettings.TileSizeY ) +mColSpace ) * TTiles.size() )  / mCurColumns )) + (4 * mGlobalSettings.TileSizeY);
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

int TileSet::renderIm(int ypos, int mScroll){
	
	mTileSetBackGround.h = mGlobalSettings.WindowHeight- mGlobalSettings.TopBarHeight;
	
	if(mCurColumns < 4){
		if( (int)( (float)( ( ( (mCurTileScale*mGlobalSettings.TileSizeX ) +mColSpace ) * TTiles.size() )  / mCurColumns ) ) > mTileSetBackGround.h ){	
			mCurTileScale--;
		
			if(mCurTileScale < 5){
				mCurColumns++;
				updateWinPos = true;
			}			
		}
	}

	mGlobalSettings.TileSetWidth = (((mCurTileScale*mGlobalSettings.TileSizeX)+mColSpace)*mCurColumns)+(mColSpace*3);

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

	ImGui::Begin("TileSet");                         

	if(mCurColumns > 0){
		for(int i = 0; i < cRowNum; i++){
			for(int j = 0; j < mCurColumns; j++){
				TileAreas[(i * mCurColumns) + j] = TTiles[(i*mCurColumns) + j]->renderIm((mTileSetBackGround.x+ (mColSpace*2) +  ((mCurTileScale*mGlobalSettings.TileSizeX)+mColSpace)*j),mTileSetBackGround.y + mScroll + (mColSpace*2) + (((mGlobalSettings.TileSizeY*mCurTileScale)+mColSpace)*i), mCurTileScale,true,true);				
				if((mCurColumns > 1) && (j < (mCurColumns-1))){
					ImGui::SameLine();
				} 
			}										
		}	
		
		if(isOdd){			
			int i = mCurColumns;
			for(int j = 0; j < isOdd; j++){
				TileAreas[(i * cRowNum) + j] = TTiles[(i*cRowNum)+j]->renderIm((mTileSetBackGround.x+ (mColSpace*2) +  ((mCurTileScale*mGlobalSettings.TileSizeX)+mColSpace)*j),mTileSetBackGround.y + mScroll + (mColSpace*2) + (((mGlobalSettings.TileSizeY*mCurTileScale)+mColSpace)*cRowNum), mCurTileScale,true,true);
				if((j < (isOdd-1))){
					ImGui::SameLine();
				}
			}

		}
		
	}

	mGlobalSettings.CurrentEditor->ImButtonsTileSet.updateButtonStates();
	
    ImGui::End();


	int cMax = (int)( (float)( ( ( (mCurTileScale*mGlobalSettings.TileSizeY ) +mColSpace ) * TTiles.size() )  / mCurColumns )) + (4 * mGlobalSettings.TileSizeY);
	if((cMax - mTileSetBackGround.h) > 0 ){
		mMaxScrollY = -(cMax - mTileSetBackGround.h);
	} else {
		mMaxScrollY = 0;
	}

return 0;
}


int TileMap::createNew(){

	FileData.resize(mGlobalSettings.TileMapWidth * mGlobalSettings.TileMapHeight * 2, 0);
	TileAreas.resize(mGlobalSettings.TileMapWidth * mGlobalSettings.TileMapHeight);

	return 0;
}

int TileMap::loadFromFile(std::string path, std::string filename){
    	DataPath = path; 	
    	DataFile = filename;

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
		mGlobalSettings.TileMapHeight = theight;
		mGlobalSettings.TileMapWidth = twidth;
		TileAreas.resize(twidth * theight);
		
	} else {
		std::cout << "TileMap File Error!" << std::endl;
		return 1;
	}

    return 0;
}

int TileMap::saveToFolder(std::string tpath){
   	std::ofstream outfile(tpath+DIRDEL+"map.bin", std::ios::binary );

	std::vector<unsigned char> obuffer;

	obuffer.push_back(mTilemapSizesOut[mGlobalSettings.TileMapWidth]);
	obuffer.push_back(mTilemapSizesOut[mGlobalSettings.TileMapHeight]);

	obuffer.insert(obuffer.end(), FileData.begin(), FileData.end());

	outfile.write((char*)obuffer.data(),obuffer.size());
	outfile.close();

	return 0;
}

int TileMap::removeTile(int cDropTile){
	for(int i = 0; i < (mGlobalSettings.TileMapHeight*mGlobalSettings.TileMapWidth); i++){
		int cTile = getTile(i);
		if(cTile >= cDropTile){
			setTile(i, cTile-1);
		}
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
	}
	return mCurTileProps;
}

int TileMap::render(int xpos, int ypos, TileSet* mTiles){
	for(int i=0; i < mGlobalSettings.TileMapHeight; i++){
		for(int j=0; j < mGlobalSettings.TileMapWidth; j++){
			TileAreas[j+(i*mGlobalSettings.TileMapWidth)] = mTiles->TTiles[getTile(j+(i*mGlobalSettings.TileMapWidth))]->render(xpos + (mGlobalSettings.TileSizeX * j * mGlobalSettings.TileMapScale), ypos + (mGlobalSettings.TileSizeY * i * mGlobalSettings.TileMapScale), mGlobalSettings.TileMapScale, getTileProp(j+(i*mGlobalSettings.TileMapWidth)));
			if(mGlobalSettings.bShowSelectedTile){
				if(mGlobalSettings.mSelectedTile == (j+(i*mGlobalSettings.TileMapWidth))){
					SDL_SetRenderDrawColor(mGlobalSettings.TRenderer,mGlobalSettings.DefaultHighlightColor.r,mGlobalSettings.DefaultHighlightColor.g,mGlobalSettings.DefaultHighlightColor.b, 0xff);
					SDL_RenderDrawRect(mGlobalSettings.TRenderer, &TileAreas[j+(i*mGlobalSettings.TileMapWidth)]);
				}
			}
		}
	}

	SDL_Rect mBorder;

	mBorder.x = TileAreas[0].x;
	mBorder.y = TileAreas[0].y;
	mBorder.w = mGlobalSettings.TileMapWidth * mGlobalSettings.TileSizeX * mGlobalSettings.TileMapScale;
	mBorder.h = mGlobalSettings.TileMapHeight * mGlobalSettings.TileSizeY * mGlobalSettings.TileMapScale;

	SDL_SetRenderDrawColor(mGlobalSettings.TRenderer,mGlobalSettings.DefaultGUIBorderColor.r,mGlobalSettings.DefaultGUIBorderColor.g,mGlobalSettings.DefaultGUIBorderColor.b, 0xff);
	SDL_RenderDrawRect(mGlobalSettings.TRenderer, &mBorder);

	return 0;
}
