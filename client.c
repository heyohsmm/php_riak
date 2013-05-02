/*
   Copyright 2012 Trifork A/S
   Author: Kaspar Pedersen

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include <php.h>
#include "client.h"
#include "exceptions.h"

zend_class_entry *riak_client_ce;

static zend_function_entry riak_client_methods[] = {
  PHP_ME(RiakClient, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
  {NULL, NULL, NULL}
};

void riak_client_init(TSRMLS_D) 
{
  zend_class_entry ce;
 
  INIT_CLASS_ENTRY(ce, "RiakClient", riak_client_methods);
  ce.create_object = create_client_data;
  riak_client_ce = zend_register_internal_class(&ce TSRMLS_CC);
  
  zend_declare_property_stringl(riak_client_ce, "host", sizeof("host")-1, DEFAULT_HOST, sizeof(DEFAULT_HOST)-1, ZEND_ACC_PUBLIC TSRMLS_CC);
  zend_declare_property_long(riak_client_ce, "port", sizeof("port")-1, DEFAULT_PORT, ZEND_ACC_PUBLIC TSRMLS_CC);
}

zend_object_value create_client_data(zend_class_entry *class_type TSRMLS_DC) 
{
  zend_object_value retval;
  client_data *tobj;
 
  tobj = emalloc(sizeof(client_data));
  memset(tobj, 0, sizeof(client_data));

  zend_object_std_init((zend_object *) &tobj->std, class_type TSRMLS_CC);
  object_properties_init((zend_object*) &tobj->std, class_type);
 
  retval.handle = zend_objects_store_put(tobj, (zend_objects_store_dtor_t) zend_objects_destroy_object, 
    (zend_objects_free_object_storage_t) free_client_data, NULL TSRMLS_CC);
  retval.handlers = zend_get_std_object_handlers();
  return retval;
}

void free_client_data(void *object TSRMLS_DC)
{
  client_data* data = (client_data*)object;
  zend_object_std_dtor(&data->std TSRMLS_CC);
  riack_free(data->client);
  efree(data);
}

/////////////////////////////////////////////////////////////

PHP_METHOD(RiakClient, __construct)
{
  int connResult;
  client_data *data;
  char *host, *szHost;
  int hostLen;
  long port = DEFAULT_PORT;
  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|l", &host, &hostLen, &port) == FAILURE) {
    return;
  }
  zend_update_property_stringl(riak_client_ce, getThis(), "host", sizeof("host")-1, host, hostLen TSRMLS_CC);
  zend_update_property_long(riak_client_ce, getThis(), "port", sizeof("port")-1, port TSRMLS_CC);
 
  data = (client_data*)zend_object_store_get_object(getThis() TSRMLS_CC);
  // TODO use allocater that uses PHP memory functions
  data->client = riack_new_client(&riack_default_allocator);

  szHost = estrndup(host, hostLen);
  connResult = riack_connect(data->client, szHost, port, NULL);
  efree(szHost);
  if (connResult != RIACK_SUCCESS) {
    zend_throw_exception(riak_connection_exception_ce, "Connection error", 1000 TSRMLS_CC);
  }
}

