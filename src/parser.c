// parser.c
#include "parser.h"
#include "element.h"
#include "entity.h"
#include "file_operations.h"
#include "lexer.h"
#include "scene.h"
#include "scene_manager.h"
#include "scene_properties.h"
#include "transform.h"
#include "scene_script_tags.h"
#include "syntax_tree.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Function to sanitize the token and assign their specific token type.
Token **parser_sanitize_tokens(Token **token_stream) {
    if (token_stream == NULL) {
        fprintf(stderr, "ERROR: Cannot sanitize a NULL 'token_stream'.\n");
        return NULL;
    }

    
    int index = 0;
    
    // Analyze the value of the current token and determine its TokenType
    // Update the TokenType field of the token accordingly
    
    // STEP 1 Align the tags to the correct token->type
    while(token_stream[index] != NULL) {

        parser_align_token_tag(token_stream[index]);

        index++;
    }


    return token_stream;

} 


void parser_align_token_tag(Token *token) {
    if (token == NULL) {
        fprintf(stderr, "ERROR: cannot align a NULL 'token'.\n");
        return;
    }

    // TOKEN_TAG
    if (token->type == TOKEN_TAG) {
        

        // [Scene]
        if (strcmp(token->value, "[Scene]") == 0) {
            token->type = TOKEN_SCENE_TAG;
            return;
        }


        // [/Scene]
        if (strcmp(token->value, "[/Scene]") == 0) {
            token->type = TOKEN_SCENE_END_TAG;
            return;
        }


        // [Entity]
        if (strcmp(token->value, "[Entity]") == 0) {
            token->type = TOKEN_ENTITY_TAG;
            return;
        }


        // [/Entity]
        if (strcmp(token->value, "[/Entity]") == 0) {
            token->type = TOKEN_ENTITY_END_TAG;
            return;
        }


        // [Transform]
        if (strcmp(token->value, "[Transform]") == 0) {
            token->type = TOKEN_TRANSFORM_TAG;
            return;
        }


        // [/Transform]
        if (strcmp(token->value, "[/Transform]") == 0) {
            token->type = TOKEN_TRANSFORM_END_TAG;
            return;
        }


        // [array**]
        if (strcmp(token->value, "[array**]") == 0) {
            token->type = TOKEN_ARRAY_PP_TAG;
            return;
        }


        // [/array**]
        if (strcmp(token->value, "[/array**]") == 0) {
            token->type = TOKEN_ARRAY_PP_END_TAG;
            return;
        }


        // [array*]
        if (strcmp(token->value, "[array*]") == 0) {
            token->type = TOKEN_ARRAY_P_TAG;
            return;
        }


        // [/array*]
        if (strcmp(token->value, "[/array*]") == 0) {
            token->type = TOKEN_ARRAY_P_END_TAG;
            return;
        }

        
        // [array]
        if (strcmp(token->value, "[array]") == 0) {
            token->type = TOKEN_ARRAY_TAG;
            return;
        }


        // [/array**]
        if (strcmp(token->value, "[/array]") == 0) {
            token->type = TOKEN_ARRAY_END_TAG;
            return;
        }
        

        // [array(int)]
        if (strcmp(token->value, "[array(int)]") == 0) {
            token->type = TOKEN_ARRAY_INT_TAG;
            return;
        }


        // [/array(int)]
        if (strcmp(token->value, "[/array(int)]") == 0) {
            token->type = TOKEN_ARRAY_INT_END_TAG;
            return;
        }


        // [array(int32_t)]
        if (strcmp(token->value, "[array(int32_t)]") == 0) {
            token->type = TOKEN_ARRAY_INT32_T_TAG;
            return;
        }


        // [/array(int32_t)]
        if (strcmp(token->value, "[/array(int32_t)]") == 0) {
            token->type = TOKEN_ARRAY_INT32_T_END_TAG;
            return;
        }


        // [array(float)]
        if (strcmp(token->value, "[array(float)]") == 0) {
            token->type = TOKEN_ARRAY_FLOAT_TAG;
            return;
        }


        // [array(/float)]
        if (strcmp(token->value, "[array(/float)]") == 0) {
            token->type = TOKEN_ARRAY_FLOAT_END_TAG;
            return;
        }


        // [int32_t]
        if (strcmp(token->value, "[int32_t]") == 0) {
            token->type = TOKEN_INT32_T_TAG;
            token->property_type = PROPERTY_TYPE_INT32_T;
            return;
        }


        // [float]
        if (strcmp(token->value, "[float]") == 0) {
            token->type = TOKEN_FLOAT_TAG;
            token->property_type = PROPERTY_TYPE_FLOAT;
            return;
        }


        // [char]
        if (strcmp(token->value, "[char]") == 0) {
            token->type = TOKEN_CHAR_TAG;
            token->property_type = PROPERTY_TYPE_CHAR;
            return;
        }


        // [char*]
        if (strcmp(token->value, "[char*]") == 0) {
            token->type = TOKEN_CHAR_P_TAG;
            token->property_type = PROPERTY_TYPE_CHAR_P;
            return;
        }


        // [Vector2]
        if (strcmp(token->value, "[Vector2]") == 0) {
            token->type = TOKEN_VECTOR2_TAG;
            token->property_type = PROPERTY_TYPE_VECTOR2;
            return;
        }


        // [Vector2f]
        if (strcmp(token->value, "[Vector2f]") == 0) {
            token->type = TOKEN_VECTOR2F_TAG;
            token->property_type = PROPERTY_TYPE_VECTOR2F;
            return;
        }


        // [Vector3]
        if (strcmp(token->value, "[Vector3]") == 0) {
            token->type = TOKEN_VECTOR3_TAG;
            token->property_type = PROPERTY_TYPE_VECTOR3;
            return;
        }


        // [Vector3f]
        if (strcmp(token->value, "[Vector3f]") == 0) {
            token->type = TOKEN_VECTOR3F_TAG;
            token->property_type = PROPERTY_TYPE_VECTOR3F;
            return;
        }

    }
    

    // Property Values

    if (token->type == TOKEN_STRING) {
        token->type = TOKEN_PROPERTY_VALUE;
        return;
    }
    

    if (token->type == TOKEN_NUMBER) {
        token->type = TOKEN_PROPERTY_VALUE;
        return;
    }



}



// Function to handle property names while building the syntax tree
void parser_handle_property_name(SyntaxTreeNode *current_node, const char *property_name) {
    // NULL check
    if (current_node == NULL) {
        fprintf(stderr, "ERROR: Cannot handle a property name with a NULL 'current_node'.\n");
        return;
    }

    if (property_name == NULL) {
        fprintf(stderr, "ERROR: Cannot handle a NULL 'property_name'.\n");
        return;
    }

    // Add property name as a child node to the current node
    syntax_tree_add_property(current_node, property_name);

}



// Function to handle property value while building the syntax tree
void parser_handle_property_value(SyntaxTreeNode *current_node, const char *property_value) {
     // NULL check
    if (current_node == NULL) {
        fprintf(stderr, "ERROR: Cannot handle a property name with a NULL 'current_node'.\n");
        return;
    }

    if (property_value == NULL) {
        fprintf(stderr, "ERROR: Cannot handle a NULL'property_value'.\n");
        return;
    }

    syntax_tree_add_property_value(current_node, property_value);

}

// Function to organize tokens into sections and build the syntax tree
SyntaxTreeNode* parser_build_syntax_tree(Token **token_stream) {
    if (token_stream == NULL) {
        fprintf(stderr, "ERROR: Cannot build syntax tree with a NULL 'token_stream'.\n");
        return NULL;
    }

    printf("Building syntax tree.\n");

    // Create a root node for the syntax tree
    SyntaxTreeNode *root = syntax_tree_create_node("Scene");
    if(root == NULL) {
        fprintf(stderr, "ERROR: Failed to create a Scene node while building the 'syntax_tree'.\n");
        return NULL;
    }
    root->value = strdup("Scene Root");
    root->type = SCENE_NODE;
    int index = 0;
    SyntaxTreeNode *current_node = root;
    

    while (token_stream[index] != NULL) {
        // Check token type to determine how to handle it
        Token *token = token_stream[index];

        switch (token->type) {
            case TOKEN_SCENE_TAG:
                printf("Parsing Scene\n");
                // Create a new node for the current tag
                //current_node = syntax_tree_add_child(current_node, token->value);
                break;
           
            case TOKEN_PROPERTY_NAME:
                printf("Parsing at property name at Scene Level: %s\n", token->value);
                // Add property name as a child node
               parser_handle_property_name(current_node, token->value);
                break;

            case TOKEN_PROPERTY_VALUE:
                printf("Parsing at property value at Scene Level: %s\n", token->value);
                // Add property value as a child node
                parser_handle_property_value(current_node, token->value);
                break;

            case TOKEN_ARRAY_PP_TAG:

                printf("Parsing at Array at Scene Level: %s\n", token->value);
                parse_array(token_stream, &index, current_node);

                // Assign the node's 'value' to the correct type
                SyntaxTreeNode *last_child = syntax_tree_get_last_child(current_node);
                
                if (strcmp(last_child->value, "Array entities") == 0){
                    last_child->value = strdup(SCENE_PROPERTY_ENTITIES_VALUE_TYPE);
                    break;
                }
                
                break;

            case TOKEN_SCENE_END_TAG:
                break;
            
                // Add cases for other token types as needed
            default:
                // Ignore or handle other token types
                break;
        }

        if (token->type == TOKEN_SCENE_END_TAG) {
            // Finshed building the syntax tree, break the while loop and return
            // the root node.
            break;
        }


        index++;
    }

    return root;
}




// Function to parse a scene token stream and return a scene
Scene* parse_scene(Token** token_stream){
    if (token_stream == NULL) {
        fprintf(stderr, "ERROR: Cannot parse scene with a NULL 'token_stream'.\n");
        return NULL;
    }

    // Sanitize token stream
    parser_sanitize_tokens(token_stream);
    printf("'tokens' after sanitization.\n");
    lexer_print_tokens(token_stream);
    SyntaxTreeNode *syntax_tree = parser_build_syntax_tree(token_stream);
    
    // Print the syntax tree after building it
    printf("PRINTING SYNTAX TREE:\n");
    print_syntax_tree_box_style(syntax_tree, 0, 0);

    // Create the scene from the syntax_tree
    Scene *scene = scene_create();
    
    // We need the properties of the Scene
    //  char *name
    // int32_t entity_count
    // Entity ** entities (Array entities) <- in the syntax tree
    SyntaxTreeNode *node = syntax_tree_get_node_by_name(syntax_tree, "name");
    if (node == NULL) {
        fprintf(stderr, "ERROR: 'name' not defined in scene file\n");
        syntax_tree_purge_entire_tree(syntax_tree);
        return NULL;
    }
    char* name = strdup(node->value);
    scene_set_name(scene, node->value);
    printf("%s\n", name);


    node = syntax_tree_get_node_by_name(syntax_tree, "entity_count");
    if(node == NULL) {
        fprintf(stderr, "ERROR: 'entity_count' not defined in scene file.\n");
        syntax_tree_purge_entire_tree(syntax_tree);
        return NULL;
    }
    char* entity_count_value = strdup(node->value);
    printf("%s\n", node->value);
    // Convert from char* to int32_t
    int32_t entity_count = parse_int32(node->value);
    

    // Parse the entities
    node = syntax_tree_get_node_by_name(syntax_tree, "Array entities");
    for (int i = 0; i < entity_count; i++) {
        
        if (strcmp(node->children->name, "Entity") == 0) {
            node = node->children;
        }
        // call function that builds entity from the syntax tree
        Entity *entity = parser_build_entity_from_syntax_tree_test(node);
        printf("Adding Entity: %s to scene\n", entity->name);
        scene_manager_add_entity(scene, entity);

        if (node->next != NULL) {
            node = node->next;
            printf("%s\n", node->children->name);
        }

    }
   


    // Free the syntax tree as it is no longer needed
    syntax_tree_purge_entire_tree(syntax_tree);

    return scene;

}

void *parser_build_transform_from_sytanx_tree(void *owner, SyntaxTreeNode *node) {
    // NULL check
    // NULL Check
    if (node == NULL) {
        fprintf(stderr, "ERROR: Cannot build an Transform from a NULL 'node'.\n");
        return NULL;
    }

    if (owner == NULL) {
        fprintf(stderr, "ERROR: Cannot build an from Transform from a NULL 'owner'\n");
        return NULL;
    }

    Transform *transform = transform_prepare_type();
    transform_init((Element *) transform, owner);
    SyntaxTreeNode *transform_node = node;

    // nested node entry points
    SyntaxTreeNode *position_node = NULL;
    SyntaxTreeNode *rotation_node = NULL;
    SyntaxTreeNode *scale_node = NULL;
    
    while (transform_node != NULL) {
       switch (transform_node->type) {

           case PROPERTY_NODE:
               printf("Found property: %s\n", transform_node->name);
               if (strcmp(transform_node->name, TRANSFORM_PROPERTY_NAME) == 0) {
                   printf("%s Value: %s\n", transform_node->name, transform_node->value);
                   transform->base.name = strdup(transform_node->value);
                   printf("applied: %s to: %s\n", transform_node->value, transform_node->name);
               }

               if (strcmp(transform_node->name, "owner.name") == 0) {
                   printf("%s Value: %s\n", transform_node->name, transform_node->value);
                   transform->owner->name = transform_node->value;
                   printf("applied: %s to: %s\n", transform_node->value, transform_node->name);
               }

               if (strcmp(transform_node->name, TRANSFORM_PROPERTY_POSITION) == 0) {
                   position_node = transform_node;
                   transform_node = transform_node->children;
                   break;
               }
               
               if (strcmp(transform_node->name, TRANSFORM_PROPERTY_ROTATION) == 0) {
                   rotation_node = transform_node;
                   transform_node = transform_node->children;
                   break;

               }

               if (strcmp(transform_node->name, TRANSFORM_PROPERTY_SCALE) == 0) {
                   scale_node = transform_node;
                   transform_node = transform_node->children;
                   break;

               }

               
               // if position properties has run out of members move to up to the position node and move to the next property
               if(strcmp(transform_node->parent->name, "position properties") == 0) {
                   if (strcmp(transform_node->name, "position property 0") == 0) {
                       printf("%s Value: %s\n", transform_node->name, transform_node->value);
                       transform->position->x = parse_int32(transform_node->value);
                       printf("applied: %s to: %s\n", transform_node->value, transform_node->name);
                       
                   }

                   if (strcmp(transform_node->name, "position property 1") == 0) {
                       printf("%s Value: %s\n", transform_node->name, transform_node->value);
                       transform->position->y = parse_int32(transform_node->value);
                       printf("applied: %s to: %s\n", transform_node->value, transform_node->name);
                   }

                   if (strcmp(transform_node->name, "position property 2") == 0) {
                       printf("%s Value: %s\n", transform_node->name, transform_node->value);
                       transform->position->z = parse_int32(transform_node->value);
                       printf("applied: %s to: %s\n", transform_node->value, transform_node->name);
                   }
                   
                   if (transform_node->next == NULL) {
                       transform_node = position_node->next;
                       printf("Next is NULL, node at: %s\n", transform_node->name);
                       break;
                   }
               } 

               // No more members in rotation, move to the scale
               if (strcmp(transform_node->parent->name, "rotation properties") == 0) {
                   if (strcmp(transform_node->name, "rotation property 0") == 0) {
                       printf("%s Value: %s\n", transform_node->name, transform_node->value);
                       transform->position->x = parse_int32(transform_node->value);
                       printf("applied: %s to: %s\n", transform_node->value, transform_node->name);

                   }

                   if (strcmp(transform_node->name, "rotation property 1") == 0) {
                       printf("%s Value: %s\n", transform_node->name, transform_node->value);
                       transform->position->y = parse_int32(transform_node->value);
                       printf("applied: %s to: %s\n", transform_node->value, transform_node->name);
                   }

                   if (strcmp(transform_node->name, "rotation property 2") == 0) {
                       printf("%s Value: %s\n", transform_node->name, transform_node->value);
                       transform->position->z = parse_int32(transform_node->value);
                       printf("applied: %s to: %s\n", transform_node->value, transform_node->name);
                   }

                   if (transform_node->next == NULL) {
                       transform_node = rotation_node->next;
                       printf("Next is NULL, node at: %s\n", transform_node->name);
                       break;
                   }

               }
               
               if (strcmp(transform_node->parent->name, "scale properties") == 0) {
                   if (strcmp(transform_node->name, "scale property 0") == 0) {
                       printf("%s Value: %s\n", transform_node->name, transform_node->value);
                       transform->position->x = parse_float(transform_node->value);
                       printf("applied: %s to: %s\n", transform_node->value, transform_node->name);
                   }

                   if (strcmp(transform_node->name, "scale property 1") == 0) {
                       printf("%s Value: %s\n", transform_node->name, transform_node->value);
                       transform->position->y = parse_float(transform_node->value);
                       printf("applied: %s to: %s\n", transform_node->value, transform_node->name);
                   }

                   if (strcmp(transform_node->name, "scale property 2") == 0) {
                       printf("%s Value: %s\n", transform_node->name, transform_node->value);
                       transform->position->z = parse_float(transform_node->value);
                       printf("applied: %s to: %s\n", transform_node->value, transform_node->name);
                   }

                   // No more members in scale transform should be complete
                   if (transform_node->next == NULL) {
                       printf("scale properties parent: %s\n", transform_node->parent->parent->name); 
                       transform_node = NULL;
                       break;

                   }
               }

                
               if (transform_node->next != NULL) {

                   transform_node = transform_node->next;
                   break;
               } else {
                   return (Element *) transform;
               }
               break;

           case TUPLE_NODE:
               printf("Found tuple: %s\n", transform_node->name);
               transform_node = transform_node->children;
               printf("First property in tuple: %s\n", transform_node->name);
               break;


           default:
               break;
       } 
    }
    printf("Returning Transform. \n");
    return (Element *)transform;

}

Element *parser_build_element_from_syntax_tree(void *owner, SyntaxTreeNode *node) {

    // NULL Check
    if (node == NULL) {
        fprintf(stderr, "ERROR: Cannot build an Element from a NULL 'node'.\n");
        return NULL;
    }

    if (owner == NULL) {
        fprintf(stderr, "ERROR: Cannot build an from Element from a NULL 'owner'\n");
        return NULL;
    }
    SyntaxTreeNode *element_node = node;
    Element * element = malloc(sizeof(Element));


    while (element_node != NULL) {
        printf("Element node: %s\n", element_node->name);
        switch (element_node->type) {

            case TRANSFORM_NODE:
                printf("Found %s\n", element_node->name);
                element_node = element_node->children;
                element = (Element *)parser_build_transform_from_sytanx_tree(owner, element_node);
                return element;

                break;

            default:
                break;
        }
        break;
    }
    

    return NULL;
}



Entity *parser_build_entity_from_syntax_tree_test(SyntaxTreeNode *node) {

    // NULL Check
    if (node == NULL) {
        fprintf(stderr, "ERROR: Cannot build an Entity from a NULL 'node'.\n");
        return NULL;
    }

    SyntaxTreeNode *entity_node = node;
    printf("%s\n", node->name);
    entity_node = entity_node->children;
    printf("%s\n", node->name);
    SyntaxTreeNode *id_node = NULL;
    SyntaxTreeNode *elements_node = NULL;
    int32_t element_count = 0;


    Entity *entity = entity_create_for_deserialization();
    // Fails as it goes into an infinite loop NEED HELP
    while (entity_node != NULL) {
 

        switch(entity_node->type) {

            case PROPERTY_NODE:
                
                printf("Found property node: %s, Value: %s\n", entity_node->name, entity_node->value);
                if (strcmp(entity_node->name, ENTITY_PROPERTY_ID) == 0) {
                    id_node = entity_node;
                    entity_node = entity_node->children;
                    break;
                }

                if (strcmp(entity_node->parent->name, "id properties") == 0) {
                    
                    if (strcmp(entity_node->name, "id property 0") == 0) {
                        entity->id.prefix = entity_node->value;
                        entity_node = entity_node->next;
                        break;
                        
                    }

                    if (strcmp(entity_node->name, "id property 1") == 0) {
                        entity->id.number = parse_int32(entity_node->value);
                        
                    }

                    if (entity_node->next == NULL) {
                        entity_node = id_node;
                        entity_node = entity_node->next;
                        break;
                    }
                }

                if (strcmp(entity_node->name, ENTITY_PROPERTY_NAME) == 0) {
                    printf("Applying name '%s' to Entity\n", entity_node->value);
                    entity_set_name(entity, entity_node->value);
                    entity_node = entity_node->next;
                    
                }

                if (strcmp(entity_node->name, ENTITY_PROPERTY_ELEMENT_COUNT) == 0) {
                    printf("storing 'element_count': %s\n", entity_node->value);
                    element_count = parse_int32(entity_node->value);
                }

                if(entity_node->next != NULL) {
                    entity_node = entity_node->next;
                    break;
                } else {
                    printf("entity_node : %s, next is NULL.\n", entity_node->name);
                    entity_node = NULL;
                    
                }
                
                break;

            case TUPLE_NODE:

                printf("Found tuple node: %s\n", entity_node->name);
                entity_node = entity_node->children;

                break;

            case ARRAY_NODE:

                printf("Found array node: %s\n", entity_node->name);
                elements_node = entity_node->children;
                entity_node = entity_node->children;
                Element *element = parser_build_element_from_syntax_tree(entity, entity_node);
                entity_add_element(entity, element);
                if (elements_node->next != NULL) {
                    entity_node = elements_node->next;
                    elements_node = elements_node->next;
                }
                entity_node = entity_node->children;

                break;


            default:
                break;
        }
        
    }
    

    if (entity->element_count != element_count) {
        printf("Entity: %s has 'element_count' mismatch.\nIn file: %"PRId32",\tOn Entity: %"PRId32"\n", entity->name, element_count, entity->element_count);
    }
    return entity;

}

Entity *parser_build_entity_from_syntax_tree(SyntaxTreeNode *node) {
    // NULL Check
    if (node == NULL) {
        fprintf(stderr, "ERROR: Cannot build an Entity from a NULL 'null'.\n");
        return NULL;
    }

    SyntaxTreeNode *entity_node = node;
    printf("%s\n", node->name);
    


    Entity * entity = entity_create_for_deserialization();


    // Handle the Entity Id as defined as a tuple
    if (strcmp(entity_node->children->name, ENTITY_PROPERTY_ID) == 0) {
        // Get the Entity Id
        entity_node = entity_node->children->children;
        printf("%s\n", entity_node->name);

        // Check if the next node is a tuple or property node
        if (entity_node->type == TUPLE_NODE){
            printf("Tuple node found for Id.\n");
            // Handle the tuple value assignment of ID

            // To access the members of the tuple we need to move into the
            // first child of the tuple node.
            SyntaxTreeNode *id_properties_node = entity_node;
            id_properties_node = id_properties_node->children;
            printf("%s\n", id_properties_node->name);

            // we are now in the first property node
            // Apply the id.prefix to the new entity.
            entity->id.prefix = strdup(id_properties_node->value);
            printf("%s\n", id_properties_node->value);

            // Move to the next property of Id
            id_properties_node = id_properties_node->next;
            int32_t id_number = parse_int32(id_properties_node->value);
            printf("%"PRId32"\n", id_number);
            // Set the id_number to the entity
            entity->id.number = id_number;
            // Id assignment complete
            printf("Id assignment complete.\n");

            // set the entity node to the name property
            entity_node = entity_node->parent;
            entity_node = entity_node->children->next;

            printf("%s\n", entity_node->name);
        }
    }     

    // Get the name node
    if (strcmp(entity_node->name, ENTITY_PROPERTY_NAME) == 0) {

        // Set the entity's name
        entity_set_name(entity, entity_node->value);
        printf("Set entity name to: %s\n", entity->name);

        // Move to the next property
        entity_node = entity_node->next;
    }

    // Get the element count
    if(strcmp(entity_node->name, ENTITY_PROPERTY_ELEMENT_COUNT) == 0) {
        int32_t element_count = parse_int32(entity_node->value);
        printf("Read 'element_count' value: %"PRId32"\n", element_count);

        // Move to the next property
        entity_node = entity_node->next;
    }
    printf("%s\n", entity_node->name);




    //printf("%s\n", entity_node->children->name);
    return entity;
}


void parse_entity(Token** token_stream, int *index, SyntaxTreeNode* current_node) {
    if (token_stream == NULL) {
        fprintf(stderr, "ERROR: Cannot parse entity with a NULL 'token_stream'.\n");
        return;
    }
    
    if (current_node == NULL) {
        fprintf(stderr, "ERROR: Cannot parse entity from a NULL 'SyntaxTreeNode'.\n");
        return;
    }

    if (index == NULL) {
        fprintf(stderr, "ERROR: Cannot parse entity when given a NULL 'index' pointer.\n");
        return;
    }

    // Increment index
    (*index)++;

    // Create a new node for the current tag
    SyntaxTreeNode *entity_node = syntax_tree_add_child(current_node, "Entity");
    entity_node->value = strdup("Entity*");
    entity_node->type = ENTITY_NODE;
    current_node = entity_node;

    printf("Parsing Entity.\n");
    printf("Parent node name: %s\n", current_node->parent->name);

    // go through the token stream from the entity tag position
    while (token_stream[*index] != NULL){
        Token *token = token_stream[*index];

        switch (token->type) {

            // return from function when [/Entity] tag is found
            case TOKEN_ENTITY_END_TAG:
                return;

            case TOKEN_PROPERTY_NAME:
                printf("Parsing Entity property name: %s\n", token->value);
                parser_handle_property_name(current_node, token->value);

                // check Entity property name to assign the correct value
                SyntaxTreeNode *last_child = syntax_tree_get_last_child(current_node);


                if(strcmp(token->value, ENTITY_PROPERTY_ID) == 0) {
                    last_child->value = strdup(ENTITY_PROPERTY_ID_VALUE_TYPE);
                    break;
                }

                
                if (strcmp(token->value, ENTITY_PROPERTY_NAME) == 0) {
                    last_child->value = strdup(ENTITY_PROPERTY_NAME_VALUE_TYPE);
                    break;
                }

                
                if (strcmp(token->value, ENTITY_PROPERTY_ELEMENT_COUNT) == 0) {
                        last_child->value = strdup(ENTITY_PROPERTY_ELEMENT_COUNT_VALUE_TYPE);
                        break;
                }
                
                break;

            case TOKEN_PROPERTY_VALUE:
                printf("Parsing Entity property value: %s\n", token->value);
                parser_handle_property_value(current_node, token->value);
                break;

            case TOKEN_ARRAY_PP_TAG:
                printf("Parsing Entity Array: %s\n", token->value);
                parse_array(token_stream, index, current_node);

                // Assign the node's correct value type
                last_child = syntax_tree_get_last_child(current_node);

                if (strcmp(last_child->value, "Array elements") == 0) {
                    last_child->value = strdup(ENTITY_PROPERTY_ELEMENTS_VALUE_TYPE);
                    break;
                }

                break;

            case TOKEN_TUPLE:
                printf("Parsing Entity Tuple Value: %s\n", token->value);
                parse_tuple(current_node, token->value);
                
                break;

            default:
                // Ignore or handle other tokens;
                break;
        }
        (*index)++;
    }
    return;
}






void parse_tuple(SyntaxTreeNode *current_node, char *tuple_value) {
    // NULL Checks
    if (current_node == NULL) {
        fprintf(stderr, "ERROR: Cannot parse tuple from a NULL 'SyntaxTreeNode'.\n");
        return;
    }

    if (tuple_value == NULL) {
        fprintf(stderr, "ERROR: Cannot parse tuple when given a NULL 'index' pointer.\n");
        return;
    }

    printf("Tuple value passed into 'parse_tuple' function: %s\n", tuple_value);

    // Copy the value for manipulation
    char* tuple_copy = strdup(tuple_value);
    if (tuple_copy == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'tuple_copy'\n");
        return;
    }

    // Get last child's name
    SyntaxTreeNode *last_child = syntax_tree_get_last_child(current_node);


    printf("TUPLE CURRENT NODE NAME: %s\n", last_child->name);
    printf("Parent node name in TUPLE: %s\n", last_child->parent->name);
    
    char last_child_name[(strlen(last_child->name) + 1 * sizeof(char))];
    sprintf(last_child_name, "%s", last_child->name);

    char tuple_name[256];
    snprintf(tuple_name, sizeof(tuple_name), "%s properties", last_child_name);
    
    char p_count[256];


    SyntaxTreeNode *tuple_node = syntax_tree_add_child(last_child, tuple_name);
    if (tuple_node == NULL) {
        fprintf(stderr, "ERROR: Failed to add child node for tuple properties.\n");
        return;
    }
    tuple_node->parent = current_node;
    tuple_node->value = strdup(tuple_name);
    tuple_node->type = TUPLE_NODE;

    int count = 0;
    // Tokenize tuple based on parentheses and commas strtok("(),")
    char *token = strtok(tuple_copy, "(),");

    while (token != NULL) {

        // Trim whitespace from the token
        char *trimmed_token = trim(token);
        printf("%s\n", trimmed_token);

        // Add each token as a child node to the current node

        snprintf(p_count, sizeof(p_count), "%s property %d", last_child_name, count);
        
        syntax_tree_add_property(tuple_node, p_count);
        syntax_tree_add_property_value(tuple_node, trimmed_token);
        printf("Adding value from tuple to syntax tree: %s\n", trimmed_token);

                
        // Move to the next token
        token = strtok(NULL, "(),");
        count++;
    }

    free(tuple_copy);

}


char *parse_string(Token **token_stream, int index) {

    return NULL;
}



void *parse_number(Token **token_stream,int index) {


    return NULL;
}




TokenType parser_get_array_end_tag(TokenType *array_tag) {
    if (array_tag == NULL) {
        fprintf(stderr, "ERROR: Cannot get Array end tag from a NULL 'array_tag'.\n");
        return TOKEN_ERROR;
    }

    printf("Parsing Array of type: %s\n", lexer_get_token_type_as_string(array_tag));

    TokenType array_end_tag;
    switch (*array_tag) {

        case TOKEN_ARRAY_TAG:
            array_end_tag = TOKEN_ARRAY_END_TAG;
            break;

        case TOKEN_ARRAY_P_TAG:
            array_end_tag = TOKEN_ARRAY_P_END_TAG;
            break;
            
        case TOKEN_ARRAY_PP_TAG:
            array_end_tag = TOKEN_ARRAY_PP_END_TAG;
            break;

        case TOKEN_ARRAY_INT_TAG:
            array_end_tag = TOKEN_ARRAY_INT_END_TAG;
            break;

        case TOKEN_ARRAY_INT32_T_TAG:
            array_end_tag = TOKEN_ARRAY_INT32_T_END_TAG;
            break;

        case TOKEN_ARRAY_FLOAT_TAG:
            array_end_tag = TOKEN_ARRAY_FLOAT_END_TAG;
            break;

        default:
            break;
    
    }

    return array_end_tag;

}



void parse_array(Token **token_stream,int *index, SyntaxTreeNode *current_node) {
    // NULL checks

    if (token_stream == NULL) {
        fprintf(stderr, "ERROR: Cannot parse array with a NULL 'token_stream'.\n");
        return;
    }
    
    if (current_node == NULL) {
        fprintf(stderr, "ERROR: Cannot parse array from a NULL 'SyntaxTreeNode'.\n");
        return;
    }

    if (index == NULL) {
        fprintf(stderr, "ERROR: Cannot parse array when given a NULL 'index' pointer.\n");
        return;
    }

    
    // Get the type of array that called the function.
    TokenType array_type_tag = token_stream[*index]->type;
    
    // set the corresponding ending tag for the array
    TokenType array_type_end_tag = parser_get_array_end_tag(&array_type_tag);
    if (array_type_end_tag == TOKEN_ERROR) {
        fprintf(stderr, "ERROR: Recieved TOKEN_ERROR, when getting the 'array_type_end_tag'.\n");
        printf("Check the .scene file syntax.\n");
        return;
    }
    


    // Move through the tokens until the first property name is found
    // This will be the name of the array.
        
    while (token_stream[*index]->type != TOKEN_PROPERTY_NAME) {
        (*index)++;
    }


    // Set the array name
    char array_name[256];
    sprintf(array_name, "Array %s", token_stream[*index]->value);

    // Add array to the sytanx tree
    
    SyntaxTreeNode *array_node = syntax_tree_add_child(current_node,  array_name);
    array_node->value = strdup(array_name);
    array_node->type = ARRAY_NODE;
    current_node = array_node;
    
    // move to the next token 
    (*index)++;

    while (token_stream[*index]->type != array_type_end_tag) {

        Token* token = token_stream[*index];

        switch(token->type) {

            case TOKEN_PROPERTY_NAME:
                parser_handle_property_name(current_node, token->value);
                break;
            
            case TOKEN_PROPERTY_VALUE:
                parser_handle_property_value(current_node, token->value);
                break;

            case TOKEN_ENTITY_TAG:
                parse_entity(token_stream, index, current_node);
                break;

            case TOKEN_TRANSFORM_TAG:
                parse_element(token_stream, index, current_node);
            default:
                break;
        }
        (*index)++;
    }

    return;
}



void parse_element(Token** token_stream, int *index, SyntaxTreeNode *current_node) {
    if (token_stream == NULL) {
        fprintf(stderr, "ERROR: Cannot parse element with a NULL 'token_stream'.\n");
        return;
    }
    TokenType element_type = token_stream[*index]->type;

    switch(element_type){

        case TOKEN_TRANSFORM_TAG:
            // Transform
            printf("Parsing Transform\n");
            SyntaxTreeNode *transform_node = syntax_tree_add_child(current_node,"Transform");
            transform_node->value = strdup("Transform*");
            transform_node->type = TRANSFORM_NODE;
            current_node = transform_node;
            while (token_stream[*index] != NULL) {

                Token *token = token_stream[*index];

                if(token->type == TOKEN_TRANSFORM_END_TAG) {
                    return;
                }

                if (token->type == TOKEN_PROPERTY_NAME) {
                    printf("Parsing Transform property name: %s\n", token->value);
                    parser_handle_property_name(current_node,token->value);
                }

                if (token->type == TOKEN_PROPERTY_VALUE) {
                    printf("Parsing Transform property value: %s\n", token->value);
                    parser_handle_property_value(current_node, token->value);
                }

                if (token->type == TOKEN_TUPLE) {
                    printf("Parsing Transform tuple value: %s\n", token->value);
                    parse_tuple(current_node, token->value);

                    // Assign the node's correct 'value'
                    SyntaxTreeNode *last_child = syntax_tree_get_last_child(current_node);

                    if (strcmp(last_child->name, TRANSFORM_PROPERTY_POSITION) == 0) {
                        last_child->value = strdup(TRANSFORM_PROPERTY_POSITION_VALUE_TYPE);
                    }

                    if (strcmp(last_child->name, TRANSFORM_PROPERTY_ROTATION) == 0) {
                        last_child->value = strdup(TRANSFORM_PROPERTY_ROTATION_VALUE_TYPE);
                    }

                    if (strcmp(last_child->name, TRANSFORM_PROPERTY_SCALE) == 0) {
                        last_child->value = strdup(TRANSFORM_PROPERTY_SCALE_VALUE_TYPE);
                    }
                }

                (*index)++;
            
            }

            break;


        default:
            break;

        
    }
    return;
}

int32_t parse_int32(const char *value) {
    
    return atoi(value);
}


float parse_float(const char *value) {
    return atof(value);
}
