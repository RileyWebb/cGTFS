#include <stdlib.h>
#include <string.h>

#include "debug.h"

#include "gtfs.h"

int gtfs_parse_stops(gtfs_schedule_t *schedule, wchar_t *buffer)
{
    if (!schedule) { DEBUG_ERROR("Schedule is uninitialized"); return -1; }

    if (schedule->stops) free(schedule->stops);
    schedule->num_stops = 0;

    wchar_t *context; // Context for wcstok_s
    wchar_t *token = wcstok_s(buffer, L",\n", &context); // Tokenize by comma and newline

    if (*token == L'\uFEFF')
        token++;

    wchar_t **cols[4];
    wchar_t *id = NULL, *name = NULL, *lat = NULL, *lon = NULL;

    for (int i = 0; i < 4; ++i)
    {
        if (!wcscmp(token, L"stop_id"))
        {
            cols[i] = &id;
        } else if (!wcscmp(token, L"stop_name"))
        {
            cols[i] = &name;
        } else if (!wcscmp(token, L"stop_lat"))
        {
            cols[i] = &lat;
        } else if (!wcscmp(token, L"stop_lon"))
        {
            cols[i] = &lon;
        } else { DEBUG_ERROR("Unable to parse stops file"); return -1; }

        if (i<3)
            token = wcstok_s(NULL, L",\n", &context);
    }

    gtfs_stop_t *current_stop;

    do
    {
        for (int i = 0; i < 4; ++i)
        {
            *cols[i] = wcstok_s(NULL, L",\n", &context);
            if (!*cols[i])
                goto done;//{ DEBUG_ERROR("Unable to parse point in shapes file"); return -1; }
        }

        schedule->num_stops++;
        // TODO: FIX THIS YOU LAZY SLOB
        schedule->stops = realloc(schedule->stops, sizeof(gtfs_stop_t) * schedule->num_stops);
        current_stop = &schedule->stops[schedule->num_stops - 1];
        memset(current_stop, 0, sizeof(gtfs_stop_t));

        if (id[wcslen(id) - 1] == '\"' && id[0] == '\"')
        {
            current_stop->id = calloc(wcslen(id) - 1, sizeof(wchar_t));
            wcsncat(current_stop->id, (wchar_t*)(id + 1), wcslen(id) - 2);
        }
        else
            current_stop->id = wcsdup(id);

        if (name[wcslen(name) - 1] == '\"' && name[0] == '\"')
        {
            current_stop->name = calloc(wcslen(name) - 1, sizeof(wchar_t));
            wcsncat(current_stop->name, (wchar_t*)(name + 1), wcslen(name) - 2);
        }
        else
            current_stop->name = wcsdup(name);

        wchar_t *lat_end = (wchar_t*)(lat + wcslen(lat) - 1);
        if (*lat_end == '\"' && lat[0] == '\"')
            current_stop->lat = wcstod(lat + 1, &lat_end - 1);
        else
            current_stop->lat = wcstod(lat, &lat_end);

        wchar_t *lon_end = (wchar_t*)(lon + wcslen(lon) - 1);
        if (*lon_end == '\"' && lon[0] == '\"')
            current_stop->lon = wcstod(lon + 1, &lon_end - 1);
        else
            current_stop->lon = wcstod(lon, &lon_end);
    } while (1);

    done:

    return 0;
}