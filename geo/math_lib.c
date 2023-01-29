#include "math_lib.h"

double cal_polygon_area(gps_t *points, uint32_t num)
{
    if (num < 3)
    {
        return 0.0;
    }

    double s = points[0].lat * (points[num - 1].lon - points[1].lon);
    for(int i = 1; i < num; ++i)
    {
        s += points[i].lat * (points[i - 1].lon - points[(i + 1) % num].lon);
    }

    return fabs(s / 2.0);
}

// 交换精度坐标系
void reverse_gps(gps_t *points, uint32_t num)
{
    for (uint32_t i = 0; i < num; i++)
    {
        if (points[i].lon < 0)
        {
            points[i].lon = (-180.0f) - points[i].lon;
        }
        else
        {
            points[i].lon = (180.0f) - points[i].lon;
        }
    }
}

// 0: 不需要交换 1: 需要交换经度坐标系
uint8_t is_need_reverse(gps_t *gps, uint32_t num)
{
    gps_t *points = (gps_t *)malloc(sizeof(gps_t) * num);
    memcpy(points, gps, sizeof(gps_t) * num);
    double s1, s2;
    s1= cal_polygon_area(points, num);
    reverse_gps(points, num);
    s2 = cal_polygon_area(points, num);

    free(points);

    if (s1 > s2)
    {
        return 1;
    }

    return 0;
}

uint32_t point_in_polygon(uint32_t num, gps_t *points, gps_t gps)
{
    gps_t *polygon = NULL;
    while (polygon == NULL)
    {
        polygon = (gps_t *)malloc(sizeof(gps_t) * num);
    }

    memcpy(polygon, points, sizeof(gps_t) * num);
    gps_t point;
    memcpy(&point, &gps, sizeof(gps_t));

    if (is_need_reverse(polygon, num))
    {
        reverse_gps(polygon, num);
        reverse_gps(&point, 1);
    }

    // 上边沿重合时没有算入内部
    uint32_t i, j, c = 0;
    for (i = 0, j = num - 1; i < num; j = i++) {
        if ( ((polygon[i].lat > point.lat) != (polygon[j].lat > point.lat)) &&
        (point.lon < (polygon[j].lon - polygon[i].lon) * (point.lat - polygon[i].lat) / (polygon[j].lat - polygon[i].lat) + polygon[i].lon) )
        {
            c = !c;
        }
    }

    free(polygon);


    return c;
error:
    return 0;
}

// float test_x[] = {-170.0, 170.0, 170.0, -170.0};
// float test_y[] = {20.0, 20.0, 0.0, 0.0};

// void test()
// {
//     gps_t gps_test_[10];
//     gps_test_[0].lat = 20.0;
//     gps_test_[0].lon = -170.0;
//     gps_test_[1].lat = 20.0;
//     gps_test_[1].lon = 170.0;
//     gps_test_[2].lat = 0.0;
//     gps_test_[2].lon = 170.0;
//     gps_test_[3].lat = 0.0;
//     gps_test_[3].lon = -170.0;

//     gps_t gps_test_1;
//     gps_test_1.lat = 19.9;
//     gps_test_1.lon = -170.1;

//     uint32_t ret = point_in_polygon(4, gps_test_, gps_test_1);
//     printf("%d\n", ret);
// }
