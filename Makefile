#Makefile par Pacôme Perrotin

#Dossier contenant les .c :
CREP = src
#Dossier contenant les .h :
HREP = src
#Dossier contenant les .o :
OREP = obj
#Dossier contenant les executables :
BREP = bin
#Nom de l'executable :
BIN = hmm
#Nom du compilateur :
CC = clang

INCLUDE = -I/usr/local/include -I/usr/include -lm
FLAGS = -L/usr/local/lib -L/usr/lib -g -Wall -std=c99

#Pour chaque a.c dans CREP, on considère le module a
RAW_LIB = $(shell echo $(CREP)/*.c)
LIB = $(RAW_LIB:$(CREP)/%.c=%)

CFILES = $(LIB:%=%.c)
HDR = $(LIB:%=%.h)

SRC = $(CFILES) $(HDR)

OBJ = $(LIB:%=%.o)
OBJ_OUT = $(OBJ:%=$(OREP)/%)

BIN_OUT = $(BIN:%=$(BREP)/%)

vpath %.c $(CREP)
vpath %.h $(HREP)
vpath %.o $(OREP)

all: $(SRC) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(FLAGS) $(OBJ_OUT) $(INCLUDE) -o $(BREP)/$@

%.o: %.c
	$(CC) $(FLAGS) $< $(INCLUDE) -c -o $(OREP)/$@

clear:
	-rm $(BIN_OUT)
	-rm $(OBJ_OUT)

