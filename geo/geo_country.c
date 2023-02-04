#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "cJSON.h"
#include "geo_country.h"

static country_geo_t country_geo[255] = {0};

size_t get_file_size(const char *filepath)
{
    /*check input para*/
    if (NULL == filepath)
    {
        printf("get file size null\n");
        return 0;
    }

    struct stat filestat = {0};

    /*get file information*/
    if (0 == stat(filepath,&filestat))
    {
        return filestat.st_size;
    }
    else
    {
        return 0;
    }
}

char *read_file_to_buf(const char *filepath)
{
    /*check input para*/
    if(NULL == filepath)
    {
        printf("file path null");
        return NULL;
    }
    /*get file size*/
    size_t size = get_file_size(filepath);
    if(0 == size)
    {
        printf("get file size null\n");
        return NULL;
    }

    /*malloc memory*/
    char *buf = malloc(size+1);
    if(NULL == buf)
    {
        printf("malloc fali\n");
        return NULL;
    }

    memset(buf, 0, size+1);

    /*read string from file*/
    FILE *fp = fopen(filepath,"rb");
    size_t readSize = fread(buf, 1, size, fp);

    if(readSize != size)
    {
        /*read error*/
        free(buf);
        printf("read size error\n");
        buf = NULL;
        return NULL;
    }

    buf[size] = 0;
    return buf;
}

cJSON *prepare_parse_json(const char *filePath)
{
    /*check input para*/
    if(NULL == filePath)
    {
        printf("input para is NULL\n");
        return NULL;
    }
    /*read file content to buffer*/
    char *buf = read_file_to_buf(filePath);
    if(NULL == buf)
    {
        printf("read file to buf failed\n");
        return NULL;
    }
    /*parse JSON*/
    cJSON *pTemp = cJSON_Parse(buf);
    free(buf);
    buf = NULL;
    return pTemp;
}

void parse_geo_country()
{
    char *filename = "../geo-countries/data/countries.geojson";
    cJSON *pJson = NULL;
    cJSON *pTemp = NULL;
    cJSON *country_p = NULL;
    cJSON *country_code_p = NULL;
    cJSON *country_gps_p = NULL;

    pJson = prepare_parse_json(filename);
    if(NULL == pJson)
    {
        printf("parse json failed\n");
        goto error;
    }

    country_p = cJSON_GetObjectItem(pJson, "features");
    int32_t size = cJSON_GetArraySize(country_p);
    // printf("country num = %d\n", size);

    for (uint32_t i = 0; i < size; i++)
    {
        pTemp = cJSON_GetArrayItem(country_p, i);

        country_code_p = cJSON_GetObjectItem(pTemp, "properties");

        country_code_p = cJSON_GetObjectItem(country_code_p, "ADMIN");

        printf("%d country code %s\n", i, country_code_p->valuestring);

        country_code_p = cJSON_GetObjectItem(pTemp, "properties");
        country_code_p = cJSON_GetObjectItem(country_code_p, "ISO_A3");

        country_geo[i].country_code = (char *)malloc(4);
        if (country_geo[i].country_code == NULL)
        {
            printf("NULL\n");
            goto error;
        }
        strcpy(country_geo[i].country_code, country_code_p->valuestring);

        country_gps_p = cJSON_GetObjectItem(pTemp, "geometry");
        country_gps_p = cJSON_GetObjectItem(country_gps_p, "coordinates");

        int32_t gps_size = cJSON_GetArraySize(country_gps_p);
        // printf("size %d\n", gps_size);
        country_geo[i].region_num = gps_size;

        country_geo[i].region = (region_t *)malloc(sizeof(region_t) * gps_size);
        if (country_geo[i].region == NULL)
        {
            printf("NULL\n");
            goto error;
        }

        for (uint32_t j = 0; j < gps_size; j++)
        {
            pTemp = cJSON_GetArrayItem(country_gps_p, j);
            pTemp = pTemp->child;

            int32_t gps_point_num = cJSON_GetArraySize(pTemp);
            // printf("gps point num %d\n", gps_point_num);

            country_geo[i].region[j].gps_num = gps_point_num;

            country_geo[i].region[j].gps = (gps_t *)malloc(sizeof(gps_t) * gps_point_num);
            if(country_geo[i].region[j].gps == NULL)
            {
                goto error;
            }

            cJSON *gps_point = pTemp->child;
            cJSON *gps_point_lat_lon = gps_point->child;

            for (uint32_t k = 0; k < gps_point_num; k++)
            {
                country_geo[i].region[j].gps[k].lon = gps_point_lat_lon->valuedouble;
                country_geo[i].region[j].gps[k].lat = gps_point_lat_lon->next->valuedouble;
                gps_point = gps_point->next;
                if (gps_point == NULL)
                {
                    break;
                }
                gps_point_lat_lon = gps_point->child;
            }
            // // gps point 0
            // printf("%.20lf ,", gps_point_lat_lon->valuedouble);
            // printf("%.20lf\n", gps_point_lat_lon->next->valuedouble);
            // // gps point 1
            // gps_point = gps_point->next;
            // gps_point_lat_lon = gps_point->child;
            // printf("%.20lf ,", gps_point_lat_lon->valuedouble);
            // printf("%.20lf\n", gps_point_lat_lon->next->valuedouble);
        }
    }

    cJSON_Delete(pJson);
    pJson = NULL;

    return;

error:
    printf("parse error\n");
    return;
}

country_geo_t *get_parse_geo()
{
    return country_geo;
}

char *is_point_in_country(gps_t point)
{
    country_geo_t *geo = get_parse_geo();

    for (uint32_t i = 0; i < 254; i++)
    {
        uint32_t region_num = get_parse_geo()[i].region_num;
        for (uint32_t j = 0; j < region_num; j++)
        {
            uint32_t gps_num = get_parse_geo()[i].region[j].gps_num;
            gps_t *gps = get_parse_geo()[i].region[j].gps;

            uint32_t ret = point_in_polygon(gps_num, gps, point);

            if (ret)
            {
                return geo[i].country_code;
            }
        }
    }

    return NULL;
}

// char *is_country_in_region(gps_t *gps, uint32_t num)
// {

//     return NULL;
// }

char *is_geo_hash_in_country(uint8_t *geo_hash, uint32_t len)
{
    if (geo_hash == NULL || len == 0)
    {
        return NULL;
    }

    gps_t gps[4];

    geo_hash_gps(geo_hash, len, gps);

    char *point1 = is_point_in_country(gps[0]);

    char *point2 = is_point_in_country(gps[1]);

    char *point3 = is_point_in_country(gps[2]);

    char *point4 = is_point_in_country(gps[3]);

    if (point1 == NULL && point2 == NULL && point3 == NULL && point4 == NULL)
    {
        char *no_country = "NONE";
        return no_country;
    }

    if (point1 == NULL || point2 == NULL || point3 == NULL || point4 == NULL)
    {
        return NULL;
    }

    if (0 == strcmp(point1, point2) && 0 == strcmp(point3, point4) && 0 == strcmp(point1, point3))
    {
        return point1;
    }

    return NULL;
}

// void test()
// {
//     uint8_t geo_hash[] = "wx4gjk32";
//     for (uint32_t i = 0; i < 8; i++)
//     {
//         char *test = is_geo_hash_in_country(geo_hash, i);
//         if (test != NULL)
//         {
//             printf("find it %d in %s\n", i, test);
//         }
//     }
// }
