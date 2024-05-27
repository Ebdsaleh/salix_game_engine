// parser.h
#ifndef PARSER_H
#define PARSER_H

#include "scene_manager.h" // Include the header for scene-related data structures
#include "lexer.h" // Include the header for lexer functions
#include "syntax_tree.h"

// Function declarations

// Function to sanitize the token and assign their specific token type.
Token **parser_sanitize_tokens(Token **token_stream);

// Function to align a token's type with its value
void parser_align_token_tag(Token *token);

// Function to handle property names while building the syntax tree
void parser_handle_property_name(SyntaxTreeNode *current_node, const char *property_name);

// Function to handle property value while building the syntax tree
void parser_handle_property_value(SyntaxTreeNode *current_node, const char *value);

// Function to organize tokens into sections and build the syntax tree
SyntaxTreeNode* parser_build_syntax_tree(Token **token_stream);

// Function to parse the syntax tree.
void parse_syntax_tree(SyntaxTreeNode *tree);

Element *parser_build_element_from_syntax_tree(void *owner, SyntaxTreeNode *node);
void *parser_build_transform_from_syntax_tree(void *owner, SyntaxTreeNode *node);
Entity *parser_build_entity_from_syntax_tree_test(SyntaxTreeNode *node);
Entity *parser_build_entity_from_syntax_tree(SyntaxTreeNode *node);
Scene *parse_scene(Token **token_stream);
void parse_entity(Token **token_stream, int *index, SyntaxTreeNode *current_node);
void parse_element(Token **token_stream, int *index, SyntaxTreeNode *current_node);
void parse_tuple(SyntaxTreeNode *current_node, char *tuple_value);
char *parse_string(Token **token_stream, int index);
void *parse_number(Token **token_stream,int index);
TokenType parser_get_array_end_tag(TokenType *array_tag);
void parse_array(Token **token_stream,int *index, SyntaxTreeNode *current_node);
int32_t parse_int32(const char *value);
float parse_float(const char *value);
#endif /* PARSER_H */

