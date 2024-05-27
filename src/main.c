// main.c

// Salix Game Engine
#include<SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>
#include "element_counts.h"
#include "engine_core.h"
#include "scene_manager.h"

ElementCounts element_count;
SDL_Renderer *renderer;

void render_test() {
    // Create a surface with the desired color
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, 100, 100, 32, SDL_PIXELFORMAT_RGBA32);
    if (surface == NULL) {
        fprintf(stderr, "ERROR: Failed to create surface: %s\n", SDL_GetError());
        return;
    }
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 255, 255)); // Blue color

    // Create a texture from the surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // Free the surface as it is no longer needed
    if (texture == NULL) {
        fprintf(stderr, "ERROR: Failed to create texture: %s\n", SDL_GetError());
        return;
    }

    // Render the texture onto the renderer
    SDL_Rect rect = {400, 300, 100, 100}; // x, y, width, height
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    // Destroy the texture
    SDL_DestroyTexture(texture);
}
int main(){
   
    element_counts_init(&element_count);
    int frame_count = 0;


    // Initalize SDL
    if (SDL_Init(SDL_INIT_VIDEO)< 0){
        printf("SDL could not initialize! SDL ERROR: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow("Salix Game Engine",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            800, 600, SDL_WINDOW_SHOWN);

    // Create a renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL){
        printf("Renderer could not be created! SDL ERROR: %s\n", SDL_GetError());
        return 1;
    }

    // Main Loop flag
    int quit = 0;

    // Event handler
    SDL_Event e;
    // Scene creation test
    printf("PRINT DATA TYPE SIZES...\n");
    printf("Size of 'char' %zu bytes.\n", sizeof(char));
    printf("Size of 'int' %zu bytes.\n", sizeof(int));
    printf("Size of 'int32_t' %zu bytes.\n", sizeof(int32_t));
    printf("Size of 'float' %zu bytes.\n", sizeof(float));
    printf("Size of 'Scene' %zu bytes.\n", sizeof(Scene));
    printf("Size of 'Entity' %zu bytes.\n", sizeof(Entity));
    printf("Size of 'ElementType' %zu bytes.\n", sizeof(ElementType));
    printf("Size of 'Element' %zu bytes.\n", sizeof(Element));
    printf("Size of 'Transform' %zu bytes.\n", sizeof(Transform));
    printf("Size of 'Vector3' %zu bytes.\n", sizeof(Vector3));
    printf("Size of 'Vector3f' %zu bytes.\n", sizeof(Vector3f));
    printf("Size of 'Square' %zu bytes.\n", sizeof(Square));
    
    printf("TODO: CREATE A FILE FORMAT, STRUCTURE IT TO ALLOW ALL THE\n");
    printf("SCENE PROPERTIES AND THEIR PROPERTY VALUES TO BE WRITTEN AND READ CORRECTLY!\n");
    ///*
    printf("CREATING SCENE WITH SCENE MANAGER DESERIALIZATION...\n");

    Scene *scene = scene_manager_deserialize_scene("parse_test.scene");

    // */
    /* 
    //Entity and Element testing
    printf("CREATING SCENE WITH SCENE MANAGER MANUALLY...\n");
    Scene *scene = scene_manager_create_scene("Scene Manager Made Scene");
    printf("OK.\n");
    printf("CREATING ENTITY 'Player'...\n");
    Entity *player = entity_create();
    printf("OK.\n");
    printf("SETTING ENTITY NAME TO 'Player'...\n");
    entity_set_name(player, "Player");

    printf("ADDING ENTITY TO SCENE VIA SCENE MANAGER..\n");
    scene_add_entity(scene, player);
    printf("OK.\n");
    printf("SETTING SCENE NAME TO 'Test Scene'...\n");
    scene_set_name(scene, "Test Scene");
    element_set_name((Element *)player->elements[0], "Player_Transform");
    scene_manager_serialize_scene(scene, "test_scene.scene");
    //   */  
    

    //Transform *player_transform = (Transform *)entity_get_element(player, "transform");
    //Vector3 *player_position = &player_transform->position;
    //printf("Player's Position : (%d, %d, %d)\n", player_position->x, player_position->y, player_position->z);
    /*Square *player_body = square_prepare_type();
    printf("player_body default values: (x: %d. y: %d, size: %d)\n\
            rect.x: %d, rect.y: %d, rect.w: %d, rect.h: %d,\n",
            player_body->x, player_body->y, player_body->size,
            player_body->rect.x, player_body->rect.y,
            player_body->rect.w, player_body->rect.h);
    entity_add_element(player, (Element *)player_body);
    player_transform->position.x = 400;
    player_transform->position.y = 300;
    player_body->x = player_position->x - player_body->size / 2;
    player_body->y = player_position->y - player_body->size / 2;
    player_body->rect.x = (int)player_body->x;
    player_body->rect.y = (int)player_body->y;
    if (player_body->texture == NULL) {
        printf("texture is NULL!\n");
    }
    else {
        printf("texture exists! in int main()\n");
    }
    printf("player_body values after player assignment: (x: %d, y: %d, size: %d)\n\
            rect.x: %d, rect.y: %d, rect.w: %d, rect.h: %d,\n",
            player_body->x, player_body->y, player_body->size,
            player_body->rect.x, player_body->rect.y,
            player_body->rect.w, player_body->rect.h);
    printf("transform's name: %s\n", element_get_name((Element *)player_transform));
    printf("Enity 'player' name: %s\n", entity_get_name(player));
    element_set_name((Element *) player_body, "player body");
    
    printf("player body: %s.\n", element_get_name((Element *)player_body));
    */
    //element_set_name((Element *) player_transform, "player transform");
    //printf("player transform: %s.\n", element_get_name((Element *)player_transform));
    // Main Loop
    while (!quit){
        //printf("Frame Count: %d\n", frame_count);
        // Handle events on the queue
        while (SDL_PollEvent(&e) != 0){
            // User requests quit
            if (e.type == SDL_QUIT){
                quit = 1;
                break;
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Render objects, update game state, etc.
        //render_test();
        //square_render(player_body);
        //printf("square_render complete, about to call SDL_RenderPresent in main()\n");

        // Update screen
        SDL_RenderPresent(renderer);

        // Cap frame rate
        SDL_Delay(16); // Targeting approximately 60 frames per second
        frame_count++;

    }
    // Serialize the scene
    // Free engine resources
    //
    // UNCOMMENT WHEN NEEDING TO TEST SERIALIZATION AND DE-SERIALIZATION
    // AND TESTING ENITY, ELEMENTS OR OTHER THINGS.
    ///*
    printf("Window closed, Purging scene...\n");
    scene_purge_entities(scene);
    // */
    // Free resources and close SDL
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    return 0;
}
