#ifndef JSON_PARSER_H_
#define JSON_PARSER_H_

#define JSON_PARSER_CONF_FILL_STRING(jobj, member, member_name) \
    do \
    { \
        json_object *val; \
        json_bool ret; \
        ret = json_object_object_get_ex(jobj, member_name, &val); \
        if (!ret) \
        { \
            printf("Error. \"%s\" was not found in configuration\n", member_name); \
            return -1; \
        } \
        if(json_object_get_type(val) != json_type_string) \
        { \
            printf("Error. incorrect type \"%s\" not a str\n", member_name); \
            return -1; \
        } \
        strncpy(member, json_object_get_string(val), json_object_get_string_len(val)); \
    } while(0);

#define JSON_PARSER_CONF_FILL_INT(jobj, member, member_name) \
    do \
    { \
        json_object *val; \
        json_bool ret; \
        ret = json_object_object_get_ex(jobj, member_name, &val); \
        if (!ret) \
        { \
            printf("Error. \"%s\" was not found in configuration\n", member_name); \
            return -1; \
        } \
        if(json_object_get_type(val) != json_type_int) \
        { \
            printf("Error. incorrect type \"%s\" not an int\n", member_name); \
            return -1; \
        } \
        member = json_object_get_int(val); \
    } while(0);

typedef struct ttn_conf_
{
    char   host[100];
    int    port;
    char   app_id[100];
    char   app_key[100];
    char   region_handler[100];
    char   topic[100];
    char   client_id[100];
} Ttn_Conf;

typedef struct mqtt_conf_
{
    char   host[100];
    int    port;
    char   client_id[100];
    char   topic_prefix[100];
    char   topic_suffix[100];
    int    qos;
    int    cleansess;
    int    reconnect_delay;
} Mqtt_Conf;

typedef struct conf_
{
    Ttn_Conf    ttn;
    Mqtt_Conf   mqtt;
} Conf;

int parse_configuration(char *path, Mqtt_Conf *mqtt, Ttn_Conf *ttn);

#endif

