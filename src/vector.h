// vector.h
#ifndef VECTOR_H
#define VECTOR_H
#include<stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


// Define Vector3
typedef struct {
    int32_t x;
    int32_t y;
    int32_t z;
} Vector3;


// Define Vector3f
typedef struct {
    float x;
    float y;
    float z;
} Vector3f; 


// Define Vector2
typedef struct {
    int32_t x;
    int32_t y;
} Vector2;


// Define Vector2f 
typedef struct {
    float x;
    float y;
} Vector2f;


// Serialization functions

size_t vector3_calculate_size(Vector3 *vector);
size_t vector3f_calculate_size(Vector3f *vector);
size_t vector2_calculate_size(Vector2 *vector);
size_t vector2f_calculate_size(Vector2f *vector);
char *vector3_serialize(const Vector3 *vector);
Vector3 *vector3_deserialize(const char *buffer);
char *vector3f_serialize(const Vector3f *vector);
Vector3f *vector3f_deserialize(const char *buffer);

#endif //VECTOR_H
