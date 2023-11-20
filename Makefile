#######################################
# Makefile                            #
#######################################


PROG = tp5

all : $(PROG)

# Variables for file compilation
CC        =  gcc
CFLAGS    =  -g -Wall
CPPFLAGS  =  -DDEBUG
LDFLAGS   =  -g -lm

#pxmtopxm : pxmtopxm.o Util.o

tp5 : tp5.o imageFormationUtils.o -lm

clean :
	@rm -f *.o

cleanall : clean
	@rm -f $(PROG)

