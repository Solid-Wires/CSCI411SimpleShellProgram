/* Name: Jared Whytsell
   Project: SEED Lab Shell Program
   Class: CSCI 411 001
   Description: A shell program similar to Bash (and may use some of Bash's commands) made with the intent to understand how a shell program works.
*/

// I kept this file to show you and me in the future on what I thought wasn't ideal for working with command instances.

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <signal.h>
#include <unistd.h>
using namespace std;

// Environment variables for program
char **environ;

// Arbitrary version and command history
string version = "0.2.0";
ofstream cmdHistoryFile;

// Calls when an interrupt occurs, or program termination is handled here as well.
void quitShell(int signum) {
	cmdHistoryFile.close();
	cout << '\n' << "Goodbye" << '\n';
	// Read command history from the file generated in shell's program directory
	cout << "Command history:" << '\n';
	fstream fsHistory("cmd_history.txt");
	string line = "";
	while (getline(fsHistory, line)) {
		cout << line << '\n';
	}
	fsHistory.close();
	cout << "End of command history" << '\n';
	
	exit(0);
}

// Commands available
//	I wanted to use a map, but this C++ version on the SEED Lab machine is outdated...
//	Therefore, I have to compensate with an array of commands identified in order.
//POSTNOTE: Yikes. That'll make a lot of if branches.
string cmd[] = {"help", "quit", "clr", "dir", "myprocess", "allprocesses", "chgd", "environ", "repeat", "hiMom"};
// Help command
void displayHelpTopic(string topic) {
	if (topic == cmd[0]) {
		cout << topic << ": You're using it right now..." << '\n';
		cout << "Displays information about command usage. Sometimes opens from the previous shell's manual if this shell's command derives from it." << '\n';
		cout << "Usage: " << topic << " [topic]" << '\n';
		cout << "\t no args - displays the commands available in the shell." << '\n';
		cout << "\t [topic] - describes the summary of the command usage, whilst outlining what all the arguments do. Underneath the usage information, arguments in brackets [] are optional, while arguments in tags <> are required." << '\n';
	}
	else if (topic == cmd[1]) {
		cout << topic << ": simply terminates the shell and outputs the history file of all the commands executed during the session." << '\n';
		cout << "Usage: quit" << '\n';
	}
	else if (topic == cmd[2]) {
		// Open the manual for clear screen
		system("man clear");
	}
	else if (topic == cmd[3]) {
		// Open the manual for list contents in directory
		system("man ls");
	}
	else if (topic == cmd[4]) {
		// Open the manual for getpid
		system("man getpid");
	}
	else if (topic == cmd[5]) {
		// Open the manual for ps
		system("man ps");
	}
	else if (topic == cmd[6]) {
		cout << topic << ": changes the current working directory." << '\n';
		cout << "Usage: "<< topic << " [directory]" << '\n';
		cout << "\t no args - the current directory remains the working directory. This command does nothing." << '\n';
		cout << "\t [directory] - changes the working directory to the directory specified The working directory is where the shell is looking at." << '\n';
	}
	else if (topic == cmd[7]) {
		cout << topic << ": lists all of the pointers to the environment strings." << '\n';
		cout << "Usage: "<< topic << '\n';
	}
	else if (topic == cmd[8]) {
		//TODO
	}
	else if (topic == cmd[9]) {
		//TODO
	}
}
// Execute given command by calling its appropriate function
//POSTNOTE: While nothing was wrong with half of this implementation, what I was thinking even then was: if only there was some way
//	I could just directly call the commands without having to parse the input with a bunch of if branches.
void execCmd(string input) {
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
	while (ssbuf >> next) {
		args.push_back(next);
	}

	// Check to see what command it is.
	//	For simplicity, the commands ignore additional args and rather expects  a minimum amount of args.
	if (cmdIn == cmd[0]) { // Help <topic>
		if (args.size() == 0) {
			cout << "Welcome to help! Write 'help <topic>' to learn more about a particular command." << '\n';
			cout << "The commands available in this shell are: ";
			for (int i = 0; i < sizeof(cmd)/sizeof(cmd[0]); i++) {
				cout << cmd[i];
				if (i < sizeof(cmd)/sizeof(cmd[0]) - 1) {
					cout << ", ";
				}
			}
			cout << '\n';
		}
		else {
			displayHelpTopic(args[0]);
		}
	}
	else if (cmdIn == cmd[1]) { // Quit
		quitShell(0); //Uses the signal handler function to do the same thing
	}
	else if (cmdIn == cmd[2]) { // Clear
		system("clear"); // Uses the previous shell's command
	}
	else if (cmdIn == cmd[3]) { // Dir
		// Uses the previous shell's command
		if (args.size() == 0) {
			system("ls -al"); 
		}
		else {
			string cmdStr = "ls -al " + args[0];
			system(cmdStr.c_str()); 
		}
	}
	else if (cmdIn == cmd[4]) { // Myprocess
		cout << "The pid of this process is " << getpid() << '\n';
	}
	else if (cmdIn == cmd[5]) { // Allprocesses
		system("ps"); // Uses the previous shell's command
	}
	else if (cmdIn == cmd[6]) { // Chgd
		//TODO
	}
	else if (cmdIn == cmd[7]) { // Environ
		//TODO
	}
	else if (cmdIn == cmd[8]) { // Repeat
		//TODO
	}
	else if (cmdIn == cmd[9]) { // hiMom
		//TODO
	}
	else if (cmdIn == "") { // Blank, just make a new line
		return;
	}
	else {
		// Unrecognized command.
		cout << "No command named " << "'" << cmdIn << "'" << " found." << '\n';
	}
}

// POSTNOTE: This was largely left unchanged.
int main() {
	// Connect the exit signal to signal handler
	signal(SIGINT, quitShell);

	// Begin writing to the history file
	cmdHistoryFile.open("cmd_history.txt");

	// Initial Prompt (clear bash first)
	system("clear");
	cout << "Welcome to Jared's shell program. Version " << version << '\n';
	cout << "Type " << "'" << cmd[0] << "'" << " to see available commands." << '\n';
	// Shell engine
	while (true) {
		cout << "SimpleShellJW: ";
		string cmdIn = "";
		getline(cin, cmdIn);
		execCmd(cmdIn);
	}
	return 0;
}


