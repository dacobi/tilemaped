#include <stdio.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>

#include "waypoints.h"

int main(){

	std::ofstream outfile("waypoints.bin", std::ios::binary );

	std::vector<unsigned char> obuffer;

	obuffer.push_back(points_level);
	
	for(int wi = 0; wi < points_level; wi++){
		obuffer.push_back(way_level[wi][0]);			
		obuffer.push_back(way_level[wi][1]);	
		obuffer.push_back(way_level[wi][2]);			
	}
	
	outfile.write((char*)obuffer.data(),obuffer.size());
	outfile.close();
	
	
	return 0;
}

