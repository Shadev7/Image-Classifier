string category_b[] = {"bagel","bread","brownie","chickennugget",
                           "churro","croissant","frenchfries","hamburger",
                           "hotdog","jambalaya","kungpaochicken","lasagna",
                           "muffin","paella","pizza","popcorn","pudding",
                           "salad","salmon","scone","spaghetti","sushi",
                           "taco","tiramisu","waffle"};
#include <fstream>
using namespace std;
class BagOfWords : public Classifier
{
public:
  BagOfWords(const vector<string> &_class_list) : Classifier(_class_list) {}
  	
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
	
	int get_bag(vector<vector<double> > k_points,SiftDescriptor ip, int k)
	{
		double min_dist = std::numeric_limits<double>::max();
		int bag;
		double temp;
		//int k = k_points.size();
		for (int i=0; i<k; i++)
		{
			temp = sift_distance2(k_points[i], ip);
			//cout << "dist" << i << " : " << temp << endl;
			if (min_dist > temp)
			{
				min_dist = temp;
				bag = i;
			}
		}
		//cout << "mindist"<< " : " << min_dist << endl;
		//cout << "bag"<< " : " << bag<< endl;
		return bag;
	}
	
	/* CImg<double> image_bag(CImg<double> ip_img,vector<vector<double> > k_points, int k)
	{
		//int k = k_points.size();
		
		CImg<double> gray = ip_img.get_RGBtoYCbCr().get_channel(0);;
		gray.resize(size, size);
		vector<SiftDescriptor> descriptors = Sift::compute_sift(gray);
		int bag;
		CImg<double> bag_vec(k,1);
		for (int i =0; i<k;i++)
		{
			bag_vec.atXY(1,i) = i;
		}
		for (int i =0; i<k;i++)
		{
			cout << bag_vec.atXY(1,i);
		}
		cout << endl;
		cin.get();
		cout << "desc size : " << descriptors.size() <<endl;
		for (int i =0; i<descriptors.size();i++)
		{
			//cout << "4" << endl;
			bag = get_bag(k_points, descriptors[i], k);
			cout << "bag : " << bag <<endl;
			//cout << "bag at " << bag_vec.atXY(bag,1);
			//bag_vec.atXY(bag,1) += bag_vec.atXY(bag,1) + 1;
			//cin.get();
			bag_vec.atXY(1,bag) = bag_vec.atXY(1,bag) + 1;
			//cout << "5" << endl;
			
		}
		for (int i =0; i<k;i++)
		{
			cout << bag_vec.atXY(1,i) << ", ";
		}
		//int bag = get_bag(k_points, gray);
		cout << endl;
		cin.get();
		return bag_vec;		
		
	}  */
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
		/* for (int i =0; i<k;i++)
		{
			cout << bag_vec[i];
		} */
		//cout << endl;
		//cin.get();
		//cout << "desc size : " << descriptors.size() <<endl;
		for (int i =0; i<descriptors.size();i++)
		{
			//cout << "4" << endl;
			bag = get_bag(k_points, descriptors[i], k);
			//cout << "bag : " << bag <<endl;
			//cout << "bag at " << bag_vec.atXY(bag,1);
			//bag_vec.atXY(bag,1) += bag_vec.atXY(bag,1) + 1;
			//cin.get();
			bag_vec[bag] = bag_vec[bag] + 1;
			//cout << "5" << endl;
			
		}
		/* for (int i =0; i<k;i++)
		{
			cout << bag_vec[i] << ", ";
		}
		//int bag = get_bag(k_points, gray);
		cout << endl; */
		//cin.get();
		//return bag_vec_t;
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
	  /* for (int i = 0; i < k; i++)
	  {
		   //k_points[i] = gen_k();
		   vector <double> temprow;
			for (int j=0; j<128; j++)
			{
				temp = rand()%(256);
				temprow.push_back (temp);
				//k_pt_img.atXY(j,i) = temp;
				//descriptors
			}
			k_points.push_back(temprow);
	  } */
	  //k_pt_img.save_png("k_means_points.png");

	  
	//For k means
	cout << "K means clustering" << endl;
	
	//Sample for creating bag of words
	vector<SiftDescriptor> alldescriptors ;
    for(Dataset::const_iterator c_iter=filenames.begin(); c_iter != filenames.end(); ++c_iter)
      {
	
	CImg<double> class_vectors(k, filenames.size(), 1);
	
	//pair<int,SiftDescriptor> temppair;
	//cout << c_iter->second << endl;

	
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
	
	/* for(int i =0;i<centers.rows;i++)
		{
		  for(int j=0;j<centers.cols;j++)
		  {
		   cout << centers.at<float>(i,j)<< ",";
		  }
		  cout<< endl;
		} */
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
	k_pt_img.save_png("k_means_points.png"); // to be used while classifying
	
	/* for(int i =0;i<k;i++)
		{
		  for(int j=0;j<128;j++)
		  {
		   cout << k_points[i][j]<< ",";
		  }
		  cout<< endl;
		} */
	
	//BOW
	
	ofstream myfile;
    myfile.open ("train_bow.data");	
	int category = 0;
	for(Dataset::const_iterator c_iter=filenames.begin(); c_iter != filenames.end(); ++c_iter)
      {
		category += 1;
		cout << "Processing " << c_iter->first << endl;
		CImg<double> class_vectors(k, filenames.size(), 1);
		
		//cout << c_iter->second << endl;
		// convert each image to be a row of this "model" image
		for(int i=0; i<c_iter->second.size(); i++)
		{

			CImg<double> ip_img(c_iter->second[i].c_str());
			//CImg<double> im_bag (image_bag(ip_img,k_points,k));
			vector <int> bags_dist = image_bag(ip_img,k_points,k);
			//class_vectors = class_vectors.draw_image(0, i, 0, 0, im_bag);
			myfile << category << " ";
			for (int s = 0; s < k; s++) {
	 
				//myfile << (s+1) << ":"<<  ((int)im_bag.atXY(s,1) / 255.0) << " ";
				myfile << (s+1) << ":"<<  bags_dist[s] << " ";
			}
			 myfile << endl;
			//break;
		}
	  
	//cout << "3" << endl;
	//class_vectors.save_png(("bow_model." + c_iter->first + ".png").c_str());
	
	//break;
      }
	  myfile.close();
	  system ("./svm_multiclass/svm_multiclass_learn -c 50 train_bow.data bow_model > .info");
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
	cout << "cat " << category_b[value-1];
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
	  /* CImg<double> ip_img(filename.c_str());
    //CImg<double> test_image = extract_features(filename);
		CImg<double> gray = ip_img.get_RGBtoHSI().get_channel(2);
		gray.resize(size, size);
		vector<SiftDescriptor> descriptors = Sift::compute_sift(gray);
		int bag; */
		
		//Read 
		vector<vector<double> > k_points;
		
		CImg<double> K_image("k_means_points.png");
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
	   /* for (int i = 0; i < k; i++)
	  {
		   
			for (int j=0; j<128; j++)
			{	
				cout << k_points[i][j]<< ", ";
			}
			cout << endl;
	  }
	  cin.get();  */ 
	  CImg<double> ip_img(filename.c_str());
	  vector <int> bags_dist = image_bag(ip_img,k_points,k);
	  
	  cout << "a"<< endl;
	ofstream myfile;
    myfile.open(".temp");
cout << "b"<< endl;
    //int temp = filename.find("/");
    string category_name = filename.substr(filename.find("/") + 1,filename.find_last_of("/") - filename.find("/") -1);
	
    //write_data(category_map[category_name]+1, query_image, myfile);
	//int category = category_map[category_name]+1 ;
	//myfile << category << " ";
	int category = category_map(category_name)+1 ;
	myfile << category << " ";
	//Build n-dimensional vector
	for (int j=0; j<k ; j++)
	{

		myfile << j << ":"<< bags_dist[j] << " ";
	}
	myfile << endl;
	
    myfile.close();
	cout << "c"<< endl;
    system("./svm_multiclass/svm_multiclass_classify .temp bow_model bow_predict > .info");
	cout << "d"<< endl;
    return read_predict_file("bow_predict");

     
    // figure nearest neighbor
    /* pair<string, double> best("", 10e100);
    double this_cost;
    for(int c=0; c<class_list.size(); c++)
      for(int row=0; row<models[ class_list[c] ].height(); row++)
	if((this_cost = (bag_vec - models[ class_list[c] ].get_row(row)).magnitude()) < best.second)
	  best = make_pair(class_list[c], this_cost);

    return best.first; */
  }

  virtual void load_model()
  {
    /* for(int c=0; c < class_list.size(); c++)
      models[class_list[c] ] = (CImg<double>(("bow_model." + class_list[c] + ".png").c_str())); */
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
