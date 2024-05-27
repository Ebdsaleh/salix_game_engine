// scene_script_tags.h
#ifndef SCENE_SCRIPT_TAGS_H
#define SCENE_SCRIPT_TAGS_H

#define SCENE_TAG "[Scene]"
#define SCENE_END_TAG "[/Scene]"

#define ENTITY_TAG "[Entity]"
#define ENTITY_END_TAG "[/Entity]"

#define TRANSFORM_TAG "[Transform]"
#define TRANSFORM_END_TAG "[/Transform]"

// explict data types   
#define ARRAY_TAG "[array]"
#define ARRAY_END_TAG "[/array]"

#define ARRAY_P_TAG "[array*]"
#define ARRAY_P_END_TAG "[/array*]"

#define ARRAY_PP_TAG "[array**]"
#define ARRAY_PP_END_TAG "[/array**]"

#define ARRAY_INT_TAG "[array(int)]"
#define ARRAY_INT_END_TAG "[/array(int)]"

#define ARRAY_INT32_T_TAG "[array(int32_t)]"
#define ARRAY_INT32_T_END_TAG "[/array(int32_t)]"


#define ARRAY_FLOAT_TAG "[array(float)]"
#define ARRAY_FLOAT_END_TAG "[/array(float)]"

// I want to use these for specifying data types explicitly
// Say for if the User creates a new type but the engine doesn't know about it
// I'm stil not sure if I should use the '[' and ']' for the following because

// will be difficult to read especially if you are expressing different types inside 
// a tuple for example: id: ("ENT", 0) should be parsed as 'id' is the property name
// and "ENT" is the first value of that property, and 0 is the second value.

// To write 'id' explicitly it would look like this:
// id: ([char*] "ENT", [int32_t] 0)
// which is messy and using '(' type ')' would also confuse the lexer and end the tuple
// reading prematurely.

// Therefore I want to make a rule when using tuples '(' ')'  that values are parsed
// by order and separated by a comma ',' otherwise they fail or the default values are 
// written instead.

// That way 'id: ("ENT", 0)' and 'position: (0, 0, 0)' will work because all values
// of the property are given.

#define INT32_T_TAG "[int32_t]"
#define FLOAT_TAG "[float]"
#define CHAR "[char]"
#define CHAR_P_TAG "[char*]"
#define VECTOR2_TAG "[Vector2]"
#define VECTOR2F_TAG "[Vector2f]"
#define VECTOR3_TAG "[Vector3]"
#define VECTOR3F_TAG "[Vector3f]"

#define SINGLE_LINE_COMMENT_TAG "#"
#define MULTI_LINE_COMMENT_TAG "###"
// Add more tags as needed...

#endif /* SCENE_SCRIPT_TAGS_H */

