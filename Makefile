-include config.mk
CFLAGS=-DDEBUG -g

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

all: pputil.a

pputil.a: append_buf.o
	$(AR) rc $@ $^

test: append_buf_test 
	./append_buf_test

append_buf_test: test.o append_buf_test.o append_buf.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) $^ -o $@

clean:
	rm -rf *.o *.a *_test 

.PHONY: all clean test

