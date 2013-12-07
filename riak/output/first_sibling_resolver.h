/*
   Copyright 2013: Kaspar Bach Pedersen

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

#ifndef RIAK_OUTPUT__FIRST_SIBLING_RESOLVER__H__
#define RIAK_OUTPUT__FIRST_SIBLING_RESOLVER__H__

#include "php_riak_internal.h"

PHP_METHOD(Riak_Output_FirstSiblingResolver, resolve);

extern zend_class_entry *riak_output_first_sibling_resolver;

void riak_output_first_sibling_resolver_init(TSRMLS_D);

#endif //RIAK_OUTPUT__FIRST_SIBLING_RESOLVER__H__
