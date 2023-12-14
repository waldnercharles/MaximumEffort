#include "json.h"

const char *cf_json_get_string(CF_JVal val, const char *name)
{
	return cf_json_get_string(cf_json_get(val, name));
}

int cf_json_get_int(CF_JVal val, const char *name)
{
	val = cf_json_get(val, name);
	return cf_json_is_int(val) ? cf_json_get_int(val) : cf_json_get_float(val);
}

float cf_json_get_float(CF_JVal val, const char *name)
{
	val = cf_json_get(val, name);
	return cf_json_is_float(val) ? cf_json_get_float(val)
								 : cf_json_get_int(val);
}

bool cf_json_get_bool(CF_JVal val, const char *name)
{
	val = cf_json_get(val, name);
	return cf_json_is_bool(val) ? cf_json_get_bool(val) : cf_json_get_int(val);
}
