// lexer.h
#ifndef LEXER_H
#define LEXER_H
#include "syntax_tree.h"
typedef enum {
    TOKEN_TAG,
    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_TUPLE,
    TOKEN_SCENE_TAG,
    TOKEN_SCENE_END_TAG,
    TOKEN_ENTITY_TAG,
    TOKEN_ENTITY_END_TAG,
    TOKEN_TRANSFORM_TAG,
    TOKEN_TRANSFORM_END_TAG,
    TOKEN_PROPERTY_NAME,
    TOKEN_PROPERTY_VALUE,
    TOKEN_ARRAY_TAG,
    TOKEN_ARRAY_END_TAG,
    TOKEN_ARRAY_P_TAG,
    TOKEN_ARRAY_P_END_TAG,
    TOKEN_ARRAY_PP_TAG,
    TOKEN_ARRAY_PP_END_TAG,
    TOKEN_ARRAY_INT_TAG,
    TOKEN_ARRAY_INT_END_TAG,
    TOKEN_ARRAY_INT32_T_TAG,
    TOKEN_ARRAY_INT32_T_END_TAG,
    TOKEN_ARRAY_FLOAT_TAG,
    TOKEN_ARRAY_FLOAT_END_TAG,
    TOKEN_INT32_T_TAG,
    TOKEN_FLOAT_TAG,
    TOKEN_CHAR_TAG,
    TOKEN_CHAR_P_TAG,
    TOKEN_VECTOR2_TAG,
    TOKEN_VECTOR2F_TAG,
    TOKEN_VECTOR3_TAG,
    TOKEN_VECTOR3F_TAG,
    TOKEN_COMMENT,
    TOKEN_EOF,  // End of file token
    TOKEN_ERROR // Error token
} TokenType;

typedef enum {
    PROPERTY_TYPE_INT,
    PROPERTY_TYPE_INT32_T,
    PROPERTY_TYPE_FLOAT,
    PROPERTY_TYPE_CHAR,
    PROPERTY_TYPE_CHAR_P,
    PROPERTY_TYPE_STRING,
    PROPERTY_TYPE_VECTOR2,
    PROPERTY_TYPE_VECTOR2F,
    PROPERTY_TYPE_VECTOR3,
    PROPERTY_TYPE_VECTOR3F,
    // Add more types as needed...
    PROPERTY_TYPE_UNKNOWN // For unknown or unspecified types
} PropertyType;

// Token Structure
typedef struct {
    TokenType type; // Type of token
    char *value;
    PropertyType property_type;
} Token;


// Function to create a Token
Token *lexer_create_token(TokenType type, const char *value);


// Function to free a Token from memory
void lexer_free_token(Token *token);


// Function to free the an array of tokens
void lexer_free_tokens(Token **tokens, int token_count);


// Function to skip whitespace while moving through a file buffer
int lexer_skip_whitespace(const char *buffer, int index, int buffer_size);


// Function to extract tags from a scene file buffer
int lexer_extract_tags(Token **tokens, int *token_count, const char *buffer, int index, int buffer_length);


// Function to extract property names front a scene file buffer
int lexer_extract_property_names(Token **tokens, int *token_count, const char *buffer, int index, int buffer_length);


// Function to extract property values from a scene file buffer
int lexer_extract_property_values(Token **tokens, int *token_count, const char *buffer, int index, int buffer_length);


// Function to extract tuples from a scene file buffer
int lexer_extract_tuple_values(Token **tokens, int *token_count, const char *buffer, int index, int buffer_length);


// Function to extract all the tokens from a scene file and return the token stream
Token **lexer_extract_tokens(const char *buffer);


// Function to print ta token's type to string
void lexer_print_token_type(Token *token);


// Function to retrieve a token's type as a string
const char* lexer_get_token_type_as_string(TokenType *type);


// Function to retrieve token count from a token stream
int lexer_get_token_count(Token **token_stream);


// Function to print a token's properties
void lexer_print_token(Token *token);


// Function to print all tokens in the token stream
void lexer_print_tokens(Token **token_stream);



#endif // LEXER_H
