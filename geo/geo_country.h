#ifndef __GEO_COUNTRY__
#define __GEO_COUNTRY__

#include "geo_hash.h"
#include "math_lib.h"
typedef struct
{
    uint32_t gps_num;
    gps_t   *gps;
} PACKED region_t;

typedef struct
{
    char     *country_code;
    uint32_t region_num;
    region_t *region;
} PACKED country_geo_t;

void parse_geo_country();
country_geo_t *get_parse_geo();
char *is_point_in_country(gps_t point);
char *is_geo_hash_in_country(uint8_t *geo_hash, uint32_t len);
// void test();

#endif
