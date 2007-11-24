include ../common.mk

OBJS = example_1.o 
TARGET = example-1
LIBS = -L ../../mihl/$(CONFIG_NAME) -l mihl
CFLAGS += -I ../../mihl/Src
CFLAGS += $(if $(filter 1 YES, $(DEBUG)), -g -O0, -g0 -O2)
LDFLAGS += 
include ../compile.mk	
