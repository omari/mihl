include ../common.mk

OBJS = example_1.o 
TARGET = libmihl.so
LIBS = -L ../mihl/$(CONFIG_NAME) -l mihl
CFLAGS += $(if $(filter 1 YES, $(DEBUG)), -g -O0, -g0 -O2)
LDFLAGS += 
include ../compile.mk	
