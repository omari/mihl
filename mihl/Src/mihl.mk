include ../common.mk

OBJS = b64.o mihl.o tcp_util.o 
TARGET = mihl
LIBS = 
CFLAGS += $(if $(filter 1 YES, $(DEBUG)), -g -O0, -g0 -O2)
LDFLAGS += 
include ../compile.mk	
