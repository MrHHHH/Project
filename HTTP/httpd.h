/*************************************************************************
	> File Name: httpd.h
	> Author: XH
	> Mail: X_H_fight@163.com 
	> Created Time: Sun 02 Apr 2017 02:03:10 PM CST
 ************************************************************************/

#ifndef __HTTPD_H__
#define __HTTPD_H__

#include <iostream>
#include <cassert>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
using namespace std;

#define _SIZE_ (1024*4)
//日志等级
enum logGrade
{
	NORMAL,
	WARNING,
	FATAL,
};


int StartUp(const char *ip, int port);
void PrintLog(const char *logMsg, logGrade grade);
int Handle_Request(int sock);
#endif
