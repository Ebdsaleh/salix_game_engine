#!/bin/bash
clear
gcc -g -Wall -Wextra -pedantic-errors main.c element.c scene.c entity.c transform.c primitives.c scene_manager.c file_operations.c vector.c element_counts.c lexer.c parser.c syntax_tree.c -o ../build/salix -lSDL2 -lSDL2_ttf -lm

if [ $? -eq 0 ]; then
    echo "Build successful. Executable created: salix"
    echo "program start..."
    cd ../build/
    ./salix
else
    echo "Build failed."
fi
