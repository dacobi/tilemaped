#include <fstream>
#include <iterator>
#include <algorithm>

#include "newpalette.h"

int main(int argc, char *argv[])
{
    if(argc != 2){
    	return 1;
    }

    std::ofstream output( argv[1], std::ios::binary );
    std::vector<unsigned char> outbuffer;

    outbuffer.push_back(16);
    outbuffer.push_back(42);

    unsigned char tmpChar;
    int cCount=0;

    for(int i = 0; i < 256; i++){
	for(int j = 0; j < 3; j++){
	    if(cCount == 0){
	    	tmpChar = (palette[i][j])/0xf;
		tmpChar = tmpChar << 4;
		cCount++;
		continue;
	    }
	    if(cCount == 1){
	    	tmpChar += (palette[i][j])/0xf;
	        outbuffer.push_back(tmpChar);
		cCount = 0;		
	    }
	}
    }

    output.write((char*)outbuffer.data(),outbuffer.size());
    output.close();
}
