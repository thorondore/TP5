#include "imageFormationUtils.h"
#include <stdio.h>
#include <stdlib.h>

int x_size = 1920; /* pixels/meter */
int y_size = 1080; /* pixels/meter */

struct point2d {
   float  x;
   float  y;
   int red;
   int green;
   int blue;
};

typedef unsigned char color;


/*---------------------------------------------------------------------------*/
/*                   functions for reading a ppm file                        */
/*---------------------------------------------------------------------------*/

void pm_erreur(char *texte) {
    fprintf(stderr, "\n%s \n\n", texte);
    exit(1);
}

char pm_getc(FILE* file) {
    int value_of_current_char;
    char current_char;

    value_of_current_char = getc(file);
    if (value_of_current_char == EOF) {
        pm_erreur("EOF / read error" );
    }
    current_char = (char) value_of_current_char;

    if (current_char == '#') {
        do {
            value_of_current_char = getc(file);
            if (value_of_current_char == EOF) {
                pm_erreur("EOF / read error");
            }
            current_char = (char) value_of_current_char;
        } while (current_char != '\n' && current_char != '\r');
    }
    return current_char;
}

color pm_getrawbyte(FILE* file) {
    int value_of_byte;

    value_of_byte = getc( file );
    if ( value_of_byte == EOF ) {
        pm_erreur("EOF / read error " );
    }
    return (color) value_of_byte;
}

int pm_getint(FILE* file) {
    char current_char;
    int number;
    do {
        current_char = pm_getc(file);
    } while (current_char == ' ' || current_char == '\t' || current_char == '\n' || current_char == '\r');

    if (current_char < '0' || current_char > '9') {
        pm_erreur("read error : != [0-9]");
    }

    number = 0;
    do {
        number = number * 10 + current_char - '0';
        current_char = pm_getc(file);
    } while (current_char >= '0' && current_char <= '9');

    return number;
}

/*---------------------------------------------------------------------------*/
/*                              functioooooooons                             */
/*---------------------------------------------------------------------------*/



struct point2d perspective_projection(float f, struct point3d coordinate, float alpha, float u0, float v0){

    struct point2d new_point;
    new_point.x = coordinate.x/(1.0+(coordinate.z/f));
    new_point.y = coordinate.y/(1.0+(coordinate.z/f));
    new_point.red = coordinate.r;
    new_point.green = coordinate.g;
    new_point.blue = coordinate.b;

    new_point.x = ((new_point.x)/alpha) + u0;
    new_point.y = ((new_point.y)/alpha) + v0;

    return new_point;
}


int main(int argc, char* argv[]) {
    float f;

    float u0 = x_size/2.0;
    float v0 = y_size/2.0;
    float alpha = 1.0/x_size;

    printf("Select the desired focal value :\n");
    scanf("%f", &f);

    struct point3d *points;
    int N_v = 0;
    points = readOff(argv[1], &N_v);

    color* final_map = calloc(1920 * 1080 * 3, sizeof(color));
    struct point2d point;
    for (int i = 0; i < N_v; i++) {
        point = perspective_projection(f, points[i], alpha, u0, v0);
        if (point.x < 0 || point.x >= x_size || point.y < 0 || point.y >= y_size) {
            continue;
        }
        int coord = (((int) point.y * 1920) + (int)(point.x)) * 3;
        final_map[coord] = point.red;
        // printf("%d\n", final_map[coord]);
        final_map[coord + 1] = point.green;
        final_map[coord + 2] = point.blue;
    }

    /* Opening output file */
    FILE* file_output = fopen(argv[2],"w");
    if (file_output == NULL) {
        printf("error in opening file %s\n", argv[2]);
        exit(1);
    }

    /* Write file header */
    fprintf(file_output, "%s\n", "P3");
    fprintf(file_output, "%d %d\n", 1920, 1080);
    fprintf(file_output, "%d\n", 255);


    /* We write to the output file */
    for (long int i = 0; i < 1920 * 1080 * 3; i++) {
        fprintf(file_output, "%d ", final_map[i]);
    }

    fclose(file_output);

    // centerThePCL(points, N_v);

    return EXIT_SUCCESS;
} 


