#include "nemu.h"
#include <stdlib.h>
#include <time.h>

CPU_state cpu;
SELECTOR current_sreg;
uint32_t SegDesc_index;



const uint32_t set_EFLAGS_num[] = {0x00000001, 0x00000004, 0x00000040, 0x00000080, 0x00000200, 0x00000400, 0x00000800, 0x00000010};
const uint32_t unset_EFLAGS_num[] ={0xfffffffe, 0xfffffffb, 0xffffffbf, 0xffffff7f, 0xfffffdff, 0xfffffbff, 0xfffff7ff, 0xffffffef} ;

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

void reg_test() {
	srand(time(0));
	uint32_t sample[8];
	uint32_t eip_sample = rand();
	cpu.eip = eip_sample;

	int i;
	for(i = R_EAX; i <= R_EDI; i ++) {
		sample[i] = rand();
		reg_l(i) = sample[i];
		assert(reg_w(i) == (sample[i] & 0xffff));
	}

	assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
	assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
	assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
	assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
	assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
	assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
	assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
	assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

	assert(sample[R_EAX] == cpu.eax);
	assert(sample[R_ECX] == cpu.ecx);
	assert(sample[R_EDX] == cpu.edx);
	assert(sample[R_EBX] == cpu.ebx);
	assert(sample[R_ESP] == cpu.esp);
	assert(sample[R_EBP] == cpu.ebp);
	assert(sample[R_ESI] == cpu.esi);
	assert(sample[R_EDI] == cpu.edi);

	assert(eip_sample == cpu.eip);
}

void insertSegDesc(uint32_t base, uint32_t limit){
	data *r;
	SegDesc sd;
	sd.limit_15_0 = limit & 0xffff;
	sd.limit_19_16 = (limit >> 16) & 0xf;
	sd.base_15_0 = base & 0xffff;
	sd.base_23_16 = (base >> 16) & 0xff;
	sd.base_31_24 = (base >> 24) & 0xff;

	r = (data *)(&sd);
	lnaddr_write(cpu.GDTR.base + SegDesc_index * 64, 4, r->low);	
	lnaddr_write(cpu.GDTR.base + SegDesc_index * 64 + 4, 4, r -> high);
}

