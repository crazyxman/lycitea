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
#include "main/SAPI.h"
#include "ext/standard/php_string.h"
#include "Zend/zend_interfaces.h"

#include "php_lycitea.h"
#include "lycitea_helpers_common.h"


zval depthFind(int args,  char* types, zval * const arr, ...) {

    va_list argptr;
    va_start(argptr, arr);

    char *next_char;
    int i = 0, next_int;
    zval *pzval = arr;

    while (i < args) {

        if(NULL != pzval && IS_ARRAY == Z_TYPE_P(pzval)){
            if(*types == 'l'){
                next_int = va_arg(argptr, int);
                pzval = zend_hash_index_find(Z_ARRVAL_P(pzval), next_int);
            }else{
                next_char = va_arg(argptr, char*);
                pzval = zend_hash_str_find(Z_ARRVAL_P(pzval), next_char, strlen(next_char));
            }
        }

        if (NULL == pzval) {
            zval nval;
            ZVAL_NULL(&nval);
            pzval = &nval;
            break;
        }

        i++;
        types++;

    }
    va_end(argptr);
    return *pzval;

}

void depthAdd(int args,  char* types,  zval *value, zval *arr, ...) {

    va_list argptr;
    va_start(argptr, arr);

    char *next_char;
    int i = 0, next_int;
    zval *pzval = arr, *val = NULL;
    while (i < args) {
        if(*types == 'l'){
            next_int = va_arg(argptr, int);
            val = zend_hash_index_find(Z_ARRVAL_P(pzval), next_int);
        }else{
            next_char = va_arg(argptr, char*);
            val = zend_hash_str_find(Z_ARRVAL_P(pzval), next_char, strlen(next_char));
        }
        if(NULL != val){
            pzval = val;
        }else if(i < (args - 1)){
            zval emptyArr;
            array_init(&emptyArr);
            if(*types == 'l'){
                add_index_zval(pzval, next_int, &emptyArr);
                pzval = zend_hash_index_find(Z_ARRVAL_P(pzval), next_int);
            }else{
                add_assoc_zval(pzval, next_char, &emptyArr);
                pzval = zend_hash_str_find(Z_ARRVAL_P(pzval), next_char, strlen(next_char));
            }
        }
        i++;
        types++;
    }

    va_end(argptr);

    if(NULL != val){
        ZVAL_COPY_VALUE(pzval, value);
        return;
    }

    types--;
    if(*types == 'l'){
        add_index_zval(pzval, next_int, value);
    }else{
        add_assoc_zval(pzval, next_char, value);
    }
}

void z_strcat(zval *left, zval *right, zval *dest, zend_long interned)
{
    zend_string *odest = Z_STR_P(dest);
    zend_string *str = zend_string_alloc(Z_STRLEN_P(left) + Z_STRLEN_P(right), 0);
    memcpy(ZSTR_VAL(str), Z_STRVAL_P(left), Z_STRLEN_P(left));
    memcpy(ZSTR_VAL(str) + Z_STRLEN_P(left), Z_STRVAL_P(right), Z_STRLEN_P(right));
    ZSTR_VAL(str)[ZSTR_LEN(str)] = '\0';
    if(0 == interned){
        ZVAL_STR(dest, str);
    }else{
        ZVAL_INTERNED_STR(dest, zend_new_interned_string(str));
    }
    zend_string_release(odest);
}




