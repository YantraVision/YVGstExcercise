# Note: plugindir is set in configure

##############################################################################
# TODO: change libgstplugin.la to something else, e.g. libmysomething.la     #
##############################################################################
plugin_LTLIBRARIES = libgstplugin.la libgstaudiofiltertemplateplugin.la libgstmyplugin.la

##############################################################################
# TODO: for the next set of variables, name the prefix if you named the .la, #
#  e.g. libmysomething.la => libmysomething_la_SOURCES                       #
#                            libmysomething_la_CFLAGS                        #
#                            libmysomething_la_LIBADD                        #
#                            libmysomething_la_LDFLAGS                       #
##############################################################################

## Plugin 1

# sources used to compile this plug-in
libgstplugin_la_SOURCES = gstplugin.c gstplugin.h

# compiler and linker flags used to compile this plugin, set in configure.ac
libgstplugin_la_CFLAGS = $(GST_CFLAGS)
libgstplugin_la_LIBADD = $(GST_LIBS)
libgstplugin_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libgstplugin_la_LIBTOOLFLAGS = --tag=disable-static

## Plugin 2 (audio filter example)

# sources used to compile this plug-in
libgstaudiofiltertemplateplugin_la_SOURCES = gstaudiofilter.c

# compiler and linker flags used to compile this plugin, set in configure.ac
libgstaudiofiltertemplateplugin_la_CFLAGS = $(GST_CFLAGS)
libgstaudiofiltertemplateplugin_la_LIBADD = $(GST_LIBS)
libgstaudiofiltertemplateplugin_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libgstaudiofiltertemplateplugin_la_LIBTOOLFLAGS = --tag=disable-static


## Plugin 3 (my filter)

# sources used to compile this plug-in
libgstmyplugin_la_SOURCES = gstmyplugin.c gstmyplugin.h

# compiler and linker flags used to compile this plugin, set in configure.ac
libgstmyplugin_la_CFLAGS = $(GST_CFLAGS)
libgstmyplugin_la_LIBADD = $(GST_LIBS)
libgstmyplugin_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libgstmyplugin_la_LIBTOOLFLAGS = --tag=disable-static

# headers we need but don't want installed
noinst_HEADERS = gstplugin.h gstmyplugin.h
