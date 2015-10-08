#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ,NEQ,AND,OR,NO,CITE,DEC_NUM,HEX_NUM,REG

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"-",'-'},						//minus
	{"/",'/'},						//divide
	{"\\*",'*'},			
	{"==",EQ},				//equal
	{"!=",NEQ},			//non-equal
	{"\\&{2}",AND},
	{"\\|{2}",OR},
	{"!",NO},
	{"\\(",'('},
	{")",')'},
	{"0x[0-9a-fA-F]+",HEX_NUM},			//十六进制整数
	{"[0-9]+",DEC_NUM},		//十进制整数
	{"\\$[a-zA-Z]{2,3}",REG}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

#define op_num (NR_REGEX-2)
static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret != 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

	/*upper数组按NOTYPE,  ‘+’, '-', '/', '*', EQ, '(', ')', '*'(引用)  顺序存放其优先级
	 * -1表示对应位置元素优先级低，0表示相同，1表示优先级高
	 */
int stage_table[][op_num]={
			{0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},		//NOTYPE
			{1, 1, 1,-1,-1,1,-1,1,-1,1,1,1,-1},				//'+'
			{1,1,1,-1,-1,1,-1,1,-1,1,1,1,-1},				//'-'
			{1,1,1,1,1,1,-1,1,-1,1,1,1,-1},					//'/'
			{1,1,1,1,1,1,-1,1,-1,1,1,1,-1},					//'*'(乘)
			{1,-1,-1,-1,-1,1,-1,1,-1,1,1,1,-1},				//EQ
			{1,-1,-1,-1,-1,-1,-1,0,-1, -1,-1,-1,-1},				//'('
			{1,1,1,1,1,1,65535,1,1,1,1,1,1},				//')'
			{1,1,1,1,1,1,-1,1,-1,1,1,1,-1},					//'*'(引用)
			{1,-1,-1,-1,-1,1,-1,1,-1,1,1,1,-1},					//NEQ
			{1,-1,-1,-1,-1,-1,-1,1,-1,-1,1,1,-1},			//AND
			{1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1,1,-1},				 //OR
			{1,1,1,1,1,1,-1,1,-1,1,1,1,-1}						//NO
	};
	
int getIndex(int e,int i){
	int k=0;
	switch(e){
		case NOTYPE:k=0;break;
		case '+':k=1;break;
		case '-':k=2;break;
		case '/':k=3;break;
		case '*':
			k=4;
			if(i==0||!(tokens[i-1].type==HEX_NUM||tokens[i-1].type==DEC_NUM||tokens[i-1].type==REG||tokens[i-1].type==')')) {
				k=8;
				tokens[i].type=CITE;
			}
			break;
		case EQ:k=5;break;
		case '(':k=6;break;
		case ')':k=7;break;
		case CITE:k=8;break;
		case NEQ:k=9;break;
		case AND:k=10;break;
		case OR:k=11;break;
		case NO:k=12;break;
		default: break;
	}
	return k;
}

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				//Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */
				if(rules[i].token_type!=NOTYPE){
					tokens[nr_token].type=rules[i].token_type;
					strncpy(tokens[nr_token].str,substr_start,substr_len);
					tokens[nr_token].str[substr_len]='\0';
					nr_token++;
				}
				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}
	return true; 
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	uint32_t num_stack[16];
	Token  sign_stack[16];
	Token space={NOTYPE," "};
	char regstr[8][4]={"a","c","d","b","sp","bp","si","di"};
	int i,s1,s2;

	sign_stack[0]=space;
	s1=0;s2=1;i=0;

	while(s2){
			int type=(i==nr_token?space.type:tokens[i].type);		//待入栈元素类型
			int top_type=sign_stack[s2-1].type;									//符号栈顶元素类型
			uint32_t op1,op2;
			char *s;
			int upper,index,t_index;
			if(!(type==HEX_NUM||type==DEC_NUM||type==REG)){
				index=getIndex(type,i);
				t_index=getIndex(top_type,0);
				upper=stage_table[t_index][index];
				switch(upper){
					case -1:		//栈顶元素优先级低
						sign_stack[s2++]=(i==nr_token?space:tokens[i]);
						if(i<nr_token) i++;
						break;
					case 0: 		//优先级相同
						s2--;
						if(i<nr_token) i++;
						break;
					case 1: 		//栈顶元素优先级高
						switch(sign_stack[s2-1].type){
							case '+':
								op1=num_stack[--s1];
								op2=num_stack[--s1];
								num_stack[s1++]=op1+op2;
								break;
							case '-':
								op1=num_stack[--s1];
								op2=num_stack[--s1];
								num_stack[s1++]=op2-op1;
								break;
							case '*':
								op1=num_stack[--s1];
								op2=num_stack[--s1];
								num_stack[s1++]=op2*op1;
								break;
							case '/':
								op1=num_stack[--s1];
								op2=num_stack[--s1];
								num_stack[s1++]=op2/op1;
								break;
							case EQ:
								op1=num_stack[--s1];
								op2=num_stack[--s1];
								num_stack[s1++]=(op2==op1);
								break;
							case CITE:
								op1=num_stack[--s1];
								num_stack[s1++]=swaddr_read(op1,4);
								break;
							case NEQ:
								op1=num_stack[--s1];
								op2=num_stack[--s1];
								num_stack[s1++]=(op2!=op1);
								break;
							case AND:
								op1=num_stack[--s1];
								op2=num_stack[--s1];
								num_stack[s1++]=(op2&&op1);
								break;
							case OR:
								op1=num_stack[--s1];
								op2=num_stack[--s1];
								num_stack[s1++]=(op2||op1);
								break;
							case NO:
								op1=num_stack[--s1];
								num_stack[s1++]=!op1;
								break;
							default:break;
					}
					--s2;
				}
			}else{
					switch(type){
						case DEC_NUM:
							s=(i==nr_token?space.str:tokens[i].str);
							sscanf(s,"%d",&op1);
							num_stack[s1++]=op1;
							break;
						case HEX_NUM:
							s=(i==nr_token?space.str:tokens[i].str);
							sscanf(s,"0x%x",&op1);
							num_stack[s1++]=op1;
							break;
						case REG:
							s=(i==nr_token?space.str:tokens[i].str);
							int index;
							if(!strcasecmp("eip",s)) {
								op1=cpu.eip;
								break;
							}
							for(index=0;index<8;index++){
								if(strcasestr(s,regstr[index]) ) break;
							}
							if(index==8){
								*success=false;
								return 0;
							}
							if(index<4){
								if(strcasestr(s,"e")){
									op1=reg_l(index)&0xffffffff;
								}else if(strcasestr(s,"x")) op1=reg_w(index)&0xffff;
								else if(strcasestr(s,"l")) op1=reg_b(index)&0xff;
								else if(strcasestr(s,"h")) op1=reg_b((index+4))&0xff;
								else{
									*success=false;
									return 0;
								}
							}else{
								if(strcasestr(s,"e")){
									op1=reg_l(index)&0xffffffff;
								}else{
									op1=reg_w(index)&0xffff;
								}
							}
							num_stack[s1++]=op1;
							break;
						default:break;
					}
                    if(i<nr_token)  i++;
			}
	}

	if(s1==1) {
		*success=true;
		return num_stack[--s1];
	}else{
		printf("表达式格式错误");
		*success=false;
		return 0;
	}
	/* TODO: Insert codes to evaluate the expression. */
	//panic("please implement me");
	//return 0;
}

