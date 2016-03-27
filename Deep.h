

// http://stackoverflow.com/a/478960/847357

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>

using namespace std;



class Deep : public Classifier
{
public:
  Deep(const vector<string> &_class_list) : Classifier(_class_list) {

  }



  virtual void train(const Dataset &filenames)
  {
    system("python make_overfeat.py train > .info");
  }

  string exec(const char* cmd) 
  {
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe.get())) {
      if (fgets(buffer, 128, pipe.get()) != NULL)
        result += buffer;
    }
    return result;
  }

  virtual string classify(const string &filename)
  {
    string result = exec(("python overfeat_helper.py " + filename).c_str());
    cout<< "result: "<<result<<endl;
    return result;
  }

  virtual void load_model()
  {

  }

};
