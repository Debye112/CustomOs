#include "kernel.h"



int strlen(char *resp){
    int index = 0;
    while (resp[index] != '\0'){
        index++;
    }
    return index;   
}

int nameBoundCheck(char *name){
    if (strlen(name) < 32) return 1;
    return 0;
}

int stringcom(char *comref, int s_comref, char *com, int s_com){
    int relevance = 0;
    if(s_comref == s_com){

        for(int i = 0; i < s_com; i++){
            if(comref[i] == com[i]) relevance++;
        }

        if(relevance == s_com) return 1;
        
        else 
            return -1;
    }
    else 
        return -1;
    
}
