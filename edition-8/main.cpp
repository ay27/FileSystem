#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stack>
#include "function.h"
#include "data.h"
#include "const.h"
using namespace std;


vector<data> cache;
vector<bool> cache_info;
vector<disk_data> info;
stack<open_data> tree;
vector<string> route;

fstream disk("disk.disk", ios::in | ios::out);

int main()
{
	freopen("d1.in","r",stdin);

	tree.push(open_data(0, "/"));
	route.push_back("/");
	cache.clear();
	cache_info.clear();

	init_read_disk_info();

	menu();
	//data temp = read_disk(0, READ_FOLDER);

	check_cache_finally();

	disk.close();

	return 0;
}
