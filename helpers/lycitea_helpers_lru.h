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

#ifndef PHP_LYCITEA_HELPERS_LRU_H
#define PHP_LYCITEA_HELPERS_LRU_H

typedef struct _lru_qnode
{
    struct _lru_qnode *prev, *next;
    unsigned pageNumber;
    char *key;
} lru_qnode;

typedef struct _lru_queue
{
    unsigned count;
    unsigned numberOfFrames;
    lru_qnode *front, *rear;
} lru_queue;

typedef struct _lru_hash
{
    int capacity;
    lru_qnode* *array;
} lru_hash;

typedef struct _lru_container
{
    int             count;
    lru_queue       **queue;
    lru_hash        **hash;
    zval            **value;
    zval            *versionMap;
    zend_long       last_page_number;
    zend_long       clean_cycle;
    zend_long       prev_clean_time;
    zend_long       memory_limit;
    zend_long       memory_used;
} lru_container;


int lycitea_helpers_lru_apply_cache(char* , int );
int lycitea_helpers_lru_referencepage( lru_queue* , lru_hash* , int, char*  , HashTable *);
void lycitea_helpers_lru_destroy(time_t);
#endif
