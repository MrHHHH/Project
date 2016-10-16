#pragma once
#include <string>
#include <iostream>
#include <cassert>
#include "HuffmanTree.h"
using namespace std;

typedef long long LongType;
struct CharInfo
{
	char _ch;
	LongType _count; //字符出现次数
	string _code; //字符对应的huffman编码


	//如果不初始化则：char,int这种默认类型被初始化为随机值，string调用它的默认构造函数初始化为空串。
	CharInfo(LongType count = 0)
		:_ch(0)
		,_count(count)
		,_code("")
	{}

	bool operator<(const CharInfo& charInfo) const
	{
		return _count < charInfo._count;
	}

	bool operator!=(const CharInfo& charInfo) const
	{
		return _count != charInfo._count;
	}

	CharInfo operator+(const CharInfo& charInfo)
	{
		return CharInfo(_count+charInfo._count);
	}
};


class FileCompress
{
public:
	FileCompress()
	{
		for(int i=0; i<256; ++i)
		{
			_infos[i]._ch = i;
		}
	}

	const char* Compress(char* fileName)
	{
		assert(fileName);

		FILE* fOut = fopen(fileName, "r");
		assert(fOut);
		char ch = fgetc(fOut);
		//统计字符出现的个数
		while(ch != EOF)
		{
			_infos[ch]._count++;
			ch = fgetc(fOut);
		}

		//构造HuffmanTree
		CharInfo invalid;
		HuffmanTree<CharInfo> ht(_infos, 256, invalid);

		//生成Huffman编码
		string code = "";
		CreateHuffmanCode(ht.GetRoot(), code);

		//压缩文件
		string CompressFileName = fileName;
		CompressFileName += ".compress";
		FILE* fIn = fopen(CompressFileName.c_str(), "w");
		assert(fIn);

		char c = 0; //一个字节（8位）的内容
		int num = 0; //当前字节的位数
		fseek(fOut, 0, SEEK_SET);
		ch = fgetc(fOut);
		while(ch != EOF)
		{
			string& code = _infos[ch]._code;
			int size = code.size();
			for(int i=0; i<size; ++i)
			{
				c <<= 1;
				c |= code[i]-'0';
				num++;
			}
			if(num==8)
			{
				fputc(c, fIn);
				num = 0;
			}

			ch = fgetc(fOut);
		}
		//未满8位用0补全
		if(num < 8)
		{
			c <<= (8-num);
		}
		fputc(c, fIn);

		fclose(fOut);
		fclose(fIn);

		return CompressFileName.c_str();
	}
protected:
	void CreateHuffmanCode(HuffmanTreeNode<CharInfo>* root, string code)
	{
		if(root == NULL)
			return;

		if(root->_left==NULL && root->_right==NULL)
		{
			_infos[root->_weight._ch]._code = code;
			return;
		}

		CreateHuffmanCode(root->_left, code+'0');
		CreateHuffmanCode(root->_right, code+'1');

	}
protected:
	CharInfo _infos[256];
};
