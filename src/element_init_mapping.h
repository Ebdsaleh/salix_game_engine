// element_init_mapping.h
#ifndef ELEMENT_INIT_MAPPING_H
#define ELEMENT_INIT_MAPPING_H

#include "element_type.h"

// Define InitFuncMapping
typedef struct {
    const char *type_name;
    ElementTypeInitFunc init_func;
} InitFuncMapping;

extern InitFuncMapping init_mappings[];
#endif // ELEMENT_INIT_MAPPING_H
