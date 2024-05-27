// element.h
#ifndef ELEMENT_H
#define ELEMENT_H
#include <stdio.h>
#include "id.h"
#include "element_init_mapping.h"
#include "element_name_mapping.h"
#include "element_purge_mapping.h"
#include "element_serialization_mapping.h"
#include "element_size_mapping.h"
#include "element_type.h"

typedef struct {
    Id id;
    char *name;
    ElementType *type; 
} Element;

extern InitFuncMapping init_mappings[];
extern ElementNameMapping name_mappings[];
extern ElementPurgeMapping purge_mappings[];
extern ElementSerializationMapping serialization_mappings[];
extern ElementSizeMapping calculate_size_mappings[];
void element_init(Element *element, void *owner);
void element_set_name(Element *element, const char *new_name);
char *element_get_name(Element * element);
void element_purge(Element *element);

size_t element_calculate_size(Element *element);
char *element_serialize(Element *element);
void *element_deserialize(char *buffer, size_t offset, void* owner);
#endif // ELEMENT_H
