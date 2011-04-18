X11_DIR=/usr/X11R6/

X11_INC=-${X11_DIR}/include -I/usr/local/include
X11_LIB=${X11_DIR}/lib

OBJS=xgetter.o

BIN=xgetter

objs:
	gcc ${X11_INC}  -c xgetter.c -o xgetter.o

bin: 
	gcc  -o xgetter xgetter.o -L/usr/X11R6/lib -lX11

clean:
	-rm *.o ${BIN}


all: objs bin

