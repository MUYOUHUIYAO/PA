#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <elf.h>

extern char *strtab;
extern Elf32_Sym *symtab;
extern int nr_symtab_entry;

typedef struct
{
	swaddr_t prev_ebp;	//上一个%ebp地址
	swaddr_t ret_addr;	//函数返回地址	
	uint32_t args[4];		//函数参数
}PartOfStackFrame;

#define StackFrame_size (sizeof(PartOfStackFrame))

void cpu_exec(uint32_t);

/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_x(char *args);

static int cmd_p(char *args);

static int cmd_w(char *args);

static int cmd_d(char *args);

static int cmd_bt(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si","让程序单步执行N个指令后暂停，N缺省值为1",cmd_si},
	{ "info","info r打印寄存器状态，info w打印监视点状态",cmd_info},
	{"x","x N EXPR求出表达式EXPR的值，将结果作为起始内存地址，求十六进制输出的连续N个4字节",cmd_x},
	{"p","求表达式EXPR的值",cmd_p},
	{"w","w EXPR 当表达式值发生变化时终止程序运行",cmd_w},
	{"d","d N 删除序号为N的监视点",cmd_d},
	{"bt","打印栈帧链",cmd_bt}
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

static int cmd_si(char *args){
	uint32_t n=1;
	int args_len=args?strlen(args):0;
	int i;
	for(i=0;i<args_len;i++){
		if(!(args[i]>='0'&&args[i]<='9')){
			printf("输入格式错误\n");
			return 0;
		}
	}
	if(args_len) n=atoi(args);
	cpu_exec(n);
	return 0;
}

static int cmd_info(char *args){
	if(args == NULL) return 0;
	if(!strcmp("r",args)){
		printf("eax\t\t0x%X\t\t%u\n",cpu.eax,cpu.eax);
		printf("ebx\t\t0x%X\t\t%u\n",cpu.ebx,cpu.ebx);
		printf("ecx\t\t0x%X\t\t%u\n",cpu.ecx,cpu.ecx);
		printf("edx\t\t0x%X\t\t%u\n",cpu.edx,cpu.edx);
		printf("esp\t\t0x%X\t\t0x%X\n",cpu.esp,cpu.esp);
		printf("ebp\t\t0x%X\t\t0x%X\n",cpu.ebp,cpu.ebp);
		printf("esi\t\t0x%X\t\t%u\n",cpu.esi,cpu.esi);
		printf("edi\t\t0x%X\t\t%u\n",cpu.edi,cpu.edi);
		printf("eip\t\t0x%X\t\t0x%X\n",cpu.eip,cpu.eip);
		printf("EFLAGS\t\t0x%.8x\n",cpu.EFLAGS);
	}else if(!strcmp("w",args)){
		WP *tail=getHead();
		if(tail==NULL){
			printf("监视点列表为空\n");
		}
		while(tail){
			printf("监视点%d: %s\tresult=%d\n",tail->NO,tail->expr,tail->result);
			tail=tail->next;
		}
	}else{
		int i=0;
		for(;i<nr_symtab_entry;i++){
			printf("%u\t%x\t%d\t",symtab[i].st_name,symtab[i].st_value, symtab[i].st_info);
			printf("%s\n",&strtab[symtab[i].st_name] );
		}
		return 0;
	}
	return 0;
}

static int cmd_x(char *args){
		size_t n=1;
		char *addr,*num_str;
		swaddr_t _addr;
		int i=0;
		bool f;
		
		num_str=strtok(args," ");
		if(num_str==NULL) return 0;
		addr=strtok(NULL," ");
		if(addr==NULL) return 0;
		
		n=atoi(num_str);
		_addr=expr(addr,&f);
		if(f==false) return 0;
		//sscanf(addr,"0x%X",&_addr);
		printf("0x%x:\t",_addr);
		for(i=0;i<n;i++){
			printf("0x%.8x\t",swaddr_read(_addr,4));
			_addr+=4;
		}
		printf("\n");
		return 0;
}

static int cmd_p(char *args){
		if(args==NULL) return 0;
		uint32_t result;
		bool f;
		result=expr(args,&f);
		if(f==false) return 0;
		printf("0x%x\n",result);
		return 0;
}

static int cmd_w(char *args){
	if(args==NULL) return 0;
	uint32_t result;
	bool f;
	WP *r;
	result=expr(args,&f);
	if(f==false) return 0;
	r=new_wp(args,result);
	if(r) printf("成功创建序号为%d的监视点\n",r->NO);
	return 0;
}

static int cmd_d(char *args){
	if(args==NULL) return 0;
	int n=atoi(args);
	if(n<1||n>32) return 0;
	if(free_wp(n))
		printf("成功删除序号为%d的监视点\n",n);
	return 0;
	
}

static int cmd_bt(char *args){
	if(args !=NULL) {
		printf("输入格式错误\n");
		return 0;
	}
	if(cpu.ebp == 0) {
		printf("No stack\n");
		return 0;
	}
	int len = 0 ;
	swaddr_t ebp = cpu.ebp, prev_ebp;
	while(ebp) {
		prev_ebp=swaddr_read(ebp, 4);
		ebp= prev_ebp;
		len++;
	}

	PartOfStackFrame *func_stack;
	ebp = cpu.ebp, prev_ebp;
	int i=0, k=0;

	func_stack = (PartOfStackFrame *)malloc(len*StackFrame_size);
	for(k=0; k<len;k++){
		func_stack[k].prev_ebp = swaddr_read(ebp, 4);
		func_stack[k].ret_addr= swaddr_read(ebp -= 4, 4);
		for(i = 0; i<4 &&ebp; i++){
			func_stack[k].args[i] = swaddr_read(ebp -= 4, 4);
		}
		ebp = func_stack[k].prev_ebp;
	}

	for(k=0; k<len; k++){
		PartOfStackFrame s = func_stack[k];
		for(i=0; i< nr_symtab_entry; i ++){
			if(symtab[i].st_info == STT_FUNC && symtab[i].st_value == s.ret_addr){
				printf("STT_FUNC = %d, %d\n",STT_FUNC,  i);
				break;
			}
		}
		if(k == 0)
			printf("#%d \tin %x \n",k, s.ret_addr);
		else
			printf("#%d \t0x%x in %s\n", k, s.ret_addr, strtab+symtab[i].st_name );
	}

	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " "); //指向命令
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;	//指向命令参数
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
