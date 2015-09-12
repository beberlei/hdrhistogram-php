PHP_ARG_WITH(hdrhistogram,
    [if to enable the "hdrhistogram" extension"],
    [  --with-hdrhistogram[=DIR]    Enable "hdrhistogram" extension support])

if test "$PHP_HDRHISTOGRAM" != "no"; then
    SEARCH_PATH="/usr/local /usr"
    SEARCH_FOR="/include/hdr/hdr_histogram.h"  # you most likely want to change this

    if test -r $PHP_HDRHISTOGRAM/$SEARCH_FOR; then
        HDRHISTOGRAM_PATH=$PHP_HDRHISTOGRAM
    else
        AC_MSG_CHECKING([for hdrhistogram files in default path])
        for i in $SEARCH_PATH ; do
            if test -r $i/$SEARCH_FOR; then
            HDRHISTOGRAM_PATH=$i
            AC_MSG_RESULT(found in $i)
        fi
        done
    fi

    if test -z "$HDRHISTOGRAM_PATH"; then
      AC_MSG_RESULT([not found])
      AC_MSG_ERROR([Please reinstall the hdrhistogram library])
    fi

    PHP_ADD_INCLUDE($HDRHISTOGRAM_PATH/include)

    LIBNAME=hdr_histogram
    LIBSYMBOL=hdr_init

    PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
        [
            PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $HDRHISTOGRAM_PATH/lib, HDRHISTOGRAM_SHARED_LIBADD)
            AC_DEFINE(HAVE_HDRHISTOGRAM,1,[ ])
        ],[
            AC_MSG_ERROR([wrong hdrhistogram lib version or lib not found])
        ],[
            -L$HDRHISTOGRAM_PATH
        ]
    )

    PHP_SUBST(HDRHISTOGRAM_SHARED_LIBADD)

    PHP_NEW_EXTENSION(hdrhistogram, hdrhistogram.c, $ext_shared)
fi
