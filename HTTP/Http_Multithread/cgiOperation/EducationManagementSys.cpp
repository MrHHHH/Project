/*************************************************************************
	> File Name: dateCalculator.cpp
	> Author: XH
	> Mail: X_H_fight@163.com 
	> Created Time: Sun 09 Apr 2017 05:43:29 PM CST
 ************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <cstring>
#include "mysql_op.h"
using namespace std;

#define _SIZE_ 1024

//进行运算的函数
static int Calculator(char *data)
{
	printf("begin calculator\n");
	//解析resource字符串获取参数
	char *cur = data;
	char *arr[4];
	int i = 0;
	while(*cur)
	{
		if(*cur == '=')
		{
			arr[i++] = cur+1;
		}
		else if(*cur == '&')
		{
			*cur = '\0';
		}
		
		++cur;
	}
	arr[i] = NULL;

	char *name = arr[0];
	int age = atoi(arr[1]);
	char *sex = arr[2];
	database db;
	//一定要注意第一个参数127.0.0.1与localhost不同。
	//127.0.0.1:使用tcp/ip连接
	//localhost：使用unix domain socket连接
	if(db.connect("127.0.0.1", "root", "", "education_management_sys") == false)
		return 1;
	printf("connect is end\n");

	char buf_in[1024];
	sprintf(buf_in, "INSERT INTO stu_info(name, age, sex) VALUES ('%s', %d, '%s')",\
			name, age, sex);
	if(db.INSERT(buf_in) == false)
		return 2;
	printf("insert is end\n");
	if(db.SELECT("SELECT * FROM stu_info") == false)
		return 3;

	return 0;
}

int main()
{
	//通用网关协议/接口
	char method[_SIZE_/2];
	int contentLength;
	char resource[_SIZE_];
	printf("main\n");
	if(getenv("METHOD") == NULL)
	{
		printf("METHOD is not exist!\n");
		return 1;
	}
	else
	{
		strcpy(method, getenv("METHOD"));
	}

	if(strcasecmp(method, "GET") == 0)
	{
		if(getenv("RESOURCE") == NULL)
		{
			printf("RESOURCE is not exist!\n");
			return 2;
		}
		else
			strcpy(resource, getenv("RESOURCE"));
	}
	else  //POST		
	{
		if(getenv("CONTENTLENGTH") == NULL)
		{
			printf("CONTENTLENGTH is not exist!\n");
			return 3;
		}
		else
		{
			int i = 0;
			char c = '\0';
			contentLength = atoi(getenv("CONTENTLENGTH"));
			for( ; i < contentLength; ++i)
			{
				read(0, &c, 1);
				resource[i] = c;
			}
			resource[i] = '\0';
		}
	}
	Calculator(resource);
	return Calculator(NULL);
	
}
