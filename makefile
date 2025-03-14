CC:=g++
LIBS:= -lglew32 -lopengl32 -lglfw3
INC:=

main:
	$(CC) square.cpp $(LIBS) $(INC)