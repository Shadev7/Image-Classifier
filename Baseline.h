
#define SIZE 28

#include <fstream>
using namespace std;

class Baseline : public Classifier
{
public:
  Baseline(const vector<string> &_class_list) : Classifier(_class_list) {}


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
    int c = 0;
    for (Dataset::const_iterator c_iter = filenames.begin(); c_iter != filenames.end(); ++c_iter)
    {
      cout << "Processing " <<name <<"/" << c_iter->first << endl;
      // convert each image to be a row of this "model" image
      for (int i = 0; i < c_iter->second.size(); i++) 
      {
        CImg<unsigned char> query_image(c_iter->second[i].c_str());
        query_image = query_image.get_RGBtoYCbCr().get_channel(0);
        query_image.resize(SIZE, SIZE);
        write_data(c, query_image, myfile);
      }
      c++;
    }
    myfile.close();
  }

  // int read_predict_file(string &filename){
  //   ifstream myReadFile;
  //   myReadFile.open(filename);
  //   char output[2];
  //   myReadFile >> output;
  //   myReadFile >> output;

  //   cout << output << endl;
  //   myReadFile.close();
  //   return 0;
  // }

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
    // write_data(i+1, query_image, myfile);
    myfile.close();
    system("./svm_multiclass/svm_multiclass_classify baseline/test.data baseline/model baseline/predict > .info");
    // read_predict_file();

  }

  virtual void load_model()
  {

  }

};
