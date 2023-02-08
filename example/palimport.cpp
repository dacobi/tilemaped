#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <filesystem>

//#include "newpalette.h"

int main(int argc, char *argv[])
{
    if(argc != 3){
	std::cout << argv[0] << " <Gimp palette> <filename.bin>" << std::endl;
    	return 1;
    }

    std::ifstream input( argv[1], std::ios::in );
    std::ofstream output( argv[2], std::ios::binary );
    std::vector<unsigned char> outbuffer;

    std::filesystem::path pPath = std::string(argv[1]);

    if(pPath.extension() == ".gpl"){
    	std::cout << "Gimp palette found" << std::endl;
    } else {
    	std::cout << "Not Gimp palette!" << std::endl;
    	return 1;
    }

    outbuffer.push_back(16);
    outbuffer.push_back(42);

    unsigned char tmpChar;
    
    unsigned char palette[256][3];
    
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
    	//std::cout << nline << std::endl;
    	convert << nline << std::endl;
    	convert >> mr >> mg >> mb >> ntmp;
    	//std::cout << "R: " << mr << " G: " << mg << " B: " << mb << std::endl;
    	palette[i][0] = mr;
    	palette[i][1] = mg;
    	palette[i][2] = mb;
    	
    }

    for(int i = 0; i < 256; i++){
    	tmpChar = (palette[i][1])/16;

	tmpChar = tmpChar << 4;
        tmpChar += (palette[i][2])/16;

	outbuffer.push_back(tmpChar);

	tmpChar = (palette[i][0])/16;
        outbuffer.push_back(tmpChar);
    }

    output.write((char*)outbuffer.data(),outbuffer.size());
    output.close();
    
}
