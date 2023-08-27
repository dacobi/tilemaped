#include <stdio.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>

#define VRAM_offset 0xA800
#define CARS_size 5632
#define BOOM_size 4096
#define CDTXT_size 10240
#define TRTXT_size 4608

int main(int argc, char *argv[]){


	if( (argc < 2) || (argc > 6) ){
	
		std::cout << "Usage: " << std::string(argv[0]) << " <tiles size track1> [ <tiles size track2> ... <tiles size track5> ]" << std::endl;
		std::cout << "Where <tiles size #> is the size of the main track tiles file in bytes in decimal, including the 2 byte header" << 	std::endl;
		return 0;
	
	}


	std::ofstream outfile("game.bin", std::ios::binary );

	std::vector<unsigned char> obuffer;
		
	char mLevels = (argc - 1);

	obuffer.push_back(mLevels);
	
	std::cout <<  std::hex << std::endl;
	
	for(int ti = 0; ti < mLevels; ti++){
	
		std::stringstream conv;

		conv << std::string(argv[ti+1]) << std::endl;

		unsigned int tSize;

		conv >> tSize;
		tSize -= 2;

		std::cout << "Tiles Size Track" << (ti+1) << " Is : " << tSize << std::endl << std::endl;

		//Car Sprite		
		unsigned int mCarsAddr = VRAM_offset + tSize;

		std::cout << "Car Addr: " << mCarsAddr << std::endl;
		
		unsigned char mCarLo,mCarHi;
		
		mCarLo = (unsigned char)(mCarsAddr & 0x00ff);
		mCarHi = (unsigned char)((mCarsAddr >> 8) & 0x00ff);
	
		std::cout << "Car Addr Hi: " << (int)(mCarHi) << std::endl;
		std::cout << "Car Addr Lo: " << (int)(mCarLo) << std::endl;		
	
		obuffer.push_back(mCarHi);
		obuffer.push_back(mCarLo);		
		
		mCarsAddr = mCarsAddr >> 5;
		
		mCarLo = (unsigned char)(mCarsAddr & 0x00ff);
		mCarHi = (unsigned char)((mCarsAddr >> 8) & 0x00ff);

		std::cout << "Car Short Hi: " << (int)(mCarHi) << std::endl;
		std::cout << "Car Short Lo: " << (int)(mCarLo) << std::endl << std::endl;		

	
		obuffer.push_back(mCarHi);
		obuffer.push_back(mCarLo);		

		//Boom Sprite		
		unsigned int mBoomAddr = VRAM_offset + tSize + CARS_size;

		std::cout << "Boom Addr: " << mBoomAddr << std::endl;
		
		unsigned char mBoomLo,mBoomHi;
		
		mBoomLo = (unsigned char)(mBoomAddr & 0x00ff);
		mBoomHi = (unsigned char)((mBoomAddr >> 8) & 0x00ff);
	
		std::cout << "Boom Addr Hi: " << (int)(mBoomHi) << std::endl;
		std::cout << "Boom Addr Lo: " << (int)(mBoomLo) << std::endl;		
	
		obuffer.push_back(mBoomHi);
		obuffer.push_back(mBoomLo);		
		
		mBoomAddr = mBoomAddr >> 5;
		
		mBoomLo = (unsigned char)(mBoomAddr & 0x00ff);
		mBoomHi = (unsigned char)((mBoomAddr >> 8) & 0x00ff);

		std::cout << "Boom Short Hi: " << (int)(mBoomHi) << std::endl;
		std::cout << "Boom Short Lo: " << (int)(mBoomLo) << std::endl << std::endl;		

	
		obuffer.push_back(mBoomHi);
		obuffer.push_back(mBoomLo);		

		
		//CD TXT Sprite	
		unsigned int mCDAddr = VRAM_offset + tSize + CARS_size + BOOM_size;
		
		unsigned char mCDLo,mCDHi;
		
		std::cout << "TXT CD Addr: " << mCDAddr << std::endl;
		
		mCDLo = (unsigned char)(mCDAddr & 0x00ff);
		mCDHi = (unsigned char)((mCDAddr >> 8) & 0x00ff);
	
		std::cout << "TXT CD Addr Hi: " << (int)(mCDHi) << std::endl;
		std::cout << "TXT CD Addr Lo: " << (int)(mCDLo) << std::endl;		

	
		obuffer.push_back(mCDHi);
		obuffer.push_back(mCDLo);		
		
		mCDAddr = mCDAddr >> 5;
		
		mCDLo = (unsigned char)(mCDAddr & 0x00ff);
		mCDHi = (unsigned char)((mCDAddr >> 8) & 0x00ff);
		
		std::cout << "TXT CD Short Hi: " << (int)(mCDHi) << std::endl;
		std::cout << "TXT CD Short Lo: " << (int)(mCDLo) << std::endl << std::endl;		

	
		obuffer.push_back(mCDHi);
		obuffer.push_back(mCDLo);		

		//LAPS TXT Sprite	
		unsigned int mLPAddr = VRAM_offset + tSize + CARS_size + BOOM_size + CDTXT_size;
		
		std::cout << "TXT Laps Addr: " << mLPAddr << std::endl;
		
		unsigned char mLPLo,mLPHi;
		
		mLPLo = (unsigned char)(mLPAddr & 0x00ff);
		mLPHi = (unsigned char)((mLPAddr >> 8) & 0x00ff);
		
		std::cout << "TXT Laps Addr Hi: " << (int)(mLPHi) << std::endl;
		std::cout << "TXT Laps Addr Lo: " << (int)(mLPLo) << std::endl;		

	
		obuffer.push_back(mLPHi);
		obuffer.push_back(mLPLo);		
		
		mLPAddr = mLPAddr >> 5;
		
		mLPLo = (unsigned char)(mLPAddr & 0x00ff);
		mLPHi = (unsigned char)((mLPAddr >> 8) & 0x00ff);
		
		std::cout << "TXT Laps Short Hi: " << (int)(mLPHi) << std::endl;
		std::cout << "TXT Laps Short Lo: " << (int)(mLPLo) << std::endl << std::endl;		

	
		obuffer.push_back(mLPHi);
		obuffer.push_back(mLPLo);		
	
	}
		
	outfile.write((char*)obuffer.data(),obuffer.size());
	outfile.close();
	
	std::cout << "\"game.bin\" has been written" << std::endl;
		
	return 0;
}

