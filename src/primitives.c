// primitives.c
#include "primitives.h"
#include "element_type.h"
#include "entity.h"
#include "transform.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

Square *square_prepare_type(){
    Square *new_square = malloc(sizeof(Square));
    if (new_square == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for new_square.\n");
        return NULL;
    }
    
    const char *initial_name = "square";
    size_t name_length = strlen(initial_name) + 1;

    new_square->base.name = malloc(name_length * sizeof(char));
    
    if (new_square->base.name == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for new_square 'name'.\n");
        free(new_square);
        return NULL;
    }
    strcpy(new_square->base.name, initial_name);

    new_square->base.type = malloc(sizeof(ElementType));
    if (new_square->base.type == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for new_square ElementType.\n");
        free(new_square->base.name);
        free(new_square);
        return NULL;
    }

   
    // Make space for an owning Entity
    new_square->owner = malloc(sizeof(Entity));
    if (new_square->owner == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for new_square 'owner' Entity.\n");
        free(new_square->base.type);
        free(new_square->base.name);
        free(new_square);
        return NULL;
    }

    const char *type_name = "Square";    
    new_square->base.type->name = strdup(type_name);

    if (new_square->base.type->name == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for new_square ElementType 'name'.\n");
        free(new_square->owner);
        free(new_square->base.type);
        free(new_square->base.name);
        free(new_square);
        return NULL;
    }

    // Initialize other members
    new_square->x = 0;
    new_square->y = 0;
    new_square->size = 0;
    new_square->color = (SDL_Color){0, 0, 0, 255}; // Black color
    new_square->rect = (SDL_Rect){0, 0, 0, 0};    // Empty rectangle

    new_square->texture = NULL;  // No initial texture.
    return new_square;
}

void square_init(Element *element, Entity *owner){
    if (element == NULL){
        fprintf(stderr, "ERROR: Square contains NULL Element in 'base'.\n");
        return;
    }
    if (owner == NULL){
        fprintf(stderr, "ERROR: Sqaure contains NULL Entity in 'owner'");
        return;
    }
    // cast the element to Square
    Square *square = (Square *)element;
    // initialize square
    square->owner = owner;
    Transform *transform = (Transform *)entity_get_element(owner, "transform");
    square->x = transform->position->x;
    square->y = transform->position->y;
    square->size = 100 * transform->scale->x;
    square->color = (SDL_Color){22, 138, 185, 255};
    square->rect = (SDL_Rect) {square->x, square->y,
        square->size, square->size };
    // setup the Surface
    // Create a surface with a single pixel of the desired color
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, square->rect.w,
            square->rect.h, 32, SDL_PIXELFORMAT_RGBA32);
    if (surface == NULL) {
        fprintf(stderr, "ERROR: Failed to create for texture. SDL ERROR: %s\n", SDL_GetError());
        return;
    }
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, square->color.r,
                square->color.g, square->color.b, square->color.a));

    // Create texture from surface
    square->texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    if (square->texture == NULL) {
        fprintf(stderr, "ERROR: Failed to create texture from surface. SDL ERROR: %s\n", SDL_GetError());
        return;
    } 
    SDL_FreeSurface(surface); // Free the surface as it is no longer needed
}

void square_render(Square *square) {
    if (square == NULL) {
        fprintf(stderr, "ERROR: Cannot render a NULL Square.\n");
        return;
    }
    /*SDL_SetRenderDrawColor(renderer, square->color.r, square->color.g,
            square->color.b, square->color.a);
            */
    // SDL_RenderFillRect(renderer, &square->rect);
    if (square->texture == NULL) {
        fprintf(stderr, "ERROR: square->texture is NULL while trying to render.\n");
        return;

    } else {
        //printf("square->text IS NOT NULL inside 'square_render'. \n");
    }
    //printf("copying texture to the renderer. \n");
    // Check that square->rect is not null
    if (square->rect.w <= 0 || square->rect.h <= 0){
        fprintf(stderr, "ERROR: Invalid rectangle dimensions\n");
        return;
    }
    // Render texture onto the square's rectangle
    if (SDL_RenderCopy(renderer, square->texture, NULL, &square->rect) != 0){
          fprintf(stderr, "ERROR: SDL_RenderCopy Failed: SDL ERROR: %s", SDL_GetError());
          return;
          // FAILS!!!
    }
}


void square_set_name(Element *element, const char *new_name) {
    if (element == NULL || new_name == NULL) {
        fprintf(stderr, "ERROR: Invalid square of new_name.\n");
        return;
    }

    size_t new_name_length = strlen(new_name) + 1;
    char *new_name_copy = malloc(new_name_length * sizeof(char));

    if (new_name_copy == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for Sqaure 'new_name'.\n");
        return;
    }
    
    // Convert back to Square Element type
    Square * square = (Square *) element;

    // Copy the new name string into allocated memory
    strcpy(new_name_copy, new_name);

    // free existing name string from memory if exists
    if (square->base.name != NULL){
        free(square->base.name);
    }
    
    // Allocate memory for square's name field
    square->base.name = malloc(new_name_length * sizeof(char));
    
    if (square->base.name == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for Square 'name'.\n");
        free(new_name_copy);
        return;
    }
    // Update the square's name field
    strcpy(square->base.name, new_name_copy);

    // free temporary copy of the name.
    free(new_name_copy);

}


char *square_get_name(Element *element) {
    if (element == NULL) {
        fprintf(stderr, "ERROR: Invalid square.\n");
        return NULL;
    }
    // Convert back to a Square Element
    Square * square = (Square *) element;
    return square->base.name;
}


void square_purge(Element *element) {
    if (element == NULL) {
        printf("Inside 'primitives.c'\n");
        fprintf(stderr, "ERROR: Invalid (Element *) Square, cannot purge.\n");
        return;
    }

    // Cast element back to Square
    Square * square = (Square *) element;

    free(square->base.name);
    free(square->base.type);
    // Use SDL_DestroyTexture to free the SDL_Texture* 
    // Otherwise it will hang when trying to call SDL_DestroyRenderer
    SDL_DestroyTexture(square->texture);

    // Set to NULL
    square->base.name = NULL;
    square->base.type = NULL;
    square->texture = NULL;

    // Finally free the Square;
    free(square);

}



//SERIALIZATION

int square_serialize(FILE *file, Element *element){
    if (file == NULL) {
        fprintf(stderr, "ERROR: Invalid file pointer, cannot perform Square serialization.\n");
        return -1;
    }
    
    if (element == NULL) {
        fprintf(stderr, "ERROR: Cannot serialize a Square from a NULL Element.\n");
        return -1;
    }


    // Cast the element to a Square
    Square *square = (Square*) element;

    // Element directive base type name
    size_t directive_base_type_name_length = strlen(square->base.type->name);
    fwrite(&directive_base_type_name_length, sizeof(size_t), 1, file);
    fwrite(square->base.type->name, sizeof(char), directive_base_type_name_length, file);

    // Serialize members
    // Element details (base)
    // base name
    size_t base_name_length = strlen(square->base.name);
    fwrite(&base_name_length, sizeof(size_t), 1, file);
    fwrite(square->base.name, sizeof(char), base_name_length, file);

    // base type name
    size_t base_type_name_length = strlen(square->base.type->name);
    fwrite(&base_type_name_length, sizeof(size_t), 1, file);
    fwrite(square->base.type->name, sizeof(char), base_type_name_length, file);
    
    // Entity details (owner)
    // owner
    size_t owner_name_length = strlen(square->owner->name);
    fwrite(&owner_name_length, sizeof(size_t), 1, file);
    fwrite(square->owner->name, sizeof(char), owner_name_length, file);

    // Square's unique members (int) types x, y, size
    fwrite(&square->x, sizeof(int), 1, file);
    fwrite(&square->y, sizeof(int), 1, file);
    fwrite(&square->size, sizeof(int), 1, file);

    // Square's unique members (SDL) types rect, color, texture
    // in fine detail
    fwrite(&square->rect.x, sizeof(int), 1, file);
    fwrite(&square->rect.y, sizeof(int), 1, file);
    fwrite(&square->rect.w, sizeof(int), 1, file);
    fwrite(&square->rect.h, sizeof(int), 1, file);
    fwrite(&square->color.r, sizeof(Uint8), 1, file);
    fwrite(&square->color.g, sizeof(Uint8), 1, file);
    fwrite(&square->color.b, sizeof(Uint8), 1, file);
    fwrite(&square->color.a, sizeof(Uint8), 1, file);
    fwrite(&square->texture, sizeof(SDL_Texture*), 1, file);
    
    return 0;
}



void square_deserialize(FILE *file, Entity *owner) {
    if (file == NULL || owner == NULL) {
        fprintf(stderr, "ERROR: Invalid file pointer or Entity pointer, cannot perform Sqaure de-serialization.\n");
        return;
    }

    // Read the square's name-length and name
    size_t base_name_length;
    fread(&base_name_length, sizeof(size_t), 1, file);

    char *base_name = malloc((base_name_length + 1) * sizeof(char));
    if (base_name == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'base_name' during Square de-serialization.\n");
        return;
    }


    fread(base_name, sizeof(char), base_name_length, file);
    base_name[base_name_length] = '\0';

    
    // Read the square's base type name-length and type name
    size_t base_type_name_length;
    fread(&base_type_name_length, sizeof(size_t), 1, file);

    char *base_type_name = malloc((base_type_name_length + 1) * sizeof(char));
    if (base_type_name == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'base_type_name' during Square de-serialization.\n");
        free(base_name);
        return;
    }

    fread(base_type_name, sizeof(char), base_type_name_length, file);
    base_type_name[base_type_name_length] = '\0';

    
    // Prepare square for full initilization with default attributes
    Square *square = square_prepare_type();

    // Assign the de-serialized name to the Square
    square_set_name((Element *)square, base_name);

    // Check that the base.type->name is correctly de-serialized
    if (strcmp(base_type_name, square->base.type->name) == 0){
        fprintf(stderr, "ERROR: Square ElementType does not match with de-serialized data.\n");
        return;
    }
    

    // Attach the square to the owning entity
    entity_add_element(owner,(Element *) square);

    // read the (int) members x, y, size  from the file
    int x, y, size;

    fread(&x, sizeof(int), 1, file);
    fread(&y, sizeof(int), 1, file);
    fread(&size, sizeof(int), 1, file);

    // assign the values to the square;
    square->x = x;
    square->y = y;
    square->size = size;

    // read the (SDL) type members from the file, rect, color, texture.
    SDL_Rect rect;
    SDL_Color color;
    SDL_Texture *texture;

    fread(&rect.x, sizeof(SDL_Rect), 1, file);
    fread(&rect.y, sizeof(SDL_Rect), 1, file);
    fread(&rect.w, sizeof(SDL_Rect), 1, file);
    fread(&rect.h, sizeof(SDL_Rect), 1, file);
    fread(&color.r, sizeof(SDL_Color), 1, file);
    fread(&color.g, sizeof(SDL_Color), 1, file);
    fread(&color.b, sizeof(SDL_Color), 1, file);
    fread(&color.a, sizeof(SDL_Color), 1, file);
    fread(&texture, sizeof(SDL_Texture*), 1, file);

    // Hopefully all OK, return from the function.
    return;

}
