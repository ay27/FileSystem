#include <string>
#include <vector>
#include "data.h"
#include "const.h"
#include "function.h"
using namespace std;

extern vector<data> cache;
extern vector<bool> cache_info;
extern vector<disk_data> info;


void check_cache();


data read_cache(int sec_id, int type)
{
	for (int i=0; i<cache.size(); i++)
		if (cache[i].sec_id == sec_id)
			return cache[i];

	data temp = read_disk(sec_id, type);
	cache.push_back(temp);
	cache_info.push_back(READ);

	check_cache();

	return temp;
}


void write_cache(data &temp)
{
	info[temp.sec_id]= disk_data(temp.flag, temp.name);

	for (int i=0; i<cache.size(); i++)
		if (temp.sec_id == cache[i].sec_id)
		{
			cache[i] = temp;
			cache_info[i] = WRITE;
			return;
		}

	cache.push_back(temp);
	cache_info.push_back(WRITE);

	check_cache();
}


void check_cache(int bound)
{
	if (cache.size() >= bound)
	{
		int i=0;
		while (i<cache.size())
		{
			if (cache_info[i] == WRITE)
			{
				write_disk(cache[i]);
				cache.erase(cache.begin()+i);
				cache_info.erase(cache_info.begin()+i);
			}
			else i++;
		}
		while (cache.size() >= bound)
		{
			cache.erase(cache.begin());
			cache_info.erase(cache_info.begin());
		}
	}
}

void check_cache()
{
	check_cache(CACHE_BOUND);
}

void check_cache_finally()
{
	check_cache(1);
}