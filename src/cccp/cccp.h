#include <string>
#include <vector>

enum actionList {INSTALL_LOCAL,INSTALL_FROM_REPO,CHECK,LIST,REMOVE,CREATE,GET,HELP,UPDATE};

int cccp(actionList option , std::vector<std::string> parameters, bool DEBUG=false, bool TESTING=false);