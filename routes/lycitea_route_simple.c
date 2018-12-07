/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Jinxi Wang <1054636713@qq.com>                                                             |
  +----------------------------------------------------------------------+
*/


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <zend_compile.h>
#include "php.h"
#include "main/SAPI.h" /* for sapi_header_line */
#include "ext/standard/php_string.h" /* for php_implode */
#include "ext/pcre/php_pcre.h"
#include "Zend/zend_interfaces.h"
#include "Zend/zend_smart_str.h"


#include "php_lycitea.h"
#include "lycitea_route_simple.h"
#include "helpers/lycitea_helpers_router.h"
#include "helpers/lycitea_helpers_common.h"
#include "lycitea_exception.h"

zend_class_entry *lycitea_route_simple_ce;

void addRoute(zval *, zval *, zval *, zend_long , zval *);
void getData(zval *, zval *);

ZEND_BEGIN_ARG_INFO_EX(lycitea_route_simple_construct_arginfo, 0, 0, 0)
ZEND_ARG_INFO(0, runmode)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(lycitea_route_simple_addroute_arginfo, 0, 0, 3)
ZEND_ARG_INFO(0, httpMethod)
ZEND_ARG_INFO(0, route)
ZEND_ARG_INFO(0, handler)
ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(lycitea_route_simple_addgroup_arginfo, 0, 0, 2)
ZEND_ARG_INFO(0, prefix)
ZEND_ARG_INFO(0, callback)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(lycitea_route_simple_gppdpho_arginfo, 0, 0, 2)
ZEND_ARG_INFO(0, route)
ZEND_ARG_INFO(0, handler)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(lycitea_route_simple_void_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(lycitea_route_simple_dispatch_arginfo, 0, 0, 2)
ZEND_ARG_INFO(0, httpMethod)
ZEND_ARG_INFO(0, uri)
ZEND_END_ARG_INFO()


PHP_METHOD(lycitea_route_simple, __construct) {
    zend_long runmode = LYCITEA_ROUTE_SIMPLE_RUN_STRICT;
    zval *self = getThis();
    zend_class_entry *ce = Z_OBJCE_P(self);

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "|l", &runmode) == FAILURE) {
        return;
    }

    if (!self) {
        RETURN_FALSE;
    }

    zval staticRoutes, regexRoutes;
    array_init(&staticRoutes);
    array_init(&regexRoutes);
    zend_update_property(ce, self, ZEND_STRL(LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_STATICROUTE), &staticRoutes);
    zend_update_property(ce, self, ZEND_STRL(LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_REGEXROUTE), &regexRoutes);
    zend_update_property_long(ce, self, ZEND_STRL(LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_RUNMODE), runmode);
    zval_ptr_dtor(&staticRoutes);
    zval_ptr_dtor(&regexRoutes);

}

PHP_METHOD(lycitea_route_simple, __destruct) {
}

PHP_METHOD(lycitea_route_simple, addRoute) {

    zval *httpMethod, *route, *handler;
    zend_long  mode = LYCITEA_ROUTE_SIMPLE_STATIC;
    zval *self = getThis();
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zzz|l", &httpMethod, &route, &handler, &mode) == FAILURE) {
        return;
    }
    addRoute(httpMethod, route, handler, mode, self);

}

PHP_METHOD(lycitea_route_simple, addGroup) {

    zval *prefix, *callback;
    zval *self = getThis();
    zend_class_entry *ce = Z_OBJCE_P(self);
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zz", &prefix, &callback) == FAILURE) {
        return;
    }
    zval *prevGroupPrefix = zend_read_property(ce, self, ZEND_STRL(LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_PREFIX), 1, NULL);
    smart_str joinPrefix = {0};
    smart_str_appendl(&joinPrefix, Z_STRVAL_P(prevGroupPrefix), Z_STRLEN_P(prevGroupPrefix));
    smart_str_appendl(&joinPrefix, Z_STRVAL_P(prefix), Z_STRLEN_P(prefix));
    smart_str_0(&joinPrefix);
    zval newPrefix;
    ZVAL_STRING(&newPrefix, joinPrefix.s->val);
    zend_update_property(ce, self, ZEND_STRL(LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_PREFIX), &newPrefix);
    zend_fcall_info_cache fcc;
    zend_string *key = NULL;
    if (!zend_is_callable_ex(callback, NULL, 0, &key, &fcc, NULL)) {
        php_error_docref(NULL, E_RECOVERABLE_ERROR, "the 2 parameter given by addGroup cannot be call", NULL);
    }
    zend_string_release(key);
    zval  retval;
    zend_call_method_with_1_params(NULL, NULL, &(fcc.function_handler), "callback", &retval, self);
    zval_ptr_dtor(&newPrefix);
}


PHP_METHOD(lycitea_route_simple, dispatch) {
    zval *httpMethod, *uri;
    zval *self = getThis();
    zend_class_entry *ce = Z_OBJCE_P(self);
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zz", &httpMethod, &uri) == FAILURE) {
        return;
    }

    zval routeData;
    array_init(&routeData);
    getData(self, &routeData);

    zval tmpHandler = depthFind(3, "lss", &routeData, 0, Z_STRVAL_P(httpMethod), Z_STRVAL_P(uri));
    zval rtn;
    array_init(&rtn);
    if(IS_NULL != Z_TYPE(tmpHandler)){
        zval handler;
        ZVAL_COPY(&handler, &tmpHandler);
        add_next_index_long(&rtn, 1);
        add_next_index_zval(&rtn, &handler);
        zval emptyArr;
        array_init(&emptyArr);
        add_next_index_zval(&rtn, &emptyArr);
        zval_ptr_dtor(&routeData);
        RETURN_ZVAL(&rtn, 1, 1);
    }

    zval routeInfo = depthFind(2, "ls", &routeData, 1, Z_STRVAL_P(httpMethod));
    if(IS_ARRAY == Z_TYPE(routeInfo)){
        zval *entry;
        ZEND_HASH_FOREACH_VAL(Z_ARRVAL(routeInfo), entry){

            zval *regex = zend_hash_str_find(Z_ARRVAL_P(entry), ZEND_STRL("regex"));
            pcre_cache_entry *pce;
            zval matches, subpats;
            ZVAL_NULL(&subpats);
            if ((pce = pcre_get_compiled_regex_cache(Z_STR_P(regex))) != NULL) {
                php_pcre_match_impl(pce, Z_STRVAL_P(uri), Z_STRLEN_P(uri), &matches, &subpats, 0, 0, 0, 0);
            }
            if(!Z_LVAL(matches)){
                zval_ptr_dtor(&subpats);
                continue;
            }
            zend_long count = zend_array_count(Z_ARRVAL(subpats));
            zval tmpHandler = depthFind(3, "sll", entry, "routeMap", count, 0);
            zval handler;
            ZVAL_COPY(&handler, &tmpHandler);
            add_next_index_long(&rtn, 1);
            add_next_index_zval(&rtn, &handler);
            zval varNames = depthFind(3, "sll", entry, "routeMap", count, 1);

            zval *var, vars;
            array_init(&vars);
            zend_long i = 0;
            ZEND_HASH_FOREACH_VAL(Z_ARRVAL(varNames), var){
                zval *tmpVal= zend_hash_index_find(Z_ARRVAL(subpats), ++i);
                zval value;
                ZVAL_COPY(&value, tmpVal);
                add_assoc_zval(&vars, Z_STRVAL_P(var), &value);
            }ZEND_HASH_FOREACH_END();
            zval_ptr_dtor(&subpats);
            add_next_index_zval(&rtn, &vars);
            zval_ptr_dtor(&routeData);
            RETURN_ZVAL(&rtn, 1, 1);
         }ZEND_HASH_FOREACH_END();
    }
    zval_ptr_dtor(&routeData);
    add_next_index_long(&rtn, 0);
    RETURN_ZVAL(&rtn, 1, 1);

}

PHP_METHOD(lycitea_route_simple, get) {
    zval *self = getThis();
    zval *route, *handler;
    zend_long  mode = LYCITEA_ROUTE_SIMPLE_STATIC;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zz|l", &route, &handler, &mode) == FAILURE) {
        return;
    }
    zval httpMethod;
    array_init(&httpMethod);
    add_next_index_string(&httpMethod, "GET");
    addRoute(&httpMethod, route, handler, mode, self);
}

PHP_METHOD(lycitea_route_simple, post) {
    zval *self = getThis();
    zval *route, *handler;
    zend_long  mode = LYCITEA_ROUTE_SIMPLE_STATIC;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zz|l", &route, &handler, &mode) == FAILURE) {
        return;
    }
    zval httpMethod;
    array_init(&httpMethod);
    add_next_index_string(&httpMethod, "POST");
    addRoute(&httpMethod, route, handler, mode, self);
}

PHP_METHOD(lycitea_route_simple, put) {
    zval *self = getThis();
    zval *route, *handler;
    zend_long  mode = LYCITEA_ROUTE_SIMPLE_STATIC;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zz|l", &route, &handler, &mode) == FAILURE) {
        return;
    }
    zval httpMethod;
    array_init(&httpMethod);
    add_next_index_string(&httpMethod, "PUT");
    addRoute(&httpMethod, route, handler, mode, self);
}

PHP_METHOD(lycitea_route_simple, delete) {
    zval *self = getThis();
    zval *route, *handler;
    zend_long  mode = LYCITEA_ROUTE_SIMPLE_STATIC;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zz|l", &route, &handler, &mode) == FAILURE) {
        return;
    }
    zval httpMethod;
    array_init(&httpMethod);
    add_next_index_string(&httpMethod, "DELETE");
    addRoute(&httpMethod, route, handler, mode, self);
}

PHP_METHOD(lycitea_route_simple, patch) {
    zval *self = getThis();
    zval *route, *handler;
    zend_long  mode = LYCITEA_ROUTE_SIMPLE_STATIC;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zz|l", &route, &handler, &mode) == FAILURE) {
        return;
    }
    zval httpMethod;
    array_init(&httpMethod);
    add_next_index_string(&httpMethod, "PATCH");
    addRoute(&httpMethod, route, handler, mode, self);
}

PHP_METHOD(lycitea_route_simple, head) {
    zval *self = getThis();
    zval *route, *handler;
    zend_long  mode = LYCITEA_ROUTE_SIMPLE_STATIC;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zz|l", &route, &handler, &mode) == FAILURE) {
        return;
    }
    zval httpMethod;
    array_init(&httpMethod);
    add_next_index_string(&httpMethod, "HEAD");
    addRoute(&httpMethod, route, handler, mode, self);
}

PHP_METHOD(lycitea_route_simple, options) {
    zval *self = getThis();
    zval *route, *handler;
    zend_long  mode = LYCITEA_ROUTE_SIMPLE_STATIC;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zz|l", &route, &handler, &mode) == FAILURE) {
        return;
    }
    zval httpMethod;
    array_init(&httpMethod);
    add_next_index_string(&httpMethod, "OPTIONS");
    addRoute(&httpMethod, route, handler, mode, self);
}

zend_function_entry lycitea_route_simple[] = {
    PHP_ME(lycitea_route_simple, __construct, lycitea_route_simple_construct_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    PHP_ME(lycitea_route_simple, addRoute, lycitea_route_simple_addroute_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(lycitea_route_simple, addGroup, lycitea_route_simple_addgroup_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(lycitea_route_simple, dispatch, lycitea_route_simple_dispatch_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(lycitea_route_simple, get, lycitea_route_simple_gppdpho_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(lycitea_route_simple, post, lycitea_route_simple_gppdpho_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(lycitea_route_simple, put, lycitea_route_simple_gppdpho_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(lycitea_route_simple, delete, lycitea_route_simple_gppdpho_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(lycitea_route_simple, patch, lycitea_route_simple_gppdpho_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(lycitea_route_simple, head, lycitea_route_simple_gppdpho_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(lycitea_route_simple, options, lycitea_route_simple_gppdpho_arginfo, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

LYCITEA_STARTUP_FUNCTION(route_simple) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Lycitea\\Route\\Simple", lycitea_route_simple);
    lycitea_route_simple_ce = zend_register_internal_class_ex(&ce, NULL);
    zend_declare_property_string(lycitea_route_simple_ce, ZEND_STRL(LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_PREFIX), "", ZEND_ACC_PROTECTED);
    zend_declare_property_null(lycitea_route_simple_ce, ZEND_STRL(LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_STATICROUTE), ZEND_ACC_FINAL | ZEND_ACC_PRIVATE);
    zend_declare_property_null(lycitea_route_simple_ce, ZEND_STRL(LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_REGEXROUTE), ZEND_ACC_FINAL | ZEND_ACC_PRIVATE);
    zend_declare_property_long(lycitea_route_simple_ce, ZEND_STRL(LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_RUNMODE), LYCITEA_ROUTE_SIMPLE_RUN_STRICT, ZEND_ACC_FINAL| ZEND_ACC_PRIVATE);
    return SUCCESS;
}

