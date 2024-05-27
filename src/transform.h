// transform.h
#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "element.h"
#include "entity.h"
#include "vector.h"
#include "element_counts.h"

// Define Transform
typedef struct{
    Element base;
    Entity *owner;
    Vector3 *position;
    Vector3 *rotation;
    Vector3f *scale;
} Transform;

// Function pointer type for Transform initialization

Transform *transform_prepare_type();

// Function to initialize Transform
void transform_init(Element *element, Entity *owner);

// Function to set name of a Transform
void transform_set_name(Element *element, const char * new_name);

// Function to get name of a Transform
char *transform_get_name(Element *element);

// Function to purge a Transform from memory
void transform_purge(Element *element);

// Function to display the properties of the Transform to the console
void transform_show_properties(Element *element);

// Function to calculate the size of a Transform and all its members
size_t transform_calculate_size(Element *element);

// Function to serialize a Transform and return a buffer
char *transform_serialize(const Element *element);

// Function to de-serialize a Transform from a buffer
Transform *transform_deserialize(const char* buffer, size_t offset,  Entity *owner);
#endif // TRANSFORM_H 
