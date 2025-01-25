#include <stdint.h>

typedef struct gtfs_point_s
{
    float lat, lon;
} gtfs_point_t;

typedef struct gtfs_shape_s
{
    wchar_t *shape_id;

    gtfs_point_t *points;
    size_t num_points;
} gtfs_shape_t;

typedef struct gtfs_schedule_s
{
    gtfs_shape_t *shapes;
    size_t num_shapes;
} gtfs_schedule_t;

void *gtfs_parse_shapes(gtfs_schedule_t *schedule, char *buffer);

gtfs_schedule_t *gtfs_load_schedule(wchar_t *uri_string);