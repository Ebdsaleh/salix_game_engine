// primitives.h
#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "element.h"
#include "entity.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

extern SDL_Renderer *renderer;

typedef struct {
  Element base;
  Entity *owner;
  int x;
  int y;
  int size;
  SDL_Color color;
  SDL_Rect rect;
  SDL_Texture *texture;
  
} Square;

Square *square_prepare_type();

void square_init(Element *element, Entity *owner);
void square_render(Square *square);
void square_set_name(Element *element, const char *new_name);
char *square_get_name(Element *element);
void square_purge(Element *element);
int square_serialize(FILE *file, Element *element);
void square_deserialize(FILE *file, Entity *owner);
#endif // PRIMITIVES_H 
