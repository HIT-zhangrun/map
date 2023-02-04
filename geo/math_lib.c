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

// 计算两个向量的叉积 
double multiply_2(vector_t v1, vector_t v2)
{
    return v1.x * v2.y - v2.x * v1.y;
}

vector_t cal_2point_vector(gps_t p_1, gps_t p_2)
{
    vector_t vector;
    vector.x = p_2.lon - p_1.lon;
    vector.y = p_2.lat - p_1.lat;

    return vector;
}

// 判断两个线段是否相交
uint8_t is_line_segment_cross(line_t l_1, line_t l_2)
{
    gps_t a = l_1.p1;
    gps_t b = l_1.p2;
    gps_t c = l_2.p1;
    gps_t d = l_2.p2;

    vector_t ab = cal_2point_vector(a, b);
    vector_t ad = cal_2point_vector(a, d);
    vector_t ac = cal_2point_vector(a, c);

    vector_t cd = cal_2point_vector(c, d);
    vector_t ca = cal_2point_vector(c, a);
    vector_t cb = cal_2point_vector(c, b);

    double ret_1 = multiply_2(ab, ad) * multiply_2(ab, ac);
    double ret_2 = multiply_2(ab, ad) * multiply_2(ab, ac);

    return ((ret_1 * ret_2) <= 0);
}

void test()
{
    line_t line_1;
    line_t line_2;

    line_1.p1.lat = 0;  //y
    line_1.p1.lon = 0;

    line_1.p2.lat = 1;
    line_1.p2.lon = 0;

    line_2.p1.lat = 0;
    line_2.p1.lon = 1;

    line_2.p2.lat = 0.5;
    line_2.p2.lon = 1;

    printf("%d\n", is_line_segment_cross(line_1, line_2));
}
