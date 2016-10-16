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
	LongType _count; //�ַ����ִ���
	string _code; //�ַ���Ӧ��huffman����


	//�������ʼ����char,int����Ĭ�����ͱ���ʼ��Ϊ���ֵ��string��������Ĭ�Ϲ��캯����ʼ��Ϊ�մ���
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
		//ͳ���ַ����ֵĸ���
		while(ch != EOF)
		{
			_infos[ch]._count++;
			ch = fgetc(fOut);
		}

		//����HuffmanTree
		CharInfo invalid;
		HuffmanTree<CharInfo> ht(_infos, 256, invalid);

		//����Huffman����
		string code = "";
		CreateHuffmanCode(ht.GetRoot(), code);

		//ѹ���ļ�
		string CompressFileName = fileName;
		CompressFileName += ".compress";
		FILE* fIn = fopen(CompressFileName.c_str(), "w");
		assert(fIn);

		char c = 0; //һ���ֽڣ�8λ��������
		int num = 0; //��ǰ�ֽڵ�λ��
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
		//δ��8λ��0��ȫ
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
