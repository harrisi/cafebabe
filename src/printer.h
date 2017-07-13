//
// Created by Ian Harris on 17/7/13.
//

#ifndef CAFEBABE_PRINTER_H
#define CAFEBABE_PRINTER_H

#include "constantpool.h"

void print_constant_utf8_info(cp_info *);

void print_constant_integer_info(cp_info *);

void print_constant_float_info(cp_info *);

void print_constant_long_info(cp_info *);

void print_constant_double_info(cp_info *);

void print_constant_class_info(cp_info *);

void print_constant_string_info(cp_info *);

void print_constant_fieldref_info(cp_info *);

void print_constant_methodref_info(cp_info *);

void print_constant_interfacemethodref_info(cp_info *);

void print_constant_nameandtype_info(cp_info *);

void print_constant_methodhandle_info(cp_info *);

void print_constant_methodtype_info(cp_info *);

void print_constant_invokedynamic_info(cp_info *);

void print_constant_pool(cp_info *);

void print_interfaces(u2 *);

void print_fields(field_info *);

void print_methods(method_info *);

void print_attributes(attribute_info *);

// XXX: Not sure if this is even needed.
struct cp_printing {
    int tag;

    void (*func)(cp_info *);
};

struct cp_printing cp_print[19];

#endif //CAFEBABE_PRINTER_H
