#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "monitor/exprelf.h"
#include "memory/cache.h"//test cache
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

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

//static int cmd_set(char *args);

static int cmd_d(char *args);

static int cmd_bt(char *args);

static int cmd_cache(char *args);//check cache

//static int cmd_test_expr();

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
    { "si", "Execute N instructions",cmd_si },
	{ "info","Print the information of register or watchpoints",cmd_info },
	{ "x","Scan Memory with the format of ’ x N EXPR ' ",cmd_x },
	{ "p","EXPR",cmd_p },
	{ "w","set watchpoint",cmd_w},
	{ "d","delete watchpoint",cmd_d},
	{ "bt","print the backtrace",cmd_bt},
	{ "cache","check cache",cmd_cache},
//	{"test_expr","function used for test",cmd_test_expr},
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))
static int cmd_d(char *args){
	char input;
	if(args==NULL){
		printf("Delete all the watchpoints?(Y/N)\n");
		scanf("%c",&input);
		while(1){
			if(input!='y'&&input!='Y'&&input!='N'&&input!='n'){
				printf("Please input right character!\n");
				scanf("%c",&input);
			}
			else{
				break;
			}
		}
		if(input=='Y'||input=='y'){
			init_wp_list();
			return 0;
		}
		else{
			return 0;
		}
	}
		
	bool success=true;
	uint32_t ans=expr(args,&success);
	if(search_NO(ans)==NULL){
		printf("Watchpoint %u not exit!\n",ans);
		return 0;
	}
	else{
		free_wp(search_NO(ans));
		return 0;
	}
}
/*static int cmd_set(char *args){
	init_wp_list();
	return 0;
}*/
static int cmd_w(char *args){
	if(new_wp(args)==false){
		printf("Set watchpoints failed!\n");
	}
	else{
		printf("Set watchpoints success!\n");
	}
	return 0;
}

//static int cmd_test_expr(){
//	test_tokens("$     ah");
//	return 0;

//}
static int cmd_p(char *args){
	bool success=true;
	uint32_t result=expr(args,&success);
	printf("result:   %u\n",result);
	return 0;
}

static int cmd_x(char *args){
	char *arg = strtok(NULL," ");
    int size=strlen(arg);
	char *e=args+size+1;
    int num=0;
    int i,j;
	bool *success=NULL;
	for(i=0;i<strlen(arg);i++){
		num = (num*10)+arg[i]-'0';
	}
    unsigned address=0;
	address = expr(e,success);
//	printf("%08x\n",address);
    for(j=0;j<num;j++){
		printf("at $0x%.8X  : 0x%.8X\n",address+4*j,swaddr_read(address+4*j,4,S_DS));
	}
	/*for(j=0;j<strlen(arg);j++){// if the address is HEX
		if(arg[j]>='0'&&arg[j]<='9'){
		address =(address*16)+arg[j]-'0';
		}
		else if(arg[j]>='a'&&arg[j]<='f'){
			address=(address*16)+10+arg[j]-'a';
		}
		else{
			address = (address*16)+10+arg[j]-'A';
		}
	}
	for(j=0;j<num;j++){
    printf("%02X\n",hwaddr_read(address+4*j,8));
	}*/
	return 0;
}
static int cmd_bt(char *args){
	char *arg = strtok(NULL," ");
	if(arg==NULL){
		bool *success=NULL;
		unsigned int ebp=expr("$ebp",success);
		unsigned int eip=expr("$eip",success);
		backtrace(ebp,eip);
		return 0;
	}
	else{
		printf("Error input!\n");
		return 0;
	}
}
static int cmd_cache(char *args){
	char *arg=strtok(NULL," ");
	bool *success =NULL;
    hwaddr_t addr=expr(arg,success);
    bool in_cache_L1=find_cache_L1(addr,4);
	bool in_cache_L2=find_cache_L2(addr,4);
	if(in_cache_L1==true){
		printf("0x%.8X in cache_L1!\n",addr);
	}
	if(in_cache_L2==true){
		printf("0x%.8X in cache_L2!\n",addr);
	}
	if(in_cache_L1==false&&in_cache_L2==false){
		printf("0x%.8X not in cache!\n",addr);
	}
	return 0;
}
static int cmd_si(char *args){
	char *arg = strtok(NULL," ");
	int i,j,step_num=0;
	if(arg == NULL){
		cpu_exec(1);
		return 0;
	}
    else{
		i=strlen(arg);
		for(j=0;j<i;j++)
		{
			step_num = (step_num*10)+arg[j]-'0';
		}
		cpu_exec(step_num);
		return 0;
	}
}       
static int cmd_info(char *args){
	char *arg = strtok(NULL," ");
	if(arg[0]=='r'){
    	printf("<eax>:	0x%.8X\n",cpu.eax);
		printf("<ecx>:	0x%.8X\n",cpu.ecx);
		printf("<edx>:	0x%.8X\n",cpu.edx);
		printf("<ebx>:	0x%.8X\n",cpu.ebx);
		printf("<esp>:	0x%.8X\n",cpu.esp);
		printf("<ebp>:	0x%.8X\n",cpu.ebp);
		printf("<esi>:	0x%.8X\n",cpu.esi);
		printf("<edi>:	0x%.8X\n",cpu.edi);
		printf("<eip>:  0x%.8X\n",cpu.eip);
		return 0;
	}
	else if(arg[0]=='w'){
		watchpoint_info();

	}
    return 0;

}
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

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
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
