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
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

//日志等级
enum logGrade
{
	NORMAL,
	WARNING,
	FATAL,
};


int StartUp(const char *ip, int port);
void PrintLog(const char *logMsg, logGrade grade);
void *Handle_Request(int sock);
#endif
