#include <stdio.h>
#include <json.h>
#include <string.h>
#include "json_parser.h"

static int _json_parser_process_config_file(json_object **jobj, const char *path);
static int _json_parser_mqtt_conf_fill(json_object *jobj, Mqtt_Conf *mqtt);
static int _json_parser_ttn_conf_fill(json_object *jobj, Ttn_Conf *ttn);
static int _json_parser_generic_conf(json_object *jobj, void **struct_to_fill,
                                     const char *obj_name);
static int _json_parser_print_mqtt_conf(Mqtt_Conf *mqtt);
static int _json_parser_print_ttn_conf(Ttn_Conf *ttn);

int
parse_configuration(const char *path, Mqtt_Conf *mqtt, Ttn_Conf *ttn)
{
    json_object *jobj;
    int ret;

    ret = _json_parser_process_config_file(&jobj, path);
    if (ret)
        return ret;

    ret = _json_parser_generic_conf(jobj, (void *)mqtt, "Mqtt");
    if (ret)
        return ret;

    ret = _json_parser_generic_conf(jobj, (void *)ttn, "Ttn");
    if (ret)
        return ret;

    printf("Parsed configuration:\n");
    _json_parser_print_mqtt_conf(mqtt);
    _json_parser_print_ttn_conf(ttn);

    return 0;
}

static int
_json_parser_process_config_file(json_object **jobj, const char *path)
{
    const char *json_util_last_error = NULL;

    *jobj = json_object_from_file(path);
    json_util_last_error = json_util_get_last_err();
    if (json_util_last_error)
    {
        printf("Error config file: %s", json_util_last_error);
        return -1;
    }

    return 0;
}

static int
_json_parser_generic_conf(json_object *jobj, void **struct_to_fill,
                          const char *obj_name)
{
    int ret_val;
    json_bool ret;
    json_object *sub_jobj;

    ret = json_object_object_get_ex(jobj, obj_name, &sub_jobj);
    if (!ret) {
        printf("Error. %s object was not found in configuration\n", obj_name);
        return -1;
    }

    if (json_object_get_type(sub_jobj) != json_type_object) {
        printf("Error\n");
        return -1;
    }

    printf("%s object\n---\n%s\n---\n", obj_name,
            json_object_to_json_string_ext(sub_jobj, JSON_C_TO_STRING_SPACED |
                JSON_C_TO_STRING_PRETTY));

    if (!strncmp("Mqtt", obj_name, sizeof(*obj_name)))
    {
        ret_val = _json_parser_mqtt_conf_fill(sub_jobj, (Mqtt_Conf *)struct_to_fill);
    }
    else if (!strncmp("Ttn", obj_name, sizeof(*obj_name)))
    {
        ret_val = _json_parser_ttn_conf_fill(sub_jobj, (Ttn_Conf *)struct_to_fill);
    }
    else
    {
        printf("Error. %s subobject is wrong. SHOULD never happen\n", obj_name);
        return -1;
    }

    return ret_val;
}

static int
_json_parser_print_mqtt_conf(Mqtt_Conf *mqtt)
{
    if (!mqtt)
    {
        printf("Error printing Mqtt conf, parameter is NULL\n");
        return -1;
    }

    printf("%s\n", mqtt->host);
    printf("%d\n", mqtt->port);
    printf("%s\n", mqtt->client_id);
    printf("%s\n", mqtt->topic_prefix);
    printf("%s\n", mqtt->topic_suffix);
    printf("%d\n", mqtt->qos);
    printf("%d\n", mqtt->cleansess);
    printf("%d\n", mqtt->reconnect_delay);

    return 0;
}

static int
_json_parser_print_ttn_conf(Ttn_Conf *ttn)
{
    if (!ttn)
    {
        printf("Error printing Ttn conf, parameter is NULL\n");
        return -1;
    }

    printf("%s\n", ttn->host);
    printf("%d\n", ttn->port);
    printf("%s\n", ttn->app_id);
    printf("%s\n", ttn->app_key);
    printf("%s\n", ttn->region_handler);
    printf("%s\n", ttn->topic);
    printf("%s\n", ttn->client_id);

    return 0;
}


static int
_json_parser_mqtt_conf_fill(json_object *jobj, Mqtt_Conf *mqtt)
{
    JSON_PARSER_CONF_FILL_STRING(jobj, mqtt->host, "Host");
    JSON_PARSER_CONF_FILL_INT(jobj, mqtt->port, "Port");
    JSON_PARSER_CONF_FILL_STRING(jobj, mqtt->client_id, "ClientID");
    JSON_PARSER_CONF_FILL_STRING(jobj, mqtt->topic_prefix, "TopicPrefix");
    JSON_PARSER_CONF_FILL_STRING(jobj, mqtt->topic_suffix, "TopicSuffix");
    JSON_PARSER_CONF_FILL_INT(jobj, mqtt->qos, "Qos");
    JSON_PARSER_CONF_FILL_INT(jobj, mqtt->cleansess, "Cleansess");
    JSON_PARSER_CONF_FILL_INT(jobj, mqtt->reconnect_delay, "ReconnectDelay");

    return 0;
}

static int
_json_parser_ttn_conf_fill(json_object *jobj, Ttn_Conf *ttn)
{
    JSON_PARSER_CONF_FILL_STRING(jobj, ttn->host, "Host");
    JSON_PARSER_CONF_FILL_INT(jobj, ttn->port, "Port");
    JSON_PARSER_CONF_FILL_STRING(jobj, ttn->app_id, "AppID");
    JSON_PARSER_CONF_FILL_STRING(jobj, ttn->app_key, "AppKey");
    JSON_PARSER_CONF_FILL_STRING(jobj, ttn->region_handler, "RegionalHandler");
    JSON_PARSER_CONF_FILL_STRING(jobj, ttn->topic, "Topic");
    JSON_PARSER_CONF_FILL_STRING(jobj, ttn->client_id, "ClientID");

    return 0;
}
