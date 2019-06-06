DB_FLAGS	= -Wall -g -ggdb 
#DB_FLAGS	= -O3

CFLAGS_GTK	= `pkg-config --cflags glib-2.0` `pkg-config --cflags gtk+-2.0`

INCLUDE		= 

INC_FLAGS	= $(INCLUDE)
LINK_FLAGS	= 
LINK_GTK	= `pkg-config --libs glib-2.0` `pkg-config --libs gtk+-2.0`
LINK_LIBS	= $(LINK_GTK) -lm

LOPTS		=

CFLAGS		= $(DB_FLAGS) $(INC_FLAGS) $(CFLAGS_GTK)
LFLAGS		= $(DB_FLAGS) $(LINK_FLAGS)

CC		= gcc
CP		= cp
RM		= rm
LS		= ls

OBJS		= 

EXEC		= 


all: comp link

comp: projecto.o

link: projecto

projecto.o: projecto.c
	$(CC) $(CFLAGS) -c projecto.c

projecto: projecto.o
	$(CC) $(LFLAGS) -o projecto projecto.o $(LINK_LIBS)

clean:
	$(RM) -f *~ *.o projecto
