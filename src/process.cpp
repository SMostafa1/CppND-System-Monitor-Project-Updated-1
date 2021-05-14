#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int i) {
  pid_ = i;
  command_ = LinuxParser::Command(pid_);
  ram_ = LinuxParser::Ram(pid_);
  user_ = LinuxParser::User(pid_);
  UpTime_ = LinuxParser::UpTime(pid_);
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    //total-idle / total
    return ((LinuxParser::Jiffies()-LinuxParser::IdleJiffies())/LinuxParser::Jiffies());
 }

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return UpTime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }