// scene_properties.h
#ifndef SCENE_PROPERTIES_H
#define SCENE_PROPERTIES_H

// Define properties for Scene.
// this how they are written in the scene file using the '.scene' script format
#define SCENE_PROPERTY_NAME "name"
#define SCENE_PROPERTY_ENTITY_COUNT "entity_count"
#define SCENE_PROPERTY_ENTITIES "[array**] entities"

// Value types not defined in the script file but used to align
// the tokens and build the scene via the syntax tree.
#define SCENE_PROPERTY_NAME_VALUE_TYPE "char*"
#define SCENE_PROPERTY_ENTITY_COUNT_VALUE_TYPE "int32_t"
#define SCENE_PROPERTY_ENTITIES_VALUE_TYPE "Entity**"


// Define properties for Entity
#define ENTITY_PROPERTY_ID "id"
#define ENTITY_PROPERTY_NAME "name"
#define ENTITY_PROPERTY_ELEMENTS "[array**] elements"
#define ENTITY_PROPERTY_ELEMENT_COUNT "element_count"


// Value types
#define ENTITY_PROPERTY_ID_VALUE_TYPE "Id"
#define ENTITY_PROPERTY_NAME_VALUE_TYPE "char*"
#define ENTITY_PROPERTY_ELEMENTS_VALUE_TYPE "Element**"
#define ENTITY_PROPERTY_ELEMENT_COUNT_VALUE_TYPE "int32_t"

// Add more properties as needed...

// Define properties for Transform
#define TRANSFORM_PROPERTY_NAME "name"
#define TRANSFORM_PROPERTY_OWNER "owner"
#define TRANSFORM_PROPERTY_POSITION "position"
#define TRANSFORM_PROPERTY_ROTATION "rotation"
#define TRANSFORM_PROPERTY_SCALE "scale"


// Value types
#define TRANSFORM_PROPERTY_NAME_VALUE_TYPE "char*"
#define TRANSFORM_PROPERTY_OWNER_VALUE_TYPE "Entity*"
#define TRANSFORM_PROPERTY_POSITION_VALUE_TYPE "Vector3*"
#define TRANSFORM_PROPERTY_ROTATION_VALUE_TYPE "Vector3*"
#define TRANSFORM_PROPERTY_SCALE_VALUE_TYPE "Vector3f*"
// Add more properties as needed...

#endif /* SCENE_PROPERTIES_H */
