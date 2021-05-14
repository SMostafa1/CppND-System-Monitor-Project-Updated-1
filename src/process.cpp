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
float Process::CpuUtilization() const{
    //total-idle / total
    // return ((LinuxParser::Jiffies()-LinuxParser::IdleJiffies())/LinuxParser::Jiffies());
     string line;
    string key;
    string value;

    long utime = 0;
    long stime = 0;
    long cutime = 0;
    long cstime = 0;
    long starttime = 0;
    long upTime;

    float total_time = 0;
    double seconds = 0;
    float cpu_usage=0;
    std::ifstream filestream(LinuxParser::kProcDirectory+ to_string(pid_)+LinuxParser::kStatFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) 
        {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            int index = 0;
            while (linestream  >> value) 
            {
                if(index == 13)
                {
                    utime = atol(value.c_str());
                    index ++ ;
                }
                else if(index == 14)
                {
                    stime = atol(value.c_str());;
                    index ++ ;
                }
                else if(index == 15)
                {
                    cutime = atol(value.c_str());
                    index ++ ;
                }
                else if(index == 16)
                {
                    cstime = atol(value.c_str());
                    index ++ ;
                }
                else if(index == 21)
                {
                    starttime = atol(value.c_str());
                    index ++ ;
                }
                else
                {
                    index ++ ;
                }
                
            }
        }
    }

    upTime = LinuxParser::UpTime() ;
    total_time = utime + stime +  cutime + cstime;
    seconds = upTime - (starttime/sysconf(_SC_CLK_TCK)) ;

    cpu_usage = (total_time/sysconf(_SC_CLK_TCK)/seconds);
    return cpu_usage;
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
bool Process::operator<(Process const& a) const {
     return (CpuUtilization() < a.CpuUtilization());
    
 }