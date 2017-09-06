/** \file json_generator.h
 * \brief JSON String Generator
 *
 * This module can be used to create JSON strings with a facility
 * to flush out data if the destination buffer is full. All commas
 * and colons as required are automatically added by the APIs
 */
#ifndef _JSON_GENERATOR_H
#define _JSON_GENERATOR_H

#include <stdint.h>
#include <stdbool.h>

/** JSON string flush callback prototype
 *
 * This is a prototype of the function that needs to be passed to
 * json_str_start() and which will be invoked by the JSON generator
 * module either when the buffer is full or json_str_end() ins invoked.
 *
 * \param[in] buf Pointer to a NULL terminated JSON string
 * \param[in] priv Private data to be passed to the flush callback. Will
 * be the same as the one passed to json_str_start()
 */
typedef void (*json_flush_cb_t) (char *buf, void *priv);

/** JSON String structure
 *
 * Please do not set/modify any elements.
 * Just define this structure and pass a pointer to it in the APIs below
 */
typedef struct {
	char *buf;
	int buf_size;
	json_flush_cb_t flush_cb;
	void *priv;
	bool comma_req;
	char *free_ptr;
} json_str_t;

/** Start a JSON String
 *
 * This is the first function to be called for creating a JSON string.
 * It initializes the internal data structures. After the JSON string
 * generation is over, the json_str_end() function should be called.
 *
 * \param[out] jstr Pointer to an allocated \ref json_str_t structure.
 * This will be initialised internally and needs to be passed to all
 * subsequent function calls
 * \param[out] buf Pointer to an allocated buffer into which the JSON
 * string will be written
 * \param[in] buf_size Size of the buffer
 * \param[in] Pointer to the flushing function of type \ref json_flush_cb_t
 * which will be invoked either when the buffer is full or when json_str_end()
 * is invoked. Can be left NULL.
 * \param[in] priv Private data to be passed to the flushing function callback.
 * Can be something like a session identifier (Eg. socket). Can be left NULL
 */
void json_str_start(json_str_t *jstr, char *buf, int buf_size,
		json_flush_cb_t flush_cb, void *priv);

/** End JSON string
 *
 * This should be the last function to be called after the entire JSON string
 * has been generated.
 *
 * \param[in] jstr Pointer to the \ref json_str_t structure initilised by
 * json_str_start()
 */
void json_str_end(json_str_t *jstr);

/** Start a JSON object
 *
 * This starts a JSON object by adding a '{'
 *
 * \param[in] jstr Pointer to the \ref json_str_t structure initilised by
 * json_str_start()
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_start_object(json_str_t *jstr);

/** End a JSON object
 *
 * This ends a JSON object by adding a '}'
 *
 * \param[in] jstr Pointer to the \ref json_str_t structure initilised by
 * json_str_start()
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_end_object(json_str_t *jstr);

/** Start a JSON array
 *
 * This starts a JSON object by adding a '['
 *
 * \param[in] jstr Pointer to the \ref json_str_t structure initilised by
 * json_str_start()
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_start_array(json_str_t *jstr);

/** End a JSON object
 *
 * This ends a JSON object by adding a ']'
 *
 * \param[in] jstr Pointer to the \ref json_str_t structure initilised by
 * json_str_start()
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_end_array(json_str_t *jstr);

/** Push a named JSON object
 *
 * This adds a JSON object like "name":{
 *
 * \param[in] jstr Pointer to the \ref json_str_t structure initilised by
 * json_str_start()
 * \param[in] name Name of the object
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_push_object(json_str_t *jstr, char *name);

/** Pop a named JSON object
 *
 * This ends a JSON object by adding a '}'. This is basically same as
 * json_end_object() but included so as to complement json_push_object()
 *
 * \param[in] jstr Pointer to the \ref json_str_t structure initilised by
 * json_str_start()
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_pop_object(json_str_t *jstr);

/** Push a named JSON array
 *
 * This adds a JSON array like "name":[
 *
 * \param[in] jstr Pointer to the \ref json_str_t structure initilised by
 * json_str_start()
 * \param[in] name Name of the array
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_push_array(json_str_t *jstr, char *name);

/** Pop a named JSON array
 *
 * This ends a JSON array by adding a ']'. This is basically same as
 * json_end_array() but included so as to complement json_push_array()
 *
 * \param[in] jstr Pointer to the \ref json_str_t structure initilised by
 * json_str_start()
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_pop_array(json_str_t *jstr);

/** Add a boolean element to an object
 *
 * This adds a boolean element to an object. Eg. "bool_val":true
 *
 * \note This must be called between json_start_object()/json_push_object()
 * and json_end_object()/json_pop_object()
 *
 * \param[in] jstr Pointer to the \ref json_str_t structure initilised by
 * json_str_start()
 * \param[im] name Name of the element
 * \param[in] val Boolean value of the element
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_obj_set_bool(json_str_t *jstr, char *name, bool val);

/** Add an integer element to an object
 *
 * This adds an integer element to an object. Eg. "int_val":28
 *
 * \note This must be called between json_start_object()/json_push_object()
 * and json_end_object()/json_pop_object()
 *
 * \param[in] jstr Pointer to the \ref json_str_t structure initilised by
 * json_str_start()
 * \param[im] name Name of the element
 * \param[in] val Integer value of the element
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_obj_set_int(json_str_t *jstr, char *name, int val);

/** Add a float element to an object
 *
 * This adds a float element to an object. Eg. "float_val":23.8
 *
 * \note This must be called between json_start_object()/json_push_object()
 * and json_end_object()/json_pop_object()
 *
 * \param[in] jstr Pointer to the \ref json_str_t structure initilised by
 * json_str_start()
 * \param[im] name Name of the element
 * \param[in] val Float value of the element
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_obj_set_float(json_str_t *jstr, char *name, float val);

/** Add a string element to an object
 *
 * This adds a string element to an object. Eg. "string_val":"my_string"
 *
 * \note This must be called between json_start_object()/json_push_object()
 * and json_end_object()/json_pop_object()
 *
 * \param[in] jstr Pointer to the \ref json_str_t structure initilised by
 * json_str_start()
 * \param[im] name Name of the element
 * \param[in] val Null terminated string value of the element
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_obj_set_string(json_str_t *jstr, char *name, char *val);

/** Add a NULL element to an object
 *
 * This adds a NULL element to an object. Eg. "null_val":null
 *
 * \note This must be called between json_start_object()/json_push_object()
 * and json_end_object()/json_pop_object()
 *
 * \param[in] jstr Pointer to the \ref json_str_t structure initilised by
 * json_str_start()
 * \param[im] name Name of the element
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_obj_set_null(json_str_t *jstr, char *name);

/** Add a boolean element to an array
 *
 * \note This must be called between json_start_array()/json_push_array()
 * and json_end_array()/json_pop_array()
 *
 * \param[in] jstr Pointer to the \ref json_str_t structure initilised by
 * json_str_start()
 * \param[in] val Boolean value of the element
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_arr_set_bool(json_str_t *jstr, bool val);

/** Add an integer element to an array
 *
 * \note This must be called between json_start_array()/json_push_array()
 * and json_end_array()/json_pop_array()
 *
 * \param[in] jstr Pointer to the \ref json_str_t structure initilised by
 * json_str_start()
 * \param[in] val Integer value of the element
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_arr_set_int(json_str_t *jstr, int val);

/** Add a float element to an array
 *
 * \note This must be called between json_start_array()/json_push_array()
 * and json_end_array()/json_pop_array()
 *
 * \param[in] jstr Pointer to the \ref json_str_t structure initilised by
 * json_str_start()
 * \param[in] val Float value of the element
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_arr_set_float(json_str_t *jstr, float val);

/** Add a string element to an array
 *
 * \note This must be called between json_start_array()/json_push_array()
 * and json_end_array()/json_pop_array()
 *
 * \param[in] jstr Pointer to the \ref json_str_t structure initilised by
 * json_str_start()
 * \param[in] val Null terminated string value of the element
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_arr_set_string(json_str_t *jstr, char *val);

/** Add a NULL element to an array
 *
 * \note This must be called between json_start_array()/json_push_array()
 * and json_end_array()/json_pop_array()
 *
 * \param[in] jstr Pointer to the \ref json_str_t structure initilised by
 * json_str_start()
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_arr_set_null(json_str_t *jstr);

#endif
