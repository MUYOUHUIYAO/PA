#include "cpu/exec/template-start.h"

#define instr jcc

static void do_execute(){
	uint8_t condition=0;
	uint8_t tbyte = 0;

	switch(ops_decoded.opcode){
		case 0x77: condition = (CF ==0 && ZF ==0);break;				//JA, JNBE
		case 0x73: condition = (CF ==0);break; 					//JAE, JNB, JNC
		case 0x72: condition = (CF == 1);break; 					//JB, JC, JNAE
		case 0x76: condition = (CF == 1 || ZF == 1);break;			//JBE, JNA
		case 0xE3: condition = (reg_w(R_CX) == 0 || reg_l(R_ECX) == 0); break;	//JCXZ
		case 0x74: condition = (ZF == 1); break; 					//JE, JZ, JZ
		case 0x7f: condition = (ZF == 0 && SF == OF); break;			//JG, JNLE
		case 0x7d: condition = (SF == OF); break;					//JGE, JNL
		case 0x7c: condition =(SF != OF); break;					//JL , JNGE
		case 0x7e: condition = (ZF == 1 ||SF != OF); break;			//JLE, JNG
		case 0x75: condition = (ZF == 0);break;					//JNE,JNZ
		case 0x71: condition = (OF == 0); break;					//JNO
		case 0x7b: condition = (PF == 0);break;					//JNP, JPO
		case 0x79: condition = (SF == 0);break;					//JNS
		case 0x70: condition = (OF == 1); break;					//JO
		case 0x7a: condition = (PF == 1); break;					//JP, JPE
		case 0x78: condition = (SF == 1); break;					//JS
		default: tbyte = 1;break;
	}

	switch(ops_decoded.opcode & 0x0ff){
		case 0x87: condition = (CF == 0 && ZF == 0); break;			//JA, JNBE
		case 0x83: condition = (CF == 0); break;					//JAE, JNB, JNC
		case 0x82: condition = (CF == 1); break;					//JB, JC, JNAE
		case 0x86: condition = (CF == 1 || ZF == 1); break;			//JBE, JNA
		case 0x84: condition = (ZF == 1); break;					//JE, JZ
		case 0x8f: condition = (ZF == 0 && SF == OF); break;			//JG, JNLE
		case 0x8d: condition = (SF == OF); break;					//JGE, JNL
		case 0x8c: condition = (SF !=OF); break;					//JL, JNGE
		case 0x8e: condition = (ZF == 1 || SF != OF); break;			//JLE, JNG
		case 0x85: condition = (ZF == 0); break;					//JNE, JNZ
		case 0x81: condition = (OF == 0); break;					//JNO
		case 0x8b: condition = (PF == 0); break;					//JNP, JPO
		case 0x89: condition = (SF == 0); break;					//JNS
		case 0x80: condition = (OF == 1); break;					//JO
		case 0x8a: condition = (PF == 1); break;					//JP, jpe
		case 0x88: condition = (SF == 1); break;					//JS

	}

	if(condition == 1){
		if(tbyte ==0) cpu.eip+= (int32_t)((int8_t) (op_src->val));
		else if(DATA_BYTE == 2){
			cpu.eip += (int32_t)((int16_t)op_src -> val);
		}
		else cpu.eip +=(int32_t)op_src -> val;
		if(DATA_BYTE == 2){
			cpu.eip = cpu.eip & 0x0000ffff;
		}	
	}
	/**update EFLAGS***/
	print_asm_template1();
}

make_instr_helper(i);

#include "cpu/exec/template-end.h"
