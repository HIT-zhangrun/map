#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "geo_hash.h"
#include "cJSON.h"
#include "geo_country.h"
#include "math_lib.h"

// void geo_encode(uint8_t *geo_hash, uint32_t len)
// {
//     if (geo_hash == NULL)
//     {
//         goto ret1;
//     }

//     gps_t gps[4];
//     geo_hash_gps(geo_hash, len, gps);
//     for (uint32_t j = 0; j < 255)
//     {

//     }
//     country_geo_t *get_parse_geo();


// ret1:
//     uint8_t *geo_hash_n = (uint8_t *)malloc(sizeof(len + 1));
//     if (geo_hash_n == NULL)
//     {
//         goto error1;
//     }
//     memcpy(geo_hash_n, geo_hash, len);
//     free(geo_hash);

//     for (uint32_t i = 0; i < 32; i++)
//     {
//         geo_hash_n[len] = get_base32(i);
//         geo_encode(geo_hash_n, len);
//     }

//     return;

// error1:
//     printf("malloc error\n");
//     return;
// }

int main()
{
    // uint8_t buf[100];
    parse_geo_country();


    // gps_t gps_test;
    // gps_test.lat = 39.923201;
    // gps_test.lon = 116.390705;

    // geo_hash(gps_test, buf, 10);

    // gps_t gps;
    // gps.lat = 39.9257460000;
    // gps.lon = 116.5998310000;
    // geo_hash(gps, buf, 8);

    test();

    // is_point_in_country(gps);

    return 0;
}
