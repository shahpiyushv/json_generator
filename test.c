/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>

#include <json_generator.h>

void print_buf(char *buf, void *priv)
{
	printf("%s", buf);
}
/* Creating JSON
{
    "first_bool": true,
    "first_int": 30,
    "float_val": 54.1643,
    "my_str": "new_name",
    "null_obj": null,
    "arr": [
            ["arr_string", false, 45.2, null, 25, {
             "arr_obj_str": "sample"
             }]
            ],
    "my_obj": {
        "only_val": 5
    }
}
*/
int main(int argc, char **argv)
{
	char buf[20];
	printf("Creating JSON string [may require Line wrap enabled on console]\r\n");
	json_str_t jstr;
	json_str_start(&jstr, buf, sizeof(buf), print_buf, NULL);
	json_start_object(&jstr);
	json_obj_set_bool(&jstr, "first_bool", true);
	json_obj_set_int(&jstr, "first_int", 30);
	json_obj_set_float(&jstr, "float_val", 54.1643);
	json_obj_set_string(&jstr, "my_str", "new_name");
	json_obj_set_null(&jstr, "null_obj");
	json_push_array(&jstr, "arr");
	json_start_array(&jstr);
	json_arr_set_string(&jstr, "arr_string");
	json_arr_set_bool(&jstr, false);
	json_arr_set_float(&jstr, 45.12);
	json_arr_set_null(&jstr);
	json_arr_set_int(&jstr, 25);
	json_start_object(&jstr);
	json_obj_set_string(&jstr, "arr_obj_str", "sample");
	json_end_object(&jstr);
	json_end_array(&jstr);
	json_pop_array(&jstr);
	json_push_object(&jstr, "my_obj");
	json_obj_set_int(&jstr, "only_val", 5);
	json_pop_object(&jstr);
	json_end_object(&jstr);
	json_str_end(&jstr);
	printf("\r\n");
	return 0;

}
