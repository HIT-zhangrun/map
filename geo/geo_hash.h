#ifndef __GEO_HASH__
#define __GEO_HASH__

typedef struct
{
    double lat;//纬度
    double lon;//经度
} gps_t;

void geo_hash(gps_t gps, uint8_t *geo_hash, uint32_t acc);
void geo_hash_gps(uint8_t *geo_hash, uint32_t len, gps_t *gps);

void test();

#endif
