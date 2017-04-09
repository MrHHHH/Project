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
using namespace std;

#define _SIZE_ 1024

//进行运算的函数
static void Calculator(char *data)
{
	//解析resource字符串获取参数
	char *cur = data;
	char *arr[3];
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
	int data1 = atoi(arr[0]);
	int data2 = atoi(arr[1]);
	printf("%d + %d = %d\n", data1, data2, data1+data2);
}

int main()
{
	//通用网关协议/接口
	char method[_SIZE_/2];
	int contentLength;
	char resource[_SIZE_];

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

}
