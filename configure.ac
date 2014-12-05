# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ptrfuzzer], [0.0], [felicitashetzelt@gmail.com])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
#AC_PROG_INSTALL
#AC_PROG_LN_S
#AC_PROG_RANLIB

# automake initialization (mandatory) including a check for automake API version >= 1.9
AM_INIT_AUTOMAKE([1.9])

# Checks for libraries.
# FIXME: Replace `main' with a function in `-ldistorm3':
AC_CHECK_LIB([distorm3], [main])
# FIXME: Replace `main' with a function in `-lelf':
AC_CHECK_LIB([elf], [main])
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [main])
# FIXME: Replace `main' with a function in `-lutil':
AC_CHECK_LIB([util], [main])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdint.h stdlib.h string.h sys/time.h syslog.h unistd.h utmp.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([dup2 gettimeofday localtime_r memset mkdir select strerror strstr])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
