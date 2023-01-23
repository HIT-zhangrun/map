#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "geo_hash.h"

#define MAX_ACC 100

typedef enum print_type
{
    D = 0,
    C,
} print_type_e;

void print_colomn(uint8_t *buf, uint32_t len, print_type_e print_type)
{
    for (uint32_t i = 0; i < len; i++)
    {
        switch (print_type)
        {
        case D:
        {
            printf("%d", buf[i]);
            break;
        }
        case C:
        {
            printf("%c", buf[i]);
            break;
        }
        default:
            break;
        }
    }
    printf("\r\n");
}

void half_code(double left_val, double right_val, double val, uint32_t bits, uint8_t *buf)
{
    uint8_t half_code_bin[200];
    double left = left_val;
    double right = right_val;
    for (uint32_t i = 0; i < bits; i++)
    {
        if (val - left <= (right - left) / 2)
        {
            half_code_bin[i] = 0;
            right = (right - left) / 2 + left;
        }
        else
        {
            half_code_bin[i] = 1;
            left = right - (right - left) / 2;
        }
    }

    memcpy(buf, half_code_bin, bits);
}

void lat_lon_bin_code(uint8_t *lat_bin, uint8_t *lon_bin, uint32_t lat_len, uint32_t lon_len, uint8_t *buf)
{
    for (uint32_t i = 0; i < lat_len; i++)
    {
        memcpy(buf + i * 2 + 1, lat_bin + i, sizeof(uint8_t));
    }

    for (uint32_t i = 0; i < lon_len; i++)
    {
        memcpy(buf + i * 2, lon_bin + i, sizeof(uint8_t));
    }
}

uint8_t base32[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'j', 'k', 'm', 'n', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

void base32_encode(uint8_t *lat_lon_bin, uint32_t len, uint8_t *buf)
{
    for (uint32_t i = 0; i < len / 5; i++)
    {
        uint32_t val = lat_lon_bin[i * 5] * 16 + lat_lon_bin[i * 5 + 1] * 8 + lat_lon_bin[i * 5 + 2] * 4 + lat_lon_bin[i * 5 + 3] * 2 + lat_lon_bin[i * 5 + 4];
        memcpy(buf + i, &base32[val], sizeof(uint8_t));
    }
}

void geo_hash(gps_t gps, uint8_t *geo_hash, uint32_t acc)
{
    uint32_t geo_hash_bit = acc * 5;
    uint32_t geo_hash_lat_bit;
    uint32_t geo_hash_lon_bit;

    geo_hash_lat_bit = geo_hash_bit / 2;
    geo_hash_lon_bit = geo_hash_bit / 2;
    if (geo_hash_bit % 2 != 0)
    {
        geo_hash_lon_bit = geo_hash_bit / 2 + 1;
    }
    printf("geo acc: %d, lat bit: %d, lon bit: %d", acc, geo_hash_lat_bit, geo_hash_lon_bit);

    uint8_t lat_bin[100];
    uint8_t lon_bin[100];
    half_code(-90.0f, 90.0f, gps.lat, geo_hash_lat_bit, lat_bin);
    half_code(-180.0f, 180.0f, gps.lon, geo_hash_lon_bit, lon_bin);

    uint8_t lat_lon_bin[200];
    lat_lon_bin_code(lat_bin, lon_bin, geo_hash_lat_bit, geo_hash_lon_bit, lat_lon_bin);

    printf("[geo hash bin]:\r\n");
    printf("lat:");
    print_colomn(lat_bin, geo_hash_lat_bit, D);
    printf("lon:");
    print_colomn(lon_bin, geo_hash_lon_bit, D);
    printf("lat lon bin:");
    print_colomn(lat_lon_bin, acc * 5, D);

    uint8_t geo_hash_encode[100];
    base32_encode(lat_lon_bin, acc * 5, geo_hash_encode);

    printf("geo hash:");
    print_colomn(geo_hash_encode, acc, C);


}

int main()
{
    gps_t gps_test;
    gps_test.lat = 39.923201;
    gps_test.lon = 116.390705;
    geo_hash(gps_test, NULL, 10);

    gps_t gps;
    gps.lat = 39.9257460000;
    gps.lon = 116.5998310000;
    geo_hash(gps, NULL, 8);
    return 0;
}