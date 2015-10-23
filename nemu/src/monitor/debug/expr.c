#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
enum {
	NOTYPE = 256, EQ,
    NUM=255,
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
    {"[0-9x]+",NUM},               // decimal integer
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
	int j,k;
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
						tokens[nr_token].type='+';
						nr_token++;
						break;
					case '-':
						tokens[nr_token].type='-';
						nr_token++;
						break;
					case '*':
						tokens[nr_token].type='*';
						nr_token++;
						break;
					case '/':
						tokens[nr_token].type='/';
						nr_token++;
						break;
					case NOTYPE:
                        break;
					case NUM:
						tokens[nr_token].type=NUM;
						j=k=0;
						if(substr_len<=32){
							for(j=position-1;j>position-substr_len;j--){
								tokens[nr_token].str[k++]=e[j]+48;
								printf("test%c\n",e[j]);
							}
						}
						else{
							printf("Decimal integer exceed!\n");
							assert(0);
						}
						nr_token++;
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
bool check_parentheses(){//unchecked
	int i,count=0;
	if(tokens[0].type!='(')
	{
		return false;
	}
    for(i=1;i<32;){
		 if(i!=31&&tokens[i+1].type==0){
			 break;
		 }
	}
	if(tokens[i].type!=')'){
		return false;
	}
	for(i=0;i<32;i++){
		if(tokens[i].type=='('){
			count++;
		}
		if(tokens[i].type==')'){
			count--;
		}
	    if(i!=31&&tokens[i+1].type==0){
			break;
		}
		if(count<=0){
			return false;
		}
	}
	if(count==0){
		return true;
	}
	else{
		return false;
	}
}


int eval(int p,int q){//uncompleted
   if(p>q){
	printf("Error1:Bad expression!\n");
	return -1;
   }
   else if(p==q){
	if(tokens[p].type!=NUM){
	  printf("Error2:Bad expression!\n");
	  return -1;
	}
	else{
	int result=0,i;
	for(i=0;i<32;i++){
		result = result*10+(tokens[p].str[i]-'0');
	}
	return result;
	}
   }
   else{
	   return -1;
   }
}






void test_tokens(char *e)
{    
	 make_token(e);
	 printf("%d\n",tokens[0].str[1]);
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

