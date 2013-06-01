dnl $Id$
dnl config.m4 for extension base32

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(base32, for base32 support,
dnl Make sure that the comment is aligned:
dnl [  --with-base32             Include base32 support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(base32, whether to enable base32 support,
Make sure that the comment is aligned:
[  --enable-base32           Enable base32 support])

if test "$PHP_BASE32" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-base32 -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/base32.h"  # you most likely want to change this
  dnl if test -r $PHP_BASE32/$SEARCH_FOR; then # path given as parameter
  dnl   BASE32_DIR=$PHP_BASE32
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for base32 files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       BASE32_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$BASE32_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the base32 distribution])
  dnl fi

  dnl # --with-base32 -> add include path
  dnl PHP_ADD_INCLUDE($BASE32_DIR/include)

  dnl # --with-base32 -> check for lib and symbol presence
  dnl LIBNAME=base32 # you may want to change this
  dnl LIBSYMBOL=base32 # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $BASE32_DIR/lib, BASE32_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_BASE32LIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong base32 lib version or lib not found])
  dnl ],[
  dnl   -L$BASE32_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(BASE32_SHARED_LIBADD)

  PHP_NEW_EXTENSION(base32, base32.c, $ext_shared)
fi
