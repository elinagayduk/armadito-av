#ifndef _UHURU_JSON_
#define _UHURU_JSON_

#include "linkhash.h"
#include "json_util.h"

void print_json_value(json_object *jobj);
void json_parse_array(json_object *jobj, char *key);
void json_parse_and_print(json_object * jobj);
const char* json_parse_and_process(json_object * jobj);
const char* json_get_protocol_err_msg(const char* err_msg);
const char* json_get_basic_scan_response(int scan_id);
json_object * create_json_obj();

#endif