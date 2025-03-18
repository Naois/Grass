CC:=g++
LIBS:= -lglew32 -lopengl32 -lglfw3
INC:=

main:
	$(CC) grass.cpp $(LIBS) $(INC) -o grass.exe