#include <iostream>
#include <string>
#include <vector>


/*
To uninstall packages , we'll use the location data ( all the files that were installed by the program )
If you want more detail go to store_spm() in spm.cpp
So we have all list of all thoses file in a json array inside the package's spm file in DATA_DIR
(each package have a spm file in the DATA_DIR , and each spm file contains a json array of all the files that were installed by the program)
And we will cycle through all the files in the json array and remove them from the system.

This is simple . It could be simpler but really i dont know how .
So dont touch this , except if there are a critical bug or an important missing feature.
*/
// this function is for uninstaling packages
void remove(const std::string& PName,bool DEBUG)
{
    //All the block of code below is for getting the data from the spm file of the package
    // creating a string variable to hold the package's spm file path
    std::string PPath = DATA_DIR + PName + ".spm";
    //small message , its uselless but i'll leave it there
    std::cout << "Uninstalling package " << PName << std::endl;
    //paring spm file using open_spm()
    open_spm(PPath);

    //remove all the files in the data["locations"]
    for (int i = 0; i < locations.size(); i++)
    {
        /*
            We had a small issue here earlier with the folders ,
            because some essential non empty system folder (like /usr ) where removed
            I fixed it by using std::filesystem::remove() , because it can't remove non empty folders
            but this created another problem because it would drop an exception and kill the program 
            so i had to use a try catch block to catch the exception and continue the program
            But the try catch block is not the best way to do it , but i had to do it because i was not sure
            More on that later
        */
        try {
            std::cout << "Removing " << locations[i] << std::endl;
            remove(locations[i].c_str());
        }
        catch (std::exception& e)
        {
            std::string str_e = e.what();

            /*
            Here we are checking if the exception is the one that we sould catch ("Directory not empty") or another exception
            The way we are doing this is not very good for performance but i was not sure how to do it better
            TODO: find a better way 
            We could use error codes or smth like that
            But i dont know how
            */

            //checking if the excetion string we should catch is inside the catched exception 
            // I know it isn't clear (Again , DM me on discord if you dont understand  )
            //Or maybe read the docs (at the time i write this the docs are not yet good enough to understand this)
            if (!(str_e.find("Directory not empty") != std::string::npos))
            {
                std::cout << "This is BAD, terminating : " << str_e << '\n';
                exit(1);
            }

        }
        
    }
    //removing the entries in packages.json
    rm_pkg_data(DATA_FILE,PName);

    //remove the spm file from DATA_DIR
    std::string rm_spm_cmd = "rm -rf " + DATA_DIR + PName + ".spm";
    system((rm_spm_cmd).c_str());
}
