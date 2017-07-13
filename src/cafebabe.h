#ifndef CAFEBABE_H
#define CAFEBABE_H

#include <stdio.h>

#include "cafebabetypes.h"
#include "constantpool.h"

typedef struct class_file class_file;
typedef void class_file_fn(class_file *);

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

void print_cf(class_file *);

#endif // CAFEBABE_H
