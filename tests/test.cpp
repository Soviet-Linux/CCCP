
#include "../include/cccp.hpp"
#include <cstdio>
#include <chrono>
// The filesystem root
std::string soviet::ROOT = "soviet";
// For real use it must be set to "/"

std::string soviet::MAIN_DIR = soviet::ROOT + "var/cccp/";
std::string soviet::WORK_DIR = soviet::MAIN_DIR + "work";
std::string soviet::DATA_DIR = soviet::MAIN_DIR + "data/";
std::string soviet::SPM_DIR = soviet::MAIN_DIR + "spm/";
std::string soviet::LOG_DIR = soviet::MAIN_DIR + "log/";
std::string soviet::BUILD_DIR = soviet::WORK_DIR + "build/";
std::string soviet::MAKE_DIR = soviet::WORK_DIR + "make/";
std::string soviet::INSTALLED_FILE = soviet::DATA_DIR + "installed.json";
std::string soviet::ALL_FILE = soviet::DATA_DIR + "all.json";
std::string soviet::CONFIG_FILE = soviet::ROOT + "cccp.conf";
std::vector<std::string> soviet::REPOS;

bool soviet::DEBUG = true;
bool soviet::TESTING = true;

void exec_tests();
void cmd_perfs();

int main()
{
    std::cout << "Testing exec functions" << std::endl;
    exec_tests();
    std::cout << "Testing exec functions done" << std::endl;
    std::cout << "mesuring exec() and system() preformances" << std::endl;
    cmd_perfs();


}

void exec_tests()
{
    std::cout << "Testing cmd functions" << std::endl;
    std::string cmd = "ls -l";
    std::string result = soviet::exec(cmd.c_str());
    std::cout << result << std::endl;
}
void cmd_perfs()
{
    std::string cmd = "ls -l";
        // Get starting timepoint
    auto exec_start = std::chrono::high_resolution_clock::now();

    // Execute the command
    soviet::exec(cmd.c_str());
    // Get ending timepoint
    auto exec_stop = std::chrono::high_resolution_clock::now();
    // Get starting timepoint
    auto exec_results_start = std::chrono::high_resolution_clock::now();

    // Execute the command
    std::string result = soviet::exec(cmd.c_str());
    std::cout << result << std::endl;
    // Get ending timepoint
    auto exec_results_stop = std::chrono::high_resolution_clock::now();
        // Get starting timepoint
    auto system_start = std::chrono::high_resolution_clock::now();

    // Execute the command
    system(cmd.c_str());
    // Get ending timepoint
    auto system_stop = std::chrono::high_resolution_clock::now();

    auto exec_results_duration = std::chrono::duration_cast<std::chrono::microseconds>(exec_results_stop - exec_results_start);
    auto exec_duration = std::chrono::duration_cast<std::chrono::microseconds>(exec_stop - exec_start);
    auto system_duration = std::chrono::duration_cast<std::chrono::microseconds>(system_stop - system_start);

    std::cout << "system took : " << system_duration.count() << " microseconds" << std::endl;
    std::cout << "exec took : " << exec_duration.count() << " microseconds" << std::endl;
    std::cout << "exec results took : " << exec_results_duration.count() << " microseconds" << std::endl;
}
