// scene_manager.h
#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H
#include "entity.h"
#include "scene.h"
#include "scene_script_tags.h"
#include "scene_properties.h"

// Function to create a new scene
Scene *scene_manager_create_scene(const char *new_scene_name);


// Function to add an entity to scene
void scene_manager_add_entity(Scene *scene, Entity *entity);


// Function to remove and entity from a scene
void scene_manager_remove_entity(Scene *scene, Entity *entity);


// Function to serialize scene to a binary file
int scene_manager_serialize_scene(Scene *scene, const char *file_name);


// Function to deserialize scene from a binary file
Scene *scene_manager_deserialize_scene(const char *file_name);


// Function to read a scene file and return it as a file buffer
char *scene_manager_read_scene_file(const char* file_name, size_t *buffer_size);


// Function that returns the defined scene_script_tags.h header file
char * scene_manager_get_engine_script_tags();


// Function that prints out the scene script tags that are defined in the engine
void scene_manager_print_engine_script_tags();


// Function that validates the scene file structure.
int scene_manager_validate_scene_file(char * buffer, size_t *buffer_size);


// Function that returns the properties defined in the scene_properties.h header file
char* scene_manager_get_scene_properties();


void print_scene_properties();
#endif // SCENE_MANAGER_H
