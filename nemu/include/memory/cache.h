#ifndef __CACHE_H__
#define __CACHE_H__

#include "common.h"

typedef struct CacheBlock{
	bool valid;
	uint32_t tag;		
	uint8_t data[64];
}CacheBlock;

#define CacheSize (64 << 10)
#define CacheBlockSize 64

#define CacheRow (CacheSize/CacheBlockSize)

CacheBlock cache[CacheRow];

#define ROWNUM 8

#define GROUPNUM (CacheRow/ROWNUM)

#define TAG(addr) ((addr >> 13) & 0x7ffff)	//标记位
#define INDEX(addr) ((addr >> 6) & 0x7f)	//索引位 7位
#define ADDR(addr) (addr & 0x3f)		//块内偏移 6位

#define GPADDR(addr) ((CacheBlock *)(&cache[0] + ROWNUM * INDEX(addr))) 

uint64_t px;

void init_cache();

bool shot(hwaddr_t , CacheBlock** );

void CacheWriteByte(hwaddr_t , uint8_t );
void CacheReadByte(hwaddr_t, uint8_t *);

#endif

