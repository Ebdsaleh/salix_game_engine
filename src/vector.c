// vector.h
#include "vector.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Function to calculate size of the Vector3
size_t vector3_calculate_size(Vector3 *vector) {
    if (vector == NULL) {
        fprintf(stderr, "ERROR: Cannot get the size of a NULL Vector3.\n");
        return -1;
    }

    size_t component_size = sizeof(int32_t);

    size_t total_size = component_size * 3;

    return total_size;
}

// Function to calculate size of The Vector3f
size_t vector3f_calculate_size(Vector3f *vector) {
    if (vector == NULL) {
        fprintf(stderr, "ERROR: Cannot get the size of a NULL Vector3.\n");
        return -1;
    }

    size_t component_size = sizeof(float);

    size_t total_size = component_size * 3;

    return total_size;

}

/// Function to calculate size of The Vector2
size_t vector2_calculate_size(Vector2 *vector) {
    if (vector == NULL) {
        fprintf(stderr, "ERROR: Cannot get the size of a NULL Vector3.\n");
        return -1;
    }

    size_t component_size = sizeof(int32_t);

    size_t total_size = component_size * 2;

    return total_size;

}

// Function to calculate size of The Vector2f
size_t vector2f_calculate_size(Vector2f *vector) {
    if (vector == NULL) {
        fprintf(stderr, "ERROR: Cannot get the size of a NULL Vector3.\n");
        return -1;
    }

    size_t component_size = sizeof(float);

    size_t total_size = component_size * 2;

    return total_size;

}




// Serialize Vector3
char *vector3_serialize(const Vector3 *vector) {
  
    if (vector == NULL) {
        fprintf(stderr, "ERROR: Cannot serialize a NULL Vector3.\n");
        return NULL;
    }
    // Calculate the buffer size
    size_t buffer_size = sizeof(int32_t) * 3;
    
    // Allocate memory for serialization buffer
    char *buffer = (char*) malloc(buffer_size);
    if (buffer == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for Vector3 serialization buffer.\n");
        return NULL;
    }


    // Serialize each member of Vector3 to the buffer.
    size_t offset = 0;
    memcpy(buffer + offset, &(vector->x), sizeof(int32_t));
    // Move the position of the offset to assure correct alignment.
    offset += sizeof(int32_t);
    memcpy(buffer + offset, &(vector->y), sizeof(int32_t));
    offset += sizeof(int32_t);
    memcpy(buffer + offset, &(vector->z), sizeof(int32_t));

    // display the values at the time of serialization.
    printf("Serialized Vector3 members x: %" PRId32 ", y: %" PRId32 ", z: %" PRId32 ".\n", vector->x, vector->y, vector->z);
    return buffer;
}




Vector3 *vector3_deserialize(const char *buffer) {
    if (buffer == NULL) {
        fprintf(stderr, "ERROR: Cannot de-serialize a 'Vector3' from a NULL 'char*' buffer.\n");
        return NULL;
    }
    
    // Allocate memory for Vector3
    Vector3 *vector = (Vector3 *)malloc(sizeof(Vector3));
    if (vector == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'vector' during Vector3 de-serialization\n");
        return NULL;
    }

    // De-serialize each member of Vector3 from the buffer
    size_t offset = 0;

    memcpy(&(vector->x), buffer + offset, sizeof(int32_t));
    // Move the offset along the buffer
    offset += sizeof(int32_t);

    memcpy(&(vector->y), buffer + offset, sizeof(int32_t));
    offset += sizeof(int32_t);

    memcpy(&(vector->z), buffer + offset, sizeof(int32_t));

    
    // Display de-serialized values from the buffer
    printf("De-Serialized Vector3 members x: %" PRId32 ", y: %" PRId32 ", z: %" PRId32 ".\n",
            vector->x, vector->y, vector->z);

    return vector;
}



char *vector3f_serialize(const Vector3f *vector) {
    if (vector == NULL) {
        fprintf(stderr, "ERROR: Cannot serialize a NULL Vector3f.\n");
        return NULL;
    }
    // Calculate the size of the Vector3f
    size_t buffer_size = sizeof(float) * 3;

    // Allocate memory for serialization buffer
    char *buffer = (char*)malloc(buffer_size);
    if (buffer == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for Vector3f serialization buffer.\n");
        return NULL;
    }
    
    // Serialize each member of Vector3f to the buffer
    size_t offset = 0;

    memcpy(buffer + offset, &(vector->x), sizeof(float));
    
    // Move the offset position to maintain correct alignment
    offset += sizeof(float);
    
    memcpy(buffer + offset, &(vector->y), sizeof(float));
    
    offset += sizeof(float);
    memcpy(buffer + offset, &(vector->z), sizeof(float));

    // Display the Member values at the time of serialization
    printf("Serialized Vector3f members x: %f, y: %f, z: %f .\n",
            vector->x, vector->y, vector->z);

    return buffer;
}



Vector3f *vector3f_deserialize(const char *buffer) {
    if (buffer == NULL) {
        fprintf(stderr, "ERROR: Cannot deserialize a 'Vector3f' from a NULL 'char*' buffer.\n");
        return NULL;
    }

    Vector3f *vector = (Vector3f *)malloc(sizeof(Vector3f));
    if (vector == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'vector' during Vector3f de-serialization.\n");
        return NULL;
    }

    // De-serialize members of the Vector3f from the buffer
    size_t offset = 0;
    memcpy(&(vector->x), buffer + offset, sizeof(float));
    // Move the offset position to maintain correct alignment
    offset += sizeof(float);

    memcpy(&(vector->y), buffer + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&(vector->z), buffer + offset, sizeof(float));
    
    printf("De-Serialized Vector3f members x: %f, y: %f, z: %f .\n",
            vector->x, vector->y, vector->z);

    return vector;
}
