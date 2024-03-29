#include "TSettings.h"
#include "TileMap.h"
#include "cx16_palette.h"
#include "TEditor.h"
#include <SDL.h>
#include "ImGuiFileDialog.h"
#include <cmath>


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
	TileTex = SDL_CreateTexture(mGlobalSettings.TRenderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, mTexParam->TexSizeX,mTexParam->TexSizeY);
	SDL_SetTextureBlendMode(TileTex, SDL_BLENDMODE_BLEND);
	return 0;
}

int TTexture::updateTexture(TPalette* tpal){
	if(PixelData.size()){		
		PixelData.erase(PixelData.begin(),PixelData.end());
	}

	for(int i = 0; i < (mTexParam->TexSizeX * mTexParam->TexSizeY); i++){
    		PixelData.push_back(tpal->mapPaletteColor(getPixel(i)));
	}

    SDL_UpdateTexture(TileTex, NULL, PixelData.data(), mTexParam->TexSizeX * sizeof(Uint32));
	return 0;
}

int TPixel::setPixelColor(unsigned char tcolor, TPalette* tpal){
	PixelIndex = tcolor;
	PixelColor = tpal->TPalette[tcolor];
	if(tcolor == 0){
		PixelColor.a = 0x00;
	}
	return 0;
}

SDL_Rect TPixel::renderIm(int xpos, int ypos, int tscale, bool updateRect ,bool drawGrid){
		CurrentArea = { xpos, ypos, mGlobalSettings.mGlobalTexParam.TexRenderSize*tscale, mGlobalSettings.mGlobalTexParam.TexRenderSize*tscale};

		ImVec2 elmin;
		ImVec2 elmax;

		elmin.x = xpos;
		elmin.y = ypos;
		elmax.x = xpos + mGlobalSettings.mGlobalTexParam.TexRenderSize*tscale;
		elmax.y = ypos + mGlobalSettings.mGlobalTexParam.TexRenderSize*tscale;

		ImDrawList *tList = ImGui::GetWindowDrawList();

		tList->AddRectFilled(elmin, elmax, mGlobalSettings.mEditor->mPalette.getImColor(PixelColor));

		if(bPixelSelected){
			ImVec2 exmin,exmax;
			exmin.x = elmin.x -1;
			exmin.y = elmin.y -1;
			exmax.x = elmax.x +1;
			exmax.y = elmax.y +1;			
			tList->AddRect(elmin, elmax, mGlobalSettings.ImAltHighLightColor);
			tList->AddRect(exmin, exmax, mGlobalSettings.ImAltHighLightColor);
			exmin.x--;
			exmin.y--;
			exmax.x++;
			exmax.y++;
			tList->AddRect(exmin, exmax, mGlobalSettings.ImAltHighLightColor);
		} else {
			if(drawGrid){
				tList->AddRect(elmin, elmax, mGlobalSettings.ImPixelGridColor); //mGlobalSettings.CurrentEditor->mPalette.getImColor(mGlobalSettings.PixelGridColor));
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
		elmax.x = xpos + mGlobalSettings.mGlobalTexParam.TexRenderSize*tscale;
		elmax.y = ypos + mGlobalSettings.mGlobalTexParam.TexRenderSize*tscale;

		ImDrawList *tList = ImGui::GetWindowDrawList();

		int dCol = PixelColor.r + PixelColor.g + PixelColor.b;
		dCol /= 0x6;
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
		CurrentArea = { xpos, ypos, mGlobalSettings.mGlobalTexParam.TexRenderSize*tscale, mGlobalSettings.mGlobalTexParam.TexRenderSize*tscale};

		ImVec2 elmin;
		ImVec2 elmax;

		elmin.x = xpos;
		elmin.y = ypos;
		elmax.x = xpos + mGlobalSettings.mGlobalTexParam.TexRenderSize*tscale;
		elmax.y = ypos + mGlobalSettings.mGlobalTexParam.TexRenderSize*tscale;

		ImDrawList *tList = ImGui::GetWindowDrawList();

		tList->AddRectFilled(elmin, elmax, mGlobalSettings.mEditor->mPalette.getImColor(mGlobalSettings.mEditor->mPalette.TPaletteEdit[PixelIndex]));

		if(bPixelSelectedEdit){			
			ImVec2 exmin,exmax;
			exmin.x = elmin.x -1;
			exmin.y = elmin.y -1;
			exmax.x = elmax.x +1;
			exmax.y = elmax.y +1;			
			tList->AddRect(elmin, elmax, mGlobalSettings.ImAltHighLightColor);
			tList->AddRect(exmin, exmax, mGlobalSettings.ImAltHighLightColor);
			exmin.x--;
			exmin.y--;
			exmax.x++;
			exmax.y++;
			tList->AddRect(exmin, exmax, mGlobalSettings.ImAltHighLightColor);
		} else {
			if(drawGrid){
				tList->AddRect(elmin, elmax, mGlobalSettings.ImPixelGridColor); //mGlobalSettings.CurrentEditor->mPalette.getImColor(mGlobalSettings.PixelGridColor));
			}	
		}

		return CurrentArea;
}

SDL_Rect TPixel::renderEdSel(int xpos, int ypos, int tscale, bool drawGrid){
	CurrentArea = { xpos, ypos, tscale, tscale};

	SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, PixelColor.r,PixelColor.g,PixelColor.b,PixelColor.a);
	
	SDL_RenderFillRect(mGlobalSettings.TRenderer, &CurrentArea);
    
	if(drawGrid && !bPixelSelected){
		SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, mGlobalSettings.PixelGridColor.r ,mGlobalSettings.PixelGridColor.g ,mGlobalSettings.PixelGridColor.b ,0xff);
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &CurrentArea);
	}

	if(bPixelSelected){
		SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, mGlobalSettings.AltHighlightColor.r, mGlobalSettings.AltHighlightColor.g, mGlobalSettings.AltHighlightColor.b, 0xff); 
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &CurrentArea);
		/*SDL_Rect sndRect = CurrentArea;
		sndRect.x = sndRect.x+1;
		sndRect.y = sndRect.y+1;
		sndRect.w = sndRect.w-2;
		sndRect.h = sndRect.h-2;

		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &sndRect);*/
    }

    return CurrentArea;
}


SDL_Rect TPixel::renderEd(int xpos, int ypos, TextureParameters *mTexParam, int tscale, bool updateRect ,bool drawGrid){
	CurrentArea = { xpos, ypos, mTexParam->TexPixelSize*tscale, mTexParam->TexPixelSize*tscale};
	
	SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, PixelColor.r,PixelColor.g,PixelColor.b,PixelColor.a);
	
	SDL_RenderFillRect(mGlobalSettings.TRenderer, &CurrentArea);
    
	if(drawGrid && !bPixelSelected){
		SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, mGlobalSettings.PixelGridColor.r ,mGlobalSettings.PixelGridColor.g ,mGlobalSettings.PixelGridColor.b ,0xff);
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &CurrentArea);
	}

    if(bPixelSelected){						
		SDL_SetRenderDrawColor(mGlobalSettings.TRenderer, mGlobalSettings.AltHighlightColor.r, mGlobalSettings.AltHighlightColor.g, mGlobalSettings.AltHighlightColor.b, 0xff); 		
		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &CurrentArea);				
		SDL_Rect sndRect = CurrentArea;
		sndRect.x = sndRect.x+1;
		sndRect.y = sndRect.y+1;
		sndRect.w = sndRect.w-2;
		sndRect.h = sndRect.h-2;

		SDL_RenderDrawRect(mGlobalSettings.TRenderer, &sndRect);
		
    }
    return CurrentArea;
}


SDL_Rect TTexture::renderEx(int xpos, int ypos, int tscale, SDL_RendererFlip flip){
	SDL_Rect renderQuad = { xpos, ypos, mTexParam->TexSizeX*tscale, mTexParam->TexSizeY*tscale};
        SDL_RenderCopyEx(mGlobalSettings.TRenderer, TileTex, NULL, &renderQuad, 0, NULL, flip);	
        return renderQuad;
}

void TTexture::renderEd(int xpos, int ypos, TPalette* tpal){
	for(int i=0; i < mTexParam->TexSizeY; i++){
		for(int j=0; j < mTexParam->TexSizeX; j++){
			tpal->renderTileEd(xpos + mTexParam->TexSizeX*j, ypos + mTexParam->TexSizeY*i, getPixel(j+(i*mTexParam->TexSizeX)), mTexParam); //FileData[j+(i*mGlobalSettings.TileSizeX)]);			
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

ImU32 TPalette::getImColorInv(SDL_Color cColor){
	ImU32 tColor;

	unsigned char nR, nG, nB;
	
	nR = cColor.r;
	nG = cColor.g;	
	nB = cColor.b;
	
	nR = ~nR;
	nG = ~nG;
	nB = ~nB;

	tColor = cColor.a;
	tColor = tColor << 8;
	tColor += nB;
	tColor = tColor << 8;
	tColor += nG;
	tColor = tColor << 8;
	tColor += nR;

	return tColor;
}

ImVec4 TPalette::getIm4ColorIm(ImU32 cColor){
	ImVec4 tColor;

	tColor.w = 1.0f;
	tColor.x = (cColor & 0x000000FF) / 255.0f;
	tColor.y = ((cColor & 0x0000FF00) >> 8)/ 255.0f;
	tColor.z = ((cColor & 0x00FF0000) >> 16) / 255.0f;

	return tColor;
}

SDL_Color TPalette::getSDLColorIm(ImU32 cCol){

	SDL_Color tColor;

	tColor.a = 0xff;
	tColor.r = (cCol & 0x000000FF);
	tColor.g = (cCol & 0x0000FF00) >> 8;
	tColor.b = (cCol & 0x00FF0000) >> 16;

	return tColor;

}

ImU32 TPalette::getImColorVec4(ImVec4 cColor){
	ImU32 tColor;
	tColor = (int)(cColor.w * 255.0);
	tColor = tColor << 8;
	tColor += (int)(cColor.z * 255.0);
	tColor = tColor << 8;
	tColor += (int)(cColor.y * 255.0);
	tColor = tColor << 8;
	tColor += (int)(cColor.x * 255.0);

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

int TPalette::getClosest4Bit(int cCol){
	return (int)std::round( (float(cCol)/17.0) );
}

int TPalette::importPaletteEdit(std::string palPath, int exStart, int exRange, int inStart){

	int pType = mGlobalSettings.testPaletteFile(palPath);

	if(pType == 0){		
		return 1;
	}

	if(pType == 2){		
		std::ifstream input( palPath, std::ios::in );
		unsigned char tpalette[256][3];

		std::string nline;
    	std::string ntmp1;    
		std::string ntmp2;
		    
    	std::getline(input, nline);
    	std::getline(input, nline);
    	std::getline(input, nline);
    	std::getline(input, nline); 

		int pStartPos = exStart;

		while(pStartPos > 0){
			pStartPos--;
			std::getline(input, nline); 
		}    
            	
    	int mr, mg, mb;

		int cColNum = 0;
    
    	for(int i = 0; i < exRange; i++){
 			std::getline(input, nline);

			if(input.eof()){
				break;
			}

			cColNum = i;

			ntmp2 = nline;

			std::stringstream conwc(ntmp2); 

			int wordc = 0;

			while(conwc >> ntmp1){
				wordc++;			
			}       	
    		
			if(wordc < 3){
				return 1;
			}

			std::stringstream convert(nline); 

			convert >> mr >> mg >> mb;	
    		
			tpalette[i][0] = mMapColorIn[getClosest4Bit(mr)];
    		tpalette[i][1] = mMapColorIn[getClosest4Bit(mg)];
    		tpalette[i][2] = mMapColorIn[getClosest4Bit(mb)];  			    	
    	}

		if(cColNum < (exRange-1)){
			exRange = cColNum + 1;
		}

		for(int i = inStart; i < (inStart + exRange); i++){
						
			TPaletteEdit[i].r = tpalette[i - inStart][0];
			TPaletteEdit[i].g = tpalette[i - inStart][1];
			TPaletteEdit[i].b = tpalette[i - inStart][2];

			if((i  == 0) ) {								
				TPaletteEdit[i].r = 0;
				TPaletteEdit[i].g = 0;
				TPaletteEdit[i].b = 0;
				TPaletteEdit[i].a = 0;
			} else {
				TPaletteEdit[i].a = 255;
			}

			if(i == mGlobalSettings.mEditor->mColorSelectedEdit){
				setSelectedColor(TPaletteEdit[i]);				
			}					
		}
		
		return 0;
	}

	if(pType == 1){
	
		std::ifstream infile(palPath, std::ios::binary );
    	std::vector<unsigned char> tbuffer(std::istreambuf_iterator<char>(infile), {});

		int magic1,magic2;

		magic1 = tbuffer[0];
		magic2 = tbuffer[1];

		tbuffer.erase(tbuffer.begin());
		tbuffer.erase(tbuffer.begin());

		std::vector<SDL_Color> tmpPal;

		if((magic1 == 16) && (magic2 == 42) && (tbuffer.size() == 512)){
	
			for(int i = 0; i < 512; i+=2){
				SDL_Color tmpcol;
			
				tmpcol.r = mMapColorIn[tbuffer[i+1]];
				tmpcol.g = mMapColorIn[tbuffer[i] >> 4];
				tmpcol.b = mMapColorIn[tbuffer[i] & 0xf];
				if((i  == 0) ) {
					tmpcol.r = 0;
					tmpcol.g = 0;
					tmpcol.b = 0;			
					tmpcol.a = 0;
				} else {
					tmpcol.a = 255;
				}
			
				tmpPal.push_back(tmpcol);
			}

			if((inStart + exRange) > 256){
				exRange = 256 - inStart;
			}

			for(int i = inStart; i < (inStart + exRange); i++){
				TPaletteEdit[i] = tmpPal[exStart + (i - inStart)];
				if(i == mGlobalSettings.mEditor->mColorSelectedEdit){
					setSelectedColor(TPaletteEdit[i]);				
				}			
			}

			return 0;	
		}
	
		return 1;
	}

	return 1;
}

void TPalette::setSelectedColor(SDL_Color cColor){
	mR = mMapColorOut[cColor.r];
	mG = mMapColorOut[cColor.g];
	mB = mMapColorOut[cColor.b];
	mEditColor = getIm4Color(cColor);
}

int TPalette::importPaletteEdit(std::string palPath){

	int pType = mGlobalSettings.testPaletteFile(palPath);

	if(pType == 0){
		mGlobalSettings.mEditor->showMessage("Error: Not Palette File!", true);
		return 1;
	}

	if(pType == 2){		
		std::ifstream input( palPath, std::ios::in );
		unsigned char tpalette[256][3];

		std::string nline;
    	std::string ntmp1;    
		std::string ntmp2;
	
    	std::getline(input, nline);
    	std::getline(input, nline);
    	std::getline(input, nline);
    	std::getline(input, nline);            
            	
    	int mr, mg, mb;

		int cColNum = 0;
    
    	for(int i = 0; i < 256; i++){
 			std::getline(input, nline);       	

			if(input.eof()){
				break;
			}

			cColNum = i;
    		
			ntmp2 = nline;

			std::stringstream conwc(ntmp2); 

			int wordc = 0;
			
			while(conwc >> ntmp1){
				wordc++;			
			}
    				
			if(wordc < 3){				
				return 1;
			}

			std::stringstream convert(nline); 

			convert >> mr >> mg >> mb;	

			tpalette[i][0] = mMapColorIn[getClosest4Bit(mr)];
    		tpalette[i][1] = mMapColorIn[getClosest4Bit(mg)];
    		tpalette[i][2] = mMapColorIn[getClosest4Bit(mb)];  			
    	}

		if(cColNum < 255){
			while(cColNum < 256){
				tpalette[cColNum][0] = 0;
    			tpalette[cColNum][1] = 0;
    			tpalette[cColNum][2] = 0;    	
				cColNum++;
			}
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
				tmpcol.r = 0;
				tmpcol.g = 0;
				tmpcol.b = 0;
				tmpcol.a = 0;
			} else {
				tmpcol.a = 255;
			}
			
			TPaletteEdit.push_back(tmpcol);
			
			if(i == mGlobalSettings.mEditor->mColorSelectedEdit){
				setSelectedColor(TPaletteEdit[i]);				
			}
		}
		mGlobalSettings.mEditor->showMessage("Palette Import Succesful");
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

			int pixindex = 0;
	
			for(int i = 0; i < 512; i+=2){
				SDL_Color tmpcol;
			
				tmpcol.r = mMapColorIn[tbuffer[i+1]];
				tmpcol.g = mMapColorIn[tbuffer[i] >> 4];
				tmpcol.b = mMapColorIn[tbuffer[i] & 0xf];
				if((i  == 0) ) {
					tmpcol.r = 0;
					tmpcol.g = 0;
					tmpcol.b = 0;			
					tmpcol.a = 0;
				} else {
					tmpcol.a = 255;
				}
			
				TPaletteEdit.push_back(tmpcol);

				if(pixindex == mGlobalSettings.mEditor->mColorSelectedEdit){				
					setSelectedColor(TPaletteEdit[pixindex]);			
				}

				pixindex++;
			}
		
			mGlobalSettings.mEditor->showMessage("Palette Import Succesful");		
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
    	std::string ntmp1;    
		std::string ntmp2;
		    
    	std::getline(input, nline);
    	std::getline(input, nline);
    	std::getline(input, nline);
    	std::getline(input, nline);            
        		
    	int mr, mg, mb;
    
		int cColNum = 0;

    	for(int i = 0; i < 256; i++){
 			std::getline(input, nline);

			if(input.eof()){
				break;
			}

			cColNum = i;
    		
			ntmp2 = nline;

			std::stringstream conwc(ntmp2); 

			int wordc = 0;

			while(conwc >> ntmp1){
				wordc++;			
			}
			
			if(wordc < 3){				
				return 1;
			}

			std::stringstream convert(nline);

			convert >> mr >> mg >> mb;	    		    	
			
			tpalette[i][0] = mMapColorIn[getClosest4Bit(mr)];
    		tpalette[i][1] = mMapColorIn[getClosest4Bit(mg)];
    		tpalette[i][2] = mMapColorIn[getClosest4Bit(mb)];  						
    	}

		if(cColNum < 255){
			while(cColNum < 256){
				tpalette[cColNum][0] = 0;
    			tpalette[cColNum][1] = 0;
    			tpalette[cColNum][2] = 0;    	
				cColNum++;
			}
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
				tmpcol.r = 0;
				tmpcol.g = 0;
				tmpcol.b = 0;
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
    		tmpChar = mMapColorOut[tpalette[i][1]];
			tmpChar = tmpChar << 4;
        	tmpChar += mMapColorOut[tpalette[i][2]];

			tbuffer.push_back(tmpChar);

			tmpChar = mMapColorOut[tpalette[i][0]];
        	tbuffer.push_back(tmpChar);
    	}
		
		mGlobalSettings.ProjectPalette = tbuffer;
		TPaletteEdit = TPalette;
		
		return 0;
	}
	std::cout << "Not Gimp palette!" << std::endl;
	return 1;
}

int TPalette::exportGimpPaletteEdit(std::string palPath, int expStart, int expRange){

	if(fs::exists(fs::status(palPath))){
		if(fs::is_directory(fs::status(palPath))){
			std::cout << "Palette path is directory: " << palPath << std::endl;
			return 1;
		}
	}

	if((expStart + expRange) > 256){
		return 1;
	}

	std::string tmpStr,tmpStr2;
    std::ofstream output(palPath, std::ios::out );
    std::stringstream convert;	

	tmpStr = "GIMP Palette";
	output << tmpStr << std::endl;

	fs::path cPalName = mGlobalSettings.ProjectPath;

#ifdef _WIN64
	tmpStr = "Name: " + cPalName.parent_path().filename().string();
#else
	tmpStr = "Name: " + cPalName.filename().string();
#endif

	output << tmpStr << std::endl;

	tmpStr = "Columns: 0";
	output << tmpStr << std::endl;

	tmpStr = "# Exported From TilemapEd";
	output << tmpStr << std::endl;

	for(int i = expStart; i < (expStart + expRange); i++){
		tmpStr = "";

		convert << (int)TPaletteEdit[i].r << std::endl;
		tmpStr2 = "";
		convert >> tmpStr2;
		tmpStr += tmpStr2 + "\t";

		convert << (int)TPaletteEdit[i].g << std::endl;
		tmpStr2 = "";
		convert >> tmpStr2;
		tmpStr += tmpStr2 + "\t";

		convert << (int)TPaletteEdit[i].b << std::endl;
		tmpStr2 = "";
		convert >> tmpStr2;
		tmpStr += tmpStr2 + "\t";

		convert << i << std::endl;
		tmpStr2 = "";
		convert >> tmpStr2;
		tmpStr += "Index " + tmpStr2;

		output << tmpStr << std::endl;
	}

	output.close();

	return 0;
}


int TPalette::exportGimpPaletteEdit(std::string palPath){

	if(fs::exists(fs::status(palPath))){
		if(fs::is_directory(fs::status(palPath))){
			std::cout << "Palette path is directory: " << palPath << std::endl;
			return 1;
		}
	}

	std::string tmpStr,tmpStr2;
    std::ofstream output(palPath, std::ios::out );
    std::stringstream convert;	

	tmpStr = "GIMP Palette";
	output << tmpStr << std::endl;

	fs::path cPalName = mGlobalSettings.ProjectPath;

#ifdef _WIN64
	tmpStr = "Name: " + cPalName.parent_path().filename().string();
#else
	tmpStr = "Name: " + cPalName.filename().string();
#endif

	output << tmpStr << std::endl;

	tmpStr = "Columns: 0";
	output << tmpStr << std::endl;

	tmpStr = "# Exported From TilemapEd";
	output << tmpStr << std::endl;

	for(int i = 0; i < 256; i++){
		tmpStr = "";

		convert << (int)TPaletteEdit[i].r << std::endl;
		tmpStr2 = "";
		convert >> tmpStr2;
		tmpStr += tmpStr2 + "\t";

		convert << (int)TPaletteEdit[i].g << std::endl;
		tmpStr2 = "";
		convert >> tmpStr2;
		tmpStr += tmpStr2 + "\t";

		convert << (int)TPaletteEdit[i].b << std::endl;
		tmpStr2 = "";
		convert >> tmpStr2;
		tmpStr += tmpStr2 + "\t";

		convert << i << std::endl;
		tmpStr2 = "";
		convert >> tmpStr2;
		tmpStr += "Index " + tmpStr2;

		output << tmpStr << std::endl;
	}

	output.close();
    
    return 0;
}

int TPalette::exportPaletteEdit(std::string palPath, int expStart, int expRange){

	if(fs::exists(fs::status(palPath))){
		if(fs::is_directory(fs::status(palPath))){
			std::cout << "Palette path is directory: " << palPath << std::endl;
			return 1;
		}
	}

	if((expStart + expRange) > 256){
		return 1;
	}

	std::vector<unsigned char> tbuffer;
	tbuffer.resize((expRange * 2) + 2);
	tbuffer[0] = 16;
	tbuffer[1] = 42;

	int pindex = expStart;
	for(int i = (2 + (expStart * 2)); i < ((expStart + expRange) * 2) + 2; i+=2){			
		tbuffer[i - (expStart * 2)] = mMapColorOut[TPaletteEdit[pindex].g] << 4;
		tbuffer[i - (expStart * 2)] += mMapColorOut[TPaletteEdit[pindex].b];
		tbuffer[i - (expStart * 2) +1] = mMapColorOut[TPaletteEdit[pindex].r];
		pindex++;
	}

	std::ofstream outfile(palPath, std::ios::binary );
	outfile.write((const char*)tbuffer.data(), tbuffer.size());
	outfile.close();

	return 0;

	return 0;
}

int TPalette::exportPaletteEdit(std::string palPath){

	if(fs::exists(fs::status(palPath))){
		if(fs::is_directory(fs::status(palPath))){
			std::cout << "Palette path is directory: " << palPath << std::endl;
			return 1;
		}
	}

	std::vector<unsigned char> tbuffer;
	tbuffer.resize(514);
	tbuffer[0] = 16;
	tbuffer[1] = 42;

	int pindex=0;
	for(int i = 2; i < 514; i+=2){			
		tbuffer[i] = mMapColorOut[TPaletteEdit[pindex].g] << 4;
		tbuffer[i] += mMapColorOut[TPaletteEdit[pindex].b];
		tbuffer[i+1] = mMapColorOut[TPaletteEdit[pindex].r];
		pindex++;
	}

	std::ofstream outfile(palPath, std::ios::binary );
	outfile.write((const char*)tbuffer.data(), tbuffer.size());
	outfile.close();

	return 0;
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
				tmpcol.r = 0;
				tmpcol.g = 0;
				tmpcol.b = 0;
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
			tmpcol.r = 0;
			tmpcol.g = 0;
			tmpcol.b = 0;		
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

int TPalette::initPaletteEdit(){
	
	for(int i = 0; i < 256; i++){		
		TPaletteEdit[i].r = palette[i][0];
		TPaletteEdit[i].g = palette[i][1];
		TPaletteEdit[i].b = palette[i][2];

		if(i == 0) {
			TPaletteEdit[i].r = 0;
			TPaletteEdit[i].g = 0;
			TPaletteEdit[i].b = 0;		
			TPaletteEdit[i].a = 0;
		} else {
			TPaletteEdit[i].a = 255;
		}		
		
		if(i == mGlobalSettings.mEditor->mColorSelectedEdit){
			setSelectedColor(TPaletteEdit[i]);				
		}
	}
		
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

	bool bColZero = false;	

	if(tcolor != 0) {ccolor = tcolor;} else { if(mGlobalSettings.mGlobalTexParam.PaletteOffset > 0){if(mGlobalSettings.mEditor->mColorSelectedTile->bPixelSelected){ if((mGlobalSettings.mEditor->mColorSelected % 16) == 0){ bColZero = true;}}}}
	
	if(bColZero){		
		TPixels[ccolor]->bPixelSelected = true;
	}

	SDL_Rect retval = TPixels[ccolor]->renderEdSel(xpos, ypos, cScale,mGlobalSettings.bShowTilePixelSelGrid);	

	if(bColZero){
		TPixels[ccolor]->bPixelSelected = false;
	}

	return retval;
}

SDL_Rect TPalette::renderTileEd(int xpos,int ypos, int tcolor, int cScale, TextureParameters *mTexParam){
	SDL_SetRenderDrawBlendMode(mGlobalSettings.TRenderer, SDL_BLENDMODE_BLEND);
	int ccolor=0;
	//if(tcolor != 0) ccolor = tcolor + (mTexParam->PaletteOffset*16);

	bool bColZero = false;	
	if(tcolor != 0){ccolor = tcolor + (mTexParam->PaletteOffset*16);} else { if(mTexParam->PaletteOffset > 0){if(mGlobalSettings.mEditor->mColorSelectedTile->bPixelSelected){ if((mGlobalSettings.mEditor->mColorSelected % 16) == 0){ bColZero = true;}}}}

	if(bColZero){
		TPixels[ccolor]->bPixelSelected = true;
	}

	SDL_Rect retval = TPixels[ccolor]->renderEdSel(xpos, ypos, cScale,mGlobalSettings.bShowTilePixelGrid);	

	if(bColZero){
		TPixels[ccolor]->bPixelSelected = false;
	}

	return retval;
}

SDL_Rect TPalette::renderTileEd(int xpos,int ypos, int tcolor, TextureParameters *mTexParam){
	SDL_SetRenderDrawBlendMode(mGlobalSettings.TRenderer, SDL_BLENDMODE_BLEND);
	int ccolor=0;
	bool bColZero = false;	
	if(tcolor != 0){ccolor = tcolor + (mTexParam->PaletteOffset*16);} else { if(mTexParam->PaletteOffset > 0){if(mGlobalSettings.mEditor->mColorSelectedTile->bPixelSelected){ if((mGlobalSettings.mEditor->mColorSelected % 16) == 0){ bColZero = true;}}}}

	if(bColZero){
		TPixels[ccolor]->bPixelSelected = true;
	}

	SDL_Rect retval = TPixels[ccolor]->renderEd(xpos, ypos, mTexParam, mTexParam->TexEditScale,false,mGlobalSettings.bShowPixelGrid);

	if(bColZero){
		TPixels[ccolor]->bPixelSelected = false;	
	}

	return retval;
}

SDL_Rect TPalette::renderSpriteEd(int xpos,int ypos, int tcolor, TextureParameters *mTexParam){
	SDL_SetRenderDrawBlendMode(mGlobalSettings.TRenderer, SDL_BLENDMODE_BLEND);
	int ccolor=0;
	
	bool bColZero = false;	
	if(tcolor != 0){ccolor = tcolor + (mTexParam->PaletteOffset*16);} else { if(mTexParam->PaletteOffset > 0){if(mGlobalSettings.mEditor->mColorSelectedTile->bPixelSelected){ if((mGlobalSettings.mEditor->mColorSelected % 16) == 0){ bColZero = true;}}}}

	if(bColZero){
		TPixels[ccolor]->bPixelSelected = true;
	}
	
	//if(tcolor != 0) ccolor = tcolor + (mTexParam->PaletteOffset*16);
	SDL_Rect retval = TPixels[ccolor]->renderEd(xpos, ypos, mTexParam, mTexParam->TexEditScale,false,mGlobalSettings.bShowPixelGridSprite);	

	if(bColZero){
		TPixels[ccolor]->bPixelSelected = false;
	}


	return retval;
}

int TPalette::renderIm(int xpos,int ypos, TextureParameters *mTexParam){
	
	ImVec2 cSize;
	if(mTexParam->TexBPP < 0x8){
		cSize.x = 90 + ((mTexParam->TexRenderSize*mGlobalSettings.PaletteScale+4) * 16);
		cSize.y = 50 + ((mTexParam->TexRenderSize*mGlobalSettings.PaletteScale+4) * 16);
	} else {
		cSize.x = 20 + ((mTexParam->TexRenderSize*mGlobalSettings.PaletteScale+4) * 16);
		cSize.y = 50 + ((mTexParam->TexRenderSize*mGlobalSettings.PaletteScale+4) * 16);
	}

	int rXpos = xpos;

	if(xpos > (mGlobalSettings.WindowWidth - cSize.x)){
		rXpos = mGlobalSettings.WindowWidth - cSize.x;
	}

	Dialog::render(rXpos, ypos);

	ImGui::Begin("Palette", NULL, ImGuiWindowFlags_NoNav);

	ImGui::SetWindowSize(cSize, ImGuiCond_Once);

	ImVec2 cPos = ImGui::GetWindowPos();
	cPos.x += 10;
	cPos.y += 35;

	if(mTexParam->TexBPP < 0x8){

	cPos.x += 70;

	ImGui::PushFont(mGlobalSettings.LFont);
		
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

	ImGui::PopFont();


		for(int i = 0; i < 16; i++){
			if(i == mTexParam->PaletteOffset){
				if(mTexParam->TexBPP == 0x4){
					for(int j = 0; j < 16; j++){
						PixelAreas[(i*16)+j] = TPixels[(i*16)+j]->renderIm(
						cPos.x + ((mTexParam->TexRenderSize*mGlobalSettings.PaletteScale+3)*j),
						cPos.y + ((mTexParam->TexRenderSize*mGlobalSettings.PaletteScale+3)*i),
						mGlobalSettings.PaletteScale,true,true);
					}
				} else  {
					for(int j = 0; j < 4; j++){
						PixelAreas[(i*16)+j] = TPixels[(i*16)+j]->renderIm(
						cPos.x + ((mTexParam->TexRenderSize*mGlobalSettings.PaletteScale+3)*j),
						cPos.y + ((mTexParam->TexRenderSize*mGlobalSettings.PaletteScale+3)*i),
						mGlobalSettings.PaletteScale,true,true);
					}
					for(int j = 4; j < 16; j++){
						PixelAreas[(i*16)+j] = TPixels[(i*16)+j]->renderImDisabled(
						cPos.x + ((mTexParam->TexRenderSize*mGlobalSettings.PaletteScale+3)*j),
						cPos.y + ((mTexParam->TexRenderSize*mGlobalSettings.PaletteScale+3)*i),
						mGlobalSettings.PaletteScale,true,true);
					}
				}
			} else {
				for(int j = 0; j < 16; j++){
					PixelAreas[(i*16)+j] = TPixels[(i*16)+j]->renderImDisabled(
					cPos.x + ((mTexParam->TexRenderSize*mGlobalSettings.PaletteScale+3)*j),
					cPos.y + ((mTexParam->TexRenderSize*mGlobalSettings.PaletteScale+3)*i),
					mGlobalSettings.PaletteScale,true,true);
				}
			}
		}

	} else {

	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j++){
			PixelAreas[(i*16)+j] = TPixels[(i*16)+j]->renderIm(
					cPos.x + ((mTexParam->TexRenderSize*mGlobalSettings.PaletteScale+3)*j),
					cPos.y + ((mTexParam->TexRenderSize*mGlobalSettings.PaletteScale+3)*i),
					mGlobalSettings.PaletteScale,true,true);
		}
	}
	}

	mGlobalSettings.mEditor->ImButtonsPalette.updateButtonStates();

	ImGui::End();

return 0;
}

void TPalette::setEditColor(){
	mR = getClosest4Bit((int)(mEditColor.x * 255.0f));
	mG = getClosest4Bit((int)(mEditColor.y * 255.0f));
	mB = getClosest4Bit((int)(mEditColor.z * 255.0f));
}

int TPalette::renderEditor(int xpos,int ypos){
	
	Dialog::render(xpos, ypos);

	ImGui::Begin("Palette: Edit Mode", NULL, ImGuiWindowFlags_NoNav);

	if(bImportingPalette){
		Dialog::render();
		ImGui::SetNextWindowSize(ImVec2(800, 600));
    	ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKeyPal", "Choose Palette File", ".gpl,.bin", ".");	
		bImportingPalette = false;
		bImportingOpen = true;
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
		bImportingOpen = false;
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
				mR = getClosest4Bit((int)(mEditColorPick.x * 255.0f));
				mG = getClosest4Bit((int)(mEditColorPick.y * 255.0f));
				mB = getClosest4Bit((int)(mEditColorPick.z * 255.0f));

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
	cSize.x = 20 + ((mGlobalSettings.mGlobalTexParam.TexRenderSize*mGlobalSettings.PaletteScale+4) * 16);
	cSize.y = 270 + ((mGlobalSettings.mGlobalTexParam.TexRenderSize*mGlobalSettings.PaletteScale+4) * 16);

	ImGui::SetWindowSize(cSize, ImGuiCond_Once);

	ImVec2 cPos = ImGui::GetWindowPos();
	cPos.x += 10;
	cPos.y += 255;

	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j++){
			PixelAreas[(i*16)+j] = TPixels[(i*16)+j]->renderEditor(
					cPos.x + ((mGlobalSettings.mGlobalTexParam.TexRenderSize*mGlobalSettings.PaletteScale+4)*j),
					cPos.y + ((mGlobalSettings.mGlobalTexParam.TexRenderSize*mGlobalSettings.PaletteScale+4)*i),
					mGlobalSettings.PaletteScale,true,true);
		}
	}

	mGlobalSettings.mEditor->ImButtonsPalette.updateButtonStates();

	if(mGlobalSettings.mEditor->mColorSelectedEdit > 0){
		TPaletteEdit[mGlobalSettings.mEditor->mColorSelectedEdit] = getSDLColor4Bit(mR, mG, mB);
		mEditColor = getIm4Color(TPaletteEdit[mGlobalSettings.mEditor->mColorSelectedEdit]);
	}

	//TPaletteEdit[mGlobalSettings.CurrentEditor->mColorSelectedEdit] = getSDLColor(mEditColor);

	if(ImGui::Button("Apply Changes")){
		mGlobalSettings.mEditor->activateDTDialog(EDIALOG_PALETTEUPDATE);
	}

	ImGui::SameLine();

	if(ImGui::Button("Cancel Changes")){
		TPaletteEdit = TPalette;
		mEditColor = getIm4Color(TPalette[mGlobalSettings.mEditor->mColorSelectedEdit]);
		setEditColor();	
	}

	ImGui::End();

return 0;
}


Tile::~Tile(){	
	freeTexture();
}

void Tile::freeTexture(){
	if(mTexParam->TexBPP < 8){
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

int TTexture::setPixel(int pindex, unsigned char pcolor, std::vector<unsigned char> &tBuf, TextureParameters *cTexParam){
	
	if(cTexParam->TexBPP == 0x8){
		tBuf[pindex] = pcolor;
	}

	if(cTexParam->TexBPP == 0x4){
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

	return 0;

}

int TTexture::setPixel(int pindex, unsigned char pcolor, std::vector<unsigned char> &tBuf){
	if(mTexParam->TexBPP == 0x8){
		tBuf[pindex] = pcolor;
	}

	if(mTexParam->TexBPP == 0x4){
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

	if(mTexParam->TexBPP == 0x2){
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
	if(mTexParam->TexBPP == 0x8){
		FileData[pindex] = pcolor;
	}
	if(mTexParam->TexBPP == 0x4){
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
	if(mTexParam->TexBPP == 0x2){
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
		nindex = pindex - 2 * (pindex % mTexParam->TexSizeX ) + mTexParam->TexSizeX  - 1;	
	}

	if(tflip == 2){				
		nindex = (mTexParam->TexSizeY - 1 - pindex / mTexParam->TexSizeX) * mTexParam->TexSizeX + pindex % mTexParam->TexSizeX;		
	}

	if(tflip == 3){
		nindex =  (mTexParam->TexSizeX *mTexParam->TexSizeY ) - pindex - 1;		
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
		nindex = pindex - 2 * (pindex % mTexParam->TexSizeX ) + mTexParam->TexSizeX  - 1;
		tpix = getPixel(nindex);
	}

	if(tflip == 2){				
		nindex = (mTexParam->TexSizeY - 1 - pindex / mTexParam->TexSizeX) * mTexParam->TexSizeX + pindex % mTexParam->TexSizeX;
		tpix = getPixel(nindex);
	}

	if(tflip == 3){
		nindex =  (mTexParam->TexSizeX *mTexParam->TexSizeY ) - pindex - 1;
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
	if(mTexParam->TexBPP == 0x8){
		 return FileData[pindex];
	}
	if(mTexParam->TexBPP == 0x4){
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
	if(mTexParam->TexBPP == 0x2){
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
	if(mTexParam->TexSizeX != mTexParam->TexSizeY){return 1;}

	std::vector<unsigned char> tmpData;
	tmpData.resize(FileData.size());

	int index;
	int lsize = mTexParam->TexSizeX;
	unsigned char tmpPix;

	
	for(int i = 0; i < mTexParam->TexSizeY; i++){
		for(int j = 0; j < mTexParam->TexSizeX; j++){
			index = (lsize*(lsize-j-1))+i;
			tmpPix = getPixel(index);		
			setPixel((i*(lsize))+j, tmpPix, tmpData);
		}
	}

	FileData = tmpData;

	updateTexture(&mGlobalSettings.mEditor->mPalette);

	return 0;
}

int Tile::rotatel(){
	if(mTexParam->TexSizeX != mTexParam->TexSizeY){return 1;}

	std::vector<unsigned char> tmpData;
	tmpData.resize(FileData.size());

	int index;
	int lsize = mTexParam->TexSizeX;
	unsigned char tmpPix;

	
	for(int i = 0; i < mTexParam->TexSizeY; i++){
		for(int j = 0; j < mTexParam->TexSizeX; j++){			
			index = (i*(lsize))+j;
			tmpPix = getPixel(index);		
			setPixel((lsize*(lsize-j-1))+i, tmpPix, tmpData);
		}
	}

	FileData = tmpData;

	updateTexture(&mGlobalSettings.mEditor->mPalette);

	return 0;
}

int getEuclideanDistance(const SDL_Color& color1, const SDL_Color& color2) {
    int dR = color1.r - color2.r;
    int dG = color1.g - color2.g;
    int dB = color1.b - color2.b;
    int dA = color1.a - color2.a;

    return dR * dR + dG * dG + dB * dB + dA * dA;
}

unsigned char findClosestPaletteColor(const SDL_Color& color, int cMaxPal, int cOffset = 0) {
    unsigned char closestColor = 0;
    int minDistance = std::numeric_limits<int>::max();

    for (int i = 0; i < cMaxPal; i++) {

		int pOffset = 0;

		if((i > 0) && (cOffset > 0)){
			pOffset = 16 * cOffset;
		}

        SDL_Color paletteColor;
        paletteColor.r = mGlobalSettings.mEditor->mPalette.TPalette[i + pOffset].r;
        paletteColor.g = mGlobalSettings.mEditor->mPalette.TPalette[i + pOffset].g;
        paletteColor.b = mGlobalSettings.mEditor->mPalette.TPalette[i + pOffset].b;
		paletteColor.a = mGlobalSettings.mEditor->mPalette.TPalette[i + pOffset].a;

        int distance = getEuclideanDistance(color, paletteColor);

        if (distance < minDistance) {
            minDistance = distance;
            closestColor = i;
        }
    }
	
    return closestColor;
}

int Tile::rotate(double cAngle){

	if(((mTexParam->TexBPP == 8) || (mTexParam->TexBPP == 4) ) && (mTexParam->TexSizeX == mTexParam->TexSizeY)){
		
		std::vector<unsigned char> bitmap;
		bitmap.resize((mTexParam->TexSizeX * mTexParam->TexSizeY)/mGlobalSettings.mTileBPPSize[mTexParam->TexBPP],0);
		int *pixels = new int[mTexParam->TexSizeX * mTexParam->TexSizeY];

		if(mGlobalSettings.mUseTextureFiltering){
			std::stringstream convert;
			convert << mGlobalSettings.mUseTextureFiltering << std::endl;
			std::string cQual;
			convert >> cQual;
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, cQual.c_str());
			if(mGlobalSettings.mUseTextureFiltering > 1){
				recreateTexture(&mGlobalSettings.mEditor->mPalette);
			}
		}
		
		SDL_Texture *rTexture = SDL_CreateTexture( mGlobalSettings.TRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, mTexParam->TexSizeX * 8, mTexParam->TexSizeY * 8);
		SDL_Texture *rTexRot = SDL_CreateTexture( mGlobalSettings.TRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, mTexParam->TexSizeX * 8, mTexParam->TexSizeY * 8);
		SDL_Texture *rTexDest = SDL_CreateTexture( mGlobalSettings.TRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, mTexParam->TexSizeX, mTexParam->TexSizeY );

 		if(rTexture == NULL){
        	std::cout <<  "Unable to Create Texture Target: " <<  SDL_GetError()  << std::endl;
    	}
    	
		if(SDL_SetRenderTarget(mGlobalSettings.TRenderer, rTexture)){
			std::cout <<  "Unable to Set RenderTarget: " <<  SDL_GetError()  << std::endl;
		}

		if(mTexParam->TexBPP == 4){
			SDL_RenderCopyEx(mGlobalSettings.TRenderer, TPOffset[mTexParam->PaletteOffset], NULL, NULL, 0, NULL, SDL_FLIP_NONE );
		} else {
			SDL_RenderCopyEx(mGlobalSettings.TRenderer, TileTex, NULL, NULL, 0, NULL, SDL_FLIP_NONE );
		}

		SDL_RenderPresent( mGlobalSettings.TRenderer );

		if(SDL_SetRenderTarget(mGlobalSettings.TRenderer, rTexRot)){
			std::cout <<  "Unable to Set RenderTarget Rot: " <<  SDL_GetError()  << std::endl;
		}

		SDL_RenderCopyEx(mGlobalSettings.TRenderer, rTexture, NULL, NULL, cAngle, NULL, SDL_FLIP_NONE );

		SDL_RenderPresent( mGlobalSettings.TRenderer );

		if(SDL_SetRenderTarget(mGlobalSettings.TRenderer, rTexDest)){
			std::cout <<  "Unable to Set RenderTarget Dest: " <<  SDL_GetError()  << std::endl;
		}

		SDL_RenderCopyEx(mGlobalSettings.TRenderer, rTexRot, NULL, NULL, 0, NULL, SDL_FLIP_NONE );

		SDL_RenderPresent( mGlobalSettings.TRenderer );
		
		if(SDL_RenderReadPixels(mGlobalSettings.TRenderer, NULL, SDL_PIXELFORMAT_RGBA8888, pixels, mTexParam->TexSizeX * 4 )){			
			std::cout <<  "Unable to Read Pixels: " <<  SDL_GetError()  << std::endl;
		}

	    for (int y = 0; y < mTexParam->TexSizeY; y++) {
    	    for (int x = 0; x < mTexParam->TexSizeX; x++) {
				
				SDL_Color cTestCol;
				cTestCol.r = (pixels[(y * mTexParam->TexSizeX) + x] & 0xFF000000) >> 24;
				cTestCol.g = (pixels[(y * mTexParam->TexSizeX) + x] & 0x00FF0000) >> 16;
				cTestCol.b = (pixels[(y * mTexParam->TexSizeX) + x] & 0x0000FF00) >> 8;
				cTestCol.a = (pixels[(y * mTexParam->TexSizeX) + x] & 0x000000FF);
				
				if(mTexParam->TexBPP == 4){
					TTexture::setPixel(y * mTexParam->TexSizeX + x, findClosestPaletteColor(cTestCol, 16, mTexParam->PaletteOffset), bitmap, mTexParam);					
				} else {
        	    	bitmap[y * mTexParam->TexSizeX + x] = findClosestPaletteColor(cTestCol, 256);
				}
   		     }
    	}	
   	
		SDL_SetRenderTarget(mGlobalSettings.TRenderer, mGlobalSettings.TTarget);
		SDL_DestroyTexture(rTexture);		
		SDL_DestroyTexture(rTexRot);
		SDL_DestroyTexture(rTexDest);

		if(mGlobalSettings.mUseTextureFiltering){
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
		}

		FileData = bitmap;
		recreateTexture(&mGlobalSettings.mEditor->mPalette);
		delete[] pixels;

	}
	return 0;
}

int Tile::applyFilter(){

	if(mGlobalSettings.mUseTextureFiltering){

		std::vector<unsigned char> bitmap;
		bitmap.resize((mTexParam->TexSizeX * mTexParam->TexSizeY)/mGlobalSettings.mTileBPPSize[mTexParam->TexBPP],0);
		int *pixels = new int[mTexParam->TexSizeX * mTexParam->TexSizeY];


		std::stringstream convert;
		convert << mGlobalSettings.mUseTextureFiltering << std::endl;
		std::string cQual;
		convert >> cQual;
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, cQual.c_str());
		if(mGlobalSettings.mUseTextureFiltering > 1){
			recreateTexture(&mGlobalSettings.mEditor->mPalette);			
		}

		SDL_Texture *rTexture = SDL_CreateTexture( mGlobalSettings.TRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, mTexParam->TexSizeX * 8, mTexParam->TexSizeY * 8);
		SDL_Texture *rTexScale = SDL_CreateTexture( mGlobalSettings.TRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, mTexParam->TexSizeX * 8, mTexParam->TexSizeY * 8);
		SDL_Texture *rTexDest = SDL_CreateTexture( mGlobalSettings.TRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, mTexParam->TexSizeX, mTexParam->TexSizeY );

		if(SDL_SetRenderTarget(mGlobalSettings.TRenderer, rTexture)){
			std::cout <<  "Unable to Set RenderTarget: " <<  SDL_GetError()  << std::endl;
		}
		
		if(mTexParam->TexBPP == 4){
			SDL_RenderCopyEx(mGlobalSettings.TRenderer, TPOffset[mTexParam->PaletteOffset], NULL, NULL, 0, NULL, SDL_FLIP_NONE );
		} else {
			SDL_RenderCopyEx(mGlobalSettings.TRenderer, TileTex, NULL, NULL, 0, NULL, SDL_FLIP_NONE );
		}

		SDL_RenderPresent( mGlobalSettings.TRenderer );

		if(SDL_SetRenderTarget(mGlobalSettings.TRenderer, rTexScale)){
			std::cout <<  "Unable to Set RenderTarget Scale: " <<  SDL_GetError()  << std::endl;
		}

		SDL_RenderCopyEx(mGlobalSettings.TRenderer, rTexture, NULL, NULL, 0, NULL, SDL_FLIP_NONE );

		SDL_RenderPresent( mGlobalSettings.TRenderer );

		if(SDL_SetRenderTarget(mGlobalSettings.TRenderer, rTexDest)){
			std::cout <<  "Unable to Set RenderTarget Dest: " <<  SDL_GetError()  << std::endl;
		}

		SDL_RenderCopyEx(mGlobalSettings.TRenderer, rTexScale, NULL, NULL, 0, NULL, SDL_FLIP_NONE );

		SDL_RenderPresent( mGlobalSettings.TRenderer );

		if(SDL_RenderReadPixels(mGlobalSettings.TRenderer, NULL, SDL_PIXELFORMAT_RGBA8888, pixels, mTexParam->TexSizeX * 4 )){			
			std::cout <<  "Unable to Read Pixels: " <<  SDL_GetError()  << std::endl;
		}

	    for (int y = 0; y < mTexParam->TexSizeY; y++) {
    	    for (int x = 0; x < mTexParam->TexSizeX; x++) {
				
				SDL_Color cTestCol;
				cTestCol.r = (pixels[(y * mTexParam->TexSizeX) + x] & 0xFF000000) >> 24;
				cTestCol.g = (pixels[(y * mTexParam->TexSizeX) + x] & 0x00FF0000) >> 16;
				cTestCol.b = (pixels[(y * mTexParam->TexSizeX) + x] & 0x0000FF00) >> 8;
				cTestCol.a = (pixels[(y * mTexParam->TexSizeX) + x] & 0x000000FF);				
				
				if(mTexParam->TexBPP == 4){
					TTexture::setPixel(y * mTexParam->TexSizeX + x, findClosestPaletteColor(cTestCol, 16, mTexParam->PaletteOffset), bitmap, mTexParam);					
				} else {
        	    	bitmap[y * mTexParam->TexSizeX + x] = findClosestPaletteColor(cTestCol, 256);
				}
   		     }
    	}	
   	
		SDL_SetRenderTarget(mGlobalSettings.TRenderer, mGlobalSettings.TTarget);
		SDL_DestroyTexture(rTexture);

		
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");			

		FileData = bitmap;
		recreateTexture(&mGlobalSettings.mEditor->mPalette);
		delete[] pixels;

		return 0;
	}

	return 1;
}

int Tile::upscale(Tile *cCopyTile){
	if( (mTexParam->TexBPP == 8) || (mTexParam->TexBPP == 4) ){

		std::vector<unsigned char> bitmap;
		bitmap.resize((mTexParam->TexSizeX * mTexParam->TexSizeY)/mGlobalSettings.mTileBPPSize[mTexParam->TexBPP],0);
		int *pixels = new int[mTexParam->TexSizeX * mTexParam->TexSizeY];
		
		if(mGlobalSettings.mUseTextureFiltering){
			std::stringstream convert;
			convert << mGlobalSettings.mUseTextureFiltering << std::endl;
			std::string cQual;
			convert >> cQual;
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, cQual.c_str());
			if(mGlobalSettings.mUseTextureFiltering > 1){
				recreateTexture(&mGlobalSettings.mEditor->mPalette);
				cCopyTile->recreateTexture(&mGlobalSettings.mEditor->mPalette);
			}
		}
		
		SDL_Texture *rTexture = SDL_CreateTexture( mGlobalSettings.TRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, mTexParam->TexSizeX, mTexParam->TexSizeY);

		if(SDL_SetRenderTarget(mGlobalSettings.TRenderer, rTexture)){
			std::cout <<  "Unable to Set RenderTarget: " <<  SDL_GetError()  << std::endl;
		}
		
		if(mTexParam->TexBPP == 4){
			SDL_RenderCopyEx(mGlobalSettings.TRenderer, cCopyTile->TPOffset[mTexParam->PaletteOffset], NULL, NULL, 0, NULL, SDL_FLIP_NONE );
			std::cout <<  "Render Target Sprite BPP 4" << std::endl;
		} else {
			SDL_RenderCopyEx(mGlobalSettings.TRenderer, cCopyTile->TileTex, NULL, NULL, 0, NULL, SDL_FLIP_NONE );
			std::cout <<  "Render Target Sprite BPP 8" << std::endl;
		}

		SDL_RenderPresent( mGlobalSettings.TRenderer );
		
		if(SDL_RenderReadPixels(mGlobalSettings.TRenderer, NULL, SDL_PIXELFORMAT_RGBA8888, pixels, mTexParam->TexSizeX * 4 )){			
			std::cout <<  "Unable to Read Pixels: " <<  SDL_GetError()  << std::endl;
		}

	    for (int y = 0; y < mTexParam->TexSizeY; y++) {
    	    for (int x = 0; x < mTexParam->TexSizeX; x++) {
				
				SDL_Color cTestCol;
				cTestCol.r = (pixels[(y * mTexParam->TexSizeX) + x] & 0xFF000000) >> 24;
				cTestCol.g = (pixels[(y * mTexParam->TexSizeX) + x] & 0x00FF0000) >> 16;
				cTestCol.b = (pixels[(y * mTexParam->TexSizeX) + x] & 0x0000FF00) >> 8;
				cTestCol.a = (pixels[(y * mTexParam->TexSizeX) + x] & 0x000000FF);				
				
				if(mTexParam->TexBPP == 4){
					TTexture::setPixel(y * mTexParam->TexSizeX + x, findClosestPaletteColor(cTestCol, 16, mTexParam->PaletteOffset), bitmap, mTexParam);					
				} else {
        	    	bitmap[y * mTexParam->TexSizeX + x] = findClosestPaletteColor(cTestCol, 256);
				}
   		     }
    	}	
   	
		SDL_SetRenderTarget(mGlobalSettings.TRenderer, mGlobalSettings.TTarget);
		SDL_DestroyTexture(rTexture);

		if(mGlobalSettings.mUseTextureFiltering){
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
		}	

		FileData = bitmap;
		recreateTexture(&mGlobalSettings.mEditor->mPalette);
		cCopyTile->recreateTexture(&mGlobalSettings.mEditor->mPalette);
		delete[] pixels;
	}
	return 0;
}

int Tile::scale(double cScale){

	if(((mTexParam->TexBPP == 8) || (mTexParam->TexBPP == 4) ) && (mTexParam->TexSizeX == mTexParam->TexSizeY)){
		
		std::vector<unsigned char> bitmap;
		bitmap.resize((mTexParam->TexSizeX * mTexParam->TexSizeY)/mGlobalSettings.mTileBPPSize[mTexParam->TexBPP],0);
		int *pixels = new int[mTexParam->TexSizeX * mTexParam->TexSizeY];
		
		if(mGlobalSettings.mUseTextureFiltering){
			std::stringstream convert;
			convert << mGlobalSettings.mUseTextureFiltering << std::endl;
			std::string cQual;
			convert >> cQual;
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, cQual.c_str());
			if(mGlobalSettings.mUseTextureFiltering > 1){
				recreateTexture(&mGlobalSettings.mEditor->mPalette);
			}
		}
		
		SDL_Texture *rTexture = SDL_CreateTexture( mGlobalSettings.TRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, mTexParam->TexSizeX, mTexParam->TexSizeY);
		
 		if(rTexture == NULL){
        	std::cout <<  "Unable to Create Texture Target: " <<  SDL_GetError()  << std::endl;
    	}
    			
		SDL_Rect tDest;
		SDL_Rect tSrc;

		int nWidth = (mTexParam->TexSizeX * cScale);
		int nHeight = (mTexParam->TexSizeY * cScale);

		tDest.x = -((nWidth - (mTexParam->TexSizeX))/2);
		tDest.y = -((nHeight - (mTexParam->TexSizeY))/2);
		tDest.h = nHeight;
		tDest.w = nWidth;

		tSrc.x = 0;
		tSrc.y = 0;
		tSrc.h = mTexParam->TexSizeY;;
		tSrc.w = mTexParam->TexSizeX;
		
		if(SDL_SetRenderTarget(mGlobalSettings.TRenderer, rTexture)){
			std::cout <<  "Unable to Set RenderTarget: " <<  SDL_GetError()  << std::endl;
		}
		
		if(mTexParam->TexBPP == 4){
			SDL_RenderCopyEx(mGlobalSettings.TRenderer, TPOffset[mTexParam->PaletteOffset], &tSrc, &tDest, 0, NULL, SDL_FLIP_NONE );
		} else {
			SDL_RenderCopyEx(mGlobalSettings.TRenderer, TileTex, &tSrc, &tDest, 0, NULL, SDL_FLIP_NONE );
		}

		SDL_RenderPresent( mGlobalSettings.TRenderer );
		
		if(SDL_RenderReadPixels(mGlobalSettings.TRenderer, NULL, SDL_PIXELFORMAT_RGBA8888, pixels, mTexParam->TexSizeX * 4 )){			
			std::cout <<  "Unable to Read Pixels: " <<  SDL_GetError()  << std::endl;
		}

	    for (int y = 0; y < mTexParam->TexSizeY; y++) {
    	    for (int x = 0; x < mTexParam->TexSizeX; x++) {
				
				SDL_Color cTestCol;
				cTestCol.r = (pixels[(y * mTexParam->TexSizeX) + x] & 0xFF000000) >> 24;
				cTestCol.g = (pixels[(y * mTexParam->TexSizeX) + x] & 0x00FF0000) >> 16;
				cTestCol.b = (pixels[(y * mTexParam->TexSizeX) + x] & 0x0000FF00) >> 8;
				cTestCol.a = (pixels[(y * mTexParam->TexSizeX) + x] & 0x000000FF);				
				
				if(mTexParam->TexBPP == 4){
					TTexture::setPixel(y * mTexParam->TexSizeX + x, findClosestPaletteColor(cTestCol, 16, mTexParam->PaletteOffset), bitmap, mTexParam);					
				} else {
        	    	bitmap[y * mTexParam->TexSizeX + x] = findClosestPaletteColor(cTestCol, 256);
				}
   		     }
    	}	
   	
		SDL_SetRenderTarget(mGlobalSettings.TRenderer, mGlobalSettings.TTarget);
		SDL_DestroyTexture(rTexture);

		if(mGlobalSettings.mUseTextureFiltering){
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
		}	

		FileData = bitmap;
		recreateTexture(&mGlobalSettings.mEditor->mPalette);
		delete[] pixels;

	}
	return 0;
}

int Tile::recreateTexture(TPalette* tpal){
	
	freeTexture();
	initTile();
	initTexture();
    FileData.resize(((mTexParam->TexSizeX * mTexParam->TexSizeY)/mGlobalSettings.mTileBPPSize[mTexParam->TexBPP]), 0);	
	updateTexture(tpal);
	
	return 0;
}


int Tile::updateTexture(TPalette* tpal){
	if(mTexParam->TexBPP < 0x8){
		for(int j = 0; j < 16; j++){
			if(PixelData.size()){		
				PixelData.erase(PixelData.begin(),PixelData.end());
			}

			for(int i = 0; i < (mTexParam->TexSizeX * mTexParam->TexSizeY); i++){
    			PixelData.push_back(tpal->mapPaletteColor(getPixel(i, j)));
			}			
    		SDL_UpdateTexture(TPOffset[j], NULL, PixelData.data(), mTexParam->TexSizeX * sizeof(Uint32));		
		}
	} else {
		TTexture::updateTexture(tpal);
	}

	return 0;
}


int Tile::createNew(TPalette* tpal){
	
	initTile();
	initTexture();
    FileData.resize(((mTexParam->TexSizeX * mTexParam->TexSizeY)/mGlobalSettings.mTileBPPSize[mTexParam->TexBPP]), 0);	
	updateTexture(tpal);
	
	return 0;
}

int Tile::initTexture(){
	if(mTexParam->TexBPP < 0x8){
		for(int i = 0; i < 16; i++){			
			TPOffset[i] = SDL_CreateTexture(mGlobalSettings.TRenderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, mTexParam->TexSizeX,mTexParam->TexSizeY);
			SDL_SetTextureBlendMode(TPOffset[i], SDL_BLENDMODE_BLEND);
		}

	} else {
		TTexture::initTexture();
	}
	
	return 0;
}



int Tile::initTile(){
	SDL_Rect eRect = {0,0,1,1};
	PixelAreas.resize(mTexParam->TexSizeX*mTexParam->TexSizeY,eRect);
	if(mTexParam->TexBPP < 0x8){
		TPOffset.resize(16, 0);
	}
	mSelection.init(mTexParam->TexSizeX, mTexParam->TexSizeY,&mTexParam->TexPixelSize, &mTexParam->TexPixelSize, &mTexParam->TexEditScale);
	return 0;
}


SDL_Rect Tile::renderImCol(int xpos, int ypos, int mIndex, int mColVal, int tscale, bool bColEditSelected){
	
	SDL_Rect tmpRect;

	if(mTexParam->TexBPP < 0x8){
		ImGui::ImageButton((ImTextureID)(intptr_t)TPOffset[mTexParam->PaletteOffset], ImVec2(mTexParam->TexSizeX * tscale, mTexParam->TexSizeY * tscale) , ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1));	
	} else {
		ImGui::ImageButton((ImTextureID)(intptr_t)TileTex, ImVec2(mTexParam->TexSizeX * tscale, mTexParam->TexSizeY * tscale), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1));
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
		elmin.x--;
		elmin.y--;
		elmax.x++;;
		elmax.y++;
		tList->AddRect(elmin, elmax, mGlobalSettings.ImAltHighLightColor); //ImAltHighLightColor
		elmin.x--;
		elmin.y--;
		elmax.x++;;
		elmax.y++;
		tList->AddRect(elmin, elmax, mGlobalSettings.ImAltHighLightColor); //ImAltHighLightColor
	} else {
		tList->AddRect(elmin, elmax, mGlobalSettings.ImHighLightColor);		
	}
	
	if( mGlobalSettings.mEditor->mTileMap->bRenderOverlay ||mGlobalSettings.mEditor->mTileMap->bRenderOverlayColMap || mGlobalSettings.mProjectSettings.ColMap_ShowOverlay->bvalue ){
		
		if( ((elmax.x - elmin.x) > mGlobalSettings.mEditor->mTileSet.mOverlayScaleX) && ((elmax.y - elmin.y) > (mGlobalSettings.mEditor->mTileSet.mOverlayScaleY * 2 ))){
		
			std::stringstream conv;
			std::string tidx;

			conv << mIndex << std::endl;
			conv >> tidx;

			tList->AddText(ImVec2((float) elmin.x + 3, (float) elmin.y), mGlobalSettings.mEditor->mPalette.getImColor(mGlobalSettings.OverlayTextColor), tidx.c_str());

			conv << mColVal << std::endl;
			conv >> tidx;

			tidx = "C" + tidx;

			tList->AddText(ImVec2((float) elmin.x + 3, (float) elmin.y + mGlobalSettings.mEditor->mTileSet.mOverlayScaleY), mGlobalSettings.mEditor->mPalette.getImColor(mGlobalSettings.OverlayTextColor), tidx.c_str());
		}
	}

	return tmpRect;
}

SDL_Rect Tile::renderIm(int xpos, int ypos, int mIndex, int &mDragAndDropped, int tscale, bool updateRect ,bool drawGrid){
	
	SDL_Rect tmpRect;

	if(mTexParam->TexBPP < 0x8){
		ImGui::ImageButton((ImTextureID)(intptr_t)TPOffset[mTexParam->PaletteOffset], ImVec2(mTexParam->TexSizeX * tscale, mTexParam->TexSizeY * tscale) , ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1));	
	} else {
		ImGui::ImageButton((ImTextureID)(intptr_t)TileTex, ImVec2(mTexParam->TexSizeX * tscale, mTexParam->TexSizeY * tscale), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1));
	}

	int mode = 0;
	int n = mIndex;

	if(mGlobalSettings.mEditor->bLCTRLisDown){
		mode = 1;
	} else if(mGlobalSettings.mEditor->bLShiftIsDown){
		mode = 2;				
	}

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)){                    
        ImGui::SetDragDropPayload("DND_TILE", &n, sizeof(int));
		if (mode == 2) { ImGui::Text("Move"); }                    
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
		tList->AddRect(elmin, elmax, mGlobalSettings.ImAltHighLightColor); //ImAltHighLightColor
		elmin.x--;
		elmin.y--;
		elmax.x++;;
		elmax.y++;
		tList->AddRect(elmin, elmax, mGlobalSettings.ImAltHighLightColor); //ImAltHighLightColor
		elmin.x--;
		elmin.y--;
		elmax.x++;;
		elmax.y++;
		tList->AddRect(elmin, elmax, mGlobalSettings.ImAltHighLightColor); //ImAltHighLightColor
	} else {
		tList->AddRect(elmin, elmax, mGlobalSettings.ImHighLightColor);
	}

	if( (mGlobalSettings.mEditor->mTileMap->bRenderOverlay) || (mGlobalSettings.mEditor->mTileSet.bImRenderOverlay)){
		
		if((elmax.x - elmin.x) > mGlobalSettings.mEditor->mTileSet.mOverlayScaleX){
		
			std::stringstream conv;
			std::string tidx;

			conv << mIndex << std::endl;
			conv >> tidx;

			tList->AddText(ImVec2((float) elmin.x + 3, (float) elmin.y + 3), mGlobalSettings.mEditor->mPalette.getImColor(mGlobalSettings.OverlayTextColor), tidx.c_str());
		}
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

	if(mTexParam->TexBPP < 0x8){		
		tmpRect = { xpos, ypos, mTexParam->TexSizeX*tscale, mTexParam->TexSizeY*tscale};
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
	for(int i=0; i < mTexParam->TexSizeY; i++){
		for(int j=0; j < mTexParam->TexSizeX; j++){
			PixelAreas[j+(mTexParam->TexSizeX*i)] = tpal->renderTileEd(xpos + (cScale)*j, ypos + (cScale)*i, getPixel(j+(i*mTexParam->TexSizeX)), cScale, mTexParam); 			//mGlobalSettings.TilePixelSize * 
		}
	}

}

void Tile::renderEd(int xpos, int ypos, TPalette* tpal){
	for(int i=0; i < mTexParam->TexSizeY; i++){
		for(int j=0; j < mTexParam->TexSizeX; j++){
			PixelAreas[j+(mTexParam->TexSizeX*i)] = tpal->renderTileEd(xpos + (mTexParam->TexPixelSize * mTexParam->TexEditScale)*j, ypos + (mTexParam->TexPixelSize * mTexParam->TexEditScale)*i, getPixel(j+(i*mTexParam->TexSizeX)), mTexParam); 			
		}
	}
	
	if(mGlobalSettings.mEditor->mCurrentBrushPixel){ // && !mGlobalSettings.CurrentEditor->mBrushesPixel.bIsEditing){
		if(mGlobalSettings.mEditor->mCurrentBrushPixel->mSelected.size()){
			for(int i=0; i < mTexParam->TexSizeY; i++){
				for(int j=0; j < mTexParam->TexSizeX; j++){	
					if(mGlobalSettings.mEditor->mCurrentBrushPixel->findInSelection((j+(i*mTexParam->TexSizeX))) != -1){
						int findex = mGlobalSettings.mEditor->mCurrentBrushPixel->findInSelection((j+(i*mTexParam->TexSizeX)));
						if(mGlobalSettings.mEditor->mCurrentBrushPixel->mBrushElements[findex] != -1){
							tpal->renderTileEd(xpos + (mTexParam->TexPixelSize * mTexParam->TexEditScale)*j, ypos + (mTexParam->TexPixelSize * mTexParam->TexEditScale)*i, mGlobalSettings.mEditor->mCurrentBrushPixel->mBrushElements[findex], mTexParam); 							
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

int Tile::applyOffset(int poffset, bool bReplaceZero){
	unsigned char pixval;
	
	if(mTexParam->TexBPP == 8){
		for(int pi = 0; pi < (mTexParam->TexSizeX * mTexParam->TexSizeY); pi++){
			pixval = FileData[pi];

			if((pixval > 0) || bReplaceZero){

				pixval += poffset;				
				
				if((pixval == 0) && !bReplaceZero){
					if(poffset < 0){
						pixval = 255;
					} else {
						pixval = 1;
					}
				}
				FileData[pi] = pixval;
			}
		}

		updateTexture(&mGlobalSettings.mEditor->mPalette);
		return 0;
	}

	if(mTexParam->TexBPP == 4){
		for(int pi = 0; pi < (mTexParam->TexSizeX * mTexParam->TexSizeY); pi++){
			pixval = getPixel(pi);
			
			if((pixval > 0) || bReplaceZero){
				
				pixval += poffset;				
				
				if(pixval > 15){
					pixval = pixval % 16;
				}

				if((pixval == 0) && !bReplaceZero){
					if(poffset < 0){
						pixval = 15;
					} else {
						pixval = 1;
					}
				}
				setPixel(pi, pixval);
			}
		}
		updateTexture(&mGlobalSettings.mEditor->mPalette);
		return 0;
	}

	return 1;
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
	mClipboardTiles.close();
	mClipboardTileSet.close();
	
	mCurColumns = 1;		
	mSelEdWidth = 4;
	mCurEdScale = 10;	
	mMaxColumns = 4;

	mMaxTileScale = 4;
	
	mCurTileScale = TileSet::MaxTile;

	mMinTileScale = 7;
	mChildTop = 0;
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
		if(newSurf && (mGlobalSettings.mGlobalTexParam.TexBPP > 0x2)){
			if(newSurf->format->BitsPerPixel == 8){
				if((newSurf->w == mGlobalSettings.mGlobalTexParam.TexSizeX) && (newSurf->h == mGlobalSettings.mGlobalTexParam.TexSizeY)){
					std::vector<unsigned char> tbuffer;
					if(mGlobalSettings.mGlobalTexParam.TexBPP == 0x4){
						unsigned char tmpChar;
						bool sndPix=false;
						for(int i = 0; i < (mGlobalSettings.mGlobalTexParam.TexSizeX*mGlobalSettings.mGlobalTexParam.TexSizeY); i++){
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
						for(int i = 0; i < (mGlobalSettings.mGlobalTexParam.TexSizeX*mGlobalSettings.mGlobalTexParam.TexSizeY); i++){
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

			if(tbuffer.size() == ((mGlobalSettings.mGlobalTexParam.TexSizeX * mGlobalSettings.mGlobalTexParam.TexSizeY)/mGlobalSettings.mTileBPPSize[mGlobalSettings.mGlobalTexParam.TexBPP])){
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
		if(newSurf && (mGlobalSettings.mGlobalTexParam.TexBPP > 0x2)){
			int retval = importPNG(newSurf,  &mGlobalSettings.mEditor->mPalette);
			if(retval == 1){				
					std::cout << "Error Importing TileSet: " << cTilePath << std::endl;
					return 1;								
			} else {
				for(int i = mTileNum; i < TTiles.size(); i++){
					cNewTiles.push_back(TTiles[i]);
				}
				if(retval == 2){
					std::cout << "TileSet Size Overflow" << std::endl;
					return 2;
				}
			}
	
		} else {
			fs::path cNewPath = cTilePath;			
			int retval = importFile(cNewPath.parent_path().string(), cNewPath.filename().string(), &mGlobalSettings.mEditor->mPalette);
			if(retval == 1){
				std::cout << "Error Importing TileSet: " << cTilePath << std::endl;
				return 1;
			} else {
				for(int i = mTileNum; i < TTiles.size(); i++){
					cNewTiles.push_back(TTiles[i]);
				}
				if(retval == 2){
					std::cout << "TileSet Size Overflow" << std::endl;
					return 2;
				}
			}
		}
	}

	return 0;
}

int TileSet::importPNG(SDL_Surface *newSurf, TPalette* tpal){
	if(newSurf->format->BitsPerPixel == 8){
		if(!(newSurf->w % mGlobalSettings.mGlobalTexParam.TexSizeX) && !(newSurf->h % mGlobalSettings.mGlobalTexParam.TexSizeY)){
			int cTilesX,cTilesY;
			cTilesX = newSurf->w / mGlobalSettings.mGlobalTexParam.TexSizeX;
			cTilesY = newSurf->h / mGlobalSettings.mGlobalTexParam.TexSizeY;
			
			std::cout << "Import PNG: " << cTilesX << "," <<  cTilesY << std::endl;

			std::vector<std::vector<unsigned char>> tbuffers;

			tbuffers.resize(cTilesX*cTilesY);

			for(auto &tbuf : tbuffers){
				tbuf.resize((mGlobalSettings.mGlobalTexParam.TexSizeX*mGlobalSettings.mGlobalTexParam.TexSizeY) / mGlobalSettings.mTileBPPSize[mGlobalSettings.mGlobalTexParam.TexBPP],0);
			}

			//Tile tmpTile;
			int cBufIndex;
			int cPixIndex;

			for(int ii=0; ii < newSurf->h; ii++){
				for(int jj=0; jj < newSurf->w; jj++){
					cBufIndex = mSelection.getTileIndex((ii*newSurf->w)+jj, newSurf->w, newSurf->h, cPixIndex, &mGlobalSettings.mGlobalTexParam);					
					//tmpTile.
					TTexture::setPixel(cPixIndex, ((unsigned char*)(newSurf->pixels))[(ii*newSurf->w)+jj], tbuffers[cBufIndex], &mGlobalSettings.mGlobalTexParam);
				}
			}

			bool bTileSetOverflow = false;

			for(auto &tbufl : tbuffers){
				if(TTiles.size() < mGlobalSettings.TileSetMaxSize){
					createNewFromBuffer(tbufl, tpal);
				} else {
					bTileSetOverflow = true;
				}
			}

			TileAreas.resize(TTiles.size());

			resizeEdit();
			
			SDL_FreeSurface(newSurf);

			if(bTileSetOverflow){
				return 2;
			}

			return 0;
		}
	}
			
	SDL_FreeSurface(newSurf);
	return 1;
}

int TileSet::applyOffset(int poffset, bool bReplaceZero){

	if(mGlobalSettings.mGlobalTexParam.TexBPP > 2){
		for(auto *cTile : TTiles){
			cTile->applyOffset(poffset, bReplaceZero);
		}
		return 0;
	}

	return 1;
}

Tile* TileSet::createNewFromBuffer(std::vector<unsigned char> &newBuf, TPalette* tpal){

	Tile *newTile = new Tile();
	SDL_Rect newRect;

	newTile->loadFromBuffer(newBuf, tpal);
	TileAreas.push_back(newRect);
	TTiles.push_back(newTile);

	if(mGlobalSettings.mGlobalTexParam.TexSizeX == 8){
		mMinTileScale = 10;
	} 

	resizeEdit();
	return newTile;

}

Tile* TileSet::createNewCopy(Tile* cCopyTile, TPalette* tpal){	
	return createNewFromBuffer(cCopyTile->FileData, tpal);
}

Tile* TileSet::createNew(TPalette* tpal){	
	std::vector<unsigned char> tbuf;	
	tbuf.resize(((mGlobalSettings.mGlobalTexParam.TexSizeX * mGlobalSettings.mGlobalTexParam.TexSizeY)/mGlobalSettings.mTileBPPSize[mGlobalSettings.mGlobalTexParam.TexBPP]),0);
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

	if((mTileSizeIn[twidth] != mGlobalSettings.mGlobalTexParam.TexSizeX) || (mTileSizeIn[theight] != mGlobalSettings.mGlobalTexParam.TexSizeY)){
		std::cout << "Wrong Tile Size: " << cTileFile << std::endl;
		return 1;	
	}

	if((magic2 != mGlobalSettings.mGlobalTexParam.TexBPP) ){
		std::cout << "Wrong Tile BPP: " << cTileFile << std::endl;
		return 1;	
	}

	int tmpTileSize = ((mGlobalSettings.mGlobalTexParam.TexSizeX * mGlobalSettings.mGlobalTexParam.TexSizeY)/mGlobalSettings.mTileBPPSize[mGlobalSettings.mGlobalTexParam.TexBPP]);

	tbuffer.erase(tbuffer.begin());
	tbuffer.erase(tbuffer.begin());

	if(tbuffer.size() % tmpTileSize){
		std::cout << "Error in Tile File: " << cTileFile << std::endl;
		return 1;
	}

	ntiles = tbuffer.size() / tmpTileSize;

	Tile *mTile;

	bool bTileSetOverflow = false;

	for(int tCount = 0; tCount < ntiles; tCount++){
		if(TTiles.size() < mGlobalSettings.TileSetMaxSize){
			mTile = new Tile();
			std::vector<unsigned char>::const_iterator first = tbuffer.begin() + (tCount * tmpTileSize);
			std::vector<unsigned char>::const_iterator last = tbuffer.begin() + ((tCount * tmpTileSize) + (tmpTileSize));
			std::vector<unsigned char> tbuffer2(first, last);
			mTile->loadFromBuffer(tbuffer2 ,tpal);
			TTiles.push_back(mTile);
		} else {
			bTileSetOverflow = true;
		}
	}

	TileAreas.resize(TTiles.size());
	resizeEdit();

	if(bTileSetOverflow){
		return 2;
	}

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

	mGlobalSettings.mGlobalTexParam.TexSizeX = mTileSizeIn[twidth];
	mGlobalSettings.mGlobalTexParam.TexSizeY = mTileSizeIn[theight];

	if( (magic2 == 0x8) ||(magic2 == 0x4) ||(magic2 == 0x2) ){
		mGlobalSettings.mGlobalTexParam.TexBPP = magic2;
	} else {
		std::cout << "Error in Tile File: " << cTileFile << std::endl;
		return 1;	
	}

	int tmpTileSize = ((mGlobalSettings.mGlobalTexParam.TexSizeX * mGlobalSettings.mGlobalTexParam.TexSizeY)/mGlobalSettings.mTileBPPSize[mGlobalSettings.mGlobalTexParam.TexBPP]);

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

	magic1 = mTileSizeOut[mGlobalSettings.mGlobalTexParam.TexSizeX];
	magic1 = (magic1 << 4) + mTileSizeOut[mGlobalSettings.mGlobalTexParam.TexSizeY];

	magic2 = mGlobalSettings.mGlobalTexParam.TexBPP;

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
		if( (int)( (float)( ( ( ((mCurTileScale+1)*mGlobalSettings.mGlobalTexParam.TexSizeX )+ mColSpace ) * TTiles.size() )  / ( mCurColumns - 1 ) ) ) < mTileSetBackGround.h ){
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
			for (int yy = 0; yy < mGlobalSettings.mGlobalTexParam.TexSizeY; yy++){
				for (int xx = 0; xx < mGlobalSettings.mGlobalTexParam.TexSizeX; xx++){				
					cPadding.push_back(getXY(yy, xx,  i + isOdd, cRowNum));					
				}				
			}			
		}
	}
	return cPadding;	
}


void TileSet::initoverlay(){
	
	mOverlay.setRects(&TileSetAreas);
	
	int isOdd = TTiles.size() % mSelEdWidth;

	if(isOdd > 0){isOdd = 1;}

	mOverlay.setGrid(mSelEdWidth, (TTiles.size() / mSelEdWidth) + isOdd);

	mOverlay.setSize(mGlobalSettings.mGlobalTexParam.TexSizeX, mGlobalSettings.mGlobalTexParam.TexSizeY);
	
	mOverlay.setScale("1023", 1);

    mOverlay.mRender = [this]{
		mGlobalSettings.mOverlayText.renderNum(mOverlay.mIndex, mOverlay.mX, mOverlay.mY);		
	};
		
}

void TileSet::setoverlay(){
	int isOdd = TTiles.size() % mSelEdWidth;

	if(isOdd > 0){isOdd = 1;}

	mOverlay.setGrid(mSelEdWidth, (TTiles.size() / mSelEdWidth) + isOdd);
}

void TileSet::resizeScale(){
	if(!mGlobalSettings.mProjectSettings.Editor_TileSetMaximized->bvalue){
		bUpdateWinPos = true;
	}
	mCurColumns = 1;
	mCurTileScale = TileSet::MaxTile;
}

void TileSet::resizeEdit(){

	resizeScale();

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

	mSelectionAreaX = mSelEdWidth * mGlobalSettings.mGlobalTexParam.TexSizeX;
	mSelectionAreaY = mGlobalSettings.mGlobalTexParam.TexSizeY * (cRowNum + cPad);
	
	EditPixelAreas.resize(mSelectionAreaX*mSelectionAreaY);

	TileSetAreas.resize(TTiles.size());

	for(int i = 0; i <  EditPixelAreas.size(); i++){
		EditPixelAreas[i].x = rEmpty.x;
		EditPixelAreas[i].y = rEmpty.y;
		EditPixelAreas[i].w = rEmpty.w;
		EditPixelAreas[i].h = rEmpty.h;
	}
	
	mSelection.clearSelection();	
	mSelection.init(mSelectionAreaX , mSelectionAreaY, &mPixelScale, &mPixelScale, &mCurEdScale);
	setoverlay();
}

int TileSet::getXY(int xpos, int ypos, int cxpos, int cypos){
	int index;
	int lineL = mSelectionAreaX; 
	int fullL = lineL * mGlobalSettings.mGlobalTexParam.TexSizeY;
	index = xpos + (ypos * lineL) + (cxpos * mGlobalSettings.mGlobalTexParam.TexSizeX) + (cypos * fullL);		
	return index;
}

void TileSet::updateEditAreas(std::vector<SDL_Rect> &cTile, int xpos, int ypos){
	for(int i = 0; i < mGlobalSettings.mGlobalTexParam.TexSizeY; i++){
		for(int j = 0; j < mGlobalSettings.mGlobalTexParam.TexSizeX; j++){
			EditPixelAreas[getXY(j,i, xpos, ypos)] = cTile[(i*mGlobalSettings.mGlobalTexParam.TexSizeX)+j];
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

		mSelection.resize(mSelectionAreaX, mSelectionAreaY, &mPixelScale, &mPixelScale, &mCurEdScale);

	} 

	int isOdd = TTiles.size() % mSelEdWidth;
	int cRowNum = TTiles.size() / mSelEdWidth;
	
	SDL_Rect cBorder;
		for(int i = 0; i < mSelEdWidth; i++){
			for(int j = 0; j <  cRowNum; j++){
				int cxpos = xpos +  (mCurEdScale*mGlobalSettings.mGlobalTexParam.TexSizeX)*i;
				int cypos = ypos + (mGlobalSettings.mGlobalTexParam.TexSizeY*mCurEdScale)*j;

				for(int ii=0; ii < mGlobalSettings.mGlobalTexParam.TexSizeY; ii++){
					for(int jj=0; jj < mGlobalSettings.mGlobalTexParam.TexSizeX; jj++){
						EditPixelAreas[getXY(jj,ii, i, j)] = mGlobalSettings.mEditor->mPalette.renderTileEd(cxpos + (mCurEdScale)*jj, cypos + (mCurEdScale)*ii, TTiles[(j*mSelEdWidth)+i]->getPixel(jj+(ii*mGlobalSettings.mGlobalTexParam.TexSizeX)), mCurEdScale, &mGlobalSettings.mGlobalTexParam); 			//mGlobalSettings.TilePixelSize * 
					}
				}
			
				cBorder.x = xpos + ((mCurEdScale*mGlobalSettings.mGlobalTexParam.TexSizeX)*i);
				cBorder.y = ypos + ((mGlobalSettings.mGlobalTexParam.TexSizeY*mCurEdScale)*j);
				cBorder.w = (mCurEdScale*mGlobalSettings.mGlobalTexParam.TexSizeX);
				cBorder.h = (mCurEdScale*mGlobalSettings.mGlobalTexParam.TexSizeY);

				TileSetAreas[(j*mSelEdWidth)+i] = cBorder;

				if(mGlobalSettings.bShowTileGrid){									
					Tile::renderSelection(cBorder, mGlobalSettings.DefaultHighlightColor);
				}				
			}								
		}	
		if(isOdd){			
			int i = mSelEdWidth;
			for(int j = 0; j < isOdd; j++){			
				int cxpos = xpos +  (mCurEdScale*mGlobalSettings.mGlobalTexParam.TexSizeX)*j;
				int cypos = ypos + (mGlobalSettings.mGlobalTexParam.TexSizeY*mCurEdScale)*cRowNum;

				for(int ii=0; ii < mGlobalSettings.mGlobalTexParam.TexSizeY; ii++){
					for(int jj=0; jj < mGlobalSettings.mGlobalTexParam.TexSizeX; jj++){
						EditPixelAreas[getXY(jj,ii, j, cRowNum)] = mGlobalSettings.mEditor->mPalette.renderTileEd(cxpos + (mCurEdScale)*jj, cypos + (mCurEdScale)*ii, TTiles[(i*cRowNum)+j]->getPixel(jj+(ii*mGlobalSettings.mGlobalTexParam.TexSizeX)), mCurEdScale, &mGlobalSettings.mGlobalTexParam); 			//mGlobalSettings.TilePixelSize * 
					}
				}

				cBorder.x = xpos + ((mCurEdScale*mGlobalSettings.mGlobalTexParam.TexSizeX)*j);
				cBorder.y = ypos + ((mGlobalSettings.mGlobalTexParam.TexSizeY*mCurEdScale)*cRowNum);
				cBorder.w = (mCurEdScale*mGlobalSettings.mGlobalTexParam.TexSizeX);
				cBorder.h = (mCurEdScale*mGlobalSettings.mGlobalTexParam.TexSizeY);

				TileSetAreas[(i*cRowNum)+j] = cBorder;

				if(mGlobalSettings.bShowTileGrid){									
					Tile::renderSelection(cBorder, mGlobalSettings.DefaultHighlightColor);
				}	

				/*
				if(mGlobalSettings.bShowTileGrid){	
					cBorder.x = xpos + ((mCurEdScale*mGlobalSettings.mGlobalTexParam.TexSizeX)*j);
					cBorder.y = ypos + ((mGlobalSettings.mGlobalTexParam.TexSizeY*mCurEdScale)*cRowNum);
					cBorder.w = (mCurEdScale*mGlobalSettings.mGlobalTexParam.TexSizeX);
					cBorder.h = (mCurEdScale*mGlobalSettings.mGlobalTexParam.TexSizeY);
					Tile::renderSelection(cBorder, mGlobalSettings.DefaultHighlightColor);
				}*/
			}
		}

		if(mGlobalSettings.mEditor->mCurrentBrushPixelTileSet){// && !mGlobalSettings.CurrentEditor->mBrushesPixel.bIsEditing){
			if(mGlobalSettings.mEditor->mCurrentBrushPixelTileSet->mSelected.size()){
				for(int i = 0; i < mSelEdWidth; i++){
					for(int j = 0; j <  cRowNum; j++){
						int cxpos = xpos +  (mCurEdScale*mGlobalSettings.mGlobalTexParam.TexSizeX)*i;
						int cypos = ypos + (mGlobalSettings.mGlobalTexParam.TexSizeY*mCurEdScale)*j;

						for(int ii=0; ii < mGlobalSettings.mGlobalTexParam.TexSizeY; ii++){
							for(int jj=0; jj < mGlobalSettings.mGlobalTexParam.TexSizeX; jj++){
								if(mGlobalSettings.mEditor->mCurrentBrushPixelTileSet->findInSelection(getXY(jj,ii, i, j)) != -1){
									int findex = mGlobalSettings.mEditor->mCurrentBrushPixelTileSet->findInSelection(getXY(jj,ii, i, j));
									if(mGlobalSettings.mEditor->mCurrentBrushPixelTileSet->mBrushElements[findex] != -1){
										mGlobalSettings.mEditor->mPalette.renderTileEd(cxpos + (mCurEdScale)*jj, cypos + (mCurEdScale)*ii, mGlobalSettings.mEditor->mCurrentBrushPixelTileSet->mBrushElements[findex], mCurEdScale, &mGlobalSettings.mGlobalTexParam); 
									}
								}
							}					
						}								
					}	
				}
		
				if(isOdd){			
					int i = mSelEdWidth;
					for(int j = 0; j < isOdd; j++){			
						int cxpos = xpos +  (mCurEdScale*mGlobalSettings.mGlobalTexParam.TexSizeX)*j;
						int cypos = ypos + (mGlobalSettings.mGlobalTexParam.TexSizeY*mCurEdScale)*cRowNum;

						for(int ii=0; ii < mGlobalSettings.mGlobalTexParam.TexSizeY; ii++){
							for(int jj=0; jj < mGlobalSettings.mGlobalTexParam.TexSizeX; jj++){
								if(mGlobalSettings.mEditor->mCurrentBrushPixelTileSet->findInSelection(getXY(jj,ii, j, cRowNum)) != -1){
									int findex = mGlobalSettings.mEditor->mCurrentBrushPixelTileSet->findInSelection(getXY(jj,ii, j, cRowNum));
									if(mGlobalSettings.mEditor->mCurrentBrushPixelTileSet->mBrushElements[findex] != -1){
										mGlobalSettings.mEditor->mPalette.renderTileEd(cxpos + (mCurEdScale)*jj, cypos + (mCurEdScale)*ii, mGlobalSettings.mEditor->mCurrentBrushPixelTileSet->mBrushElements[findex], mCurEdScale, &mGlobalSettings.mGlobalTexParam); 
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

	if(bRenderOverlay){		
		mOverlay.render();
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

int TileSet::renderImMax(int ypos){

	ImVec2 cOverSize = ImGui::CalcTextSize("1023");

	mOverlayScaleX = cOverSize.x;
	mOverlayScaleY = cOverSize.y - (cOverSize.y / 4);

	ImVec2 cWinPos;	
	ImVec2 cWinSize;	
	
	if(bUpdateWinPos){
		mTileSetBackGround.x = 10;
		mTileSetBackGround.y = ypos + 10;
		mTileSetBackGround.w = mGlobalSettings.WindowWidth - 20;
		mTileSetBackGround.h = mGlobalSettings.WindowHeight - mGlobalSettings.TopBarHeight - 20;

		cWinPos.x = mTileSetBackGround.x;
		cWinPos.y = mTileSetBackGround.y;

		cWinSize.x = mTileSetBackGround.w;
		cWinSize.y = mTileSetBackGround.h;


		ImGui::SetNextWindowPos(cWinPos, ImGuiCond_Always);
		ImGui::SetNextWindowSize(cWinSize, ImGuiCond_Always);
		bUpdateWinPos = false;
	}

	if(bUpdateWinPosMax){
		int cOldWidth = mTileSetBackGround.w;
		
		if(cOldWidth > (mGlobalSettings.WindowWidth - 20)){
			mTileSetBackGround.x = 10;
			mTileSetBackGround.y = ypos + 10;
			mTileSetBackGround.w = mGlobalSettings.WindowWidth - 20;
			mTileSetBackGround.h = mGlobalSettings.WindowHeight - mGlobalSettings.TopBarHeight - 20;

			cWinPos.x = mTileSetBackGround.x;
			cWinPos.y = mTileSetBackGround.y;

			cWinSize.x = mTileSetBackGround.w;
			cWinSize.y = mTileSetBackGround.h;


			ImGui::SetNextWindowPos(cWinPos, ImGuiCond_Always);
			ImGui::SetNextWindowSize(cWinSize, ImGuiCond_Always);
			
			bUpdateWinPosMax = false;
		} else {

			mTileSetBackGround.x = mGlobalSettings.WindowWidth - cOldWidth - 10;
			mTileSetBackGround.y = ypos + 10;
			mTileSetBackGround.w = cOldWidth;
			mTileSetBackGround.h = mGlobalSettings.WindowHeight - mGlobalSettings.TopBarHeight - 20;

			cWinPos.x = mTileSetBackGround.x;
			cWinPos.y = mTileSetBackGround.y;

			cWinSize.x = mTileSetBackGround.w;
			cWinSize.y = mTileSetBackGround.h;

			ImGui::SetNextWindowPos(cWinPos, ImGuiCond_Always);
			ImGui::SetNextWindowSize(cWinSize, ImGuiCond_Always);
			
			bUpdateWinPosMax = false;
		}

	}

	ImGui::Begin("TileSet", NULL, ImGuiWindowFlags_NoNav);

	cWinPos = ImGui::GetWindowPos();
	cWinSize = ImGui::GetWindowSize();

	mTileSetBackGround.x = cWinPos.x;
	mTileSetBackGround.y = cWinPos.y;
	mTileSetBackGround.w = cWinSize.x;
	mTileSetBackGround.h = cWinSize.y;

	if(ImGui::Button("Move Up")){
		mGlobalSettings.mEditor->moveTileUp();
	}

	ImGui::SameLine();

	if(ImGui::Button("Move Down")){
		mGlobalSettings.mEditor->moveTileDown();
	}

	ImGui::BeginChild("TTiles", ImVec2(0,0), false, ImGuiWindowFlags_NoNav);

	ImVec2 cChildPos = ImGui::GetWindowPos();
	mChildTop = cChildPos.y;

	bool bIsDragged = false;
	int mDragged = -1;
	int mDragSource = 0;
	int mDragTarget = 0;

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(mGlobalSettings.DefaultBGColor.r,mGlobalSettings.DefaultBGColor.g,mGlobalSettings.DefaultBGColor.b)));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(mGlobalSettings.DefaultBGColor.r,mGlobalSettings.DefaultBGColor.g,mGlobalSettings.DefaultBGColor.b)));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(mGlobalSettings.DefaultBGColor.r,mGlobalSettings.DefaultBGColor.g,mGlobalSettings.DefaultBGColor.b)));
	
	ImVec2 cpos = ImGui::GetCursorPos();

	cpos.x += 5;
	cpos.y += 5;

	ImGui::SetCursorPos(cpos);

	bool bCurserUpdate = false;

	for(int ti = 0; ti < TTiles.size(); ti++){

		if(bCurserUpdate){
			cpos = ImGui::GetCursorPos();

			cpos.x += 5;
			cpos.y += 5;

			ImGui::SetCursorPos(cpos);

			bCurserUpdate = false;
		}

		TileAreas[ti] = TTiles[ti]->renderIm(0,0, ti, mDragged, mMaxTileScale,true,true);								
		
		if((mDragged > -1) && !bIsDragged){
				bIsDragged = true;
				mDragSource = mDragged;
				mDragTarget = ti;
		}

		if((TileAreas[ti].x + TileAreas[ti].w) < (mTileSetBackGround.x + mTileSetBackGround.w - TileAreas[ti].w - 5)){
			ImGui::SameLine();
		} else {
			bCurserUpdate = true;			
		}
	}
	
	ImGui::PopStyleColor(3);

	if(bIsDragged){		
		mGlobalSettings.mEditor->swapTiles(mDragSource, mDragTarget, true);
	}

	mGlobalSettings.mEditor->ImButtonsTileSet.updateButtonStates();

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::EndChild();
	
    ImGui::End();


	int cMax = (int)( (float)( ( ( (mCurTileScale*mGlobalSettings.mGlobalTexParam.TexSizeY ) +mColSpace ) * TTiles.size() )  / mCurColumns )) + (4 * mGlobalSettings.mGlobalTexParam.TexSizeY);
	if((cMax - mTileSetBackGround.h) > 0 ){
		mMaxScrollY = -(cMax - mTileSetBackGround.h);
	} else {
		mMaxScrollY = 0;
	}

	if(bCheckMaxSize){		
		if((mTileSetBackGround.w > (mGlobalSettings.WindowWidth - 20)) || (mTileSetBackGround.h > (mGlobalSettings.WindowHeight - 20))){
			bUpdateWinPosMax = true;		
		}						
		bCheckMaxSize = false;
	}

	return 0;

}

int TileSet::renderIm(int ypos){
	
	mTileSetBackGround.h = mGlobalSettings.WindowHeight- mGlobalSettings.TopBarHeight;
	
	if(mCurColumns < mMaxColumns){
		while( (int)( (float)( ( ( (mCurTileScale*mGlobalSettings.mGlobalTexParam.TexSizeY ) +mColSpace ) * TTiles.size() )  / mCurColumns ) ) > (mTileSetBackGround.h - mChildTop)){	
			mCurTileScale--;
			bUpdateWinPos = true;

			if(mCurTileScale == 0){
				mCurTileScale = 1;
			}

			if(mCurTileScale < mMinTileScale){
				if(mCurColumns < mMaxColumns){
					mCurColumns++;					
				} else {
					mCurTileScale++;
					break;
				}				
			}
		}
	}

	mGlobalSettings.TileSetWidth = (((mCurTileScale*mGlobalSettings.mGlobalTexParam.TexSizeX)+mColSpace)*mCurColumns)+(mColSpace*3);

	int isOdd = TTiles.size() % mCurColumns;
	int cRowNum = TTiles.size() / mCurColumns;

	mTileSetBackGround.x = mGlobalSettings.WindowWidth - mGlobalSettings.TileSetWidth;
	mTileSetBackGround.y = ypos;
	mTileSetBackGround.w = mGlobalSettings.TileSetWidth; 

	ImVec2 cOverSize = ImGui::CalcTextSize("1023");

	mOverlayScaleX = cOverSize.x;
	mOverlayScaleY = cOverSize.y - (cOverSize.y / 4);

	ImVec2 cWinPos;
	cWinPos.x = mTileSetBackGround.x;
	cWinPos.y = mTileSetBackGround.y;
	

	ImVec2 cWinSize;
	cWinSize.x = mTileSetBackGround.w;
	cWinSize.y = mTileSetBackGround.h;

	if(bUpdateWinPos){
		ImGui::SetNextWindowPos(cWinPos, ImGuiCond_Always);
		ImGui::SetNextWindowSize(cWinSize, ImGuiCond_Always);
		bUpdateWinPos = false;
	} else {
		ImGui::SetNextWindowPos(cWinPos, ImGuiCond_Once);
		ImGui::SetNextWindowSize(cWinSize, ImGuiCond_Once);
	}

	ImGui::Begin("TileSet", NULL, ImGuiWindowFlags_NoNav);    

	if(ImGui::Button("Move Up")){
		mGlobalSettings.mEditor->moveTileUp();
	}

	if(ImGui::Button("Move Down")){
		mGlobalSettings.mEditor->moveTileDown();
	}

	ImGui::BeginChild("TTiles", ImVec2(0,0), false, ImGuiWindowFlags_NoNav);

	ImVec2 cChildPos = ImGui::GetWindowPos();
	mChildTop = cChildPos.y;

	bool bIsDragged = false;
	int mDragged = -1;
	int mDragSource = 0;
	int mDragTarget = 0;

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(mGlobalSettings.DefaultBGColor.r,mGlobalSettings.DefaultBGColor.g,mGlobalSettings.DefaultBGColor.b)));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(mGlobalSettings.DefaultBGColor.r,mGlobalSettings.DefaultBGColor.g,mGlobalSettings.DefaultBGColor.b)));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(mGlobalSettings.DefaultBGColor.r,mGlobalSettings.DefaultBGColor.g,mGlobalSettings.DefaultBGColor.b)));
	
	

	if(mCurColumns > 0){
		for(int i = 0; i < cRowNum; i++){

			ImVec2 cpos = ImGui::GetCursorPos();

			cpos.x += 5;
			cpos.y += 5;

			ImGui::SetCursorPos(cpos);

			for(int j = 0; j < mCurColumns; j++){
				TileAreas[(i * mCurColumns) + j] = TTiles[(i*mCurColumns) + j]->renderIm((mTileSetBackGround.x+ (mColSpace*2) +  ((mCurTileScale*mGlobalSettings.mGlobalTexParam.TexSizeX)+mColSpace)*j),mTileSetBackGround.y  + (mColSpace*2) + (((mGlobalSettings.mGlobalTexParam.TexSizeY*mCurTileScale)+mColSpace)*i), (i*mCurColumns) + j, mDragged, mCurTileScale,true,true);								
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

			ImVec2 cpos = ImGui::GetCursorPos();

			cpos.x += 5;
			cpos.y += 5;

			ImGui::SetCursorPos(cpos);

			int i = mCurColumns;
			for(int j = 0; j < isOdd; j++){
				TileAreas[(i * cRowNum) + j] = TTiles[(i*cRowNum)+j]->renderIm((mTileSetBackGround.x+ (mColSpace*2) +  ((mCurTileScale*mGlobalSettings.mGlobalTexParam.TexSizeX)+mColSpace)*j),mTileSetBackGround.y  + (mColSpace*2) + (((mGlobalSettings.mGlobalTexParam.TexSizeY*mCurTileScale)+mColSpace)*cRowNum), (i*cRowNum)+j, mDragged,  mCurTileScale,true,true);				
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
		mGlobalSettings.mEditor->swapTiles(mDragSource, mDragTarget, true);
	}

	mGlobalSettings.mEditor->ImButtonsTileSet.updateButtonStates();

	ImGui::Spacing();

	ImGui::EndChild();
	
    ImGui::End();


	int cMax = (int)( (float)( ( ( (mCurTileScale*mGlobalSettings.mGlobalTexParam.TexSizeY ) +mColSpace ) * TTiles.size() )  / mCurColumns )) + (4 * mGlobalSettings.mGlobalTexParam.TexSizeY);
	if((cMax - mTileSetBackGround.h) > 0 ){
		mMaxScrollY = -(cMax - mTileSetBackGround.h);
	} else {
		mMaxScrollY = 0;
	}

return 0;
}
void TileMap::initoverlay(){
	mOverlay.setRects(&TileAreas);
	mOverlay.setGrid(TileMapWidth, TileMapHeight);
	mOverlay.setSize(mGlobalSettings.mGlobalTexParam.TexSizeX, mGlobalSettings.mGlobalTexParam.TexSizeY);
	mOverlay.setScale("1023", 2);

	if(mGlobalSettings.mGlobalTexParam.TexBPP < 8){

		mOverlay.mRender = [this]{
			mGlobalSettings.mOverlayText.renderNum(getTile(mOverlay.mIndex) , mOverlay.mX , mOverlay.mY);
						
			mGlobalSettings.mOverlayText.renderNum(getOffset(mOverlay.mIndex) , mOverlay.mX , mOverlay.mY + mGlobalSettings.mOverlayText.mLastHeight);
			
			unsigned char cflip = getFlip(mOverlay.mIndex);
			
			if(cflip > 0){
				switch (cflip){
					case 1:
						mGlobalSettings.mOverlayText.renderText("X" , mOverlay.mX + mGlobalSettings.mOverlayText.mLastWidth, mOverlay.mY + mGlobalSettings.mOverlayText.mLastHeight);		
						break;				
					case 2:
						mGlobalSettings.mOverlayText.renderText("Y" , mOverlay.mX + mGlobalSettings.mOverlayText.mLastWidth , mOverlay.mY + mGlobalSettings.mOverlayText.mLastHeight);		
						break;				
					case 3:
						mGlobalSettings.mOverlayText.renderText("XY" , mOverlay.mX + mGlobalSettings.mOverlayText.mLastWidth , mOverlay.mY + mGlobalSettings.mOverlayText.mLastHeight);												
						break;				
					default:
						break;
					}
				}			
		};		


	} else {

		mOverlay.mRender = [this]{
			mGlobalSettings.mOverlayText.renderNum(getTile(mOverlay.mIndex) , mOverlay.mX , mOverlay.mY);
			unsigned char cflip = getFlip(mOverlay.mIndex);
			
			if(cflip > 0){
				switch (cflip){
					case 1:
						mGlobalSettings.mOverlayText.renderText("X" , mOverlay.mX , mOverlay.mY + mGlobalSettings.mOverlayText.mLastHeight);		
						break;				
					case 2:
						mGlobalSettings.mOverlayText.renderText("Y" , mOverlay.mX , mOverlay.mY + mGlobalSettings.mOverlayText.mLastHeight);		
						break;				
					case 3:
						mGlobalSettings.mOverlayText.renderText("XY" , mOverlay.mX , mOverlay.mY + mGlobalSettings.mOverlayText.mLastHeight);												
						break;				
					default:
						break;
					}
				}			
		};

	}
}

void TileMap::init(){
	mSelection.init(TileMapWidth, TileMapHeight, &mGlobalSettings.mGlobalTexParam.TexSizeX, &mGlobalSettings.mGlobalTexParam.TexSizeY, &mGlobalSettings.TileMapScale);
	initoverlay();
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

int TileMap::createCollisionMap(int cFormat){
	if(bHasCollisionMap){
		return 1;
	}
	
	bHasCollisionMap = true;
	mColMap.createNew(this, cFormat);

	return 0;
}
int TileMap::removeCollisionMap(){
	if(bHasCollisionMap){
		bHasCollisionMap = false;
		bRenderOverlayColMap = false;
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

int TileMap::moveTile(int cSource, int cTarget){
	if(cSource == cTarget){
		return 1;
	}

	for(int i = 0; i < (mGlobalSettings.TileMapHeight*mGlobalSettings.TileMapWidth); i++){
			
		int cTile = getTile(i);

		if(cSource < cTarget){

			if(cTile == cSource){
				setTile(i, cTarget);				
			} else {
				if((cTile <= cTarget) && (cTile > cSource)){
					setTile(i, cTile - 1);
				}
			}

		} else {
			if(cTile == cSource){
				setTile(i, cTarget);				
			} else {
				if((cTile >= cTarget) && (cTile < cSource)){
					setTile(i, cTile + 1);
				}
			}

		}		
	}	

	if(bHasCollisionMap){
		mColMap.moveTile(cSource, cTarget);
	}

	return 0;
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
			TileAreas[j+(i*mGlobalSettings.TileMapWidth)] = mTiles->TTiles[tIndex]->render(xpos + (mGlobalSettings.mGlobalTexParam.TexSizeX * j * mGlobalSettings.TileMapScale), ypos + (mGlobalSettings.mGlobalTexParam.TexSizeY * i * mGlobalSettings.TileMapScale), mGlobalSettings.TileMapScale, getTileProp(j+(i*mGlobalSettings.TileMapWidth)));			
			if(mGlobalSettings.bShowSelectedTile){
				if((mGlobalSettings.mSelectedTile == (j+(i*mGlobalSettings.TileMapWidth)))){
					SDL_SetRenderDrawColor(mGlobalSettings.TRenderer,mGlobalSettings.DefaultHighlightColor.r,mGlobalSettings.DefaultHighlightColor.g,mGlobalSettings.DefaultHighlightColor.b, 0xff);
					SDL_RenderDrawRect(mGlobalSettings.TRenderer, &TileAreas[j+(i*mGlobalSettings.TileMapWidth)]);
				}
			}
		}
	}
	
	if(mGlobalSettings.mEditor->mCurrentBrushTile){ // && !mGlobalSettings.CurrentEditor->mBrushesTile.bIsEditing){
		if(mGlobalSettings.mEditor->mCurrentBrushTile->mSelected.size()){
			for(int i=0; i < mGlobalSettings.TileMapHeight; i++){
				for(int j=0; j < mGlobalSettings.TileMapWidth; j++){					
					if(mGlobalSettings.mEditor->mCurrentBrushTile->isInSelection((j+(i*mGlobalSettings.TileMapWidth)))){						
						int findex = mGlobalSettings.mEditor->mCurrentBrushTile->findInSelection((j+(i*mGlobalSettings.TileMapWidth)));
						if(mGlobalSettings.mEditor->mCurrentBrushTile->mBrushElements[findex] != -1){
							mTiles->TTiles[mGlobalSettings.mEditor->mCurrentBrushTile->mBrushElements[findex]]->render(xpos + (mGlobalSettings.mGlobalTexParam.TexSizeX * j * mGlobalSettings.TileMapScale), ypos + (mGlobalSettings.mGlobalTexParam.TexSizeY * i * mGlobalSettings.TileMapScale), mGlobalSettings.TileMapScale, mGlobalSettings.mEditor->mCurrentBrushTile->getElementProps(findex));			
						}						
					}
				}
			}
		}
	}


	mBorder.x = TileAreas[0].x;
	mBorder.y = TileAreas[0].y;
	mBorder.w = mGlobalSettings.TileMapWidth * mGlobalSettings.mGlobalTexParam.TexSizeX * mGlobalSettings.TileMapScale;
	mBorder.h = mGlobalSettings.TileMapHeight * mGlobalSettings.mGlobalTexParam.TexSizeY * mGlobalSettings.TileMapScale;

	SDL_SetRenderDrawColor(mGlobalSettings.TRenderer,mGlobalSettings.DefaultGUIBorderColor.r,mGlobalSettings.DefaultGUIBorderColor.g,mGlobalSettings.DefaultGUIBorderColor.b, 0xff);
	SDL_RenderDrawRect(mGlobalSettings.TRenderer, &mBorder);

	if(mGlobalSettings.mEditor->mTileMap->mSelection.bHasSelection){
		mGlobalSettings.mEditor->mTileMap->mSelection.renderSelection(xpos, ypos);
	}

	if(bRenderOverlay){
		mOverlay.render();
	}

	if(bHasCollisionMap){
		if(bRenderOverlayColMap){
			mColMap.mOverlay.render();
		}
	}



	return 0;
}
