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
	//char* fileName = "Input.BIG"; //�ı�
	const char* fileName = "294121.jpg";   //ͼƬ
	//const char* fileName = "1.mp3";   //��Ƶ
	//const char* fileName = "jdbc1.exe";   //��Ƶ
	fc.Compress(fileName);
	cout << "ѹ��" << endl;
}

void TestUncompress()
{
	FileCompress fc;
	//const char* fileName = "Input.BIG";  //�ı�
	const char* fileName = "294121.jpg";   //ͼƬ
	//const char* fileName = "1.mp3";   //��Ƶ
	//const char* fileName = "jdbc1.exe";   //��Ƶ

	string compressFileName = fileName;
	compressFileName += ".compress";
	fc.UnCompress(compressFileName.c_str());
	cout << "��ѹ��" << endl;
}
int main()
{
	double startCom = clock();
	TestCompress();
	double timeCom = (clock() - startCom)/1000;
	cout <<"��ʱ��"<< timeCom << endl;

	double startUncom = clock();
	TestUncompress();
	double timeUncom = (clock() - startUncom)/1000;
	cout <<"��ʱ��"<< timeUncom << endl;
	system("pause");

	return 0;
}
