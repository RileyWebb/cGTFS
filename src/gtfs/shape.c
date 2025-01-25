#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gtfs.h"

gtfs_point_t gtfs__parse_point(gtfs_shape_t *shape, char *line)
{
    gtfs_point_t point;

    char *token = strtok(line, ",");

    while( *token != NULL ) {
        printf( " %s\n", token ); //printing each token
        //token = strtok(NULL, " ");
    }

    return point;
}

const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ",");
         tok && *tok;
         tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

void *gtfs_parse_shapes(gtfs_schedule_t *schedule, char *buffer)
{
    if (schedule->shapes) free(schedule->shapes);
    schedule->num_shapes = 0;

    gtfs_shape_t shapes[20000] = {0}; 

    {
        char *str = strdup(buffer);
        char *line = strtok(str, "\n");
        while (line != NULL)
        {
            while(*line != '\"')
                line++;

            char *start = ++line;
            size_t len = 0;
            while(*line != '\"')
            {
                line++;
                len++;
            }

            char *id = malloc(len + 1);
            strncpy(id, start, len);
            id[len] = '\0';

            int found = 0;
            for (size_t i = 0; i < schedule->num_shapes; ++i)
            {
                if (!strcmp(shapes[i].shape_id, id))
                {
                    shapes[i].num_points++;
                    found = 1;
                    break;
                }
            }

            if (found == 0)
            {
                shapes[schedule->num_shapes].shape_id = strdup(id);
                schedule->num_shapes++;
            }

            free(id);

            //printf("%s\n", id );

            line = strtok(NULL, "\n");
        }

        free(str);
    }

    schedule->shapes = malloc(sizeof(gtfs_shape_t) * schedule->num_shapes);

    for (size_t i = 0; i < schedule->num_shapes; ++i)
    {
        schedule->shapes[i].shape_id = shapes[i].shape_id;
        schedule->shapes[i].points = malloc(sizeof(gtfs_point_t) * shapes[i].num_points);
        schedule->shapes[i].num_points = shapes[i].num_points;
    }

    {
        char *str = strdup(buffer);
        char *line = strtok(str, "\n");
        line = strtok(NULL, "\n");
        while (line != NULL)
        {
            while(*line != '\"')
                line++;

            char *start = ++line;
            size_t len = 0;
            while(*line != '\"')
            {
                line++;
                len++;
            }

            char *id = malloc(len + 1);
            strncpy(id, start, len);
            id[len] = '\0';

            float lat, lon;
            int index;
            float distance;
            sscanf(line, "\",\"%f\",\"%f\",\"%i\",\"%f\"", &lat, &lon, &index, &distance);
            index--;
            for (size_t i = 0; i < schedule->num_shapes; ++i)
            {
                if (!strcmp(schedule->shapes[i].shape_id, id))
                {
                    schedule->shapes[i].points[index].lat = lat;
                    schedule->shapes[i].points[index].lon = lon;
                    break;
                }
            }

            free(id);

            line = strtok(NULL, "\n");
        }

        free(str);
    }

    for (size_t i = 0; i < schedule->num_shapes; ++i)
    {
        //if (schedule->shapes[i].shape_id)
            //printf("%s\n", schedule->shapes[i].shape_id);
    }

    //return schedule;
}