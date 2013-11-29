#pragma once

#include "data.h"
using namespace std;

void menu();


void exe_new_file();
void exe_new_folder();
void exe_open();
void exe_delete();
void exe_cd();
void exe_ls();
void exe_help();



//type=1, only read flag and name
//type=2, read folder
//type=3, read file info
//type=4, read file
data read_disk(int sec_id, int type);
void write_disk(data &);

data read_cache(int sec_id, int type);
void write_cache(data &);
void check_cache_finally();

void init_read_disk_info();

int read_sector_num();

int find_empty_sector();


void format();