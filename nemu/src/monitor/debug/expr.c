#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ,DEC_NUM,HEX_NUM,BIN_NUM,REG

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
	{"(",'('},
	{")",')'},
	{"[0-9]+",DEC_NUM},		//十进制整数
	{"0x[0-9a-fA-F]+",HEX_NUM},			//十六进制整数
	{"[0-1]+",BIN_NUM},		//二进制整数
	{"\\$[eE][a-zA-Z]{2}",REG}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

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

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */
				if(rules[i].token_type!=NOTYPE)
					tokens[nr_token].type=rules[i].token_type;
				switch(rules[i].token_type) {
					//case NOTYPE: break;
					case '+':
					case  '-' :
					case  '/' :
					case  '*':
					case  EQ:
					case  '(': 
					case  ')':
					case  DEC_NUM:case  HEX_NUM:case BIN_NUM:
							strncpy(tokens[nr_token].str,substr_start,substr_len);
							break;
					default: panic("please implement me");
				}
				nr_token++;
				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
		int k;
		for(k=0;k<nr_token;k++){
			printf("%d\t%s\n",tokens[k].type,tokens[k].str);
		}
	}

	return true; 
}

uint32_t expr(char *e, bool *success) {
	printf("hello");
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
	panic("please implement me");
	return 0;
}

