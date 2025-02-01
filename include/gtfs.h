#include <stdint.h>

typedef struct gtfs_point_s
{
    double lat, lon, dist;
} gtfs_point_t;

typedef struct gtfs_shape_s
{
    wchar_t *shape_id;

    gtfs_point_t *points;
    size_t num_points;
} gtfs_shape_t;

typedef struct gtfs_stop_s
{
    wchar_t *id;
    wchar_t *name;
    double lat, lon;
} gtfs_stop_t;

typedef struct gtfs_schedule_s
{
    gtfs_shape_t *shapes;
    size_t num_shapes;

    gtfs_stop_t *stops;
    size_t num_stops;
} gtfs_schedule_t;

int gtfs_parse_shapes(gtfs_schedule_t *schedule, wchar_t *buffer);
int gtfs_parse_stops(gtfs_schedule_t *schedule, wchar_t *buffer);
gtfs_schedule_t *gtfs_load_schedule(wchar_t *uri_string);