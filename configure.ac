#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT(lyricsFetcher, 0.1, ynwlgh@icloud.com)
AM_INIT_AUTOMAKE(lyricsFetcher,0.1)
AC_CONFIG_SRCDIR([src/main.cpp])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([gethostbyname memset socket strchr strstr])

AC_OUTPUT(Makefile src/Makefile) 

