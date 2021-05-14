#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>


#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
string CAT = "cat";
// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  float MemTotal;
  float MemFree;
 
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >>  key >> value)
    {
      if (key == "MemTotal"){
        MemTotal = std::stof(value);
      }
      else if (key == "MemFree"){
        MemFree = std::stof(value);
        break;
      }
    }
  }
  return (MemTotal - MemFree) / MemTotal; 
  
 }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string value;
  string other;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) 
      {
         return std::stol(value);
      }
    
  }
  
  return 0; 
  }
  

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return (ActiveJiffies() + IdleJiffies());
  }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  auto Active = CpuUtilization();
  // user + nice + system + irq + softirq + steal
   return ( stol(Active[CPUStates::kUser_])    + stol(Active[CPUStates::kNice_])+
            stol(Active[CPUStates::kSystem_])  + stol(Active[CPUStates::kIRQ_])+
            stol(Active[CPUStates::kSoftIRQ_]) + stol(Active[CPUStates::kSteal_])
            ); 
   }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  // idle + iowait
  auto Idle = CpuUtilization();
  return (stol(Idle[CPUStates::kIdle_]) +stol(Idle[CPUStates::kIOwait_]) );
 }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> CPUData;
  string line,value;
  string key;
  std::ifstream stream(kProcDirectory + kStatFilename );
  if (stream.is_open())
  {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "cpu") {
        while(linestream>>value)
        {
          CPUData.push_back(value);
        }
      }
  }
 }
   return CPUData;

}
 

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int totalProc =-1;
  string line;
  string value;
  string key;
  std::ifstream stream(kProcDirectory + kStatFilename );
  if (stream.is_open())
  {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "processes") {
        linestream >> totalProc;
        break;
      }
  }
 }
 return totalProc;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
   int RunningProc =-1;
  string line;
  string value;
  string key;
  std::ifstream stream(kProcDirectory + kStatFilename );
  if (stream.is_open())
  {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "procs_running") {
        linestream >> RunningProc;
        break;
      }
  }
 }
 return RunningProc;
 }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
 string LinuxParser::Command(int pid) {
   string line;
   std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
   if (stream.is_open()) {
     std::getline(stream, line);
   }
   return line;
 }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string value;
  long memory;
  // std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  // if (stream.is_open()) {
  //   std::getline(stream, line);
  //   std::istringstream linestream(line);

  // linestream >> key;
  // if (key == "VmSize:") {
  //     linestream >> memory;
  //     memory /= 1000;
  //     value = std::to_string(memory);
  //   }
  //   }
  
  // return value;
 
  std::ifstream filestream(kProcDirectory+ to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          memory = atoi(value.c_str()) * 0.001;
          return to_string(memory);
        }
      }
    }
  }
  return to_string(memory);

 }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
 string LinuxParser::Uid(int pid) {
   string line;
   string key;
   string value;
   std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
   if (stream.is_open()) {
     std::getline(stream, line);
     std::istringstream linestream(line);
     while (linestream >> key >> value) {
       if (key == "Uid:") {
         break;
       }
     }
   }
   return value;
 }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
 string LinuxParser::User(int pid) {
   string line;
   string user;
   string id;
   string skip;
   string UID = Uid(pid);
   std::ifstream stream(kPasswordPath);
   if (stream.is_open()) {
     while (std::getline(stream, line)) {
       std::replace(line.begin(), line.end(), ':', ' ');
       std::istringstream linestream(line);
       linestream >> user >> skip >> id;
        if (UID == id) {
          break;
       }
     }
   }
   return user;
 }
  // TODO: Read and return the uptime of a process
  // REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory+ to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      int count = 0;
      while (linestream  >> value) {
        if(count == 21)
        {
          return UpTime()- atol(value.c_str())/sysconf(_SC_CLK_TCK);
        }
        else
        {
          count++;
        }
        }
      }
    }

  return 0; 
  }
