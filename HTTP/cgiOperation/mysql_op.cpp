#include "mysql_op.h"

bool database::connect(const char *host, const char *user, const char* passwd, const char *db)
{
	_conn = mysql_init(NULL);
	if(_conn == NULL)
	{
		cout << "mysql_init failed!" << endl;
		return false;
	}

	if(mysql_real_connect(_conn, host, user, passwd, db, 0, NULL, 0)==NULL)
	{
		cout << "mysql_real_connect failed" << endl;
		fprintf(stderr, "Failed to connect to database: Error: %s\n", \
				mysql_error(_conn));
		
		return false;
	}
	return true;
}

bool database::INSERT(const char *insComm)//插入
{
	if(strncasecmp(insComm, "insert into", strlen("insert into")) != 0)
	{
		cout << "插入语句输入错误" << endl;
		return false;
	}

	if(mysql_real_query(_conn, insComm, strlen(insComm))) //失败
	{
		cout << "插入失败" << endl;
		return false;
	}
	else
	{
		return true;
	}
}

bool database::DELETE(const char *delComm)//删除
{
	if(strncasecmp(delComm, "delete from", strlen("delete from")) != 0)
	{
		cout << "删除语句输入错误" << endl;
		return false;
	}

	if(mysql_real_query(_conn, delComm, strlen(delComm))) //失败
	{
		cout << "删除失败" << endl;
		return false;
	}
	else
	{
		return true;
	}
}

bool database::UPDATE(const char *updComm)//修改
{
	if(strncasecmp(updComm, "update", strlen("update")) != 0)
	{
		cout << "更改语句输入错误" << endl;
		return false;
	}

	if(mysql_real_query(_conn, updComm, strlen(updComm))) //失败
	{
		cout << "更改失败" << endl;
		return false;
	}
	else
	{
		return true;
	}
}

bool database::SELECT(const char *selComm)//查找
{
	if(mysql_real_query(_conn, selComm, strlen(selComm)))
	{
		cout << "SELECT: mysql_real_query failed" << endl;
		return false;
	}

	MYSQL_RES *result = NULL;
	//一次性取得数据集
	if((result = mysql_store_result(_conn)) == NULL)
	{
		cout << "SELECT: mysql_store_reselt" << endl;
		return false;
	}
	//获取行数
	int rows = mysql_num_rows(result);
	cout << "rows:" << rows << endl;

	//取得并打印各字段的名称
	int fieldCount = mysql_num_fields(result);
	MYSQL_FIELD *field = NULL;
	for(int i = 0; i < fieldCount; ++i)
	{
		field = mysql_fetch_field_direct(result, i);
		cout << field->name << "\t\t";
	}
	cout << endl;

	//打印各行内容
	MYSQL_ROW row = NULL;
	while(row = mysql_fetch_row(result))
	{
		for(int i = 0; i < fieldCount; ++i)
		{
			cout << row[i] << "\t\t"; 
		}
		cout << endl;
	}
	return true;
}

