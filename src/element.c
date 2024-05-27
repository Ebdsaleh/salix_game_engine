// element.c
#include "element.h"
#include <stdio.h>
#include <stddef.h>
#include "element_init_mapping.h"
#include "element_purge_mapping.h"
#include "element_serialization_mapping.h"
#include "element_size_mapping.h"
#include "element_type.h"
#include <stdlib.h>
#include <string.h>


// MAPPINGS

// Forward delcaration of each element_types init function is required.
void transform_init(void *element, void *owner);
void square_init(void *element, void *owner);

// Define the init mappings array
InitFuncMapping init_mappings[] = {
    {"Transform", &transform_init },
    {"Square", &square_init },
};


// Forward declaration of each element's naming operations function is required
void transform_set_name(void *element, const char *new_name);
char *transform_get_name(void *element);
void square_set_name(void *element, const char *new_name);
char *square_get_name(void *element);

ElementNameMapping name_mappings[] = {
    {"Transform", &transform_set_name, &transform_get_name},
    {"Square", &square_set_name, &square_get_name},
};


// Forward declaration of each element's purge function is required.
void transform_purge(void *element);
void square_purge(void *element);

ElementPurgeMapping purge_mappings[] = {
    {"Transform", &transform_purge },
    {"Square", &square_purge },
};


// Forward delclaration of each elements calculate size functions
size_t transform_calculate_size(void *element);

ElementSizeMapping calculate_size_mappings[] = {
    {"Transform", &transform_calculate_size},
//    {"Square", &square_calculate_size},
}; 



// Forward declaration of each element's serialization and deserialization functions
char *transform_serialize(void*element);
void *transform_deserialize(char *buffer, size_t offset, void *owner);
char *square_serialize(void *element);
void *square_deserialize(char *buffer, size_t offset, void *owner);

ElementSerializationMapping serialization_mappings[] = {
    {"TRFM", &transform_serialize, &transform_deserialize},
    {"SQUR", &square_serialize, &square_deserialize},
};


// END MAPPINGS


// INIT FUNCTIONS
void element_init(Element *element, void *owner){
    if (element == NULL || element->type == NULL){
        fprintf(stderr,"ERROR: Element or ElementType is NULL.\n");
        return;
    }
    if (owner == NULL) {
        fprintf(stderr,"ERROR: Owner cannot be NULL when initializing Element\n");
        return;
    }
    // Find the appropriate element's init function to call
    for (int i = 0; i < (int)(sizeof(init_mappings) / sizeof(init_mappings[0])); i++) {
        if (strcmp(element->type->name, init_mappings[i].type_name) == 0) {
            // Call the init function associated with the element type
            init_mappings[i].init_func(element, owner);
            return;
        }
    }
}



// NAMING OPERATIONS
void element_set_name(Element *element, const char *new_name) {
    if (element == NULL || new_name == NULL) {
        fprintf(stderr, "ERROR: Invalid Element of new_name string.\n");
        return;
    }
    for (int i = 0; i < (int)(sizeof(name_mappings) / sizeof(name_mappings[0])); i++) {
        if (strcmp(element->type->name, name_mappings[i].type_name) == 0 ){
            name_mappings[i].set_name_func(element, new_name);
            return;
        }
    }


}


char *element_get_name(Element *element) {
    if (element == NULL) {
        fprintf(stderr, "ERROR: Invalid Element.\n");
        return NULL;
    }

    // Find the appropriate element's get_name function to call
    for (int i = 0; i < (int)(sizeof(name_mappings) / (int)sizeof(name_mappings[0])); i++) {
        if (strcmp(element->type->name, name_mappings[i].type_name) == 0) {
            // Call the get_name function associated with the element type.
            return name_mappings[i].get_name_func(element);
        
        }
    }
    return NULL;
}



// PURGING FUNCTIONS
void element_purge(Element *element){
    if (element == NULL) {
        printf("Inside 'element.c'\n");
        fprintf(stderr, "ERROR: Invalid Element, cannot purge.\n");
        return;
    }

    if (element->type == NULL || element->name == NULL){
        fprintf(stderr, "ERROR: Element 'type' or 'name' is not properly initialized.\n");
        return;
    }

    // Find the appropriate element's purge function to call
    for (int i = 0; i < (int)(sizeof(purge_mappings) / (int)sizeof(purge_mappings[0])); i++) {
        if (purge_mappings[i].type_name == NULL){
            fprintf(stderr, "ERROR: type name in purge_mappings[%d] is NULL.\n", i);
            continue;
        }
        
        if (strcmp(element->type->name, purge_mappings[i].type_name) == 0) {
            purge_mappings[i].purge_func(element);
            return;
        }
    }
    fprintf(stderr, "ERROR: No purge function found for element type '%s'.\n", element->type->name);
}


// Function to calculate the size of an element
size_t element_calculate_size(Element *element) {
    if (element == NULL) {
        fprintf(stderr, "ERROR: Cannot calculate the size of a NULL Element.\n");
        return -1;
    }
    
    // Find the approriate element's calculate size function to call
    for (int i = 0; i < (int)(sizeof(calculate_size_mappings) / (int)sizeof(calculate_size_mappings[0])); i++) {
        if (calculate_size_mappings[i].type_name == NULL) {
        fprintf(stderr, "ERROR: 'type_name' in 'calculate_size_mappings[%d] is NULL.\n", i);
        continue;
        }  

        if (strcmp(element->type->name, calculate_size_mappings[i].type_name) == 0) {
            return calculate_size_mappings[i].calculate_func(element);

        }

    }
    fprintf(stderr, "ERROR: No 'calculate_size' function found for element type '%s'\n", element->type->name);
    return -1;
}


char *element_serialize(Element *element) {
    // Check that element's type_name is not NULL
    if (element->id.prefix == NULL) {
        fprintf(stderr, "ERROR: Invalid Element passed in for serialization. 'id.prefix' is NULL");
        printf("Aborting Element serialization.\n");
        return NULL;
    }

    // Find the appropriate element's serialization function to call
    for (int i = 0; i < (int)(sizeof(serialization_mappings) / (int) sizeof(serialization_mappings[0])); i++) {
        // check for NULL id_prefix in serializatoin_mappings
        if (serialization_mappings[i].id_prefix == NULL) {
            fprintf(stderr, "ERROR: id prefix in serialization_mappings[%d] is NULL.\n", i);
            continue;
        }

        if (serialization_mappings[i].serialize_func == NULL) {
            fprintf(stderr, "ERROR: 'serialize_func' in serialization_mappings[%d] is NULL.\n", i);
            continue;
        }

                
        if (strcmp(element->id.prefix, serialization_mappings[i].id_prefix) == 0) {
            printf("Element 'id.prefix': '%s' found, calling serialize function.\n", element->id.prefix);
            return serialization_mappings[i].serialize_func(element);
            
        }
    }
    return NULL;
}

 
void *element_deserialize(char *buffer, size_t offset, void *owner) {
    if (buffer == NULL) {
        fprintf(stderr, "ERROR: Invalid 'buffer', cannot perform Element de-serialization. Received a NULL 'buffer'.\n");
        return NULL;
    }
    if (owner == NULL) {
        fprintf(stderr, "ERROR: Invalid Entity 'owner' cannot perform Element de-serialization.\n");
        return NULL;
    }
    if (offset == 0) {
        fprintf(stderr, "ERROR: Invalid 'offset' value, cannot perform Element de-serialization.\n");
        return NULL;
    }

    printf("'offset': %zu.\n", offset);
    size_t offset_copy = offset;
    printf("'offset_copy': %zu.\n", offset_copy);
    printf("'buffer' size, %zu\n", sizeof(buffer));
    size_t id_prefix_length = strlen(buffer + offset_copy) + 1;
    // Test read value
    printf("'id_prefix_length': %zu.\n", id_prefix_length);

    char *id_prefix = (char *)malloc(id_prefix_length);
    if (id_prefix ==  NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'id_prefix', cannot perform Element de-serialization.\n");
        return NULL;
    }
    memcpy(id_prefix, buffer + offset, id_prefix_length);
    offset += id_prefix_length;
    // Test read value
    printf("'id_prefix': '%s'.\n", id_prefix);
    
    // find the appropriate element's de-serialization function to call
    for (int i = 0; (int)(sizeof(serialization_mappings) / (int)sizeof(serialization_mappings[0])); i++) {
        // check for NULL id_prefix and deserialize_func in serialization_mappings
        if (serialization_mappings[i].id_prefix == NULL) {
            fprintf(stderr, "ERROR: id_prefix in 'serialization_mappings[%d]' is NULL.\n", i);
            continue;

        }

        if (serialization_mappings[i].deserialize_func == NULL) {
            fprintf(stderr, "ERROR: 'deserialize_func' in 'serialization_mappings[%d]' is NULL.\n", i);
            continue;
        }

        if (strcmp(id_prefix, serialization_mappings[i].id_prefix) == 0) {
            printf("Found a match: '%s'.\n", serialization_mappings[i].id_prefix);
            free(id_prefix);
            return serialization_mappings[i].deserialize_func(buffer, offset, owner);
        }
    }
    // no longer required.
    free(id_prefix); 
    return NULL;
}

