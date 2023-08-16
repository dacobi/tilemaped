#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>


unsigned char colmapcar[16][16][2];

float getlength(int cx, int cy){

	return sqrt((cx*cx)+(cy*cy));

}

void extentvec(int *cx, int *cy){
	int nx,ny;
	int dx,dy;
	float cl,dl;
	float fl;

	nx = *cx;
	ny = *cy;
	
	cl = getlength(nx,ny);
	
	//dl = float(nx) / float(ny);
	
	fl = 16.0 / cl;
		
	dx = (int)ceil((float)nx * fl);
	dy = (int)ceil((float)ny * fl);	

	*cx = dx - nx;
	*cy = dy - ny;
}

void calc_colval(int cx, int cy, unsigned char *nx, unsigned char *ny){

	int cnx, cny;

	if( (cx == 0) && ( cy == 0) ){
		*nx = 12;
		*ny = 12;
		return;
	}
	
	if(cy == 0){
		*nx = 16 - cx;
		*ny = 0;
		return;	
	}
	
	if(cx == 0){
		*ny = 16 - cy;
		*nx = 0;
		return;	
	}
	
	if( ((cx * cx) + (cy * cy)) >= 256 ){
		*ny = 0;
		*nx = 0;
		return;		
	}

	cnx = cx;
	cny = cy;

	extentvec(&cnx, &cny);
	
	if(cnx > 15){
		cnx = 15;
	}
	
	if(cny > 15){
		cny = 15;
	}

	
	*nx = cnx;
	*ny = cny;
}

void init_colmap(){

	for(int y = 0; y < 16; y++){
		for(int x = 0; x < 16; x++){
			calc_colval(x, y, &colmapcar[x][y][0], &colmapcar[x][y][1]);
		}
	
	}

}

void print_colmap(bool bDoXY){

	if(bDoXY){
		std::cout << "unsigned char colmapcar[16][16][2] = {";
	} else {
		std::cout << "unsigned char colmapcar[16][16] = {";
	}

	for(int y = 0; y < 16; y++){
	
		std::cout << "{";
	
		for(int x = 0; x < 16; x++){
		
			if(bDoXY){		
				std::cout << "{" ;
				std::cout << (int)colmapcar[x][y][0];
				std::cout << "," ;
				std::cout << (int)colmapcar[x][y][1];
				std::cout << "}";				
			} else {			
				int cVal = colmapcar[x][y][1];
				cVal = cVal << 4;
				cVal += colmapcar[x][y][0];
				
				std::cout << cVal;			
			}
			
			if(x != 15){
				std::cout << ",";
			}	
				 
		}
		
		std::cout << "}";
		if(y != 15){
			std::cout << "," << std::endl;
		}		 
	}
	
	std::cout << "};" << std::endl;

}

int main(int argc, char *argv[]){

	init_colmap();
	print_colmap(argc == 2);
	
	return 0;
}

