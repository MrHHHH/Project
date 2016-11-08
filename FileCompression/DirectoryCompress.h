#pragma once
#include "FileCompress.h"
#include <vector>
#include <string>
#include <io.h>
#include <direct.h>

class DirectionCompress
{
public:
	const string Compress(const string& path)
	{
		_getFiles(path);//��ȡ�ļ���·���µ������ļ���������_filename�С�
		//
		////�����ϼ�·��
		//string prevPath(path);
		//size_t ret = prevPath.rfind("\\");
		//prevPath = prevPath.substr(0, ret);

		//�����ļ���
		string comDirName = path + ".compress";
		if (_mkdir(comDirName.c_str()) == 0)
		{
			size_t size = _filename.size();
			for (size_t i = 0; i < size; ++i)
			{
				FileCompress f;
				const string oldName = f.Compress(_filename[i].c_str());

				//��ѹ���������ļ��������ļ�����ѹ��Ŀ¼��
				//1.�ƶ�ѹ���ļ�
				string newName(comDirName);
				newName += (oldName.substr(oldName.rfind("\\")));

				int ret1 = rename(oldName.c_str(), newName.c_str());
				if (ret1 != 0)
				{
					_filename.clear();
					perror("�ļ��ƶ���ѹ���ļ�����ʱ����");
					return comDirName;

				}
				//2.�ƶ������ļ�
				string oldConfigName = _filename[i] + ".config";
				string newConfigName = newName.substr(0, newName.rfind(".")) + ".config";

				int ret2 = rename(oldConfigName.c_str(), newConfigName.c_str());
				if (ret2 != 0)
				{
					_filename.clear();
					perror("�ļ��ƶ���ѹ���ļ�����ʱ����");
					return comDirName;
				}
			}
		}
		else
		{
			 _filename.clear();
			 perror("����ѹ���ļ���ʧ��");
			 return comDirName;
		}
		_filename.clear();
		return comDirName;
	}

	const string Uncompress(const string& path)
	{
		_getFiles(path);

		//�����ļ���
		string uncomDirName = path.substr(0, path.rfind('.')) + ".uncompress";
		if (_mkdir(uncomDirName.c_str()) == 0)
		{
			size_t size = _filename.size();
			for (size_t i = 0; i < size; ++i)
			{
				string name = _filename[i];
				if (strcmp(name.substr(name.rfind('.')).c_str() , ".config") != 0)
				{
					FileCompress f;
					const string oldName = f.UnCompress(_filename[i].c_str());

					string newName(uncomDirName);
					newName += (oldName.substr(oldName.rfind("\\")));

					int ret = rename(oldName.c_str(), newName.c_str());
					if (ret != 0)
					{
						perror("�ļ��ƶ�����ѹ���ļ�����ʱ����");
						_filename.clear();
						return uncomDirName;
					}
				}
			}
		}
		else
		{
			perror("����ѹ���ļ���ʧ��");
			_filename.clear();
			return uncomDirName;
		}
		_filename.clear();
		return uncomDirName;
	}
protected:
	void _getFiles(const string& path)
	{
		
		long hFile = 0;//�ļ����
		struct _finddata_t fileinfo;//����ṹ���������洢�ļ�������Ϣ��
		string p(path);
		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
		{
			do
			{
				if ((fileinfo.attrib &  _A_SUBDIR))
				{
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					{
						DirectionCompress subDir;
						subDir.Compress(p.assign(path).append("\\").append(fileinfo.name).c_str());
					}
				}
				else
				{
					_filename.push_back(p.assign(path).append("\\").append(fileinfo.name));
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
	}

protected:
	vector<string> _filename;//����ļ���������ļ���
};