/*
 * littleFS.h
 *
 *  Created on: Nov 15, 2021
 *      Author: GPROTECH
 */

#ifndef LITTLEFS_H_
#define LITTLEFS_H_

#include "table.h"
#include "target.h"
#include "databuffer.h"

//flash size 16MB -> 8MB for storage -> one block 4096
#define MAX_SPACE2 			6291456//6M,///8388608//->8M //8000000
#define MAX_BYTEBLOCK 		4096
#define RESERVE_INBLOCK		256//for header / metadata any else use in the system
#define MAX_BYTEWRITEABLE	(MAX_BYTEBLOCK-RESERVE_INBLOCK)
#define MAX_WRITEBYTEBLOCK 	(MAX_BYTEWRITEABLE/2)//write in hex string
#define MIN_READWRITE		128
//#define MAX_METADATA 		9
#define DEFAULT_USEDBLOCK	2//by systems
#define MAX_BLOCK 			((MAX_SPACE2/MAX_BYTEBLOCK)-DEFAULT_USEDBLOCK) //2046
#define MAX_RECORDPERBLOCK	30
#define MAX_FS_DATABLOCK 	500//1000
#define MAX_FS_RECORD 		(MAX_FS_DATABLOCK*MAX_RECORDPERBLOCK) //30000, every block can store 30 records
//#define MAX_HIGHDATABLOCK 	100//50
//#define MAX_HIGHRECORD2 		(MAX_HIGHDATABLOCK*MAX_RECORDPERBLOCK) //1500
//#define MAX_BUFFER_LEN 		64
//#define MAX_TABLEBLOCK		100 //reserve for table.c used
//#define BAL_TABLEBLOCK		(100-MAX_SET_FILE) //balance

//#define MAX_BUFFERBLOCK		(MAX_LOWDATABLOCK+MAX_HIGHDATABLOCK)
//#define RESERVE_BLOCK		(MAX_BLOCK-MAX_TABLEBLOCK-MAX_BUFFERBLOCK)

//#define MAX_CHARPAGE_2	 	1400
//#define MAX_LOWRECORD_2		1000

//#define FILE_POINTERFS 			0



void initLittleFs(void);
//void checkfilesBackupSpiffs(void);
//T8U readBackupSpiffs(int title, int len, T8U *data);
//T8U writeBackupSpiffs(int title, int len, T8U *data);
//void checkfilesLittleFs(void);
//void readLittleFsBlock(int title, int len, T8U *data);
//void writeLittleFsBlock(int title, int len, T8U *data);
T8U readLittleFsPage(int title, int len, T8U *data);
T8U writeLittleFsPage(int title, int len, T8U *data);
T8U readLittleFsBuffer(char *name, T8U *data);
T8U writeLittleFsBuffer(char *name, T8U *data);
//void save_FS_pointer(void);
void Set_FS_MemoryFull(void);
void Clear_FS_Memory(void);
T16U Get_RecordFSCount(void);
void save_record_FS(const LOG_NAME *log_name, T8U * data);
void get_FSrecord(T8U * data, T8U otherRecord);
void flush_FS_record(T8U record);
//void spiffsToLittleFS(void);
void deletePageFS(char *name);
void formatLittleFs(void);
void endLittleFs(void);

#endif /* LITTLEFS_H_ */
