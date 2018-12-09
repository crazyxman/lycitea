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
#include "main/SAPI.h" /* for sapi_header_line */
#include "ext/standard/php_string.h" /* for php_implode */
#include "ext/standard/php_math.h"
#include "ext/pcre/php_pcre.h"
#include "Zend/zend_interfaces.h"
#include "Zend/zend_smart_str.h"


#include "php_lycitea.h"
#include "helpers/lycitea_helpers_common.h"
#include "routes/lycitea_route_simple.h"
#include "lycitea_exception.h"

#include <math.h>

#ifndef MIN
# define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
# define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#define ROUTER_PREG_SET_ORDER                2
#define ROUTER_PREG_OFFSET_CAPTURE            (1<<8)
#define ROUTER_VARIABLE_REGEX              "\\{ \\s* ([a-zA-Z_][a-zA-Z0-9_-]*) \\s* (?: : \\s* ([^{}]*(?:\\{(?-1)\\}[^{}]*)*) )? \\}"
#define ROUTER_DEFAULT_REGEX      "[^/]+"

int lycitea_route_simple_matches(zend_string *regex, zval *routeStr)
{

    smart_str pattern = {0};
    smart_str_appendl(&pattern, "~^", sizeof("~^") - 1);
    smart_str_appendl(&pattern, ZSTR_VAL(regex), ZSTR_LEN(regex));
    smart_str_appendl(&pattern, "$~", sizeof("$~") - 1);
    smart_str_0(&pattern);
    pcre_cache_entry *pce;
    if ((pce = pcre_get_compiled_regex_cache(pattern.s)) == NULL) {
        return IS_FALSE;
    }
    smart_str_free(&pattern);
    zval matches, subpats;
    ZVAL_NULL(&subpats);
    php_pcre_match_impl(pce, Z_STRVAL_P(routeStr), Z_STRLEN_P(routeStr), &matches, &subpats, 0, 0, 0, 0);
    if (!zend_hash_num_elements(Z_ARRVAL(subpats))) {
        zval_ptr_dtor(&subpats);
        return IS_FALSE;
    }
    zval_ptr_dtor(&subpats);
    return IS_TRUE;
}

void lycitea_route_simple_add_staticroute(zval *httpMethods, zval *routeData, zval *handler, zval *obj)
{

    zval *routeStr = zend_hash_index_find(Z_ARRVAL_P(routeData), 0);
    zval *sRoutes = zend_read_property(Z_OBJCE_P(obj), obj, ZEND_STRL(LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_STATICROUTE), 1, NULL);
    zval *runMode = zend_read_property(Z_OBJCE_P(obj), obj, ZEND_STRL(LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_RUNMODE), 1, NULL);

    zval *entry = NULL;
    ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(httpMethods), entry){
        if(Z_LVAL_P(runMode) == LYCITEA_ROUTE_SIMPLE_RUN_STRICT){
            zval sRoute = lycitea_helpers_common_depthfind(2, "ss", sRoutes, Z_STRVAL_P(entry), Z_STRVAL_P(routeStr));
            if(IS_NULL != Z_TYPE(sRoute)){
                char excmsg[LYCITEA_MAX_MSG_EXCEPTION];
                snprintf(excmsg, LYCITEA_MAX_MSG_EXCEPTION, "Cannot register two routes matching \"%s\" for method \"%s\"", Z_STRVAL_P(routeStr), Z_STRVAL_P(entry));
                lycitea_throw_exception(LYCITEA_ERR_ROUTE_FAILED, excmsg);
                break;
            }
            zval *rRoutes = zend_read_property(Z_OBJCE_P(obj), obj, ZEND_STRL(LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_REGEXROUTE), 1, NULL);
            zval *rRoute = zend_hash_str_find(Z_ARRVAL_P(rRoutes), Z_STRVAL_P(entry), Z_STRLEN_P(entry));
            if(NULL != rRoute && IS_ARRAY == Z_TYPE_P(rRoute)){
                zend_string *key = NULL;
                ZEND_HASH_FOREACH_STR_KEY(Z_ARRVAL_P(rRoute), key){
                    u_short isMatch = lycitea_route_simple_matches(key, routeStr);
                    if(IS_TRUE == isMatch){
                        char excmsg[LYCITEA_MAX_MSG_EXCEPTION];
                        snprintf(excmsg, LYCITEA_MAX_MSG_EXCEPTION, "Static route \"%s\" is shadowed by previously defined variable route \"%s\" for method \"%s\"", Z_STRVAL_P(routeStr), ZSTR_VAL(key), Z_STRVAL_P(entry));
                        lycitea_throw_exception(LYCITEA_ERR_ROUTE_FAILED, excmsg);
                        break;
                    }
                }ZEND_HASH_FOREACH_END();
            }
        }
        lycitea_helpers_common_depthadd(2, "ss", handler, sRoutes, Z_STRVAL_P(entry), Z_STRVAL_P(routeStr));
    }ZEND_HASH_FOREACH_END();

}

void lycitea_route_simple_add_regexroute(zval *httpMethods, zval *routeDatas, zval *handler, zval *obj)
{
    zval *entry = NULL, variables;
    smart_str regex = {0};
    array_init(&variables);
    ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(routeDatas), entry){
        if(IS_STRING == Z_TYPE_P(entry)){
            zval fn_return, fn_pregQuote, p2;
            ZVAL_STRING(&fn_pregQuote, "preg_quote");
            ZVAL_STRING(&p2, "~");
            zval fn_params[2];
            fn_params[0] = *entry;
            fn_params[1] = p2;
            call_user_function_ex(&EG(function_table), NULL, &fn_pregQuote, &fn_return, 2, fn_params, 1, NULL);
            smart_str_appendl(&regex, Z_STRVAL_P(&fn_return), Z_STRLEN_P(&fn_return));
            zval_ptr_dtor(&fn_return);
            zval_ptr_dtor(&fn_pregQuote);
            zval_ptr_dtor(&p2);
            continue;
        }
        zval *varName = zend_hash_index_find(Z_ARRVAL_P(entry), 0);
        zval *regexPart = zend_hash_index_find(Z_ARRVAL_P(entry), 1);
        zval *varValue = zend_hash_str_find(Z_ARRVAL(variables), Z_STRVAL_P(varName), Z_STRLEN_P(varName));
        if(NULL != varValue){
            char excmsg[LYCITEA_MAX_MSG_EXCEPTION];
            snprintf(excmsg, LYCITEA_MAX_MSG_EXCEPTION, "Cannot use the same placeholder \"%s\" twice", Z_STRVAL_P(varName));
            lycitea_throw_exception(LYCITEA_ERR_ROUTE_FAILED, excmsg);
            break;
        }

        if(NULL != strchr(Z_STRVAL_P(regexPart), ')')){
            char *checkRegex = "~"
                               "(?:"
                               "\\(\\?\\("
                               "| \\[ [^\\]\\\\\\\\]* (?: \\\\\\\\ . [^\\]\\\\\\\\]* )* \\]"
                               "| \\\\\\\\ ."
                               ") (*SKIP)(*FAIL) |"
                               "\\("
                               "(?!"
                               "\\? (?! <(?![!=]) | P< | \\' )"
                               "| \\*"
                               ")"
                               "~x";
            zend_string *zsregex = zend_string_init(checkRegex, strlen(checkRegex), 0);
            pcre_cache_entry *pce;
            zval matches, subpats;
            ZVAL_NULL(&subpats);
            if ((pce = pcre_get_compiled_regex_cache(zsregex)) != NULL) {
                php_pcre_match_impl(pce, Z_STRVAL_P(regexPart), Z_STRLEN_P(regexPart), &matches, &subpats, 0, 0, 0, 0);
            }
            zend_string_release(zsregex);
            zval_ptr_dtor(&subpats);
            if (Z_LVAL(matches)) {
                char excmsg[LYCITEA_MAX_MSG_EXCEPTION];
                snprintf(excmsg, LYCITEA_MAX_MSG_EXCEPTION, "Regex \"%s\" for parameter \"%s\" contains a capturing group", Z_STRVAL_P(regexPart), Z_STRVAL_P(varName));
                lycitea_throw_exception(LYCITEA_ERR_ROUTE_FAILED, excmsg);
                break;
            }
        }
        zval varField;
        ZVAL_COPY(&varField, varName);
        add_assoc_zval(&variables, Z_STRVAL(varField), &varField);
        smart_str prefix = {0};
        smart_str_appendc(&prefix, '(');
        smart_str_appendl(&prefix, Z_STRVAL_P(regexPart), Z_STRLEN_P(regexPart));
        smart_str_appendc(&prefix, ')');
        smart_str_append_smart_str(&regex, &prefix);
        smart_str_free(&prefix);
    }ZEND_HASH_FOREACH_END();
    smart_str_0(&regex);
    zval *runMode = zend_read_property(Z_OBJCE_P(obj), obj, ZEND_STRL(LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_RUNMODE), 1, NULL);
    zval *rRoutes = zend_read_property(Z_OBJCE_P(obj), obj, ZEND_STRL(LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_REGEXROUTE), 1, NULL);
    ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(httpMethods), entry){
        if(Z_LVAL_P(runMode) == LYCITEA_ROUTE_SIMPLE_RUN_STRICT){
            zval regexMap = lycitea_helpers_common_depthfind(2, "ss", rRoutes, Z_STRVAL_P(entry), regex.s->val);
            if(IS_NULL != Z_TYPE(regexMap)){
                char excmsg[LYCITEA_MAX_MSG_EXCEPTION];
                snprintf(excmsg, LYCITEA_MAX_MSG_EXCEPTION, "Cannot register two routes matching \"%s\" for method \"%s\"", regex.s->val, Z_STRVAL_P(entry));
                lycitea_throw_exception(LYCITEA_ERR_ROUTE_FAILED, excmsg);
                zval_ptr_dtor(&variables);
                break;
            }
        }
        zval tmpArr;
        array_init(&tmpArr);
        add_assoc_zval(&tmpArr, "variables", &variables);
        add_assoc_zval(&tmpArr, "handler", handler);
        lycitea_helpers_common_depthadd(2, "ss", &tmpArr, rRoutes, Z_STRVAL_P(entry), regex.s->val);
    }ZEND_HASH_FOREACH_END();
    smart_str_free(&regex);

}


void lycitea_route_simple_parse_placeholders(char *route, zval *placeholders) {

    char *pattern = "~"
                    ROUTER_VARIABLE_REGEX
                    "~x";
    zend_string *s_pattern = zend_string_init(pattern, strlen(pattern), 0);
    pcre_cache_entry *pce;
    if ((pce = pcre_get_compiled_regex_cache(s_pattern)) == NULL) {
        zend_string_release(s_pattern);
        return;
    }
    zend_string_release(s_pattern);
    zval matches, subpats, z_route;
    ZVAL_NULL(&subpats);
    ZVAL_STRING(&z_route, route);
    php_pcre_match_impl(pce, route, strlen(route), &matches, &subpats, 1, 1, ROUTER_PREG_OFFSET_CAPTURE | ROUTER_PREG_SET_ORDER, 0);

    if (!zend_hash_num_elements(Z_ARRVAL(subpats))) {
        add_next_index_zval(placeholders, &z_route);
        zval_ptr_dtor(&subpats);
        return;
    }
    zval offset, fn_substr;
    ZVAL_LONG(&offset, 0);
    ZVAL_STRING(&fn_substr, "substr");

    if (IS_ARRAY == Z_TYPE_P(&subpats)) {
        zval defRegex, *entry;
        ZVAL_STRING(&defRegex, ROUTER_DEFAULT_REGEX);
        int is_use_deferegex = 0;
        ZEND_HASH_FOREACH_VAL(Z_ARRVAL(subpats), entry)
        {
            zval subArr, fn_return;
            array_init(&subArr);
            zval zeroOne = lycitea_helpers_common_depthfind(2, "ll", entry, 0, 1);
            if (Z_LVAL(zeroOne) > Z_LVAL(offset)) {
                zval diffVal;
                ZVAL_LONG(&diffVal, Z_LVAL(zeroOne) - Z_LVAL(offset));
                zval fn_params[3];
                fn_params[0] =  z_route;
                fn_params[1] =  offset;
                fn_params[2] =  diffVal;
                call_user_function_ex(&EG(function_table), NULL, &fn_substr, &fn_return, 3, fn_params, 1, NULL);
                add_next_index_zval(placeholders, &fn_return);
            }
            zval oneZero = lycitea_helpers_common_depthfind(2, "ll", entry, 1, 0);
            zval_copy_ctor_func(&oneZero);
            add_next_index_zval(&subArr, &oneZero);

            zval twoZero = lycitea_helpers_common_depthfind(2, "ll", entry, 2, 0);
            if (IS_STRING == Z_TYPE(twoZero)) {
                zval_copy_ctor_func(&twoZero);
                add_next_index_zval(&subArr, &twoZero);
            } else {
                is_use_deferegex = 1;
                add_next_index_zval(&subArr, &defRegex);
            }
            add_next_index_zval(placeholders, &subArr);

            zval zeroZero = lycitea_helpers_common_depthfind(2, "ll", entry, 0, 0);
            Z_LVAL(offset) = Z_LVAL(zeroOne) + Z_STRLEN(zeroZero);

        }ZEND_HASH_FOREACH_END();
        zval_ptr_dtor(&subpats);
        if(!is_use_deferegex){
            zval_ptr_dtor(&defRegex);
        }
    } else {
        add_next_index_string(placeholders, route);
        goto clean;
    }

    if (Z_LVAL(offset) != Z_STRLEN(z_route)) {
        zval fn_return;
        zval fn_params[] = {z_route, offset};
        call_user_function_ex(&EG(function_table), NULL, &fn_substr, &fn_return, 2, fn_params, 1, NULL);
        add_next_index_zval(placeholders, &fn_return);
    }

    clean:
    zval_ptr_dtor(&z_route);
    zval_ptr_dtor(&fn_substr);

}

void  lycitea_route_simple_parse(zval *route, zval *return_value)
{
    zend_string *cutRoute = php_trim(Z_STR_P(route), ZEND_STRL("]"), 2);
    zend_long numOptionals = Z_STRLEN_P(route) - ZSTR_LEN(cutRoute);
    char *pattern = "~"
                    ROUTER_VARIABLE_REGEX
                    "(*SKIP)(*F) | \\[~x";
    zend_string *regex = zend_string_init(pattern, strlen(pattern), 0);
    pcre_cache_entry *pce;
    zval chars;
    if ((pce = pcre_get_compiled_regex_cache(regex)) != NULL) {
#if PHP_VERSION_ID >= 70200
        php_pcre_split_impl(pce, cutRoute, &chars, -1, 0);
#else
        php_pcre_split_impl(pce, ZSTR_VAL(cutRoute), ZSTR_LEN(cutRoute), &chars, -1, 0);
#endif
    }
    zend_string_release(regex);
    if (numOptionals != (zend_array_count(Z_ARR(chars)) - 1)) {
        zval matches, subpats;

        char *pattern = "~"
                        ROUTER_VARIABLE_REGEX
                        "(*SKIP)(*F) | \\]~x";
        zend_string *regex = zend_string_init(pattern, strlen(pattern), 0);
        pcre_cache_entry *pce;
        zval chars;
        if ((pce = pcre_get_compiled_regex_cache(regex)) != NULL) {
        }
        zend_string_release(regex);
        php_pcre_match_impl(pce, ZSTR_VAL(cutRoute), ZSTR_LEN(cutRoute), &matches, &subpats, 0, 0, 0, 0);
        char excmsg[LYCITEA_MAX_MSG_EXCEPTION];
        if (Z_LVAL(matches)) {
            snprintf(excmsg, LYCITEA_MAX_MSG_EXCEPTION, "Optional segments can only occur at the end of a route");
        }else{
            snprintf(excmsg, LYCITEA_MAX_MSG_EXCEPTION, "Number of opening '[' and closing ']' does not match");
        }
        lycitea_throw_exception(LYCITEA_ERR_ROUTE_FAILED, excmsg);
        zval_ptr_dtor(&chars);
        zval_ptr_dtor(&matches);
        zval_ptr_dtor(&subpats);
        zend_string_release(cutRoute);
        return;
    }
    zend_string_release(cutRoute);

    ulong num_key;
    zval *entry;
    smart_str currentRoute = {0};
    ulong rc_size = 0;
    ZEND_HASH_FOREACH_NUM_KEY_VAL(Z_ARRVAL(chars), num_key, entry)
    {
        if (Z_STRLEN_P(entry) == 0 && num_key != 0) {
            char excmsg[LYCITEA_MAX_MSG_EXCEPTION];
            snprintf(excmsg, LYCITEA_MAX_MSG_EXCEPTION, "Empty optional part");
            lycitea_throw_exception(LYCITEA_ERR_ROUTE_FAILED, excmsg);
            zval_ptr_dtor(return_value);
            array_init(return_value);
            break;
        }
        rc_size += strlen(Z_STRVAL_P(entry));
        smart_str_appendl(&currentRoute, Z_STRVAL_P(entry), Z_STRLEN_P(entry));
        smart_str_0(&currentRoute);
        zval placeholders;
        array_init(&placeholders);
        lycitea_route_simple_parse_placeholders(currentRoute.s->val, &placeholders);
        add_next_index_zval(return_value, &placeholders);

    }ZEND_HASH_FOREACH_END();
    zval_ptr_dtor(&chars);
    smart_str_free(&currentRoute);

}

void lycitea_route_simple_add_route(zval *httpMethod, zval *route, zval *handler, zend_long mode, zval *obj)
{

    zval *prefix = zend_read_property(Z_OBJCE_P(obj), obj, ZEND_STRL(LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_PREFIX), 0, NULL);
    lycitea_helpers_common_zvalcat(prefix, route, route, 1);
    zval routeDatas;
    array_init(&routeDatas);
    if(mode == LYCITEA_ROUTE_SIMPLE_STATIC){
        lycitea_helpers_common_depthadd(2, "ll", route, &routeDatas, 0, 0);
    }else{
        lycitea_route_simple_parse(route, &routeDatas);
    }
    if(IS_STRING == Z_TYPE_P(httpMethod)){
        convert_to_array(httpMethod);
    }
    if(IS_ARRAY == Z_TYPE_P(httpMethod)){
        zval *entry;
        ZEND_HASH_FOREACH_VAL(Z_ARRVAL(routeDatas), entry){
            if(mode == LYCITEA_ROUTE_SIMPLE_STATIC){
                lycitea_route_simple_add_staticroute(httpMethod, entry, handler, obj);
            }else{
                if(zend_array_count(Z_ARRVAL_P(entry)) == 1){
                    lycitea_route_simple_add_staticroute(httpMethod, entry, handler, obj);
                }else{
                    lycitea_route_simple_add_regexroute(httpMethod, entry, handler, obj);
                }
            }
        }ZEND_HASH_FOREACH_END();
    }
    zval_ptr_dtor(&routeDatas);
}

void lycitea_route_simple_get_data(zval *obj, zval *return_value)
{

    zval *sRoutes = zend_read_property(Z_OBJCE_P(obj), obj, ZEND_STRL(LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_STATICROUTE), 1, NULL);
    zval *rRoutes = zend_read_property(Z_OBJCE_P(obj), obj, ZEND_STRL(LYCITEA_ROUTE_SIMPLE_PROPERTY_NAME_REGEXROUTE), 1, NULL);
    zval_add_ref(sRoutes);
    add_next_index_zval(return_value, sRoutes);
    if(IS_NULL == Z_TYPE_P(rRoutes)){
        zval emptyArr;
        array_init(&emptyArr);
        add_next_index_zval(return_value, &emptyArr);
        return;
    }
    zval *entry;
    zend_string *key;

    zval rdata;
    array_init(&rdata);

    ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(rRoutes), key, entry){
        zend_long count = zend_array_count(Z_ARRVAL_P(entry));
        double numParts =  MAX(1, _php_math_round(count/10,  0, 1));
        zend_long chunkSize = (int)ceil(count / numParts);
        zval emptyArr;
        array_init(&emptyArr);
        zval *sentry, routeMap;
        zend_string *skey;
        int i = 1, numGroups = 0;
        array_init(&routeMap);
        smart_str regex= {0};
        ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(entry), skey, sentry){
            zval *variables = zend_hash_str_find(Z_ARRVAL_P(sentry), ZEND_STRL("variables"));
            zval_add_ref(variables);
            zval *handler = zend_hash_str_find(Z_ARRVAL_P(sentry), ZEND_STRL("handler"));
            zend_long numVariables = zend_array_count(Z_ARRVAL_P(variables));
            numGroups = MAX(numGroups, numVariables);
            zend_long repeatNum = MAX(numGroups - numVariables, 0);
            smart_str_appendl(&regex, ZSTR_VAL(skey), ZSTR_LEN(skey));
            int j = 0;
            for (j = 0; j < repeatNum; j++) {
                smart_str_appendl(&regex, "()", strlen("()"));
            }
            smart_str_0(&regex);
            zval tmpArr;
            array_init(&tmpArr);
            add_next_index_zval(&tmpArr, handler);
            add_next_index_zval(&tmpArr, variables);
            add_index_zval(&routeMap, numGroups+1, &tmpArr);
            numGroups++;
            if( !(i % chunkSize) ||  i == count){
                smart_str regexes = {0};
                smart_str_appendl(&regexes, "~^(?|", strlen("~^(?|"));
                smart_str_append_smart_str(&regexes, &regex);
                smart_str_appendl(&regexes, ")$~", strlen(")$~"));
                smart_str_0(&regexes);
                zval tmpArr;
                array_init(&tmpArr);
                add_assoc_string(&tmpArr, "regex", regexes.s->val);
                add_assoc_zval(&tmpArr, "routeMap", &routeMap);
                add_next_index_zval(&emptyArr, &tmpArr);
                array_init(&routeMap);
                smart_str_free(&regex);
                smart_str_free(&regexes);
                numGroups = 0;
            }else{
                smart_str_appendc(&regex,'|');
            }
            i++;
        }ZEND_HASH_FOREACH_END();
        add_assoc_zval(&rdata, ZSTR_VAL(key), &emptyArr);
        zval_ptr_dtor(&routeMap);
    }ZEND_HASH_FOREACH_END();
    add_next_index_zval(return_value, &rdata);

}











