//
// Created by Ian Harris on 17/7/13.
//

#ifndef CAFEBABE_CAFEBABETYPES_H
#define CAFEBABE_CAFEBABETYPES_H

#include <stdint.h>

typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;

typedef struct attribute_info {
    u2 attribute_name_index;
    u4 attribute_length;
    u1 *info; // [attribute_length]
} attribute_info;

typedef struct field_info {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    attribute_info *attributes; // [attributes_count]
} field_info, method_info;

#endif //CAFEBABE_CAFEBABETYPES_H
