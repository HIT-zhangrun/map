#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "geo_hash.h"
#include "cJSON.h"
#include "geo_country.h"

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
    printf("country num = %d\n", size);

    country_geo_t country_geo[255];

    for (uint32_t i = 0; i < size; i++)
    {
        pTemp = cJSON_GetArrayItem(country_p, i);

        country_code_p = cJSON_GetObjectItem(pTemp, "properties");

        country_code_p = cJSON_GetObjectItem(country_code_p, "ADMIN");

        printf("%d country code %s\n", i, country_code_p->valuestring);

        country_geo[i].country_code = (char *)malloc(sizeof(country_code_p->valuestring) + 1);
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

            pTemp = pTemp->child->child;
            // gps point
            // printf("%.20lf ,", pTemp->valuedouble);
            // printf("%.20lf\n", pTemp->next->valuedouble);
        }
    }

    cJSON_Delete(pJson);
    pJson = NULL;

    return;

error:
    printf("parse error\n");
    return;
}