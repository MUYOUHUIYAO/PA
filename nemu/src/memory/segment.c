#include "nemu.h"
#include "../../lib-common/x86-inc/mmu.h"

typedef struct data64
{
	uint32_t low;
	uint32_t high;
}data;

static void check_segReg(){
	if(current_sreg.val == cpu.CS.val){
		printf("current_sreg is CS");
	}else if(current_sreg.val == cpu.DS.val){
		printf("current_sreg is DS" );
	}else if(current_sreg.val == cpu.SS.val){
		printf("current_sreg is SS" );
	}else if(current_sreg.val == cpu.ES.val){
		printf("current_sreg is ES" );
	}

	printf("\t PE = %d\n", cpu.cr0.protect_enable );
}

lnaddr_t seg_translate(swaddr_t addr, size_t len, SELECTOR selector){
	check_segReg();
	if(cpu.cr0.protect_enable == 0) return addr;
	printf("cpu.cr0.protect_enable = %u\n", cpu.cr0.protect_enable);

	uint32_t segdesc_addr = cpu.GDTR.base;
	//uint16_t segdesc_limit = cpu.GDTR.limit;
	data r;
	SegDesc *segdesc;
	uint16_t index = selector.index;

	r.low = lnaddr_read(segdesc_addr + 8 * index, 4);
	r.high = lnaddr_read(segdesc_addr + 8 * index + 4, 4);
	segdesc = (SegDesc *)(&r);

	uint32_t base = (segdesc -> base_31_24 << 24)| (segdesc -> base_23_16 << 16) | (segdesc -> base_15_0);
	uint32_t limit = ((segdesc -> limit_19_16 << 16) | (segdesc -> limit_15_0)) & 0xfffff;
	
	Assert(addr + len < limit, "segment out limit, 0x%x, %d, 0x%x", addr, len, limit);	
	return base + addr;
}