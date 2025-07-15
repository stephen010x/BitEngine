# gcc options
# https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html
# ld options
# https://sourceware.org/binutils/docs/ld/Options.html


TARGET := test

TMPDIR := tmp
SRCDIR := src
BINDIR := bin
LIBDIR := lib


INCLUDE :=
LIBS :=

WINDOWS_LIBS :=
LINUX_LIBS   :=
# TODO: consider renaming "wasm" to "web"
WASM_LIBS    :=


CC := gcc
# change fvisibility for when you turn this into a library
#CFLAGS := -fvisibility=hidden -DENABLE_BITWIN_TEST_C -std=c23
# NOTE: -std=c23 is strict c23 compliance, whereas -std=gnu23 includes gnu specific features
# Here are the extensions
# https://gcc.gnu.org/onlinedocs/gcc/C-Extensions.html
CFLAGS := -DENABLE_BITWIN_TEST_C -std=gnu23
# -static
#LFLAGS := -Wl,-nmagic
LFLAGS := -no-pie
BFLAGS := -Wall -Wpedantic -Wextra
#LIBS   :=

DB := gdb
DBFLAGS :=


FAST_CFLAGS := -D__DEBUG__
FAST_LFLAGS :=
FAST_BFLAGS := -g -O0 

DEBUG_CFLAGS := -D__DEBUG__
DEBUG_LFLAGS :=
DEBUG_BFLAGS := -g -Og

RELEASE_CFLAGS :=
# place each function and data into its own section so that they can get removed by --gc-sections if unused
# -Wl,-Bsymbolic
# remove or change --exclude-libs,ALL for when you turn this into a library
RELEASE_LFLAGS := -s -ffunction-sections -fdata-sections -Wl,--gc-sections,--exclude-libs,ALL
RELEASE_BFLAGS := -Os -g0


WINDOWS_CFLAGS := -D__WIN32__
WINDOWS_LFLAGS :=
WINDOWS_BFLAGS :=

LINUX_CFLAGS := -D__LINUX__
LINUX_LFLAGS := -lX11 -lrt
# dammit no!!!
#LINUX_BFLAGS := -mx32 -m32
LINUX_BFLAGS := -m32
#LINUX_LIBS   :=

WASM_CFLAGS := -D__WEBASM__
WASM_LFLAGS :=
WASM_BFLAGS :=



DEFAULT := fast
WHITELIST := all fast release debug


# ============================================
# ============================================

GOAL := $(firstword $(MAKECMDGOALS))
GOAL := $(if $(GOAL),$(GOAL),$(DEFAULT))
# should grab all sources relative to makefile.
# colon equal (:=) removed, so as to grab all generated c files as well
SRCS = $(shell find $(SRCDIR) -name "*.c")
ifneq ($(filter $(GOAL),$(WHITELIST)),)
	OBJS = $(SRCS:%.c=$(TMPDIR)/$(GOAL)/%.o)
	DEPS = $(SRCS:%.c=$(TMPDIR)/$(GOAL)/%.d)
endif
BINTARG := $(BINDIR)/$(TARGET)



ifeq ($(OS),Windows_NT) 
    $(error Winbows not yet implemented)
else
    OS := $(shell sh -c 'uname 2>/dev/null || echo Unknown')
    ifeq ($(OS),Linux)
    	LINUX := LINUX
    	#BLFLAGS += -target *-*-linux-gnu
    	#MAKEOSFILE := make/linux.mk
    else
		$(error Incompatable operating system)
    endif
endif



#==========================================
#==========================================


all: $(DEFAULT)
fast: _fast $(BINTARG)
debug: _debug $(BINTARG)
release: _release $(BINTARG)


-include $(DEPS)


# -sGL_DEBUG=1
_fast:
	$(eval CFLAGS += $(FAST_CFLAGS))
	$(eval LFLAGS += $(FAST_LFLAGS))
	$(eval BFLAGS += $(FAST_BFLAGS))

# -sGL_DEBUG=1
_debug:
	$(eval CFLAGS += $(DEBUG_CFLAGS))
	$(eval LFLAGS += $(DEBUG_LFLAGS))
	$(eval BFLAGS += $(DEBUG_BFLAGS))

# -Oz  ## smaller than -Os, but also slower
#  -sMODULARIZE
_release:
	$(eval CFLAGS += $(RELEASE_CFLAGS))
	$(eval LFLAGS += $(RELEASE_LFLAGS))
	$(eval BFLAGS += $(RELEASE_BFLAGS))

_optimize: _release
	



#=========================================#
#      LINUX                              #
#=========================================#
ifdef LINUX


CFLAGS += $(LINUX_CFLAGS)
LFLAGS += $(LINUX_LFLAGS)
BFLAGS += $(LINUX_BFLAGS)



$(BINTARG): $(OBJS)
	mkdir -p $(BINDIR)
	$(CC) $(BFLAGS) -o $@ -o $(basename $@) $^ $(LFLAGS)

$(TMPDIR)/$(GOAL)/%.o: %.c $(TMPDIR)/$(GOAL)/%.d
	mkdir -p $(dir $@)
	$(CC) $(INCLUDE) $(BFLAGS) $(CFLAGS) -c $< -o $@

$(TMPDIR)/$(GOAL)/%.d: %.c
	mkdir -p $(dir $@)
	$(CC) $(INCLUDE)  $(BFLAGS) $(CFLAGS) -MM -MT $(patsubst %.d,%.o,$@) -MF $@ $<


clean:
	rm -rf $(TMPDIR)
	rm -rf $(BINDIR)
	rm -rf src/shaders/gen


run:
	./$(BINTARG)


gdb:
	gdb -x script.gdb ./$(BINTARG)


analyze:
	readelf -a $(BINTARG)


endif
#==========================================
#==========================================




# Example rule behavior
#
# rule: dependancies
#	:
#
# I don't remember what this does, but it is useful, because rules without it behave differently



preprocess:
	gcc -DENABLE_BITWIN_TEST_C -D__DEBUG__ -D__LINUX__ -E src/linux/bitwin.c -o bin/bitwin.pp.c




# figure out what to do with this one
.PHONY: preprocess

.PHONY: all fast debug release static dynamic clean test chrome shaders run gdb analyze
.PHONY: _build _fast _debug _release _dynamic _static _optimize
