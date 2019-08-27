#include "blenderstruct.h"
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

BlenderStore::BlenderStore(){
    list = NULL;
    last=list;
    count=0;
}

char * BlenderStore::listAll(){
    SBlender *mut;
    char * res = (char*)calloc(count,11);    
    mut = list;
    int i=0;
    while (mut!=NULL){
        strcat(res,mut->name);
        strcat(res,"|");
        i++;
        mut=mut->next;
    }
    return res;
}

SBlender * BlenderStore::indexof(const char *name, int * idx ){
    SBlender *mut;
    mut = list;
    int i=0;
    while (mut!=NULL){
        if ( strcmp(name,mut->name)==0 ){
            *idx = i;
            return mut;
        }
        i++;
        mut=mut->next;
    }
    *idx= -1;
    return NULL;
}

void BlenderStore::save(const char *name,const char *value){
    int idx;
    SBlender * sb = indexof(name,&idx);
    if (sb!=NULL){        
        switch (sb->type){
            case BTs: 
            case BTUnknown:
                strncpy(sb->value.s,value,sizeof(SBlenderType));
                break;
            case BTd:
                save(name,strtod(value,0));
                break;
            case BTui:
                save(name,(uint16_t)strtol(value,0,10));
                break;
            case BTub:
                save(name,(uint8_t)strtol(value,0,10));
                break;
            //TODO                
        }
        return;
    }
    insert();
    strncpy(last->value.s,value,sizeof(SBlenderType));
    strncpy(last->name,name,10);
    last->type=BTs;
}

void BlenderStore::save(const char *name,double value){
    int idx;
    SBlender * sb = indexof(name,&idx);
    if (sb!=NULL){        
        sb->value.d=value;
        return;
    }
    insert();
    last->value.d=value;    
    strncpy(&last->name[0],name,10);
    last->type=BTd;    
}

void BlenderStore::save(const char *name,uint16_t value){
    int idx;
    SBlender * sb = indexof(name,&idx);
    if (sb!=NULL){        
        sb->value.ui[0]=value;
        return;
    }
    insert();
    last->value.ui[0]=value;
    strncpy(last->name,name,10);
    last->type=BTui;
}

void BlenderStore::save(const char *name,uint8_t value){
    int idx;
    SBlender * sb = indexof(name,&idx);
    if (sb!=NULL){        
        sb->value.ub[0]=value;
        return;
    }
    insert();
    last->value.ub[0]=value;
    strncpy(last->name,name,10);
    last->type=BTub;
}

void BlenderStore::insert(){
    count++;
    SBlender * mut = (SBlender*)calloc(1,sizeof(SBlender));
    mut->next=NULL;
    if (list==NULL){
        list = mut;
        last = mut;
        return ;
    }
    last->next=mut;
    last=mut;
} 


double BlenderStore::doubleload(const char * name){        
    int idx;
    
    SBlender * sb = indexof(name,&idx);
    
    if (sb==NULL){
        return 0;
    } 
    return sb->value.d;
};
char * BlenderStore::stringload(const char * name){
    int idx;
    SBlender * sb = indexof(name,&idx);
    if (sb==NULL){
        return NULL;
    } 
    return (char *)(sb->value.s);
};
uint16_t BlenderStore::uintload(const char * name){
    int idx;
    SBlender * sb = indexof(name,&idx);
    if (sb==NULL){
        return 0;
    } 
    return sb->value.ui[0];
};

uint8_t BlenderStore::ubyteload(const char * name){
    int idx;
    SBlender * sb = indexof(name,&idx);
    if (sb==NULL){
        return 0;
    } 
    return sb->value.ub[0];
};


