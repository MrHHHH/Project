#include <iostream>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <mysql.h>
using namespace std;

class Database
{
public:
	Database();
	~Database();
public:
	bool connect();
	bool INSERT(const char *insComm);//插入
	bool DELETE(const char *delComm);//删除
	bool UPDATE(const char *updComm);//修改
	bool SELECT(const char *selComm);//查找
private:
	MYSQL* _conn;
};
