class Triple {
public:
	int x;
	int y;
	int d;
	Triple() { x=0, y=0; d=0;  }
	(int n1, int n2, int n3) { x=n1; y=n2; d=n3;  }
		
};

CImg<double> IntegralImage(CImg<double> input) //generate the Integral Image of an input image
{
	int w=input._width ; 
	h=input._height;
	CImg<double> output(w, l, 1, 3, 0);
	 	for (int j = 0; j < h; j++) 
	 		{ for (int i= 0; i < w; i++) 
	 			{ for (int k = 0; k < j+1 ; k++) 
	 				{ for (int m = 0; m < i+1 ; m++) 
						{output(i,j,0,1)=output(i,j,0,1)+input(k,m,0,1);
						output(i,j,0,2)=output(i,j,0,2)+input(k,m,0,2);
						output(i,j,0,3)=output(i,j,0,3)+input(k,m,0,3);
						}
					}
				}
					
			}
return output ; 
}
vector<Pair> generateFeature(CImg<double> input) 	//Generate 1500 features of different sizes and positions

{	int w=input._width;
	h=input._height;
	nfeatures= 1500 ;
	maxDiam= 20 ;
	ycoor, xcoor ;
	vector<Triple> feature;
	/
	for (int i=0; i<nfeatures; i++)
	{	xcoor=rand()%w;
		ycoor=rand()%h;
		diam = rand()%maxDiam;
		feature[i].x=xcoor;
		feature[i].y=ycoor;
		feature[i].d=diam ; 	
	}
	
	return feature ; 
}

vector<int> featurevector(CImg<double> input)
{ vector<int> featurevector ; 
 CImg<double> II=IntegralImage(input); 
	for(int i=0, i<500,i++ ) //generate two-rectangle feature and store the ouput in the feature vector
	{	featurevector[i]= II(feature[i].x,feature[i].y)+II(feature[i].x+feature[i].d,feature[i].y+feature[i].d)-II(feature[i].x+feature[i].d,feature[i].y)-II(feature[i].x,feature[i].y+feature[i].d)-
		(II(feature[i].x +feature[i].d,feature[i].y)+II(feature[i].x+2*feature[i].d,feature[i].y+feature[i].d)-II(feature[i].x+2*feature[i].d,feature[i].y)-II(feature[i].x+feature[i].d,feature[i].y+feature[i].d));
	}
	for(int i=500, i<1000,i++ ) //generate three-rectangle feature and store the ouput in the feature vector
	{	featurevector[i]= II(feature[i].x,feature[i].y)+II(feature[i].x+feature[i].d,feature[i].y+feature[i].d)-II(feature[i].x+feature[i].d,feature[i].y)-II(feature[i].x,feature[i].y+feature[i].d)
		+ II(feature[i].x+2*feature[i].d,feature[i].y)+II(feature[i].x+3*feature[i].d,feature[i].y+feature[i].d)-II(feature[i].x+3*feature[i].d,feature[i].y)-II(feature[i].x+2*feature[i].d,feature[i].y+feature[i].d)
		-(II(feature[i].x+feature[i].d,feature[i].y)+II(feature[i].x+2*feature[i].d,feature[i].y+feature[i].d)-II(feature[i].x+2*feature[i].d,feature[i].y)-II(feature[i].x+feature[i].d,feature[i].y+feature[i].d))
	}
	for(int i=0, i<500,i++ ) //generate four-rectangle feature and store the ouput in the feature vector
	{	featurevector[i]= II(feature[i].x,feature[i].y)+II(feature[i].x+feature[i].d,feature[i].y+feature[i].d)-II(feature[i].x+feature[i].d,feature[i].y)-II(feature[i].x,feature[i].y+feature[i].d)
		+ II(feature[i].x+feature[i].d,feature[i].y+feature[i].d)+II(feature[i].x+2*feature[i].d,feature[i].y+2*feature[i].d)-II(feature[i].x+2*feature[i].d,feature[i].y+feature[i].d)-II(feature[i].x+feature[i].d,feature[i].y+2*feature[i].d)
		- (II(feature[i].x+feature[i].d,feature[i].y)+II(feature[i].x+2*feature[i].d,feature[i].y+feature[i].d)-II(feature[i].x+2*feature[i].d,feature[i].y)-II(feature[i].x+feature[i].d,feature[i].y+feature[i].d))
		- (II(feature[i].x,feature[i].y+feature[i].d)+II(feature[i].x+feature[i].d,feature[i].y+2*feature[i].d)-II(feature[i].x+feature[i].d,feature[i].y+feature[i].d)-II(feature[i].x,feature[i].y+2*feature[i].d));
	} 
	}	
	return featurevector ;
}



