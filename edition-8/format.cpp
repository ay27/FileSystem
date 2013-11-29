#include <fstream>
#include <string>
#include "function.h"
#include "const.h"
#include "data.h"

using namespace std;

extern fstream disk;
extern char* char_expected(int, int);

void format()
{
	disk.seekp(ios::beg);

	char *p;
	p=char_expected(256, DISK_INFO);
	disk.write(p, DISK_INFO*BYTE);

	const char *x="00000000\0";

	data temp(0, 0, "/", 0);
	write_disk(temp);

	disk.seekp(ios::beg);
	disk.seekp((DISK_INFO+SECTOR_SIZE)*BYTE);

	for (int i=0; i<255; i++)
	{
		for (int j=0; j<SECTOR_SIZE; j++)
			disk.write(x, BYTE);
	}

	init_read_disk_info();
}
