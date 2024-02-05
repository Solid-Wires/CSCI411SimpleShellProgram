PROGRAM=SimpleShellProgram

all: $(PROGRAM)

main: src/SimpleShellJW.cpp
	gcc -c src/SimpleShellJW.cpp -o obj/main.o

commands: src/ShellCommands.cpp src/CustomCommands.cpp
	gcc -c src/ShellCommands.cpp -o obj/shellcmd.o
	gcc -c src/CustomCommands.cpp -o obj/customcmd.o

$(PROGRAM): main commands
	g++ obj/main.o obj/shellcmd.o obj/customcmd.o -o $(PROGRAM)

.PHONY: clean
clean:
	rm -f $(PROGRAM)
	rm -f obj/*.o

