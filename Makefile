-include config.mk
override CFLAGS+= -std=c99 -D_GNU_SOURCE

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

all: libpputil.a

libpputil.a: append_buf.o strndup_unescaped.o mkdir_p.o log.o
	$(AR) rc $@ $^

test: append_buf_test strndup_unescaped_test log_test 
	./append_buf_test
	./strndup_unescaped_test
	./log_test

strndup_unescaped_test: test.o strndup_unescaped_test.o strndup_unescaped.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

append_buf_test: test.o append_buf_test.o append_buf.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

log_test: test.o log_test.o log.o mkdir_p.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)
clean:
	rm -rf *.o *.a *_test 

.PHONY: all clean test

