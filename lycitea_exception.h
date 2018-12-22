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


#ifndef PHP_LYCITEA_EXCEPTION_H
#define PHP_LYCITEA_EXCEPTION_H

#define LYCITEA_MAX_BUILDIN_EXCEPTION	1
#define LYCITEA_MAX_MSG_EXCEPTION 		255

#define LYCITEA_ERR_BASE 				512
#define LYCITEA_ERR_MASK				127

#define LYCITEA_ERR_CACHE_FAILED 		512

#define LYCITEA_EXCEPTION_OFFSET(x) (x & LYCITEA_ERR_MASK)

extern zend_class_entry *lycitea_exception_ce;
extern void lycitea_throw_exception(long, char *);

LYCITEA_STARTUP_FUNCTION(exception);

#endif
