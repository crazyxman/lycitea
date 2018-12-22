dnl PHP_ARG_WITH(lycitea, for lycitea support,
dnl [  --with-lycitea             Include lycitea support])

PHP_ARG_ENABLE(lycitea, whether to enable lycitea support,
[  --enable-lycitea           Enable lycitea support])

if test -z "$PHP_DEBUG" ; then
    AC_ARG_ENABLE(debug, [--enable-debug compile with debugging system], [PHP_DEBUG=$enableval],[PHP_DEBUG=no] )
fi

if test "$PHP_LYCITEA" != "no"; then
  dnl Write more examples of tests here...

  dnl # get library FOO build options from pkg-config output
  dnl AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  dnl AC_MSG_CHECKING(for libfoo)
  dnl if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists foo; then
  dnl   if $PKG_CONFIG foo --atleast-version 1.2.3; then
  dnl     LIBFOO_CFLAGS=`$PKG_CONFIG foo --cflags`
  dnl     LIBFOO_LIBDIR=`$PKG_CONFIG foo --libs`
  dnl     LIBFOO_VERSON=`$PKG_CONFIG foo --modversion`
  dnl     AC_MSG_RESULT(from pkgconfig: version $LIBFOO_VERSON)
  dnl   else
  dnl     AC_MSG_ERROR(system libfoo is too old: version 1.2.3 required)
  dnl   fi
  dnl else
  dnl   AC_MSG_ERROR(pkg-config not found)
  dnl fi
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBDIR, LYCITEA_SHARED_LIBADD)
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)

  dnl # --with-lycitea -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/lycitea.h"  # you most likely want to change this
  dnl if test -r $PHP_LYCITEA/$SEARCH_FOR; then # path given as parameter
  dnl   LYCITEA_DIR=$PHP_LYCITEA
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for lycitea files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       LYCITEA_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$LYCITEA_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the lycitea distribution])
  dnl fi

  dnl # --with-lycitea -> add include path
  dnl PHP_ADD_INCLUDE($LYCITEA_DIR/include)

  dnl # --with-lycitea -> check for lib and symbol presence
  dnl LIBNAME=lycitea # you may want to change this
  dnl LIBSYMBOL=lycitea # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $LYCITEA_DIR/$PHP_LIBDIR, LYCITEA_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_LYCITEALIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong lycitea lib version or lib not found])
  dnl ],[
  dnl   -L$LYCITEA_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(LYCITEA_SHARED_LIBADD)



  PHP_NEW_EXTENSION(lycitea, lycitea.c                  \
    helpers/lycitea_helpers_common.c                    \
    helpers/lycitea_helpers_lru.c                       \
    caches/lycitea_cache_lru.c                          \
    lycitea_exception.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)

fi
