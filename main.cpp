#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#pragma pack(2)
using namespace std;



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
    unsigned char* buffer;
    unsigned char* data;
    int data_size;
};
bool read_bmp(char* filename,BMP &bmp){
    fstream file;
    FILE * pFile;
    pFile = fopen (filename, "rb");

    //將資料從檔案輸入到記憶體
    unsigned char* ptr;
    ptr = (unsigned char *)&(bmp.h);
    fread(ptr, sizeof(unsigned char),sizeof(bmpheader),pFile);
    if(bmp.h.bitmap_headersize>40){
        bmp.buffer = new unsigned char[bmp.h.bitmap_headersize-40];
        fread(bmp.buffer, sizeof(unsigned char),bmp.h.bitmap_headersize-40,pFile);
    }
    bmp.data_size = (bmp.h).width*(bmp.h).height*((bmp.h).bits_perpixel/8);
    bmp.data = new unsigned char[bmp.data_size];
    fread(bmp.data, sizeof(unsigned char), bmp.data_size,pFile);
    fclose(pFile);
    return true;
}
bool write_bmp(char* filename,BMP &bmp){
//將資料從記憶體輸出到檔案
    FILE* ofp;
    unsigned char* ptr;
    ofp = fopen (filename, "wb");
    ptr = (unsigned char *)&(bmp.h);
    fwrite(ptr, sizeof(unsigned char),sizeof(bmpheader),ofp);
    if(bmp.h.bitmap_headersize>40){
        fwrite(bmp.buffer, sizeof(unsigned char),bmp.h.bitmap_headersize-40,ofp);
    }
    fwrite((bmp.data), sizeof(unsigned char), bmp.data_size,ofp);
    fclose(ofp);
    return true;
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
void bmp_resolution(unsigned char q,BMP &bmp){
    unsigned char d = 256/q;

    for (int i = 0; i <bmp.data_size; ++i) {
        bmp.data[i] = (bmp.data[i]/d)*d;
    }
}
int main(){
    BMP image;
    read_bmp("input1.bmp",image);
    bmp_resolution(8,image);
    bmp_info(image);
    write_bmp("output.bmp",image);
    return 0;
}