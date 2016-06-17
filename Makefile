CC = gcc
CFLAGS =  
LDFLAGS = -pthread
EXEC = exec
SRCDIR = src
OBJDIR = obj

all: $(EXEC)

$(OBJDIR)/main.o : $(SRCDIR)/main.c $(SRCDIR)/main.h $(SRCDIR)/train.h $(SRCDIR)/header_master.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJDIR)/communication.o : $(SRCDIR)/communication.c $(SRCDIR)/communication.h $(SRCDIR)/header_master.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJDIR)/manager.o : $(SRCDIR)/manager.c $(SRCDIR)/manager.h $(SRCDIR)/communication.h $(SRCDIR)/header_master.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJDIR)/train.o : $(SRCDIR)/train.c $(SRCDIR)/train.h $(SRCDIR)/communication.h $(SRCDIR)/header_master.h $(SRCDIR)/parser.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJDIR)/parse.o : $(SRCDIR)/parser.c $(SRCDIR)/parser.h $(SRCDIR)/header_master.h
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC): $(OBJDIR)/parse.o $(OBJDIR)/main.o $(OBJDIR)/communication.o $(OBJDIR)/manager.o $(OBJDIR)/train.o
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	rm -rf $(OBJ) $(EXEC)