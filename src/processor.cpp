#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    
    // LinuxParser::ActiveJiffies();
    return LinuxParser::ActiveJiffies()/(float) LinuxParser::Jiffies() ;
     }