
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

  virtual void train(const Dataset &filenames)
  {
    for (Dataset::const_iterator c_iter = filenames.begin(); c_iter != filenames.end(); ++c_iter)
    {
      cout << "Processing " << c_iter->first << endl;
      ofstream myfile;
      myfile.open("baseline/train.data");
      // convert each image to be a row of this "model" image
      for (int i = 0; i < c_iter->second.size(); i++) 
      {
        CImg<unsigned char> query_image(c_iter->second[i].c_str());
        query_image = query_image.get_RGBtoYCbCr().get_channel(0);
        query_image.resize(SIZE, SIZE);
        write_data(i+1, query_image, myfile);
      }

    }
  }

  virtual string classify(const string &filename)
  {
    CImg<double> test_image = extract_features(filename);

    // figure nearest neighbor
    pair<string, double> best("", 10e100);
    double this_cost;
    for (int c = 0; c < class_list.size(); c++)
      for (int row = 0; row < models[ class_list[c] ].height(); row++)
        if ((this_cost = (test_image - models[ class_list[c] ].get_row(row)).magnitude()) < best.second)
          best = make_pair(class_list[c], this_cost);

    return best.first;
  }

  virtual void load_model()
  {
    for (int c = 0; c < class_list.size(); c++)
      models[class_list[c] ] = (CImg<double>(("nn_model." + class_list[c] + ".png").c_str()));
  }
protected:
  // extract features from an image, which in this case just involves resampling and
  // rearranging into a vector of pixel data.
  CImg<double> extract_features(const string &filename)
  {
    return (CImg<double>(filename.c_str())).resize(size, size, 1, 3).unroll('x');
  }

  static const int size = 20; // subsampled image resolution
  map<string, CImg<double> > models; // trained models
};
