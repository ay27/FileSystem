#include <string>
#include "data.h"
#include "const.h"
#include "function.h"

using namespace std;

extern vector<disk_data> info;

void init_read_disk_info()
{
	info.clear();

	int num = read_sector_num();
	data temp;

	for (int i=0; i<num; i++)
	{
		temp = read_disk(i, READ_FLAG);
		info.push_back(disk_data(temp.flag, temp.name));
	}

}

int find_empty_sector()
{
	for (int i=0; i<info.size(); i++)
	{
		if (info[i].flag == 0)
			return i;
	}
	throw((string)"´ÅÅÌÒÑÂú£¡");
}
