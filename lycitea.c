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

#include "php.h"
#include "php_ini.h"
#include "main/SAPI.h"
#include "Zend/zend_alloc.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"

#include "php_lycitea.h"
#include "routes/lycitea_route_simple.h"
#include "lycitea_exception.h"
#include "helpers/lycitea_helpers_common.h"


ZEND_DECLARE_MODULE_GLOBALS(lycitea);


zend_function_entry lycitea_functions[] = {
        {NULL, NULL, NULL}
};

PHP_GINIT_FUNCTION(lycitea)
{
     memset(lycitea_globals, 0, sizeof(*lycitea_globals));
}

PHP_MINIT_FUNCTION(lycitea)
{
    LYCITEA_G(pre_time) = time(NULL);
    array_init_persistent(&LYCITEA_G(route_cache));
    REGISTER_LONG_CONSTANT("LYCITEA_ROUTE_SIMPLE_STATIC", 		LYCITEA_ROUTE_SIMPLE_STATIC, CONST_PERSISTENT | CONST_CS);
    REGISTER_LONG_CONSTANT("LYCITEA_ROUTE_SIMPLE_REGEX", 		LYCITEA_ROUTE_SIMPLE_REGEX, CONST_PERSISTENT | CONST_CS);
    REGISTER_LONG_CONSTANT("LYCITEA_ROUTE_SIMPLE_RUN_STRICT", 		LYCITEA_ROUTE_SIMPLE_RUN_STRICT, CONST_PERSISTENT | CONST_CS);
    REGISTER_LONG_CONSTANT("LYCITEA_ROUTE_SIMPLE_RUN_FAST", 		LYCITEA_ROUTE_SIMPLE_RUN_FAST, CONST_PERSISTENT | CONST_CS);

    LYCITEA_STARTUP(route_simple);
    LYCITEA_STARTUP(exception);

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(lycitea)
{
    zend_hash_clean(Z_ARRVAL(LYCITEA_G(route_cache)));
    return SUCCESS;
}

PHP_RINIT_FUNCTION(lycitea)
{

    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(lycitea)
{
    time_t cur_time = time(NULL);
    if( (cur_time - LYCITEA_G(pre_time)) > 3600 ){
        zend_hash_clean(Z_ARRVAL(LYCITEA_G(route_cache)));
        LYCITEA_G(pre_time) = cur_time;
    }
    return SUCCESS;
}

PHP_MINFO_FUNCTION(lycitea)
{
    php_info_print_table_start();
    if (PG(expose_php) && !sapi_module.phpinfo_as_text) {
        php_info_print_table_header(2, "lycitea support", "enabled");
    } else {
        php_info_print_table_header(2, "lycitea support", "enabled");
    }

    php_info_print_table_row(2, "Version", PHP_LYCITEA_VERSION);
    php_info_print_table_row(2, "Supports", "http://www.lycitea.com");
    php_info_print_table_end();

    DISPLAY_INI_ENTRIES();
}

#ifdef COMPILE_DL_LYCITEA
ZEND_GET_MODULE(lycitea)
#endif

#if ZEND_MODULE_API_NO >= 20050922
zend_module_dep lycitea_deps[] = {
	{NULL, NULL, NULL}
};
#endif

zend_module_entry lycitea_module_entry = {
#if ZEND_MODULE_API_NO >= 20050922
STANDARD_MODULE_HEADER_EX, NULL,
lycitea_deps,
#else
    STANDARD_MODULE_HEADER,
#endif
    "lycitea",
lycitea_functions,
    PHP_MINIT(lycitea),
    PHP_MSHUTDOWN(lycitea),
    PHP_RINIT(lycitea),
    PHP_RSHUTDOWN(lycitea),
    PHP_MINFO(lycitea),
    PHP_LYCITEA_VERSION,
    PHP_MODULE_GLOBALS(lycitea),
    PHP_GINIT(lycitea),
    NULL,
    NULL,
    STANDARD_MODULE_PROPERTIES_EX
};





