// element_name_mapping.h
#ifndef ELEMENT_NAME_MAPPING_H
#define ELEMENT_NAME_MAPPING_H

// Define function pointer type for element name setting
typedef void (*ElementSetNameFunc)(void *element, const char *name);

// Define function pointer type for element name getting
typedef char * (*ElementGetNameFunc)(void *element);
// Mapping structure for element name operations
typedef struct {
    const char *type_name;
    ElementSetNameFunc set_name_func;
    ElementGetNameFunc get_name_func;
} ElementNameMapping;

#endif // ELEMENT_NAME_MAPPING_H
