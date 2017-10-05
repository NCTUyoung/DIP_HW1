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
struct Pixel32
{
    unsigned R, G, B,A;
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
//    int data_size;
};
bool read_bmp(char* filename,BMP &bmp){
    fstream file;
    FILE * pFile;
    pFile = fopen (filename, "rb");
    long size;
    fseek (pFile, 0, SEEK_END);   // non-portable
    size=ftell (pFile);
    cout<<"file actual size "<<size<<endl;
    fseek (pFile, 0, SEEK_SET);
    //將資料從檔案輸入到記憶體
    unsigned char* ptr;
    ptr = (unsigned char *)&(bmp.h);
    fread(ptr, sizeof(unsigned char),sizeof(bmpheader),pFile);

    if(bmp.h.bitmap_headersize>40){
        bmp.buffer = new unsigned char[bmp.h.bitmap_headersize-40];
        fread(bmp.buffer, sizeof(unsigned char),bmp.h.bitmap_headersize-40,pFile);
    }
//    bmp.data_size = (bmp.h).width*(bmp.h).height*((bmp.h).bits_perpixel/8);
    bmp.data = new unsigned char[bmp.h.bitmap_datasize];
    fread(bmp.data, sizeof(unsigned char), bmp.h.bitmap_datasize,pFile);
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
    fwrite((bmp.data), sizeof(unsigned char), bmp.h.bitmap_datasize,ofp);
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

    for (int i = 0; i <bmp.h.bitmap_datasize; ++i) {
        bmp.data[i] = (bmp.data[i]/d)*d;
    }
}
void bmp_resize(double rate,BMP &bmp){
    unsigned char channel = (unsigned char)(bmp.h.bits_perpixel/8);
    int new_width = (int)bmp.h.width*1.5;
    int new_height = (int)bmp.h.height*1.5;
    int map_x,map_y;
    double d1,d3,left_up,left_down,right_up,right_down;
    unsigned char* padding_img = new unsigned char[(bmp.h.width+1)*(bmp.h.height+1)*channel]();
    unsigned char* resize_data = new unsigned char[new_height*new_width*channel];
    for (int i = 0; i < bmp.h.height; ++i) {
        for (int j = 0; j < bmp.h.width; ++j) {
            for (int k = 0; k < channel; ++k) {
                padding_img[i*(bmp.h.width+1)*channel+ j*channel+ k] = bmp.data[i*bmp.h.width*channel+j*channel+k];
            }
        }
    }



    for (int i = 0; i < new_height; ++i) {
        for (int j = 0; j < new_width; ++j) {
            for (int k = 0; k < channel; ++k) {
                map_x = (int)(i/1.5);
                map_y = (int)(j/1.5);
                d1 = j/1.5-map_y;
                d3 = i/1.5-map_x;
                left_up = padding_img[map_x*(bmp.h.width+1)*channel+map_y*channel+k];
                left_down = padding_img[(map_x+1)*(bmp.h.width+1)*channel+map_y*channel+k];
                right_up = padding_img[map_x*(bmp.h.width+1)*channel+(map_y+1)*channel+k];
                right_down = padding_img[(map_x+1)*(bmp.h.width+1)*channel+(map_y+1)*channel+k];
                double fx1 = left_up + d1*(right_up-left_up);
                double fx2 = left_down + d1*(right_down-left_down);
//                if((fx1 + d3*(fx1-fx2))>255){
//                    resize_data[i*new_width*channel+j*channel+k] = 255;
//                    cout<<fx1<<" "<<fx2<<" "<<d3<<endl;
//                }
//                else if((fx1 + d3*(fx1-fx2))<0){
//                    resize_data[i*new_width*channel+j*channel+k] = 0;
//                }
//                else{
//
//                }
                resize_data[i*new_width*channel+j*channel+k] = (fx1 + d3*(fx2-fx1));


            }
        }
    }
    unsigned char* a = bmp.data;
    delete(a);
    bmp.data = resize_data;
    bmp.h.width =new_width;
    bmp.h.height=new_height;
    bmp.h.bitmap_datasize = bmp.h.width*bmp.h.height*channel;
    bmp.h.filesize = bmp.h.bitmap_dataoffset+bmp.h.bitmap_datasize;

}
int main(){
    BMP image;
    read_bmp("input1.bmp",image);
//    bmp_resolution(8,image);

    bmp_resize(1.5,image);
    bmp_info(image);
    write_bmp("output.bmp",image);

    FILE * pFile;
    pFile = fopen ("output.bmp", "rb");
    long size;
    fseek (pFile, 0, SEEK_END);   // non-portable
    size=ftell (pFile);
    cout<<"file output size "<<size<<endl;
    return 0;
}