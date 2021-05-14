#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    
    // LinuxParser::ActiveJiffies();
    // return (LinuxParser::Jiffies() - LinuxParser::IdleJiffies())/LinuxParser::Jiffies() ;
     long cpu_user;
    long cpu_nice;
    long cpu_system;
    long cpu_idle;
    long cpu_iowait;
    long cpu_irq;
    long cpu_softirq;
    long cpu_steal;
    float CPU_Percentage;

    std::vector<std::string> cpu_data;

    cpu_data = LinuxParser::CpuUtilization();

    cpu_user = std::stoi(cpu_data.back());
    cpu_data.pop_back();

    cpu_nice = std::stoi(cpu_data.back());
    cpu_data.pop_back();

    cpu_system = std::stoi(cpu_data.back());
    cpu_data.pop_back();

    cpu_idle = std::stoi(cpu_data.back());
    cpu_data.pop_back();

    cpu_iowait= std::stoi(cpu_data.back());
    cpu_data.pop_back();

    cpu_irq = std::stoi(cpu_data.back());
    cpu_data.pop_back();

    cpu_softirq = std::stoi(cpu_data.back());
    cpu_data.pop_back();

    cpu_steal= std::stoi(cpu_data.back());
    cpu_data.pop_back();

    



    const long idle = cpu_idle + cpu_iowait;


    const long nonIDLE = cpu_user + cpu_nice + cpu_system + cpu_irq + cpu_softirq + cpu_steal;

    const long total = idle + nonIDLE;


    CPU_Percentage =(total - idle)/static_cast<float>( total);

    return CPU_Percentage;
     }