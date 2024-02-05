// Here are the commands I made for the purpose of this assignment.

#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../inc/Commands.h"

// Repeat
string RepeatCommand::GetName() { return "repeat"; }
void RepeatCommand::PrintHelp() {
	cout << this->GetName() << ": outputs a string to the console. Similar to the echo command, but less sophisticated." << '\n';
	cout << "Usage: "<< this->GetName() << " <string>" << '\n';
	cout << "\t <string> - the string to repeat from." << '\n';
	cout << "\t <string> > <file> - Redirect output to a file of your choosing." << '\n';
}
int RepeatCommand::Execute(vector<string> args) {
	// arg0 must be a string.
	// arg1 must be a redirection operator.
	// arg2 must be a file name.
	if (args.size() > 0) {
		string str = args[0];
		if (args.size() > 2) {
			char rOp = args[1].at(0);
			if (rOp == '>') {
				// Snippit of Dr. Doman's code... Slightly modified due to hiccups.
				string redirectedFile = args[2];
				// NOTE: 664 is supposed to set rw rw r to the file, but it isn't working from open for some reason.
				//	Therefore, I just ran a system command chmod 664 redirectedFile to set the correct permissions.
				int fileDescriptor = open(redirectedFile.c_str(), O_RDWR | O_TRUNC | O_CREAT | O_APPEND, 664); //opens file using the extension entered
				if (fileDescriptor != -1) {
					string changePermissionHack = "chmod 664 " + redirectedFile;
					system(changePermissionHack.c_str());
					int saved_stdout = dup(1); // save stdout I/O file handle
					dup2(fileDescriptor, 1); // Set stdout direction to our file
					cout << str << '\n';	// Output to stdout
					dup2(saved_stdout, 1);	// Restore stdout to I/O
				}
				else {
					cout << "Repeat failed: could not write to file. " << '\n';
					return 1;
				}
			}
			else {
				cout << "Repeat failed: only output redirect is supported. " << '\n';
				return 1;
			}
		}
		else {
			// just repeat to the shell (only provided with arg0).
			cout << str << '\n';
		}
	}
	else {
		// Print out the help if the user does not provide any arguments.
		cout << "Repeat failed: not enough arguments. Help topic: " << '\n';
		this->PrintHelp();
	}
	return 0;
}

// HiMom
string HiMomCommand::GetName() { return "himom"; }
void HiMomCommand::PrintHelp() {
	cout << this->GetName() << ": creates a child process that sends a message over to this parent, the shell process. The message will be sent through a pipe, and then the child exits on its own." << '\n';
	cout << "Usage: "<< this->GetName() << '\n';
}
int HiMomCommand::Execute(vector<string> args) {
	int fd[2]; // Prepare the pipe
	int nbytes;
	char readbuf[80];
	if (pipe(fd) == -1) { // Try opening the pipe
		cout << "Something went wrong with creating the pipe." << '\n';
		return 1;
	}
	pid_t pid = fork(); //Create the child
	// Didn't need to save the stdout and stdin - I left these over when
	//	experimenting on the pipes
	if (pid == -1) { // Error
		cout << "Something went wrong with creating the child." << '\n';
		return 1;
	}
	else if (pid == 0) { // I am the child, a duplicate of this process
		close(fd[0]); // We're not reading from pipe, the child is writing to pipe.
		string toSend = "Hi mom! \n";
		write(fd[1], toSend.c_str(), toSend.size()); // Write to writing end
		close(fd[1]); // Done writing
		exit(0); // Stop the child (note this does not go to quitShell())
	}
	else { // I am the parent
		close(fd[1]); // We're not writing to pipe, the parent is listening on the other end.
		wait(NULL); // Wait for child
		nbytes = read(fd[0], readbuf, sizeof(readbuf)); // Listen from reading end
		close(fd[0]); // Done reading
		cout << readbuf;
	}
	return 0;
}

