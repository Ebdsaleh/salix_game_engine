// scene_manager.c

#include "scene_manager.h"
#include "entity.h"
#include "lexer.h"
#include "parser.h"
#include "scene.h"
#include "file_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAG_BUFFER_SIZE 2048
#define PROPERTY_BUFFER_SIZE 2048

// Function to create a new scene
Scene *scene_manager_create_scene(const char *new_scene_name) {
    if (new_scene_name == NULL) {
        fprintf(stderr, "ERROR: Cannot create new Scene with a NULL 'new_scene_name'.\n");
        return NULL;
    }
    
    Scene *new_scene = scene_create();

    if(new_scene == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for Scene 'new_scene'.\n");
        return NULL;
    }
    
    scene_set_name(new_scene, new_scene_name);
    
    return new_scene;
}


void scene_manager_add_entity(Scene *scene, Entity *entity) {
    if (scene == NULL) {
        fprintf(stderr, "ERROR: Cannot add an Entity to an NULL Scene.\n");
        return;
    }

    if (entity == NULL) {
        fprintf(stderr,"ERROR: Cannot add a NULL Entity to Scene %s.\n", scene->name);
        return;
    }

    scene_add_entity(scene, entity);
    return;

}


void scene_manager_remove_entity(Scene *scene, Entity *entity) {
    if (scene == NULL) {
        fprintf(stderr, "ERROR: Cannot remove Entity from a NULL Scene.\n");
        return;
    }

    if (entity == NULL) {
        fprintf(stderr, "ERROR Cannot remove a NULL Entity from Scene %s", scene->name);
    }

    scene_remove_entity(scene, entity);
    return;
}







 // NEED TO CHECK SERIALIZATION AND DE-SERIALIZATION OF ALL ELEMENTS THE MUST ALIGN!

int scene_manager_serialize_scene(Scene *scene, const char *file_name) {
    if (scene == NULL || file_name == NULL) {
        fprintf(stderr, "ERROR: Invalid Scene or file name.\n");
        return -1;
    }

    // check if file exists
    if (check_if_file_exists(file_name)) {
        printf("File '%s' already exists.\nOverwriting file...\n", file_name);
        // overwrite file

    }
    else {
        printf("File '%s' does not exist or cannot be opened for reading.\n", file_name);
        // maybe create a new file
        const char *data = "";
        if(write_file(file_name, data, BINARY) != 0) {
                fprintf(stderr, "ERROR: Failed to write to new scene file to prepare for scene serialization.\n");
                return -1;
                }
    }
    
    // open file
    FILE *file = fopen(file_name, "wb");
    if (file == NULL) {
        fprintf(stderr, "ERROR: failed to open file for writing\n");
        return -1;
    }

    // Serialize Scene header
    fprintf(file, "[Scene]\n");
    fprintf(file, "Name: %s\n", scene->name);
    int32_t entity_count = scene->entity_count;
    fprintf(file, "EntityCount: %"PRId32"\n", entity_count);
    fprintf(file, "[Entities]\n");
    
    // Serialize each Entity
    for (int i = 0; i < entity_count; i++) {
        char *entity_buffer = entity_serialize(scene->entities[i]);
        if (entity_buffer == NULL) {
            fprintf(stderr, "Failed to serialize entity.\n");
            fclose(file);
            return -1;
        }

        // Write entity buffer size to file as int32_t
        int32_t entity_buffer_size = strlen(entity_buffer);
        fwrite(&entity_buffer_size, sizeof(int32_t), 1, file);
        // Write entity buffer to file
        fwrite(entity_buffer, sizeof(char), entity_buffer_size, file);
        // Free entity buffer memory
        free(entity_buffer);

    }
    // close off the file
    fprintf(file, "[/Entities]\n");
    fprintf(file, "[/Scene]\n");
    fclose(file);
    return 0;
}



// Function to deserialize scene from a binary file
Scene *scene_manager_deserialize_scene(const char *file_name) {
    if (file_name == NULL) {
        fprintf(stderr, "ERROR: Invalid file name.\n");
        return NULL;
    }
    
    
    size_t buffer_size;
    char *buffer = scene_manager_read_scene_file(file_name, &buffer_size);
    

    /*
    if (scene_manager_validate_scene_file(buffer, &buffer_size) != 0) {
        fprintf(stderr, "ERROR: Invalid 'scene script' file structure, cannot deserialize '%s'.\n", file_name);
        return NULL;
    }    
    */
    

    Token **tokens = lexer_extract_tokens(buffer);
    if (tokens == NULL) {
        fprintf(stderr, "ERROR: Failed to create tokens from scene file.\n");
        return NULL;
    }
        

    // Get token count to allow 'tokens' to be freed from memory later
    int token_count = lexer_get_token_count(tokens);


    // create scene from the parsed tokens
    Scene *scene = parse_scene(tokens);


    // free the tokens from memory
    lexer_free_tokens(tokens, token_count);


    // scene built and ready to be returned
    return scene;
}



      
char *scene_manager_read_scene_file(const char* file_name, size_t *buffer_size) {
    if (file_name == NULL) {
        fprintf(stderr, "ERROR: Invalid 'file_name', cannot be NULL.\n");
        return NULL;
    }
    
    if (check_if_file_exists(file_name)) {
        printf("File '%s' found, attempting to read the Scene file.\n", file_name);
    }
    else {
        fprintf(stderr, "ERROR: Scene file '%s' not found, aborting operation.\n", file_name);
        return NULL;
    }

    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Cannot open '%s' for reading.\n", file_name);
        fclose(file);
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* scene_buffer = (char*)malloc(file_size + 1);
    if(scene_buffer == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'scene_buffer' during 'scene_manager_read_scene_file'.\n");
        fclose(file);
        return NULL;
    }

    // Read the file into the scene buffer
    if (fread(scene_buffer,1, file_size, file) != file_size) {
        fprintf(stderr, "ERROR: Failed to read file '%s' into the 'scene_buffer'.\n", file_name);
        fclose(file);
        return NULL;
    } 
    scene_buffer[file_size] = '\0';
    fclose(file);
    *buffer_size = file_size;
    return scene_buffer;

}


char * scene_manager_get_engine_script_tags() {
    const char* file_name = "../src/scene_script_tags.h";
    // open the scene_script_tags.h file for reading
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Failed to open '%s' for reading.\n", file_name);
        return NULL;
    }

    // Allocate memory for tag buffer
    size_t buffer_size = TAG_BUFFER_SIZE;
    char* tag_buffer = (char*)malloc(buffer_size * sizeof(char));
    if (tag_buffer == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'tag_buffer'.\n");
        fclose(file);
        return NULL;
    }

    // Initialize tag_buffer as an empty string.
    tag_buffer[0] = '\0';

    char line[TAG_BUFFER_SIZE];

    // Read the contents of the file and search for tags
    while (fgets(line, TAG_BUFFER_SIZE, file) != NULL) {

        // check if the line contains a tag definition
        if (strstr(line, "#define") != NULL){
            // Extract the tag value between double quotes
            char *quote_start = strchr(line, '"');
            char *quote_end = strrchr(line, '"');
            if (quote_start != NULL && quote_end != NULL && quote_start != quote_end) {
                *quote_end = '\0'; // Terminate the string at the end quote
                // Concatenate the tag value to tag_buffer
                if (strlen(tag_buffer) + strlen(quote_start + 1) + 1 > buffer_size) {
                    buffer_size *= 2; // Double the buffer size
                    tag_buffer = realloc(tag_buffer, buffer_size * sizeof(char));
                    if (tag_buffer == NULL) {
                        fprintf(stderr, "ERROR: Failed to reallocate memory for 'tag_buffer'.\n");
                        fclose(file);
                        return NULL;
                    }
                }
                strcat(tag_buffer, quote_start + 1);
                strcat(tag_buffer, " "); // Add a space delimiter between tags
            }    
        }
    }
    // close the file
    fclose(file);

    return tag_buffer;

}


void scene_manager_print_engine_script_tags() {
    char *tag_buffer = scene_manager_get_engine_script_tags();
    if (tag_buffer == NULL) {
        fprintf(stderr, "ERROR: Failed to retrieve scene script tags.\n");
        return;
    }
    char *ptr = tag_buffer;
    while (*ptr != '\0') {
        printf("%c", *ptr);
        ptr++;
    }
    printf("\n");

    free(tag_buffer);
}


int scene_manager_validate_scene_file(char *buffer, size_t *buffer_size) {
    if (buffer == NULL) {
        fprintf(stderr, "ERROR: Cannot validate scene file with a NULL 'buffer'.\n");
        return -1;
    }
    
    char *tag_list = scene_manager_get_engine_script_tags();
    if (tag_list == NULL) {
        fprintf(stderr, "ERROR: Failed to retrieve 'tag_list'.\n");
        return -1;
    }

    char *property_list = scene_manager_get_scene_properties();
    if (property_list == NULL) {
        fprintf(stderr, "ERROR: Failed to retrieve 'property_list'.\n");
        return -1;
    }

    // Combine the lists
    // +2 for space delimiter and null terminator
    char *combined_list = (char*)malloc(strlen(tag_list) + strlen(property_list) + 2);
    if (combined_list == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for 'combined_list'.\n");
        free(tag_list);
        free(property_list);
        return -1;
    }
    
    strcpy(combined_list, tag_list);
    strcat(combined_list, " ");
    strcat(combined_list, property_list);

    printf("Combined list: %s\n", combined_list);

    // Split the tag_list into indvidual tags and properties
    char *token = strtok(combined_list, " ");

    
    while (token != NULL) {        
        // Go through the buffer and search for each tag or property
        char* ptr = buffer;        
        while((ptr = strstr(ptr, token)) != NULL) {
            printf("Found: '%s' in scene file.\n", token);
            // Move to the next character after the token
            ptr += strlen(token);
        }
        // Get the next tag or property
        token = strtok(NULL, " ");
        
    }
    // free the memory
 while (token != NULL) {        
        // Go through the buffer and search for each tag or property
        char* ptr = buffer;        
        while((ptr = strstr(ptr, token)) != NULL) {
            printf("Found: '%s' in scene file.\n", token);
            // Move to the next character after the token
            ptr += strlen(token);
        }
        // Get the next tag or property
        token = strtok(NULL, " ");
        
    }
    free(tag_list);
    free(property_list);
    free(combined_list);
    
    return 0;
}

char* scene_manager_get_scene_properties(){

    const char* file_name = "../src/scene_properties.h";
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Failed to open file '%s' for reading.\n", file_name);
        return NULL;
    }

    // Allocate memory for the property_buffer
    size_t buffer_size = PROPERTY_BUFFER_SIZE;
    char *property_buffer = (char*)malloc(buffer_size * sizeof(char));
    if (property_buffer == NULL) {
        fprintf(stderr,"ERROR: Failed to allocate memory for the 'property_buffer'.\n");
        fclose(file);
        return NULL;
    }

    // Initialize the property buffer as an empty string
    property_buffer[0] = '\0';

    char line[PROPERTY_BUFFER_SIZE];

    // Read the contents of the file and search for the properties
    while (fgets(line, PROPERTY_BUFFER_SIZE, file) != NULL) {

        // Check if the line contains a property definition
        if(strstr(line, "#define") != NULL) {
            
            // Extract the property value between double quotes
            char *quote_start = strchr(line, '"');
            if (quote_start != NULL) {
                char *quote_end = strchr(quote_start + 1, '"');

                if (quote_end != NULL) {
                    // Terminate the string at the end quote
                    *quote_end = '\0';
                    
                    // prepare the property buffer for concatenation
                    size_t new_size = strlen(property_buffer) + strlen(quote_end + 1) + 1;
                    if (new_size > buffer_size) {

                        // Double the buffer size
                        buffer_size *= 2;
                        property_buffer = realloc(property_buffer, buffer_size * sizeof(char));

                        // NULL check after attempting reallocation
                        if (property_buffer == NULL) {
                            fprintf(stderr, "ERROR: Failed to reallocate memory for 'property_buffer'.\n");
                            fclose(file);
                            return NULL;
                        }
                    }
                    // Concatentate the property value to the property buffer
                    strcat(property_buffer, quote_start + 1);
                    // Add a space delimiter
                    strcat(property_buffer, " ");
                }
            }
        }
    }

    // Close the file
    fclose(file);
    return property_buffer;

}


void scene_manager_print_scene_properties() {
    char *property_buffer = scene_manager_get_scene_properties();
    if (property_buffer == NULL) {
        fprintf(stderr, "ERROR: Failed to retrieve scene properties.\n");
        return;
    }

    char *token = strtok(property_buffer, " ");
    while (token != NULL) {
        printf("%s, ", token);
        token = strtok(NULL, " ");
    }
    printf("\n");
    free(property_buffer);
}
