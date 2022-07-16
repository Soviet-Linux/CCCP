
#include <curl/curl.h>
#include "../include/libspm.hpp"
#include <cstdio>
#include <chrono>
#include <unistd.h>
// The filesystem root




void exec_tests();
void cmd_perfs();
int tests_on_childrens();
int dowload_test();

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int main()
{
   dowload_test();
}

void init_vars()
{


    std::string ROOT = "soviet";
    // For real use it must be set to "/"

    std::string soviet::vars.MAIN_DIR = soviet::ROOT + "var/cccp/";
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
int tests_on_childrens()
{
    // Here we are forking a child
    int pid = fork();
    if (pid > 0)
    {
        printf("I'm the parent with pid: %d, and created child with %d\n", getpid(), pid);
    }
    else if (pid == 0)
    {
        printf("I'm the daughter with pid: %d and parent is %d\n", getpid(), getppid());
        // here we are killing the child
        exit(0);
    }
    else // pid == -1
    {
        printf("fork() faled\n");
    }
    return 0;
}
int dowload_test()
{
    std::cout << "Testing dowload functions" << std::endl;
    CURL *curl;
    FILE *fp;
    CURLcode res;
    char *url = "https://our.sovietlinux.ml/all.json";
    char outfilename[FILENAME_MAX] = "all.json";
    curl = curl_easy_init();
    if (curl) {
        std::cout << "Downloading " << url << std::endl;
        fp = fopen(outfilename,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);
    }
    return 0;
}
