#include "TTFTexture.h"
#include "TSettings.h"

extern TSettings mGlobalSettings;


int TTFTexture::loadTTFFromUTF8(std::string cTexText, SDL_Color textColor, TTF_Font *cFont){
	if(TTFTex){
		SDL_DestroyTexture(TTFTex);
		TTFTex = NULL;
	}
	
	if(!cTexText.size()){
		return 1;	
	}

	
	mTextColor = textColor;

	mTexText = cTexText;

	SDL_Surface* textSurf =  TTF_RenderUTF8_Solid(cFont, cTexText.c_str(), textColor);
	if( textSurf != NULL ){
        	TTFTex = SDL_CreateTextureFromSurface(mGlobalSettings.TRenderer, textSurf);
		if(TTFTex == NULL){
			std::cout << "SDL Error: " << SDL_GetError() << std::endl;
			return 1;
		} else {
			mTexWidth = textSurf->w;
			mTexHeight = textSurf->h;
			//std::cout << "(" << mTexWidth << "," << mTexHeight << ")" << std::endl;
		}
		SDL_FreeSurface(textSurf);
	}
	else{
		std::cout << "SDL_ttf Error: " << TTF_GetError() << std::endl;
		return 1;
	}

	return 0;
}

int TTFTexture::loadTTFFromUTF8(std::string cTexText, SDL_Color textColor){
	return loadTTFFromUTF8(cTexText, textColor, mGlobalSettings.TFont);
}

int TTFTexture::loadTTFFromString(std::string cTexText, SDL_Color textColor)
{
	if(TTFTex){
		SDL_DestroyTexture(TTFTex);
		TTFTex = NULL;
	}
	
	if(!cTexText.size()){
		return 1;	
	}

	
	mTexText = cTexText;

	SDL_Surface* textSurf = TTF_RenderText_Solid(mGlobalSettings.TFont, cTexText.c_str(), textColor);
	if( textSurf != NULL ){
        	TTFTex = SDL_CreateTextureFromSurface(mGlobalSettings.TRenderer, textSurf);
		if(TTFTex == NULL){
			std::cout << "SDL Error: " << SDL_GetError() << std::endl;
			return 1;
		} else {
			mTexWidth = textSurf->w;
			mTexHeight = textSurf->h;
		}
		SDL_FreeSurface(textSurf);
	}
	else{
		std::cout << "SDL_ttf Error: " << TTF_GetError() << std::endl;
		return 1;
	}

	return 0;
}

SDL_Rect TTFTexture::render(int xpos, int ypos){
	SDL_Rect renderQuad = {xpos, ypos, mTexWidth, mTexHeight};
	if(TTFTex) SDL_RenderCopy( mGlobalSettings.TRenderer, TTFTex, NULL, &renderQuad);
	return renderQuad;
}

