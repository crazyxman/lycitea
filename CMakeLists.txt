cmake_minimum_required(VERSION 3.12)
project(lycitea_new)

#set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(PHP_BIN /usr/local/php7_debug/bin)

add_executable(lycitea
        php_lycitea.h
        lycitea.c
        lycitea_exception.c
        lycitea_exception.h
        helpers/lycitea_helpers_common.h
        helpers/lycitea_helpers_common.c
        helpers/lycitea_helpers_lru.c helpers/lycitea_helpers_lru.h caches/lycitea_cache_lru.c caches/lycitea_cache_lru.h)


set(PHP_SOURCE /usr/local/php7_debug/include/php)

include_directories(${PHP_SOURCE}/main)
include_directories(${PHP_SOURCE}/Zend)
include_directories(${PHP_SOURCE}/TSRM)
include_directories(${PHP_SOURCE}/sapi)
include_directories(${PHP_SOURCE}/pear)
include_directories(${PHP_SOURCE}/ext)
include_directories(${PHP_SOURCE})
include_directories(/Users/wangjinxi/dockerWorkspace/lycitea_new)

#add_custom_target(lycitea_new COMMAND sudo /usr/local/php7_debug/bin/phpize && ./configure --with-php-config=/usr/local/php7_debug/bin/php-config &&  make WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
add_custom_target(lycitea_new COMMAND ${PHP_BIN}/phpize && ./configure --with-php-config=${PHP_BIN}/php-config && $(MAKE) clean && $(MAKE) -j4 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
#add_custom_target(lycitea_new COMMAND $(MAKE) clean && $(MAKE) -j4 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
#add_custom_target(lycitea_new COMMAND  $(MAKE) -j4 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})