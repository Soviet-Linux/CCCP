#include <string>
#include <vector>



std::vector<std::string> init_pkg_list (std::string DATA_DIR);
int make_pkg (std::string PName,std::string download_info,std::string build_info,std::string CURRENT_DIR,std::string DATA_DIR);
int check_dependencies (std::string dependencies) ;