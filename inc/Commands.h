#ifndef JW_COMMAND_OBJ
#define JW_COMMAND_OBJ
#include <iostream>
#include <vector>
#include <map>
using namespace std;

// Abstract class for all commands
class Command {
	public:
		virtual string GetName() = 0;
		virtual void PrintHelp() = 0;
		virtual int Execute(vector<string> args) = 0;
};

// Shell commands
class HelpCommand : public Command {
	public:
		HelpCommand(map<string, Command*>& cmdMap) {cmdMap[this->GetName()] = this;}
		string GetName();
		void PrintHelp();
		int Execute(vector<string> args);
};
class QuitCommand : public Command {
	public:
		QuitCommand(map<string, Command*>& cmdMap) {cmdMap[this->GetName()] = this;}
		string GetName();
		void PrintHelp();
		int Execute(vector<string> args);
};
class ClearCommand : public Command {
	public:
		ClearCommand(map<string, Command*>& cmdMap) {cmdMap[this->GetName()] = this;}
		string GetName();
		void PrintHelp();
		int Execute(vector<string> args);
};
class DirCommand : public Command {
	public:
		DirCommand(map<string, Command*>& cmdMap) {cmdMap[this->GetName()] = this;}
		string GetName();
		void PrintHelp();
		int Execute(vector<string> args);
};
class MyProcessCommand : public Command {
	public:
		MyProcessCommand(map<string, Command*>& cmdMap) {cmdMap[this->GetName()] = this;}
		string GetName();
		void PrintHelp();
		int Execute(vector<string> args);
};
class AllProcessesCommand : public Command {
	public:
		AllProcessesCommand(map<string, Command*>& cmdMap) {cmdMap[this->GetName()] = this;}
		string GetName();
		void PrintHelp();
		int Execute(vector<string> args);
};
class ChgdCommand : public Command {
	public:
		ChgdCommand(map<string, Command*>& cmdMap) {cmdMap[this->GetName()] = this;}
		string GetName();
		void PrintHelp();
		int Execute(vector<string> args);
};
class EnvironCommand : public Command {
	public:
		EnvironCommand(map<string, Command*>& cmdMap) {cmdMap[this->GetName()] = this;}
		string GetName();
		void PrintHelp();
		int Execute(vector<string> args);
};


// Custom commands
class RepeatCommand : public Command {
	public:
		RepeatCommand(map<string, Command*>& cmdMap) {cmdMap[this->GetName()] = this;}
		string GetName();
		void PrintHelp();
		int Execute(vector<string> args);
};
class HiMomCommand : public Command {
	public:
		HiMomCommand(map<string, Command*>& cmdMap) {cmdMap[this->GetName()] = this;}
		string GetName();
		void PrintHelp();
		int Execute(vector<string> args);
};

#endif
