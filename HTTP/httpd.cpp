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
	
	int opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
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

void EchoErrno()
{
	//
}

//从sock中读取一行
static int ReadLine(int sock, char *buf, size_t len)
{
	assert(buf);
	int n = 0;
	char c = 0;
	ssize_t s = 0;
	while(n < len-1 && c != '\n')
	{
		s = recv(sock, &c, 1, 0);
		//  \r和\r\n都是换行
		if(s > 0 && c == '\r')
		{
			s = recv(sock, &c, 1, MSG_PEEK);
			if(s > 0 && c == '\n')
				s = recv(sock, &c, 1, 0);
			else
				c = '\n';
		}
		buf[n++] = c;
	}
	buf[n] = '\0';
	return n;
}

static void ClearHeader(int sock)
{
	char buf[_SIZE_];
	int size = 0;
	
	do
	{
		size = ReadLine(sock, buf, sizeof(buf));
	}while(size!=1 || strcmp(buf, "\n")!=0);   //注意逻辑
}

//处理cgi模式的函数
static int ExcuCgi(int sock, const char *method, const char *path, const char *resource)
{
	char methodEnv[_SIZE_/16];
	char resourceEnv[_SIZE_/4];
	char contentLengthEnv[_SIZE_/8];
	//处理http头部信息
	int contentLength = -1; // 请求正文内容长度--处理粘包问题
	char buf[_SIZE_/8];
	if(strcasecmp(method, "GET") == 0)
	{
		ClearHeader(sock);
	}
	else //POST 
	{
		ReadLine(sock, buf, sizeof(buf));
		if(strncmp(buf, "Content-Length: ", 16) == 0)
		{
			contentLength = atoi(buf+16);
		}
		ClearHeader(sock);
	}
	
	//发送应答头部
	const char* statusLine = "http/1.0 200 ok\r\n";
	send(sock, statusLine, strlen(statusLine), 0);
	send(sock, "\r\n", strlen("\r\n"), 0);

	//fork子进程进行程序替换执行文件，利用管道进行进程间通信
	int input[2];
	int output[2];

	if(pipe(input)<0 || pipe(output)<0)
	{
		EchoErrno();
		PrintLog("ExcuCgi pipe error", FATAL);
		return 9;
	}

	pid_t pid = fork();
	if(pid < 0)
	{
		EchoErrno();
		PrintLog("ExcuCgi fork error", FATAL);
		return 10;
	}
	else if(pid == 0)  //child
	{
		//关闭对应管道
		close(input[1]);
		close(output[0]);

		//文件描述符重定向
		dup2(input[0], 0);
		dup2(output[1], 1);

		//通过环境变量传递参数
		sprintf(methodEnv, "METHOD=%s", method);
		if(putenv(methodEnv) != 0)
		{
			EchoErrno();
			PrintLog("ExcuCgi putenv error", FATAL);
			return 11;
		}
		if(strcasecmp(method, "GET") == 0)
		{
			sprintf(resourceEnv, "RESOURCE=%s", resource);
			putenv(resourceEnv);
		}
		else //POST
		{
			sprintf(contentLengthEnv, "CONTENTLENGTH=%d", contentLength);
			putenv(contentLengthEnv);
		}

		//程序替换
		execl(path, path, NULL);
	}
	else  //father
	{
		//关闭对应管道
		close(input[0]);
		close(output[1]);
		
		//读取POST方法正文内容通过管道发送给子进程
		char c = '\0';
		if(strcasecmp(method, "POST") == 0)
		{
			int i = 0;
			for( ; i < contentLength; ++i)
			{
				recv(sock, &c, 1, 0);
				write(input[0], &c, 1);
			}
		}

		//从管道中读取内容发送给sock
		while(read(output[1], &c, 1) > 0)
		{
			send(sock, &c, 1, 0);
		}

		waitpid(pid, NULL , 0);
	}

}

//非cgi模式：将指定路径文件发送给客户端
static int ShowPage(int sock, const char *path, ssize_t size)
{
	int fd = open(path, O_RDONLY);	
	if(fd < 0)
	{
		PrintLog("open file failed", FATAL);
		EchoErrno();
		return 7;
	}
	char buf[_SIZE_];
	sprintf(buf, "HTTP/1.0 200 OK\r\n");
	send(sock, buf, strlen(buf), 0);
	send(sock, "\r\n", strlen("\r\n"), 0);
	if(sendfile(sock, fd, NULL, size) < 0)
	{
		EchoErrno();
		PrintLog("send file failed", FATAL);
		close(fd);
		return 8;
	}
	close(fd);
	return 0;
}

int Handle_Request(int sock)
{
	char line[_SIZE_]; //存储一行的内容
	char *c = line;
	char method[64]; //存储请求方法
	char uri[_SIZE_];
	char *resource = NULL; //指向GET方法的资源
	char path[_SIZE_]; //存储
	int ret = 0;
	int i = 0; //下标标记
	int cgi = 0; //是否为cgi模式
	int size = ReadLine(sock, line, sizeof(line));
	if(size == 0)
	{
		EchoErrno();
		PrintLog("request is error", FATAL);
		ret = 4;
		goto END;
	}
	
	//得到第一行
	//获取请求方法
	while(i <64 && *c != ' ')
	{
		method[i++] = *c++;
	}
	method[i] = 0;
	//获取request-uri
	++c;
	i = 0;
	while(i < _SIZE_ && *c != ' ')
	{
		uri[i++] = *c++;
	}
	uri[i] = 0;
	

	//设置cgi模式
	//1.POST方法肯定为cgi模式，
	//2.GET方法如果uri中有?(即携带资源)则使用cgi模式, 
	//3.请求路径为可执行文件用cgi模式
	if(strcasecmp("POST", method) == 0)
	{
		cgi = 1;
	}
	else if(strcasecmp("GET", method) == 0)
	{
		resource = uri;
		while(*resource && *resource!='?')
		{
			++resource;
		}
		if(*resource == '?')
		{
			//uri指向路径，resource指向资源
			*resource = 0;
			++resource;
			cgi = 1;
		}
	}
	else //既不是GET方法又不是POST方法的先不处理，直接报错
	{
		EchoErrno();
		PrintLog("方法既不是GET又不是POST", FATAL);
		ret = 5;
		goto END;
	}

	//处理请求路径
	sprintf(path, "wwwRoot%s", uri);
	if(path[strlen(path)-1] == '/') //目录
	{
		strcat(path, "homepage.html");
	}
	
	struct stat st;
	
	if(stat(path, &st) < 0) //从文件名中获取文件信息保存在stat结构体中。
	{
		EchoErrno();
		PrintLog("获取path文件信息失败", FATAL);
		ret = 6;
		goto END;
	}
	//获取path文件信息成功
	if(S_ISDIR(st.st_mode)) //检查文件是否为目录
		strcat(path, "/homepage.html");
	else if((st.st_mode) & S_IXUSR ||             //检查文件是否为可执行文件
			(st.st_mode) & S_IXGRP || 
			(st.st_mode) & S_IXOTH )
	{
		cgi = 1;
	}

	//处理cgi模式和非cgi模式
	if(cgi) //cgi
	{
		ret = ExcuCgi(sock, method, path, resource);
	}
	else
	{
		ClearHeader(sock);
		ret = ShowPage(sock, path, st.st_size);
	}

END:
	close(sock);
	return ret;
}

