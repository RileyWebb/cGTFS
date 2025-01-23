#include <stdint.h>

typedef struct gtfs_point_s
{
    float lat, lon;
} gtfs_point_t;

typedef struct gtfs_shape_s
{
    char *shape_id;

    gtfs_point_t *points;
    size_t num_points;
} gtfs_shape_t;

typedef struct gtfs_schedule_s
{
    gtfs_shape_t *shapes;
    size_t num_shapes;
} gtfs_schedule_t;

void *gtfs_parse_shapes(gtfs_schedule_t *schedule, char *str);