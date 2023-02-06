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

		std::string mWindow = "\uf2d0";
		std::string mFloppy = "\ue240"; //Floppy disk		
		std::string mPrompt = "\uf120"; //Prompt
		//std::string mPrompt = "\ue7a2"; //TEST prompt good		
		std::string mFile = "\uf15c"; // File Icon
//		std::string mBook = "\uf02d"; //lying book
//		std::string mBook = "\ufd2c"; //ring binder
		std::string mBook = "\uf405"; //Open Book SEL		
		//std::string mBook = "\uf129"; Info 1		
		std::string mInfo = "\uf449"; // Info circle		
//		std::string mBook = "\uf7d5"  // ? mark
//		std::string mBook = "\ufc89"; // ? mark box
};	


#endif