//
// Created by 王进喜 on 2018-12-17.
//

#ifndef PHP_LYCITEA_CACHE_LRU_H
#define PHP_LYCITEA_CACHE_LRU_H

#define LYCITEA_CACHE_LRU_PROPERTY_NAME_VERSION          "version"
#define LYCITEA_CACHE_LRU_PROPERTY_NAME_NUMBERS          "numbers"

extern zend_class_entry *lycitea_cache_lru_ce;
LYCITEA_STARTUP_FUNCTION(cache_lru);
#endif 
