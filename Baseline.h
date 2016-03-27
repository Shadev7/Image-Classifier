
#define SIZE 28

#include <fstream>
#include <sstream>
using namespace std;


string category[] = {"bagel","bread","brownie","chickennugget",
                           "churro","croissant","frenchfries","hamburger",
                           "hotdog","jambalaya","kungpaochicken","lasagna",
                           "muffin","paella","pizza","popcorn","pudding",
                           "salad","salmon","scone","spaghetti","sushi",
                           "taco","tiramisu","waffle"};

class Baseline : public Classifier
{
public:
  map<string, int> category_map;
  Baseline(const vector<string> &_class_list) : Classifier(_class_list) {
    for(int i=0; i<25;i++){
      category_map[category[i]] = i;
    }
  }


  void write_data(int category,
                  CImg<unsigned char> output,
                  ofstream & myfile) 
  {

    myfile << category << " ";
    for (int y = 0; y < output._height; y++) {
      for (int x = 0; x < output._width; x++) {
        int featureN = output._height * y + x + 1;
        myfile << featureN << ":" <<  ((int)output(x, y, 0, 0) / 255.0) << " ";
      }
    }
    myfile << endl;

  }

  void make_data(const Dataset &filenames, string name){
    ofstream myfile;
    myfile.open("baseline/" + name + ".data");
    for (Dataset::const_iterator c_iter = filenames.begin(); c_iter != filenames.end(); ++c_iter)
    {
      cout << "Processing " <<name <<"/" << c_iter->first << endl;
      // convert each image to be a row of this "model" image
      for (int i = 0; i < c_iter->second.size(); i++) 
      {
        CImg<unsigned char> query_image(c_iter->second[i].c_str());
        query_image = query_image.get_RGBtoYCbCr().get_channel(0);
        query_image.resize(SIZE, SIZE);
        write_data(category_map[c_iter->first.c_str()]+1, query_image, myfile);
      }
    }
    myfile.close();
  }

  string read_predict_file(const string &filename){
    ifstream myReadFile;
    myReadFile.open(filename.c_str());
    char output[3];
    myReadFile.get(output[0]);
    myReadFile.get(output[1]);
    output[2] = '\0';
    myReadFile.close();
    string categoryi = string(output);
    int value;
    istringstream buffer(categoryi);
    buffer >> value; 
    return category[value-1];
  }

  virtual void train(const Dataset &filenames)
  {
    make_data(filenames, "train");
    // make_data(filenames, "test");
    system ("./svm_multiclass/svm_multiclass_learn -c 50 baseline/train.data baseline/model > .info");
  }

  virtual string classify(const string &filename)
  {
    ofstream myfile;
    myfile.open(".temp");
    CImg<unsigned char> query_image(filename.c_str());
    query_image = query_image.get_RGBtoYCbCr().get_channel(0);
    query_image.resize(SIZE, SIZE);
    int temp = filename.find("/");
    string category_name = filename.substr(temp+1, filename.find("/"));
    write_data(category_map[category_name]+1, query_image, myfile);
    myfile.close();
    system("./svm_multiclass/svm_multiclass_classify .temp baseline/model baseline/predict > .info");
    return read_predict_file("baseline/predict");
  }

  virtual void load_model()
  {

  }

};
