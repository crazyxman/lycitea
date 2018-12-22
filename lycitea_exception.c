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
#include "Zend/zend_exceptions.h"

#include "php_lycitea.h"
#include "lycitea_exception.h"


zend_class_entry *lycitea_exception_ce;
zend_class_entry *lycitea_buildin_exceptions[LYCITEA_MAX_BUILDIN_EXCEPTION];

void lycitea_throw_exception(long code, char *message) {
    zend_class_entry *base_exception = lycitea_exception_ce;

    if ((code & LYCITEA_ERR_BASE) == LYCITEA_ERR_BASE
        && lycitea_buildin_exceptions[LYCITEA_EXCEPTION_OFFSET(code)]) {
        base_exception = lycitea_buildin_exceptions[LYCITEA_EXCEPTION_OFFSET(code)];
    }

    zend_throw_exception(base_exception, message, code);
}

zend_function_entry lycitea_exception_methods[] = {
    {NULL, NULL, NULL}
};

LYCITEA_STARTUP_FUNCTION(exception) {
    zend_class_entry ce;
    zend_class_entry cache_ce;

    INIT_CLASS_ENTRY(ce, "Lycitea\\Exception", lycitea_exception_methods);
    lycitea_exception_ce = zend_register_internal_class_ex(&ce, zend_exception_get_default());
    zend_declare_property_null(lycitea_exception_ce, ZEND_STRL("message"), 	ZEND_ACC_PROTECTED);
    zend_declare_property_long(lycitea_exception_ce, ZEND_STRL("code"), 0,	ZEND_ACC_PROTECTED);
    zend_declare_property_null(lycitea_exception_ce, ZEND_STRL("previous"),  ZEND_ACC_PROTECTED);

    INIT_CLASS_ENTRY(cache_ce, "Lycitea\\Exception\\Cache", NULL);
    lycitea_buildin_exceptions[LYCITEA_EXCEPTION_OFFSET(LYCITEA_ERR_CACHE_FAILED)] = zend_register_internal_class_ex(&cache_ce, lycitea_exception_ce);

    return SUCCESS;
}
