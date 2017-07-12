// function pointer with state implementation
// (source: https://stackoverflow.com/a/1383453)

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct state;
struct class_file;
typedef void state_fn(struct state *);
typedef void class_file_fn(struct class_file *);

typedef uint8_t  u1;
typedef uint16_t u2;
typedef uint32_t u4;

struct state {
    state_fn * next;
    int i; // data
};

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

struct class_file {
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
    cp_info         *constant_pool;//[constant_pool_count-1]
    u2              access_flags;
    u2              this_class;
    u2              super_class;
    u2              interfaces_count;
    u2              *interfaces;//[interfaces_count]
    u2              fields_count;
    field_info      *fields;//[fields_count]
    u2              methods_count;
    method_info     *methods;//[methods_count]
    u2              attributes_count;
    attribute_info  *attributes;//[attributes_count]
};

state_fn foo, bar;

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

void foo(struct state * state) {
    printf("%s %i\n", __func__, ++state->i);
    state->next = bar;
}

void bar(struct state * state) {
    printf("%s %i\n", __func__, ++state->i);
    state->next = state->i < 10 ? foo : 0;
}

void print_constant_pool(cp_info *cp) {
    printf("not yet implemented\n");
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
    for (int i = 0; i < cf->interfaces_count; i++) {
        printf("interfaces[%u]: ", i);
        // this should be able to be generalized for all of the arrays, I think.
        print_interfaces(&cf->interfaces[i]);
    }
    printf("fields_count: %u\n", cf->fields_count);
    for (int i = 0; i < cf->fields_count; i++) {
        printf("fields[%u]: ", i);
        // this should be able to be generalized for all of the arrays, I think.
        print_fields(&cf->fields[i]);
    }
    printf("methods_count: %u\n", cf->methods_count);
    for (int i = 0; i < cf->methods_count; i++) {
        printf("methods[%u]: ", i);
        // this should be able to be generalized for all of the arrays, I think.
        print_methods(&cf->methods[i]);
    }
    printf("attributes_count: %u\n", cf->attributes_count);
    for (int i = 0; i < cf->attributes_count; i++) {
        printf("attributes[%u]: ", i);
        // this should be able to be generalized for all of the arrays, I think.
        print_attributes(&cf->attributes[i]);
    }
    printf("\n");
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

    cf->next = 0;
}

#define CONSTANT_Class 7
#define CONSTANT_Fieldref 9
#define CONSTANT_NameAndType 12

void read_constant_class_info(struct class_file *cf, int index) {

}

void read_constant_fieldref_info(struct class_file *cf, int index) {

}

typedef struct CONSTANT_NameAndType_info {
    u1 tag;
    u2 name_index;
    u2 descriptor_index;
} CONSTANT_NameAndType_info;

u2 read_u2(FILE *f) {
    return 0;
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

void (*read_tag(int c))(struct class_file *, int) {
    switch (c) {
        case CONSTANT_Class: return read_constant_class_info;
        case CONSTANT_Fieldref: return read_constant_fieldref_info;
        case CONSTANT_NameAndType: return read_nameandtype_info;
        default: return 0;
    }
}

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
        read_tag(c)(cf, i);
        // set cf->next..
    }
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
