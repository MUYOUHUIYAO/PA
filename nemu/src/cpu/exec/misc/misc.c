#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(nop) {
	print_asm("nop");
	return 1;
}

make_helper(int3) {
	void do_int3();
	do_int3();
	print_asm("int3");

	return 1;
}

make_helper(lea) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	reg_l(m.reg) = op_src->addr;

	print_asm("leal %s,%%%s", op_src->str, regsl[m.reg]);
	return 1 + len;
}

make_helper(leave){
	cpu.esp = cpu.ebp; //stackaddrsize == 32

	//if(DATA_BYTE == 2){
	//	reg_w(R_BP) = swaddr_read(cpu.esp,2);
	//	cpu.esp += 2;
	//}else if(DATA_BYTE == 4){
		reg_l(R_EBP) = swaddr_read(cpu.esp, 4);
		cpu.esp += 4;
	//}
	print_asm("leave");
	return 1;
}

make_helper(ret){
	cpu.eip = swaddr_read(cpu.esp, 4);
	printf("%x\n",cpu.eip );
	cpu.esp += 4;
	print_asm("ret");
	int len= decode_i_l(cpu.eip);
	return len+1;
}