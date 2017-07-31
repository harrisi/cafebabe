#ifndef CAFEBABE_ATTRIBUTES_H
#define CAFEBABE_ATTRIBUTES_H

typedef struct ConstantValue_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 constantvalue_index;
} ConstantValue_attribute;

typedef struct Code_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 max_stack;
    u2 max_locals;
    u4 code_length;
    u1 *code; // [code_length]
    u2 exception_table_length;
    struct {
        u2 start_pc;
        u2 end_pc;
        u2 handler_pc;
        u2 catch_type;
    } *exception_table; // [exception_table_length]
    u2 attributes_count;
    attribute_info *attributes; // [attributes_count]
} Code_attribute;



#endif
