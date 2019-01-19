#include <stdio.h>
#include <json.h>
#include <string.h>
#include "json_parser.h"

int _json_parser_mqtt_conf_fill(json_object *jobj, Mqtt_Conf *mqtt);
int _json_parser_ttn_conf_fill(json_object *jobj, Ttn_Conf *ttn);

int
parse_configuration(char *path, Mqtt_Conf *mqtt, Ttn_Conf *ttn)
{
    json_object *jobj, *mqtt_obj, *ttn_obj;
    json_bool ret;
    const char *json_util_last_error = NULL;
    int ret_val;

    jobj = json_object_from_file(path);
    json_util_last_error = json_util_get_last_err();
    if (json_util_last_error)
    {
        printf("Last error %s", json_util_last_error);
        exit(EXIT_FAILURE);
    }

    ret = json_object_object_get_ex(jobj, "Mqtt", &mqtt_obj);
    if (!ret) {
        printf("Error. Mqtt object was not found in configuration\n");
        return -1;
    }

    if (json_object_get_type(mqtt_obj) != json_type_object) {
        printf("Error\n");
        return -1;
    }

    ret_val = _json_parser_mqtt_conf_fill(mqtt_obj, mqtt);
    if (ret_val)
        return -1;

    ret = json_object_object_get_ex(jobj, "Ttn", &ttn_obj);
    if (!ret) {
        printf("Error. Ttn object was not found in configuration\n");
        return -1;
    }

    if (json_object_get_type(ttn_obj) != json_type_object) {
        printf("Error\n");
        return -1;
    }
    ret_val = _json_parser_ttn_conf_fill(ttn_obj, ttn);
    if (ret_val)
        return -1;

    printf("---\n%s\n---\n",
            json_object_to_json_string_ext(mqtt_obj, JSON_C_TO_STRING_SPACED |
                JSON_C_TO_STRING_PRETTY));

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

int
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

int
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
