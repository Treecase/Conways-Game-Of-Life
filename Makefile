CC=gcc

NAME=life

SRCDIR=src

OBJNAMES=util graphics option
OBJDIR=$(SRCDIR)/objs
OBJS=$(addprefix $(OBJDIR)/, $(addsuffix .o, $(OBJNAMES)))
OBJSRC=$(addprefix $(SRCDIR)/, $(addsuffix .c, $(OBJNAMES)))
OBJHEADERS=$(addprefix $(SRCDIR)/, $(addsuffix .h, $(OBJNAMES)))

HEADERNAMES=defs.h
HEADERS=$(addprefix $(SRCDIR)/, $(HEADERNAMES))

LIBS=-lSDL2
CFLAGS=-g -Wall


$(NAME) : $(SRCDIR)/main.c $(OBJS) $(HEADERS)
	$(CC) $(SRCDIR)/main.c $(OBJS) -o $(NAME) $(LIBS) $(CFLAGS)

debug : $(SRCDIR)/*.c $(SRCDIR)/*.h
	$(CC) $(SRCDIR)/*.c -o $(NAME)_debug $(LIBS) $(CFLAGS)

$(OBJS) : $(OBJSRC) $(OBJHEADERS) $(HEADERS)
	$(CC) -c $(OBJSRC) $(CFLAGS)
	mv $(addsuffix .o, $(OBJNAMES)) $(OBJDIR)

software : $(SRCDIR)/main.c $(OBJS) $(HEADERS)
	$(CC) $(SRCDIR)/main.c $(OBJS) -o $(NAME) $(LIBS) $(CFLAGS)

clean :
	rm $(OBJS) $(NAME) $(NAME)_debug

