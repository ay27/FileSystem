#include <fstream>
#include <string>
#include <iostream>
#include "function.h"
#include "data.h"
#include "const.h"
using namespace std;

extern fstream disk;
extern vector<disk_data> info;

int read_flag(data &);
void read_name(data &);
void read_folder(data &);
void read_file_info(data &);
void read_file(data &);
int read_some(int num);
int int_expected(char *);

int read_sector_num()
{
	disk.seekg(ios::beg);

	return read_some(DISK_INFO);

}


data read_disk(int sec_id, int type)
{
	disk.seekg(ios::beg);
	disk.seekg((DISK_INFO+(sec_id)*SECTOR_SIZE)*BYTE);

	data temp;
	temp.sec_id = sec_id;

	int flag = read_flag(temp);

	if (type == READ_FLAG) return temp;

	if (flag==FLAG_FOLDER && type==READ_FOLDER)
		read_folder(temp);

	if (flag==FLAG_FILE && type==READ_FILE_INFO)
		read_file_info(temp);

	if (flag==FLAG_FILE && type==READ_FILE)
		read_file(temp);

	return temp;
}



int read_flag(data &temp)
{
	int flag = read_some(FLAG);
	temp.flag = flag;

	if (flag == 0) return flag;

	int next = read_some(NEXT);
	temp.next = next;

	read_name(temp);
	
	return flag;
}

void read_folder(data &temp)
{

	int son_sum = read_some(SON_SUM);
	temp.son_sum = son_sum;

	int cur;
	int next = temp.next;
	int sec_id = temp.sec_id;
	temp.son.clear();
	for (int i=0; i<son_sum; i++)
	{
		cur = disk.tellg();
		if (cur-((DISK_INFO+(sec_id)*SECTOR_SIZE)*BYTE)>(SECTOR_SIZE-SON)*BYTE)
		{
			sec_id = next;
			disk.seekg(((DISK_INFO+(next)*SECTOR_SIZE)*BYTE)-cur);
			read_some(FLAG);
			next = read_some(NEXT);
		}
		temp.son.push_back(read_some(SON));
	}
}

void read_file_info(data &temp)
{

	temp.size = read_some(FILE_SIZE);

}

void read_file(data &temp)
{
	read_file_info(temp);

	char p[BYTE+1]={0};
	disk.read(p, BYTE);
	int x, cur;
	int next = temp.next;
	int sec_id = temp.sec_id;
	temp.file.clear();

	while ((x=int_expected(p)) != END_BY_ZERO)
	{
		temp.file += x;
		
		cur = disk.tellg();
		if (cur-((DISK_INFO+(sec_id)*SECTOR_SIZE)*BYTE)>(SECTOR_SIZE-FILE_CHAR)*BYTE)
		{
			sec_id = next;
			disk.seekp(ios::beg);
			disk.seekp((DISK_INFO+sec_id*SECTOR_SIZE)*BYTE);
			read_some(FLAG);
			next = read_some(NEXT);
		}

		disk.read(p, BYTE);
	}
}




void read_name(data &temp)
{
	char p[BYTE+1]={0};
	disk.read(p, BYTE);
	int x;
	string name;
	while ((x=int_expected(p)) != END_BY_ZERO)
	{
		name += x;
		disk.read(p, BYTE);
	}

	temp.name = name;
}



int read_some(int num)
{
	char p[BYTE+1]={0};
	int x=0, e=1;
	for (int i=0; i<num; i++)
	{
		disk.read(p, BYTE);
		x += int_expected(p)*e;
		e *=256;
	}
	return x;
}


int int_expected(char *p)
{
	int x=0, e=1;
	for (int i=0; i<BYTE; i++)
	{
		x += (p[i]-'0') * e;
		e*=2;
	}

	return x;
}




void write_flag(data &);
void write_folder(data &);
void write_file(data &);
char* char_expected(int x, int size);
void write_name(string &);

void write_disk(data &temp)
{
	disk.seekp(ios::beg);
	disk.seekp((DISK_INFO+(temp.sec_id)*SECTOR_SIZE)*BYTE);

	write_flag(temp);

	if (temp.flag == 1)
		write_folder(temp);
	else if (temp.flag == 2)
		write_file(temp);
}

void write_flag(data &temp)
{
	char *p;

	info[temp.sec_id] = disk_data(temp.flag, temp.name);

	p = char_expected(temp.flag, FLAG);
	disk.write(p, FLAG*BYTE);

	int next;
	if (temp.next == -2 || temp.next==0)
	{
		if (temp.son_sum/4 > (SECTOR_SIZE-FLAG-NEXT-temp.name.size())
			|| temp.file.size()>(SECTOR_SIZE-FLAG-NEXT-temp.name.size()))
			{
				next = find_empty_sector();
				info[next] = disk_data(temp.flag, "");
			}
		else next = -1;
		temp.next = next;
	}
	else next = temp.next;

	p = char_expected(next, NEXT);
	disk.write(p, NEXT*BYTE);

}

void write_folder(data &temp)
{
	write_name(temp.name);

	char *p;

	p = char_expected(temp.son_sum, SON_SUM);
	disk.write(p, SON_SUM*BYTE);

	int sec_id = temp.sec_id;
	int next = find_empty_sector();
	int cur;

	for (int i=0; i<temp.son.size(); i++)
	{
		p = char_expected(temp.son[i], SON);
		disk.write(p, SON*BYTE);
		
		cur = disk.tellp();
		if (cur-(DISK_INFO+(sec_id)*SECTOR_SIZE)*BYTE>(SECTOR_SIZE-SON)*BYTE)
		{
			sec_id = next;
			if (temp.son.size()-i>(SECTOR_SIZE-FLAG-NEXT))
				next = find_empty_sector();
			else next = -1;
			disk.seekp(ios::beg);
			disk.seekp((DISK_INFO+sec_id*SECTOR_SIZE)*BYTE);
			write_flag(data(sec_id, next, temp.flag));
		}
	}
}

void write_file(data &temp)
{
	write_name(temp.name);

	char *p;

	p = char_expected(temp.size, FILE_SIZE);
	disk.write(p, BYTE*FILE_SIZE);

	int sec_id = temp.sec_id;
	int next = temp.next;
	int cur, kk;

	for (int i=0; i<temp.file.size(); i++)
	{
		p = char_expected(temp.file[i], FILE_CHAR);
		disk.write(p, FILE_CHAR*BYTE);

		cur = disk.tellp();
		if (cur-(DISK_INFO+(sec_id)*SECTOR_SIZE)*BYTE>(SECTOR_SIZE-FILE_CHAR)*BYTE)
		{
			kk = sec_id;
			sec_id = next;
			if (temp.file.size()-i>SECTOR_SIZE-FLAG-NEXT)
				next = find_empty_sector();
			else next = -1;
			disk.seekp(ios::beg);
			disk.seekp((DISK_INFO+sec_id*SECTOR_SIZE)*BYTE);
			write_flag(data(sec_id, next, temp.flag));
		}
	}
	disk.write(char_expected(END_BY_ZERO, FILE_CHAR), FILE_CHAR*BYTE);
}

char* char_expected(int x, int size)
{
	static string p;
	p.clear();

	while (x>0)
	{
		p+= (x%2 + '0');
		x /=2;
	}
	while (p.size()<size*BYTE)
		p+='0';
	p+='\0';

	static char *tt=(char *) malloc(size*BYTE+1);
	
	tt = const_cast<char*>(p.data());
	
	return tt;

}

void write_name(string &str)
{
	char *p;

	for (int i=0; i<str.size(); i++)
	{
		p = char_expected(str[i], NAME_CHAR);
		disk.write(p, NAME_CHAR*BYTE);
	}
	disk.write(char_expected(END_BY_ZERO, NAME_CHAR), NAME_CHAR*BYTE);
}
