.PHONY: all
all: example3

CROSS=
CC = gcc
LD = gcc


%.d: %.c
	@set -e; $(CC) -MM $(CFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $< 2>&1 | tee $*.err

# C Compiler and Linker flags
CFLAGS	= -Wall 
LDFLAGS	=  

ifdef DEBUG
CFLAGS += -g -O0 -D DEBUG -D PRINT_TRACE
LDFLAGS += -g2
else
CFLAGS += -g0 -O3
LDFLAGS +=
endif

vpath %.c ./
vpath %.h ./

LDFLAGS += -L../../obj -lmihl
CFLAGS += -I. -I../../src $(DEBUG_FLAGS)


# ----------------
# 
# ----------------

OBJS1 =	example3
LIST_OBJS1 = $(foreach w,$(OBJS1),$(w).o)

example3: $(foreach w,$(OBJS1),$(w).o) 
	$(CC) $(LDFLAGS) -o $@ $(LIST_OBJS1)

-include $(LIST_OBJS1:.o=.d)


# --------
# Clean-up
# --------

.PHONY: clean
clean: 
	@$(RM) *.o *.d *.err *.core core *~ cscope.out cscope.in.out cscope.po.out example3
