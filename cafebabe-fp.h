#ifndef __CAFEBABE_FP_H__
#define __CAFEBABE_FP_H__

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

typedef struct class_file class_file;
typedef void class_file_fn(class_file *);

typedef uint8_t  u1;
typedef uint16_t u2;
typedef uint32_t u4;

typedef struct CONSTANT_Methodref_info {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
} CONSTANT_Methodref_info;

typedef struct cp_info {
    u1 tag;
    u1 *info;
} cp_info;

typedef struct attribute_info {
    u2 attribute_name_index;
    u4 attribute_length;
    u1 *info; // [attribute_length]
} attribute_info;

typedef struct field_info {
    u2              access_flags;
    u2              name_index;
    u2              descriptor_index;
    u2              attributes_count;
    attribute_info  *attributes; // [attributes_count]
} field_info, method_info;

/* Java class file structure */

typedef struct class_file {
    // "class file state-related fields"
    class_file_fn *next;
    FILE *file;
    // classfile structure
    // source:
    // https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.1
    u4              magic;
    u2              minor_version;
    u2              major_version;
    u2              constant_pool_count;
    cp_info         **constant_pool;//[constant_pool_count-1]
    u2              access_flags;
    u2              this_class;
    u2              super_class;
    u2              interfaces_count;
    u2              **interfaces;//[interfaces_count]
    u2              fields_count;
    field_info      **fields;//[fields_count]
    u2              methods_count;
    method_info     **methods;//[methods_count]
    u2              attributes_count;
    attribute_info  **attributes;//[attributes_count]
} class_file;

/* END Java class file structure */

/* functions that the FSM will iterate through */
class_file_fn read_magic,
              read_minor_version,
              read_major_version,
              read_constant_pool_count,
              read_constant_pool,
              read_access_flags,
              read_this_class,
              read_super_class,
              read_interfaces_count,
              read_interfaces,
              read_fields_count,
              read_fields,
              read_methods_count,
              read_methods,
              read_attributes_count,
              read_attributes;
/* END FSM functions */

#endif // __CAFEBABE_FP_H__
