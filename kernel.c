#include "kernel.h"

// -------- Kernel entry --------
void kmain() { 

	uart_init(); 
	uart_puts("BUILD_XYZ\n");
	
    int security_code = 0;
    char input_pass[64];
    char pass[] = "Devesh$4321";
    
    while(security_code == 0) {
        uart_puts("\nEnter OS password:");
        uart_gets(input_pass);
        int correction = stringcom(pass, strlen(pass), input_pass, strlen(input_pass));
        if(correction == 1) break;
    }
    char buffer[MAX_INPUT];
    int jk = 0;
    char name[] = "root";
    
    while (name[jk]) {
        nodePool[0].nodeName[jk] = name[jk];
        jk++;
    }
    nodePool[0].nodeName[jk] = '\0';
    nodePool[0].size = 0;
    nodePool[0].layer = 1;
    nodePool[0].parentIndex = -1;
    
    currentDirectory = 0;
    int loop_break = 0;
    while(loop_break != -1){

            uart_puts("\n");
            uart_puts("leos/");
            uart_puts(nodePool[currentDirectory].nodeName);
            uart_puts("> ");
            
        
            uart_gets(buffer);

            loop_break = commands(buffer);

            while (1) {
                terminal_tick();
            }
            
    }
    

    while(1) { }
}
