#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ,
    
	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
`	 */

	{" +",	NOTYPE},				// spaces 256
	{"\\+", '+'},					// plus  43
	{"==", EQ},                   	// equal
	{"-",'-'},                   	// minus 45
	{"\\*",'*'},                    // multi 42
	{"/",'/'},                    // round 47
	{"\\(",'('},                    // left 40
	{"\\)",')'},                    // right 41
    {"[0-9x]+",'d'},               // decimal integer
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

static bool make_token(char *e) {//shibie token
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {    //NR_REGEX means the size of expr
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;
				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */

				switch(rules[i].token_type) {
					case '+':
						nr_token++;
						tokens[i].type='+';
						break;
					case '-':
						tokens[i].type='-';
						nr_token++;
						break;
					case '*':
						tokens[i].type='*';
						nr_token++;
						break;
					case '/':
						tokens[i].type='/';
						nr_token++;
						break;
					case NOTYPE:
                        break;
					case 'd':
						 nr_token++;
						tokens[i].type='d';
						int j,k=0;
						for(j=position-substr_len;j<substr_len;j++){
							tokens[i].str[k++]=e[j];
						}
						printf("%d",tokens[i].str[2]);
					   break;
					case EQ:
						nr_token++;
						tokens[i].type=EQ;
						break;
					case '(':
                        nr_token++;
						tokens[i].type='(';
						break;
					case ')':
						nr_token++;
						tokens[i].type=')';
						break;
					default: panic("please implement me");
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
void test_tokens(char *e)
{    
	 make_token(e);
	 printf("%c\n",tokens[0].str[0]);
     printf("%d\n",nr_token);
		 
}
uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
    
	/* TODO: Insert codes to evaluate the expression. */
	panic("please implement me");
	return 0;
}

