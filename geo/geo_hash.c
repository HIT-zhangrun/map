#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "geo_hash.h"

#define MAX_ACC 100

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
        case NO_ENTER_C:
        {
            printf("%c", buf[i]);
            break;
        }
        default:
            break;
        }
    }
    if (print_type == NO_ENTER_C)
    {
        return;
    }
    printf("\n");
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

static uint8_t base32[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'j', 'k', 'm', 'n', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

uint8_t get_base32(uint32_t index)
{
    return base32[index];
}

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
    printf("geo acc: %d, lat bit: %d, lon bit: %d \n", acc, geo_hash_lat_bit, geo_hash_lon_bit);

    uint8_t lat_bin[100];
    uint8_t lon_bin[100];
    half_code(-90.0f, 90.0f, gps.lat, geo_hash_lat_bit, lat_bin);
    half_code(-180.0f, 180.0f, gps.lon, geo_hash_lon_bit, lon_bin);

    uint8_t lat_lon_bin[200];
    lat_lon_bin_code(lat_bin, lon_bin, geo_hash_lat_bit, geo_hash_lon_bit, lat_lon_bin);

    printf("[geo hash bin]: \n");
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

    memcpy(geo_hash, geo_hash_encode, acc);
}

uint8_t base32_hash_1[4][8] =
{
    {'b', 'c', 'f', 'g', 'u', 'v', 'y', 'z'},
    {'8', '9', 'd', 'e', 's', 't', 'w', 'x'},
    {'2', '3', '6', '7', 'k', 'm', 'q', 'r'},
    {'0', '1', '4', '5', 'h', 'j', 'n', 'p'}
};

uint8_t base32_hash_2[8][4] =
{
    {'p', 'r', 'x', 'z'},
    {'n', 'q', 'w', 'y'},
    {'j', 'm', 't', 'v'},
    {'h', 'k', 's', 'u'},
    {'5', '7', 'e', 'g'},
    {'4', '6', 'd', 'f'},
    {'1', '3', '9', 'c'},
    {'0', '2', '8', 'b'}
};

uint32_t find_char_in_colomn(uint8_t tar, uint8_t *buf, uint32_t len)
{
    uint32_t i = 0;

    for (i = 0; i < len; i++)
    {
        if (buf[i] == tar)
        {
            break;
        }
    }

    return i + 1;
}

void geo_hash_gps(uint8_t *geo_hash, uint32_t len, gps_t *gps)
{
    double lat_l = -90.0f;
    double lat_r = 90.0f;
    double lon_l = -180.0f;
    double lon_r = 180.0f;

    for (uint32_t i = 0; i < len; i++)
    {
        uint32_t index = 0;
        uint32_t x, y = 0;
        if (i % 2 == 0)//实际对应geo hash奇数位
        {
            index = find_char_in_colomn(geo_hash[i], (uint8_t *)base32_hash_1, sizeof(base32_hash_1));
            y = index / 8;//行数
            x = index % 8;
            if (x != 0)
            {
                y++;
            }
            else
            {
                x += 8;
            }

            double lat_dis = lat_r - lat_l;
            double lat_index = lat_dis / 4;
            lat_r = lat_r - lat_index * (y - 1);
            lat_l = lat_r - lat_index;

            double lon_dis = lon_r - lon_l;
            double lon_index = lon_dis / 8;
            lon_l = lon_l + lon_index * (x - 1);
            lon_r = lon_l + lon_index;

            continue;
        }
        if (i % 2 == 1)//实际对应geo hash偶数位
        {
            index = find_char_in_colomn(geo_hash[i], (uint8_t *)base32_hash_2, sizeof(base32_hash_2));
            y = index / 4;//行数
            x = index % 4;
            if (x != 0)
            {
                y++;
            }
            else
            {
                x += 4;
            }

            double lat_dis = lat_r - lat_l;
            double lat_index = lat_dis / 8;
            lat_r = lat_r - lat_index * (y - 1);
            lat_l = lat_r - lat_index;

            double lon_dis = lon_r - lon_l;
            double lon_index = lon_dis / 4;
            lon_l = lon_l + lon_index * (x - 1);
            lon_r = lon_l + lon_index;

            continue;
        }
    }

    gps[0].lat = lat_r;
    gps[0].lon = lon_l;

    gps[1].lat = lat_r;
    gps[1].lon = lon_r;

    gps[2].lat = lat_l;
    gps[2].lon = lon_r;

    gps[3].lat = lat_l;
    gps[3].lon = lon_l;

    // printf("%0.10lf, %0.10lf\n", gps[0].lat, gps[0].lon);
    // printf("%0.10lf, %0.10lf\n", gps[1].lat, gps[1].lon);
    // printf("%0.10lf, %0.10lf\n", gps[2].lat, gps[2].lon);
    // printf("%0.10lf, %0.10lf\n", gps[3].lat, gps[3].lon);
}

// void test()
// {
//     // uint8_t test_geo[] = "wx4g0ec1eb";
//     // gps_t gps[4];

//     // geo_hash_gps(test_geo, 8, gps);

//     // printf("%0.10lf, %0.10lf\n", gps[0].lat, gps[0].lon);
//     // printf("%0.10lf, %0.10lf\n", gps[1].lat, gps[1].lon);
//     // printf("%0.10lf, %0.10lf\n", gps[2].lat, gps[2].lon);
//     // printf("%0.10lf, %0.10lf\n", gps[3].lat, gps[3].lon);
// }
