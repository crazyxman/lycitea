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
#include "lycitea_exception.h"
#include "helpers/lycitea_helpers_common.h"
#include "caches/lycitea_cache_lru.h"


ZEND_DECLARE_MODULE_GLOBALS(lycitea);

ZEND_BEGIN_ARG_INFO(arginfo_lru_memory_usage, 0)
ZEND_END_ARG_INFO()


PHP_FUNCTION(lycitea_lru_memory_usage)
{
    RETURN_LONG(LYCITEA_G(lru_cache).memory_usage);
}

zend_function_entry lycitea_functions[] = {
        PHP_FE(lycitea_lru_memory_usage,                                 arginfo_lru_memory_usage)
        {NULL, NULL, NULL}
};

PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("lycitea.lru_memory_limit", "2", PHP_INI_SYSTEM, OnUpdateLongGEZero, lru_cache.memory_limit, zend_lycitea_globals, lycitea_globals)
    STD_PHP_INI_ENTRY("lycitea.lru_clean_cycle", "3600", PHP_INI_SYSTEM, OnUpdateLongGEZero, lru_cache.clean_cycle, zend_lycitea_globals, lycitea_globals)
PHP_INI_END();

PHP_GINIT_FUNCTION(lycitea)
{
     memset(lycitea_globals, 0, sizeof(*lycitea_globals));
}

PHP_MINIT_FUNCTION(lycitea)
{
    REGISTER_INI_ENTRIES();
    LYCITEA_G(lru_cache).prev_clean_time = time(NULL);
    LYCITEA_G(lru_cache).count = 0;
    LYCITEA_G(lru_cache).last_page_number = 0;
    LYCITEA_G(lru_cache).memory_limit = (1024 * 1024 * LYCITEA_G(lru_cache).memory_limit);
    LYCITEA_G(lru_cache).versionMap = pemalloc(sizeof(zval), 1);
    array_init_persistent(LYCITEA_G(lru_cache).versionMap);

    LYCITEA_STARTUP(exception);
    LYCITEA_STARTUP(cache_lru);

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(lycitea)
{
    return SUCCESS;
}

PHP_RINIT_FUNCTION(lycitea)
{
    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(lycitea)
{
    if(LYCITEA_G(lru_cache).clean_cycle >= 60){
        time_t timestamp = time(NULL);
        if(( (timestamp - LYCITEA_G(lru_cache).prev_clean_time)) > LYCITEA_G(lru_cache).clean_cycle){
            lycitea_helpers_lru_destroy(timestamp);
        }
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





