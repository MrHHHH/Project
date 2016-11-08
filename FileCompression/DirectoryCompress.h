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
		_getFiles(path);//获取文件夹路径下的所有文件名，放入_filename中。
		//
		////计算上级路径
		//string prevPath(path);
		//size_t ret = prevPath.rfind("\\");
		//prevPath = prevPath.substr(0, ret);

		//创建文件夹
		string comDirName = path + ".compress";
		if (_mkdir(comDirName.c_str()) == 0)
		{
			size_t size = _filename.size();
			for (size_t i = 0; i < size; ++i)
			{
				FileCompress f;
				const string oldName = f.Compress(_filename[i].c_str());

				//将压缩出来的文件和配置文件放在压缩目录下
				//1.移动压缩文件
				string newName(comDirName);
				newName += (oldName.substr(oldName.rfind("\\")));

				int ret1 = rename(oldName.c_str(), newName.c_str());
				if (ret1 != 0)
				{
					_filename.clear();
					perror("文件移动到压缩文件夹下时出错");
					return comDirName;

				}
				//2.移动配置文件
				string oldConfigName = _filename[i] + ".config";
				string newConfigName = newName.substr(0, newName.rfind(".")) + ".config";

				int ret2 = rename(oldConfigName.c_str(), newConfigName.c_str());
				if (ret2 != 0)
				{
					_filename.clear();
					perror("文件移动到压缩文件夹下时出错");
					return comDirName;
				}
			}
		}
		else
		{
			 _filename.clear();
			 perror("创建压缩文件夹失败");
			 return comDirName;
		}
		_filename.clear();
		return comDirName;
	}

	const string Uncompress(const string& path)
	{
		_getFiles(path);

		//创建文件夹
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
						perror("文件移动到解压缩文件夹下时出错");
						_filename.clear();
						return uncomDirName;
					}
				}
			}
		}
		else
		{
			perror("创建压缩文件夹失败");
			_filename.clear();
			return uncomDirName;
		}
		_filename.clear();
		return uncomDirName;
	}
protected:
	void _getFiles(const string& path)
	{
		
		long hFile = 0;//文件句柄
		struct _finddata_t fileinfo;//这个结构体是用来存储文件各种信息的
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
	vector<string> _filename;//存放文件夹里面的文件名
};