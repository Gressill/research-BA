#pragma once;
#include <string>
using namespace std;

class Constant{
    //movielens
    public:
//        const static int nodeNum ;//= 943;
//        const static int itemNum ;//= 1682;
//        const static char* inFileName;// = "../recomdationReviewVS/Data/movielens/movielens.dat";//polblogs1  realPolblogbeginfrom1
//        const static std::string DATAPATH;// = "../recomdationReviewVS/Data/movielens/";
};
	const int nodeNum = 5000;
	//const int TOPL_CONFIG=20;
	//const int TOPL=50;
	const int HASHLENGTH= 3000;

	//delicious
	//const static int nodeNum = 10000;//943;
	//const static int itemNum = 232657;//1682;
	//const static char* inFileName = "../recomdationReviewVS/Data/delicious_pnas/delicious_pnas.txt";//polblogs1  realPolblogbeginfrom1
	//const static std::string DATAPATH = "../recomdationReviewVS/Data/movielens/";
	//"../recomdationReviewVS/Data/movielens/";

    //movielens
    const static char* inFileName = "../recomdationReviewVS/Data/movielens/movielens.dat";//polblogs1  realPolblogbeginfrom1
	const static std::string DATAPATH = "../recomdationReviewVS/Data/movielens/";
	const static string resultfilepath = "F:/Documents/Dropbox/program/project/recomdationReviewVS/Result/movielens/";


	const static char* outFileName = "../answer/answer.txt";
	const static char* smartoutFileName = "D:/smartpolanswer.txt";
	//const static std::string configFile = "../recomdationReviewVS/config.ini";
	const static std::string configFile = "config.ini";

	const bool doPrint = true;
	/*int topoNetwork[PointNum][PointNum];
	int oriTopoNetwork[PointNum][PointNum];*/
	//#define GETSIZE(c) (sizeof(c)/sizeof(c[0]));
