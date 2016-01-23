#include "common.h"
#include "burst.h"
#include "misc.h"

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

#define TAG(addr) ((addr >> 13) & 0x7ffff)
#define INDEX(addr) ((addr >> 6) & 0x7f)
#define ADDR(addr) (addr & 0x3f)

#define GPADDR(addr) (cache + ROWNUM * INDEX(addr)) 

void init_cache(){
	int i = 0;
	for(i = 0; i < CacheRow; i ++){
		cache[i].valid = false;
	}
}

bool shot(hwaddr_t addr, CacheBlock *cb){
	uint32_t tag = TAG(addr);
	uint32_t i;
	cb = GPADDR(addr);
	for(i = 0; i < ROWNUM; i ++){
		cb = cb + i;
		if(cb -> tag == tag) return true;
	}
	cb = NULL;
	return false;
}

void CacheWriteByte(hwaddr_t addr, CacheBlock *cb, uint8_t data){
	uint32_t offset = ADDR(addr);
	cb -> data[offset] = data;
}

size_t CacheWrite(hwaddr_t addr, CacheBlock *cb, size_t len, uint32_t data){
	uint32_t offset = ADDR(addr);
	while(len && (offset + len < CacheBlockSize)){
		CacheWriteByte(addr ++, cb, (data >> ((4 - (len--)) * 8)));
	}
	return len;
}

void CacheReadByte(hwaddr_t addr, CacheBlock *cb, uint8_t *data){
	uint32_t offset = ADDR(addr);
	*data = cb -> data[offset];
}

size_t CacheRead(hwaddr_t addr, CacheBlock *cb, size_t len, uint32_t *data){
	uint32_t offset = ADDR(addr);
	uint8_t temp;
	while(len && (offset + len < CacheBlockSize)){
		CacheReadByte(addr ++, cb, &temp);
		*data = (*data) & (temp << ((4 - len) * 8));
	}
	return len;
}


