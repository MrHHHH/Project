/*************************************************************************
	> File Name: Test.cpp
	> Author: XH
	> Mail: X_H_fight@163.com 
	> Created Time: Sat 15 Oct 2016 01:48:22 PM CST
 ************************************************************************/

#include <iostream>
#include "FileCompress.h"
#include <time.h>
using namespace std;


void TestCompress()
{
	FileCompress fc;
	//char* fileName = "Input.BIG"; //文本
	const char* fileName = "294121.jpg";   //图片
	//const char* fileName = "1.mp3";   //音频
	//const char* fileName = "jdbc1.exe";   //视频
	fc.Compress(fileName);
	cout << "压缩" << endl;
}

void TestUncompress()
{
	FileCompress fc;
	//const char* fileName = "Input.BIG";  //文本
	const char* fileName = "294121.jpg";   //图片
	//const char* fileName = "1.mp3";   //音频
	//const char* fileName = "jdbc1.exe";   //视频

	string compressFileName = fileName;
	compressFileName += ".compress";
	fc.UnCompress(compressFileName.c_str());
	cout << "解压缩" << endl;
}
int main()
{
	double startCom = clock();
	TestCompress();
	double timeCom = (clock() - startCom)/1000;
	cout <<"用时："<< timeCom << endl;

	double startUncom = clock();
	TestUncompress();
	double timeUncom = (clock() - startUncom)/1000;
	cout <<"用时："<< timeUncom << endl;
	system("pause");

	return 0;
}
