#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "qrlib/qrcodegen.h"

// // Prints the given QR Code to the console.
// static void printQr(const uint8_t qrcode[]) {
// 	int size = qrcodegen_getSize(qrcode);
// 	int border = 4;
// 	for (int y = -border; y < size + border; y++) {
// 		for (int x = -border; x < size + border; x++) {
// 			fputs((qrcodegen_getModule(qrcode, x, y) ? "##" : "  "), stdout);
// 		}
// 		fputs("\n", stdout);
// 	}
// 	fputs("\n", stdout);
// }

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
	int totalsize = *size * *size;
	bool* result = (bool*) calloc(totalsize, 1);
	for (int i = 0; i < totalsize; ++i)
	{
		result[i] = qrcodegen_getModule(qrcode, i / (*size), i % (*size));
	}
	return result;
}

// create QR Code from binary data
// // Binary data
// 	uint8_t dataAndTemp[qrcodegen_BUFFER_LEN_FOR_VERSION(7)]
// 	    = {0x00, 0x00, 0x01};
// 	uint8_t qr1[qrcodegen_BUFFER_LEN_FOR_VERSION(7)];
// 	bool ok = qrcodegen_encodeBinary(dataAndTemp, 3, qr1,
// 	    qrcodegen_Ecc_HIGH, 2, 7, qrcodegen_Mask_4, false);
// 	if (ok){
// 		int size = qrcodegen_getSize(qr1);
// 		printf("%d\n", size);
// 		printQr(qr1);
// 	}
// 	else
// 		printf("error\n");

int main(int argc, char const *argv[]){
	int size;
	bool* qrcode = qrFromString("hello world", &size);
	if (!qrcode) return -1;
	FILE* f = fopen("qrcode.txt", "w+");
	for (int i = 0; i < size*size; ++i)
	{
		if (i % size == 0)
			fprintf(f, "\n");

		if (qrcode[i])
			fprintf(f, "1");	
		else
			fprintf(f, "0");	
		
	}
	fprintf(f, "\n");
	free(qrcode);
	fclose(f);
	return 0;
}