#include "cpu/exec/helper.h"



make_helper(cmps) {
	int DATA_BYTE = 0, IncDec=0;
	uint32_t result = 0;
	uint32_t src = swaddr_read(reg_l(R_ESI), DATA_BYTE);
	uint32_t dest = swaddr_read(reg_l(R_EDI), DATA_BYTE);

	if(ops_decoded.opcode == 0xa4) DATA_BYTE = 1;
	else if(ops_decoded.opcode == 0xa5 && ops_decoded.is_data_size_16 == 0){
		DATA_BYTE = 4;
	}else DATA_BYTE = 2;

	IncDec = (DF == 0) ? DATA_BYTE : -DATA_BYTE;

	result =  src - dest;

	reg_l(R_EDI) += IncDec;
	reg_l(R_ESI) += IncDec;

	if(result == 0) set_EFLAGS(E_ZF);
	else unset_EFLAGS(E_ZF);

	if((uint32_t)(dest) < (uint32_t)(src)) set_EFLAGS(E_CF);
	else unset_EFLAGS(E_CF);

	uint8_t lowByte = result & 0xff;
	if((! (lowByte & 0x01)) ^ (! (lowByte & 0x02) )^ (! (lowByte & 0x04)) ^ (! (lowByte & 0x08) ) ^ (! (lowByte & 0x10)) 
		^ (! (lowByte & 0x20) )^(! (lowByte & 0x40) )^ (! (lowByte & 0x80) )){
		unset_EFLAGS(E_PF);
	}else set_EFLAGS(E_PF);

	if(DATA_BYTE == 1) {
		if( (dest & 0x0f) < (src & 0x0f)) set_EFLAGS(E_AF);
	}else unset_EFLAGS(E_AF);

	if(DATA_BYTE == 1 && (result & 0x80) ) set_EFLAGS(E_SF);
	else if(DATA_BYTE == 2 && (result & 0x8000)) set_EFLAGS(E_SF);
	else if(DATA_BYTE == 4 && (result & 0x80000000)) set_EFLAGS(E_SF);
	else unset_EFLAGS(E_SF);

	//if(result + src != dest) set_EFLAGS(E_OF);
	//else unset_EFLAGS(E_OF);

	if(((1 << (8 * DATA_BYTE - 1) )&dest) ==0 && ((1 << (8 * DATA_BYTE - 1) )&src) == 1 && ((1 << (8 * DATA_BYTE - 1) )&result) == 1) set_EFLAGS(E_OF);
	else if(((1 << (8 * DATA_BYTE - 1) )&dest) == 1 && ((1 << (8 * DATA_BYTE - 1) )&src) == 0 && ((1 << (8 * DATA_BYTE - 1) )&result) == 0 ) set_EFLAGS(E_OF);
	else unset_EFLAGS(E_OF);

#ifdef DEBUG
	print_asm_template1();
#endif
	
	return 1;
}

make_helper(cmps_b){
	return cmps(eip);
}

make_helper(cmps_v){
	return cmps(eip);
}