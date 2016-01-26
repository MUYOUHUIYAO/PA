#ifndef __CACHE2_H__
#define __CACHE2_H__

#include "common.h"

typedef struct L2CacheBlock{
	bool valid;
	bool dirty;
	uint32_t tag;		
	uint8_t data[64];
}L2CacheBlock;

#define L2CacheSize (4 << 20)
#define L2CacheBlockSize 64

#define L2CacheRow (L2CacheSize/L2CacheBlockSize)

L2CacheBlock L2cache[L2CacheRow];

#define L2ROWNUM 16

#define L2GROUPNUM (L2CacheRow/L2ROWNUM)

#define L2TAG1(addr) ((addr >> 22) & 0x3ff)	//区号
#define L2INDEX(addr) ((addr >> 10) & 0xfff)	//区内组号 12位
#define L2TAG2(addr) ((addr >> 6) & 0xf)		//组内块号 4位
#define L2ADDR(addr) (addr & 0x3f)		//块内偏移 6位

#define L2TAG(addr) (((L2TAG1(addr) << 4) | (L2TAG2(addr))) & (0x3fff) )

#define L2GPADDR(addr) ((L2CacheBlock *)(&L2cache[0] + L2ROWNUM * L2INDEX(addr))) 
#define L2TAG2ADDR(tag, addr) ((((tag & 0x3ff0) << 18) | ((tag & 0xf) << 6) | (addr & 0x3ffc00)) & 0xffffffc0)

uint64_t L2px;

void init_L2cache();

bool L2shot(hwaddr_t , L2CacheBlock** );

void L2CacheWriteByte(hwaddr_t , uint8_t );
void L2CacheReadByte(hwaddr_t, uint8_t *);

#endif

