#
# PROGRAM:    Create mini shell 
# PROGRAMMER: Jonathon Molley
#

CC = g++
CCFLAGS = -Wall
EXE = microshell
SOURCE = microshell.c
OBJ = microshell.o

$(EXE): $(OBJ)
	$(CC) $(CCFLAGS) -o $(EXE) $(OBJ)
         
$(OBJ): $(SOURCE)
	$(CC) $(CCFLAGS) -c $(SOURCE)

clean:
	- rm *.o $(EXE)
 

  
