#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <render.h>
#include <obj_loader.h>

void
parse_vertex(const char * str,
             Point3d * v);

void
parse_face(char * str,
           Point3d v[],
           void (* face_handler)(Queue * vertexes, void * args),
           void * args);

void
load_obj(const char * filename,
         void (* face_handler)(Queue * vertexes, void * args),
         void * args) {
    
    Point3d vertexes[10000];
    int vertexes_cnt = 0;
    
    FILE * fp = fopen(filename, "r");
    
    char * line = NULL;
    size_t len = 0;
    ssize_t read = 0;
    
    while ((read = getline(&line, &len, fp)) > 0) {
        
        if((line[0] != 'v') && (line[0] != 'f'))
            continue;
        
        if((line[0] == 'v') && (line[1] == ' '))
            parse_vertex(&line[2], &vertexes[vertexes_cnt++]);
        
        if((line[0] == 'f') && (line[1] == ' '))
            parse_face(&line[2], vertexes, face_handler, args);
    }
    
    if (line)
        free(line);
}

void
parse_vertex(const char * str,
             Point3d * v) {
    sscanf(str, "%lf %lf %lf", &v->y, &v->x, &v->z);
}

void
parse_face(char * str,
           Point3d v[],
           void (* face_handler)(Queue * vertexes, void * args),
           void * args) {
    
    Queue * tokens = new_queue();
    
    char * token = NULL;
    token = strtok(str, " \n");
    while(token) {
        add(token, tokens);
        token = strtok(NULL, " \n");
    }
    
    Queue * vertexes = new_queue();
    
    int vertex_index = 0;
    char * token2 = NULL;
    while(!is_empty(tokens)) {
        token = (char *) get(tokens);
        
        token2 = strtok(token, "/ ");
        vertex_index = atoi(token2);
        
        add(&v[vertex_index - 1], vertexes);
    }
    
    face_handler(vertexes, args);
    
    release_queue(tokens);
    release_queue(vertexes);
}