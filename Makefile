.PHONY: all
all: libmihl.so

CROSS=
CC = gcc
LD = gcc
CTAGS = /usr/bin/ctags 


%.d: %.c
	@set -e; $(CC) -MM $(CFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@

%.o: %.c
	@if [ -x $(CTAGS) ]; then \
                mkdir -p obj/ctags; \
		cd src && $(CTAGS) -f ../obj/ctags/$*.tags $*.c ; \
	fi
	@$(CC) $(CFLAGS) -o obj/$@ -c $< 2>&1 | tee obj/$*.err

# C Compiler and Linker flags
CFLAGS	= -Wall -std=c9x
LDFLAGS	=  

ifdef DEBUG
CFLAGS += -g -O0 -D DEBUG -D PRINT_TRACE
LDFLAGS += -g
else
CFLAGS += -g0 -O3
LDFLAGS +=
endif

vpath %.c src/
vpath %.h src/

LDFLAGS += 
CFLAGS += -I. $(DEBUG_FLAGS)


# ----------------
# MIHL library
# ----------------

OBJS =	mihl tcp_utils
LIST_OBJS = $(foreach w,$(OBJS),obj/$(w).o)

libmihl.so: $(foreach w,$(OBJS),$(w).o) 
	@$(CC) $(LDFLAGS) -shared -o obj/libmihl.so $(LIST_OBJS)
	@if [ -x $(CTAGS) ] ; then \
		cat obj/ctags/*.tags > src/tags; \
	fi

-include $(LIST_OBJS:.o=.d)


.PHONY: install

ifndef DESTDIR
DESTDIR = /usr/bin
endif

#install:
#	install -D atilo $(DESTDIR)/usr/bin/atilo
#	strip $(DESTDIR)/usr/bin/atilo


# --------
# Clean-up
# --------

.PHONY: clean
clean: 
	@$(RM) *.o *.d *.err *.core core *~ cscope.out cscope.in.out cscope.po.out
	@+cd obj ; $(RM) *.err *.o *.so
	@+cd obj/ctags ; $(RM) *.tags
	@+cd src ; $(RM) cscope.out cscope.in.out cscope.po.out tags
