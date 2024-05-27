// entity.c
#include "entity.h"
#include "element.h"
#include "transform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to create a new Entity
Entity *entity_create(){
    Entity *new_entity = malloc(sizeof(Entity));
    if (new_entity == NULL) {
        fprintf(stderr,"ERROR: Failed to allocate memory for 'new_entity'.\n");
        return NULL;
    }

    
    // id_prefix
    const char *id_prefix = "ENT";
    size_t id_prefix_length = strlen(id_prefix) + 1;
    new_entity->id.prefix = malloc(id_prefix_length);
    if (new_entity->id.prefix == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'id_prefix'.\n");
        free(new_entity);
        return NULL;
    }
    strcpy(new_entity->id.prefix, id_prefix);
    

    // id_number
    int32_t id_number = 0;
    new_entity->id.number = id_number;


    // name
    const char *initial_name = "entity";
    size_t name_length = strlen(initial_name) + 1;

   new_entity->name = malloc(name_length * sizeof(char));
   if (new_entity->name == NULL) {
       fprintf(stderr, "ERROR: Failed to allocate memory for new_entity 'name'.\n");
       free(new_entity->id.prefix);
       free(new_entity);
           return NULL;
   }
 
   strcpy(new_entity->name, initial_name);
   new_entity->elements = NULL;
   new_entity->element_count = 0;

   // Add the Transform
   Transform *new_transform = transform_prepare_type();
   if (new_transform == NULL) {
       fprintf(stderr, "ERROR: Failed to allocate memory for Transform during Entity creation.\n");
       free(new_entity->name);
       free(new_entity->id.prefix);
       free(new_entity);
       return NULL;
   }

   entity_add_element(new_entity, (void *)new_transform);
   // Return the new Entity
   return new_entity;
} 



// Function to create and Entity without a Transform, use this during de-serialization
Entity *entity_create_for_deserialization() {
    Entity *new_entity = malloc(sizeof(Entity));
    if(new_entity == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'new_entity'.\n");
        return NULL;
    }

    // id_prefix
    const char *id_prefix = "ENT";
    size_t id_prefix_length = strlen(id_prefix) + 1;
    new_entity->id.prefix = malloc(id_prefix_length);
    if (new_entity->id.prefix == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'id_prefix'.\n");
        free(new_entity);
        return NULL;
    }
    strcpy(new_entity->id.prefix, id_prefix);

    // id_number
    int32_t id_number = 0;
    new_entity->id.number = id_number;

    const char *initial_name = "entity";
    size_t name_length = strlen(initial_name);

    new_entity->name = malloc(name_length * sizeof(char));
    if (new_entity->name == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for new_entity 'name'.\n");
        free(new_entity->id.prefix);
        return NULL;
    }

    strcpy(new_entity->name, initial_name);
    new_entity->elements = NULL;
    new_entity->element_count = 0;

    return new_entity;
}


// Function to add an element to an entity
void entity_add_element(Entity *entity, void *element) {
    if (entity == NULL || element == NULL) {
        printf("ERROR IN: entity_add_element.\n");
        fprintf(stderr, "ERROR: Invalid Entity or ElementType.\n");
        return;
    }
    
   entity->elements = realloc(entity->elements, (entity->element_count + 1) * sizeof(Element *));
   if (entity->elements == NULL) {
       fprintf(stderr, "ERROR: Failed to reallocate memory for Enitity->elements.\n");
       return;
   }
   entity->elements[entity->element_count++] = element;
   // initialize the element based on type
   element_init(element, entity);
}
 

// Function to remove an element from an entity
void entity_remove_element(Entity *entity, Element *element) {
    for (int i = 0; i < entity->element_count; i++) {
        if (entity->elements[i] == element) {
            free(entity->elements[i]);
            for (int j = i; j < entity->element_count - 1; j++) {
                entity->elements[j] = entity->elements[j + 1];
            }
            entity->element_count--;
            entity->elements = realloc(entity->elements, entity->element_count * sizeof(Element *));
            break;
        }
    }
}






// Function to purge all elements from an entity
void entity_purge_elements(Entity *entity) {
    if (entity == NULL) {
        fprintf(stderr, "ERROR: Cannot purge elements from a NULL Entity.\n");
    }
    printf("purging elements from Entity '%s'\n", entity->name);
    for (int i = entity->element_count - 1; i >= 0; i--) {
        printf("Purging element '%s' of type: '%s' at index: %d, from Entity: %s \n", entity->elements[i]->name, entity->elements[i]->type->name, i, entity->name);
        
        element_purge(entity->elements[i]);
    }
    printf("successfully purged elements.\n");
    free(entity->elements);
    entity->elements = NULL;
    entity->element_count = 0;
    return; 

}





// Function to get an element from an entity by name
Element *entity_get_element(Entity *entity, const char *name) {
    if (entity == NULL) {
        fprintf(stderr, "ERROR in entity_get_element_by_name: Cannot get an element from a NULL Entity.\n");
        return NULL;
    }

    if (entity->elements == NULL) {
        fprintf(stderr, "WARNING in entity_get_element_by_name: No elements exist inside entity.\n");
        return NULL;
    }
    for (int i = 0; i < entity->element_count; i++) {
        if (strcmp(name, entity->elements[i]->name) == 0){
            return entity->elements[i];
        }
    }
    fprintf(stderr, "WARNING in entity_get_element_by_name: No elements exist inside entity %s", entity->name);
    return NULL;
 }






// Function for setting the name of an entity
void entity_set_name(Entity *entity, const char *new_name) {
    if (entity == NULL || new_name == NULL){
        fprintf(stderr, "ERROR: Invalid entity or new_name.\n");
        return;
    }
    size_t new_name_length = strlen(new_name) + 1;
    char *new_name_copy = malloc(new_name_length * sizeof(char));
    if (new_name_copy == NULL) {
        fprintf(stderr, "ERROR: failed allocate memory for Entity 'new_name_copy'.\n");
        return;
    }
    // Copy the new name string into allocated memory
    strcpy(new_name_copy, new_name);

    // free existing name string memory if exists
    if (entity->name != NULL){
        free(entity->name);
    }
    
    // Allocate memory for the entity's name field
    entity->name = malloc(new_name_length * sizeof(char));
    if (entity->name == NULL){
        fprintf(stderr, "ERROR: Failed to allocate memory for Entity 'name'.\n");
        free(new_name_copy);
        return;
    }

    // Update the entity's name field
    strcpy(entity->name, new_name_copy);

    // free tempory copy of the new_name.
    free(new_name_copy);

}




// Function for getting the name of an entity
char *entity_get_name(Entity *entity){
    if (entity == NULL) {
        fprintf(stderr, "ERROR: Cannot return name from a NULL Entity.\n");

    }

    if (entity->name == NULL) {
        fprintf(stderr, "ERROR: Entity 'name' is not initialized correctly, returned NULL.\n");
        return NULL;
    }

    return entity->name;
} 





size_t entity_calculate_size(Entity *entity){
    // NULL check
    if (entity == NULL) {
        fprintf(stderr, "ERROR: Cannot calculate the size of a NULL Entity.\n");
        return -1;
    }

    // Get properties sizes

    size_t id_prefix_size = strlen(entity->id.prefix) + 1;
    size_t id_number_size = sizeof (int32_t);
    size_t name_size = strlen(entity->name) + 1;
    size_t element_count_size = sizeof(int32_t);
    size_t elements_size = 0;
    // Get the size of all elements
    for (int i = 0; i < entity->element_count; i++) {
        elements_size += element_calculate_size(entity->elements[i]);
    }
    
    // Calculate the total size of the entity
    // 
    size_t total_size = id_prefix_size + id_number_size + name_size +
        element_count_size + elements_size;

    return total_size;
}





char *entity_serialize(Entity *entity) {
    if (entity == NULL) {
        fprintf(stderr, "ERROR: Invalid Entity, cannot perform serialization on a NULL.\n");
        return NULL;
    }
    
    size_t entity_buffer_size = entity_calculate_size(entity);
    printf("Entity buffer size: %zu.\n", entity_buffer_size);
    char *entity_buffer = (char *)malloc(entity_buffer_size);
    if (entity_buffer == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for entity_buffer, during serialization.\n");
        return NULL;
    }

    size_t offset = 0;
    // STEP 0:
    // Write the size of the entity_buffer into the entity_buffer
    memcpy(entity_buffer + offset, &entity_buffer_size, sizeof(size_t));
    offset += sizeof(size_t);
    // STEP 1: 
    // prepare entity id and name string lengths
    size_t id_prefix_size = strlen(entity->id.prefix) + 1;
    size_t name_size = strlen(entity->name) + 1;
    
    // prepare the element_count
    size_t element_count_size = sizeof(int32_t);

    
    // STEP 2:
    // Copy the id,  name and element_count to the buffer

    // id
    // prefix
    memcpy(entity_buffer + offset, &id_prefix_size, sizeof(size_t));
    offset += sizeof(size_t);

    memcpy(entity_buffer + offset, entity->id.prefix, id_prefix_size);
    offset += id_prefix_size;
    
    // id
    // number
    memcpy(entity_buffer + offset, &entity->id.number, sizeof(int32_t));
    offset += sizeof(int32_t);
    

    // name
    memcpy(entity_buffer + offset, &name_size, sizeof(size_t));
    offset += sizeof(size_t);

    memcpy(entity_buffer + offset, entity->name, name_size);
    offset += name_size;
    

    // element_count
    memcpy(entity_buffer + offset, &element_count_size, sizeof(size_t));
    offset += sizeof(size_t);
    memcpy(entity_buffer + offset, &entity->element_count, element_count_size);
    offset += element_count_size;
           
    // STEP 3:
    // Check that enity has elements to serialize
    if (entity->element_count == 0) {
        // No elements to serialize, return from the function.
        printf("NOTE: Entity '%s' has no elements to write to file.\n", entity->name);
        return entity_buffer;
    }
    printf("Serialized entity: '%s' element_count, value: %d.\n", entity->name, entity->element_count);
    
    // STEP 4:
    // Serialize elements
    for (int i = 0; i < entity->element_count; i++) {
        printf("Serializing entity '%s' elements[%d]\n", entity->name, i);
        // get element directive
        size_t ele_id_prefix_length = strlen(entity->elements[i]->id.prefix) + 1;
        char * ele_id_prefix = (char*)malloc(ele_id_prefix_length);
        if (ele_id_prefix == NULL) {
            return NULL;
        }
        // write the element directive to the buffer.
        memcpy(entity_buffer + offset, ele_id_prefix, ele_id_prefix_length);
        offset += ele_id_prefix_length;

        char *element_data = element_serialize((Element *)entity->elements[i]);
        size_t element_data_size = strlen(element_data);
        memcpy(entity_buffer + offset, &element_data_size, sizeof(size_t));
        offset += sizeof(size_t);

        memcpy(entity_buffer + offset, element_data, element_data_size);
        offset += element_data_size;
        free(element_data);
        free(ele_id_prefix);

    }
    printf("Finished serializing Entity: '%s'.\n", entity->name);
    return entity_buffer;
}




Entity *entity_deserialize(char *buffer, size_t offset) {
    if (buffer == NULL) {
        fprintf(stderr, "ERROR: Invalid file pointer, cannot perform de-serialization.\n");
        return NULL;
    }
    
    if (offset == 0) {
        fprintf(stderr, "ERROR: Invalid 'buffer', cannot perform Entity de-serialization.\n");
        return NULL;
    }
    printf("Checking 'buffer' and 'offset' values.\n");
    printf("buffer in bytes: %zu.\n", sizeof(buffer));
    printf("offset in bytes: %zu.\n", offset);


    // get the buffer size of entity to validate before returning
    size_t entity_buffer_size;
    memcpy(&entity_buffer_size, buffer + offset, sizeof(size_t));
    offset += sizeof(size_t);
    
    // Read entity id_prefix and number
    // id_prefix length
    size_t id_prefix_length;
    memcpy(&id_prefix_length, buffer + offset, sizeof(size_t));
    offset += sizeof(size_t);
    
    // id_prefix
    char * id_prefix = (char *)malloc(id_prefix_length);
    if (id_prefix == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'id_prefix' during 'entit_deserialize'\n");
        return NULL;
    }
    // Read id_prefix 
    memcpy(id_prefix, buffer + offset, id_prefix_length);
    offset += id_prefix_length;
    // test value
    printf("'id_prefix' of 'entity': %s.\n", id_prefix);


    // id_number
    int32_t id_number;
    memcpy(&id_number, buffer + offset, sizeof(int32_t));
    offset += sizeof(int32_t);


    // Read the entity name
    size_t name_length = strlen(buffer + offset) + 1;
    char * name = (char*) malloc(name_length);
    if (name == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'name' during 'entity_deserialize'.\n");
        free(id_prefix);
        return NULL;
    }

    memcpy(name, buffer + offset, name_length);
    offset += name_length;


    // Read the element_count 
    int32_t element_count;
    memcpy(&element_count, buffer + offset, sizeof(int32_t));
    offset += sizeof(int32_t);


    // Create entity
    printf("Creating new Entity to prepare for de-serialization\n");
    Entity *entity = entity_create_for_deserialization();

    if (entity == NULL) {
        fprintf(stderr, "ERROR: Invalid Entity pointer, cannot perform deserialization.\n");
        return NULL;
    }

    printf("Assigning entity properties.\n");
    entity->id.prefix = id_prefix;
    entity->id.number = id_number;
    entity->name = name;
    

    for (int i = 0; i < element_count; i++)
    {   
        printf("De-serializing entity '%s's element at index: %d'.\n", entity->name, i);
        size_t directive_size = strlen(buffer + offset) + 1;
        char *ele_directive = (char *)malloc(directive_size);
        if (ele_directive == NULL) {
            return NULL;
        }
        memcpy(ele_directive, buffer + offset, directive_size);
        offset += directive_size;

        Element *element = (Element *)element_deserialize(buffer, offset, entity);
        if (element == NULL) {
            fprintf(stderr, "ERROR: Failed to de-serialize 'element' during 'entity_deserialize'.\n");
            free(entity);
            return NULL;
        }
        entity_add_element(entity, (Element *)element); // this function updates element count as elements are added.
        offset += element_calculate_size((Element *)element);
        
    }
    
    // return deserialized entity.
    return entity;

}


