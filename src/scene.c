// scene.c
#include "scene.h"
#include "entity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Function to create a scene
Scene *scene_create() {
    // initialize a scene with default arguments.
    Scene *new_scene = malloc(sizeof(Scene));
    if (new_scene == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for new_scene\n");
        return NULL;
    }

    const char *initial_name = "Untitled";
    size_t name_length = strlen(initial_name) + 1;

    // Allocate memory for Scene 'name'
    new_scene->name = malloc(name_length * sizeof(char));
    if (new_scene->name == NULL){
        fprintf(stderr, "ERROR: Failed to allocate memory for new_scene 'name'.\n");
        free(new_scene);
        return NULL;
    }
    strcpy(new_scene->name, initial_name);
    new_scene->entities = NULL;
    new_scene->entity_count = 0;    
    return new_scene;

}


// Function to set a the name of a scene
void scene_set_name(Scene *scene, const char *new_name) {
    if (scene == NULL || new_name == NULL) {
        fprintf(stderr, "ERROR: Invalid Scene or new_name.\n");
        return;
    }

    size_t new_name_length = strlen(new_name) + 1;
    char *new_name_copy = malloc(new_name_length * sizeof(char));

    if (new_name_copy == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for Scene 'new_name_copy'\n");
        return;
    }

    // Copy the new name string into allocated memory
    strcpy(new_name_copy, new_name);
    
    // clear out the existing name.
    if (scene->name != NULL) {
        free(scene->name);
    }

    // Allocate memory for the scene's name field
    scene->name = malloc(new_name_length * sizeof(char));

    if (scene->name == NULL){
        fprintf(stderr, "ERROR: Failed to allocate memory for Scene 'name'.\n");
        free(new_name_copy);
        return;
    }

    // Update the Scene's name field
    strcpy(scene->name, new_name_copy);

    // free temporary copy of the new_name
    free(new_name_copy);
    
}


// Function to get the name of a scene
char *scene_get_name(Scene *scene) {
    if (scene == NULL) {
        fprintf(stderr, "ERROR: Cannot return name from a NULL Scene");
        return NULL;
    }

    if (scene->name == NULL) {
        fprintf(stderr, "ERROR: Scene 'name' is not initialized correctly, returned NULL.\n");
        return NULL;
    }
    
    return scene->name;
}


// Function to render a scene 
void scene_render(SDL_Renderer *renderer, const Scene *scene) {

}

// Function to update a scene
void scene_update(Scene *scene) {
    
}


// Function to add an Enity to a scene
void scene_add_entity(Scene *scene, Entity *entity) {
    if (scene == NULL || entity == NULL) {
        fprintf(stderr, "ERROR: Scene or Entity is NULL!\n");
        return;
    }
    
    // Increase the size of the entities array
    Entity **temp = realloc(scene->entities, (scene->entity_count + 1) * sizeof(Entity *));
    if (temp == NULL) {
        fprintf(stderr,"ERROR: Failed to re-allocate memory for Scene->entities.\n");
        return;
    }
    scene->entities = temp;
    // Add entity to scene
    scene->entities[scene->entity_count++] = entity;
}


// Function to remove an entity from a scene
void scene_remove_entity(Scene *scene, Entity *entity) {
    for (int i = 0; i < scene->entity_count; i++) {
        if (scene->entities[i] == entity) {
            free(scene->entities[i]);
            for (int j = i; j < scene->entity_count; j++) {
                scene->entities[j] = scene->entities[ j + 1];
            }
            scene->entity_count--;
            scene->entities = realloc(scene->entities, (scene->entity_count) * sizeof(Entity *));
            break;
        }
    }
}


// Function to purge all entities from a scene
void scene_purge_entities(Scene *scene) {
    if (scene == NULL) {
        fprintf(stderr, "ERROR: Cannot purge entities from a NULL scene.\n");
        return;
    }
    
    printf("purging entities from Scene '%s'.\n", scene->name);

    for (int i = scene->entity_count - 1; i >= 0; i--){
        printf("Purging entity '%s' at index: %d from Scene: %s.\n", scene->entities[i]->name, i, scene->name);
        entity_purge_elements(scene->entities[i]);
    }
    free(scene->entities);
    scene->entities = NULL;
    scene->entity_count = 0;
    return;
}



// Function to calculate the size of the a scene
size_t scene_calculate_size(Scene *scene) {
    if (scene == NULL) {
        fprintf(stderr, "ERROR: Cannot calculate the size if NULL scene.\n");
        return -1;
    }
    
    size_t scene_name_size = strlen(scene->name) + 1;
    size_t entity_count_size = sizeof(int32_t);
    size_t entities_size;
    
    // Get the size of all the entities in the scene
    for (int i = 0; i < scene->entity_count; i++) {
        entities_size += entity_calculate_size(scene->entities[i]);
    }

    size_t total_size = scene_name_size + entity_count_size + entities_size;
    return total_size;
}
