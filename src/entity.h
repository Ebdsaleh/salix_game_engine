// entity.h
#ifndef ENTITY_H
#define ENTITY_H
#include <SDL2/SDL.h>
#include <stdint.h>
#include "element.h"

typedef struct {
    Id id;
    char *name;
    Element **elements;
    int32_t element_count;
} Entity; 


// Function to create and entity with out any elements added.
// For use during de-serialization
Entity *entity_create_for_deserialization();
// Function to create an entity with a Transform.
// For use inside of editor
Entity *entity_create();
// Function to add an element to an entity
void entity_add_element(Entity *entity, void *element);

// Function to remove an element from an entity
void entity_remove_element(Entity *entity, Element *element);

// Function to clear all elements from an entity
void entity_purge_elements(Entity *entity);

// Function to get an element from an entity by name
Element *entity_get_element(Entity *entity, const char *name);

// Function to set name of an entity
void entity_set_name(Entity *entity, const char *name);

char *entity_get_name(Entity *entity);

// Function to get Entity by its name
Entity *entity_get_by_name(const char * name);

// Function to calculate the size of an entity
size_t entity_calculate_size(Entity *entity);

// Function to serialize an entity
char *entity_serialize(Entity *entity);

// Function de-serialize an entity from a buffer
Entity *entity_deserialize(char * buffer, size_t offset);

#endif // ENTITY_H 
