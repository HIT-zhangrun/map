#ifndef __GEO_HASH__
#define __GEO_HASH__

#ifndef PACKED
#define PACKED __attribute__((packed))
#endif

typedef struct
{
    double lat;//纬度
    double lon;//经度
} PACKED gps_t;

uint8_t get_base32(uint32_t index);

void geo_hash(gps_t gps, uint8_t *geo_hash, uint32_t acc);
void geo_hash_gps(uint8_t *geo_hash, uint32_t len, gps_t *gps);

// void test();

#endif
