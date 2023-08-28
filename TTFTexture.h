#ifndef __TTFTEXTURE__
#define __TTFTEXTURE__

#include <SDL.h> 
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <set>
#include <map>
#include <filesystem>
#include <sstream>
#include <cstdlib>

class TTFTexture{
	public:
		SDL_Texture *TTFTex=NULL;
		std::string mTexText;
		int mTexWidth = 0;
		int mTexHeight = 0;
		SDL_Color mTextColor;
		int loadTTFFromString(std::string cTexText, SDL_Color textColor);
		int loadTTFFromUTF8(std::string cTexText, SDL_Color textColor);		
		int loadTTFFromUTF8(std::string cTexText, SDL_Color textColor, TTF_Font *cFont);		
		SDL_Rect render(int xpos,int ypos);
};	


#endif
