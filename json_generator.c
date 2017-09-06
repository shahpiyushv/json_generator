#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <json_generator.h>

#define MAX_INT_IN_STR  	24
#define MAX_FLOAT_IN_STR 	30

static inline int json_get_empty_len(json_str_t *jstr)
{
	return (jstr->buf_size - (jstr->free_ptr - jstr->buf) - 1);
}
static int json_add_to_str(json_str_t *jstr, char *str)
{
	int len = strlen(str);
	char *cur_ptr = str;
	while (1) {
		int len_remaining = json_get_empty_len(jstr);
		int copy_len = len_remaining > len ? len : len_remaining;
		memmove(jstr->free_ptr, cur_ptr, copy_len);
		cur_ptr += copy_len;
		jstr->free_ptr += copy_len;
		len -= copy_len;
		if (len) {
			*jstr->free_ptr = '\0';
			if (!jstr->flush_cb) {
				printf("Cannot flush. End of string");
				return -1;
			}
			jstr->flush_cb(jstr->buf, jstr->priv);
			jstr->free_ptr = jstr->buf;
		} else
			break;
	}
	return 0;
}


void json_str_start(json_str_t *jstr, char *buf, int buf_size,
		json_flush_cb_t flush_cb, void *priv)
{
	memset(jstr, 0, sizeof(json_str_t));
	jstr->buf = buf;
	jstr->buf_size = buf_size;
	jstr->flush_cb = flush_cb;
	jstr->free_ptr = buf;
	jstr->priv = priv;
}

void json_str_end(json_str_t *jstr)
{
	*jstr->free_ptr = '\0';
	if (jstr->flush_cb)
		jstr->flush_cb(jstr->buf, jstr->priv);
	memset(jstr, 0, sizeof(json_str_t));
}

static inline void json_handle_comma(json_str_t *jstr)
{
	if (jstr->comma_req)
		json_add_to_str(jstr, ",");
}


static int json_handle_name(json_str_t *jstr, char *name)
{
	json_add_to_str(jstr, "\"");
	json_add_to_str(jstr, name);
	return json_add_to_str(jstr, "\":");
}


int json_start_object(json_str_t *jstr)
{
	json_handle_comma(jstr);
	jstr->comma_req = false;
	return json_add_to_str(jstr, "{");
}

int json_end_object(json_str_t *jstr)
{
	jstr->comma_req = true;
	return json_add_to_str(jstr, "}");
}


int json_start_array(json_str_t *jstr)
{
	json_handle_comma(jstr);
	jstr->comma_req = false;
	return json_add_to_str(jstr, "[");
}

int json_end_array(json_str_t *jstr)
{
	jstr->comma_req = true;
	return json_add_to_str(jstr, "]");
}

int json_push_object(json_str_t *jstr, char *name)
{
	json_handle_comma(jstr);
	json_handle_name(jstr, name);
	jstr->comma_req = false;
	return json_add_to_str(jstr, "{");
}
int json_pop_object(json_str_t *jstr)
{
	jstr->comma_req = true;
	return json_add_to_str(jstr, "}");
}
int json_push_array(json_str_t *jstr, char *name)
{
	json_handle_comma(jstr);
	json_handle_name(jstr, name);
	jstr->comma_req = false;
	return json_add_to_str(jstr, "[");
}
int json_pop_array(json_str_t *jstr)
{
	jstr->comma_req = true;
	return json_add_to_str(jstr, "]");
}

static int json_set_bool(json_str_t *jstr, bool val)
{
	jstr->comma_req = true;
	if (val)
		return json_add_to_str(jstr, "true");
	else
		return json_add_to_str(jstr, "false");
}
int json_obj_set_bool(json_str_t *jstr, char *name, bool val)
{
	json_handle_comma(jstr);
	json_handle_name(jstr, name);
	return json_set_bool(jstr, val);
}

int json_arr_set_bool(json_str_t *jstr, bool val)
{
	json_handle_comma(jstr);
	return json_set_bool(jstr, val);
}

static int json_set_int(json_str_t *jstr, int val)
{
	jstr->comma_req = true;
	char str[MAX_INT_IN_STR];
	snprintf(str, MAX_INT_IN_STR, "%d", val);
	return json_add_to_str(jstr, str);
}

int json_obj_set_int(json_str_t *jstr, char *name, int val)
{
	json_handle_comma(jstr);
	json_handle_name(jstr, name);
	return json_set_int(jstr, val);
}

int json_arr_set_int(json_str_t *jstr, int val)
{
	json_handle_comma(jstr);
	return json_set_int(jstr, val);
}


static int json_set_float(json_str_t *jstr, float val)
{
	jstr->comma_req = true;
	char str[MAX_FLOAT_IN_STR];
	snprintf(str, MAX_FLOAT_IN_STR, "%f", val);
	return json_add_to_str(jstr, str);
}
int json_obj_set_float(json_str_t *jstr, char *name, float val)
{
	json_handle_comma(jstr);
	json_handle_name(jstr, name);
	return json_set_float(jstr, val);
}
int json_arr_set_float(json_str_t *jstr, float val)
{
	json_handle_comma(jstr);
	return json_set_float(jstr, val);
}

static int json_set_string(json_str_t *jstr, char *val)
{
	jstr->comma_req = true;
	json_add_to_str(jstr, "\"");
	json_add_to_str(jstr, val);
	return json_add_to_str(jstr, "\"");
}

int json_obj_set_string(json_str_t *jstr, char *name, char *val)
{
	json_handle_comma(jstr);
	json_handle_name(jstr, name);
	return json_set_string(jstr, val);
}

int json_arr_set_string(json_str_t *jstr, char *val)
{
	json_handle_comma(jstr);
	return json_set_string(jstr, val);
}


static int json_set_null(json_str_t *jstr)
{
	jstr->comma_req = true;
	return json_add_to_str(jstr, "null");
}
int json_obj_set_null(json_str_t *jstr, char *name)
{
	json_handle_comma(jstr);
	json_handle_name(jstr, name);
	return json_set_null(jstr);
}

int json_arr_set_null(json_str_t *jstr)
{
	json_handle_comma(jstr);
	return json_set_null(jstr);
}
