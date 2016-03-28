
#define SIZE2 100

#include "CImg.h"
#include <ctime>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <vector>
#include <Sift.h>
#include <sys/types.h>
#include <dirent.h>
#include <map>
#include <numeric>
#include <fstream>

using namespace std;


//string category[] = {"bagel","bread","brownie","chickennugget",
//                           "churro","croissant","frenchfries","hamburger",
//                           "hotdog","jambalaya","kungpaochicken","lasagna",
//                           "muffin","paella","pizza","popcorn","pudding",
//                           "salad","salmon","scone","spaghetti","sushi",
  //                         "taco","tiramisu","waffle"};

class haar : public Classifier
{
public:
struct Triple{
	int x;
	int y;
	int d;
};
CImg<unsigned char> IntegralImage(CImg<unsigned char> input) //generate the Integral Image of an input image
{
//cout << "14" << endl;
	int w=input._width ; 
	int h=input._height;
	CImg<unsigned char> output(w, h);
	 	for (int j = 0; j < h; j++) 
	 		{ for (int i= 0; i < w; i++) 
	 			{ for (int k = 0; k < j ; k++) 
	 				{ for (int m = 0; m < i ; m++) 
						{
							/*output(i,j,0,1)=output(i,j,0,1)+input(k,m,0,1);
						output(i,j,0,2)=output(i,j,0,2)+input(k,m,0,2);
						output(i,j,0,3)=output(i,j,0,3)+input(k,m,0,3);*/
						output.atXY(i,j)=output.atXY(i,j)+input.atXY(k,m);
						//output.atXYZC(i,j,0,2)=output.atXYZC(i,j,0,2)+input.atXYZC(k,m,0,2);
						//output.atXYZC(i,j,0,3)=output.atXYZC(i,j,0,3)+input.atXYZC(k,m,0,3);
						}
					}
				}
					
			}
//cout << "15" << endl;
return output ; 
}
vector<Triple> generateFeature(int size) 	//Generate 1500 features of different sizes and positions 

{	int maxDiam= 20 ;
	int	i,ycoor, xcoor, diam ;
	vector<Triple> feature;
	
	for (int i=0; i<1500; i++)
	{	xcoor=rand()%SIZE2;
		ycoor=rand()%SIZE2;
		
		diam = rand()%maxDiam;
		//cout << xcoor << "," << ycoor << "," << diam << endl;
		Triple temp;
		temp.x=xcoor;
		temp.y=ycoor;
		temp.d=diam ; 
		feature.push_back(temp);	
	}
	
	return feature ; 
}

vector<int> featurevector(CImg<unsigned char> II, vector<Triple> feature)
{ 
vector<int> feature_vector ;
 //cout << "size" << feature.size()<< endl;
//cout << "13" << endl;
	for(int i=0; i<500;i++ ) //generate two-rectangle feature and store the ouput in the feature vector
	{	feature_vector.push_back( II(feature[i].x,feature[i].y)+II(feature[i].x+feature[i].d,feature[i].y+feature[i].d)-II(feature[i].x+feature[i].d,feature[i].y)-II(feature[i].x,feature[i].y+feature[i].d)-
		(II(feature[i].x +feature[i].d,feature[i].y)+II(feature[i].x+2*feature[i].d,feature[i].y+feature[i].d)-II(feature[i].x+2*feature[i].d,feature[i].y)-II(feature[i].x+feature[i].d,feature[i].y+feature[i].d)));
	}
	//cout << "20" << endl;
	for(int i=500; i<1000;i++ ) //generate three-rectangle feature and store the ouput in the feature vector
	{	feature_vector.push_back(II(feature[i].x,feature[i].y)+II(feature[i].x+feature[i].d,feature[i].y+feature[i].d)-II(feature[i].x+feature[i].d,feature[i].y)-II(feature[i].x,feature[i].y+feature[i].d)
		+ II(feature[i].x+2*feature[i].d,feature[i].y)+II(feature[i].x+3*feature[i].d,feature[i].y+feature[i].d)-II(feature[i].x+3*feature[i].d,feature[i].y)-II(feature[i].x+2*feature[i].d,feature[i].y+feature[i].d)
		-(II(feature[i].x+feature[i].d,feature[i].y)+II(feature[i].x+2*feature[i].d,feature[i].y+feature[i].d)-II(feature[i].x+2*feature[i].d,feature[i].y)-II(feature[i].x+feature[i].d,feature[i].y+feature[i].d)));
	}
	//cout << "21" << endl;
	for(int i=1000; i<1500;i++ ) //generate four-rectangle feature and store the ouput in the feature vector
	{	feature_vector.push_back( II(feature[i].x,feature[i].y)+II(feature[i].x+feature[i].d,feature[i].y+feature[i].d)-II(feature[i].x+feature[i].d,feature[i].y)-II(feature[i].x,feature[i].y+feature[i].d)
		+ II(feature[i].x+feature[i].d,feature[i].y+feature[i].d)+II(feature[i].x+2*feature[i].d,feature[i].y+2*feature[i].d)-II(feature[i].x+2*feature[i].d,feature[i].y+feature[i].d)-II(feature[i].x+feature[i].d,feature[i].y+2*feature[i].d)
		- (II(feature[i].x+feature[i].d,feature[i].y)+II(feature[i].x+2*feature[i].d,feature[i].y+feature[i].d)-II(feature[i].x+2*feature[i].d,feature[i].y)-II(feature[i].x+feature[i].d,feature[i].y+feature[i].d))
		- (II(feature[i].x,feature[i].y+feature[i].d)+II(feature[i].x+feature[i].d,feature[i].y+2*feature[i].d)-II(feature[i].x+feature[i].d,feature[i].y+feature[i].d)-II(feature[i].x,feature[i].y+2*feature[i].d)));
	} 
		//cout << "12" << endl;
	return feature_vector ;
}
  map<string, int> category_map;
  haar(const vector<string> &_class_list) : Classifier(_class_list) {
    for(int i=0; i<25;i++){
      category_map[category[i]] = i;
    }
  }

vector<Triple> feature=generateFeature(SIZE2);
  void write_data(int category,
                  CImg<unsigned char> output,
                  ofstream & myfile) 
  {
  	//cout << "10" << endl;

    myfile << category << " ";
    CImg<unsigned char> II=IntegralImage(output);
    for (int i = 0; i < 1500 ; i++)
    	{
    		//cout << "11" << endl;
    		vector<int> feature_vector= featurevector(II, feature);
        //cout << "30" << endl;
        myfile << i << ":" <<  feature_vector[i] << " ";
      }
    
    myfile << endl;

  }

  void make_data(const Dataset &filenames, string name){
  	//cout << "3" << endl;
    ofstream myfile;
    myfile.open(("haar/" + name + ".data").c_str());
    for (Dataset::const_iterator c_iter = filenames.begin(); c_iter != filenames.end(); ++c_iter)
    {
    	//cout << "4" << endl;
      cout << "Processing " <<name <<"/" << c_iter->first << endl;
      // convert each image to be a row of this "model" image
      for (int i = 0; i < c_iter->second.size(); i++) 
      {
      	//cout << "5" << endl;
        CImg<unsigned char> query_image(c_iter->second[i].c_str());
        query_image = query_image.get_RGBtoYCbCr().get_channel(0);
        query_image.resize(SIZE2, SIZE2);
        //cout << i << endl;
        write_data(category_map[c_iter->first.c_str()]+1, query_image, myfile);
      }
    }
    //cout << "7" << endl;
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
  	//cout << "2" << endl;
    make_data(filenames, "train");
    // make_data(filenames, "test");
//cout << "8" << endl;
    system ("./svm_multiclass/svm_multiclass_learn -c 50 haar/train.data haar/model > .info");
  }

  virtual string classify(const string &filename)
  {
    ofstream myfile;
    myfile.open(".temp");
    CImg<unsigned char> query_image(filename.c_str());
    query_image = query_image.get_RGBtoYCbCr().get_channel(0);
    query_image.resize(SIZE2, SIZE2);
    int temp = filename.find("/");
    string category_name = filename.substr(temp+1, filename.find("/"));
    write_data(category_map[category_name]+1, query_image, myfile);
    myfile.close();
    system("./svm_multiclass/svm_multiclass_classify .temp haar/model haar/predict > .info");
    return read_predict_file("haar/predict");
  }

  virtual void load_model()
  {

  }

};



