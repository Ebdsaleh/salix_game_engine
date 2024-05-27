// syntax_tree.c
#include "syntax_tree.h"
#include <stdio.h>
#include <string.h>


SyntaxTreeNode *syntax_tree_create_node(const char* node_name) {
    SyntaxTreeNode *tree_node = malloc(sizeof(SyntaxTreeNode));
    if (tree_node == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'tree_node'.\n");
        return NULL;
    }
    
    // initalize tree node to default values
    tree_node->type = UNKNOWN_NODE;

    // Allocate memory and copy node_name
    tree_node->name = malloc(strlen(node_name) + 1);
    if (tree_node->name == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'tree_node->name'.\n");
        return NULL;
    }
    strcpy(tree_node->name, node_name);


    tree_node->value = NULL;
    tree_node->next = NULL;
    tree_node->children = NULL;
    tree_node->parent = NULL;
    

    return tree_node;
}




// Function to add a child to a SyntaxTreeNode
SyntaxTreeNode* syntax_tree_add_child(SyntaxTreeNode* parent, const char* tag_value) {
    // NULL Checks
    if (parent == NULL) {
        fprintf(stderr, "ERROR: Cannot add child node to a NULL 'parent'.\n");
        return NULL;
    }

    if (tag_value == NULL) {
        fprintf(stderr, "ERROR: Cannot add child node to 'SyntaxTreeNode' with a NULL 'tag_value'.\n");
        return NULL;
    }


    
    // Create a new SyntaxTreeNode
    SyntaxTreeNode *new_node = syntax_tree_create_node(tag_value);
    if (new_node == NULL) {
        fprintf(stderr, "ERROR: Failed to create a new node.\n");
        return NULL;

    }

    new_node->parent = parent;
    
    // Set the parent-child relationship
    if (parent->children != NULL) {
        
        SyntaxTreeNode *last_child = syntax_tree_get_last_child(parent);
        last_child->next = new_node;

    } else {
        
        // If the parent has no children yet, set the new node as the first child
        parent->children = new_node;
    }


    // Return a pointer to the newly created child node
    return new_node;
}




// Function to add property name as a child node
void syntax_tree_add_property(SyntaxTreeNode *parent, const char *property_name) {
     // NULL Checks
    if (parent == NULL) {
        fprintf(stderr, "ERROR: Cannot add property to a NULL 'parent'.\n");
        return;
    }

    if (property_name == NULL) {
        fprintf(stderr, "ERROR: Cannot add property with a NULL 'property_name'.\n");
        return;
    }


    // Create new SyntaxTreeNode
    SyntaxTreeNode *property_node = syntax_tree_create_node(property_name);
    if (property_node == NULL) {
        fprintf(stderr, "ERROR: Failed to create a new'property_node'.\n");
        return;
    }

    property_node->parent = parent;
    property_node->type = PROPERTY_NODE;
    printf("PARENT NAME: %s\n", parent->name);

    // Set the parent-child relationship
    if (parent->children != NULL) {

        // Find the last child node
        SyntaxTreeNode *last_child = syntax_tree_get_last_child(parent);
        last_child->parent = parent;

        

        // Add the property node as a child
        last_child->next = property_node;

    } else {

        // If the parent has no children yet, set the new node as the first child
        parent->children = property_node;
    }

}



// Function to add property value to the syntax tree
void syntax_tree_add_property_value(SyntaxTreeNode *parent, const char *property_value) {
    // NULL Checks
    if (parent == NULL || property_value == NULL) {
        fprintf(stderr, "ERROR: Cannot add property value to a NULL parent or with a NULL value.\n");
        return;
    }

    // Find the last child node under the parent
    SyntaxTreeNode *last_child = syntax_tree_get_last_child(parent);
    // Set the value of the last child node to the property value
    if (last_child != NULL) {
        last_child->value = strdup(property_value);
        if (last_child->value == NULL) {
            fprintf(stderr, "ERROR: Failed to allocate memory for property value.\n");
            return;
        }
    } else {
        fprintf(stderr, "ERROR: Cannot add property value to a parent with no children.\n");
    }
}



// Function to free SyntaxTreeNode from memory
void syntax_tree_purge_node(SyntaxTreeNode *node) {
    if (node == NULL) {
        fprintf(stderr, "ERROR: Cannot free an already NULL 'node'.\n");
        return;
    }

    free(node->name);
    free(node->value);
    
    if (node->children != NULL) {
        syntax_tree_purge_entire_tree(node->children);

    }

    free(node);  

}



// Function to free SytanxTree from memory
void syntax_tree_purge_entire_tree(SyntaxTreeNode * node) {
    if (node == NULL) {
        fprintf(stderr, "ERROR: Cannot free an already NULL 'node'.\n");
        return;
    }

    free(node->name);
    free(node->value);

    // Free all childen recursively
    SyntaxTreeNode *current_child = node->children;
    while (current_child != NULL) {
        SyntaxTreeNode *next_child = current_child->next;
        syntax_tree_purge_node(current_child);
        current_child = next_child;
    }

    free(node);
    return;
}


// Function to get the node type as a string
const char *syntax_tree_get_node_type_string(NodeType *type) {
    if (type == NULL) {
        fprintf(stderr, "ERROR: Cannot return the value of a NULL 'NodeType'.\n");
        return NULL;
    }
    
    const char * type_name = NULL;
    switch(*type) {
        
        case SCENE_NODE:
            type_name = "SCENE_NODE";
            break;

        case PROPERTY_NODE:
            type_name = "PROPERTY_NODE";
            break;

        case ENTITY_NODE:
            type_name = "ENTITY_NODE";
            break;
            
        case TRANSFORM_NODE:
            type_name = "TRANSFORM_NODE";
            break;

        case ARRAY_NODE:
            type_name = "ARRAY_NODE";
            break;

        case TUPLE_NODE:
            type_name = "TUPLE_NODE";
            break;

        default:
            type_name = "UNKNOWN_NODE";
            break;
    }

    return type_name;
}

// Function to print the syntax tree
void print_syntax_tree(SyntaxTreeNode *node, int depth) {
    // NULL Check
    if (node == NULL) {
        fprintf(stderr, "ERROR: Cannot print a Syntax Tree from a NULL 'node'.\n");
        return;
    }

    
    // Print indentation based on depth
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    // Print node name
    printf("%s\n", node->name);
    printf("%s\n", syntax_tree_get_node_type_string(&node->type));


    // print node value is present
    
    if (node->value != NULL) {

        for (int i = 0; i <= depth; i++) {

        printf("  ");
        }

        printf("Value: %s\n", node->value);
    } else {
        printf("'node->value' is NULL.\n");
    }
    


    // Recursively print children
    if (node->children != NULL) {
        print_syntax_tree(node->children, depth + 1);
    }


    // Recursively print next sibling nodes
    if(node->next != NULL) {
        print_syntax_tree(node->next, depth);
    }
}



SyntaxTreeNode *syntax_tree_get_last_child(SyntaxTreeNode *node) {
    // NULL Check
    if (node == NULL) {
        fprintf(stderr, "ERROR: Cannot get 'last_child' from a NULL node.\n");
        return NULL;
    }
    
    SyntaxTreeNode *last_child = node->children;
    while (last_child != NULL && last_child->next != NULL) {
        // Iterate through the children until the end.
        last_child = last_child->next;

    }
    return last_child;
}




// Function to print the syntax tree in a box style
void print_syntax_tree_box_style(SyntaxTreeNode *node, int depth, int is_last) {
    // NULL Check
    if (node == NULL) {
        fprintf(stderr, "ERROR: Cannot print a Syntax Tree from a NULL 'node'.\n");
        return;
    }
    
    // Print indentation based on depth
    for (int i = 0; i < depth; i++) {
        printf("|  ");
    }

    // Print box-like representation of the node
    printf("%s\n", (is_last ? "\\--" : "+--"));

    // Print node name and value
    for (int i = 0; i <= depth; i++) {
        printf("|  ");
    }

    printf("%s: ", syntax_tree_get_node_type_string(&node->type));
    printf("%s: ", node->name);
    if (node->value != NULL) {
        printf("%s\n", node->value);
    } else {
        printf("'node->value' is NULL.\n");
    }

    // Recursively print children
    SyntaxTreeNode *child = node->children;
    while (child != NULL) {
        print_syntax_tree_box_style(child, depth + 1, child->next == NULL);
        child = child->next;
    }
}




// Function to get the node's depth in the syntax tree
int sytanx_tree_get_node_depth(SyntaxTreeNode *node) {
    // NULL checks
    if (node == NULL) {
        fprintf(stderr, "ERROR: Cannot get the 'depth' from a NULL 'node'.\n");
        return -1;
    }

    int depth = 0;
    SyntaxTreeNode *parent = node->parent;

    while (parent != NULL) {
        depth++;
        parent = parent->parent;

    }

    return depth;
}



// Function to get the name of the parent node.
char* get_parent_name(SyntaxTreeNode *node) {
    if (node == NULL || node->parent == NULL) {
        return NULL; // Invalid input or node has no parent
    }

    return node->parent->name;
}


// Function to get a node by name
SyntaxTreeNode *syntax_tree_get_node_by_name(SyntaxTreeNode* tree, const char* name){
    if (tree == NULL) {
        fprintf(stderr, "ERROR: Cannot search for a node from a NULL 'tree'.\n");
        return NULL;
    }

    if (name == NULL) {
        fprintf(stderr, "ERROR: Cannot seach for a node by 'name' with an NULL string.\n");
        return NULL;
    }

    SyntaxTreeNode *current_node = tree;

    // Start searching the tree from the root node
    while (current_node != NULL) {
        // Check that current node's name matches the target's name
        //printf("Checking current_node: '%s' against: '%s'\n", current_node->name, name);
        if (strcmp(current_node->name, name) == 0 ) {
            return current_node;
        }
        
        // Check if current node has children
        if (current_node->children != NULL) {
            // Recursively search through the children
            SyntaxTreeNode *found_node = syntax_tree_get_node_by_name(current_node->children, name);

            // if the node is found in the children, return it
            if (found_node != NULL) {
                return found_node;
            }
        }

        // Move to the next sibling
        current_node = current_node->next;

    }

    // if no node matches the given name, return NULL
    return NULL;

}
