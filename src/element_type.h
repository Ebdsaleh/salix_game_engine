// element_type.h
#ifndef ELEMENT_TYPE_H
#define ELEMENT_TYPE_H

typedef void (*ElementTypeInitFunc)(void *element, void *owner);

typedef struct {
    const char *name;
    ElementTypeInitFunc init; 
} ElementType;
#endif //  ELEMENT_TYPE_H
