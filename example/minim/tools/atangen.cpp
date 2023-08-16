#include <iostream>
#include <math.h>

int main(int argc, char *argv[]){
	
	if(argc > 1){
	
	double target = 84;
	double current;


	for(int i = 9; i < 2000; i++){
		current = ((atan(i) * 180) / M_PI) ;
		
		if(current > target){
			std::cout << current << " : "  << i <<  " - "  << ((atan(i-1) * 180) / M_PI) << " : " << i-1  <<std::endl;
			if(target > 89.5) return 0;
			target += 0.5;
		}
				
		
	}

	} else {
	
		double cstep = 0;
		unsigned char cout;
		
		std::cout << "unsigned char atantab[1000] = {" << std::endl;
		
		for(int i = 0; i < 1000; i++){
			cout = ((atan(cstep) * 180) / M_PI) ;
			std::cout << (int)cout;
			if(i < 999) std::cout << ",";
			cstep += 0.01;
		}
		
		std::cout << "};" << std::endl;
	
	}

	return 0;
}
