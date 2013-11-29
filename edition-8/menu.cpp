#include <iostream>
#include <string>
#include <vector>
#include "data.h"
#include "function.h"

using namespace std;

extern vector<string> route;

void menu()
{
	string order;
	while (true)
	try{
		for (int i=0; i<route.size(); i++)
			cout<<route[i]<<"/";
		cin>>order;

		if (order == "exit") break;
		else if (order == "new_file") exe_new_file();
		else if (order == "new_folder") exe_new_folder();
		else if (order == "open") exe_open();
		else if (order == "delete") exe_delete();
		else if (order == "cd") exe_cd();
		else if (order == "ls") exe_ls();
		else if (order == "help") exe_help();
		else if (order == "format") format();
		else {
			cout<<"输入命令有误，重新输入！"<<endl;
		}
	}catch (string exc)
	{
		cout<<exc<<endl;
	}
}
