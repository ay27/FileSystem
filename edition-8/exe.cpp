#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>
#include "data.h"
#include "function.h"
#include "const.h"

using namespace std;

extern stack<open_data> tree;
extern vector<string> route;
extern vector<disk_data> info;

string get_name();
int find_name(string &);
void _delete(int sec_id, int type);
void delete_folder(int sec_id);
void delete_file(int sec_id);

void exe_new_file()
{
	string name = get_name();

	string temp;
	temp.clear();

	char ch;
	while ((ch=getchar()) != '~')
		temp += ch;

	int tt = find_empty_sector();

	data new_file = data(tt, -2, name, temp.size(), temp);

	data cur_folder = read_cache(tree.top().sec_id, READ_FOLDER);
	cur_folder.son.push_back(tt);
	cur_folder.son_sum++;
	_delete(cur_folder.sec_id, READ_FOLDER);

	write_cache(cur_folder);

	write_cache(new_file);

}

void exe_new_folder()
{
	string name = get_name();

	int sec_id = find_empty_sector();

	data new_folder=data(sec_id, -2, name, 0);

	data cur_folder = read_cache(tree.top().sec_id, READ_FOLDER);
	cur_folder.son.push_back(sec_id);
	cur_folder.son_sum++;
	_delete(cur_folder.sec_id, READ_FOLDER);
	write_cache(cur_folder);

	tree.push(open_data(sec_id, name));
	route.push_back(name);

	write_cache(new_folder);

}

void exe_open()
{
	string name = get_name();

	int sec_id = find_name(name);

	data new_open;
	if (info[sec_id].flag == 1)
	{
		new_open = read_cache(sec_id, READ_FOLDER);
		tree.push(open_data(sec_id, new_open.name));
		route.push_back(new_open.name);
	}
	else {
		new_open = read_cache(sec_id, READ_FILE);
		for (int i=0; i<new_open.file.size(); i++)
			putchar(new_open.file[i]);
		putchar('\n');
	}

}

void exe_delete()
{
	string name = get_name();
	int sec_id = find_name(name);

	data cur_folder = read_cache(tree.top().sec_id, READ_FOLDER);
	for (int i=0; i<cur_folder.son.size(); i++)
		if (name == info[cur_folder.son[i]].name)
		{
			cur_folder.son.erase(cur_folder.son.begin()+i);
			cur_folder.son_sum--;
			_delete(cur_folder.sec_id, READ_FOLDER);
			write_cache(cur_folder);
			break;
		}

	if (info[sec_id].flag == 1)
	{
		delete_folder(sec_id);

	}
	else delete_file(sec_id);

}

void delete_folder(int sec_id)
{
	data temp = read_cache(sec_id, READ_FOLDER);
	for (int i=0; i<temp.son_sum; i++)
		_delete(temp.son[i], READ_FILE_INFO);
	_delete(sec_id, READ_FOLDER);
}

void delete_file(int sec_id)
{
	_delete(sec_id, READ_FILE_INFO);
}

void _delete(int sec_id, int type)
{
	data temp = read_cache(sec_id, type);

	int next = temp.next;

	while (next!=-1 && next!=0 && next!=-2)
	{
		write_cache(data(sec_id, 0, 0));
		sec_id = next;
		 
		temp = read_cache(sec_id, READ_FLAG);
		next = temp.next;
	}

	write_cache(data(sec_id, 0, 0));

}

void exe_cd()
{
	string name = get_name();

	if (name == "..")
	{
		if (tree.size()>1)
		{
			tree.pop();
			route.erase(route.end()-1);
		}
		else throw((string)"�����ˣ�");
	}
	else {
		int sec_id = find_name(name);
		if (info[sec_id].flag != 1)
			throw((string)"����û�����Ŀ¼����");
		tree.push(open_data(sec_id, name));
		route.push_back(name);
	}
}


void exe_ls()
{
	int sec_id = tree.top().sec_id;

	data temp = read_cache(sec_id, READ_FOLDER);

	if (temp.son.size() == 0)
		throw((string)"��ǰĿ¼��û���ļ�Ŷ��");

	sort(temp.son.begin(), temp.son.end());

	for (int i=0; i<temp.son.size(); i++)
		cout<<"������"<<temp.son[i]<<"\t"<<info[temp.son[i]].name<<endl;

}

void exe_help()
{
	cout<<"�������棺"<<endl;
	cout<<"  new_file���ڵ�ǰĿ¼�½��ļ�"<<endl;
	cout<<"  new_folder���ڵ�ǰĿ¼�½�Ŀ¼"<<endl;
	cout<<"  open���ڵ�ǰĿ¼���ļ���Ŀ¼"<<endl;
	cout<<"  delete���ڵ�ǰĿ¼ɾ���ļ���Ŀ¼"<<endl;
	cout<<"  cd�����ĵ�ǰĿ¼����һ��Ϊ.."<<endl;
	cout<<"  ls����ӡ��ǰĿ¼�������ļ���Ŀ¼"<<endl;
	cout<<"  format����ʽ������"<<endl;
	cout<<"  exit���˳�"<<endl;
}

string get_name()
{
	string name;
	cin>>name;
	getchar();
	cin.clear();

	return name;
}

int find_name(string &name)
{
	int sec_id = -1;

	data temp = read_cache(tree.top().sec_id, READ_FOLDER);

	for (int i=0; i<temp.son.size(); i++)
		if (info[temp.son[i]].name == name)
		{
			sec_id = temp.son[i];
			break;
		}
	if (sec_id == -1) throw((string)"�޷��ҵ����ļ���Ŀ¼��");

	return sec_id;
}
