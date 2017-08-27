#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
//#include "bitmap_image.hpp"

using namespace std;
using namespace System;
using namespace System::Drawing;
using namespace System::IO;

typedef struct IMAGE_INFO_STRUCT {
	LPARAM DataFilePTR;
} ImageInfoStruct;

//typedef bool(__stdcall *DLLFunc) (ImageInfoStruct&, const char *);
typedef bool(__stdcall *DLLInitImageFileFunc) (ImageInfoStruct& sImageInfo, const char* Path);
typedef bool(__stdcall *DLLUnInitImageFileFunc) (ImageInfoStruct& sImageInfo);
typedef char * (__stdcall *DLLGetImageStreamFunc) (ImageInfoStruct& sImageInfo, float fScale, int nImagePosX, int nImagePosY, int& nDataLength, unsigned char** ImageStream);
typedef bool(__stdcall *DLLDeleteImageDataFunc) (LPVOID pImageData);
typedef bool(__stdcall *DLLGetThumnailImagePathFunc) (const char* szFilePath, unsigned char** ImageData, int& nDataLength, int& nThumWidth, int& nThumHeght);
typedef bool(__stdcall *DLLGetPriviewInfoPathFunc) (const char* szFilePath, unsigned char** ImageData, int& nDataLength, int& nPriviewWidth, int& nPriviewHeight);
typedef bool(__stdcall *DLLGetLableInfoPathFunc) (const char* szFilePath, unsigned char** ImageData, int& nDataLength, int& nLabelWidth, int& nLabelHeight);
typedef bool(__stdcall *DLLGetHeaderInfoFunc) (ImageInfoStruct sImageInfo, int&khiImageHeight, int	&khiImageWidth, int	&khiScanScale, float	&khiSpendTime, double	&khiScanTime, float	&khiImageCapRes, int&khiImageBlockSize);
typedef bool(__stdcall *DLLGetImageDataRoiFunc) (ImageInfoStruct sImageInfo, float fScale, int sp_x, int sp_y, int nWidth, int nHeight, BYTE **pBuffer, int&DataLength, bool flag);
//typedef int (*DLLFunc) (int, int);

DLLInitImageFileFunc InitImageFile;
DLLUnInitImageFileFunc UnInitImageFile;
DLLGetImageStreamFunc GetImageStream;
DLLDeleteImageDataFunc DeleteImageData;
DLLGetThumnailImagePathFunc GetThumnailImagePath;
DLLGetPriviewInfoPathFunc GetPriviewInfoPath;
DLLGetLableInfoPathFunc GetLableInfoPath;
DLLGetHeaderInfoFunc GetHeaderInfo;
DLLGetImageDataRoiFunc GetImageDataRoi;

/*
Bitmap^ add2Bitmap(Bitmap ^first, Bitmap ^second) {
	int width = first->Width + second->Width;
	int height = max(first->Height, second->Height);
	cout << "bitmap width and height: " << width << height << endl;
	Bitmap ^result = (Bitmap^)(gcnew Bitmap(20736,42752));
	Graphics^g = Graphics::FromImage(result);
	g->DrawImage(first, 0, 0);
	g->DrawImage(second, 0, first->Width);
	delete first;
	delete second;
	return result;
}
*/

int main(int argc, char **argv) {

	HINSTANCE hInstLibrary = LoadLibrary(L"ImageOperationLib.dll");
	//HINSTANCE hInstLibrary = LoadLibrary("DLLGenerator.dll");
	if (hInstLibrary == NULL) {
		FreeLibrary(hInstLibrary);
		cout << GetLastError() << endl;
		cout << "LOAD dll ERROR !" << endl;
		getchar();
		return -1;
	}

	InitImageFile = (DLLInitImageFileFunc)GetProcAddress(hInstLibrary, "InitImageFileFunc");
	UnInitImageFile = (DLLUnInitImageFileFunc)GetProcAddress(hInstLibrary, "UnInitImageFileFunc");
	GetImageStream = (DLLGetImageStreamFunc)GetProcAddress(hInstLibrary, "GetImageStreamFunc");
	DeleteImageData = (DLLDeleteImageDataFunc)GetProcAddress(hInstLibrary, "DeleteImageDataFunc");
	GetThumnailImagePath = (DLLGetThumnailImagePathFunc)GetProcAddress(hInstLibrary, "GetThumnailImagePathFunc");
	GetPriviewInfoPath = (DLLGetPriviewInfoPathFunc)GetProcAddress(hInstLibrary, "GetPriviewInfoPathFunc");
	GetLableInfoPath = (DLLGetLableInfoPathFunc)GetProcAddress(hInstLibrary, "GetLableInfoPathFunc");
	GetHeaderInfo = (DLLGetHeaderInfoFunc)GetProcAddress(hInstLibrary, "GetHeaderInfoFunc");
	GetImageDataRoi = (DLLGetImageDataRoiFunc)GetProcAddress(hInstLibrary, "GetImageDataRoiFunc");


	if (InitImageFile == NULL || UnInitImageFile == NULL || GetImageStream == NULL || DeleteImageData == NULL || GetThumnailImagePath == NULL ||
		GetPriviewInfoPath == NULL || GetLableInfoPath == NULL || GetHeaderInfo == NULL || GetImageDataRoi == NULL) {
		FreeLibrary(hInstLibrary);
		cout << "GET functions ERROR !" << endl;
		getchar();
		return -1;
	}

	ImageInfoStruct sImageInfo;

	int khiImageHeight;
	int	khiImageWidth;
	int	khiScanScale;
	float khiSpendTime;
	double khiScanTime;
	float khiImageCapRes;
	int khiImageBlockSize;

	float fScale = 20.0f;
	int nImagePosX = 0; //must be multiple of 256
	int nImagePosY = 0;
	int nDataLength = 0;
	int nDataLength2 = 0;
	unsigned char *tmp1 = new unsigned char();
	unsigned char **ImageStream = &tmp1;
	unsigned char *tmp2 = new unsigned char();
	unsigned char **ImageStream2 = &tmp2;

	//argv[1] = "D:\\2017-08-07 18_19_15.kfb";
	//argv[2] = "D:\\2017-08-07-18_19_15L";
	//argv[3] = "8";

	bool iif = InitImageFile(sImageInfo, argv[1]);
	if (iif == false) {
		cout << "InitImageFile ERROR !" << endl;
		getchar();
		return -1;
	}
	else
		cout << "InitImageFile done !" << endl;

	bool ghi = GetHeaderInfo(sImageInfo, khiImageHeight, khiImageWidth, khiScanScale, khiSpendTime, khiScanTime, khiImageCapRes, khiImageBlockSize);
	if (ghi == false) {
		cout << "GetHeaderInfo ERROR !" << endl;
		getchar();
		return -1;
	}
	
	int x = 0;
	int y = 0;
	char *gis;
	while (true) {
		gis = GetImageStream(sImageInfo, fScale, nImagePosX + x * 256, nImagePosY, nDataLength, ImageStream);
		if (nDataLength == 0) {
			x = (x - 1) * 256;
			break;
		}
		x++;
	}
	while (true) {
		gis = GetImageStream(sImageInfo, fScale, nImagePosX, nImagePosY + y * 256, nDataLength, ImageStream);
		if (nDataLength == 0) {
			y = (y - 1) * 256;
			break;
		}
		y++;
	}
	//cout << "According to datalength, coordinates: " << x << ", " << y << endl;


	//get area
	int w = x;
	int h = y;
	int sp_x = 0;
	int sp_y = 0;
	bool flag = true;
	int size = (int)atof(argv[3]);

	/*
	bool gidr = GetImageDataRoi(sImageInfo, fScale, 0, 0, w / size, h, ImageStream, nDataLength, flag);
	if (gidr == false) {
		cout << "getimage roi ERROR !" << endl;
		getchar();
		return -1;
	}
	cout << "GetImageData done !" << endl;

	Stream ^ss = gcnew MemoryStream();
	unsigned char *tt = (unsigned char*)(*ImageStream);
	for (int kk = 0; kk < nDataLength; kk++) {
		ss->WriteByte(*tt);
		tt++;
	}
	ss->Position = 0;
	Bitmap ^BM = (Bitmap^)Bitmap::FromStream(ss);
	*/

	for (int ii = 0; ii < size; ii++) {
		bool gidr = GetImageDataRoi(sImageInfo, fScale, w * ii / size, 0, w / size, h, ImageStream, nDataLength, flag);
		if (gidr == false) {
			cout << "getimage roi ERROR !" << endl;
			getchar();
			return -1;
		}
		cout << "GetImageData done !" << endl;


		Stream ^s = gcnew MemoryStream();
		unsigned char *t = (unsigned char*)(*ImageStream);

		for (int k = 0; k < nDataLength; k++) {
			s->WriteByte(*t);
			t++;
		}
		s->Position = 0;

		cout << "WriteImageStream done !" << endl;

		//Bitmap ^bitmap = (Bitmap^)Bitmap::FromStream(s);
		//BM = add2Bitmap(BM, bitmap);


		string str;
		str.assign(argv[2]);
		str += std::to_string(ii);
		str.append(".jpeg");
		//cout << str << endl;
		//getchar();
		Bitmap BM(s);
		BM.Save(gcnew String(str.c_str()), System::Drawing::Imaging::ImageFormat::Jpeg);
		cout << "SaveJpegFile done !!!" << endl;
		System::IntPtr ip = BM.GetHbitmap();
		DeleteObject(HGDIOBJ(ip));
	}
	
	
	UnInitImageFile(sImageInfo);
	FreeLibrary(hInstLibrary);
	//delete(tmp1);
	//delete(ImageStream);
	//free(ImageStream);
	//free(tmp1);

	//cout << "done successfully !!!" << endl;
	//getchar();
	return  1;
}