string category_e[] = {"bagel","bread","brownie","chickennugget",
                           "churro","croissant","frenchfries","hamburger",
                           "hotdog","jambalaya","kungpaochicken","lasagna",
                           "muffin","paella","pizza","popcorn","pudding",
                           "salad","salmon","scone","spaghetti","sushi",
                           "taco","tiramisu","waffle"};
#include <fstream>
using namespace std;

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
    /* ofstream myReadFile;
    myReadFile.open(filename);
    char output[3];
    myReadFile.get(output[0]);
    myReadFile.get(output[1]);
    output[2] = '\0';
    myReadFile.close();
    string categoryi = string(output);
    //return category_e[stoi(categoryi)-1];  */
	return "aa";
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
    /* CImg<double> test_image = extract_features(filename);
	      
    // figure nearest neighbor
    pair<string, double> best("", 10e100);
    double this_cost;
    for(int c=0; c<class_list.size(); c++)
      for(int row=0; row<models[ class_list[c] ].height(); row++)
	if((this_cost = (test_image - models[ class_list[c] ].get_row(row)).magnitude()) < best.second)
	  best = make_pair(class_list[c], this_cost); */

    //return best.first;
	
	ofstream myfile;
    myfile.open(".temp");

    //int temp = filename.find("/");
    //string category_name = filename.substr(temp+1, filename.find_last_of("/"));
	string category_name = filename.substr(filename.find("/") + 1,filename.find_last_of("/") - filename.find("/") -1);
	cout << category_name << endl;
	//cin.get();
    //write_data(category_map[category_name]+1, query_image, myfile);
	int category = category_map(category_name)+1 ;
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
	
    myfile.close();
    system("./svm_multiclass/svm_multiclass_classify .temp ev_model ev_predict > .info");
    //return read_predict_file("ev_predict");
	return category_name;
  }

  virtual void load_model()
  {
    /* for(int c=0; c < class_list.size(); c++)
      models[class_list[c] ] = (CImg<double>(("ev_model." + class_list[c] + ".png").c_str())); */
  }
  CImg<double> extract_features(const string &filename)
    {
      return (CImg<double>(filename.c_str())).resize(size,size,1,3).unroll('x');
    }
  CImgList<float> eigen_decomp(CImg<double> ip_img)
    {
	  /*CImg<double> ip_img(filename.c_str());
	  ip_img.assign(ip_img.resize(size,size,1,1));
	  cout << "name : " <<filename.c_str() <<endl;
	  cout << "size : " <<size <<endl;
	  cout << "width : " <<ip_img.width() <<endl;
	  cout << "heigth : " << ip_img.height() << endl;
	  ip_img.assign(ip_img.matrix());
	  CImg<double> eVal;
	  CImg<double> eVec;
	  ip_img.eigen(eVal, eVec);
	  cout << "done" << endl;
	  return eVec;*/
	  //CImg<double> U,S,V;
	  CImgList<float> USV = ip_img.get_SVD();
	  
	  /* cout << "width U: " <<USV[0].width() <<endl;
	  cout << "height U: " <<USV[0].height() <<endl;
	  cout << "width S: " <<USV[1].width() <<endl;
	  cout << "height S: " <<USV[1].height() <<endl;
	  cout << "width V: " <<USV[2].width() <<endl;
	  cout << "height V: " <<USV[2].height() <<endl; */
	  /* for (int i = 0; i< USV[2].width();i++)
	  {
		  for (int j = 0; j< USV[2].width();j++)
		  {
			cout << USV[2].atXY(i,j) << ",";
		  }
		  cout << endl;
		  //break;
	  }
	  cout << "width S: " <<USV[1].width() <<endl;
	  cout << "height S: " <<USV[1].height() <<endl;
	  for (int i = 0; i< USV[0].height();i++)
	  {

		cout << USV[1].atXY(0,i) << ",";
		 //break;
	  }
	  cout << endl; */
	  //cin.get();
	  return USV;
    }
	
	
/* 	CImg<double> eigen_decomp2(CImg<double> ip_img)
    {

	  CImgList<float> VecVal = ip_img.get_symmetric_eigen();
	  
	  cout << "width vec: " <<VecVal[0].width() <<endl;
	  cout << "height vec: " <<VecVal[0].height() <<endl;
	  cout << "width val: " <<VecVal[1].width() <<endl;
	  cout << "height val: " <<VecVal[1].height() <<endl;

	  for (int i = 0; i< VecVal[0].width();i++)
	  {
		  for (int j = 0; j< VecVal[0].width();j++)
		  {
			cout << VecVal[0].atXY(i,j) << ",";
		  }
		  cout << endl;
		  break;
	  }
	  cout << "width S: " <<VecVal[1].width() <<endl;
	  cout << "height S: " <<VecVal[1].height() <<endl;
	  for (int i = 0; i< VecVal[1].height();i++)
	  {

		cout << VecVal[1].atXY(0,i) << ",";
		break;
		 
	  }
	  cout << endl;
	  //cin.get();
	  return VecVal[0];
    } */
/* 	
	CImg<double> haar_features(CImg<double> ip_vec)
    {
		CImg<double> haar_wavelet (ip_vec.haar('x'));
		return haar_wavelet;
	}
	CImg<double> integral_images(CImg<double> ip_vec)
    {
		CImg<double> integral_image(ip_vec) ;
		for (int i=0;i<ip_vec.width() ; i++)
		{
			for (int j=0;j<ip_vec.height() ; j++)
			{
				integral_image.atXY(i,j) = ip_vec.atXY(i,j);
				if (i > 0)
					integral_image.atXY(i,j) += integral_image.atXY(i -1,j);
				if (j > 0)
					integral_image.atXY(i,j) += integral_image.atXY(i -1,j);
				if (i>0 && j > 0)
					integral_image.atXY(i,j) -= integral_image.atXY(i -1,j -1);
				//integral_image.atXY(i,j) = ip_vec.atXY(i,j) + integral_image.atXY(i-1,j) + integral_image.atXY(i,j-1) - integral_image.atXY(i-1,j-1);
			}
		}
		return integral_image;
	}
	 */



protected:
  // extract features from an image, which in this case just involves resampling and 
  // rearranging into a vector of pixel data.


  static const int size=40;  // subsampled image resolution
  map<string, CImg<double> > models; // trained models
  static const int no_of_eigen_vecs = 10;
};
