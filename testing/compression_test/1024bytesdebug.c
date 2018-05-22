#include <stdio.h>

#define DIMENSION 32

typedef _data{
	
}data;

int EEPROM[DIMENSION];

void print_mem(){
	printf("###############################################\n");
	for (int i = 0; i < DIMENSION; ++i)
	{
			printf("%4d|", EEPROM[i]);
	}
	printf("###############################################\n");
}

void setup(){
	// initiate memory with zeros
	for (int i = 0; i < DIMENSION; ++i)
	{
		EEPROM[i] = 0;
	}
}

void loop(){

}

int main(int argc, char const *argv[])
{
	
	return 0;
}