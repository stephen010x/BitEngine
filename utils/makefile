# gcc options
# https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html
# ld options
# https://sourceware.org/binutils/docs/ld/Options.html


TARGET := utils

TMPDIR := tmp
SRCDIR := src
INCDIR := include
BINDIR := bin


WINDOWS_LIBS :=
LINUX_LIBS   :=
# TODO: consider renaming "wasm" to "web"
WASM_LIBS    :=


CC := gcc
CFLAGS := -DENABLE_BITWIN_TEST_C -std=gnu23 -fvisibility=internal
LFLAGS := -no-pie 
BFLAGS := -Wall -Wpedantic -Wextra

DB := gdb
DBFLAGS :=

AR := ar
ARFLAGS := rcs


FAST_CFLAGS := -D__DEBUG__ -D__debug__
FAST_LFLAGS :=
FAST_BFLAGS := -g -O0 

DEBUG_CFLAGS := -D__DEBUG__ -D__debug__
DEBUG_LFLAGS :=
DEBUG_BFLAGS := -g -Og

RELEASE_CFLAGS :=
# place each function and data into its own section so that they can get removed by --gc-sections if unused
RELEASE_LFLAGS := -s -ffunction-sections -fdata-sections -Wl,--gc-sections
RELEASE_BFLAGS := -Os -g0


STATIC_CFLAGS := -D__STATIC_LIB__
STATIC_LFLAGS := 
STATIC_BFLAGS := 


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



DEFAULT := static_release
WHITELIST := all fast release debug static dynamic 
WHITELIST := $(WHITELIST) static_fast static_debug static_release
WHITELIST := $(WHITELIST) dynamic_fast dynamic_debug dynamic_release


# ============================================
# ============================================


# so the first word is the goal, and subsiquent words are modifiers
# the goal is basically optimization target, but subsiquent goal can be static
GOAL := $(firstword $(MAKECMDGOALS))
GOAL := $(if $(GOAL),$(GOAL),$(DEFAULT))
# should grab all sources relative to makefile.
# colon equal (:=) removed, so as to grab all generated c files as well
SRCS = $(shell find $(SRCDIR) -name "*.c")
INCS = $(shell find $(INCDIR) -name "*.h")
ifneq ($(filter $(GOAL),$(WHITELIST)),)
	OBJS := $(SRCS:%.c=$(TMPDIR)/$(GOAL)/%.o)
	DEPS := $(SRCS:%.c=$(TMPDIR)/$(GOAL)/%.d)
endif



#BINGOAL :=  $(subst dynamic_,,$(subst static_,,$(GOAL)))
#BINTARG := $(BINDIR)/$(BINGOAL)/$(TARGET)
BINTARG := $(BINDIR)/$(TARGET)
LIBBINTARG := $(BINDIR)/lib$(TARGET)
#INCTARG := $(BINDIR)/$(INCLUDEE)


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

all: $(DEFAULT)
fast: _fast $(BINTARG)
debug: _debug $(BINTARG)
release: _release $(BINTARG)

static: static_fast
static_fast: _static _fast $(LIBBINTARG).a
static_debug: _static _debug $(LIBBINTARG).a
static_release: _static _release $(LIBBINTARG).a


-include $(DEPS)


_fast:
	$(eval GOAL := fast)
	$(eval CFLAGS += $(FAST_CFLAGS))
	$(eval LFLAGS += $(FAST_LFLAGS))
	$(eval BFLAGS += $(FAST_BFLAGS))

_debug:
	$(eval GOAL := debug)
	$(eval CFLAGS += $(DEBUG_CFLAGS))
	$(eval LFLAGS += $(DEBUG_LFLAGS))
	$(eval BFLAGS += $(DEBUG_BFLAGS))

_release:
	$(eval GOAL := release)
	$(eval CFLAGS += $(RELEASE_CFLAGS))
	$(eval LFLAGS += $(RELEASE_LFLAGS))
	$(eval BFLAGS += $(RELEASE_BFLAGS))	

_static:
	$(eval CFLAGS += $(STATIC_CFLAGS))
	$(eval LFLAGS += $(STATIC_LFLAGS))
	$(eval BFLAGS += $(STATIC_BFLAGS))



#=========================================#
#      LINUX                              #
#=========================================#
ifdef LINUX


CFLAGS += $(LINUX_CFLAGS)
LFLAGS += $(LINUX_LFLAGS)
BFLAGS += $(LINUX_BFLAGS)


$(BINTARG): $(OBJS)
	mkdir -p $(dir $@)
	#$(CC) $(BFLAGS) -o $@ -o $(basename $@) $^ -L$(LIBDIR) $(LFLAGS)
	$(CC) $(BFLAGS) -o $@ $^ -L$(LIBDIR) $(LFLAGS)

$(LIBBINTARG).a: $(OBJS)
	mkdir -p $(dir $@)
	$(AR) $(ARFLAGS) $@ $^

$(TMPDIR)/$(GOAL)/%.o: %.c $(TMPDIR)/$(GOAL)/%.d
	mkdir -p $(dir $@)
	$(CC) -I$(INCDIR)  $(BFLAGS) $(CFLAGS) -c $< -o $@

$(TMPDIR)/$(GOAL)/%.d: %.c
	mkdir -p $(dir $@)
	$(CC) -I$(INCDIR) $(BFLAGS) $(CFLAGS) -MM -MT $(patsubst %.d,%.o,$@) -MF $@ $<



clean:
	rm -rf $(TMPDIR)
	rm -rf $(BINDIR)


analyze:
	readelf -a $(BINTARG)


endif
#==========================================
#==========================================



.PHONY: all fast debug release static dynamic preprocess
.PHONY: static_fast static_debug static_release dynamic_fast dynamic_debug dynamic_release
.PHONY: clean test shaders run gdb analyze
.PHONY: _build _fast _debug _release _dynamic _static _optimize
