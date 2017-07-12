// function pointer with state implementation
// (built upon from https://stackoverflow.com/a/1383453)

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "cafebabe-fp.h"

// XXX: This is not scalable or maintanable.
void print_constant_pool(cp_info *cp) {
    switch (cp->tag) {
        case CONSTANT_Methodref: printf("method ref| tag: %u; class_index: %u; name_and_type_index: %u\n",
                                         cp->tag,
                                         ((CONSTANT_Methodref_info *)cp)->class_index,
                                         ((CONSTANT_Methodref_info *)cp)->name_and_type_index);
                                 break;
        default: printf("not yet implemented\n");
    }
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
    for (int i = 0; i < cf->constant_pool_count; i++) {
        printf("constant_pool[%u]: ", i);
        // this should be able to be generalized for all of the arrays, I think.
        print_constant_pool(&cf->constant_pool[i]);
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
        print_interfaces(&cf->interfaces[i]);
    }

    // print_fields should take a class_file * and handle this.
    printf("fields_count: %u\n", cf->fields_count);
    for (int i = 0; i < cf->fields_count; i++) {
        printf("fields[%u]: ", i);
        // this should be able to be generalized for all of the arrays, I think.
        print_fields(&cf->fields[i]);
    }

    // print_methods should take a class_file * and handle this.
    printf("methods_count: %u\n", cf->methods_count);
    for (int i = 0; i < cf->methods_count; i++) {
        printf("methods[%u]: ", i);
        // this should be able to be generalized for all of the arrays, I think.
        print_methods(&cf->methods[i]);
    }

    // print_attributes should take a class_file * and handle this.
    printf("attributes_count: %u\n", cf->attributes_count);
    for (int i = 0; i < cf->attributes_count; i++) {
        printf("attributes[%u]: ", i);
        // this should be able to be generalized for all of the arrays, I think.
        print_attributes(&cf->attributes[i]);
    }
}

void read_magic(struct class_file *cf) {
    printf("%s\n", __func__);
    int c, i = 0;
    while (i < sizeof(cf->magic) && (c = fgetc(cf->file)) != EOF) {
        cf->magic |= c;
        // can this branch be removed?
        if (i + 1 < sizeof(cf->magic))
            cf->magic <<= 8;
        i++;
    }

    cf->next = read_minor_version;
}

void read_minor_version(struct class_file *cf) {
    printf("%s\n", __func__);
    int c, i = 0;
    while (i < sizeof(cf->minor_version) && (c = fgetc(cf->file)) != EOF) {
        cf->minor_version |= c;
        if (i + i < sizeof(cf->minor_version))
            cf->minor_version <<= 8;
        i++;
    }

    cf->next = read_major_version;
}

void read_major_version(struct class_file *cf) {
    printf("%s\n", __func__);
    int c, i = 0;
    while (i < sizeof(cf->major_version) && (c = fgetc(cf->file)) != EOF) {
        cf->major_version |= c;
        if (i + i < sizeof(cf->major_version))
            cf->major_version <<= 8;
        i++;
    }

    cf->next = read_constant_pool_count;
}

void read_constant_pool_count(struct class_file *cf) {
    printf("%s\n", __func__);
    int c, i = 0;
    while (i < sizeof(cf->constant_pool_count) && (c = fgetc(cf->file)) != EOF) {
        cf->constant_pool_count |= c;
        if (i + 1 < sizeof(cf->constant_pool_count))
            cf->constant_pool_count <<= 8;
        i++;
    }

    cf->next = read_constant_pool;
}

void read_constant_class_info(struct class_file *cf, int index) {

}

void read_constant_fieldref_info(struct class_file *cf, int index) {

}

void read_constant_methodref_info(struct class_file *cf, int index) {

}

typedef struct CONSTANT_NameAndType_info {
    u1 tag;
    u2 name_index;
    u2 descriptor_index;
} CONSTANT_NameAndType_info;

u2 read_u2(FILE *f) {
    u2 tmp = 0;
    u2 i = 0;
    while (i < sizeof(u2)) {
        // XXX: this can give EOF
        tmp |= fgetc(f);
        if (i + 1 < sizeof(u2)) {
            tmp <<= 8;
        }
        i++;
    }
    return tmp;
}

// I'll have to look at the struct stacking polymorphism deal for this.

void read_nameandtype_info(struct class_file *cf, int index) {
    // set tag
    // will have value 12 always (0x0a)
    //cf->constant_pool[index] = malloc(sizeof(CONSTANT_NameAndType_info));
    //cf->constant_pool[index].tag = 12;
    //cf->constant_pool[index].name_index = read_u2(cf->file);
    //cf->constant_pool[index].descriptor_index = read_u2(cf->file);
}

void unknown(struct class_file *cf, int index) {

}

void read_methodref_info(struct class_file *cf, int index) {
    CONSTANT_Methodref_info *self = malloc(sizeof(CONSTANT_Methodref_info));
    self->tag = CONSTANT_Methodref;
    self->class_index = read_u2(cf->file);
    self->class_index = read_u2(cf->file);
}

void (*read_tag(int c))(struct class_file *, int) {
    switch (c) {
        case CONSTANT_Class: return read_constant_class_info;
        case CONSTANT_Fieldref: return read_constant_fieldref_info;
        case CONSTANT_NameAndType: return read_nameandtype_info;
        case CONSTANT_Methodref: return read_methodref_info;
        default: return unknown;
    }
}

// XXX: This is causing issues because I'm not advancing the file regularly.
// I think the real issue is in read_tag and read_constant_* functions.
void read_constant_pool(struct class_file *cf) {
    printf("%s\n", __func__);
    int c, i = 0;
    // alloc memory, but I'm not sure how much?
    cf->constant_pool = malloc(sizeof(cf->constant_pool) * cf->constant_pool_count);
    if (!cf->constant_pool) {
        printf("no luck with malloc'ing constant_pool.\n");
        cf->next = 0;
        return;
    }
    while (i < cf->constant_pool_count && (c = fgetc(cf->file)) != EOF) {
        read_tag(c)(cf, i++);
        // set cf->next..
    }
    cf->next = 0;
}

int main(int argc, char * argv[]) {
//    struct state state = { foo, 0 };
//    while (state.next) {
//        state.next(&state);
//    }

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

    return 0;
}
