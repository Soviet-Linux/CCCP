#include <string>
#include <vector>




int install_package (std::string PName,int use);
std::vector<std::string> open_spm (std::string PName);
int create_binary (std::string PName,std::string built_binaries,std::string dependencies);
int install_binary (std::string PName);
int move_binaries (std::vector<std::string> install_info);





