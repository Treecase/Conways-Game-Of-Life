CC=gcc

NAME=life

SRCDIR=src

OBJNAMES=util
OBJDIR=$(SRCDIR)/objs
OBJS=$(addprefix $(OBJDIR)/, $(addsuffix .o, $(OBJNAMES)))
OBJSRC=$(addprefix $(SRCDIR)/, $(addsuffix .c, $(OBJNAMES)))
OBJHEADERS=$(addprefix $(SRCDIR)/, $(addsuffix .h, $(OBJNAMES)))

HEADERNAMES=defs.h
HEADERS=$(addprefix $(SRCDIR)/, $(HEADERNAMES))

LIBS=SDL2


$(NAME) : $(SRCDIR)/main.c $(OBJS) $(HEADERS)
	$(CC) $(SRCDIR)/main.c $(OBJS) -o $(NAME) $(addprefix -l, $(LIBS))

$(OBJS) : $(OBJSRC) $(OBJHEADERS) $(HEADERS)
	$(CC) -c $(OBJSRC)
	mv $(addsuffix .o, $(OBJNAMES)) $(OBJDIR)

git :
	rm $(OBJS)

clean :
	rm $(OBJS) $(NAME)
