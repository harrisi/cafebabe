// function pointer with state implementation
// (built upon from https://stackoverflow.com/a/1383453)

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "cafebabe-fp.h"

u1 read_u1(FILE *f) {
    u1 tmp = 0;

    // XXX: this can give EOF
    // which is an error because it's -1, but tmp can't hold that.
    // let's just assume no EOF!
    tmp = fgetc(f);

    return tmp;
}

u2 read_u2(FILE *f) {
    u2 tmp = 0;
    u2 i = 0;

    while (i < sizeof(u2)) {
        // XXX: this can give EOF
        // which is an error because it's -1, but tmp can't hold that.
        // let's just assume no EOF!
        tmp |= fgetc(f);
        if (i + 1 < sizeof(u2)) {
            tmp <<= 8;
        }
        i++;
    }

    return tmp;
}

u4 read_u4(FILE *f) {
    u4 tmp = 0;
    u4 i = 0;

    while (i < sizeof(u4)) {
        // XXX: this can give EOF
        // which is an error because it's -1, but tmp can't hold that.
        // let's just assume no EOF!
        tmp |= fgetc(f);
        if (i + 1 < sizeof(u4)) {
            tmp <<= 8;
        }
        i++;
    }

    return tmp;
}

struct cp_printing {
    int tag;
    void (*func)(cp_info *);
};

void print_constant_class_info(cp_info *cp) {
    printf("Class| tag: %u; name_index: %u\n",
            cp->tag,
            ((CONSTANT_Class_info *)cp)->name_index);
}

void print_constant_fieldref_info(cp_info * cp) {
    CONSTANT_Fieldref_info *tmp = (CONSTANT_Fieldref_info *)cp;
    printf("Fieldref| tag: %u; class_index: %u; name_and_type_index: %u\n",
            cp->tag,
            tmp->class_index,
            tmp->name_and_type_index);
}

void print_constant_methodref_info(cp_info * cp) {
    CONSTANT_Methodref_info *tmp = (CONSTANT_Methodref_info *)cp;
    printf("Methodref| tag: %u; class_index: %u; name_and_type_index: %u\n",
            cp->tag,
            tmp->class_index,
            tmp->name_and_type_index);
}

void print_constant_interfacemethodref_info(cp_info * cp) {
    CONSTANT_InterfaceMethodref_info *tmp = (CONSTANT_InterfaceMethodref_info *)cp;
    printf("InterfaceMethodref| tag: %u; class_index: %u; name_and_type_index: %u\n",
            cp->tag,
            tmp->class_index,
            tmp->name_and_type_index);
}

void print_constant_string_info(cp_info * cp) {
    printf("String| tag: %u; string_index: %u\n",
            cp->tag,
            ((CONSTANT_String_info *)cp)->string_index);
}

void print_constant_integer_info(cp_info * cp) {
    printf("Integer| tag: %u; bytes: %u\n",
            cp->tag,
            ((CONSTANT_Integer_info *)cp)->bytes);
}

void print_constant_float_info(cp_info * cp) {
    printf("Float| tag: %u; bytes: %u\n",
            cp->tag,
            ((CONSTANT_Float_info *)cp)->bytes);
}

void print_constant_long_info(cp_info * cp) {
    CONSTANT_Long_info *tmp = (CONSTANT_Long_info *)cp;
    printf("Long| tag: %u; high_bytes: %u; low_bytes: %u\n",
            cp->tag,
            tmp->high_bytes,
            tmp->low_bytes);
}

void print_constant_double_info(cp_info * cp) {
    CONSTANT_Double_info *tmp = (CONSTANT_Double_info *)cp;
    printf("Double| tag: %u; high_bytes: %u; low_bytes: %u\n",
            cp->tag,
            tmp->high_bytes,
            tmp->low_bytes);
}

void print_constant_nameandtype_info(cp_info * cp) {
    CONSTANT_NameAndType_info *tmp = (CONSTANT_NameAndType_info *)cp;
    printf("NameAndType| tag: %u; name_index: %u; descriptor_index: %u\n",
            cp->tag,
            tmp->name_index,
            tmp->descriptor_index);
}

void print_constant_utf8_info(cp_info * cp) {
    CONSTANT_Utf8_info *tmp = (CONSTANT_Utf8_info *)cp;
    printf("Utf8| tag: %u; length: %u; bytes: %s\n",
            cp->tag,
            tmp->length,
            tmp->bytes);
}

void print_constant_methodhandle_info(cp_info * cp) {
    CONSTANT_MethodHandle_info *tmp = (CONSTANT_MethodHandle_info *)cp;
    printf("MethodHandle| tag: %u; reference_kind: %u; reference_index: %u\n",
            cp->tag,
            tmp->reference_kind,
            tmp->reference_index);
}

void print_constant_methodtype_info(cp_info * cp) {
    printf("MethodType| tag: %u; descriptor_index: %u\n",
            cp->tag,
            ((CONSTANT_MethodType_info *)cp)->descriptor_index);
}

void print_constant_invokedynamic_info(cp_info * cp) {
    CONSTANT_InvokeDynamic_info *tmp = (CONSTANT_InvokeDynamic_info *)cp;
    printf("InvokeDynamic| tag: %u; bootstrap_method_attr_index: %u; name_and_type_index: %u\n",
            cp->tag,
            tmp->bootstrap_method_attr_index,
            tmp->name_and_type_index);
}

struct cp_printing cp_print[] = {
    { 0, 0 }, // unused
    { CONSTANT_Utf8,               print_constant_utf8_info },
    { 0, 0 }, // unused
    { CONSTANT_Integer,            print_constant_integer_info },
    { CONSTANT_Float,              print_constant_float_info },
    { CONSTANT_Long,               print_constant_long_info },
    { CONSTANT_Double,             print_constant_double_info },
    { CONSTANT_Class,              print_constant_class_info },
    { CONSTANT_String,             print_constant_string_info },
    { CONSTANT_Fieldref,           print_constant_fieldref_info },
    { CONSTANT_Methodref,          print_constant_methodref_info },
    { CONSTANT_InterfaceMethodref, print_constant_interfacemethodref_info },
    { CONSTANT_NameAndType,        print_constant_nameandtype_info },
    { 0, 0 }, // unused
    { 0, 0 }, // unused
    { CONSTANT_MethodHandle,       print_constant_methodhandle_info },
    { CONSTANT_MethodType,         print_constant_methodtype_info },
    { 0, 0 }, // unused
    { CONSTANT_InvokeDynamic,      print_constant_invokedynamic_info }
};

// XXX: This is not scalable or maintanable.
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

void print_cf(struct class_file *cf) {
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
"access_flags: %u\n\
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
        print_methods(cf->methods[i]);
    }

    // print_attributes should take a class_file * and handle this.
    printf("attributes_count: %u\n", cf->attributes_count);
    for (int i = 0; i < cf->attributes_count; i++) {
        printf("attributes[%u]: ", i);
        // this should be able to be generalized for all of the arrays, I think.
        print_attributes(cf->attributes[i]);
    }
}

void read_magic(struct class_file *cf) {
//    printf("%s\n", __func__);
    cf->magic = read_u4(cf->file);

    cf->next = read_minor_version;
}

void read_minor_version(struct class_file *cf) {
//    printf("%s\n", __func__);
    cf->minor_version = read_u2(cf->file);

    cf->next = read_major_version;
}

void read_major_version(struct class_file *cf) {
//    printf("%s\n", __func__);
    cf->major_version = read_u2(cf->file);

    cf->next = read_constant_pool_count;
}

void read_constant_pool_count(struct class_file *cf) {
//    printf("%s\n", __func__);
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
    tmp->bytes = malloc(tmp->length + 1);
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

// XXX: This is causing issues because I'm not advancing the file regularly.
// I think the real issue is in read_tag and read_constant_* functions.
void read_constant_pool(struct class_file *cf) {
//    printf("%s\n", __func__);
    int c, i = 0;
    // alloc memory, but I'm not sure how much?
    cf->constant_pool = malloc(sizeof(cf->constant_pool) * cf->constant_pool_count);
    for (int i = 0; i < cf->constant_pool_count; i++) {
        cf->constant_pool[i] = NULL;
    }
    if (!cf->constant_pool) {
        printf("no luck with malloc'ing constant_pool.\n");
        cf->next = 0;
        return;
    }
    while (i < cf->constant_pool_count - 1 && (c = fgetc(cf->file)) != EOF) {
        read_tag(c)(cf, i++);
        // set cf->next..
    }
    cf->next = 0;
}

void destroy(class_file *cf) {
    // free all malloc'd memory
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
