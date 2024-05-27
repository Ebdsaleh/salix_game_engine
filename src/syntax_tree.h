// syntax_tree.h
#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H
#include "file_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
typedef enum {
    SCENE_NODE,
    ENTITY_NODE,
    TRANSFORM_NODE,
    PROPERTY_NODE,
    TUPLE_NODE,
    ARRAY_NODE,
    UNKNOWN_NODE, // Default node type
} NodeType;

// Define the syntax tree node structure
typedef struct SyntaxTreeNode {
    NodeType type;                    // Type of node
    char *name;                       // Name of the node (e.g., entity name, property name)
    char *value;                      // Value associated with the node (e.g., property value)
    struct SyntaxTreeNode *parent;    // Pointer to the parent node
    struct SyntaxTreeNode *children;  // Pointer to the first child node
    struct SyntaxTreeNode *next;      // Pointer to the next sibling node
} SyntaxTreeNode;


// Function to create a new SyntaxTreeNode.
SyntaxTreeNode *syntax_tree_create_node(const char* node_name);


// Function to add a child to a SyntaxTreeNode
SyntaxTreeNode* syntax_tree_add_child(SyntaxTreeNode* parent, const char* tag_value);


// Function to add property name as a child node
void syntax_tree_add_property(SyntaxTreeNode *parent, const char *property_name);


// Function to add a property value as a child node
void syntax_tree_add_property_value(SyntaxTreeNode *parent, const char *property_value);


// Function to free SytanxTreeNode from memory
void syntax_tree_purge_node(SyntaxTreeNode * node);


// Function to free a Syntax Tree from memory
void syntax_tree_purge_entire_tree(SyntaxTreeNode *node);


// Function to get the node type as a string
const char *syntax_tree_get_node_type_string(NodeType *type);


// Function to print the syntax_tree
void print_syntax_tree(SyntaxTreeNode *node, int depth);


// Function to print the syntax_tree in a box style format
void print_syntax_tree_box_style(SyntaxTreeNode *node, int depth, int is_last);


// Function to get the last child in the syntax tree
SyntaxTreeNode *syntax_tree_get_last_child(SyntaxTreeNode *node);


// Function to get the node's depth in the syntax tree
int sytanx_tree_get_node_depth(SyntaxTreeNode *node);


// Function to get node by name
SyntaxTreeNode *syntax_tree_get_node_by_name(SyntaxTreeNode *tree, const char* name);


// Function to get the name of the parent node.
char* get_parent_name(SyntaxTreeNode *node); 
#endif // SYNTAX_TREE_H
