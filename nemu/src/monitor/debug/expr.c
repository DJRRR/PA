#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
enum {
	NOTYPE = 255, 
    NUM = 256,
	HEX = 257,
	EQ = 258,
	NEQ = 259,
	EAX = 260,
	EBX = 261,
	EDX = 262,
	ECX = 263,
	EBP = 264,
	ESI = 265,
	EDI = 266,
	ESP = 267,
	AH = 268,
	AL = 269,
	DH = 270,
	DL = 271,
	CH = 272,
	CL = 273,
	BH =274,
	BL = 275,
	BP = 276,
	SI = 277,
	DI = 278,
	SP = 279,
	AX = 280,
	DX = 281,
	CX = 282,
	BX = 283,
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
	{"\\+", '+'},					// plus  43  level:4  size:2 means quzheng(level 2) size:1 means plus
	{"==", EQ},                  	// equal     level:7
	{"!=",NEQ},                     // notequal  level:7
	{"!",'!'},                      // Not       level:2
	{"\\$",'$'},                    // REGISTER  level:2
	{"eax",EAX},                    //eax        level:0
	{"ebx",EBX},                    //ebx        level:0
	{"edx",EDX},                    // ..................
	{"ecx",ECX},                      
	{"ebp",EBP},
	{"esi",ESI},
	{"edi",EDI},
	{"esp",ESP},
	{"ah",AH},
	{"al",AL},
	{"dh",DH},
	{"dl",DL},
	{"ch",CH},
	{"cl",CL},
	{"bh",BH},
	{"bl",BL},
	{"bp",BP},
	{"si",SI},
	{"di",DI},
	{"sp",SP},
	{"ax",AX},
	{"dx",DX},
	{"cx",CX},
	{"bx",BX},
//	{"!=",NEQ},                     // notequal  level:7
	{"-",'-'},                   	// minus 45  level:4  size:2 means qufu(level 2)  size:1 means minus 
	{"\\*",'*'},                    // multi 42  level:3
	{"%",'%'},                      // quyu      level:3
	{"/",'/'},                      // round 47  level:3
	{"\\(",'('},                    // left 40   level:1
	{"\\)",')'},                    // right 41  level:1
	{"0x[0-9a-fA-F]+",HEX},         // hexadecimal-number  level:0
	{"[0-9]+",NUM},                 //decimal integer      level:0
	{"&&",'&'},                     //AND  level:11
	{"\\|\\|",'|'},                     // OR  level:12

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
					case '$':
						tokens[nr_token].type=rules[i].token_type;
						tokens[nr_token].level=2;
						nr_token++;
						break;
					case '!':
						tokens[nr_token].type=rules[i].token_type;
						tokens[nr_token].level=2;
						nr_token++;
                        break;
					case '&':
						tokens[nr_token].type=rules[i].token_type;
						tokens[nr_token].level=11;
						nr_token++;
						break;
					case '|':
						tokens[nr_token].type=rules[i].token_type;
						tokens[nr_token].level=12;
						nr_token++;
						break;
					case '+':
					case '-':// qufu temporarily correct
						tokens[nr_token].type=rules[i].token_type;
						if(nr_token==0||(nr_token>0&&tokens[nr_token-1].type!=NUM&&tokens[nr_token-1].type!=HEX&&tokens[nr_token-1].type!=')')){
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
						tokens[nr_token].type=rules[i].token_type;
						if(nr_token==0||(nr_token>0&&tokens[nr_token-1].type!=NUM&&tokens[nr_token-1].type!=HEX&&tokens[nr_token-1].type!=')')){
							tokens[nr_token].size=2;
							tokens[nr_token].level=2;
						}
						else{
							tokens[nr_token].size=1;
							tokens[nr_token].level=3;
						}
						nr_token++;
						break;
					case '/':
					case '%':
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
					case EAX:
					case EDX:
					case ECX:
					case EBX:
					case EBP:
					case ESI:
					case EDI:
					case ESP:
					case AH:
					case DH:
					case CH:
					case BH:
					case AL:
					case DL:
					case CL:
					case BL:
					case AX:
					case DX:
					case CX:
					case BX:
					case BP:
					case SI:
					case DI:
					case SP:
						tokens[nr_token].type=rules[i].token_type;
						tokens[nr_token].level=0;
						nr_token++;
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
						    printf("DECIMAL NUM EXCEED！\n");
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
	    if(i!=q&&tokens[i+1].type==0){//necessary?
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

uint32_t eval(int p,int q){//temporarily correct
	int i;
	uint32_t result=0;
	int j,max_level=0;
	int pos=0;
	int temp_level=0;
    uint32_t val1,val2;
	int count=0;
	bool flag=false;
   for(i=p;i<=q;i++){
	   if(tokens[i].level!=2&&tokens[i].level!=0){
		   flag=true;
		   break;
	   }
   }
   if(p>q){
	printf("Error1:Bad expression![p>q]\n");
	assert(0);
	return -1;
   }
   else if(q==p+1){
	   if(tokens[p].size==2&&tokens[p].type=='-'){
		   return 0-eval(p+1,q);
	   }
	   else if(tokens[p].size==2&&tokens[p].type=='+'){
		   return eval(p+1,q);
	   }
	   else if(tokens[p].type=='!'){
		   return !eval(p+1,q);
	   }
	   else if(tokens[p].size==2&&tokens[p].type=='*'){//uncompleted
		   printf("aa\n");
		   return hwaddr_read(eval(p+1,q),8);
	   }
	   else if(tokens[p].type=='$'){
		   return eval(p+1,q);
	   }
	   else{
		   printf("Other situations!\n");
		   assert(0);
		   return -1;
	   }
   }

   else if(p==q){
	   switch(tokens[p].type){
		   case HEX:
				result=0;
				for(i=0;i<tokens[p].size;i++){//unchecked what about range exceed?
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
				break;;
		   case NUM:
			result=0;
			for(i=0;i<tokens[p].size;i++){
				result = result*10+(tokens[p].str[i]-'0');
			}
			for(i=0;i<tokens[p].size-32;i++){//uncompleted
				result *= 10;
			}
			return result;
			break;
		   case EAX:
				return cpu.eax;
				break;
		   case EBX:
				return cpu.ebx;
				break;
		   case ECX:
				return cpu.ecx;
				break;
		   case EDX:
				return cpu.edx;
				break;
		   case EBP:
				return cpu.ebp;
				break;
		   case ESI:
				return cpu.esi;
				break;
		   case EDI:
				return cpu.edi;
				break;
		   case ESP:
				return cpu.esp;
				break;
		   case AH:
				return cpu.gpr[0]._8[1];
				break;
	       case AL:
				return cpu.gpr[0]._8[0];
				break;
		   case CH:
				return cpu.gpr[1]._8[1];
				break;
		   case CL:
				return cpu.gpr[1]._8[0];
				break;
		   case DH:
				return cpu.gpr[2]._8[1];
				break;
		   case DL:
				return cpu.gpr[2]._8[0];
				break;
		   case BH:
				return cpu.gpr[3]._8[1];
				break;
		   case BL:
				return cpu.gpr[3]._8[0];
				break;
		   case AX:
				return cpu.gpr[0]._16;
				break;
		   case CX:
				return cpu.gpr[1]._16;
				break;
		   case DX:
				return cpu.gpr[2]._16;
				break;
		   case BX:
				return cpu.gpr[3]._16;
				break;
		   case SP:
				return cpu.gpr[4]._16;
				break;
		   case BP:
				return cpu.gpr[5]._16;
				break;
		   case SI:
				return cpu.gpr[6]._16;
				break;
		   case DI:
				return cpu.gpr[7]._16;
				break;
		   default:
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
	   if(tokens[p].type=='-'&&tokens[p].size==2&&flag==false){
		   return 0-eval(p+1,q);
	   }
	   if(tokens[p].type=='*'&&tokens[p].size==2&&flag==false){
		   return hwaddr_read(eval(p+1,q),8);
	   }
		max_level=-1;
	   temp_level=-1;	
	  // puts("here\n");
		 for(j=p;j<q+1;j++){
			 if(tokens[j].level>1){
			 temp_level=tokens[j].level;
			 }
			 else{
				 temp_level=-1;
			 }
			 if((temp_level>=max_level)&&(count==0)){
				 max_level=temp_level;
				 pos=j;
			 }
			 if(tokens[j].type=='('){
				  count++;
			 }
			 if(tokens[j].type==')'){
				 count--;
			  }
	}
				 val1=eval(p,pos-1);
				 val2=eval(pos+1,q);
					 switch(tokens[pos].type){
						 case '+':return val1+val2;
						 case '&':return val1&&val2;
						 case '|':return val1||val2;
						 case '-':
						     return val1-val2;

						 case '*':return val1*val2;
						 case '%':return val1%val2;
						 case '/':return val1/val2;
						 case  EQ:return (val1==val2);
						 case NEQ:return (val1!=val2);
						 default:assert(0);
					 }
				 
	   }
 //  }
}




void test_tokens(char *e)
{    
	make_token(e);
	uint32_t result=eval(0,1);
	 printf("test_result:%u\n",result);
}
uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	/* TODO: Insert codes to evaluate the expression. */
	return eval(0,nr_token-1);
}

