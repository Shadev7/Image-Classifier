

#include "CImg.h"
#include <ctime>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <vector>
#include <Sift.h>
#include <sys/types.h>
#include <dirent.h>
#include <map>
#include <numeric>

/**
with grey scale first
    0. greysclae, resize
    1. [(1,2,2,,3,3,4,.... SIZE*SIZE), ...   ]
    

no grey scale, (r,g,b)

**/

#define SIZE 40

using namespace std;

void write_data() {



}
// test
vector<string> files_in_directory(const string &directory, bool prepend_directory = false)
{
  vector<string> file_list;
  DIR *dir = opendir(directory.c_str());
  if(!dir)
    throw std::string("Can't find directory " + directory);
  
  struct dirent *dirent;
  while ((dirent = readdir(dir))) 
    if(dirent->d_name[0] != '.')
      file_list.push_back((prepend_directory?(directory+"/"):"")+dirent->d_name);

  closedir(dir);
  return file_list;
}

void printVector(vector<string> filenames){
    for (std::vector<string>::const_iterator i = filenames.begin(); i != filenames.end(); ++i)
        std::cout << *i << " | ";
}

int main(int argc, char **argv)
{
    vector<string> foldernames = files_in_directory("test", true);
    // printVector(foldernames);
    vector<string> testfiles = files_in_directory(foldernames[0], true);
    // printVector(testfiles);

    

    for(int i=0; i<foldernames.size(); ++i){

    }




}













