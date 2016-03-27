
#include <fstream>
#include <sstream>

using namespace std;

string category_e[] = {"bagel","bread","brownie","chickennugget",
                           "churro","croissant","frenchfries","hamburger",
                           "hotdog","jambalaya","kungpaochicken","lasagna",
                           "muffin","paella","pizza","popcorn","pudding",
                           "salad","salmon","scone","spaghetti","sushi",
                           "taco","tiramisu","waffle"};

class EigenVec : public Classifier
{
public:
  EigenVec(const vector<string> &_class_list) : Classifier(_class_list) {}	
  // Eigen vector training. 
  virtual void train(const Dataset &filenames) 
  {
	  
	  
	ofstream myfile;
    myfile.open ("train_ev.data");
	int category = 0;
	

    for(Dataset::const_iterator c_iter=filenames.begin(); c_iter != filenames.end(); ++c_iter)
      {
		 category += 1;
		cout << "Processing " << c_iter->first << endl;
		//CImg<double> class_vectors(size*no_of_eigen_vecs, filenames.size(), 1);

		// convert each image to be a row of this "model" image
		for(int i=0; i<c_iter->second.size(); i++)
		{
			//CImg <double> exfeatures = extract_features(c_iter->second[i].c_str());
			//eigen_decomp(exfeatures);
			//class_vectors = class_vectors.draw_image(0, i, 0, 0, eigen_decomp(exfeatures));
			CImg<double> ip_img(c_iter->second[i].c_str());
			ip_img.resize(size, size);
			ip_img = ip_img.get_RGBtoYCbCr().get_channel(0);
			//image_bag(ip_img,vector<SiftDescriptor> k_points, k);
			CImgList<float> eigen_vecs = eigen_decomp(ip_img);
			myfile << category << " ";
			//Build n-dimensional vector
			for (int j=0; j<no_of_eigen_vecs ; j++)
			{
				for (int k=0; k<eigen_vecs[2].width(); k++)
				{
					 myfile << (j*eigen_vecs[2].width() + k + 1) << ":"<< eigen_vecs[2].atXY(j,k) << " ";
				}
			}
			myfile << endl;
			
		
		//CImg<double> eigen_vec2 = eigen_decomp2(ip_img);
		//class_vectors = class_vectors.draw_image(0, i, 0, 0, image_bag(ip_img,k_points,k));
		//class_vectors = class_vectors.draw_image(0, i, 0, 0, extract_features(c_iter->second[i].c_str()));
		//cin.get();
		}
	  
	//class_vectors.save_png(("ev_model." + c_iter->first + ".png").c_str());
      }
	  myfile.close();
	  system ("./svm_multiclass/svm_multiclass_learn -c 50 train_ev.data ev_model > .info");
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
    return category_e[value-1];  
  }

  int category_map(string category_name)
  {
	  int category;
	  for (int i=0; i< (sizeof(category_e)/sizeof(category_e[0])); i++)
	  {
		  if (category_name == category_e[i])
		  {
			  category = i;
			  break;
		  }

	  }
	  return category;
  }
  virtual string classify(const string &filename)
  {
	CImg<double> test_image(filename.c_str());
	test_image = test_image.get_RGBtoYCbCr().get_channel(0);
	test_image.resize(size, size);
	CImgList<float> eigen_vecs = eigen_decomp(test_image);
    
	
	ofstream myfile;
    myfile.open(".temp");

    //int temp = filename.find("/");
    //string category_name = filename.substr(temp+1, filename.find_last_of("/"));
	string category_name = filename.substr(filename.find("/") + 1,filename.find_last_of("/") - filename.find("/") -1);
	
	//cin.get();
    //write_data(category_map[category_name]+1, query_image, myfile);
	int category = category_map(category_name)+1 ;
	myfile << category << " ";
	//Build n-dimensional vector
	for (int j=0; j<no_of_eigen_vecs ; j++)
	{
		for (int k=0; k<eigen_vecs[2].width(); k++)
		{
			 myfile << (j*eigen_vecs[2].width() + k + 1) << ":"<< (float) eigen_vecs[2].atXY(j,k) << " ";
		}
	}
	myfile << endl;
	
    myfile.close();
    system("./svm_multiclass/svm_multiclass_classify .temp ev_model ev_predict > .info");
    string result = read_predict_file("ev_predict");
    cout << result << endl;
    return result;
  }

  virtual void load_model()
  {
  }
  CImg<double> extract_features(const string &filename)
    {
      return (CImg<double>(filename.c_str())).resize(size,size,1,3).unroll('x');
    }
  CImgList<float> eigen_decomp(CImg<double> ip_img)
    {
	  
	  CImgList<float> USV = ip_img.get_SVD();
	  
	  
	  return USV;
    }
	


protected:
  // extract features from an image, which in this case just involves resampling and 
  // rearranging into a vector of pixel data.


  static const int size=40;  // subsampled image resolution
  map<string, CImg<double> > models; // trained models
  static const int no_of_eigen_vecs = 10;
};
