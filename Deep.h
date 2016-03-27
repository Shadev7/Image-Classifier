

// http://stackoverflow.com/a/478960/847357

#define SIZE 28

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
    return exec(("python overfeat_helper.py " + filename).c_str());
  }

  virtual void load_model()
  {

  }

};
