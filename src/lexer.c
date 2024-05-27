// lexer.c
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// Function to create a new Token
Token *lexer_create_token(TokenType type, const char *value) {
    Token *token = (Token *)malloc(sizeof(Token));
    
    if (token == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'token'.\n");
        exit(EXIT_FAILURE);
    }
    
    token->type = type;
    
    token->value = strdup(value);
    token->property_type = PROPERTY_TYPE_UNKNOWN;
    return token;
}


// Set the Tokens free!
void lexer_free_token(Token *token) {
    if (token == NULL) {
        fprintf(stderr, "ERROR: Cannot free a NULL 'token'.\n");
        return;
    }
    
    free(token->value);
    
    free(token);
}

void lexer_free_tokens(Token **tokens, int token_count) {
    if (tokens == NULL) {
        fprintf(stderr, "ERROR: Cannot free NULL tokens.\n");
        return;
    }

    for (int i = 0; i < token_count; i++) {
        lexer_free_token(tokens[i]);
    }
    
    free(tokens);
}


// Function to skip whitespace
int lexer_skip_whitespace(const char *buffer, int index, int buffer_length) {
    if (buffer == NULL) {
        fprintf(stderr, "ERROR: Cannot skip whitespace in a NULL 'buffer'.\n");
        return index;
    }

    
    while (index < buffer_length && isspace(buffer[index])) {
        index++;
    }

    return index;
}



// Function to extract tags
int lexer_extract_tags(Token **tokens, int *token_count, const char *buffer, int index, int buffer_length) {
    if (tokens == NULL) {
        fprintf(stderr, "ERROR: Cannot extract tags into a NULL 'tokens' array.\n");
        printf("Allocate memory successfully first.\n");
        return index;
    }

    if (buffer == NULL) {
        fprintf(stderr, "ERROR: Cannot extract 'tags' from a NULL 'buffer'.\n");
        printf("Read a 'scene' file into a 'buffer' first.\n");
        return index;
    }

    
    // Check for tags
    if (buffer[index] == '[') {
        
        // Find the end of the tag
        int tag_start = index;
        

        while (index < buffer_length && buffer[index] != ']') {
            index++;
        }

        
        if (index == buffer_length) {
            fprintf(stderr, "ERROR: Unterminated tag starting at position: %d \n", tag_start);
            lexer_free_tokens(tokens, *token_count);
            return index;
        }
        

        // Create a token for the tag
        char *tag_value = strndup(buffer + tag_start, index - tag_start + 1); // Don't exclude the '[' ']' from the tag.
        
        printf("Tag Value: '%s'\n", tag_value);
        
        tokens[*token_count] = lexer_create_token(TOKEN_TAG, tag_value);
        (*token_count)++;
        
        index++;
        
        free(tag_value);
    }

    return index;
}

// Function to extract property name
int lexer_extract_property_names(Token **tokens, int *token_count, const char *buffer, int index, int buffer_length) {
    if (tokens == NULL) {
        fprintf(stderr, "ERROR: Cannot extract property names into a NULL 'tokens' array.\n");
        printf("Allocate memory successfully first.\n");
        return index;
    }

    if (buffer == NULL) {
        fprintf(stderr, "ERROR: Cannot extract property names from a NULL 'buffer'.\n");
        printf("Read a 'scene' file into a 'buffer' first.\n");
        return index;
    }
    

    // Check for property names
    if (isalpha(buffer[index])) {
        // Find the end of the property name
        int property_name_start = index;
        

        while (index < buffer_length && buffer[index] != ':') {
            index++;
        }

            if (index == buffer_length) {
                fprintf(stderr, "ERROR: Unterminated property name starting at position: %d\n", property_name_start);
                lexer_free_tokens(tokens, *token_count);
                return index;
            }

            
            // Create token for property
            char* property_name = strndup(buffer + property_name_start, index - property_name_start);
            
            printf("Property Name: '%s'\n", property_name);
            
            tokens[*token_count] = lexer_create_token(TOKEN_PROPERTY_NAME, property_name);
            (*token_count)++;
            
            free(property_name);
            
            index++;
        }


    return index;
}


// function to extract property values
int lexer_extract_property_values(Token **tokens, int *token_count, const char *buffer, int index, int buffer_length) {

    if (tokens == NULL) {
        fprintf(stderr, "ERROR: Cannot extract property values into a NULL 'tokens' array.\n");
        printf("Allocate memory successfully first.\n");
        return index;
    }

    if (buffer == NULL) {
        fprintf(stderr, "ERROR: Cannot extract property values from a NULL 'buffer'.\n");
        printf("Read a 'scene' file into a 'buffer' first.\n");
        return index;
    }
    

    // Check for property value (string or numerical);
    if (buffer[index] == '"' || isdigit(buffer[index])) {

        // Determine the type of value (string or numerical)
        TokenType value_type = (buffer[index] == '"') ? TOKEN_STRING : TOKEN_NUMBER;

        // Find the end of the value
        int property_value_start = index;
        

        // Read to the end of the line
        while (index < buffer_length && buffer[index] != '\n') {
            index++;
        }

        
        // Create a token for the value
        char *property_value = strndup(buffer + property_value_start, index - property_value_start);
        
        printf("Property Value: '%s'\n", property_value);
        
        tokens[*token_count] = lexer_create_token(value_type, property_value);
        (*token_count)++;
        

        free(property_value);
    }

    return index;
}


// Function to extract tuple values

int lexer_extract_tuple_values(Token **tokens, int *token_count, const char *buffer, int index, int buffer_length) {

    // Check for tuple values enclosed in paranthesis
    if (buffer[index] == '(') {

        // Find the tuple end ')'
        int tuple_start = index;
        while (index < buffer_length && buffer[index] != ')') {
            index++;
        }

        if (index == buffer_length) {
            fprintf(stderr, "ERROR: Unterminated tuple starting at position: %d \n", tuple_start);
            lexer_free_tokens(tokens, *token_count);
            return index;
        }

        
        // Create a token for the tuple
        index++;
        
        char *tuple_value = strndup(buffer + tuple_start, index - tuple_start);
        
        printf("Tuple Value: %s\n", tuple_value);
        
        tokens[*token_count] = lexer_create_token(TOKEN_TUPLE, tuple_value);
        (*token_count)++;
        
        free(tuple_value);

    }

    return index;
}

// Lexer function
Token ** lexer_extract_tokens(const char *buffer) {
    if (buffer == NULL) {
        fprintf(stderr, "ERROR: Cannot use 'lex' on a NULL 'buffer'\n");
        return NULL;
    }
    
 
    // Current position in the buffer
    int index = 0;
    int buffer_length = strlen(buffer);
    int token_count = 0;


    // Initial capacity for tokens array
    int token_capacity = 10000;
    Token **tokens = (Token**)malloc(token_capacity * sizeof(Token *));
    if (tokens == NULL) {
        fprintf(stderr, "ERROR: failed to allocate memory for 'tokens'\n");
        return NULL;
    }
   
    // while loop
    while (index < buffer_length) {

        // Skip whitespace
        index = lexer_skip_whitespace(buffer, index, buffer_length);     
        // check for end of file
        if (index == buffer_length) {
            break; // Exit loop if end of buffer reached
        }



        index = lexer_extract_tags(tokens, &token_count, buffer, index, buffer_length);
        // check for end of file
        if (index == buffer_length) {
            break; // Exit loop if end of buffer reached
        }



        index = lexer_extract_property_names(tokens, &token_count, buffer, index, buffer_length);
        // check for end of file
        if (index == buffer_length) {
            break; // Exit loop if end of buffer reached
        }
 


        index = lexer_extract_property_values(tokens, &token_count, buffer, index, buffer_length); 
        // check for end of file
        if (index == buffer_length) {
            break; // Exit loop if end of buffer reached
        }

        

        index = lexer_extract_tuple_values(tokens, &token_count, buffer, index, buffer_length);
        // check for end of file
        if (index == buffer_length) {
            break; // Exit loop if end of buffer reached
        }
    }


    // Add EOF token
    tokens[token_count++] = lexer_create_token(TOKEN_EOF, "EOF");
    // Terminate the token array with NULL pointer
    tokens[token_count] = NULL; 

    printf("'token_count' at the end of 'lexer_extract_tokens' function: %d\n", token_count);
    return tokens;
}



// Function to retrieve a token's type as a string
const char* lexer_get_token_type_as_string(TokenType *type) {
    if (type == NULL) {
        fprintf(stderr, "ERROR: Cannot retrieve the type from a NULL 'token'.\n");
        return NULL;
    }
    
    const char *type_name = NULL;
    // print out the token types
    switch(*type) {
        case TOKEN_TAG:
            type_name = "TOKEN_TAG";
            break;

        case TOKEN_STRING:
            type_name = "TOKEN_STRING";
            break;

        case TOKEN_NUMBER:
            type_name = "TOKEN_NUMBER";
            break;

        case TOKEN_TUPLE:
            type_name = "TOKEN_TUPLE";
            break;

        case TOKEN_SCENE_TAG:
            type_name = "TOKEN_SCENE_TAG";
            break;

        case TOKEN_SCENE_END_TAG:
            type_name = "TOKEN_SCENE_END_TAG";
            break;

        case TOKEN_ENTITY_TAG:
            type_name = "TOKEN_ENTITY_TAG";
            break;

        case TOKEN_ENTITY_END_TAG:
            type_name = "TOKEN_ENTITY_END_TAG";
            break;

        case TOKEN_TRANSFORM_TAG:
            type_name = "TOKEN_TRANSFORM_TAG";
            break;

        case TOKEN_TRANSFORM_END_TAG:
            type_name = "TOKEN_TRANSFORM_END_TAG";
            break;

        case TOKEN_PROPERTY_NAME:
            type_name = "TOKEN_PROPERTY_NAME";
            break;

        case TOKEN_PROPERTY_VALUE:
            type_name = "TOKEN_PROPERTY_VALUE";
            break;

        case TOKEN_ARRAY_TAG:
            type_name = "TOKEN_ARRAY_TAG";
            break;

        case TOKEN_ARRAY_END_TAG:
            type_name = "TOKEN_ARRAY_END_TAG";
            break;

        case TOKEN_ARRAY_P_TAG:
            type_name = "TOKEN_ARRAY_P_TAG";
            break;

        case TOKEN_ARRAY_P_END_TAG:
            type_name = "TOKEN_ARRAY_P_END_TAG";
            break;

        case TOKEN_ARRAY_PP_TAG:
            type_name = "TOKEN_ARRAY_PP_TAG";
            break;

        case TOKEN_ARRAY_PP_END_TAG:
            type_name = "TOKEN_ARRAY_PP_END_TAG";
            break;

        case TOKEN_ARRAY_INT_TAG:
            type_name = "TOKEN_ARRAY_INT_TAG";
            break;

        case TOKEN_ARRAY_INT_END_TAG:
            type_name = "TOKEN_ARRAY_END_TAG";
            break;

        case TOKEN_ARRAY_INT32_T_TAG:
            type_name = "TOKEN_ARRAY_INT32_T_TAG";
            break;

        case TOKEN_ARRAY_INT32_T_END_TAG:
            type_name = "TOKEN_ARRAY_INT32_T_END_TAG";
            break;

        case TOKEN_ARRAY_FLOAT_TAG:
            type_name = "TOKEN_ARRAY_FLOAT_TAG";
            break;

        case TOKEN_ARRAY_FLOAT_END_TAG:
            type_name = "TOKEN_ARRAY_FLOAT_END_TAG";
            break;

        case TOKEN_CHAR_TAG:
            type_name = "TOKEN_CHAR_TAG";
            break;

        case TOKEN_CHAR_P_TAG:
            type_name = "TOKEN_CHAR_P_TAG";
            break;

        case TOKEN_VECTOR2_TAG:
            type_name = "TOKEN_VECTOR2_TAG";
            break;

        case TOKEN_VECTOR2F_TAG:
            type_name = "TOKEN_VECTOR2F_TAG";
            break;

        case TOKEN_VECTOR3_TAG:
            type_name = "TOKEN_VECTOR3_TAG";
            break;

        case TOKEN_VECTOR3F_TAG:
            type_name = "TOKEN_VECTOR3F_TAG";
            break;

        case TOKEN_COMMENT:
            type_name = "TOKEN_COMMENT";
            break;

        case TOKEN_EOF:
            type_name = "TOKEN_EOF";
            break;

        case TOKEN_ERROR:
            type_name = "TOKEN_ERROR";
            break;
        // default case
        default:
            type_name = "TOKEN_UNKNOWN";
            break;
    }

    return type_name;
}



// Function to print a token's properties
void lexer_print_token_type(Token *token) {
    if (token == NULL) {
        fprintf(stderr, "ERROR: Cannot print the 'TokenType' of a NULL 'token'.\n");
        return;
    }

    lexer_get_token_type_as_string(&token->type);
    return;
}


// Function to retrieve token count from a token stream
int lexer_get_token_count(Token **token_stream) {
    if (token_stream == NULL) {
        fprintf(stderr, "ERROR: Cannot retrieve a 'token_count' from a NULL 'token_stream'.\n");
        return -1;
    }
    
   int token_count = 0;

    // Iterate through the token stream until NULL is encountered
    while (token_stream[token_count] != NULL) {
        token_count++;
    }


   return token_count;

}



// Function to print a token's properties
void lexer_print_token(Token *token) {
    if (token == NULL) {
        fprintf(stderr, "ERROR: Cannot print a NULL 'token'.\n");
        return;
    }

    const char *type_name = lexer_get_token_type_as_string(&token->type); 
    printf("Token Type: %s, Value: %s\n", type_name, token->value);

}



// Function to print out the tokens
void lexer_print_tokens(Token **token_stream) {
    if (token_stream == NULL) {
        fprintf(stderr, "ERROR: Cannot print NULL tokens.\n");
        return;
    }
    
    int token_count = lexer_get_token_count(token_stream);
    for (int i = 0; i < token_count; i++) {
        lexer_print_token(token_stream[i]);
    }
}

