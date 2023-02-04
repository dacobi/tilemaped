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
