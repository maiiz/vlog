TARGET = libvlog.so
CC = gcc
CFLAGS = -o3 -fPIC -c

all:$(TARGET)

INCLUDE = -I/usr/include
LD := -L/usr/lib 
LDS := -lpthread 
#INCLUDE := $(foreach, def, $(INCLUDE), -I$(def))
#LD := $(foreach, def, $(LD), -L$(def))
#LDS := $(foreach, def, $(LDS), -l$(def))

SRC = $(shell find ./ -maxdepth 1 -name "*.c")
OBJS = $(SRC:.c=.o)



$(TARGET):$(OBJS)
	$(CC) -o $@ $^ -fPIC -shared $(LD) $(LDS) 

%.d:%.c
	@set -e; rm -f $@;\ 
	$(CC) -M $(CFlAGS) $(INCLUDE) $< >; $@.$$$$;\ 
	sed 's,/($*/)/.o[ :]*,/1.o $@ : ,g' < $@.$$$$ >; $@;\ 
	rm -f $@.$$$$ 
	
%.o:%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ 
	
	
clean:
	rm -fr $(OBJS) $(TARGET)
