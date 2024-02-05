// Here are all of the built-in shell commands. Most borrow from the previous shell's commands.

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../inc/Commands.h"

// Should be the exact same vars from SimpleShellJW.cpp
extern vector<Command*> cmdPointers;
extern map<string, Command*> cmd;
extern void QuitShell(int signum); // This works for methods too???

// Command definition format
/*
// Example
string ExampleCommand::GetName() { return "example"; }
void ExampleCommand::PrintHelp() {
	
}
int ExampleCommand::Execute(vector<string> args) {
	
	return 0;
}
*/

// Help
string HelpCommand::GetName() { return "help"; }
void HelpCommand::PrintHelp() {
	cout << this->GetName() << ": You're using it right now..." << '\n';
	cout << "Displays information about command usage. Sometimes opens from the previous shell's manual if this shell's command derives from it." << '\n';
	cout << "Usage: " << this->GetName() << " [topic]" << '\n';
	cout << "\t no args - displays the commands available in the shell." << '\n';
	cout << "\t [topic] - describes the summary of the command usage, whilst outlining what all the arguments do. Underneath the usage information, arguments in brackets [] are optional, while arguments in tags <> are required." << '\n';
}
int HelpCommand::Execute(vector<string> args) {
	if (args.size() == 0) {
		// Help's greeting prompt that lists all literally available commands (all commands initialized)
		cout << "Welcome to help! Write 'help <topic>' to learn more about a particular command." << '\n';
		cout << "Do note that all of the commands here are case sensitive. They are all in lower-case." << '\n';
		cout << "The commands available in this shell are: ";
		for (int i = 0; i < cmdPointers.size(); i++) {
			cout << cmdPointers[i]->GetName();
			if (i < cmdPointers.size() - 1) {
				cout << ", ";
			}
		}
		cout << '\n';
	}
	else {
		// Display requested topic, given from arg0
		string topic = args[0];
		if (cmd.find(topic) != cmd.end()) {
			cmd[topic]->PrintHelp();
		}
		else {
			cout << "No help topic found for '" << topic << "' because command does not exist." << '\n';
		}
	}
	return 0;
}

// Quit
string QuitCommand::GetName() { return "quit"; }
void QuitCommand::PrintHelp() {
	cout << this->GetName() << ": simply terminates the shell and outputs the history file of all the commands executed during the session." << '\n';
	cout << "Usage: quit" << '\n';
}
int QuitCommand::Execute(vector<string> args) {
	// Basically exit(0)
	QuitShell(0); //Uses the signal handler function (externally identified) to do the same thing
	return 0;
}

// Clear
string ClearCommand::GetName() { return "clear"; }
void ClearCommand::PrintHelp() {
	// Open the manual for clear screen
	system("man clear");
}
int ClearCommand::Execute(vector<string> args) {
	system("clear"); // Uses the previous shell's command
	return 0;
}

// Dir
string DirCommand::GetName() { return "dir"; }
void DirCommand::PrintHelp() {
	// Open the manual for list contents in directory
	system("man ls");
}
int DirCommand::Execute(vector<string> args) {
	// Uses the previous shell's command
	if (args.size() == 0) {
		system("ls -al"); 
	}
	else {
		string cmdStr = "ls -al " + args[0];
		system(cmdStr.c_str()); 
	}
	return 0;
}

// MyProcess
string MyProcessCommand::GetName() { return "myprocess"; }
void MyProcessCommand::PrintHelp() {
	// Open the manual for getpid
	system("man getpid");
}
int MyProcessCommand::Execute(vector<string> args) {
	cout << "The pid of this process is " << getpid() << '\n';
	return 0;
}

// AllProcesses
string AllProcessesCommand::GetName() { return "allprocesses"; }
void AllProcessesCommand::PrintHelp() {
	// Open the manual for ps
	system("man ps");
}
int AllProcessesCommand::Execute(vector<string> args) {
	system("ps"); // Uses the previous shell's command
	return 0;
}

// Chgd
string ChgdCommand::GetName() { return "chgd"; }
void ChgdCommand::PrintHelp() {
	cout << this->GetName() << ": changes the current working directory." << '\n';
	cout << "Usage: "<< this->GetName() << " [directory]" << '\n';
	cout << "\t no args - the current directory remains the working directory. This command does nothing." << '\n';
	cout << "\t [directory] - changes the working directory to the directory specified. The working directory is where the shell is looking at." << '\n';
}
int ChgdCommand::Execute(vector<string> args) {
	if (args.size() > 0) {
		string newDir = args[0];
		int code = chdir(newDir.c_str());
		// chdir returns -1 if it couldn't reach a directory. Otherwise, 0 if successful.
		if (code == -1) {
			cout << "Cannot access '" << newDir << "': directory does not exist" << '\n';
		}
	}
	return 0;
}

// Environ
string EnvironCommand::GetName() { return "environ"; }
void EnvironCommand::PrintHelp() {
	cout << this->GetName() << ": lists all of the environment strings." << '\n';
	cout << "Usage: "<< this->GetName() << '\n';
}
int EnvironCommand::Execute(vector<string> args) {
	// I tried using the global variable environ, but it wasn't updating the path when I changed the directory.
	//	Getting it directly from the other shell updates, though.
	system("env");
	return 0;
}

