CC=gcc

_LIBS=m mpfr
LIBS=$(addprefix -l,$(_LIBS))

CFLAGS=-O3 -Wall
LDFLAGS=-O3 $(LIBS)

SRCDIR=.
_SRCFILES=copi.c
SRCS=$(addprefix $(SRCDIR)/,$(_SRCFILES))

OBJDIR=.
_OBJFILES=$(_SRCFILES:%.c=%.o)
OBJS=$(addprefix $(OBJDIR)/,$(_OBJFILES))

DIRGUARD=@mkdir -p $(@D)

EXES=copi.exe

all: $(EXES)

%.exe: $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(DIRGUARD)
	$(CC) $< -c -o $@ $(CFLAGS)

.SECONDARY: $(OBJS)
.PHONY: clean

clean:
	rm $(OBJDIR)/*.o
	rm *.exe
