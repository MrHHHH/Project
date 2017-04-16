#include <iostream>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <mysql.h>
using namespace std;

class database
{
public:
	database()
	{
	}
	~database()
	{
		mysql_close(_conn);
	}

public:
	bool connect(const char *host, const char *user, const char* passwd, const char *db);
	bool INSERT(const char *insComm);//插入
	bool DELETE(const char *delComm);//删除
	bool UPDATE(const char *updComm);//修改
	bool SELECT(const char *selComm);//查找
private:
	MYSQL* _conn;
};
