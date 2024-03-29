#ifndef __MATH_LIB__
#define __MATH_LIB__
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <math.h>

#ifndef PACKED
#define PACKED __attribute__((packed))
#endif

typedef struct
{
    double lat;//纬度
    double lon;//经度
} PACKED gps_t;

typedef struct
{
    gps_t p1;
    gps_t p2;
} PACKED line_t;

typedef struct
{
    double x;
    double y;
} PACKED vector_t;

uint32_t point_in_polygon(uint32_t num, gps_t *points, gps_t gps);
double multiply_2(vector_t v1, vector_t v2);
vector_t point_to_vector(gps_t p_1, gps_t p_2);
vector_t line_to_vector(line_t line);
line_t point_to_line(gps_t gps_1, gps_t gps_2);
uint8_t is_line_segment_cross(line_t l_1, line_t l_2);

// void test();

#endif
