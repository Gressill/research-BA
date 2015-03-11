#include <iostream>
#include "SimpleNetwork.h"

//#include "Snap.h"
//#include<stdlib.h>
using namespace std;

SimpleNetwork sn(false);

double nodeSequenceWithSingleMethod[nodeNum]; //use the sowing to predict the sequence
double nodeSequence[nodeNum]; //use the sowing to predict the sequence
double degreeBasedSequence[nodeNum]; //use the degree to estimate the sequence of node
//int node_node_importance[nodeNum][nodeNum];
double node_node_importance[nodeNum][nodeNum];
double degrrArray[nodeNum];
double KShellDergeeNum[nodeNum];
double no_discordant_or_concordant_paris = 0;
double degreeDistrbution;

const int sowingTimes = 100;

double probablity[nodeNum];
double probs = 0.20;
double neibergProbs = 0.1;

struct Ranker {
	long int id;
	double value;
	bool operator ==(const Ranker &d1) {
		return (d1.id == this->id);
	}
};

bool Greater(const Ranker & d1, const Ranker & d2) {
	return d1.value < d2.value;
}
;
bool Smaller(const Ranker & d1, const Ranker & d2) {
	return d1.value > d2.value;
}
;

void init() {
	memset(nodeSequence, 0, sizeof(nodeSequence));
	memset(probablity, 0, sizeof(probablity));
	memset(node_node_importance, 0, sizeof(node_node_importance));
	memset(nodeSequenceWithSingleMethod,0,sizeof(nodeSequenceWithSingleMethod));
	sn.empty();
	sn.addEdge(0, 1);
	sn.addEdge(0, 2);
}

int* ReverseArray(int*orig, unsigned short int b) {
	unsigned short int a = 0;
	int swap;
	for (a; a < --b; a++) //increment a and decrement b until they meet each other
			{
		swap = orig[a];       //put what's in a into swap space
		orig[a] = orig[b];    //put what's in b into a
		orig[b] = swap;       //put what's in the swap (a) into b
	}
	return orig;    //return the new (reversed) string (a pointer to it)
}

void genBA() {
	init();
	srand(0);        // init the random
	//srand( time(NULL) );        // init the random
	for (int i = 3; i < nodeNum; i++) {
		//New node have to connet at least one existed node
		int counter = 0;
		while (counter < 2) {
			for (int j = 0; j < i; j++) {
				double randNumber = (rand() % 100) / 100.0;
				if (randNumber <= sn.attechedProbablity[j]) {
					if (sn.addEdge(j, i)) {
						counter++;
					}
					if (counter == 2)
						break;
				}
			}
			//cout<<counter<<endl;
		}

	}
}

void writefile(string filename, string content) {
	ofstream resultfile(filename, ios::app);           //打开文件用于写,若文件不存在就创建它
	if (!resultfile)
		cout << "open file:" << filename << "failure!";            //打开文件失败则结束运行
	resultfile << content << endl;     //使用插入运算符写文件内容
	resultfile.close();         //关闭文件

}

void removeAndWritefile(string filename, string content) {
	if (remove(filename.c_str()) != 0) {
		cout << "Error deleting file " << filename << endl;
		perror("Error deleting file");
	}
	ofstream resultfile(filename, ios::app);
	if (!resultfile)
		cout << "open file: " << filename << " failure!";
	resultfile << content << endl;
	resultfile.close();
}

//return inflected numbers
int sowing(int startNode, double p) {
	srand(time(NULL));        // init the random
	double immunited[nodeNum];
	int tempNode = 0, tempStartNode = 0;
	memset(immunited, 0, sizeof(immunited));
	vector<int> inflectedList;
	vector<int> tempInflectedList;
	inflectedList.push_back(startNode);
	immunited[startNode] = 1;
	while (inflectedList.size() > 0) {
		for (vector<int>::iterator itr = inflectedList.begin();
				itr != inflectedList.end();) {
			tempStartNode = *itr;
			for (int itr2 = 0;
					itr2 < sn.user_user_relation[tempStartNode].size();
					itr2++) {
				tempNode = sn.user_user_relation[tempStartNode][itr2];
				if(immunited[tempNode] == 1){
					continue;
				}
				double randNumber = (rand() % 100) / 100.0;
				if (randNumber < p) {
					tempInflectedList.push_back(tempNode);
					immunited[tempNode] = 1;
				}
			}
			itr = inflectedList.erase(itr); // here erase will add the itr, then in the loop, no need to increase itr
		}
		inflectedList.reserve(inflectedList.size() + tempInflectedList.size());
		inflectedList.insert(inflectedList.end(), tempInflectedList.begin(),
				tempInflectedList.end());
		tempInflectedList.clear();
	}

	double counter = 0;
	for (int i = 0; i < nodeNum; i++) {
		if (immunited[i] == 1) {
			counter++;
			//cout<<"immuted counter: "<<counter<<endl;
		}
	}
	//cout<<"here "<<startNode<<" inflected node: "<<counter<<endl;
	return counter;
}

//http://arxiv.org/abs/1406.2293
//Gossip: Identifying Central Individuals in a Social Network

int sowingByGrasipModel(int startNode, double p) {
	srand(time(NULL));        // init the random
	double immunited[nodeNum];
	int tempNode = 0, tempStartNode = 0;
	memset(immunited, 0, sizeof(immunited));
	vector<int> inflectedList;
	vector<int> tempInflectedList;
	inflectedList.push_back(startNode);
	//immunited[startNode] = 1;
	while (inflectedList.size() > 0) {
		for (vector<int>::iterator itr = inflectedList.begin();
				itr != inflectedList.end();) {
			tempStartNode = *itr;
			//immunited[tempStartNode] = 1;
			for (int itr2 = 0;
					itr2 < sn.user_user_relation[tempStartNode].size();
					itr2++) {
				tempNode = sn.user_user_relation[tempStartNode][itr2];
				double randNumber = (rand() % 100) / 100.0;
				if(immunited[tempNode] == 1){
					continue;
				}
				if (randNumber < p) {
					//this is the key!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//					node_node_importance[tempNode][tempStartNode] =
//							node_node_importance[tempNode][tempStartNode] + 1;

					node_node_importance[startNode][tempNode] =
												node_node_importance[startNode][tempNode] + 1;
					//this is the key!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

					if (immunited[tempNode] == 0) {
						//this is the key!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						//?node_node_importance[tempNode][tempStartNode] = node_node_importance[tempNode][tempStartNode]+1;
						//this is the key!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!s
						tempInflectedList.push_back(tempNode);
						immunited[tempNode] = 1;
					}
				}
			}
			itr = inflectedList.erase(itr);	// here erase will add the itr, then in the loop, no need to increase itr
		}
		inflectedList.reserve(inflectedList.size() + tempInflectedList.size());
		inflectedList.insert(inflectedList.end(), tempInflectedList.begin(),
				tempInflectedList.end());
		tempInflectedList.clear();
	}

	double counter = 0;
	for (int i = 0; i < nodeNum; i++) {
		if (immunited[i] == 1) {
			counter++;
			//cout<<"immuted counter: "<<counter<<endl;
		}
	}
	//cout<<"here "<<startNode<<" inflected node: "<<counter<<endl;
	return counter;
}
void printArray(double arry[], int len) {
	for (int i = 0; i < len; i++)
		cout << arry[i] << ",";
	cout << endl;
}

int MergeArray(double arry[], int start, int mid, int end, double temp[])//数组的归并操作
		{
	//int leftLen=mid-start+1;//arry[start...mid]左半段长度
	//int rightLlen=end-mid;//arry[mid+1...end]右半段长度
	int i = mid;
	int j = end;
	int k = 0;	//临时数组末尾坐标
	int count = 0;
	//设定两个指针ij分别指向两段有序数组的头元素，将小的那一个放入到临时数组中去。
	while (i >= start && j > mid) {
		if (arry[i] > arry[j]) {
			temp[k++] = arry[i--];	//从临时数组的最后一个位置开始排序
			count += j - mid;//因为arry[mid+1...j...end]是有序的，如果arry[i]>arry[j]，那么也大于arry[j]之前的元素，从a[mid+1...j]一共有j-(mid+1)+1=j-mid
		} else if (arry[i] == arry[j]) {
			temp[k++] = arry[j--];
		} else {
			temp[k++] = arry[j--];
		}
	}
	//cout<<"Counter in calling MergeArray: "<<count<<endl;
	while (i >= start)	//表示前半段数组中还有元素未放入临时数组
	{
		temp[k++] = arry[i--];
	}

	while (j > mid) {
		temp[k++] = arry[j--];
	}

	//将临时数组中的元素写回到原数组当中去。
	for (i = 0; i < k; i++)
		arry[end - i] = temp[i];

	//printArray(arry,end);//输出进过一次归并以后的数组，用于理解整体过程
	return count;
}

int InversePairsCore(double arry[], int start, int end, double temp[]) {
	int inversions = 0;
	if (start < end) {
		int mid = (start + end) / 2;
		inversions += InversePairsCore(arry, start, mid, temp);	//找左半段的逆序对数目
		inversions += InversePairsCore(arry, mid + 1, end, temp);	//找右半段的逆序对数目
		inversions += MergeArray(arry, start, mid, end, temp);//在找完左右半段逆序对以后两段数组有序，然后找两段之间的逆序对。最小的逆序段只有一个元素。
	}
	return inversions;
}

int InversePairs(double arry[], int len) {
	double *temp = new double[len];
	int counter = InversePairsCore(arry, 0, len - 1, temp);
	delete[] temp;
	return counter;
}

double slowKendallTau(double arry[], int len) {
	double tempArray[nodeNum];
	memcpy(tempArray, arry, sizeof(tempArray));
	//cout<<"len is: "<<len<<endl;
	int counter = InversePairs(tempArray, len);
	return 0;
}
//http://en.wikipedia.org/wiki/Kendall_tau_rank_correlation_coefficient
double getNoOrderPairs(double aray[], int len) {
	double paris = 0.0;
	//int degreesDistribution[len];
	map<double, int> degreesDistribution;
	int drgrees = 0;
	//memset(degreesDistribution,0,sizeof(degreesDistribution));
	for (int i = 0; i < len; i++) {
		//degrees[i]=sn.user_user_relation[i].size();
		drgrees = degreesDistribution[aray[i]];
		degreesDistribution[aray[i]] = drgrees + 1;
	}

	map<double, int>::iterator it;
	for (it = degreesDistribution.begin(); it != degreesDistribution.end();
			++it) {
		double deg = it->first;
		int degNum = it->second;
		if(deg>nodeNum||degNum > nodeNum){
			cout<< deg <<"	error "<<degNum<<endl;
			return -1111;
		}
		if (degNum > 1) {
			paris += 0.5 * (degNum * (degNum - 1));
			//cout<< deg <<"	right "<<degNum<<endl;
		}
	}
	return paris;
}

double KendallTau(double arry[], int len) {
	//int arry[]={7,5,6,4};
	//int arry[]={1,3,7,8,2,4,6,5};
	//int len=sizeof(arry)/sizeof(int);
	//printArray(arry,1000);
	//cout<<"8443	"<< arry[8443]<<endl;
	//order_paris = getNoOrderPairs(arry,len);
	double sum = 0;
	double tempArray[len];
	memcpy(tempArray, arry, sizeof(tempArray));
	no_discordant_or_concordant_paris = getNoOrderPairs(tempArray, len);
	for (int i = 0; i < len; i++) {
		//nodeSequenceWithSingleMethod[i] = nodeDrgeeNum[i];
		if (tempArray[i] != arry[i]) {
			cout <<i<<"	"<< tempArray[i] << "	123 " << arry[i] << endl;
		}
	}
	//cout<<"len is: "<<len<<endl;
	double counter = InversePairs(tempArray, len);
	//printArray(tempArray,len);
	sum = (len - 1) * 1.0 * len / 2;

	//cout<<"sum1	"<<sum<<endl;
	//    int no_discordant_or_concordant_counter = 0;
	//    for(int i = 0; i<len; i++)
	//    {
	//        if(arry[i] == i)
	//        {
	//            no_discordant_or_concordant_counter++;
	//        }
	//    }
	//   double no_discordant_or_concordant_paris = (nodeNum-1)*no_discordant_or_concordant_counter;
	//cout<<"no_discordant_or_concordant_paris "<<no_discordant_or_concordant_paris<<endl;

//	printArray(arry,len);
	double concordantPrais = sum - counter - no_discordant_or_concordant_paris;	//sum-no_discordant_or_concordant_paris-counter;
	//cout<<"sum "<<sum<<" max "<<(numeric_limits<double>::max)()<<endl;
	/*cout<<"order "<<concordantPrais<<endl;
	 cout<<"disorder "<<counter<<endl;
	 cout<<"same "<<no_discordant_or_concordant_paris<<endl;*/
	cout << neibergProbs << "   sum	" << sum << "	order " << concordantPrais
			<< "   disorder " << counter << " same "
			<< no_discordant_or_concordant_paris << endl;
//	if((concordantPrais-counter)<0)
//	{
//		//cout<<"concordantPrais "<<concordantPrais<<" sum: "<<sum<<endl;
//	}
	//cout<<"prob is: "<<probs<<" neibergProbs is: "<<neibergProbs<<" counter is: "<<counter<<" KendallTau is: "<<(concordantPrais-counter)/sum<<endl;
	return (concordantPrais - counter) * 1.0 / sum;
}
double simpleKendallTau(double a[], int len) {
	int order = 0, disorder = 0, same = 0, counter = 0;
	double sum = 0;

	for (int i = 0; i < len; i++) {
		for (int j = i + 1; j < len; j++) {
			if ((a[i] - a[j]) * (i - j) > 0) {
				order++;
			} else if ((a[i] - a[j]) * (i - j) < 0) {
				disorder++;
			} else {
				same++;
			}
			counter++;
		}
	}
	sum = len * (len - 1) * 0.5;
	cout << "Simple	" << sum << " counter " << counter << "	order " << order
			<< "   disorder " << disorder << " same " << same << endl;

	return (order - disorder) * 1.0 / sum;
}

double testKendallTau() {
	/*int arry[]={1,2,3,9,10,14,4,15,8,7,5,20,11,12,25,24,28,18,21,34,26,19,29,39,60,13,35,44,47,53,63,83,43,32,103,79,57,132,93,27,51,52,33,54,56,23,42,22,121,30,136,68,110,6,109,74,87,147,151,48,49,50,16,90,86,120,67,17,69,31,182,80,166
	 ,37,76,77,171,162,46,55,62,64,71,75,84,107,89,91,92,94,96,212,108,111,128,131,175,140,190,38,149,40,194,41,223,163,100,139,72,36,165,211,58,133,115,193,117,85,206,66,152,201,78,105,114,159,161,244,252,122,207,169,170,95,
	 172,45,137,97,203,144,187,150,218,65,188,70,168,208,119,192,99,98,153,127,88,184,146,126,242,160,173,202,219,186,256,260,261,263,276,291,129,130,134,135,138,145,148,156,164,167,174,176,177,178,179,181,183,185,189,143,191
	 ,195,199,204,210,214,217,220,221,222,225,228,233,235,236,239,73,250,251,81,82,106,113,116,269,271,124,125,352,141,158,200,104,282,155,226,262,154,229,230,196,283,142,265,157,266,237,372,258,268,284,59,102,216,245,272,273
	 ,274,292,118,197,180,277,278,279,112,318,297,259,434,304,305,306,449,313,385,315,320,451,324,441,397,398,101,354,357,364,370,255,487,198,213,264,215,270,275,464,285,410,375,288,289,290,376,458,295,378,298,280,300,61,413,
	 465,406,227,231,307,308,232,310,311,497,234,205,238,438,317,379,319,240,321,241,388,243,389,326,327,328,419,330,391,332,333,334,491,336,421,338,123,340,423,424,246,344,345,455,347,425,349,350,351,248,353,249,355,426,209,
	 453,359,360,402,403,363,253,428,366,468,507,511,515,518,528,530,538,546,576,590,613,299,301,302,303,309,312,314,316,322,323,325,329,331,335,337,339,341,342,343,346,348,356,358,361,362,365,367,368,369,371,373,374,377,380,
	 381,382,383,384,386,387,390,392,393,394,395,396,399,400,401,404,405,407,408,409,411,412,414,415,416,417,418,420,422,427,429,430,431,432,433,435,436,437,439,440,442,443,444,445,446,447,448,450,452,454,456,457,459,460,461,
	 462,463,466,467,469,470,471,472,473,474,475,476,477,478,479,480,481,482,483,484,485,486,488,489,490,492,493,494,495,496,498,499,500,501,502,503,504,505,506,224,508,509,510,247,512,513,514,254,516,517,257,519,520,521,522,
	 523,524,525,526,527,267,529,281,531,532,533,534,535,536,537,286,539,540,541,542,543,544,545,287,547,548,549,550,551,552,553,554,555,556,557,558,559,560,561,562,563,564,565,566,567,568,569,570,571,572,573,574,575,293,577,
	 578,579,580,581,582,583,584,585,586,587,588,589,294,591,592,593,594,595,596,597,598,599,600,601,602,603,604,605,606,607,608,609,610,611,612,296,614,615,616,617,618,619,620,621,622,623,624,625,626,627,628,629,630,631,632,
	 633,634,635,636,637,638,639,640,641,642,643,644,645,646,647,648,649,650,651,652,653,654,655,656,657,658,659,660,661,662,663,664,665,666,667,668,669,670,671,672,673,674,675,676,677,678,679,680,681,682,683,684,685,686,687,
	 688,689,690,691,692,693,694,695,696,697,698,699,700,701,702,703,704,705,706,707,708,709,710,711,712,713,714,715,716,717,718,719,720,721,722,723,724,725,726,727,728,729,730,731,732,733,734,735,736,737,738,739,740,741,742,
	 743,744,745,746,747,748,749,750,751,752,753,754,755,756,757,758,759,760,761,762,763,764,765,766,767,768,769,770,771,772,773,774,775,776,777,778,779,780,781,782,783,784,785,786,787,788,789,790,791,792,793,794,795,796,797,
	 798,799,800,801,802,803,804,805,806,807,808,809,810,811,812,813,814,815,816,817,818,819,820,821,822,823,824,825,826,827,828,829,830,831,832,833,834,835,836,837,838,839,840,841,842,843,844,845,846,847,848,849,850,851,852,
	 853,854,855,856,857,858,859,860,861,862,863,864,865,866,867,868,869,870,871,872,873,874,875,876,877,878,879,880,881,882,883,884,885,886,887,888,889,890,891,892,893,894,895,896,897,898,899,900,901,902,903,904,905,906,907,
	 908,909,910,911,912,913,914,915,916,917,918,919,920,921,922,923,924,925,926,927,928,929,930,931,932,933,934,935,936,937,938,939,940,941,942,943,944,945,946,947,948,949,950,951,952,953,954,955,956,957,958,959,960,961,962,
	 963,964,965,966,967,968,969,970,971,972,973,974,975,976,977,978,979,980,981,982,983,984,985,986,987,988,989,990,991,992,993,994,995,996,997,998,999,0};
	 */
	//int arry[]={10,9,8,7,6,5,4,3,2,1};
	//int arry[] = {1,3,4,2,11,15,12,9,8,5,6,7,10,14,13};
	//int arry[] = {1,3,4,2,12.7,12.7,12.7,9,9,9,9,9,9,9,9};
	//int arry[] = {10,9,8,7,4,4,4,3,2,2,2,2,2,2,2};//-0.8783
	double arry[] = { 1, 2, 3, 3, 3 };
	//int arry[]={1,3,7,8,2,4,6,5};
	int len = sizeof(arry) / sizeof(double);
	double tempArray[5];
	memcpy(tempArray, arry, sizeof(tempArray));
	//cout<<"len is: "<<len<<endl;
	//printArray(tempArray,100);
	double sum = len * (len - 1) / 2;
	int counter = InversePairs(tempArray, len);
	//printArray(tempArray,len);
	cout << "counter is: " << counter << " and kt is: "
			<< (sum - counter * 2 - 3) * 1.0 / sum << endl;

	return counter;
}

double testKendallTau123() {
	double arry[] = { 3, 3, 3, 1, 2, 2, 4, 4, 4, 4 };
	cout << KendallTau(arry, 10) << endl;
	cout << simpleKendallTau(arry, 10) << endl;
	return 0.0;
}

//int GetKendallTauDistance(double itemsA[], double itemsB[],int length)
//{
//	// 存储 0~N-1 各个数字在数组itemsB中的索引
//	double * indexesB = (double *)malloc(length * sizeof(double));
//	for (int index = 0; index < length; index++)
//	{
//		indexesB[itemsB[index]] = index;
//	}
//
//	// 存储数组itemsA中指定索引处的数字在数组itemsB中的索引
//	double * indexesA = (double *)malloc(length * sizeof(double));
//	for (int i = 0; i < nodeNum; i++)
//	{
//		indexesA[i] = indexesB[itemsA[i]];
//	}
//
//	// 统计数组indexesA中的逆序对
//	int KendallTauDistance = KendallTau(indexesA,length);
//
//	free(indexesB);
//	free(indexesA);
//
//	return KendallTauDistance;
//}

void setToInitialToSowAgain() {
	//	PNGraph Graph = TNGraph::New();
	//    Graph->AddNode(1);
	//    Graph->AddNode(5);
	//    Graph->AddEdge(1,5);
}
void getSequencebyDegree() {
	double nodeDrgeeNum[nodeNum];
	memset(nodeDrgeeNum, 0, sizeof(nodeDrgeeNum));
	memset(nodeSequenceWithSingleMethod, 0,
			sizeof(nodeSequenceWithSingleMethod));

	//memset(degreeBasedSequence,0,sizeof(degreeBasedSequence));
	for (int i = 0; i < nodeNum; i++) {
		nodeDrgeeNum[i] = sn.user_user_relation[i].size();
		//degrrArray[i] = nodeDrgeeNum[i];
		nodeSequenceWithSingleMethod[i] = nodeDrgeeNum[i];
	}
	//printArray(degreeBasedSequence,nodeNum);
	//cout<<"GetKendallTauDistance is: "<<GetKendallTauDistance(degrrArray,degreeBasedSequence,nodeNum)<<endl;
}

void getSequencebySowing() {
	double nodeInflectEfeetionScore[nodeNum];
	memset(nodeInflectEfeetionScore, 0, sizeof(nodeInflectEfeetionScore));
	int inflectedNodes = 0;
	for (int j = 0; j < sowingTimes; j++) {
		for (int i = 0; i < nodeNum; i++) {
			double infe = sowing(i, probs);
			nodeInflectEfeetionScore[i] += infe;
			inflectedNodes += infe;
			//cout<<nodeInflectEfeetionScore[i]<<endl;
		}
	}

	for (int i = 0; i < nodeNum; i++) {
		nodeSequenceWithSingleMethod[i] = nodeInflectEfeetionScore[i]
				/ sowingTimes;
		//cout<<i<<"	"<<sn.user_user_relation[i].size()<<"	"<<nodeInflectEfeetionScore[i]<<endl;
	}

	cout<<"probs:   "<<probs<<" maxdegree:  "<<sn.getUserMaxDegree()<<" inflected:  "<<inflectedNodes<<" and average coverd nodes: "<<inflectedNodes*1.0/nodeNum/sowingTimes<<endl;

}

void getSequencebyDegreeWithNeibergs() {
	double nodeDrgeeNum[nodeNum];
	memset(nodeDrgeeNum, 0, sizeof(nodeDrgeeNum));
	memset(nodeSequenceWithSingleMethod, 0,
			sizeof(nodeSequenceWithSingleMethod));
	for (int i = 0; i < nodeNum; i++) {
		int neibergDegree = 0;
		nodeDrgeeNum[i] = sn.user_user_relation[i].size();
		for (int j = 0; j < sn.user_user_relation[i].size(); j++) {
			neibergDegree +=
					sn.user_user_relation[sn.user_user_relation[i][j]].size();
		}
		nodeDrgeeNum[i] += neibergProbs * neibergDegree;
		//degrrArray[i] = nodeDrgeeNum[i];
	}

	//for (int i = 0;i<nodeNum;i++)
	//{
	//	cout<<sn.user_user_relation[i].size()<<"	"<<nodeDrgeeNum[i]<<endl;
	//}

	memcpy(nodeSequenceWithSingleMethod, nodeDrgeeNum,
			sizeof(nodeSequenceWithSingleMethod));

	//for (int i = 0;i<nodeNum;i++)
	//{
	//	//nodeSequenceWithSingleMethod[i] = nodeDrgeeNum[i];
	//	if (nodeSequenceWithSingleMethod[i] != nodeDrgeeNum[i])
	//	{
	//		cout<<nodeSequenceWithSingleMethod[i]<<"	"<<nodeDrgeeNum[i]<<endl;
	//	}
	//}
}

void getSequencebyGasip() {
	double heardGasipTimesFromNode[nodeNum];
	memset(heardGasipTimesFromNode, 0, sizeof(heardGasipTimesFromNode));

	for(int times = 0; times<sowingTimes;times++){
		for (int i = 0; i < nodeNum; i++) {
			sowingByGrasipModel(i, probs);
			//cout<<heardGasipTimesFromNode[i]<<endl;
		}
	}
	//cout<<"probs:   "<<probs<<" inflected:  "<<inflectedNodes<<" and average coverd nodes: "<<inflectedNodes*1.0/nodeNum<<endl;
	for (int i = 0; i < nodeNum; i++) {
		node_node_importance[i][i] = 0;
		for (int j = 0; j < nodeNum; j++) {
			heardGasipTimesFromNode[i] = heardGasipTimesFromNode[i]
					+ node_node_importance[i][j]/(sowingTimes*1.0);
		}
		cout<<"node "<<i<<"	cover	"<<heardGasipTimesFromNode[i]<<endl;
	}

	memcpy(nodeSequenceWithSingleMethod, heardGasipTimesFromNode,
			sizeof(nodeSequenceWithSingleMethod));
	//for (int i = 0;i<nodeNum;i++)
	//{
	//	//nodeSequenceWithSingleMethod[i] = nodeDrgeeNum[i];
	//	if (nodeSequenceWithSingleMethod[i] != heardGasipTimesFromNode[i])
	//	{
	//		cout<<nodeSequenceWithSingleMethod[i]<<"	"<<heardGasipTimesFromNode[i]<<endl;
	//	}
	//}

//	vector<Ranker> itemPredictValue;
//	Ranker p;
//	for(long int iii=0; iii<nodeNum; iii++)
//	{
//		if(heardGasipTimesFromNode[iii] != 0)
//		{
//			p.id = iii;
//			p.value = heardGasipTimesFromNode[iii];
//			itemPredictValue.push_back(p);
//		}
//	}
//	std::sort(itemPredictValue.begin(), itemPredictValue.end(), Smaller);
//	//cout<<"after: "<<itemPredictValue.size()<<endl;
//
//	for(int i = 0; i < itemPredictValue.size();i++)
//	{
//		nodeSequenceWithSingleMethod[i] = itemPredictValue[i].id;
//		//cout<<nodeSequenceWithSingleMethod[i]<<endl;
//	}
}

void getSequencebyGasipDegreeMoreThanD(int d) {
	double heardGasipTimesFromNode[nodeNum];
	memset(heardGasipTimesFromNode, 0, sizeof(heardGasipTimesFromNode));
	int inflectedNodes = 0;
	for (int i = 0; i < nodeNum; i++) {
		sowingByGrasipModel(i, probs);
		//cout<<heardGasipTimesFromNode[i]<<endl;
	}
	//cout<<"probs:   "<<probs<<" inflected:  "<<inflectedNodes<<" and average coverd nodes: "<<inflectedNodes*1.0/nodeNum<<endl;
	for (int i = 0; i < nodeNum; i++) {
		node_node_importance[i][i] = 0;
		for (int j = 0; j < nodeNum; j++) {
			heardGasipTimesFromNode[i] = heardGasipTimesFromNode[i]
					+ node_node_importance[j][i];
		}
		//cout<<heardGasipTimesFromNode[i]<<endl;
	}

	memcpy(nodeSequenceWithSingleMethod, heardGasipTimesFromNode,
			sizeof(nodeSequenceWithSingleMethod));

//	vector<Ranker> itemPredictValue;
//	Ranker p;
//	for(long int iii=0; iii<nodeNum; iii++)
//	{
//		if(heardGasipTimesFromNode[iii] != 0)
//		{
//			p.id = iii;
//			p.value = heardGasipTimesFromNode[iii];
//			itemPredictValue.push_back(p);
//		}
//	}
//	std::sort(itemPredictValue.begin(), itemPredictValue.end(), Smaller);
//	//cout<<"after: "<<itemPredictValue.size()<<endl;
//
//	for(int i = 0; i < itemPredictValue.size();i++)
//	{
//		nodeSequenceWithSingleMethod[i] = itemPredictValue[i].id;
//		//cout<<nodeSequenceWithSingleMethod[i]<<endl;
//	}
}

// 如果数组itemsB是有序数组，那么问题就转化为统计数组itemsA中的逆序对
// 而在一般情况下，问题可转化为统计数组itemsA中按照数组itemsB中数字的排列顺序确定的逆序对

//只统计度大于1的点的kintal
void getRidofDegreeLessThenOne() {

}

void getDegreeDistribution() {
	int degrees[nodeNum];
	int degreesDistribution[nodeNum];
	memset(degrees, 0, sizeof(degrees));
	memset(degreesDistribution, 0, sizeof(degreesDistribution));
	for (int i = 0; i < nodeNum; i++) {
		degrees[i] = sn.user_user_relation[i].size();
		degreesDistribution[sn.user_user_relation[i].size()] =
				degreesDistribution[sn.user_user_relation[i].size()] + 1;
	}

	for (int i = 0; i < nodeNum; i++) {
		if (degreesDistribution[i] > 0) {
			cout << i << "  " << degreesDistribution[i] << endl;
		}
	}
	//cout<<"degreesDistribution[0] "<<degrees[0]<<endl;
}

void kShell() {
	double nodeDergeeNum[nodeNum];
	memset(nodeDergeeNum, 0, sizeof(nodeDergeeNum));
	memset(KShellDergeeNum, 0, sizeof(KShellDergeeNum));
	memset(KShellDergeeNum, 0, sizeof(KShellDergeeNum));
	memset(nodeSequenceWithSingleMethod, 0,
			sizeof(nodeSequenceWithSingleMethod));

	int K_Degree = 1;
	int startMinDrgree = 2;	//BA网络最小度为2，所以最小的kshell = 2
	//memset(degreeBasedSequence,0,sizeof(degreeBasedSequence));
	while (sn.getUserMaxDegree() > 0) {
		K_Degree++;
		int KCounter = 0;
		startMinDrgree = sn.getUserMinDegree();
		//cout <<"K_Degree is: "<<K_Degree<<" UserMaxDegree is: "<<sn.getUserMaxDegree()<<endl;
		for (int i = 0; i < nodeNum; i++) {
			nodeDergeeNum[i] = sn.user_user_relation[i].size();
			//cout <<"i is: "<<i<<" nodeDergeeNum[i]: "<<nodeDergeeNum[i]<<endl;
			if (nodeDergeeNum[i] == startMinDrgree) {
				//cout <<"i is: "<<i<<" nodeDergeeNum[i]: "<<nodeDergeeNum[i]<<endl;
				KShellDergeeNum[i] = K_Degree;
				KCounter++;
			}
		}

		for (int i = 0; i < nodeNum; i++) {
			if (KShellDergeeNum[i] != 0) {
				// cout <<"KShellDergee is: "<<KShellDergeeNum[i]<<" K_Degree is: "<<K_Degree<<endl;
				if (KShellDergeeNum[i] == K_Degree) {
					//cout <<"before is: "<<sn.user_user_relation[i].size()<<endl;
					sn.removePoint(i);
					//cout <<"after  is: "<<sn.user_user_relation[i].size()<<endl;
				}
			}
		}
		//cout<<KCounter<<"   nodes shell is: "<<K_Degree<<endl;
	}
	memcpy(nodeSequenceWithSingleMethod, KShellDergeeNum,
			sizeof(nodeSequenceWithSingleMethod));
	for (int i = 0; i < nodeNum; i++) {
		//nodeSequenceWithSingleMethod[i] = nodeDrgeeNum[i];
		if (nodeSequenceWithSingleMethod[i] != KShellDergeeNum[i]) {
			cout << nodeSequenceWithSingleMethod[i] << "	" << KShellDergeeNum[i]
					<< endl;
		}
	}
}

void hybird(double lamada) {
	double dergeeArray[nodeNum];
	double hyBirdDergeeNum[nodeNum];
	memset(hyBirdDergeeNum, 0, sizeof(hyBirdDergeeNum));
	//memset(KShellDergeeNum,0,sizeof(KShellDergeeNum));
	//memset(nodeSequenceWithSingleMethod,0,sizeof(nodeSequenceWithSingleMethod));
	//memset(degreeBasedSequence,0,sizeof(degreeBasedSequence));
	for (int i = 0; i < nodeNum; i++) {
		dergeeArray[i] = sn.user_user_relation[i].size();
	}

	stringstream sssss;
	for (int i = 0; i < nodeNum; i++) {
		hyBirdDergeeNum[i] = pow(nodeSequenceWithSingleMethod[i] * 1.0, lamada)
				* pow(dergeeArray[i] * 1.0, (1 - lamada));
		cout << lamada << "	"
				<< pow(nodeSequenceWithSingleMethod[i] * 1.0, lamada) << "\t"
				<< pow(dergeeArray[i] * 1.0, (1 - lamada)) << endl;
		sssss << nodeSequenceWithSingleMethod[i] << "\t" << dergeeArray[i]
				<< "\t" << hyBirdDergeeNum[i];
		writefile("degree.txt", sssss.str());
		sssss.str("");
	}
	//memcpy(nodeSequenceWithSingleMethod,KShellDergeeNum,sizeof(nodeSequenceWithSingleMethod));
	memcpy(nodeSequence, hyBirdDergeeNum, sizeof(nodeSequence));

}

int main() {
	//testKendallTau123();
	// setToInitialToSowAgain();
	genBA();
	//getSequencebyDegree();
	//getSequencebyGasip();
	//getSequencebySowing();
	//hybird(2);
	//cout<<neibergProbs<<"\t"<<KendallTau(nodeSequenceWithSingleMethod,nodeNum)<<endl;
	//cout<<neibergProbs<<"\t"<<simpleKendallTau(nodeSequenceWithSingleMethod,nodeNum)<<endl;
	//kShell();
	//cout<<neibergProbs<<"\t"<<KendallTau(nodeSequenceWithSingleMethod,nodeNum)<<endl;
	//hybird(0.0);
	//std::reverse(nodeSequenceWithSingleMethod,nodeSequenceWithSingleMethod+nodeNum);
	//cout << "lamada" << "\t"<< KendallTau(nodeSequenceWithSingleMethod, nodeNum) << endl;
	//getDegreeDistribution();
	//cout<<sn.countDegree()<<endl;
//	getSequencebyDegree();//0.136893
//
//    stringstream ss;
//	for (double lamada = 0.1;lamada<1;lamada+=0.1)
//	{
//		genBA();
//		neibergProbs = lamada;
//		kShell();
//		//getSequencebyGasip();
//		hybird(lamada);
//        //getSequencebyDegreeWithNeibergs();
//		ss<<lamada<<"\t"<<KendallTau(nodeSequence,nodeNum)<<"\r\n";
//		//ss<<lamada<<"\t"<<simpleKendallTau(nodeSequence,nodeNum)<<"\r\n";
//	}
//    cout<<ss.str()<<endl;
//    removeAndWritefile("answer.txt",ss.str());
//    ss.str("");

//	for(double p = 0; p<0.1; p+=0.01)
//	{
//		genBA();
//		neibergProbs = p;
//		getSequencebyDegreeWithNeibergs();
//		//getSequencebyDegree();
//		cout<<neibergProbs<<"\t"<<KendallTau(nodeSequenceWithSingleMethod,nodeNum)<<endl;//152925//154061_0.383139
//	}

	for(double p = 0; p<1000; p+=1)
	{
		//probs = p;
		//neibergProbs = p;
		getSequencebySowing();//147200_0.42
		//getSequencebyGasip();//99765_0.60032
		//getSequencebyDegreeWithNeibergs();
		cout<<KendallTau(nodeSequenceWithSingleMethod,nodeNum)<<"  "<<p<<endl;//107701_0.5527
	}

	//getSequencebySowing();
	//KendallTau(nodeSequenceWithSingleMethod,sizeof(nodeSequenceWithSingleMethod)/sizeof(int));//329749
	//for(int i = 0; i< 10;i++)
	//{
	//	cout<<sn.attechedProbablity[i]<<endl;
	//}
	system("pause");
	return 0;
}
