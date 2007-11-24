include ../common.mk

OBJS = example_3.o 
TARGET = example-3
LIBS = -L ../../mihl/$(CONFIG_NAME) -l mihl
CFLAGS += -I ../../mihl/Src
CFLAGS += $(if $(filter 1 YES, $(DEBUG)), -g -O0, -g0 -O2)
LDFLAGS += 
include ../compile.mk	
