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

#include "php.h"
#include "php_lycitea.h"
#include "lycitea_helpers_common.h"


void* lycitea_helpers_lru_apply_memory(zend_long size, void *p, int expand)
{
    if(size < 1 ){
        return NULL;
    }
    int size_added = size + LYCITEA_G(lru_cache.memory_used);
    if( size_added >  LYCITEA_G(lru_cache).memory_limit || size_added > zend_memory_usage(1)){
        return NULL;
    }
    void *tmp = NULL;
    if(expand){
        tmp = realloc(p, size);
        if (EXPECTED(tmp || !size)) {
            LYCITEA_G(lru_cache.memory_used) = size_added;
            p = tmp;
            return p;
        }
    }else{
        tmp = malloc(size);
        if (EXPECTED(tmp || !size)) {
            LYCITEA_G(lru_cache.memory_used) = size_added;
            return tmp;
        }
    }
    return NULL;
}

int lycitea_helpers_lru_free_memory(void *p, u_int size)
{
    if(NULL == p){
        /* return 0; */
    }
    if(!size){
        size = sizeof(p);
    }
    LYCITEA_G(lru_cache.memory_used) -= size;
    if(LYCITEA_G(lru_cache.memory_used) < 0){
        LYCITEA_G(lru_cache.memory_used) = 0;
    }
    free(p);
    return 1;
}

int lycitea_helpers_lru_sub_usage(u_int size)
{
    if(size < 0){
         return 0;
    }
    LYCITEA_G(lru_cache.memory_used) -= size;
    if(LYCITEA_G(lru_cache.memory_used) < 0){
        LYCITEA_G(lru_cache.memory_used) = 0;
    }
    return 1;
}

void lycitea_helpers_lru_flush(lru_queue* queue, lru_hash* hash, zval *value)
{
    if(hash){
        int i;
        for( i = 0; i < hash->capacity; ++i ){
            lycitea_helpers_lru_free_memory(hash->array[i], sizeof(lru_qnode));
        }
        lycitea_helpers_lru_free_memory(hash, sizeof(lru_hash));
    }
    if(queue){
        lycitea_helpers_lru_free_memory(queue, sizeof(lru_queue));
    }
    if(value){
        zend_hash_clean(Z_ARRVAL_P(value));
        lycitea_helpers_lru_free_memory(value, sizeof(value));
    }

}

void lycitea_helpers_lru_destroy()
{
    if(LYCITEA_G(lru_cache).clean_cycle > 60){
        time_t timestamp = time(NULL);
        if(( (timestamp - LYCITEA_G(lru_cache).prev_clean_time)) > LYCITEA_G(lru_cache).clean_cycle){
            zval *index;
            ZEND_HASH_FOREACH_VAL(Z_ARRVAL(LYCITEA_G(lru_cache).versionMap), index){
                lycitea_helpers_lru_flush(LYCITEA_G(lru_cache).queue[Z_LVAL_P(index)], LYCITEA_G(lru_cache).hash[Z_LVAL_P(index)], LYCITEA_G(lru_cache).value[Z_LVAL_P(index)]);
            }ZEND_HASH_FOREACH_END();
            lycitea_helpers_lru_free_memory(LYCITEA_G(lru_cache).queue, 0);
            lycitea_helpers_lru_free_memory(LYCITEA_G(lru_cache).hash, 0);
            lycitea_helpers_lru_free_memory(LYCITEA_G(lru_cache).value, 0);
            LYCITEA_G(lru_cache).count = 0;
            LYCITEA_G(lru_cache).last_page_number = 0;
            LYCITEA_G(lru_cache).memory_used = 0;
            zend_hash_clean(Z_ARRVAL(LYCITEA_G(lru_cache).versionMap));
            LYCITEA_G(lru_cache).prev_clean_time = timestamp;
        }
    }
}

lru_qnode* lycitea_helpers_lru_create_qnode(lru_queue* queue, char* key, int pageNumber)
{
    lru_qnode* temp = lycitea_helpers_lru_apply_memory(sizeof( lru_qnode ), NULL, 0);
    if(!temp){
        return NULL;
    }
    temp->pageNumber = pageNumber;
    temp->key = estrdup(key);
    temp->prev = temp->next = NULL;
    return temp;
}

lru_queue* lycitea_helpers_lru_create_queue( int numberOfFrames )
{
    lru_queue* queue = lycitea_helpers_lru_apply_memory(sizeof( lru_queue ), NULL, 0);
    if(!queue){
        return NULL;
    }
    queue->count = 0;
    queue->front = queue->rear = NULL;
    queue->numberOfFrames = numberOfFrames;
    return queue;
}

lru_hash* lycitea_helpers_lru_create_hash( int capacity )
{
    lru_hash* hash = lycitea_helpers_lru_apply_memory(sizeof( lru_hash ), NULL, 0);
    if(!hash){
        return NULL;
    }
    hash->capacity = capacity;
    hash->array = lycitea_helpers_lru_apply_memory(hash->capacity * sizeof( lru_qnode* ), NULL, 0);
    if(!hash->array){
        lycitea_helpers_lru_free_memory(hash, sizeof(lru_hash));
        return NULL;
    }

    int i;
    for( i = 0; i < hash->capacity; ++i ){
        hash->array[i] = NULL;
    }

    return hash;
}

void lycitea_helpers_lru_del_queue( lru_queue* queue )
{
    if( queue->rear == NULL ){
        return;
    }
    if (queue->front == queue->rear){
        queue->front = NULL;
    }
    lru_qnode* temp = queue->rear;
    queue->rear = queue->rear->prev;
    if (queue->rear){
        queue->rear->next = NULL;
    }
    lycitea_helpers_lru_free_memory( temp , sizeof(lru_qnode));
    queue->count--;
}

/*
void lycitea_helpers_lru_del_qnode( lru_queue* queue, lru_qnode* qnode )
{
    if(qnode->prev){
        qnode->prev->next = qnode->next;
    }else if(qnode->next){
        qnode->next->prev = NULL;
        queue->front = qnode->next;
    }
    lycitea_helpers_lru_free_memory( qnode , sizeof(lru_qnode));
    queue->count--;
}
*/

int lycitea_helpers_lru_enqueue( lru_queue* queue, lru_hash* hash, int pageNumber, char* key, HashTable *ht)
{

    if ( queue->count == queue->numberOfFrames ){
        pageNumber = queue->rear->pageNumber;
        hash->array[ pageNumber ] = NULL;
        zval *value = zend_hash_str_find(ht, queue->rear->key, strlen(queue->rear->key));
        LYCITEA_G(lru_cache).last_page_number = Z_LVAL_P(zend_hash_index_find(Z_ARRVAL_P(value), 1));
        zend_hash_str_del(ht, queue->rear->key, strlen(queue->rear->key));
        lycitea_helpers_lru_del_queue( queue );
    }
    lru_qnode* temp = lycitea_helpers_lru_create_qnode( queue, key, pageNumber);
    if(NULL == temp){
        return -1;
    }
    temp->next = queue->front;
    if ( queue->rear == NULL ){
        queue->rear = queue->front = temp;

    }else{
        queue->front->prev = temp;
        queue->front = temp;
    }
    hash->array[ pageNumber ] = temp;
    queue->count++;
    return 1;
}

int lycitea_helpers_lru_referencepage( lru_queue* queue, lru_hash* hash, int pageNumber, char* key , HashTable *ht)
{
    lru_qnode* reqPage = NULL;
    if(pageNumber >= queue->numberOfFrames){
        pageNumber = queue->numberOfFrames-1;
    }else{
        reqPage = hash->array[ pageNumber ];
    }

    if(NULL == reqPage){
        return lycitea_helpers_lru_enqueue( queue, hash, pageNumber, key, ht );
    }else if (reqPage != queue->front){
        reqPage->prev->next = reqPage->next;
        if (reqPage->next){
            reqPage->next->prev = reqPage->prev;
        }
        if (reqPage == queue->rear){
            queue->rear = reqPage->prev;
            queue->rear->next = NULL;
        }
        reqPage->next = queue->front;
        reqPage->prev = NULL;
        reqPage->next->prev = reqPage;
        queue->front = reqPage;
    }
    return 0;
}

int lycitea_helpers_lru_apply_cache(char* version, int numbers)
{

    zval *index = zend_hash_str_find(Z_ARRVAL(LYCITEA_G(lru_cache).versionMap), version, strlen(version));
    if(NULL == index || IS_UNDEF == Z_TYPE_P(index)){
        int count = LYCITEA_G(lru_cache).count;
        void *queued, *hashed, *valued;
        if(NULL == LYCITEA_G(lru_cache).queue){
            queued = lycitea_helpers_lru_apply_memory((count + 1) * sizeof(lru_queue *), NULL, 0);
        }else{
            queued = lycitea_helpers_lru_apply_memory((count + 1) * sizeof(lru_queue *), LYCITEA_G(lru_cache).queue, 1);
        }
        if(!queued){
            return -1;
        }
        LYCITEA_G(lru_cache).queue = queued;
        LYCITEA_G(lru_cache).queue[count] = lycitea_helpers_lru_create_queue(numbers);
        if(!LYCITEA_G(lru_cache).queue[count]){
            lycitea_helpers_lru_apply_memory(count * sizeof(lru_queue *), LYCITEA_G(lru_cache).queue, 1);
            lycitea_helpers_lru_sub_usage(sizeof(lru_queue *));
            return -1;
        }
        if(NULL == LYCITEA_G(lru_cache).hash){
            hashed = lycitea_helpers_lru_apply_memory((count + 1) * sizeof(lru_hash *), NULL, 0);
        }else{
            hashed = lycitea_helpers_lru_apply_memory((count + 1) * sizeof(lru_hash *), LYCITEA_G(lru_cache).hash, 1);
        }
        if(!hashed){
            lycitea_helpers_lru_free_memory(LYCITEA_G(lru_cache).queue[count], sizeof(lru_queue));
            return -1;
        }
        LYCITEA_G(lru_cache).hash = hashed;
        LYCITEA_G(lru_cache).hash[count] = lycitea_helpers_lru_create_hash(numbers);
        if(!LYCITEA_G(lru_cache).hash[count]){
            lycitea_helpers_lru_free_memory(LYCITEA_G(lru_cache).queue[count], sizeof(lru_queue));
            lycitea_helpers_lru_apply_memory(count * sizeof(lru_hash *), LYCITEA_G(lru_cache).hash, 1);
            lycitea_helpers_lru_sub_usage(sizeof(lru_hash *));
            return -1;
        }
        if(NULL == LYCITEA_G(lru_cache).value){
            valued = lycitea_helpers_lru_apply_memory((count + 1) * sizeof(zval *), NULL, 0);
        }else{
            valued = lycitea_helpers_lru_apply_memory((count + 1) * sizeof(zval *), LYCITEA_G(lru_cache).value, 1);
        }
        if(!valued){
            lycitea_helpers_lru_free_memory(LYCITEA_G(lru_cache).queue[count], sizeof(lru_queue));
            lycitea_helpers_lru_free_memory(LYCITEA_G(lru_cache).hash[count], sizeof(lru_hash));
            return -1;
        }
        LYCITEA_G(lru_cache).value = valued;
        LYCITEA_G(lru_cache).value[count] = lycitea_helpers_lru_apply_memory(sizeof(zval *), NULL, 0);
        if(!LYCITEA_G(lru_cache).value[count]){
            lycitea_helpers_lru_free_memory(LYCITEA_G(lru_cache).queue[count], sizeof(lru_queue));
            lycitea_helpers_lru_free_memory(LYCITEA_G(lru_cache).hash[count], sizeof(lru_hash));
            lycitea_helpers_lru_apply_memory(count * sizeof(zval *), LYCITEA_G(lru_cache).value, 1);
            lycitea_helpers_lru_sub_usage(sizeof(zval *));
            return -1;
        }
        array_init_persistent(LYCITEA_G(lru_cache).value[count]);
        add_assoc_long(&(LYCITEA_G(lru_cache).versionMap), version, LYCITEA_G(lru_cache).count);
        LYCITEA_G(lru_cache).count = count + 1;
        return count;
    }
    return Z_LVAL_P(index);

}

