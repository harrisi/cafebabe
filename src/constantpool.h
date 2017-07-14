//
// Created by Ian Harris on 17/7/13.
//

#ifndef CAFEBABE_CONSTANTPOOL_H
#define CAFEBABE_CONSTANTPOOL_H

#include "cafebabetypes.h"

/* Table 4.4-A. Constant pool tags */

#define CONSTANT_Class 7
#define CONSTANT_Fieldref 9
#define CONSTANT_Methodref 10
#define CONSTANT_InterfaceMethodref 11
#define CONSTANT_String 8
#define CONSTANT_Integer 3
#define CONSTANT_Float 4
#define CONSTANT_Long 5
#define CONSTANT_Double 6
#define CONSTANT_NameAndType 12
#define CONSTANT_Utf8 1
#define CONSTANT_MethodHandle 15
#define CONSTANT_MethodType 16
#define CONSTANT_InvokeDynamic 18

/* END Constant pool tags */

typedef struct CONSTANT_Class_info {
    u1 tag;
    u2 name_index;
} CONSTANT_Class_info;

typedef struct CONSTANT_Fieldref_info {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
} CONSTANT_Fieldref_info;

typedef struct CONSTANT_Methodref_info {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
} CONSTANT_Methodref_info;

typedef struct CONSTANT_InterfaceMethodref_info {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
} CONSTANT_InterfaceMethodref_info;

typedef struct CONSTANT_String_info {
    u1 tag;
    u2 string_index;
} CONSTANT_String_info;

typedef struct CONSTANT_Integer_info {
    u1 tag;
    u4 bytes;
} CONSTANT_Integer_info;

typedef struct CONSTANT_Float_info {
    u1 tag;
    u4 bytes;
} CONSTANT_Float_info;

typedef struct CONSTANT_Long_info {
    u1 tag;
    u4 high_bytes;
    u4 low_bytes;
} CONSTANT_Long_info;

typedef struct CONSTANT_Double_info {
    u1 tag;
    u4 high_bytes;
    u4 low_bytes;
} CONSTANT_Double_info;

typedef struct CONSTANT_NameAndType_info {
    u1 tag;
    u2 name_index;
    u2 descriptor_index;
} CONSTANT_NameAndType_info;

typedef struct CONSTANT_Utf8_info {
    u1 tag;
    u2 length;
    u1 *bytes; //[length]
} CONSTANT_Utf8_info;

typedef struct CONSTANT_MethodHandle_info {
    u1 tag;
    u1 reference_kind;
    u2 reference_index;
} CONSTANT_MethodHandle_info;

typedef struct CONSTANT_MethodType_info {
    u1 tag;
    u2 descriptor_index;
} CONSTANT_MethodType_info;

typedef struct CONSTANT_InvokeDynamic_info {
    u1 tag;
    u2 bootstrap_method_attr_index;
    u2 name_and_type_index;
} CONSTANT_InvokeDynamic_info;

typedef struct cp_info {
    u1 tag;
    u1 *info;
} cp_info;


#endif //CAFEBABE_CONSTANTPOOL_H
