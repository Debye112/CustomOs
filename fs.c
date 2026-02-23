#include "kernel.h"


int currentDirectory = 0; 
int poolIndex = 0; 
int poolNumber = 0; 
struct nodes nodePool[128];

void newDirectory(char *name){
    if(++poolIndex < 128){ 
            struct nodes *n = &nodePool[poolIndex];
        
            int i = 0;
            while (name[i]) {
                n->nodeName[i] = name[i];
                i++;
            }
            n->nodeName[i] = '\0';
        
            n->size = 0;
            n->layer = nodePool[currentDirectory].layer + 1;
            n->parentIndex = currentDirectory;
    }
    else
        uart_puts("Internal Nodes ran out. Contact the os owner");
}

void list(){

    for(int i =0; i <= poolIndex; i++){
        if(nodePool[i].parentIndex == currentDirectory){
            uart_puts(nodePool[i].nodeName);
            uart_puts("  ");
        }
    }
    
}

void commandDisecter(struct commandInputs *c, char *input) {
    int i = 1, p = 0;

    while (input[i] && input[i] != '/') {
        c->command[p++] = input[i++];
    }
    c->command[p] = '\0';
    c->commandSize = p;

    if (input[i] == '/') {
        i++;
        p = 0;
        while (input[i]) {
            c->name[p++] = input[i++];
        }
        c->name[p] = '\0';
        c->nameSize = p;
    }
}

void changeDirect(char *name) {

    if(stringcom(name, strlen(name), "..", 2) == 1){
        currentDirectory = nodePool[currentDirectory].parentIndex;
    }
    else if(stringcom(name, strlen(name), "./", 2) == 1){
            currentDirectory = 0;
        }
    else{   
        for (int i = 0; i <= poolIndex; i++) {
            if (nodePool[i].parentIndex == currentDirectory) {
                if (stringcom(name, strlen(name), nodePool[i].nodeName, strlen(nodePool[i].nodeName)) == 1) {
                    currentDirectory = i;
                    return;
                }
            }
        }
        uart_puts("cd: directory not found\n");
    }
}

void removeDirectory(char *name){
    int found = 0;

    for (int i = 0; i <= poolIndex; i++) {
        if (nodePool[i].parentIndex == currentDirectory && stringcom(name, strlen(name), nodePool[i].nodeName, strlen(nodePool[i].nodeName)) == 1) {
            
            found = 1;
            for (int j = i; j < poolIndex; j++) {
                nodePool[j] = nodePool[j + 1];
            }
            poolIndex--; 
            break;       
        }
    }

    if (!found) {
        uart_puts("rm: directory not found\n");
    }
}

int copyDirectory(char *name){

    for(int i = 0; i <=poolIndex; i++){
        if (nodePool[i].parentIndex == currentDirectory && stringcom(name, strlen(name), nodePool[i].nodeName, strlen(nodePool[i].nodeName)) == 1)
            return i;
    }
    return -1;
}

void pasteDirectory(int poolNumber){
    nodePool[poolNumber].parentIndex = currentDirectory;
}

int commands(char *input) {
    int inputSize = strlen(input);
    if (input[0] != '/') {
        char ls[] = "ls";
        if (stringcom(ls, 2, input, inputSize) == 1) {
            list();
            return 1;
        }
        char exit[] = "exit";
        if (stringcom(exit, 4, input, inputSize) == 1) {
            return -1;
        }

        uart_puts("Unknown command\n");
        return 1;
    }

    struct commandInputs comInput;
    commandDisecter(&comInput, input);
    
    char mkdir[] = "mkdir";
    if (stringcom(mkdir, 5, comInput.command, comInput.commandSize) == 1) {
        if(nameBoundCheck(comInput.name) == 1){
            newDirectory(comInput.name);
        }
        else{
                uart_puts("Name exceeds limit");
        }
    }
    char cd[] = "cd";
    if (stringcom(cd, 2, comInput.command, comInput.commandSize) == 1) {
        changeDirect(comInput.name);
        return 1;
    }
    char rm[] = "rm";
        if (stringcom(rm, 2, comInput.command, comInput.commandSize) == 1) {
            removeDirectory(comInput.name);
            return 1;
    }
    char copy[] = "copy";
        if (stringcom(copy, 4, comInput.command, comInput.commandSize) == 1) {
            poolNumber = copyDirectory(comInput.name);
            if(poolNumber < 0) uart_puts("Copying failed");
            return 1;
    }
    char paste[] = "paste";
        if (stringcom(paste, 5, comInput.command, comInput.commandSize) == 1) {
            pasteDirectory(poolNumber);
            return 1;
        }

    return 1;
}
