
// class stuff
#include "../../include/libspm.hpp"

void soviet::package::getSources ()
{
        std::string sources_cmd = format(" URL=%s && cd %s && %s",url,vars.MAKE_DIR.c_str(),info["download"].c_str());

}
