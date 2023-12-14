#pragma once
#include <cute.h>

const char *cf_json_get_string(CF_JVal val, const char *name);
int cf_json_get_int(CF_JVal val, const char *name);
float cf_json_get_float(CF_JVal val, const char *name);
bool cf_json_get_bool(CF_JVal val, const char *name);