// function pointer with state implementation
// (built upon from https://stackoverflow.com/a/1383453)

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cafebabe.h"
#include "printer.h"
#include "util.h"
#include "attributes.h"

// Class file-specific functions (magic, version, etc.) might be moved
// somewhere.

void read_magic(struct class_file *cf) {
    cf->magic = read_u4(cf->file);

    cf->next = read_minor_version;
}

void read_minor_version(struct class_file *cf) {
    cf->minor_version = read_u2(cf->file);

    cf->next = read_major_version;
}

void read_major_version(struct class_file *cf) {
    cf->major_version = read_u2(cf->file);

    cf->next = read_constant_pool_count;
}

/* Constant pool stuff might make more sense in constantpool.c. */

void read_constant_pool_count(struct class_file *cf) {
    cf->constant_pool_count = read_u2(cf->file);

    cf->next = read_constant_pool;
}

void read_class_info(struct class_file *cf, int index) {
    cf->constant_pool[index] = malloc(sizeof(CONSTANT_Class_info));
    CONSTANT_Class_info *tmp = (CONSTANT_Class_info *)cf->constant_pool[index];
    tmp->tag = CONSTANT_Class;
    tmp->name_index = read_u2(cf->file);
}

void read_fieldref_info(struct class_file *cf, int index) {
    cf->constant_pool[index] = malloc(sizeof(CONSTANT_Fieldref_info));
    CONSTANT_Fieldref_info *tmp = (CONSTANT_Fieldref_info *)cf->constant_pool[index];
    tmp->tag = CONSTANT_Fieldref;
    tmp->class_index = read_u2(cf->file);
    tmp->name_and_type_index = read_u2(cf->file);
}

void read_methodref_info(struct class_file *cf, int index) {
    cf->constant_pool[index] = malloc(sizeof(CONSTANT_Methodref_info));
    CONSTANT_Methodref_info *self = (CONSTANT_Methodref_info *)cf->constant_pool[index];
    self->tag = CONSTANT_Methodref;
    self->class_index = read_u2(cf->file);
    self->name_and_type_index = read_u2(cf->file);
}

void read_interfacemethodref_info(struct class_file *cf, int index) {
    cf->constant_pool[index] = malloc(sizeof(CONSTANT_InterfaceMethodref_info));
    CONSTANT_InterfaceMethodref_info *self = (CONSTANT_InterfaceMethodref_info *)cf->constant_pool[index];
    self->tag = CONSTANT_InterfaceMethodref;
    self->class_index = read_u2(cf->file);
    self->name_and_type_index = read_u2(cf->file);
}

void read_string_info(struct class_file *cf, int index) {
    cf->constant_pool[index] = malloc(sizeof(CONSTANT_String_info));
    CONSTANT_String_info *tmp = (CONSTANT_String_info *)cf->constant_pool[index];
    tmp->tag = CONSTANT_String;
    tmp->string_index = read_u2(cf->file);
}

void read_integer_info(struct class_file *cf, int index) {
    cf->constant_pool[index] = malloc(sizeof(CONSTANT_Integer_info));
    CONSTANT_Integer_info *tmp = (CONSTANT_Integer_info *)cf->constant_pool[index];
    tmp->tag = CONSTANT_Integer;
    tmp->bytes = read_u4(cf->file);
}

void read_float_info(struct class_file *cf, int index) {
    cf->constant_pool[index] = malloc(sizeof(CONSTANT_Float_info));
    CONSTANT_Float_info *tmp = (CONSTANT_Float_info *)cf->constant_pool[index];
    tmp->tag = CONSTANT_Float;
    tmp->bytes = read_u4(cf->file);
}

void read_long_info(struct class_file *cf, int index) {
    cf->constant_pool[index] = malloc(sizeof(CONSTANT_Long_info));
    CONSTANT_Long_info *tmp = (CONSTANT_Long_info *)cf->constant_pool[index];
    tmp->tag = CONSTANT_Long;
    tmp->high_bytes = read_u4(cf->file);
    tmp->low_bytes = read_u4(cf->file);
}

void read_double_info(struct class_file *cf, int index) {
    cf->constant_pool[index] = malloc(sizeof(CONSTANT_Double_info));
    CONSTANT_Double_info *tmp = (CONSTANT_Double_info *)cf->constant_pool[index];
    tmp->tag = CONSTANT_Double;
    tmp->high_bytes = read_u4(cf->file);
    tmp->low_bytes = read_u4(cf->file);
}

void read_nameandtype_info(struct class_file *cf, int index) {
    cf->constant_pool[index] = malloc(sizeof(CONSTANT_NameAndType_info));
    CONSTANT_NameAndType_info *tmp = (CONSTANT_NameAndType_info *)cf->constant_pool[index];
    tmp->tag = CONSTANT_NameAndType;
    tmp->name_index = read_u2(cf->file);
    tmp->descriptor_index = read_u2(cf->file);
}

void read_utf8_info(struct class_file *cf, int index) {
    cf->constant_pool[index] = malloc(sizeof(CONSTANT_Utf8_info));
    CONSTANT_Utf8_info *tmp = (CONSTANT_Utf8_info *)cf->constant_pool[index];
    tmp->tag = CONSTANT_Utf8;
    tmp->length = read_u2(cf->file);
    tmp->bytes = malloc(tmp->length);
    fread(tmp->bytes, sizeof(tmp->bytes[0]), tmp->length, cf->file);
}

void read_methodhandle_info(struct class_file *cf, int index) {
    cf->constant_pool[index] = malloc(sizeof(CONSTANT_MethodHandle_info));
    CONSTANT_MethodHandle_info *tmp = (CONSTANT_MethodHandle_info *)cf->constant_pool[index];
    tmp->tag = CONSTANT_MethodHandle;
    tmp->reference_kind = read_u1(cf->file);
    tmp->reference_index = read_u2(cf->file);
}

void read_methodtype_info(struct class_file *cf, int index) {
    cf->constant_pool[index] = malloc(sizeof(CONSTANT_MethodType_info));
    CONSTANT_MethodType_info *tmp = (CONSTANT_MethodType_info *)cf->constant_pool[index];
    tmp->tag = CONSTANT_MethodType;
    tmp->descriptor_index = read_u2(cf->file);
}

void read_invokedynamic_info(struct class_file *cf, int index) {
    cf->constant_pool[index] = malloc(sizeof(CONSTANT_InvokeDynamic_info));
    CONSTANT_InvokeDynamic_info *tmp = (CONSTANT_InvokeDynamic_info *)cf->constant_pool[index];
    tmp->tag = CONSTANT_InvokeDynamic;
    tmp->bootstrap_method_attr_index = read_u2(cf->file);
    tmp->name_and_type_index = read_u2(cf->file);
}

void unknown(struct class_file *cf, int index) {
    printf("Unknown tag! Aborting! (index: %d)\n", index);
    cf->next = 0;
}

void (*read_tag(int c))(struct class_file *, int) {
    switch (c) {
        case CONSTANT_Class: return read_class_info;
        case CONSTANT_Fieldref: return read_fieldref_info;
        case CONSTANT_Methodref: return read_methodref_info;
        case CONSTANT_InterfaceMethodref: return read_interfacemethodref_info;
        case CONSTANT_String: return read_string_info;
        case CONSTANT_Integer: return read_integer_info;
        case CONSTANT_Float: return read_float_info;
        case CONSTANT_Long: return read_long_info;
        case CONSTANT_Double: return read_double_info;
        case CONSTANT_NameAndType: return read_nameandtype_info;
        case CONSTANT_Utf8: return read_utf8_info;
        case CONSTANT_MethodHandle: return read_methodhandle_info;
        case CONSTANT_MethodType: return read_methodtype_info;
        case CONSTANT_InvokeDynamic: return read_invokedynamic_info;
        default: return unknown;
    }
}

void read_constant_pool(struct class_file *cf) {
    int c, i = 0;
    cf->constant_pool = malloc(sizeof(cf->constant_pool) * cf->constant_pool_count);

    if (!cf->constant_pool) {
        printf("no luck with malloc'ing constant_pool.\n");
        cf->next = 0;
        return;
    }

    while (i < cf->constant_pool_count - 1 && (c = fgetc(cf->file)) != EOF) {
        read_tag(c)(cf, i++);
    }

    cf->next = read_access_flags;
}

/* END Constant pool stuff */

/* more class file specific functions */

void read_access_flags(class_file *cf) {
    cf->access_flags = read_u2(cf->file);

    cf->next = read_this_class;
}

void read_this_class(class_file *cf) {
    cf->this_class = read_u2(cf->file);

    cf->next = read_super_class;
}

void read_super_class(class_file *cf) {
    cf->super_class = read_u2(cf->file);

    cf->next = read_interfaces_count;
}

/* END class file specific functions */

/* Interfaces may move to interfaces.c (does not currently exist). */

void read_interfaces_count(class_file *cf) {
    cf->interfaces_count = read_u2(cf->file);

    cf->next = read_interfaces;
}

void read_interfaces(class_file *cf) {
    for (int i = 0; i < cf->interfaces_count; i++) {
        // read interfaces here
        printf("read_interfaces: not yet implemented!\n");
    }

    cf->next = read_fields_count;
}

/* END interfaces */

/* Fields may move to fields.c (does not currently exist). */

void read_fields_count(class_file *cf) {
    cf->fields_count = read_u2(cf->file);

    cf->next = read_fields;
}

void read_fields(class_file *cf) {
    for (int i = 0; i < cf->fields_count; i++) {
        // read fields
        printf("read_fields: not yet implemented!\n");
    }

    cf->next = read_methods_count;
}

/* END fields */

/* Methods will almost certainly need to be moved to a different file. */

void read_methods_count(class_file *cf) {
    cf->methods_count = read_u2(cf->file);

    cf->next = read_methods;
}

// Follows non-terminals in CP to a terminal and returns that tag.
cp_info * get_cp(class_file *cf, u2 index) {
    u2 tmp = cf->constant_pool[index]->tag; // this seems wrong
    if (tmp == CONSTANT_Utf8) {
        return cf->constant_pool[index];
    } else {
        return get_cp(cf, tmp);
    }
}

void build_attributes(class_file *, attribute_info *);

void read_code_attribute(class_file *cf, attribute_info *ai) {
    // attribute_name_index is already set.
    // I think attribute_length could also be set.
    // it is now.
    Code_attribute *tmp = (Code_attribute *)ai;
    tmp->max_stack = read_u2(cf->file);
    tmp->max_locals = read_u2(cf->file);
    tmp->code_length = read_u4(cf->file);
    tmp->code = malloc(tmp->code_length);
    // This seems to be off by one? As in it's re-reading the byte used in
    // tmp->code_length.
    tmp->code = fread(tmp->code, sizeof(tmp->code[0]), tmp->code_length, cf->file); 
    tmp->exception_table_length = read_u2(cf->file);
    for (int i = 0; i < tmp->exception_table_length; i++) {
        printf("READING EXCEPTION TABLE NOT IMPLEMENTED!\n");
        /* exception_table->start_pc */ read_u2(cf->file);
        /* exception_table->end_pc */ read_u2(cf->file);
        /* exception_table->handler_pc */ read_u2(cf->file);
        /* exception_table->catch_type */ read_u2(cf->file);
    }
    tmp->attributes_count = read_u2(cf->file);
    tmp->attributes = malloc(sizeof(attribute_info) * tmp->attributes_count);
    for (int i = 0; i < tmp->attributes_count; i++) {
        // Something is wrong.
        build_attributes(cf, &(tmp->attributes[i]));
    }
}

// XXX: This is going to be a weird one. Since there is a whole separate
// `attributes` field in the class file, but both field_info and method_info
// also have their own attribute_info members. So it seems like a common method
// that builds it up will be best. However, it will also need to handle the
// reading. Alternatively, it would take a class_file * as normal, and then both
// `methods` and `fields` would need to create an empty class_file, the
// attributes would be read into that temporary object by `read_attributes`, and
// then `read_methods` and `read_fields` would just extract the relevant data.
// This seems less than ideal, but I'm somewhat constrained by the current FSM
// implementation.
// Bleh.
// XXX
void build_attributes(class_file *cf, attribute_info *ai) {
    ai->attribute_name_index = read_u2(cf->file);
    ai->attribute_length = read_u4(cf->file);

    // based on the above, we need to jump to a different method.
    cp_info *tmp = get_cp(cf, ai->attribute_name_index - 1); // silly offset.
    char *attr = NULL;
    switch (tmp->tag) {
        case CONSTANT_Utf8:
            attr = malloc(((CONSTANT_Utf8_info *)tmp)->length + 1);
            sprintf(attr, "%*s", ((CONSTANT_Utf8_info *)tmp)->length,
                    ((CONSTANT_Utf8_info *)tmp)->bytes);
            attr[((CONSTANT_Utf8_info *)tmp)->length] = '\0';
            break;
        default:
            printf("not yet implemented!\n");
    }
    if (strcmp(attr, "Code") == 0) {
        printf("got code attribute!\n");
        read_code_attribute(cf, ai);
    } else {
        printf("didn't get code! got: %s\n", attr);
    }
    //ai->attribute_length = read_u4(cf->file);
    ai->info = malloc(sizeof(ai[0]) * ai->attribute_length);
    fread(ai->info, sizeof(ai->info[0]), ai->attribute_length, cf->file);
    //printf("ai->info: %#x\n", ai->info);
}

void read_methods(class_file *cf) {
    cf->methods = malloc(sizeof(method_info) * cf->methods_count);
    //printf("in read_methods; at loc: %ld\n", ftell(cf->file));

    if (!cf->methods) {
        printf("no luck allocating methods.\n");
        cf->next = 0;
        return;
    }

    for (int i = 0; i < cf->methods_count; i++) {
        cf->methods[i] = malloc(sizeof(method_info));
        cf->methods[i]->access_flags = read_u2(cf->file);
        cf->methods[i]->name_index = read_u2(cf->file);
        cf->methods[i]->descriptor_index = read_u2(cf->file);
        cf->methods[i]->attributes_count = read_u2(cf->file);
        cf->methods[i]->attributes = malloc(sizeof(attribute_info) * cf->methods[i]->attributes_count);
        build_attributes(cf, cf->methods[i]->attributes);
    }

    cf->next = read_attributes_count;
}

/* END methods */

/* Attributes will certainly need to be moved somewhere else. */

void read_attributes_count(class_file *cf) {
    cf->attributes_count = read_u2(cf->file);

    cf->next = read_attributes;
}

void read_attributes(class_file *cf) {
    cf->attributes = malloc(sizeof(attribute_info) * cf->attributes_count);

    if (!cf->attributes) {
        printf("no luck allocating attributes.\n");
        cf->next = 0;
        return;
    }

    for (int i = 0; i < cf->attributes_count; i++) {
        cf->attributes[i] = malloc(sizeof(attribute_info));
        //build_attributes(cf->attributes[i], cf->file);
        build_attributes(cf, cf->attributes[i]);
    }

    cf->next = 0;
}

/* END Attributes */

void destroy(class_file *cf) {
    // free all malloc'd memory
}

// This isn't the best place for this probably?

void print_cf(class_file *cf) {
    printf(
            "magic: %X\n\
minor_version: %u\n\
major_version: %u\n\
constant_pool_count: %u\n",
            cf->magic,
            cf->minor_version,
            cf->major_version,
            cf->constant_pool_count);

    // print_constant_pool should take a class_file * and handle all of this.
    for (int i = 1; i < cf->constant_pool_count; i++) {
        printf("constant_pool[%u]: ", i);
        // this should be able to be generalized for all of the arrays, I think.
        print_constant_pool(cf->constant_pool[i - 1]);
    }

    printf(
            "access_flags: %X\n\
this_class: %u\n\
super_class: %u\n\
interfaces_count: %u\n",
            cf->access_flags,
            cf->this_class,
            cf->super_class,
            cf->interfaces_count);

    // print_interfaces should take a class_file * and handle this.
    for (int i = 0; i < cf->interfaces_count; i++) {
        printf("interfaces[%u]: ", i);
        // this should be able to be generalized for all of the arrays, I think.
        print_interfaces(cf->interfaces[i]);
    }

    // print_fields should take a class_file * and handle this.
    printf("fields_count: %u\n", cf->fields_count);
    for (int i = 0; i < cf->fields_count; i++) {
        printf("fields[%u]: ", i);
        // this should be able to be generalized for all of the arrays, I think.
        print_fields(cf->fields[i]);
    }

    // print_methods should take a class_file * and handle this.
    printf("methods_count: %u\n", cf->methods_count);
    for (int i = 0; i < cf->methods_count; i++) {
        printf("methods[%u]: ", i);
        // this should be able to be generalized for all of the arrays, I think.
        if (cf->methods[i])
            print_methods(cf->methods[i]);
        else
            printf("\n");
    }

    // print_attributes should take a class_file * and handle this.
    printf("attributes_count: %u\n", cf->attributes_count);
    for (int i = 0; i < cf->attributes_count; i++) {
        printf("attributes[%u]: ", i);
        // this should be able to be generalized for all of the arrays, I think.
        if (cf->attributes[i])
            print_attributes(cf->attributes[i]);
        else
            printf("\n");
    }
}

int main(int argc, char * argv[]) {
    struct class_file cf = { read_magic, 0 };
    if (argc < 2) {
        printf("Usage: %s <classfile>\n", argv[0]);
        return 1;
    }

    cf.file = fopen(argv[1], "rb");
    if (!cf.file) {
        printf("Could not open file %s\n", argv[1]);
        return 2;
    }

    while (cf.next) {
        cf.next(&cf);
    }

    print_cf(&cf);

    destroy(&cf);

    return 0;
}
