// element_purge_mapping.h
#ifndef ELEMENT_PURGE_MAPPING_H
#define ELEMENT_PURGE_MAPPING_H
// Define function pointer type for element purging
typedef void (*ElementPurgeFunc)(void *element);

typedef struct {
    const char *type_name;
    ElementPurgeFunc purge_func;

} ElementPurgeMapping;

#endif // ELEMENT_PURGE_MAPPING_H
