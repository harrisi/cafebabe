//
// Created by Ian Harris on 17/7/13.
//

#include <stdio.h>

#include "constantpool.h"
#include "printer.h"

struct cp_printing cp_print[] = {
        {0,                           0}, // unused
        {CONSTANT_Utf8,               print_constant_utf8_info},
        {0,                           0}, // unused
        {CONSTANT_Integer,            print_constant_integer_info},
        {CONSTANT_Float,              print_constant_float_info},
        {CONSTANT_Long,               print_constant_long_info},
        {CONSTANT_Double,             print_constant_double_info},
        {CONSTANT_Class,              print_constant_class_info},
        {CONSTANT_String,             print_constant_string_info},
        {CONSTANT_Fieldref,           print_constant_fieldref_info},
        {CONSTANT_Methodref,          print_constant_methodref_info},
        {CONSTANT_InterfaceMethodref, print_constant_interfacemethodref_info},
        {CONSTANT_NameAndType,        print_constant_nameandtype_info},
        {0,                           0}, // unused
        {0,                           0}, // unused
        {CONSTANT_MethodHandle,       print_constant_methodhandle_info},
        {CONSTANT_MethodType,         print_constant_methodtype_info},
        {0,                           0}, // unused
        {CONSTANT_InvokeDynamic,      print_constant_invokedynamic_info}
};

void print_constant_class_info(cp_info *cp) {
    printf("Class| tag: %u; name_index: %u\n",
           cp->tag,
           ((CONSTANT_Class_info *) cp)->name_index);
}

void print_constant_fieldref_info(cp_info *cp) {
    CONSTANT_Fieldref_info *tmp = (CONSTANT_Fieldref_info *) cp;
    printf("Fieldref| tag: %u; class_index: %u; name_and_type_index: %u\n",
           cp->tag,
           tmp->class_index,
           tmp->name_and_type_index);
}

void print_constant_methodref_info(cp_info *cp) {
    CONSTANT_Methodref_info *tmp = (CONSTANT_Methodref_info *) cp;
    printf("Methodref| tag: %u; class_index: %u; name_and_type_index: %u\n",
           cp->tag,
           tmp->class_index,
           tmp->name_and_type_index);
}

void print_constant_interfacemethodref_info(cp_info *cp) {
    CONSTANT_InterfaceMethodref_info *tmp = (CONSTANT_InterfaceMethodref_info *) cp;
    printf("InterfaceMethodref| tag: %u; class_index: %u; name_and_type_index: %u\n",
           cp->tag,
           tmp->class_index,
           tmp->name_and_type_index);
}

void print_constant_string_info(cp_info *cp) {
    printf("String| tag: %u; string_index: %u\n",
           cp->tag,
           ((CONSTANT_String_info *) cp)->string_index);
}

void print_constant_integer_info(cp_info *cp) {
    printf("Integer| tag: %u; bytes: %u\n",
           cp->tag,
           ((CONSTANT_Integer_info *) cp)->bytes);
}

void print_constant_float_info(cp_info *cp) {
    printf("Float| tag: %u; bytes: %u\n",
           cp->tag,
           ((CONSTANT_Float_info *) cp)->bytes);
}

void print_constant_long_info(cp_info *cp) {
    CONSTANT_Long_info *tmp = (CONSTANT_Long_info *) cp;
    printf("Long| tag: %u; high_bytes: %u; low_bytes: %u\n",
           cp->tag,
           tmp->high_bytes,
           tmp->low_bytes);
}

void print_constant_double_info(cp_info *cp) {
    CONSTANT_Double_info *tmp = (CONSTANT_Double_info *) cp;
    printf("Double| tag: %u; high_bytes: %u; low_bytes: %u\n",
           cp->tag,
           tmp->high_bytes,
           tmp->low_bytes);
}

void print_constant_nameandtype_info(cp_info *cp) {
    CONSTANT_NameAndType_info *tmp = (CONSTANT_NameAndType_info *) cp;
    printf("NameAndType| tag: %u; name_index: %u; descriptor_index: %u\n",
           cp->tag,
           tmp->name_index,
           tmp->descriptor_index);
}

void print_constant_utf8_info(cp_info *cp) {
    CONSTANT_Utf8_info *tmp = (CONSTANT_Utf8_info *) cp;
    printf("Utf8| tag: %u; length: %u; bytes: %s\n",
           cp->tag,
           tmp->length,
           tmp->bytes);
}

void print_constant_methodhandle_info(cp_info *cp) {
    CONSTANT_MethodHandle_info *tmp = (CONSTANT_MethodHandle_info *) cp;
    printf("MethodHandle| tag: %u; reference_kind: %u; reference_index: %u\n",
           cp->tag,
           tmp->reference_kind,
           tmp->reference_index);
}

void print_constant_methodtype_info(cp_info *cp) {
    printf("MethodType| tag: %u; descriptor_index: %u\n",
           cp->tag,
           ((CONSTANT_MethodType_info *) cp)->descriptor_index);
}

void print_constant_invokedynamic_info(cp_info *cp) {
    CONSTANT_InvokeDynamic_info *tmp = (CONSTANT_InvokeDynamic_info *) cp;
    printf("InvokeDynamic| tag: %u; bootstrap_method_attr_index: %u; name_and_type_index: %u\n",
           cp->tag,
           tmp->bootstrap_method_attr_index,
           tmp->name_and_type_index);
}

void print_constant_pool(cp_info *cp) {
    if (!cp) {
        printf("haven't read this cp yet\n");
        return;
    }

    cp_print[cp->tag].func(cp);
}

void print_interfaces(u2 *i) {
    printf("not yet implemented\n");
}

void print_fields(field_info *fi) {
    printf("not yet implemented\n");
}

void print_methods(method_info *mi) {
    printf("not yet implemented\n");
}

void print_attributes(attribute_info *ai) {
    printf("not yet implemented\n");
}