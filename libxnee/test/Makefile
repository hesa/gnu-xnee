%.o:%.c
	gcc $(CFLAGS) $< -c -o $@


LIBTEST = libtest
TEST_SETGET = test_setget
SETGET_OBJS = test_setget.o test_all.o

BINS = $(TEST_SETGET)  $(LIBTEST) 
all: $(BINS)

CFLAGS = -g  -I../include -DHAVE_STDARG_H
LDFLAGS =  -L../src/ -lxnee -L/usr/X11R6/lib  -lX11 -lXtst  -lxnee



$(LIBTEST): libtest.o
	gcc -o $@ $< $(LDFLAGS)


$(TEST_SETGET): $(SETGET_OBJS)
	gcc -o $@ $(SETGET_OBJS) $(LDFLAGS)

test:
	./$(LIBTEST)
	./$(TEST_SETGET)


clean: 
	rm -f *.o $(BINS)
