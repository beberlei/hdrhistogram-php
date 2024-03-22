PHP_ARG_WITH(hdrhistogram,
    [if to enable the "hdrhistogram" extension"],
    [  --with-hdrhistogram[=DIR]    Enable "hdrhistogram" extension support])

if test "$PHP_HDRHISTOGRAM" != "no"; then
    PHP_ARG_WITH(hdrhistogram_static,
        [whether libhdr_histogram should be linked statically],
        [  --with-hdrhistogram-static
                          Enable static linking of libhdr_histogram], no, no)

    if test "$PHP_HDRHISTOGRAM_STATIC" = "yes"; then
        LIBNAME=hdr_histogram_static
    else
        LIBNAME=hdr_histogram
    fi

    AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

    if test "$PHP_HDRHISTOGRAM" = "yes" -a -x "$PKG_CONFIG" && $PKG_CONFIG --exists $LIBNAME; then

        AC_MSG_CHECKING([for hdrhistogram from pkg-config])
        LIBHDR_CFLAGS=`$PKG_CONFIG $LIBNAME --cflags`
        LIBHDR_LIBDIR=`$PKG_CONFIG $LIBNAME --libs`
        LIBHDR_VERSON=`$PKG_CONFIG $LIBNAME --modversion`
        AC_MSG_RESULT(found $LIBHDR_VERSON)
        if $PKG_CONFIG hdr_histogram --atleast-version 0.11.7; then
            AC_DEFINE(HAVE_HDRHISTOGRAM_0_11_7,1,[ ])
        fi
        if $PKG_CONFIG hdr_histogram --atleast-version 0.11.4; then
            AC_DEFINE(HAVE_HDRHISTOGRAM_0_11_4,1,[ ])
        fi
        PHP_EVAL_LIBLINE($LIBHDR_LIBDIR, HDRHISTOGRAM_SHARED_LIBADD)
        PHP_EVAL_INCLINE($LIBHDR_CFLAGS)
    else
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

        AC_SEARCH_LIBS(hdr_init, $LIBNAME,
            [
                PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $HDRHISTOGRAM_PATH/$PHP_LIBDIR, HDRHISTOGRAM_SHARED_LIBADD)
                AC_DEFINE(HAVE_HDRHISTOGRAM,1,[ ])
            ],[
                AC_MSG_ERROR([wrong hdrhistogram lib version or lib not found])
            ],[
                -L$HDRHISTOGRAM_PATH/$PHP_LIBDIR -lm
            ]
        )
    fi

    PHP_SUBST(HDRHISTOGRAM_SHARED_LIBADD)

    PHP_NEW_EXTENSION(hdrhistogram, hdrhistogram.c, $ext_shared)
fi
