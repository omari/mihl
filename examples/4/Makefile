.PHONY: all
all: example4

CROSS=
CC = gcc
LD = gcc


%.d: %.c
	@set -e; $(CC) -MM $(CFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< 2>&1 | tee $*.err

# C Compiler and Linker flags
CFLAGS	= -Wall -std=c9x
LDFLAGS	=  

ifdef DEBUG
CFLAGS += -g -O0 -D DEBUG -D PRINT_TRACE
LDFLAGS += -g2
else
CFLAGS += -g0 -O3
LDFLAGS +=
endif

# Add here links to your installation of FFMPEG
#CFLAGS += /usr/local/include
LDFLAGS += -lavutil -l avformat 

vpath %.c ./
vpath %.h ./

LDFLAGS += -L../../obj -lmihl
CFLAGS += -I. -I../../src $(DEBUG_FLAGS)


# ----------------
# 
# ----------------

OBJS4 =	example4
LIST_OBJS4 = $(foreach w,$(OBJS4),$(w).o)

example4: $(foreach w,$(OBJS4),$(w).o) 
	$(CC) $(LDFLAGS) -o $@ $(LIST_OBJS4)

-include $(LIST_OBJS4:.o=.d)


# --------
# Clean-up
# --------

.PHONY: clean
clean: 
	@$(RM) *.o *.d *.err *.core core *~ cscope.out cscope.in.out cscope.po.out example4
