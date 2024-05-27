// element_size_mapping.h
#ifndef ELEMENT_SIZE_MAPPING_H
#define ELEMENT_SIZE_MAPPING_H
#include <stdio.h>

// Define function pointer type for element size calculation
typedef size_t(*ElementCalculateFunc)(void *element);


// Define mapping for element size calculation
typedef struct {
    const char *type_name;
    ElementCalculateFunc calculate_func;
} ElementSizeMapping;

#endif // ELEMENT_SIZE_MAPPING_H
