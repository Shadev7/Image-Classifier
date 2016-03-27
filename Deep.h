


#include <fstream>

using namespace std;




class Deep : public Classifier
{
public:
  map<string, int> category_map;
  Deep(const vector<string> &_class_list) : Classifier(_class_list) {
    for(int i=0; i<25;i++){
      category_map[category[i]] = i;
    }
  }



  virtual void train(const Dataset &filenames)
  {
    system("python make_overfeat.py train > .info");
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


  virtual string classify(const string &filename)
  {
    system(("python overfeat_helper.py " + filename + " > .predict").c_str());
    string result = read_predict_file(".predict");
    cout<< "result: "<<result<<endl;
    return result;
  }

  virtual void load_model()
  {

  }

};
