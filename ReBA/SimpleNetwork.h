#pragma once
#define BOUNDS_CHECK
#include "Constant.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <set>
#include <map>
#include <cmath>
#include<queue>
#include<deque>
//#include <ext/hash_map>
#include <memory>
#include <time.h>
#include <stdlib.h>
//using namespace __gnu_cxx;
// import most common Eigen types
//USING_PART_OF_NAMESPACE_EIGEN;
using namespace std;

#define  ARRAYSIZE(array) ( sizeof(array)/sizeof(array[0]) )

class SimpleNetwork
{
	int degreeCounter;
	//int x;
	bool isDirected;
	bool isStartFromZero;
	string whichSet;
public:
	vector<vector <int> > oriTopoNetwork;
	vector<int> user_user_relation[nodeNum];
	double attechedProbablity[nodeNum];
	SimpleNetwork(bool = true);
	SimpleNetwork(int,int);
	~SimpleNetwork(void);
	int loadNetworkFromFile(string filename);
	int loadNetworkFromFile(string filename,bool startFromZero);
	void whoami();
	void init();
	void initUserItemMatrix();//�����ݽṹ�ӵ���ת��Ϊ����
	void empty();
	bool addEdge(int, int);
	int addGrandNode();
	void addEdge(vector<int>*);
	void addEdgeToTempNetwork(int, int);
	int removeEdge(int, int);
	int removeDirectedEdge(int a, int b);
	void removePoint(int);
	void removePoint(int,vector<int>[]);
	void setDirected(bool dirc);
	int countDegree(int flag=2);
	int getUserMaxDegree();
	int getUserMinDegree();
	int getItemMaxDegree();
	void resetgraph();
	void printNetwork(const char* filename);
	void testSymmDegree();
	bool isDirectedNet();
	bool haveUndealedEdge();
	//int HeatS();
	//int ProbS();
	//int ProbSWithSelfAvoiding();
	//int NCF(double beta);
	//int MCF(double beta);
	//int hybirdHAndPNonLinaer(double lamada=0.18);
	//int Heter_NBI(double lamada = -0.8);
	//int RE_NBI(double lamada = -0.74);
	//int PD(double lamada = -0.82);
	//int Heter_PD(double sita=-0.24, double lamada = -0.78);
	//int Biased_Heat(double lamada=0.18);
	//int Heter_hybrid(double sita=-0.24,double lamada=0.18);
	//int SPD(double sita=1.95,double lamada=0.31);
	//int UHHP_Train(int user, double lamada=0.18);
	//int UHHP_Test();
	//int GrandNodeHybird(double lamada=0.083);
	//int WeightiedGrandNodeHC(double lamada=0.083);
	//int GrandNodeHybirdMatrix(double lamada);
	//int WeightiedGrandNodeHybird(double lamada,double weight);
	//int ExtractingBackbone(double lamada,double percent = 0.5);
	//int MatrixHybird(double lamada);
	//int MatrixHybirdReal(double lamada);
	//int B_Rank();
	//int IFFSparse(int times);
	//int Cold_Start(double gama);
	//int URA_NBI(double lamada);
	//int WHC(double lamada);
	//int IHC(double grma);
	//int IHCMatrix(double grma);
	//int HHCMatrix(double grma);
	//int TheUtilmateMD(double sita,double grma, double efsilong);
	//int Heter_UtilmateMD(double sita,double grma, double efsilong);
};

