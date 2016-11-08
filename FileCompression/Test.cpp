/*************************************************************************
	> File Name: Test.cpp
	> Author: XH
	> Mail: X_H_fight@163.com
	> Created Time: Sat 15 Oct 2016 01:48:22 PM CST
	************************************************************************/

#include "FileCompress.h"
#include "DirectoryCompress.h"
#include <time.h>

void TestCompress()
{
	FileCompress fc;
	//char* fileName = "Input.BIG"; //文本
	const char* fileName = "294121.jpg";   //图片
	//const char* fileName = "1.mp3";   //音频
	//const char* fileName = "jdbc1.exe";   //视频
	string name(fileName);
	fc.Compress(name);
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
	fc.UnCompress(compressFileName);
	cout << "解压缩" << endl;
}

void TestDirCompress() //文件夹压缩
{
	DirectionCompress dc;
	const string comPath("C:\\学习\\code\\DataStruct\\文件压缩\\文件压缩\\test");
	
	double startCom = clock();
	dc.Compress(comPath);
	double timeCom = (clock() - startCom) / 1000;
	cout << "文件夹压缩用时：" << timeCom << endl;

	const string uncompath = (comPath + ".compress");
	startCom = clock();
	dc.Uncompress(uncompath);
	double timeUnCom = (clock() - startCom) / 1000;
	cout << "文件夹解压缩用时：" << timeUnCom << endl;
}

int main()
{
	//1.测试文件压缩

	/*double startCom = clock();
	TestCompress();
	double timeCom = (clock() - startCom)/1000;
	cout <<"用时："<< timeCom << endl;

	double startUncom = clock();
	TestUncompress();
	double timeUncom = (clock() - startUncom)/1000;
	cout <<"用时："<< timeUncom << endl;
	system("pause");*/

	//2.测试文件夹压缩
	TestDirCompress();

	system("pause");
	return 0;
}
