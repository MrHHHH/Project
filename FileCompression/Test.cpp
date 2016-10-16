/*************************************************************************
	> File Name: Test.cpp
	> Author: XH
	> Mail: X_H_fight@163.com 
	> Created Time: Sat 15 Oct 2016 01:48:22 PM CST
 ************************************************************************/

#include <iostream>
#include "FileCompress.h"
using namespace std;

int main()
{
	FileCompress fc;
	fc.Compress("source.txt");
	return 0;
}
