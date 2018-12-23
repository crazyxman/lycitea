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

#include "helpers/lycitea_helpers_lru.h"

#ifndef PHP_LYCITEA_H
#define PHP_LYCITEA_H

extern zend_module_entry lycitea_module_entry;
#define phpext_lycitea_ptr &lycitea_module_entry

#ifdef PHP_WIN32
#define PHP_LYCITEA_API __declspec(dllexport)
#ifndef _MSC_VER
#define _MSC_VER 1600
#endif
#else
#define PHP_LYCITEA_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#ifdef ZTS
#define LYCITEA_G(v) TSRMG(lycitea_globals_id, zend_lycitea_globals *, v)
#else
#define LYCITEA_G(v) (lycitea_globals.v)
#endif

#define PHP_LYCITEA_VERSION                  "1.0.0"


#define LYCITEA_STARTUP_FUNCTION(module)    ZEND_MINIT_FUNCTION(lycitea_##module)
#define LYCITEA_RINIT_FUNCTION(module)        ZEND_RINIT_FUNCTION(lycitea_##module)
#define LYCITEA_STARTUP(module)                ZEND_MODULE_STARTUP_N(lycitea_##module)(INIT_FUNC_ARGS_PASSTHRU)
#define LYCITEA_SHUTDOWN_FUNCTION(module)    ZEND_MSHUTDOWN_FUNCTION(lycitea_##module)
#define LYCITEA_SHUTDOWN(module)            ZEND_MODULE_SHUTDOWN_N(lycitea_##module)(INIT_FUNC_ARGS_PASSTHRU)
extern PHPAPI void php_var_dump(zval **struc, int level);
extern PHPAPI void php_debug_zval_dump(zval **struc, int level);

ZEND_BEGIN_MODULE_GLOBALS(lycitea)
    lru_container   lru_cache;
ZEND_END_MODULE_GLOBALS(lycitea)

PHP_MINIT_FUNCTION(lycitea);
PHP_MSHUTDOWN_FUNCTION(lycitea);
PHP_RINIT_FUNCTION(lycitea);
PHP_RSHUTDOWN_FUNCTION(lycitea);
PHP_MINFO_FUNCTION(lycitea);

extern ZEND_DECLARE_MODULE_GLOBALS(lycitea);

#endif