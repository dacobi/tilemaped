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

    for(int i = 0; i < 256; i++){
	for(int j = 0; j < 3; j++){
            outbuffer.push_back(palette[i][j]);
	}
    }

    output.write((char*)outbuffer.data(),outbuffer.size());
    output.close();
}
