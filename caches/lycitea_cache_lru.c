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
  | Author: Jinxi Wang <1054636713@qq.com>                               |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_lycitea.h"
#include "lycitea_cache_lru.h"
#include "helpers/lycitea_helpers_lru.h"
#include "helpers/lycitea_helpers_common.h"
#include "lycitea_exception.h"

zend_class_entry *lycitea_cache_lru_ce;
ZEND_BEGIN_ARG_INFO_EX(lycitea_cache_lru_construct_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, version)
    ZEND_ARG_INFO(0, numbers)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(lycitea_cache_lru_add_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, key)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(lycitea_cache_lru_get_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(lycitea_cache_lru_void_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()


PHP_METHOD(lycitea_cache_lru, __construct) {

    zend_long numbers = 200;
    zend_string *version = NULL;
    zval *self = getThis();
    zend_class_entry *ce = Z_OBJCE_P(self);
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S|l", &version, &numbers) == FAILURE) {
        return;
    }
    if (!self) {
        RETURN_FALSE;
    }
    if(0 >= numbers || 0 >= ZSTR_LEN(version)){
        char exc_cmsg[LYCITEA_MAX_MSG_EXCEPTION];
        if(0 >= numbers){
            snprintf(exc_cmsg, LYCITEA_MAX_MSG_EXCEPTION, "lru cache numbers must be gt 0");
        }else{
            snprintf(exc_cmsg, LYCITEA_MAX_MSG_EXCEPTION, "lru cache version can not be empty");
        }
        lycitea_throw_exception(LYCITEA_ERR_CACHE_FAILED, exc_cmsg);
    }
    zend_update_property_str(ce, self, ZEND_STRL(LYCITEA_CACHE_LRU_PROPERTY_NAME_VERSION), version);
    zend_update_property_long(ce, self, ZEND_STRL(LYCITEA_CACHE_LRU_PROPERTY_NAME_NUMBERS), numbers);

}

PHP_METHOD(lycitea_cache_lru, set) {
    zend_string *key = NULL, *value = NULL;
    zval *self = getThis();
    zend_class_entry *ce = Z_OBJCE_P(self);
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "SS", &key, &value) == FAILURE) {
        return;
    }
    zval *version = zend_read_property(ce, self, ZEND_STRL(LYCITEA_CACHE_LRU_PROPERTY_NAME_VERSION), 1, NULL);
    zval *numbers = zend_read_property(ce, self, ZEND_STRL(LYCITEA_CACHE_LRU_PROPERTY_NAME_NUMBERS), 1, NULL);
    int index = lycitea_helpers_lru_apply_cache(Z_STRVAL_P(version), Z_LVAL_P(numbers));
    if(0 > index){
        RETURN_BOOL(IS_FALSE);
    }

    int pageNumber;
    zval *old_value = zend_hash_find(Z_ARRVAL_P(LYCITEA_G(lru_cache).value[index]), key);
    if(old_value){
        pageNumber = Z_LVAL_P(zend_hash_index_find(Z_ARRVAL_P(old_value), 1));
    }else{
        pageNumber = LYCITEA_G(lru_cache).queue[index]->count;
    }
    HashTable *ht = Z_ARRVAL_P(LYCITEA_G(lru_cache).value[index]);
    int newItem = lycitea_helpers_lru_referencepage(LYCITEA_G(lru_cache).queue[index], LYCITEA_G(lru_cache).hash[index], pageNumber, ZSTR_VAL(key), ht);
    if(pageNumber == LYCITEA_G(lru_cache).queue[index]->numberOfFrames){
        pageNumber = LYCITEA_G(lru_cache).last_page_number;
    }
    if(1 == newItem){
        zval arr, pvalue;
        array_init_persistent(&arr);
        ZVAL_PSTRING(&pvalue, ZSTR_VAL(value));
        zend_hash_index_add(Z_ARRVAL(arr), 0, &pvalue);
        add_index_long(&arr, 1, pageNumber);
        zend_hash_str_add(Z_ARRVAL_P(LYCITEA_G(lru_cache).value[index]), ZSTR_VAL(key), ZSTR_LEN(key), &arr);
    }else if(0 == newItem){
        zval *arr = zend_hash_str_find(Z_ARRVAL_P(LYCITEA_G(lru_cache).value[index]), ZSTR_VAL(key), ZSTR_LEN(key));
        if(arr){
            zval *val = zend_hash_index_find(Z_ARRVAL_P(arr), 0);
            if(ZSTR_VAL(value) != Z_STRVAL_P(val)){
                zval pvalue;
                ZVAL_PSTRING(&pvalue, ZSTR_VAL(value));
                zend_hash_index_update(Z_ARRVAL_P(arr), 0, &pvalue);
            }
        }
    }
}

PHP_METHOD(lycitea_cache_lru, get) {
    zend_string *key = NULL;
    zval *self = getThis();
    zend_class_entry *ce = Z_OBJCE_P(self);
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &key) == FAILURE) {
        return;
    }

    zval *version = zend_read_property(ce, self, ZEND_STRL(LYCITEA_CACHE_LRU_PROPERTY_NAME_VERSION), 1, NULL);
    zval *index = zend_hash_find(Z_ARRVAL_P(LYCITEA_G(lru_cache).versionMap), Z_STR_P(version));

    if(NULL == index || IS_LONG != Z_TYPE_P(index)){
        RETURN_NULL();
    }

    zval *zv = zend_hash_find(Z_ARRVAL_P(LYCITEA_G(lru_cache).value[Z_LVAL_P(index)]), key);
    if(NULL == zv){
        RETURN_NULL();
    }
    zval *value = zend_hash_index_find(Z_ARRVAL_P(zv), 0);
    if(Z_STRLEN_P(value) < 1){
        RETURN_NULL();
    }
    int pageNumber = Z_LVAL_P(zend_hash_index_find(Z_ARRVAL_P(zv), 1));
    lycitea_helpers_lru_referencepage(LYCITEA_G(lru_cache).queue[Z_LVAL_P(index)], LYCITEA_G(lru_cache).hash[Z_LVAL_P(index)], pageNumber, ZSTR_VAL(key), Z_ARRVAL_P(LYCITEA_G(lru_cache).value[Z_LVAL_P(index)]));
    RETURN_ZVAL(value, 1, 0);

}

PHP_METHOD(lycitea_cache_lru, delete) {
    zend_string *key = NULL;
    zval *self = getThis();
    zend_class_entry *ce = Z_OBJCE_P(self);
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &key) == FAILURE) {
        return;
    }

    zval *version = zend_read_property(ce, self, ZEND_STRL(LYCITEA_CACHE_LRU_PROPERTY_NAME_VERSION), 1, NULL);
    zval *index = zend_hash_find(Z_ARRVAL_P(LYCITEA_G(lru_cache).versionMap), Z_STR_P(version));
    if(NULL == index || IS_LONG != Z_TYPE_P(index)){
        RETURN_FALSE;
    }
    zval *arr = zend_hash_find(Z_ARRVAL_P(LYCITEA_G(lru_cache).value[Z_LVAL_P(index)]), key);
    if(NULL == arr || IS_ARRAY != Z_TYPE_P(arr)){
        RETURN_FALSE;
    }
    zval str_null;
    ZVAL_NULL(&str_null);
    zend_hash_index_update(Z_ARRVAL_P(arr), 0, &str_null);
    RETURN_FALSE;
}

PHP_METHOD(lycitea_cache_lru, flush) {

    zval *self = getThis();
    zend_class_entry *ce = Z_OBJCE_P(self);
    zval *version = zend_read_property(ce, self, ZEND_STRL(LYCITEA_CACHE_LRU_PROPERTY_NAME_VERSION), 1, NULL);
    zval *index = zend_hash_find(Z_ARRVAL_P(LYCITEA_G(lru_cache).versionMap), Z_STR_P(version));
    if(NULL == index || IS_LONG != Z_TYPE_P(index)){
        RETURN_FALSE;
    }
    zval *rtn = LYCITEA_G(lru_cache).value[Z_LVAL_P(index)];
    zval *entry;
    ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(rtn), entry){
        zval str_null;
        ZVAL_NULL(&str_null);
        zend_hash_index_update(Z_ARRVAL_P(entry), 0, &str_null);
    }ZEND_HASH_FOREACH_END();
    RETURN_TRUE;
}

PHP_METHOD(lycitea_cache_lru, versions) {
    zval *self = getThis();
    zend_class_entry *ce = Z_OBJCE_P(self);
    zval *versions = LYCITEA_G(lru_cache).versionMap;

    zend_string *key = NULL;
    zval *entry;
    array_init_persistent(return_value);
    ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(versions), key, entry){
        add_assoc_zval(return_value, ZSTR_VAL(key), entry);
    }ZEND_HASH_FOREACH_END();
    RETVAL_ZVAL(return_value, 1, 0);

}


PHP_METHOD(lycitea_cache_lru, all) {

    zval *self = getThis();
    zend_class_entry *ce = Z_OBJCE_P(self);
    zval *version = zend_read_property(ce, self, ZEND_STRL(LYCITEA_CACHE_LRU_PROPERTY_NAME_VERSION), 1, NULL);
    zval *index = zend_hash_find(Z_ARRVAL_P(LYCITEA_G(lru_cache).versionMap), Z_STR_P(version));
    if(NULL == index || IS_LONG != Z_TYPE_P(index)){
        RETURN_NULL();
    }
    zval *rtn = LYCITEA_G(lru_cache).value[Z_LVAL_P(index)];
    zend_string *key = NULL;
    zval *entry;
    array_init_persistent(return_value);
    ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(rtn), key, entry){
        zval *value = zend_hash_index_find(Z_ARRVAL_P(entry), 0);
        if(IS_NULL != Z_TYPE_P(value)){
            add_assoc_zval(return_value, ZSTR_VAL(key), value);
        }
    }ZEND_HASH_FOREACH_END();
    RETVAL_ZVAL(return_value, 1, 0);
}


zend_function_entry lycitea_cache_lru[] = {
    PHP_ME(lycitea_cache_lru, __construct, lycitea_cache_lru_construct_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    PHP_ME(lycitea_cache_lru, set, lycitea_cache_lru_add_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(lycitea_cache_lru, get, lycitea_cache_lru_get_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(lycitea_cache_lru, delete, lycitea_cache_lru_get_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(lycitea_cache_lru, flush, lycitea_cache_lru_void_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(lycitea_cache_lru, all, lycitea_cache_lru_void_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(lycitea_cache_lru, versions, lycitea_cache_lru_void_arginfo, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

LYCITEA_STARTUP_FUNCTION(cache_lru) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Lycitea\\Cache\\lru", lycitea_cache_lru);
    lycitea_cache_lru_ce = zend_register_internal_class_ex(&ce, NULL);
    zend_declare_property_null(lycitea_cache_lru_ce, ZEND_STRL(LYCITEA_CACHE_LRU_PROPERTY_NAME_VERSION),  ZEND_ACC_PROTECTED);
    zend_declare_property_null(lycitea_cache_lru_ce, ZEND_STRL(LYCITEA_CACHE_LRU_PROPERTY_NAME_NUMBERS),  ZEND_ACC_PROTECTED);
    return SUCCESS;
}

