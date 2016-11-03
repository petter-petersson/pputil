-include config.mk
override CFLAGS+= -std=c99 -D_GNU_SOURCE

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

all: libpputil.a

libpputil.a: append_buf.o strndup_unescaped.o
	$(AR) rc $@ $^

test: append_buf_test strndup_unescaped_test 
	./append_buf_test
	./strndup_unescaped_test

strndup_unescaped_test: test.o strndup_unescaped_test.o strndup_unescaped.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) $^ -o $@

append_buf_test: test.o append_buf_test.o append_buf.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) $^ -o $@

clean:
	rm -rf *.o *.a *_test 

.PHONY: all clean test

