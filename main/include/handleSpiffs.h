
#ifndef HANDLESPIFFS_H_
#define HANDLESPIFFS_H_

#include "table.h"
#include "target.h"

#define MAX_SPACE 			2097152

#define MAX_BYTEPAGE 		256
#define MAX_WRITEBYTEPAGE 	(MAX_BYTEPAGE/2)//reserve half, possibility \0 and 0xFF need two space. Because cannot write \0.
#define MAX_RECORDPERPAGE	2

#define MAX_BYTEBUFFER 		128//2one page one record. see performance. 256 two record in one page
#define MAX_WRITEBYTEBUFFER	(MAX_BYTEBUFFER/2)//reserve half, possibility \0 and 0xFF need two space. Because cannot write \0.
//#define MAX_METADATA 		9
//#define MAX_PAGE 			7547
#define MAX_RECORDPERBUFFER	1//2
#define MAX_LOWDATABUFFER 	1000 //5000
#define MAX_LOWRECORD 		(MAX_LOWDATABUFFER*MAX_RECORDPERBUFFER) //2000
#define MAX_HIGHDATABUFFER	100
#define MAX_HIGHRECORD 		(MAX_HIGHDATABUFFER*MAX_RECORDPERBUFFER) //100
#define MAX_BUFFER_LEN 		64
#define MAX_TABLEPAGE		100 //reserve for table.c used
#define BAL_TABLEPAGE		(100-MAX_SET_FILE) //balance
//#define MAX_KEEPRECORDPAGE	1000
//#define MAX_KEEPRECORDBUFF	(1000*MAX_RECORDPERPAGE)//for set memory full -> last 2000

void initSpiffs(void);
void checkfiles(void);
//void readSpiffsPage(int title, int len, T8U *data);
T8U readSpiffsPage(int title, int len, T8U *data);
//void writeSpiffsPage(int title, int len, T8U *data);
T8U writeSpiffsPage(int title, int len, T8U *data);
//T8U readSpiffsBuffer(char *name, T8U *data);
//T8U readSpiffsBufferHexString(char *name, T8U *data);
//T8U writeSpiffsBuffer(char *name, T8U *data);
T8U readSpiffsBuffer2(char *name, T8U *data);
T8U writeSpiffsBuffer2(char *name, T8U *data);
void deletePage(char *name);
void endSpiffs(void);

#endif /* HANDLESPIFFS_H_ */
