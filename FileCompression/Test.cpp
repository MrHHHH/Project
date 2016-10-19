/*************************************************************************
	> File Name: Test.cpp
	> Author: XH
	> Mail: X_H_fight@163.com 
	> Created Time: Sat 15 Oct 2016 01:48:22 PM CST
 ************************************************************************/

#include <iostream>
#include "FileCompress.h"
using namespace std;


void TestCompress()
{
	FileCompress fc;
	 char* fileName = "source.txt";
	fc.Compress(fileName);
}

void TestUncompress()
{
	FileCompress fc;
	const char* fileName = "source.txt";
	string compressFileName = fileName;
	compressFileName += ".compress";
	fc.UnCompress(compressFileName.c_str());
}
int main()
{
	//TestCompress();
	TestUncompress();
	return 0;
}
