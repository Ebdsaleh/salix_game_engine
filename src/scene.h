// scene.h
#ifndef SCENE_H
#define SCENE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include "entity.h"
// Define a structure to represent a scene
typedef struct {
    char *name;
    Entity **entities;
    int32_t entity_count;

} Scene;

// Function to initialize a scene
Scene *scene_create();

// Function to set the name of a scene.
void scene_set_name(Scene *scene, const char *new_name);

// Function to get the name of a scene
char *scene_get_name(Scene *scene);
// Function to render a scene
void scene_render(SDL_Renderer *renderer, const Scene *scene);

// Function to update a scene
void scene_update(Scene *scene);

// Function to add an Enity to a scene
void scene_add_entity(Scene *scene, Entity *entity);

// Function to remove an entity from a scene
void scene_remove_entity(Scene *scene, Entity *entity);

void scene_purge_entities(Scene *scene);

size_t scene_calculate_size(Scene *scene);

#endif // SCENE_H
