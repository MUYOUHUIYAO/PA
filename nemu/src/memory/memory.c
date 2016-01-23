#include "common.h"
#include "memory/cache.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	uint32_t data1, data2, _len;
	CacheBlock *cb = NULL;
	if(shot(addr, cb) == true){
		_len = CacheRead(addr, cb, len, &data1);
		if(_len){
			if(shot(addr + (len - _len), cb) == true){
				CacheRead(addr+ (len - _len), cb, _len, &data2);
			}else{
				data2 = dram_read(addr+ (len - _len), _len) & (~0u >> ((4 - _len) << 3));
			}
			return data2 & data1 & (~0u >> ((4 - len) << 3));;
		}else{
			return data1 & (~0u >> ((4 - _len) << 3));;
		}
	}else
		return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	uint32_t  _len;
	CacheBlock *cb = NULL;
	if(shot(addr,cb) == true){
		_len = CacheWrite(addr,cb,len, data);
		if(_len){
			if(shot(addr + len - _len, cb) == true){
				CacheWrite(addr + len - _len, cb, _len, data);
			}else{
				dram_write(addr + len - _len, _len, data);
			}
		}
	}else{
		dram_write(addr, len, data);
	}
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

