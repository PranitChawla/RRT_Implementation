#include <iostream>
#include <cstdlib>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
using namespace std;
#include <stack>
typedef struct 
{
	Point p;
	Point parent;
}node;
vector <node> path;
bool isvalid(int i,int j,Mat b)
{
	return ((i >= 0) && (i < b.rows) &&
           (j >= 0) && (j < b.cols));
}
bool isblack (int m,int n,Mat a)
{
	if ((a.at<Vec3b>(m,n)[0]==0)&&(a.at<Vec3b>(m,n)[0]==0)&&(a.at<Vec3b>(m,n)[0]==0))
		return true;
	else 
		return false;
}
bool isred (int m,int n,Mat a)
{
	if ((a.at<Vec3b>(m,n)[0]==0)&&(a.at<Vec3b>(m,n)[1]==0)&&(a.at<Vec3b>(m,n)[2]==254))
		return true;
	else 
		return false;
}
float distance(int i,int j,int l,int m)
{
		return (sqrt((i-l)*(i-l)+(j-m)*(j-m)));
}
bool end(Mat visit,Point dest)
{
	int i,j;
	for (i=dest.y-10;i<=dest.y+10;i++)
	{
		for (j=dest.x-10;j<=dest.x+10;j++)
		{
			if (visit.at<uchar>(i,j)==255)
				return true;
		}
	}
	return false;
}
Point mindistance(Point random)
{
	int i;
	Point min=path[0].p;
	float mindist;
	float dist;
	mindist=sqrt((random.x-path[0].p.x)*(random.x-path[0].p.x)+(random.y-path[0].p.y)*(random.y-path[0].p.y));
	for(i=0;i<path.size();i++)
	{
		dist=sqrt((random.x-path[i].p.x)*(random.x-path[i].p.x)+(random.y-path[i].p.y)*(random.y-path[i].p.y));
		if (dist<mindist)
			{	
			mindist=dist;
			min=path[i].p;
			}

	}
	return min;
}
Point findpt(Mat a,int col)
{
	int sumx=0;
	int sumy=0;
	int count=0;
	int i,j;
	int c,d;
	if (col==1)
	{
		c=0;
		d=2;
	}
	else if(col==2)
	{
		c=0;
		d=1;
	}

	for (i=0;i<a.rows;i++)
	{
		for (j=0;j<a.cols;j++)
		{
			if (a.at<Vec3b>(i,j)[col]>253&&a.at<Vec3b>(i,j)[c]==0&&a.at<Vec3b>(i,j)[d]==0)
			{
				sumy=sumy+i;
				sumx=sumx+j;
				count++;
			}
		}
	}
	Point s;
	s.x=sumx/count;
	s.y=sumy/count;
	return (s);
}
bool lineblock(Point nextclosest,Point closest,Mat temp)
{
		float dis;
		dis=distance(nextclosest.x,nextclosest.y,closest.x,closest.y);
		float tan;
		if (nextclosest.x!=closest.x)
		tan=(nextclosest.y-closest.y)/(nextclosest.x-closest.x);
		float theta;
		theta=atan(tan);
		Point a;
		Point b;
		if (nextclosest.x>closest.x)
		{
			a=closest;
			b=nextclosest;
		}
		else 
		{
			a=nextclosest;
			b=closest;
		}
		int x,y;
		float d=0;
		while(x<=b.x)
		{
			x=a.x+d*cos(theta);
			y=a.y+d*sin(theta);
			d=d+0.1*dis;
			if (isblack(y,x,temp))
				return true;
		}
		return false;
		

}
int main()
{
	Mat a;
	a=imread("rrt.jpg",1);
	Mat b;
	b=a.clone();
	Point src(2,5);
	src=findpt(a,1);
	Point dest(50,50);
	dest=findpt(a,2);
	node source;
	source.p=src;
	source.parent=src;
	path.push_back(source);
	b.at<Vec3b>(src.y,src.x)[0]=255;
	Mat visit(a.rows,a.cols,CV_8UC1,Scalar(0));
	visit.at<uchar>(src.y,src.x)=255;
	Point nextclosest;
	while(!end(visit,dest))
	{
		Point random;
		random.x=rand()%(a.cols-0)+0;
		random.y=rand()%(a.rows-0)+0;
		Point closest;
		closest=mindistance(random);
		float tan;
		if (random.x!=closest.x)
		tan=(random.y-closest.y)/(random.x-closest.x);
		float theta;
		theta=atan(tan);
		int rx,ry,cx,cy;
		rx=random.x;
		ry=random.y;
		cx=closest.x;
		cy=closest.y;
		float dis=distance(rx,ry,cx,cy);
		if (dis<1)
			nextclosest=random;
		else
		{
		if (closest.x>random.x)
			nextclosest.x=closest.x-1*cos(theta);
		else
			nextclosest.x=closest.x+1*cos(theta);
		nextclosest.y=closest.y+1*sin(theta);
		if (isvalid(nextclosest.y,nextclosest.x,a)&&(!isblack(nextclosest.y,nextclosest.x,a)))
		{
		if (visit.at<uchar>(nextclosest.y,nextclosest.x)==0)
		{
			node temp;
			temp.p=nextclosest;
			temp.parent=closest;
			path.push_back(temp);
			visit.at<uchar>(nextclosest.y,nextclosest.x)=255;
			if (!lineblock(nextclosest,closest,a))
				line(b,nextclosest,closest,Scalar(255,0,0),1,CV_AA);
		}
		}
		}
		
}
int t;
for (t=0;t<path.size();t++)
{
	if (path[t].p.x==nextclosest.x&&path[t].p.y==nextclosest.y)
		break;
}
cout<<path.size()<<endl;
cout<<t;
int row=nextclosest.x;
int col=nextclosest.y;
vector <Point> showpath;
while(row!=src.x&&col!=src.y)
{
	int t;
	for (t=0;t<path.size();t++)
	{
		if (row==path[t].p.x&&col==path[t].p.y)
			break;
	}
	row=path[t].parent.x;
	col=path[t].parent.y;
	showpath.push_back(Point(row,col));	
}
int r;
for (r=0;r<showpath.size()-1;r++)
{
	line(b,showpath[r],showpath[r+1],Scalar(180,105,255),2,CV_AA);
}
cout<<endl;
cout<<showpath.size();
cout<<endl;
namedWindow("b",WINDOW_NORMAL);
imshow("temp",visit);
imshow("b",b);
waitKey(0);
}
	

