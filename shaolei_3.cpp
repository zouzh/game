// shaolei.cpp -- achieve shaolei using c++
//                the region of game is at least 10*10, and at most 30*30
//                when someone begin the game, he won't step at landmine
//                and there hasn't any landmine around the first
//                click position. So we must arrange the landmine randomly
//                after the gamer click the game region. 
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
class node
{
private:
  int * pt;
  node * next;
public:  node() {pt=0;next=0;}
  node(int * p, node * t = 0)  {pt=p;}
  node * add(int * p)
  {next = new node (p);return next;}
  int * get_pt() {return pt;}
  node * get_next() {return next;}
  ~node() {};
};
class hdtl
{
private:
  node * head;
  node * tail;
public: hdtl() {head=0;tail=0;}
  hdtl(node * p1, node * p2)
  {head=p1;tail=p2;}
  node * get_head() {return head;}
  node * get_tail() {return tail;}
  void joint(hdtl * h1)
  {
    node * temp = tail->get_next();
    temp = h1->get_head();
    tail = h1->get_tail();
    h1=this;
  }
  void seth(node * h)
  {head = h;}
  void setl(node * t)
  {tail = t;}
  ~hdtl() {};
};
typedef node * linkn;
typedef hdtl * linkh;
void arrange(int *, int, int, int, int, int);
void atob(int *, int *, int, int, int);
void inic(int, int, linkh);
void btoc(int *, linkh, int, int);
void getd(int, int, int *, linkn);
void output(int *, int, int);
void outputc(linkh, int, int);
void set(int, int, int, int, int [], int *,linkh);
void edge_row(int, int, int [], int *, linkh);
void edge_col(int, int, int, int [], int *, linkh);
int main(int argc, char *argv[])
{
  using namespace std;
  int m=atoi(argv[1]);
  int n=atoi(argv[2]);
  if (m<6||n<6)
    cout << "Error, the region is at least 10*10" << endl;
  else if (m>100||n>30)
    cout << "Error, the region is at most 100*30" << endl;
  else
    {
      int num=atoi(argv[3]);
      int temp = m*n;
      if (num > int(temp*0.8))
	cout << "Error. There are too few landmines or too many landmines.\n";
      else
	{
	  srand(time(0));
	  int * arr=new int [num];  // the information of positions of landmines
	  // brr store the information of distribution of landmines.
	  int * brr=new int [(m+2)*(n+2)]; 
	  linkh crr = new hdtl [m*n];
	  int * drr=new int [m*n];
	  int fa=rand()%m+1;
	  int fb=rand()%n+1;
	  arrange(arr,m,n,num,fa,fb);
	  atob(arr,brr,num,m+2,n+2);
	  output(brr,m+2,n+2);
	  //inic(m,n,crr);
	  btoc(brr,crr,m,n);
	  outputc(crr,m,n);
	  //getd(m+2,n+2,brr,crr[0].get_head());
	  //output(brr,m+2,n+2);
	  //output(crr,m,n);
	  delete [] arr;
	  delete [] brr;
	  delete [] crr;
	}
    }
  return 0;
}
// arr is information of position of landmines,
// n is the number of landmines,
// a is row number, b is the column number,
// (fa,fb) is the first click position.
void arrange(int *arr,int a, int b, int n, int fa, int fb)
{
  srand(time(0));
  // the first click position has nine different situations:
  //    top_left       top_middle       top_right  (1*)
  // middle_left    middle_middle    middle_right  (2*)
  // bottom_left    bottom_middle    bottom_right  (3*)
  int is=fa-2, id=fa+1, js=fb-2, jd=fb+1; // middle_middle
  if (fa==1)       // (1*)
    is++;    
  else if (fa==a)  // (3*)
    id--;
  // else          // (2*) except middle_middle
  if (fb==1)
    js++;
  else if (fb==b)
    jd--;
  // eliminate the position around the first click;
  int len=a*b;
  int * trr = new int [len];
  int temp,sub;
  for (int i=0;i<len;i++)
    trr[i]=i;
  for (int i=is;i<id;i++)
    for (int j=js;j<jd;j++)
      {
	len--;
	sub=i*b+j;
	temp = trr[sub];
	trr[sub] = trr[len];
	trr[len] = temp;
      }
  // begin arrange landmines
  while (n>0)
    {
      sub=rand()%len;
      len--;
      n--;
      temp = trr[sub];
      trr[sub] = trr[len];
      trr[len] = temp;
      arr[n] = temp;
    }
  delete [] trr;
}
// According to arr, computer the number of landmines
// around the given position, and store the information
// in brr.
void atob(int *arr, int *q, int num, int m, int n)
{
  int (*brr)[n]=(int (*)[n]) q;
  for (int i=0;i<m;i++)
    for (int j=0;j<n;j++)
      brr[i][j]=0;
  int bi,bis,bid,bj,bjs,bjd;
  for (int i=0;i<num;i++)
    {
      bis=arr[i]/(n-2);
      bjs=arr[i]%(n-2);
      bid = bis+3;
      bjd = bjs+3;
      for (bi=bis;bi<bid;bi++)
	for (bj=bjs;bj<bjd;bj++)
	  brr[bi][bj]++;
    }
   for (int i=0;i<num;i++)
    {
      bi=arr[i]/(n-2)+1;
      bj=arr[i]%(n-2)+1;
      brr[bi][bj]=999;
    }
}
void inic(int m, int n, linkh crr)
{
  int sub;
  for (int i=0;i<m;i++)
    for (int j=0;j<n;j++)
      {
	sub=i*n+j;
	crr[sub]=hdtl();
      }      
}
void btoc(int *p, linkh crr, int m, int n)
{
  int (*brr)[n+2]=(int (*)[n+2]) p;
  linkn pt;
  // the four corners, the four edges and the center rigion
  int cor1[6]={0,1,1,0,0,-1};              // top_left
  int cor2[6]={1,0,0,-1,-1,0};             // top_right
  int cor3[6]={-1,0,0,1,1,0};              // bottom_left
  int cor4[6]={0,-1,-1,0,0,1};             // bottom_right
  int matt[10]={0,-1,1,0,0,1,0,1,-1,0};    // top
  int matb[10]={0,-1,-1,0,0,1,0,1,1,0};    // bottom
  int matr[10]={-1,0,0,-1,1,0,1,0,0,1};    // right 
  int matl[10]={-1,0,0,1,1,0,1,0,0,-1};    // left  
  int matc[16]={-1,1,0,-1,0,-1,1,0,1,0,0,1,0,1,-1,0};  //center
  // the four corners
  if (brr[1][1]==0)      // up_left
    set(1,1,n,3,cor1,p,crr);
  if (brr[1][n]==0)      // up_right
    set(1,n,n,3,cor2,p,crr);   
  if (brr[m][1]==0)      // down_left
    set(m,1,n,3,cor3,p,crr);
  if (brr[m][n]==0)      // down_right
    set(m,n,n,3,cor4,p,crr);
  // the four edges;
  edge_row(1,n,matt,p,crr);    // top
  edge_col(1,m,n,matl,p,crr);  // left
  edge_col(n,m,n,matr,p,crr);  // right
  edge_row(m,n,matb,p,crr);
  // the center region 
}
void set(int is,int js, int n, int len, int mat[], int * p, linkh crr)
{
  int (*brr)[n+2]=(int (*)[n+2]) p;
  int i=is-1,j=js-1;
  linkn pt = new node (&brr[is][js]);
  crr[i*n+j].seth(pt);
  for (int k=0;k<len;k++,k++)
    {
      is += mat[k];
      js += mat[k+1];
      pt=pt->add(&brr[is][js]);
    }
  crr[i*n+j].setl(pt);
  delete pt;
}
void edge_row(int m, int n, int mat[], int *p, linkh crr)
{  
  int (*brr)[n+2]=(int (*)[n+2]) p;
  linkn pt;
  int sr=m;
  if (m!=1)
    sr--;
  for (int i=2;i<n;i++)
    {
      if (brr[m][i]==0) 
	{
	  if (brr[m][i-1]!=0)
	    set(m,i,n,5,mat,p,crr);
	  else
	    {
	      pt = crr[(m-1)*n+i-2].get_tail();
	      pt = pt->add(&brr[sr][i+1]);
	      pt = pt->add(&brr[sr+1][i+1]);
	      linkn temp = crr[(m-1)*n+i-2].get_tail();
	      temp=pt;
	      crr[(m-1)*n+i-1]=crr[(m-1)*n+i-2];
	    }
	}
    }
  if (brr[m][n-1]==0)
    crr[m*n-1]=crr[m*n-2];
}
void edge_col(int c, int m, int n, int mat[], int *p, linkh crr)
{  
  int (*brr)[n+2]=(int (*)[n+2]) p;
  linkn pt;
  int sc;
  if (c!=1)
    sc--;
  for (int i=2;i<m;i++) 
    {
      if (brr[i][c]==0) 
	{
	  if (brr[i-1][c]!=0)
	    set(i-1,c,n,5,mat,p,crr);
	  else
	    {
	      pt = crr[(i-2)*n+c-1].get_tail();
	      pt = pt->add(&brr[i+1][sc]);
	      pt = pt->add(&brr[i+1][sc+1]);
	      linkn temp = crr[(i-2)*n+c-1].get_tail();
	      temp=pt;
	      crr[(i-1)*n+c-1]=crr[(i-2)*n+c-1];
	    }
	}
    }
  if (brr[m-1][c]==0)
    crr[(m-1)*n+c-1]=crr[(m-2)*n+c-2];
}
void getd(int m, int n, int *p, linkn head)
{
  int (*brr)[n]=(int (*)[n]) p;
  int * temp;
  while (!(head->get_next()))
    {
      temp=head->get_pt();
      *temp = 999;
      head = head->get_next();
    }
}
void output(int *p, int a, int b)
{
  int (*crr)[b]=(int (*)[b]) p;
  using namespace std;
  for (int i=0;i<a;i++)
    {
      for (int j=0;j<b;j++)
	{
	  if (crr[i][j]==999)
	    cout << " " << crr[i][j];
	  else
	    cout << "   "<< crr[i][j];
	}
      cout << endl;
    }
  cout << endl;
}
void outputc(linkh p, int a, int b)
{
  int sub;
  using namespace std;
  for (int i=0;i<a;i++)
    {
      for (int j=0;j<b;j++)
	{
	  sub = i*b+j;
	  cout << p[sub].get_head() << "  ";
	}
      cout << endl;
    }
  cout << endl;
  for (int i=0;i<a;i++)
    {
      for (int j=0;j<b;j++)
	{
	  sub = i*b+j;
	  cout << (p[sub].get_head())->get_pt() << "  ";
	}
      cout << endl;
    }
  cout << endl;
}
