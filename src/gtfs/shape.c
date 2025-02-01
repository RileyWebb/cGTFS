#include <stdlib.h>
#include <string.h>

#include "debug.h"

#include "gtfs.h"

int gtfs_parse_shapes(gtfs_schedule_t *schedule, wchar_t *buffer)
{
    if (!schedule) { DEBUG_ERROR("Schedule is uninitialized"); return -1; }

    if (schedule->shapes) free(schedule->shapes);
    schedule->num_shapes = 0;

    wchar_t *context; // Context for wcstok_s
    wchar_t *token = wcstok_s(buffer, L",\n", &context); // Tokenize by comma and newline

    if (*token == L'\uFEFF')
        token++;

    wchar_t **cols[5];
    wchar_t *id = NULL, *pt_lat = NULL, *pt_lon = NULL, *pt_seq = NULL, *pt_dist = NULL;

    for (int i = 0; i < 5; ++i)
    {
        if (!wcscmp(token, L"shape_id"))
        {
            cols[i] = &id;
        } else if (!wcscmp(token, L"shape_pt_lat"))
        {
            cols[i] = &pt_lat;
        } else if (!wcscmp(token, L"shape_pt_lon"))
        {
            cols[i] = &pt_lon;
        } else if (!wcscmp(token, L"shape_pt_sequence"))
        {
            cols[i] = &pt_seq;
        } else if (!wcscmp(token, L"shape_dist_traveled"))
        {
            cols[i] = &pt_dist;
        } else { DEBUG_ERROR("Unable to parse shapes file"); return -1; }

        if (i<4)
            token = wcstok_s(NULL, L",\n", &context);
    }

    wchar_t *last_id = NULL;
    gtfs_shape_t *current_shape = NULL;

    do
    {
        for (int i = 0; i < 5; ++i)
        {
            *cols[i] = wcstok_s(NULL, L",\n", &context);
            if (!*cols[i])
                goto done;//{ DEBUG_ERROR("Unable to parse point in shapes file"); return -1; }
        }

        if (last_id == NULL || wcscmp(id, last_id))
        {
            schedule->num_shapes++;
            // TODO: FIX THIS YOU LAZY SLOB
            schedule->shapes = realloc(schedule->shapes, sizeof(gtfs_shape_t) * schedule->num_shapes);
            current_shape = &schedule->shapes[schedule->num_shapes - 1];
            memset(current_shape, 0, sizeof(gtfs_shape_t));

            if (id[wcslen(id) - 1] == '\"' && id[0] == '\"')
            {
                current_shape->shape_id = calloc(wcslen(id) - 1, sizeof(wchar_t));
                wcsncat(current_shape->shape_id, (wchar_t*)(id + 1), wcslen(id) - 2);
            }
            else
                current_shape->shape_id = wcsdup(id);
        }

        current_shape->num_points++;

        // TODO: FIX THIS YOU LAZY SLOB
        current_shape->points = realloc(current_shape->points, sizeof(gtfs_point_t) * current_shape->num_points);

        gtfs_point_t *point = &current_shape->points[current_shape->num_points - 1];

        wchar_t *pt_lat_end = (wchar_t*)(pt_lat + wcslen(pt_lat) - 1);
        if (*pt_lat_end == '\"' && pt_lat[0] == '\"')
            point->lat = wcstod(pt_lat + 1, &pt_lat_end - 1);
        else
            point->lat = wcstod(pt_lat, &pt_lat_end);

        wchar_t *pt_lon_end = (wchar_t*)(pt_lon + wcslen(pt_lon) - 1);
        if (*pt_lon_end == '\"' && pt_lon[0] == '\"')
            point->lon = wcstod(pt_lon + 1, &pt_lon_end - 1);
        else
            point->lon = wcstod(pt_lon, &pt_lon_end);

        wchar_t *pt_dist_end = (wchar_t*)(pt_dist + wcslen(pt_dist) - 1);
        if (*pt_dist_end == '\"' && pt_dist[0] == '\"')
            point->dist = wcstod(pt_dist + 1, &pt_dist_end - 1);
        else
            point->dist = wcstod(pt_dist, &pt_dist_end);

        last_id = id;
    } while (1);

done:

    return 0;
}