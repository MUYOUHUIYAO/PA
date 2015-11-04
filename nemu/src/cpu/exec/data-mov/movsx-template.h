#include "cpu/exec/template-start.h"
#define instr movsx

static void do_execute (){
	printf("%d\t%d\t%d\n",DATA_BYTE,op_src->size,op_dest->size );
	if(DATA_BYTE == 1){
		if(op_dest->size == 2){
			int16_t signExtend = (int16_t)((int8_t) op_src->val);
			OPERAND_W(op_dest , signExtend);
		}else {
			int32_t signExtend = (int32_t)((int8_t) op_src->val);
			OPERAND_W(op_dest , signExtend);
		}
	}else if(DATA_BYTE == 2){
		int32_t signExtend = (int32_t)((int16_t) op_src->val);
		OPERAND_W(op_dest , signExtend);
	}
	/*printf("%d\t%d\t%d\n",DATA_BYTE,op_src->size,op_dest->size );
	if(DATA_BYTE ==2){
		int16_t signExtend = (int16_t)((int8_t) op_src->val);
		OPERAND_W(op_dest , signExtend);
	}else{
		if(op_src->size == 1){
			int32_t signExtend = (int32_t)((int16_t) op_src->val);
			OPERAND_W(op_dest , signExtend);
		}else{
			int32_t signExtend = (int32_t)((int8_t) op_src->val);
			OPERAND_W(op_dest , signExtend);
		}
	}*/
	
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"