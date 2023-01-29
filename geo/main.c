#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "geo_hash.h"
#include "cJSON.h"
#include "geo_country.h"
#include "math_lib.h"

void geo_encode(uint8_t *geo_hash, uint32_t len)
{
    char *ret = is_geo_hash_in_country(geo_hash, len);
    if (ret != NULL)
    {
        print_colomn(geo_hash, len, NO_ENTER_C);
        printf("\t\t\t\t\t%s\n", ret);
        return;
    }

    for (uint32_t i = 0; i < 32; i++)
    {
        uint8_t *geo_hash_new = malloc(len + 1);
        if (geo_hash_new == NULL)
        {
            goto error1;
        }
        memcpy(geo_hash_new, geo_hash, len);
        geo_hash_new[len] = get_base32(i);
        geo_encode(geo_hash_new, len + 1);
        free(geo_hash_new);
    }

    return;
error1:
    printf("malloc error\n");
    return;
}

int main()
{
    uint8_t buf[100];
    parse_geo_country();


    // gps_t gps_test;
    // gps_test.lat = 39.923201;
    // gps_test.lon = 116.390705;

    // geo_hash(gps_test, buf, 10);

    // gps_t gps;
    // gps.lat = 39.9257460000;
    // gps.lon = 116.5998310000;
    // geo_hash(gps, buf, 8);

    // test();
    uint8_t test;
    geo_encode(&test, 0);


    return 0;
}
