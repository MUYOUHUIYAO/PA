#include "memory/cache.h"
#include "memory/cache2.h"

#include "burst.h"
#include "misc.h"
#include <stdlib.h>
#include <time.h>

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

void init_cache(){
	int i = 0;px=0;
	for(i = 0; i < CacheRow; i ++){
		cache[i].valid = false;
	}
	srand((unsigned)time(NULL));
}

bool shot(hwaddr_t addr, CacheBlock **cb){
	uint32_t tag = TAG(addr);
	uint32_t i;
	*cb = (CacheBlock *)GPADDR(addr);
	for(i = 0; i < ROWNUM; i ++, (*cb) += 1){
		if((*cb) -> valid == true && (*cb) -> tag == tag) return true;
	}
	cb = NULL;
	return false;
}

uint32_t RDNUM(){
	return  (uint32_t)(rand() % 8);
}

CacheBlock * CopyToCache(hwaddr_t addr){
	CacheBlock *cb = GPADDR(addr);
	int i = 0, index;
	hwaddr_t baddr = addr & 0xffffffc0;
	uint8_t data;
	// 如果cache未满
	for(i = 0; i < ROWNUM; i ++){
		if((cb + i) -> valid == false){
			index = i;
		}
	}
	if(i == ROWNUM) index = RDNUM();	//如果cache满，随机替换

	for(i = 0; i < CacheBlockSize; i++){
		//(cb + index) -> data[i] = dram_read(baddr + i,1);
		L2CacheReadByte(baddr + i,&data);
		(cb + index) -> data[i] = data;
	}
	(cb + index) -> tag = TAG(addr);
	(cb + index) -> valid = true;
	return cb + index;
}

void CacheWriteByte(hwaddr_t addr, uint8_t data){
	//printf("---------write addr = 0x%x, data = 0x%x\n",addr,data);
	CacheBlock* cb = NULL;
	uint32_t offset = ADDR(addr);
	if(shot(addr, &cb) == true){
		cb -> data[offset] = data;
		//dram_write(addr, 1, (uint32_t)data);
		L2CacheWriteByte(addr, data);
	}else{
		//dram_write(addr, 1, (uint32_t)data);
		L2CacheWriteByte(addr, data);
	}
}
/*
size_t CacheWrite(hwaddr_t addr, CacheBlock *cb, size_t len, uint32_t data){
	uint32_t offset = ADDR(addr);
	while(len && (offset + (4 - len) < CacheBlockSize)){
		CacheWriteByte(addr ++, cb, (data >> ((4 - (len--)) * 8)));
	}
	return len;
}*/

void CacheReadByte(hwaddr_t addr, uint8_t *data){
	CacheBlock *cb = NULL;
	uint32_t offset = ADDR(addr);
	if(shot(addr, &cb) == true){
		px+=2;
		*data = cb -> data[offset];
	}else{
		px+=200;
		cb = CopyToCache(addr);
		*data = cb -> data[offset];
	}
	//printf("---------read addr = 0x%x data = 0x%x\n", addr, *data);

}

/*
size_t CacheRead(hwaddr_t addr, CacheBlock *cb, size_t len, uint32_t *data){
	uint32_t offset = ADDR(addr);
	uint8_t temp;
	while(len && (offset +(4 - len) < CacheBlockSize)){
		CacheReadByte(addr ++, cb, &temp);
		*data = (*data) & (temp << ((4 - (len--)) * 8));
	}
	return len;
}
*/

