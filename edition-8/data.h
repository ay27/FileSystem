#pragma once

#include <vector>
#include <string>

using namespace std;


struct data
{
	int sec_id;
	int next;

	int flag;
	//flag=0, nothing
	//flag=1, folder
	//flag=2, file

	string name;

	//folder
	int son_sum;
	vector<int> son;

	//file
	int size;
	string file;

	//type=1, only read flag
	//type=2, read folder
	//type=3, read file info
	//type=4, read file
	data()
	{
		sec_id=flag=-1;
		son_sum = size = 0;
		next = -2;
		name.clear();
		son.clear();
		file.clear();
	}

	data(int sec_id, int next)
		:sec_id(sec_id), next(next)
	{
		name.clear();
		son_sum = 0;
		son.clear();
		size=0;
		file.clear();
	}

	data(int sec_id, int next, int flag)
		:sec_id(sec_id), next(next), flag(flag)
	{
		name.clear();
		son_sum = 0;
		son.clear();
		size = 0;
		file.clear();
	}

	//new file
	data(int sec_id, int next, string name, int size, string file)
		:sec_id(sec_id), next(next), name(name), size(size), file(file)
	{
		flag = 2;
		son_sum=0;
		son.clear();
	}

	//new folder
	data(int sec_id, int next, string name, int son_sum)
		:sec_id(sec_id), next(next), name(name), son_sum(son_sum)
	{
		flag = 1;
		size=0;
		file.clear();
	}

};


struct disk_data
{
	int flag;
	string name;
	
	disk_data()
	{}

	disk_data(int flag, string name)
		:flag(flag), name(name)
	{}
};


struct open_data
{
	int sec_id;
	string name;

	open_data() {}
	open_data(int sec_id, string name)
		:sec_id(sec_id), name(name)
	{}
};
