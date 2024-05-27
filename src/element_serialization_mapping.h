// element_serialization_mapping.h
#ifndef ELEMENT_SERIALIZATION_MAPPING_H
#define ELEMENT_SERIALIZATION_MAPPING_H
#include <stdio.h>
// Define function pointer type for element serializtion
typedef char *(*ElementSerializeFunc)(void *element);

// Define function pointer type for element de-serialization
typedef void *(*ElementDeserializeFunc)(char *buffer, size_t offset, void *owner);

// Define mappings for element serialization and de-serialization
typedef struct {
    const char *id_prefix;
    ElementSerializeFunc serialize_func;
    ElementDeserializeFunc deserialize_func;

} ElementSerializationMapping;

#endif // ELEMENT_SERIALIZTION_MAPPING_H
