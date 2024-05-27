// transform.c

#include "transform.h"
#include "element.h"
#include "element_counts.h"
#include "element_type.h"
#include "entity.h"
#include "vector.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Transform *transform_prepare_type(){
    Transform *new_transform = malloc(sizeof(Transform));
    if (new_transform == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for new_transform.\n");
        return NULL;
    }

    const char *initial_name = "transform";
    size_t name_length = strlen(initial_name) + 1;
    
    const char* id_prefix = "TRFM";
    size_t id_prefix_length = strlen(id_prefix) + 1;
    new_transform->base.id.prefix = malloc(id_prefix_length);

    
    if (new_transform->base.id.prefix == NULL){
        fprintf(stderr, "ERROR: failed to allocate memory for 'base.id.prefix'.\n");
        free(new_transform);
        return NULL;
    }
    
    strcpy(new_transform->base.id.prefix, id_prefix);
    new_transform->base.id.number = element_count.transform_count;
    new_transform->base.name = malloc(name_length * sizeof(char));


    if (new_transform->base.name == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for new_transform 'name'\n");
        free(new_transform->base.id.prefix);
        free(new_transform);
        return NULL;
    }
    
   
    strcpy(new_transform->base.name, initial_name);
    new_transform->base.type = malloc(sizeof(ElementType));
    if (new_transform->base.type == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for new_transform ElementType\n");
        free(new_transform->base.name);
        free(new_transform);
        return NULL;
    }

    
    // Make space for an owning Entity
    new_transform->owner = malloc(sizeof(Entity));
    if (new_transform->owner == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for new_transform 'owner' Entity.\n");
        free(new_transform->base.id.prefix);
        free(new_transform->base.type);
        free(new_transform->base.name);
        free(new_transform);
        return NULL;
    }
    
    // No memory allocation required.
    const char *type_name = "Transform";
    new_transform->base.type->name = strdup(type_name);


    if (new_transform->base.type->name == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory of new_transform ElementType 'name'\n");

        free(new_transform->owner);

        free(new_transform->base.id.prefix);
        free(new_transform->base.type);
        free(new_transform->base.name);
        free(new_transform);
        return NULL;
    }

     // default value position and rotation values
    Vector3 zero = { 0, 0, 0};
    // Default value for scale
    Vector3f default_scale = { 1.0f, 1.0f, 1.0f };
    
    // initalize the Transform

    // position
    new_transform->position = malloc(sizeof(Vector3));
    if (new_transform->position == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for new_transform Vector3 'position'.\n");
        free(new_transform->owner);
        free(new_transform->base.id.prefix);
        free(new_transform->base.type);
        free(new_transform->base.name);
        free(new_transform);
        return NULL;
    }

    *(new_transform->position) = zero;


    // rotation
    new_transform->rotation = malloc(sizeof(Vector3));
    if (new_transform->rotation == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for new_transform Vector3 'rotation'.\n");
        free(new_transform->position);
        free(new_transform->owner);
        free(new_transform->base.type);
        free(new_transform->base.name);
        free(new_transform->base.id.prefix);
        free(new_transform);
        return NULL;
    }

    *(new_transform->rotation) = zero;


    // scale
    new_transform->scale = malloc(sizeof(Vector3f));
    if (new_transform->scale == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for new_transform Vector3f 'scale'.\n");
        free(new_transform->rotation);
        free(new_transform->position);
        free(new_transform->owner);
        free(new_transform->base.type);
        free(new_transform->base.name);
        free(new_transform->base.id.prefix);
        free(new_transform);
    }
    *(new_transform->scale) = default_scale;

    element_count.transform_count++;
    return new_transform;

}


void transform_init(Element *element, Entity *owner){
    if (element == NULL){
        fprintf(stderr, "ERROR: Transform contains NULL Element in 'base'\n");
        return;
    }
    if (owner == NULL) {
        fprintf(stderr, "ERROR: Transform owner cannot be NULL during Transform initialization.\n");
    }
   
    // cast the element to Transform
    Transform *transform = (Transform *)element;
    
    // default value position and rotation values
    Vector3 zero = { 0, 0, 0};
    // Default value for scale
    Vector3f default_scale = { 1.0f, 1.0f, 1.0f };
    
    // initalize the Transform

    // owner
    if (transform->owner == NULL) {
        fprintf(stderr, "ERROR: 'transform' has no memory allocated for 'owner' during Transform initialization.\n"); 
        return;
    }
    transform->owner = owner;
 

    // position
    if (transform->position == NULL) {
        fprintf(stderr, "ERROR: 'transform' has no memory allocated for 'position' during Transform initialization.\n");
        return;
    }
    transform->position->x = zero.x;
    transform->position->y = zero.y;
    transform->position->z = zero.z;
   

    // rotation
    if (transform->rotation == NULL) {
        fprintf(stderr, "ERROR: 'transform' has no memory allocated for 'rotation' during Transform initialization.\n");
        return;
    }
    transform->rotation->x = zero.x;
    transform->rotation->y = zero.y;
    transform->rotation->z = zero.z;


    // scale
    if (transform->scale == NULL) {
        fprintf(stderr, "ERROR: 'transform' has no memory allocated for 'scale' during Transform initialization.\n");
        return;
    }
    transform->scale->x = default_scale.x;
    transform->scale->y = default_scale.y;
    transform->scale->z = default_scale.z;

   
}


void transform_set_name(Element *element, const char *new_name) {
    if (element == NULL || new_name == NULL) {
        fprintf(stderr, "ERROR: Invalid transform or new_name");
        return;
    }
    size_t new_name_length = strlen(new_name) + 1;
    char *new_name_copy = malloc(new_name_length * sizeof(char));
    if (new_name_copy == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for Transform 'new_name'.\n"); 
        return;
    }
    // cast back to the Transform type.
    Transform *transform = (Transform *) element;

    // Copy the new name string into allocated memory
    strcpy(new_name_copy, new_name);

    // free existing name string from memory if exists
    if (transform->base.name != NULL){
        free(transform->base.name);
    }
    
    // Allocate memory for transform's name field
    transform->base.name = malloc(new_name_length * sizeof(char));

    if (transform->base.name == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for Transform 'name'.\n");
        free(new_name_copy);
        return;
    }
    
    // Update the transform's name field
    strcpy(transform->base.name, new_name_copy);

    // free temporary copy of the name.
    free(new_name_copy);

}


char *transform_get_name(Element *element) {
    if (element == NULL) {
        fprintf(stderr, "ERROR: Invalid transform.\n");
        return NULL;
    }
    Transform *transform = (Transform *) element;
    return transform->base.name;
}


void transform_purge(Element *element) {
    if (element == NULL) {
        printf("Inside 'transform.c'\n");
        fprintf(stderr, "ERROR: Invalid (Element *)Transform, cannot purge.\n");
        return;
    }

    // Cast element back to Transform
    Transform *transform = (Transform *) element;
    free(transform->base.id.prefix);
    free(transform->base.name);
    free(transform->base.type);
    free(transform->position);
    free(transform->rotation);
    free(transform->scale);
    // prevent dangling pointers
    transform->base.name = NULL;
    transform->base.type = NULL;
    transform->position = NULL;
    transform->rotation = NULL;
    transform->scale = NULL;

    // Finally free the Transform itself
    free(transform);
}


void transform_show_properties(Element *element) {
    if (element == NULL){
        fprintf(stderr, "ERROR: Cannot properties of a NULL Transform.\n");
        return;
    }
    Transform *transform = (Transform *) element;
    printf("***Transform properties...***\n");
    printf("***Transform->base.id.prefix:\t'%s'.\n", transform->base.id.prefix);
    printf("***Transform->base.id.number:\t'%d'\n", transform->base.id.number);
    printf("***Transform->base.name:\t'%s'\n", transform->base.name);
    printf("***Transform->base.type->name\t'%s'\n", transform->base.type->name);
    printf("***Transform->owner->name:\t'%s'.\n", transform->owner->name);

    printf("Transform->position x: %" PRId32 ", y: %" PRId32 ", z: %" PRId32 "\n",
            transform->position->x, transform->position->y, transform->position->z);

    printf("Transform->rotation x: %" PRId32 ", y: %" PRId32 ", z: %" PRId32 "\n",
            transform->rotation->x, transform->rotation->y, transform->rotation->z);

    printf("Transform->scale x: %f, y: %f, z: %f \n",
            transform->scale->x, transform->scale->y, transform->scale->z);
}



// Function to calculate the size of a Transform and all its members
size_t transform_calculate_size(Element *element) {
    if(element == NULL) {
        fprintf(stderr, "ERROR: Cannot get the size of a NULL 'Transform'.\n");
        return -1;
    }
    // Cast the element back into a Transfrom
    Transform *transform = (Transform *)element;

    // Get the size of all the Transform's properties

    size_t base_id_prefix_length = strlen(transform->base.id.prefix) + 1;
    size_t base_id_prefix_size = base_id_prefix_length;
    size_t base_id_number_size = sizeof(int32_t);
    size_t base_name_length = strlen(transform->base.name) + 1;
    size_t base_name_size = base_name_length;

    size_t base_type_name_length = strlen(transform->base.type->name) + 1;
    size_t base_type_name_size = base_type_name_length;

    size_t owner_name_length = strlen(transform->owner->name) + 1;
    size_t owner_name_size = owner_name_length;

    size_t position_size = sizeof(int32_t) * 3;
    size_t rotation_size = sizeof(int32_t) * 3;
    size_t scale_size = sizeof(float) * 3;

    size_t size_total = base_id_prefix_size + base_id_number_size + 
        base_name_size + base_type_name_size + owner_name_size +
        position_size + rotation_size + scale_size;

    return size_total;

}





// Function for transform serialization
char *transform_serialize(const Element *element) {
    
    if (element == NULL) {
        fprintf(stderr, "ERROR: Cannot serialize a Transfrom from a NULL Element.\n");
        return NULL;
    }

    // Cast the element to a Transform
    Transform *transform = (Transform *) element;
    
    // Calculate the buffer size
    size_t buffer_size = transform_calculate_size((Element *)transform);
    size_t offset = 0;
    char* buffer = (char *)malloc(buffer_size);
    if (buffer == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'buffer' during 'transform_serialize'.\n");
        return NULL;
    }

    // Copy the buffer_size into buffer
    memcpy(buffer + offset, &buffer_size, sizeof(size_t));
    offset += sizeof(size_t);

    // Serialize members
    // Get the length of all the text variables
    size_t base_id_prefix_length = strlen(transform->base.id.prefix) + 1;
    size_t base_name_length = strlen(transform->base.name) + 1;
    size_t base_type_name_length = strlen(transform->base.type->name) + 1;
    size_t owner_name_length = strlen(transform->owner->name) + 1;


    // Get the size of the base_id_number
    size_t base_id_number_size = sizeof(int32_t);


    // Get the sizes of the position, rotation and scale
    size_t position_size = vector3_calculate_size(transform->position);
    size_t rotation_size = vector3_calculate_size(transform->rotation);
    size_t scale_size = vector3f_calculate_size(transform->scale);


    //base_id_prefix
    memcpy(buffer + offset, transform->base.id.prefix, base_id_prefix_length);
    offset += base_id_prefix_length;

    // base_id_number
    memcpy(buffer + offset, &transform->base.id.number, base_id_number_size);
    offset += base_id_number_size;

    // base_name
    memcpy(buffer + offset, transform->base.name, base_name_length);
    offset += base_name_length;

    // base_type_name
    memcpy(buffer + offset, transform->base.type->name, base_type_name_length);
    offset += base_type_name_length;

    // owner_name
    memcpy(buffer + offset, transform->owner->name, owner_name_length);
    offset += owner_name_length;
    
    // position
    memcpy(buffer + offset, transform->position, position_size);
    offset += position_size;

    // rotation
    memcpy(buffer + offset, transform->rotation, rotation_size);
    offset += rotation_size;

    // scale
    memcpy(buffer + offset, transform->scale, scale_size);
    offset += scale_size;


    // position, rotation and scale All typedef datatypes will require explicit
    // serialization e.g. transfrom->position.x, y, z
    printf("Serialized Transform with these properties:\n");
    transform_show_properties((Element*)transform);
    
       
    return buffer;

}






// Function for transform deserialization
Transform *transform_deserialize(const char *buffer, size_t offset, Entity *owner) {
    // Error checks
    if (buffer == NULL) {
        fprintf(stderr, "ERROR: Cannot de-serialize a 'Transform' from a NULL 'buffer'.\n");
        return NULL;
    }

    if (offset == 0) {
        fprintf(stderr, "ERROR: Cannot de-serialize a 'Transform' with an incorrect 'offset' position of: %zu.\n", offset);
        return NULL;
    }

    if (owner == NULL) {
        fprintf(stderr, "ERROR: Cannot de-serialize a 'Transform' with a NULL 'owner' to attach it to.\n");
        return NULL;
    }


    // create a Transform with default values
    Transform *transform = transform_prepare_type();
    if (transform == NULL) {
        fprintf(stderr, "ERROR: Failed to prepare 'Transform' during 'transform_deserialize'.\n");
        return NULL;
    }
    // IDEA CODE
    // could do this here then return transform at the end to let the entity element_count loop
    // add the transform to its element array.
    transform_init((Element *)transform, owner);
    // IDEA CODE END

    // get buffer_size for validation check before returning
    size_t buffer_size;
    memcpy(&buffer_size, buffer + offset, sizeof(size_t));
    offset += sizeof(size_t);
    // De-serialize Transform
    // base_id_prefix
    size_t base_id_prefix_length = strlen(buffer + offset) + 1;
    char *base_id_prefix = (char *)malloc(base_id_prefix_length);
    if (base_id_prefix == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'base_id_prefix' during 'transform_serialization'.\n");
        free(transform);
        return NULL;
    }

    memcpy(base_id_prefix, buffer + offset, base_id_prefix_length);
    offset += base_id_prefix_length;


    // base_id_number
    int32_t base_id_number;
    memcpy(&base_id_number, buffer + offset, sizeof(int32_t));
    offset += sizeof(int32_t);


    // base_name
    size_t base_name_length = strlen(buffer + offset) + 1;
    char* base_name = (char *)malloc(base_name_length);
    if (base_name == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'base_name' during 'transform_deserialize'.\n");
        free(base_id_prefix);
        free(transform);
        return NULL;
    }

    memcpy(base_name, buffer + offset, base_name_length);
    offset += base_name_length;


    // base_type_name this will only be used to check the serialized data...
    size_t base_type_name_length = strlen(buffer + offset) + 1;
    const char * base_type_name = (const char *) malloc(base_type_name_length);
    if (base_type_name == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'base_type_name' during 'transform_deserialize'.\n");
        free(base_id_prefix);
        free(base_name);
        free(transform);
        return NULL;
    }

    memcpy((void *)base_type_name, buffer + offset, base_type_name_length);
    offset += base_type_name_length;


    // owner_name
    size_t owner_name_length = strlen(buffer + offset) + 1;
    char * owner_name = (char *)malloc(owner_name_length);
    if (owner_name == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'owner_name' durning 'transform_deserialize'.\n");
        free(base_id_prefix);
        free(base_name);
        free(transform);
        return NULL;
    }

    memcpy(owner_name, buffer + offset, owner_name_length);
    offset += owner_name_length;


    // position
    memcpy(transform->position, buffer + offset, sizeof(Vector3));
    offset += sizeof(Vector3);


    // rotation
    memcpy(transform->rotation, buffer + offset, sizeof(Vector3));
    offset += sizeof(Vector3);


    // scale
    memcpy(transform->scale, buffer + offset, sizeof(Vector3f));


    // All values read from the buffer.
    // Verify the de-serializd expected values:

    // base_id_prefix
    if (strcmp(base_id_prefix, "TRFM") !=0) {
        fprintf(stderr, "ERROR: Invalid de-serialized valud for 'base_id_prefix' during 'transform_serialize'.\n");
        printf("Found '%s', expected 'TRFM'.\n", base_id_prefix);
        free(owner_name);
        free(base_id_prefix);
        free(base_name);
        free(transform);
        return NULL;
    }

    // base_type name 
    if (strcmp(base_type_name, "Transform") != 0) {
        fprintf(stderr, "ERROR: Invalid de-serialized value for 'base_type_name' during 'transform_serialize'.\n");
        printf("Found: '%s', expected: 'Transform'.\n", base_type_name);
        free(owner_name);
        free(base_id_prefix);
        free(base_name);
        free(transform);
        return NULL;
    }
    
    // Check passed in Entity's name with the name de-serialized
    if (strcmp(owner_name, owner->name) != 0) {
        fprintf(stderr, "ERROR: Deserialized 'owner_name' mismatch with 'owner' Entity reference in 'transform_deserialize'.\n");
        printf("Found: '%s', expected: '%s'.\n", owner_name, owner->name);
        free(base_id_prefix);
        free(base_name);
        free(owner_name);
        free(transform);
        return NULL;
    }

    // set the transform owner
    transform->owner = owner;
    // set the Transform's name
    transform_set_name((Element *)transform, base_name);
    
    // free temporary varaibles
    free(base_id_prefix);
    free(base_name);
    free(owner_name);
    
   

    printf("De-serialized Transform: '%s'.\n", transform->base.name);
    transform_show_properties((Element *)transform);
    return transform;

}

