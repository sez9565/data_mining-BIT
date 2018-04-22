/*Apriori算法 2012.10.31*/
#include <iostream>  
#include <fstream>
#include <vector>  
#include <map>  
#include <string>  
#include <algorithm>  
#include <cmath>  
using namespace std;

vector<string> T;                   //保存初始输入的事务集   
double minSup, minConf;              //用户设定的最小支持度和置信度   
map<string, int> mp;                //保存项目集中每个元素在事务集中出现的次数   
vector< vector<string> > F;       //存放频繁项目集   
vector<string> R;                 //存放关联规则   


void initTransactionSet()       //获取事务集   
{
	int n;
	ifstream ifs("name_cat_out.txt");
	//cout << "请输入事务集的个数:" << endl;
	ifs >> n;
	ifs.get();
	//cout << "请输入事务集:" << endl;
	while (n--)
	{
		char szLine[200];
		ifs.getline(szLine, 200);          //输入的事务集中每个元素以空格隔开,并且只能输入数字   
		T.push_back(string(szLine));
	}
	//cout << "请输入最小支持度和置信度:" << endl;      //支持度和置信度为小数表示形式   
	//cin >> minSup >> minConf;
	minSup = 3e-2;
	minConf = 5e-1;
}


vector<string> split(string str, char ch)
{
	vector<string> v;
	v.reserve(30);//设置v的备用空间为30，减少内存分配次数
	int i, j;
	i = 0;
	while (i<str.size())
	{
		if (str[i] == ch)
			i++;
		else
		{
			j = i;
			while (j<str.size())
			{
				if (str[j] != ch)
					j++;
				else
					break;
			}
			string temp = str.substr(i, j - i);
			v.push_back(temp);
			i = j + 1;
		}
	}
	return v;
}

void genarateOneFrequenceSet()          //生成1-频繁项目集   
{
	int i, j;
	vector<string> f;                 //存储1-频繁项目集   
	for (i = 0; i<T.size(); i++)
	{
		string t = T[i];
		vector<string> v = split(t, ' ');       //将输入的事务集进行切分，如输入1 2 3，切分得到"1","2","3"   
		for (j = 0; j<v.size(); j++)             //统计每个元素出现的次数，注意map默认按照key的升序排序   
		{
			if (v[j] != "nan")
			{
				mp[v[j]]++;
			}
		}
	}

	int AbsMinSup = minSup * T.size();
	for (map<string, int>::iterator it = mp.begin(); it != mp.end(); it++)    //剔除不满足最小支持度要求的项集   
	{
		if ((*it).second >= AbsMinSup)
		{
			f.push_back((*it).first);
		}
	}
	F.push_back(T);                 //方便用F[1]表示1-频繁项目集   
	if (f.size() != 0)
	{
		F.push_back(f);
	}
}

bool judgeItem(vector<string> v1, vector<string> v2) //判断v1和v2是否只有最后一项不同   
{
	int i, j;
	i = 0;
	j = 0;
	while (i<v1.size() - 1 && j<v2.size() - 1)
	{
		if (v1[i] != v2[j])
			return false;
		i++;
		j++;
	}
	return true;
}

bool judgeSubset(vector<string> v, vector<string> f)        //判断v的所有k-1子集是否在f中   
{
	int i, j;
	bool flag = true;
	for (i = 0; i<v.size(); i++)
	{
		string str;
		for (j = 0; j<v.size(); j++)
		{
			if (j != i)
				str += v[j] + " ";
		}
		str = str.substr(0, str.size() - 1);
		vector<string>::iterator it = find(f.begin(), f.end(), str);
		if (it == f.end())
			flag = false;
	}
	return flag;
}

int calculateSupportCount(vector<string> v)      //计算支持度计数   
{
	int i, j;
	int count = 0;
	for (i = 0; i<T.size(); i++)
	{
		vector<string> t = split(T[i], ' ');
		for (j = 0; j<v.size(); j++)
		{
			vector<string>::iterator it = find(t.begin(), t.end(), v[j]);
			if (it == t.end())
				break;
		}
		if (j == v.size())
			count++;
	}
	return count;
}


bool judgeSupport(vector<string> v)                  //判断一个项集的支持度是否满足要求   
{
	int count = calculateSupportCount(v);
	if (count >= ceil(minSup*T.size()))
		return true;
	return false;
}


void generateKFrequenceSet()        //生成k-频繁项目集   
{
	int k;
	for (k = 2; k <= mp.size(); k++)
	{
		if (F.size()< k)           //如果Fk-1为空，则退出   
			break;
		else                      //根据Fk-1生成Ck候选项集      
		{
			int i, j;
			vector<string> c;
			vector<string> f = F[k - 1];
			for (i = 0; i<f.size() - 1; i++)
			{
				vector<string> v1 = split(f[i], ' ');
				for (j = i + 1; j<f.size(); j++)
				{
					vector<string> v2 = split(f[j], ' ');
					if (judgeItem(v1, v2))       //如果v1和v2只有最后一项不同，则进行连接   
					{
						vector<string> tempVector = v1;
						tempVector.push_back(v2[v2.size() - 1]);
						sort(tempVector.begin(), tempVector.end());     //对元素排序，方便判断是否进行连接   

																		//剪枝的过程   
																		//判断 v1的(k-1)的子集是否都在Fk-1中以及是否满足最低支持度   
						if (judgeSubset(tempVector, f) && judgeSupport(tempVector))
						{
							int p;
							string tempStr;
							for (p = 0; p<tempVector.size() - 1; p++)
								tempStr += tempVector[p] + " ";
							tempStr += tempVector[p];

							c.push_back(tempStr);
						}
					}
				}
			}
			if (c.size() != 0)
				F.push_back(c);
		}
	}
}



vector<string> removeItemFromSet(vector<string> v1, vector<string> v2)    //从v1中剔除v2   
{
	int i;
	vector<string> result = v1;
	for (i = 0; i<v2.size(); i++)
	{
		vector<string>::iterator it = find(result.begin(), result.end(), v2[i]);
		if (it != result.end())
			result.erase(it);
	}
	return result;
}

string getStr(vector<string> v1, vector<string> v2)        //根据前件和后件得到规则  
{
	int i;
	string rStr;
	for (i = 0; i<v1.size(); i++)
		rStr += v1[i] + " ";
	rStr = rStr.substr(0, rStr.size() - 1);
	rStr += "->";

	for (i = 0; i<v2.size(); i++)
		rStr += v2[i] + " ";
	rStr = rStr.substr(0, rStr.size() - 1);
	return rStr;
}

void ap_generateRules(string fs)
{
	int i, j, k;
	vector<string> v = split(fs, ' ');
	vector<string> h;
	vector< vector<string> > H;                   //存放所有的后件   
	int fCount = calculateSupportCount(v);         //f的支持度计数   

	for (i = 0; i<v.size(); i++)                    //先生成1-后件关联规则   
	{
		vector<string> temp = v;
		temp.erase(temp.begin() + i);

		int aCount = calculateSupportCount(temp);

		if (fCount >= ceil(aCount*minConf))        //如果满足置信度要求   
		{
			h.push_back(v[i]);
			string tempStr;
			for (j = 0; j<v.size(); j++)
			{
				if (j != i)
					tempStr += v[j] + " ";
			}
			tempStr = tempStr.substr(0, tempStr.size() - 1);
			tempStr += "->" + v[i];
			R.push_back((tempStr));
		}
	}

	H.push_back(v);
	if (h.size() != 0)
		H.push_back(h);

	for (k = 2; k<v.size(); k++)              //生成k-后件关联规则   
	{
		h = H[k - 1];
		vector<string> addH;
		for (i = 0; i<h.size() - 1; i++)
		{
			vector<string> v1 = split(h[i], ' ');
			for (j = i + 1; j<h.size(); j++)
			{
				vector<string> v2 = split(h[j], ' ');

				if (judgeItem(v1, v2))
				{
					vector<string> tempVector = v1;
					tempVector.push_back(v2[v2.size() - 1]);                        //得到后件集合   
					sort(tempVector.begin(), tempVector.end());

					vector<string> filterV = removeItemFromSet(v, tempVector);      //得到前件集合   
					int aCount = calculateSupportCount(filterV);                   //计算前件支持度计数   
					if (fCount >= ceil(aCount*minConf))                           //如果满足置信度要求   
					{
						string rStr = getStr(filterV, tempVector);                  //根据前件和后件得到规则   

						string hStr;
						for (int s = 0; s<tempVector.size(); s++)
							hStr += tempVector[s] + " ";
						hStr = hStr.substr(0, hStr.size() - 1);
						addH.push_back(hStr);                                    //得到一个新的后件集合   

						R.push_back(rStr);
					}
				}
			}
		}
		if (addH.size() != 0)                                                       //将所有的k-后件集合加入到H中   
			H.push_back(addH);
	}
}


void generateRules()               //生成关联规则   
{
	int i, j, k;
	for (k = 2; k<F.size(); k++)
	{
		vector<string> f = F[k];
		for (i = 0; i<f.size(); i++)
		{
			string str = f[i];
			ap_generateRules(str);
		}
	}
}


void outputFrequenceSet()          //输出频繁项目集   
{
	int i, k;
	if (F.size() == 1)
	{
		cout << "无频繁项目集!" << endl;
		return;
	}
	for (k = 1; k<F.size(); k++)
	{
		cout << k << "-频繁项目集:" << endl;
		vector<string> f = F[k];
		for (i = 0; i<f.size(); i++)
			cout << f[i] << endl;
	}
}

void outputRules()              //输出关联规则   
{
	int i;
	cout << "关联规则:" << endl;
	for (i = 0; i<R.size(); i++)
	{
		cout << R[i] << endl;
	}
}

void Apriori()
{
	initTransactionSet();
	genarateOneFrequenceSet();
	generateKFrequenceSet();
	outputFrequenceSet();
	generateRules();
	outputRules();
}


int main(int argc, char *argv[])
{
	Apriori();
	return 0;
}