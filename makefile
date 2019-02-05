# Universal MakeFile v1.1

# Declaration of variables
CC = g++
DB = gdb
RM = rm
MD = mkdir
CP = cp
DEBUG = true

#The Target Binary Program
ROOT_DIR = $(CURDIR);
THIS_DIR = $(shell basename "$(CURDIR)")
TARGET = $(THIS_DIR).exe

#PATHS
BASE_LIBS	= ..\..\mingw-dev-libs

#The Directories, Source, Includes, Objects, Binary and Resources
BUILDDIR	= obj
TARGETDIR	= bin
SRCDIR		= src
INCDIR		= inc
RESDIR		= res
SRCEXT		= cpp
DEPEXT		= d
OBJEXT		= o

#Flags, Libraries and Includes
CFLAGS		= -std=c++11
LIB 		=
INC 		= -I$(INCDIR)
#LIB			= -L$(BASE_LIBS)\SDL2-2.0.3\lib -lmingw32 -lgdi32 -lSDL2main -lSDL2 -static-libgcc
#INC			= -I$(INCDIR) -I$(BASE_LIBS)\SDL2-2.0.3\include\SDL2

#Condizioni
ifeq ($(DEBUG),true)
	CFLAGS += -Wall -O0 -g
else
	CFLAGS += -mwindows
endif

subdirs 	= $(wildcard $(SRCDIR)/*/)
SOURCES 	= $(wildcard $(SRCDIR)/*.$(SRCEXT)) $(wildcard $(addsuffix *.$(SRCEXT),$(subdirs)))
OBJECTS		= $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

#Defauilt Make
all: resources $(TARGET)
	@echo - Ho terminato la compilazione. Evviva!

#Remake
remake: clean all

#Copy Resources from Resources Directory to Target Directory
resources: directories
	@if [ -f $(RESDIR) ]; then @echo - Copio le cartelle risorse && @$(CP) $(RESDIR)/* $(TARGETDIR)/; fi

#Make the Directories
directories:
	@echo - Creo le directory target e build
	@$(MD) -p $(TARGETDIR)
	@$(MD) -p $(BUILDDIR)

#Clean only Objecst
clean:
	@echo - Eseguo una pulizia del target
	@$(RM) -rf $(BUILDDIR)
	@$(RM) -rf $(TARGETDIR)

#Link
$(TARGET): $(OBJECTS)
	@echo - Link alle librerie
	$(CC) $(CFLAGS) -o $(TARGETDIR)/$(TARGET) $(OBJECTS) $(LIB)

#Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@$(MD) -p $(dir $@)
	@echo - Compilo oggetto $@ da $<
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

run: all
	@echo - Eseguo il programma...
	@cd $(TARGETDIR) && ./$(TARGET)

debug: all
	@echo - Eseguo il programma in debug mode...
	@cd $(TARGETDIR) && $(DB) $(TARGET)

cls:
	@cls

#Non-File Targets
.PHONY: all remake clean resources directories run debug cls
