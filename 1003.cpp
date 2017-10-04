#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#pragma pack(2)
using namespace std;


typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned long   DWORD;
typedef long            LONG;

struct Pixel
{
	unsigned R, G, B;
};
struct BmpHeader{
	WORD bfType;
	DWORD bfSize;
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits;
	DWORD biSize;
	LONG biWidth;
	LONG biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG biXPelsPerMeter;
	LONG biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
};
bool read_bmp(char* filename, BmpHeader &header,vector<vector<Pixel> > &img){
	fstream file;
	file.open(filename, ios::in | ios::binary);
	file.read((char*)&header.bfType, sizeof(WORD));
	file.read((char*)&header.bfSize, sizeof(DWORD));
	file.read((char*)&header.bfReserved1, sizeof(WORD));
	file.read((char*)&header.bfReserved2, sizeof(WORD));
	file.read((char*)&header.bfOffBits, sizeof(DWORD));
	file.read((char*)&header.biSize, sizeof(DWORD));
	file.read((char*)&header.biWidth, sizeof(LONG));
	file.read((char*)&header.biHeight, sizeof(LONG));
	file.read((char*)&header.biPlanes, sizeof(WORD));
	file.read((char*)&header.biBitCount, sizeof(WORD));
	file.read((char*)&header.biCompression, sizeof(DWORD));
	file.read((char*)&header.biSizeImage, sizeof(DWORD));
	file.read((char*)&header.biXPelsPerMeter, sizeof(LONG));
	file.read((char*)&header.biYPelsPerMeter, sizeof(LONG));
	file.read((char*)&header.biClrUsed, sizeof(DWORD));
	file.read((char*)&header.biClrImportant, sizeof(DWORD));
	//file.read((char*)&header.RGBQUAD[0][0], sizeof(RGBQUAD));
	//file.read(PIXEL[0], sizeof(PIXEL));
	file.close();
	return true;
}
int a(){
	fstream file;
	int i, j;
	char fileName[32], RGBQUAD[3][256], PIXEL[540 * 810 * 4 +2];


	char bfType[2];
	char bfSize[4];
	char bfReserved[4];
	char bfOffBits[4];
    char biSize[4];
	char biWidth[4];
	char biHeight[4];
	char biPlanes[2];
	char biBitCount[2];
	char biCompression[4];
	char biSizeImage[4];
	char biXPelsPerMeter[4];
	char biYPelsPerMeter[4];
	char biClrUsed[4];
	char biClrImportant[4];

	//將資料從檔案輸入到記憶體


	file.open("input1.bmp", ios::in | ios::binary);
	file.read(bfType, 2);
	file.read(bfSize, 4);
	file.read(bfReserved, 4);
	file.read(bfOffBits, 4);
	file.read(biSize, 4);
	file.read(biWidth, 4);
	file.read(biHeight, 4);
	file.read(biPlanes, 2);
	file.read(biBitCount, 2);
	file.read(biCompression, 4);
	file.read(biSizeImage, 2);
	file.read(biXPelsPerMeter, 4);
	file.read(biYPelsPerMeter, 4);
	file.read(biClrUsed, 4);
	file.read(biClrImportant, 4);
	//file.read(RGBQUAD[0], sizeof(RGBQUAD));
	file.read(PIXEL, sizeof(PIXEL));
	file.close();

	//顯示檔案資訊
	cout << "FILE_HEADER\n"
		<< "bfType=\t\t" << *(short*)bfType << '\n'
		<< "bfSize=\t\t" << *(unsigned int*)bfSize<< '\n'
		<< "bfReserved=\t" << *(DWORD *)bfReserved << '\n'
		<< "bfOffBits=\t" << *(unsigned int*)(bfOffBits) << "\n\n"
		<< "INFO_HEADER\n"
		<< "biSize=\t\t" << *(unsigned int*)(biSize) << '\n'
		<< "biWidth=\t" << *(unsigned int*)(biWidth) << '\n'
		<< "biHeight=\t" << *(unsigned int*)(biHeight) << '\n'
		<< "biPlanes=\t" << atoi(biPlanes) << '\n'
		<< "biBitCount=\t" << atoi(biBitCount) << '\n'
		<< "biCompression=\t" << atoi(biCompression) << '\n'
		<< "biSizeImage=\t" << atoi(biSizeImage) << '\n'
		<< "biXPelsPerMeter=" << atoi(biXPelsPerMeter) << '\n'
		<< "biYPelsPerMeter=" << atoi(biYPelsPerMeter) << '\n'
		<< "biClrUsed=\t" << atoi(biClrUsed) << '\n'
		<< "biClrImportant=\t" << atoi(biClrImportant) << "\n\n";

	//11011000 11111111
	//01001101 01000010
	//將資料從記憶體輸出到檔案


	file.open("output.bmp", ios::out | ios::binary);
	file.write(bfType, sizeof(WORD));
	file.write(bfSize, sizeof(DWORD));
	file.write(bfReserved, sizeof(DWORD));
	file.write(bfOffBits, sizeof(DWORD));
	file.write(biSize, sizeof(DWORD));
	file.write(biWidth, sizeof(DWORD));
	file.write(biHeight, sizeof(LONG));
	file.write(biPlanes, sizeof(WORD));
	file.write(biBitCount, sizeof(WORD));
	file.write(biCompression, sizeof(DWORD));
	file.write(biSizeImage, sizeof(DWORD));
	file.write(biXPelsPerMeter, sizeof(LONG));
	file.write(biYPelsPerMeter, sizeof(LONG));
	file.write(biClrUsed, sizeof(DWORD));
	file.write(biClrImportant, sizeof(DWORD));
	//file.write((char*)&RGBQUAD[0], sizeof(RGBQUAD));
	file.write(PIXEL, sizeof(PIXEL));
	file.close();
	cout<<"d";
    return 0;
}