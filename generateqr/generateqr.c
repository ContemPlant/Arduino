#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "qrlib/qrcodegen.h"

// Prints the given QR Code to the console.
static void printQr(const uint8_t qrcode[]) {
	int size = qrcodegen_getSize(qrcode);
	int border = 4;
	for (int y = -border; y < size + border; y++) {
		for (int x = -border; x < size + border; x++) {
			fputs((qrcodegen_getModule(qrcode, x, y) ? "##" : "  "), stdout);
		}
		fputs("\n", stdout);
	}
	fputs("\n", stdout);
}

bool* create_bool_array(uint8_t* qrcode, int size){
	int totalsize = size * size;
	bool* result = (bool*) calloc(totalsize, 1);
	for (int i = 0; i < totalsize; ++i)
	{
		result[i] = qrcodegen_getModule(qrcode, i % (size), i / (size));
	}
	return result;
}

// Creates a single QR Code, then prints it to the console.
bool* qrFromString(char* text, int* size) {
	// const char *text = "Hello, world!";  // User-supplied text
	enum qrcodegen_Ecc errCorLvl = qrcodegen_Ecc_LOW;  // Error correction level
	
	// Make and print the QR Code symbol
	uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
	uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
	bool ok = qrcodegen_encodeText(text, tempBuffer, qrcode, errCorLvl,
		qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
	if (!ok) return NULL;
	
	*size = qrcodegen_getSize(qrcode);
	printf("size: %d\n", *size);
	return create_bool_array(qrcode, *size);
}

// create QR Code from binary data
bool* qrFromBin(uint8_t* dataAndTemp, int* size, int len){
	
	uint8_t qrcode[qrcodegen_BUFFER_LEN_FOR_VERSION(7)];
	bool ok = qrcodegen_encodeBinary(dataAndTemp, len, qrcode,
		qrcodegen_Ecc_HIGH, 2, 7, qrcodegen_Mask_4, false);
	if (ok){
		*size = qrcodegen_getSize(qrcode);
		printf("%d\n", *size);
		// printQr(qrcode);
		return create_bool_array(qrcode, *size);
	}
	else{
		printf("error\n");
		return NULL;
	}
}

int main(int argc, char const *argv[]){
	int size;

	// code from binary
	// uint8_t dataAndTemp[qrcodegen_BUFFER_LEN_FOR_VERSION(7)] = {0x41, 0x42, 0x43, 0x44};
	// int len = 4;
	// bool* qrcode = qrFromBin(dataAndTemp, &size, len);

	// code from string
	bool* qrcode = qrFromString("MIT CONTEMPLANT GEHTS DIR GUT MIT CONTEMPLANT GEHTS DIR GUT", &size);

	if (!qrcode) return -1;
	FILE* f = fopen("qrcode.txt", "w+");
	for (int i = 0; i < size*size; ++i)
	{
		// if (i % size == 0)
		// 	fprintf(f, "\n");

		if (qrcode[i])
			fprintf(f, "1");	
		else
			fprintf(f, "0");	
		
	}
	fprintf(f, "\n");
	fprintf(f, "size: %dx%d\n", size, size);
	free(qrcode);
	fclose(f);
	return 0;
}