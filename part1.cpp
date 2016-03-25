


#include "part1.h"
#define SIZE 40

using namespace std;

void print_vector(vector<string> filenames){
    for (std::vector<string>::const_iterator i = filenames.begin(); i != filenames.end(); ++i)
        std::cout << *i << " | ";
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


void write_data(int category, 
                CImg<unsigned char> output, 
                ofstream &myfile) {

    myfile << category << " ";
    for (int y = 0; y < output._height; y++) {
        for (int x = 0; x < output._width; x++) {
            int featureN = output._height*y + x + 1;
            myfile << featureN << ":"<< (1 - ((int)output(x,y,0,0) / 255.0)) << " ";
        }
    }
    myfile << endl;

}

void make_data(string name){
    vector<string> foldernames = files_in_directory(name, true);
    // print_vector(foldernames);
    vector<string> testfiles = files_in_directory(foldernames[0], true);
    
    ofstream myfile;
    myfile.open (name + ".data");


    for(int i=0; i<foldernames.size(); ++i){
        vector<string> filenames = files_in_directory(foldernames[i], true);
        for(vector<string>::const_iterator f = filenames.begin(); 
            f != filenames.end(); 
            ++f){

            CImg<unsigned char> query_image((*f).c_str());
            query_image = query_image.get_RGBtoYCbCr().get_channel(0);
            query_image.resize(SIZE, SIZE);
            write_data(i+1, query_image, myfile);
        }
    }

    myfile.close();
}


int main(int argc, char **argv)
{
    make_data("train");
    make_data("test");

}













