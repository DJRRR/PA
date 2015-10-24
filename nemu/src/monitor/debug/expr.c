#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
enum {
	NOTYPE = 256, 
    NUM = 255,
	HEX = 254,
	EQ = 253,
	NEQ = 252,
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
	{"\\+", '+'},					// plus  43  level:4
	{"==", EQ},                   	// equal     level:7
	{"!=",NEQ},                     // notequal  level:7
	{"-",'-'},                   	// minus 45  level:4  size:2 means qufu  size:1 means minus
	{"\\*",'*'},                    // multi 42  level:3
	{"/",'/'},                      // round 47  level:3
	{"\\(",'('},                    // left 40   level:1
	{"\\)",')'},                    // right 41  level:1
	{"0x[0-9a-fA-F]+",HEX},         // hexadecimal-number  level:0
	{"[0-9]+",NUM},                 //decimal integer      level:0
	//{"",REG},                     // reg name
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
	int size;
	int level;
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
					case '-':// qufu unchecked
						tokens[nr_token].type=rules[i].token_type;
						if(nr_token==0||(nr_token>0&&tokens[nr_token-1].type!=NUM&&tokens[nr_token-1].type!=')')){
							tokens[nr_token].size=2;
							tokens[nr_token].level=2;
						}
						else{
							tokens[nr_token].size=1;
							tokens[nr_token].level=4;
						}
						nr_token++;
						break;
					case '*':
					case '/':
						tokens[nr_token].type=rules[i].token_type;
						tokens[nr_token].level=3;
						nr_token++;
						break;
					case EQ:
					case NEQ:
						tokens[nr_token].type=rules[i].token_type;
						tokens[nr_token].level=7;
						nr_token++;
						break;
					case '(':
					case ')':
						tokens[nr_token].type=rules[i].token_type;
						tokens[nr_token].level=1;
						nr_token++;
						break;
					case NOTYPE:
                        break;
					case HEX:
						tokens[nr_token].type=rules[i].token_type;
						tokens[nr_token].level=0;
						tokens[nr_token].size=substr_len-2;
						j=k=0;
						if(substr_len<=34){
							for(j=position-substr_len+2;j<=position-1;j++){
								tokens[nr_token].str[k++]=e[j];
							}
						}
						else{
							printf("HEX EXCEED!\n");
							assert(0);
						}
						nr_token++;
						break;

					case NUM:
						tokens[nr_token].type=rules[i].token_type;
						tokens[nr_token].level=0;
						tokens[nr_token].size=substr_len;
						j=k=0;
						if(substr_len<=32){
							for(j=position-substr_len;j<=position-1;j++){
								tokens[nr_token].str[k++]=e[j];
							}
						}	
						else{
						//	for(j=position-substr_len;j<position-substr_len+32;j++){
						//		tokens[nr_token].str[k++]=e[j];
						//	}
						//	if(e[j]>='5'){
						//		tokens[nr_token].str[31]++;
						//	}
						    printf("NUM EXCEED！\n");
							assert(0);
						}
						nr_token++;
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
int check_parentheses(int p,int q){//1 means true 0 means can expr -1 means
	int i,count=0;
	int result=1;
	if(tokens[p].type!='(')
	{
		result=0;
	}
	if(tokens[q].type!=')'){
	//	printf("the last\n");
		result=0;
	}
	for(i=p;i<=q;i++){
		if(tokens[i].type=='('){
			count++;
		}
		if(tokens[i].type==')'){
			count--;
		}
	    if(i!=q&&tokens[i+1].type==0){//???
			break;
		}
		if(count<0){
			return -1;
		}
		if(count==0&&i!=q){
			result=0;
		}
	}
	return result;
}

long int eval(int p,int q){//uncompleted
	int i;
	long int result=0;
	int j,max_level=0;
	int pos=0;
	int temp_level=-1;
	int val1,val2;
	int count=0;
   if(p>q){
	printf("Error1:Bad expression![p>q]\n");
	assert(0);
	return -1;
   }
   else if(p==q){
	//if(tokens[p].type!=NUM){
	 // printf("Error2:Bad expression!\n");
	 // assert(0);
	 // return -1;
//	}
	if(tokens[p].type==HEX){
		result=0;
		for(i=0;i<tokens[p].size;i++){//unchecked
			if(tokens[p].str[i]>='0'&&tokens[p].str[i]<='9'){
				result = result*16+(tokens[p].str[i]-'0');
			}
			else if(tokens[p].str[i]>='A'&&tokens[p].str[i]<='F'){
				result = result*16+(tokens[p].str[i]-'A'+10);
			}
			else{
				result = result*16+(tokens[p].str[i]-'a'+10);
			}
		}
		for(i=0;i<tokens[p].size-32;i++){
			result *= 16;
		}
		return result;
	}  
	if(tokens[p].type==NUM){
	result=0;
	for(i=0;i<tokens[p].size;i++){
		result = result*10+(tokens[p].str[i]-'0');
	}
	for(i=0;i<tokens[p].size-32;i++){//uncompleted
		result *= 10;
	}
	return result;
	}
	else{
		printf("Error 2:Bad expression![when p==q there is no num to calculate]\n");
		assert(0);
		return -1;
	}
   }
   else if(check_parentheses(p,q)==1){
	   return eval(p+1,q-1);
   }
   else if(check_parentheses(p,q)==-1){
	   printf("Error3:Bad expression![mismatched braces]\n");
	   assert(0);
	   return -1;
   }
   else{
	   if(tokens[p].size==2&&tokens[p].type=='-'){
		   return 0-eval(p+1,q);
	   }
	   else{	   
	  // puts("here\n");
		 for(j=p;j<q+1;j++){
			  if(tokens[j].level>1){
				  temp_level=tokens[j].level;
				 //  printf("test%c\n",tokens[j].type);
			 }
			 else{
			 temp_level=-1;
			 }
			 if(tokens[j].type=='('){
				  count++;
			 }
			 if(tokens[j].type==')'){
				 count--;
			  }
			 if((temp_level>=max_level&&count==0)){
				 max_level=temp_level;
				 pos=j;
				// printf("num:%d    %d\n",j,pos);
			 }
		 }
			 val1=eval(p,pos-1);
			 val2=eval(pos+1,q);
				 switch(tokens[pos].type){
				 case '+':return val1+val2;
				 case '-':return val1-val2;
				 case '*':return val1*val2;
				 case '/':return val1/val2;
				 case  EQ:return val1==val2;
				 case NEQ:return val1!=val2;
				 default:assert(0);
				 }
	   }
   }
}




void test_tokens(char *e)
{    
	make_token(e);
	 long int result=eval(0,2);
	 printf("test_result:%ld\n",result);
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

