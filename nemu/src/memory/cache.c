#include "cache.h"

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
	while(len && (offset + (4 - len) < CacheBlockSize)){
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
	while(len && (offset +(4 - len) < CacheBlockSize)){
		CacheReadByte(addr ++, cb, &temp);
		*data = (*data) & (temp << ((4 - (len--)) * 8));
	}
	return len;
}


