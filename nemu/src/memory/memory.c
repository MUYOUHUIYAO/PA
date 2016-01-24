#include "common.h"
#include "memory/cache.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	uint32_t result= 0x0, data;
	int i = 0;
	size_t l = len;
	while(len){
		CacheReadByte(addr + i, (uint8_t *)(&data));
		result |= (data << i) & (0xff << i);
		++i;
		--len;
	}
	printf("wrong in hread addr = 0x%x\tlen = %d\tdata=0x%x\n", addr, l,result);
	return result;
	//return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	int i = 0;
	while(len){
		CacheWriteByte(addr + i, (uint8_t)(data>>i));
		++i;
		--len;
	}

	//dram_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_write(addr, len, data);
}

