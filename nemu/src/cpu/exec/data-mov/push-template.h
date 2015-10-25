#include "cpu/exec/template-start.h"
#define instr push

static void do_execute (){
	if(DATA_BYTE == 2){
		cpu.esp = cpu.esp-2;
		
	}
	else if(DATA_BYTE == 4){

	}
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"