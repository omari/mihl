include ../common.mk

OBJS = b64.o mihl.o tcp_utils.o 
TARGET = libmihl.so
LIBS = 
CFLAGS += $(if $(filter 1 YES, $(DEBUG)), -g -O0, -g0 -O2)
LDFLAGS += 
include ../compile.mk	
