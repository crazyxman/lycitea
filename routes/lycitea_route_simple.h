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

#ifndef PHP_LYCITEA_ROUTE_SIMPLE_H
#define PHP_LYCITEA_ROUTE_SIMPLE_H

#define LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_PREFIX           "groupPrefix"
#define LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_GROUPDATA        "groupData"
#define LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_STATICROUTE      "staticRoutes"
#define LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_REGEXROUTE       "regexRoutes"
#define LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_RUNMODE          "runMode"
#define LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_VERSION          "version"

#define LYCITEA_ROUTE_SIMPLE_STATIC 		        612
#define LYCITEA_ROUTE_SIMPLE_REGEX 		            613

#define LYCITEA_ROUTE_SIMPLE_RUN_STRICT                     1<<0
#define LYCITEA_ROUTE_SIMPLE_RUN_FAST                       1<<1
#define LYCITEA_ROUTE_SIMPLE_RUN_MASK                       LYCITEA_ROUTE_SIMPLE_RUN_STRICT | LYCITEA_ROUTE_SIMPLE_RUN_FAST;

extern zend_class_entry *lycitea_route_simple_ce;
LYCITEA_STARTUP_FUNCTION(route_simple);
#endif