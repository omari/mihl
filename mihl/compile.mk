.PHONY: all clean

all: $(TARGET)

$(TARGET):	$(OBJS)
	$(CC) -shared -soname $(TARGET) -o $(TARGET) $(OBJS) $(LDFLAGS)

-include $(OBJS:.o=.d)

clean:
	@rm -rf *.o *.d *.err $(TARGET)
