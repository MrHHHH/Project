/*************************************************************************
	> File Name: httpd.c
	> Author: XH
	> Mail: X_H_fight@163.com 
	> Created Time: Sun 02 Apr 2017 02:02:53 PM CST
 ************************************************************************/
#include "httpd.h"

int StartUp(const char *ip, int port)
{
	assert(ip);
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);

	if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
	{
		PrintLog("bind is failed!", FATAL);
		exit(2);
	}

	if(listen(sock, 5) < 0)
	{
		PrintLog("listen is failed!", FATAL);
		exit(3);
	}

	PrintLog("listen is success!", NORMAL);
	return sock;
}

void PrintLog(const char *logMsg, logGrade grade)
{
	assert(logMsg);
	
	switch(grade)
	{
		case NORMAL:
			cout << logMsg << "NORMAL" << endl;
			break;
		case WARNING:
			cout << logMsg << "WARNING" << endl;
			break;
		case FATAL:
			cout << logMsg << "FATAL" << endl;
			break;
		default:
			break;
	}
}


void *Handle_Request(int sock)
{
	cout << "read" << endl;
	char buf[10240];
	ssize_t s = read(sock, buf, sizeof(buf)-1);
	buf[s] = 0;
	cout << buf << endl;
}

