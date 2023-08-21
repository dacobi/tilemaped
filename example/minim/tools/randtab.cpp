#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>

signed char randtab[100];

int main(){
	
	srand(0);

	std::cout << "signed char randtab[100] = {";
	
	for(int ri = 0; ri < 100; ri++){
		std::cout << (int)(rand() % 255) - 128;
		if(ri != 99){
			std::cout << ",";
		}
	}
	
	std::cout << "};" << std::endl;
	
	return 0;	
}
