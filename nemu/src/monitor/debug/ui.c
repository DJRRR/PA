#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
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

static int cmd_test_expr();

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
    { "si", "Execute N instructions",cmd_si },
	{"info","Print the information of register",cmd_info },
	{"x","Scan Memory with the format of ’ x N EXPR ' ",cmd_x },
	{"p","EXPR",cmd_p },
	{"test_expr","function used for test",cmd_test_expr},
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))
static int cmd_test_expr(){
	test_tokens("0x111*1/1+1");
	return 0;

}
static int cmd_p(char *args){
	return -1;
}//uncompleted

static int cmd_x(char *args){
	char *arg = strtok(NULL," ");
    int num=0;
	int i,j;
	for(i=0;i<strlen(arg);i++){
		num = (num*10)+arg[i]-'0';
	}
	arg=strtok(NULL," ");
	unsigned address=0;
	for(j=0;j<strlen(arg);j++){
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
    	printf("<eax>:	0x%02X\n",cpu.eax);
		printf("<ecx>:	0x%02X\n",cpu.ecx);
		printf("<edx>:	0x%02X\n",cpu.edx);
		printf("<ebx>:	0x%02X\n",cpu.ebx);
		printf("<esp>:	0x%02X\n",cpu.esp);
		printf("<ebp>:	0x%02X\n",cpu.ebp);
		printf("<esi>:	0x%02X\n",cpu.esi);
		printf("<edi>:	0x%02X\n",cpu.edi);
		return 0;
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
