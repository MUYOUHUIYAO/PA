#include "memory/cache2.h"

#include "burst.h"
#include "misc.h"
#include <stdlib.h>
#include <time.h>

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

void init_L2cache(){
	int i = 0;
	L2px=0;
	for(i = 0; i < L2CacheRow; i ++){
		L2cache[i].valid = false;
		L2cache[i].dirty = false;
	}
	srand((unsigned)time(NULL));
}

bool L2shot(hwaddr_t addr, L2CacheBlock **cb){
	uint32_t tag = L2TAG(addr);
	uint32_t i;
	*cb = (L2CacheBlock *)L2GPADDR(addr);
	for(i = 0; i < L2ROWNUM; i ++, (*cb) += 1){
		if((*cb) -> valid == true && (*cb) -> tag == tag) return true;
	}
	cb = NULL;
	return false;
}

uint32_t L2RDNUM(){
	return  (uint32_t)(rand() % 16);
}

L2CacheBlock * L2CopyToCache(hwaddr_t addr){
	L2CacheBlock *cb = L2GPADDR(addr);
	int i = 0, index;
	hwaddr_t baddr;
	// 如果cache未满
	for(i = 0; i < L2ROWNUM; i ++){
		if((cb + i) -> valid == false){
			index = i;
		}
	}
	if(i == L2ROWNUM){
		 index = L2RDNUM();	//如果cache满，随机替换
		 if((cb + index) -> dirty == true){
		 	baddr = L2TAG2ADDR(((cb + index) -> tag), addr);
		 	//printf("tag = 0x%x, addr = 0x%x baddr = 0x%x\n",((cb + index) -> tag), addr, baddr );
		 	for(i = 0; i < L2CacheBlockSize; i++){
		 		dram_write(baddr + i, 1, (uint32_t)((cb + index) -> data[i]));
		 	}
		 }
	}

	baddr = addr & 0xffffffc0;
	for(i = 0; i < L2CacheBlockSize; i++){
		(cb + index) -> data[i] = dram_read(baddr + i,1);
	}
	(cb + index) -> tag = L2TAG(addr);
	(cb + index) -> valid = true;
	(cb + index) -> dirty = false;
	return cb + index;
}

void L2CacheWriteByte(hwaddr_t addr, uint8_t data){
	L2CacheBlock* cb = NULL;
	uint32_t offset = L2ADDR(addr);
	if(L2shot(addr, &cb) == true){
		cb -> data[offset] = data;
		cb -> dirty = true;
	}else{

		dram_write(addr, 1, (uint32_t)data);
		L2CopyToCache(addr);
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

void L2CacheReadByte(hwaddr_t addr, uint8_t *data){
	L2CacheBlock *cb = NULL;
	uint32_t offset = L2ADDR(addr);
	if(L2shot(addr, &cb) == true){
		L2px+=2;
		*data = cb -> data[offset];
	}else{
		L2px+=200;
		cb = L2CopyToCache(addr);
		*data = cb -> data[offset];
	}
	//printf("-----------------------read addr = 0x%x data = 0x%x\n", addr, *data);

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

