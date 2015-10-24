#ifndef __REG_H__
#define __REG_H__

#include "common.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {
	union{
		union {
			uint32_t _32;
			uint16_t _16;
			uint8_t _8[2];
		} gpr[8];

		/* Do NOT change the order of the GPRs' definitions. */

		struct{
			uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
		};
	};

	swaddr_t eip;

	uint32_t EFLAGS;	//32位标志寄存器

} CPU_state;

extern CPU_state cpu;

enum   {E_CF, E_PF, E_ZF, E_SF, E_IF, E_DF, E_OF, E_AF };
extern const uint32_t set_EFLAGS_num[];
extern const uint32_t unset_EFLAGS_num[];

#define CF (cpu.EFLAGS & 0x00000001)
#define PF ((cpu.EFLAGS & 0x00000004)>>2)
#define ZF ((cpu.EFLAGS & 0x00000040)>>6)
#define SF ((cpu.EFLAGS & 0x00000080)>>7)
#define IF  ((cpu.EFLAGS & 0x00000200)>>9)
#define DF ((cpu.EFLAGS & 0x00000400)>>10)
#define OF ((cpu.EFLAGS & 0x00000800)>>11)
#define AF ((cpu.EFLAGS & 0x00000010)>>4)

static inline int check_EFLAGS_index(int index){
	assert(index >=0 && index < 8);
	return index;
}

#define set_EFLAGS(index) (cpu.EFLAGS = (cpu.EFLAGS | set_EFLAGS_num[index]))
#define unset_EFLAGS(index) (cpu.EFLAGS =(cpu.EFLAGS & unset_EFLAGS_num[index]))

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

#endif
