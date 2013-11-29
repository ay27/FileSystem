#pragma once


#define  BYTE                 8


#define  DISK_INFO            4


#define  SECTOR_SIZE          512
#define  FLAG                 1
#define  NEXT                 4
#define  NAME_CHAR            1
#define  SON_SUM              4
#define  SON                  4
#define  FILE_SIZE            4
#define  FILE_CHAR            1

#define  END_BY_ZERO          128


#define  READ                 1
#define  WRITE                0

#define  CACHE_BOUND          32


//type=1, only read flag and name
//type=2, read folder
//type=3, read file info
//type=4, read file
#define  READ_FLAG            1
#define  READ_FOLDER          2
#define  READ_FILE_INFO       3
#define  READ_FILE            4


	//flag=0, nothing
	//flag=1, folder
	//flag=2, file
#define  FLAG_NOTHING         0
#define  FLAG_FOLDER          1
#define  FLAG_FILE            2
