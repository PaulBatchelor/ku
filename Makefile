OBJ = lodepng.o 

LDFLAGS = -lm -ldl -Wl,-export-dynamic 

CFLAGS += -fPIC 

default: ku.so

ku.so: ku.c  $(OBJ)
	gcc -shared -I. -fPIC ku.c -o ku.so $(OBJ)
%.o : %.c
	gcc -c $(CFLAGS) -o $@ $^

clean:
	rm -rf *.so *.png *.o
