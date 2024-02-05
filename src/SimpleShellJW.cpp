/* Name: Jared Whytsell
   Project: SEED Lab Shell Program
   Class: CSCI 411 001
   Description: A shell program similar to Bash (and may use some of Bash's commands) made with the intent to understand how a shell program works.
*/
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <signal.h>
#include "../inc/Commands.h"
using namespace std;

// Arbitrary version and command history
//	0.7.0: Removed redundant stdin & stdout reservations
//		The shell now falls back to 
string version = "0.7.0";
ofstream cmdHistoryFile;
char* execDirectory;

// Command list (contains pointers to objects that represent commands)
map<string, Command*> cmd;
vector<Command*> cmdPointers;
void InitCmdList() {
	// When initialized, each command places itself into the command map.
	//	The vector places them into itself so that they can be deleted easily later.
	//	In C++, objects MUST be deleted manually.
	// Shell commands
	cmdPointers.push_back(new HelpCommand(cmd));
	cmdPointers.push_back(new QuitCommand(cmd));
	cmdPointers.push_back(new ClearCommand(cmd));
	cmdPointers.push_back(new DirCommand(cmd));
	cmdPointers.push_back(new MyProcessCommand(cmd));
	cmdPointers.push_back(new AllProcessesCommand(cmd));
	cmdPointers.push_back(new ChgdCommand(cmd));
	cmdPointers.push_back(new EnvironCommand(cmd));
	// Custom commands
	cmdPointers.push_back(new RepeatCommand(cmd));
	cmdPointers.push_back(new HiMomCommand(cmd));
}

// Calls when an interrupt occurs, or program termination is handled here as well.
void QuitShell(int signum) {
	// Stop writing the history file.
	if (cmdHistoryFile.is_open()) {
		cmdHistoryFile.close();
	}
	cout << '\n' << "Goodbye" << '\n';
	// Read command history from the file generated in shell's program directory
	//	But first change to the working directory that the executable started from... Using one of the commands I created!
	cmd["chgd"]->Execute(vector<string>(1, execDirectory));
	fstream fsHistory("cmd_history.txt");
	if (fsHistory.is_open()) {
		cout << "Command history:" << '\n';
		string line = "";
		while (getline(fsHistory, line)) {
			cout << line << '\n';
		}
		fsHistory.close();
		cout << "End of command history" << '\n';
	}
	else {
		cout << "There was a problem opening the command history." << '\n';
	}
	
	// Delete all pointers
	//	Since the vector list has the same objects as the map, the created objects should be properly deleted.
	//	Since we're leaving the program, no need to mark hangers null.
	for (int i = 0; i < cmdPointers.size(); i++) {
		Command* pointer = cmdPointers[i];
		delete pointer;
	}
	
	exit(0);
}

void InputLineInterpreter(string input) {
	// Even if it fails, log the command in the command history file.
	cmdHistoryFile << input << '\n';
	
	// First word before the whitespace is the command itself.
	string cmdIn = "";
	
	// Put the input into a string stream
	stringstream ssbuf(input);
	ssbuf >> cmdIn;

	// Parse args, if there are any.
	vector<string> args;
	string next = "";
	stringstream strArg("");
	while (ssbuf >> next) {
		if (next.at(0) == '"') {
			next.erase(0, 1);
			strArg << next;
			// For strings that terminate early
			if (next.at(next.size() - 1) == '"') {
				next.erase(next.size() - 1, 1);
				args.push_back(next);
				strArg.str("");
				strArg.clear();
			}
			else {
				strArg << " ";
			}
		}
		else if (next.at(next.size() - 1) == '"') {
			// For strings that terminate sometime later
			next.erase(next.size() - 1, 1);
			strArg << next;
			args.push_back(strArg.str());
			strArg.str("");
			strArg.clear();
		}
		else if (strArg.str() != "") {
			// Keep going with the string argument
			strArg << next << " ";
		}
		else {
			args.push_back(next);
		}
	}
	if (strArg.str() != "") {
		cout << "A string argument was not closed off with a quotation. Cannot execute command." << '\n';
		return;
	}
	
	// Execute an internal command, if it exists.
	if (cmd.find(cmdIn) != cmd.end()) {
		int code = cmd[cmdIn]->Execute(args);
		// Exec code 1 means that the command itself had an error.
		if (code == 1) {
			cout << '\n' << "There was a problem executing this command." << '\n';
		}
	}
	else if (cmdIn != "") {
		// Check if this command is an external command
		//	If it is, run that instead
		string cmdExternCheck = "which " + cmdIn + " > /dev/null";
		int cmdExternCheckNum = system(cmdExternCheck.c_str());
		//cout << cmdExternCheckNum << '\n';
		if (cmdExternCheckNum != 256) { //256 is the return value for which, that says the command doesn't exist.
			// The shell now falls back to system commands
			system(input.c_str());
		}
		else {
			// Unrecognized command (blank lines just form new indentation)
			cout << "No command named " << "'" << cmdIn << "'" << " found." << '\n';
		}
	}
}

int main() {
	//Get the working directory the executable was launched in.
	execDirectory = get_current_dir_name(); // From unistd.h
	
	// Connect the interupt signal to the exit method.
	signal(SIGINT, QuitShell);

	// Initialize commands
	InitCmdList();

	// Initial Prompt (clear bash first)
	system("clear");
	cout << "Welcome to Jared's shell program. Version " << version << '\n';
	cout << "Type " << "'" << "help" << "'" << " to see available commands." << '\n';
	// Begin writing to the history file
	cmdHistoryFile.open("cmd_history.txt");
	if (!cmdHistoryFile.is_open()) {
		cout << "The command history file could not be written to. Perhaps the program does not have write permissions here?" << '\n';
	}

	// Shell engine
	while (true) {
		cout << "SimpleShellJW: ";
		string cmdIn = "";
		getline(cin, cmdIn);
		InputLineInterpreter(cmdIn);
	}
	return 0;
}


