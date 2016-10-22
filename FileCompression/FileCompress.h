#pragma once
#include <string>
#include <iostream>
#include <cassert>
#include "HuffmanTree.h"
using namespace std;

//问题：处理哈夫曼编码>=8位的字符，这些字符没必要压缩


typedef long long LongType;
struct CharInfo
{
	unsigned char _ch;
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
	//压缩
	const char* Compress(char* fileName)
	{
		assert(fileName);

		FILE* fOut = fopen(fileName, "rb");
		assert(fOut);

		//处理汉字，char放不下
		unsigned char ch = fgetc(fOut);
		//统计字符出现的个数
		while(!feof(fOut))
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
		FILE* fIn = fopen(CompressFileName.c_str(), "wb");
		assert(fIn);

		char c = 0; //一个字节（8位）的内容
		int num = 0; //当前字节的位数
		fseek(fOut, 0, SEEK_SET);
		ch = fgetc(fOut);
		while(!feof(fOut))
		{
			string& code = _infos[ch]._code;
			int size = code.size();
			for(int i=0; i<size; ++i)
			{
				c <<= 1;
				c |= code[i]-'0';
				num++;

				if(num == 8)
				{
					fputc(c, fIn);
					num = 0;
				}
			}

			ch = fgetc(fOut);
		}
		//未满8位用0补全
		if(num < 8)
		{
			c <<= (8-num);
		}
		fputc(c, fIn);

		//写配置文件
		string configFileName = fileName;
		configFileName += ".config";
		FILE* fInConfig = fopen(configFileName.c_str(), "wb");

		string line;
		char s_count[256];
		for(int i=0; i<256; ++i)
		{
			if(_infos[i]._count != 0)
			{
				line.push_back(_infos[i]._ch);
				line.push_back(',');
				itoa(_infos[i]._count, s_count, 10);
				line += s_count;
				line += '\n';

				fputs(line.c_str(), fInConfig);
				line.clear();
			}
		}

		fclose(fInConfig);
		fclose(fOut);
		fclose(fIn);

		return CompressFileName.c_str();
	}
	//解压
	const char* UnCompress(const char* fileName)
	{
		assert(fileName);

		string name= fileName;
		size_t index = name.rfind('.');
		string configFileName = name.substr(0, index);
		string uncompressFileName = configFileName + ".uncompress";
		configFileName += ".config";

		//将配置文件中的信息读入_infos中
		FILE* foutConfig = fopen(configFileName.c_str(), "rb");
		assert(foutConfig);
		string line;
		while(_ReadLine(foutConfig, line))
		{
			string count = line.substr(2, line.size()-2);
			_infos[(unsigned char)line[0]]._count = atoi(count.c_str());
			line.clear();
		}

		//重建HuffmanTree
		CharInfo invalid;
		HuffmanTree<CharInfo> h(_infos, 256, invalid);

		//解压缩
		FILE* fOut = fopen(fileName, "rb");
		FILE* fIn = fopen(uncompressFileName.c_str(), "wb");
		char ch = fgetc(fOut);
		int pos = 7;
		HuffmanTreeNode<CharInfo>* root = h.GetRoot();
		assert(root);
		HuffmanTreeNode<CharInfo>* cur = root;
		LongType total = root->_weight._count; //根节点的权重._count为字符出现总次数

		while(!feof(fOut))
		{
			while(pos >= 0 && total>0)
			{
				if((ch & (1<<pos)) == 0)
					cur = cur->_left;
				else
					cur = cur->_right;
				
				--pos;

				if(cur->_left==NULL && cur->_right==NULL)
				{
					fputc(cur->_weight._ch, fIn);
					cur = root;
					--total;
				}
			}
			pos = 7;
			ch = fgetc(fOut);
		}

		fclose(foutConfig);
		fclose(fIn);
		fclose(fOut);
		return uncompressFileName.c_str();
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

	bool _ReadLine(FILE* fout, string& line)
	{
		assert(fout);
		char c = fgetc(fout);
		if (c == EOF)
			return false;

		line.push_back(c);
		c = fgetc(fout);
		while(c!=EOF && c!='\n')
		{
			line.push_back(c);
			c = fgetc(fout);
		}
		return true;
	}
protected:
	CharInfo _infos[256];
};
