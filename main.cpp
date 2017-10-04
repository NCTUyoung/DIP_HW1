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
typedef struct BMPheader{
    unsigned short identifier;      // 0x0000
    unsigned int filesize;          // 0x0002
    unsigned int reserved;          // 0x0006
    unsigned int bitmap_dataoffset; // 0x000A
    unsigned int bitmap_headersize; // 0x000E
    unsigned int width;             // 0x0012
    unsigned int height;            // 0x0016
    unsigned short planes;          // 0x001A
    unsigned short bits_perpixel;   // 0x001C
    unsigned int compression;       // 0x001E
    unsigned int bitmap_datasize;   // 0x0022
    unsigned int hresolution;       // 0x0026
    unsigned int vresolution;       // 0x002A
    unsigned int usedcolors;        // 0x002E
    unsigned int importantcolors;   // 0x0032
//    unsigned int palette;           // 0x0036
} __attribute__((packed,aligned(2))) bmpheader;
struct BMP {
    BMPheader h;
    unsigned char* data;
};
bool read_bmp(char* filename,BMP &bmp){
    fstream file;
    FILE * pFile;
    pFile = fopen (filename, "rb");

    //將資料從檔案輸入到記憶體
    unsigned char* ptr;
    ptr = (unsigned char *)&(bmp.h);
//	file.open("input2.bmp", ios::in | ios::binary);
    fread(ptr, sizeof(unsigned char),sizeof(bmpheader),pFile);
    //file.read(RGBQUAD[0], sizeof(RGBQUAD));
    bmp.data = new unsigned char[(bmp.h).width*(bmp.h).height*((bmp.h).bits_perpixel/8)];
    fread(bmp.data, sizeof(unsigned char), (bmp.h).width*(bmp.h).height*((bmp.h).bits_perpixel/8),pFile);
    fclose(pFile);
}
bool write_bmp(char* filename,BMP &bmp){
//將資料從記憶體輸出到檔案
    FILE* ofp;
    unsigned char* ptr;
    ofp = fopen (filename, "wb");
    ptr = (unsigned char *)&(bmp.h);
    fwrite(ptr, sizeof(unsigned char),sizeof(bmpheader),ofp);
    fwrite((bmp.data), sizeof(unsigned char), ((bmp.h).width*(bmp.h).height*((bmp.h).bits_perpixel/8)),ofp);
    fclose(ofp);
}
void bmp_info(BMP &bmp){
    //顯示檔案資訊
    cout << "FILE_HEADER\n"
         << "bfType=\t\t" << bmp.h.identifier << '\n'
         << "bfSize=\t\t" << bmp.h.filesize<< '\n'
         << "bfReserved=\t" << bmp.h.reserved << '\n'
         << "bfOffBits=\t" << bmp.h.bitmap_dataoffset << "\n\n"
         << "INFO_HEADER\n"
         << "biSize=\t\t" << bmp.h.bitmap_headersize << '\n'
         << "biWidth=\t" << bmp.h.width << '\n'
         << "biHeight=\t" << bmp.h.height << '\n'
         << "biPlanes=\t" << bmp.h.planes << '\n'
         << "biBitCount=\t" << bmp.h.bits_perpixel << '\n'
         << "biCompression=\t" << bmp.h.compression << '\n'
         << "biSizeImage=\t" << bmp.h.bitmap_datasize << '\n'
         << "biXPelsPerMeter=" << bmp.h.hresolution << '\n'
         << "biYPelsPerMeter=" << bmp.h.vresolution << '\n'
         << "biClrUsed=\t" << bmp.h.usedcolors << '\n'
         << "biClrImportant=\t" << bmp.h.importantcolors << "\n\n";
}

int main(){
    BMP image;
    read_bmp("input2.bmp",image);
    bmp_info(image);
    write_bmp("output.bmp",image);
    return 0;
}