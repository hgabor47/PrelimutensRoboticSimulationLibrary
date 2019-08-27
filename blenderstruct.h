
#ifndef _blenderstruct_h
#define _blenderstruct_h
//#include <Arduino.h>
#include <inttypes.h>

enum BlenderTypeName {
    BTUnknown=0 , BTub, BTb, BTui, BTi, BTui32, BTi32, BTd, BTf, BTs };

union SBlenderType { //0=unknown
    uint8_t  ub[8]; //1
    int8_t   b[8];  //2
    uint16_t ui[4]; //3
    int16_t  i[4];  //4
    uint32_t ui32[2];  //5
    int32_t  i32[2];   //6
    double   d;        //7
    float    f;        //8
    char     s[8];     //9
};

struct SBlender {
    char name[11];
    SBlenderType value;
    BlenderTypeName type;     //SBlenderType
    SBlender * next;
};

class BlenderStore {
    private:
        SBlender * last;
        SBlender * list;
        
    public:
        int count;

    BlenderStore();
    void insert();
    SBlender * indexof(const char *name, int * idx );
    void save(const char *name,const char *value);   
    void save(const char *name,double value);
    void save(const char *name,uint16_t value);
    void save(const char *name,uint8_t value);


    uint16_t uintload(const char * name);
    uint8_t ubyteload(const char * name);
    char * stringload(const char * name);
    double doubleload(const char * name);
    char * listAll();    
};



#endif