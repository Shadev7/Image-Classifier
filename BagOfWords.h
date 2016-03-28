
#include <fstream>
#include <sstream>

using namespace std;

string category_b[] = {"bagel","bread","brownie","chickennugget",
                           "churro","croissant","frenchfries","hamburger",
                           "hotdog","jambalaya","kungpaochicken","lasagna",
                           "muffin","paella","pizza","popcorn","pudding",
                           "salad","salmon","scone","spaghetti","sushi",
                           "taco","tiramisu","waffle"};
class BagOfWords : public Classifier
{
public:
  BagOfWords(const vector<string> &_class_list) : Classifier(_class_list) {}
  	
	//Distance between k means centroid and point
	double sift_distance2(vector<double> k_point, SiftDescriptor ip)
	{
		  
		double dist2 = 0;
		double temp;
		for (int i=0; i<128; i++)
		{
			 temp = k_point[i] - ip.descriptor[i];
			 dist2 += temp*temp;
		}
		return dist2;
	}
	
	//Classify a sift descriptor to a bag
	int get_bag(vector<vector<double> > k_points,SiftDescriptor ip, int k)
	{
		double min_dist = std::numeric_limits<double>::max();
		int bag;
		double temp;

		for (int i=0; i<k; i++)
		{
			temp = sift_distance2(k_points[i], ip);
			if (min_dist > temp)
			{
				min_dist = temp;
				bag = i;
			}
		}

		return bag;
	}
	
	//Generate bag of words from sift descriptors
	vector<int> image_bag(CImg<double> ip_img,vector<vector<double> > k_points, int k)
	{
		//int k = k_points.size();
		
		CImg<double> gray = ip_img.get_RGBtoYCbCr().get_channel(0);;
		gray.resize(size, size);
		vector<SiftDescriptor> descriptors = Sift::compute_sift(gray);
		int bag;
		vector<int> bag_vec;
		CImg<double> bag_vec_t(k,1);
		for (int i =0; i<k;i++)
		{
			bag_vec.push_back(0);
		}

		for (int i =0; i<descriptors.size();i++)
		{
			bag = get_bag(k_points, descriptors[i], k);

			bag_vec[bag] = bag_vec[bag] + 1;
			
		}

		return bag_vec;
		
	} 
  // Eigen vector training. 
  virtual void train(const Dataset &filenames) 
  {
	  int k = 20;
	  int img_no = 0;
	  //vector<SiftDescriptor> k_points [k];
	  vector<vector<double> > k_points;
	  CImg <double> k_pt_img(128,k);
	  double temp;


	  
	//For k means
	cout << "K means clustering" << endl;
	
	//Sample for creating bag of words
	vector<SiftDescriptor> alldescriptors ;
    for(Dataset::const_iterator c_iter=filenames.begin(); c_iter != filenames.end(); ++c_iter)
      {
	
	CImg<double> class_vectors(k, filenames.size(), 1);
	


	
		for(int i=0; i<2; i++)
		{
			//img_no++;
			CImg<double> ip_img(c_iter->second[i].c_str());
			CImg<double> gray = ip_img.get_RGBtoYCbCr().get_channel(0);
			gray.resize(size, size);
			vector<SiftDescriptor> descriptors = Sift::compute_sift(gray);
			
			for (int j =0; j<descriptors.size(); j++)
			{
				alldescriptors.push_back(descriptors[j]);		
			}
		}
	}
	//KMeans begins
	cv::Mat points(alldescriptors.size(),128, CV_32F,Scalar(10));
	cv::Mat labels;
	cv::Mat centers(k, 1, points.type());
	
		for(int i =0;i<points.rows;i++)
		{
		  for(int j=0;j<points.cols;j++)
		  {
		   points.at<float>(i,j)=alldescriptors[i].descriptor[j];
		  }
		}
	cv::kmeans(points, k, labels, TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0), 3, KMEANS_PP_CENTERS, centers);
	

	//KMeans ends
	
	
	//convert Mat to vector

	for(int m =0;m<k;m++)
		{
			vector <double> temprow;
		  for(int j=0;j<128;j++)
		  {
		   temp = centers.at<float>(m,j);
		   temprow.push_back (temp);
		   k_pt_img.atXY(j,m) = temp;
		   //k_pt_img.atXY(m,j) = temp;
		  }
		  k_points.push_back(temprow);
	}
	k_pt_img.save_png("bow/k_means_points.png"); // to be used while classifying
	

	
	//BOW
	
	ofstream myfile;
    myfile.open ("bow/train_bow.data");	
	int category = 0;
	for(Dataset::const_iterator c_iter=filenames.begin(); c_iter != filenames.end(); ++c_iter)
      {
		category = category_map(c_iter->first);
		cout << "Processing " << c_iter->first << endl;
		CImg<double> class_vectors(k, filenames.size(), 1);
		

		for(int i=0; i<c_iter->second.size(); i++)
		{

			CImg<double> ip_img(c_iter->second[i].c_str());

			vector <int> bags_dist = image_bag(ip_img,k_points,k);

			myfile << category << " ";
			for (int s = 0; s < k; s++) {
	 

				myfile << (s+1) << ":"<<  bags_dist[s] << " ";
			}
			 myfile << endl;

		}

      }
	  myfile.close();
	  system ("./svm_multiclass/svm_multiclass_learn -c 50 bow/train_bow.data bow/bow_model > .info");
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
    return category_b[value-1];
  }
  int category_map(string category_name)
  {
	  int category;
	  for (int i=0; i< (sizeof(category_b)/sizeof(category_b[0])); i++)
	  {
		  if (category_name == category_b[i])
		  {
			  category = i;
			  break;
		  }

	  }
	  return category;
  }
  virtual string classify(const string &filename)
  {
	
		//Read 
		vector<vector<double> > k_points;
		
		CImg<double> K_image("bow/k_means_points.png");
		int k = K_image.height();
		
		for (int i = 0; i < k; i++)
	  {
		   vector <double> temprow;
			for (int j=0; j<128; j++)
			{
				temprow.push_back (K_image.atXY(j,i));
			}
			k_points.push_back(temprow);
	  }

	  CImg<double> ip_img(filename.c_str());
	  vector <int> bags_dist = image_bag(ip_img,k_points,k);
	  
	ofstream myfile;
    myfile.open(".temp");

    string category_name = filename.substr(filename.find("/") + 1,filename.find_last_of("/") - filename.find("/") -1);
	

	int category = category_map(category_name)+1 ;
	myfile << category << " ";
	//Build n-dimensional vector
	for (int j=0; j<k ; j++)
	{

		myfile << (j+1) << ":"<< bags_dist[j] << " ";
	}
	myfile << endl;
	
    myfile.close();
    system("./svm_multiclass/svm_multiclass_classify .temp bow/bow_model bow/bow_predict > .info");
    return read_predict_file("bow/bow_predict");
  }

  virtual void load_model()
  {

  }
  CImg<double> extract_features(const string &filename)
    {
      return (CImg<double>(filename.c_str())).resize(size,size,1,3).unroll('x');
    }


protected:
  // extract features from an image, which in this case just involves resampling and 
  // rearranging into a vector of pixel data.


  static const int size=100;  // subsampled image resolution
  map<string, CImg<double> > models; // trained models
};
