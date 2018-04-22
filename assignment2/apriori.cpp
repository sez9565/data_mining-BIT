/*Apriori�㷨 2012.10.31*/
#include <iostream>  
#include <fstream>
#include <vector>  
#include <map>  
#include <string>  
#include <algorithm>  
#include <cmath>  
using namespace std;

vector<string> T;                   //�����ʼ���������   
double minSup, minConf;              //�û��趨����С֧�ֶȺ����Ŷ�   
map<string, int> mp;                //������Ŀ����ÿ��Ԫ���������г��ֵĴ���   
vector< vector<string> > F;       //���Ƶ����Ŀ��   
vector<string> R;                 //��Ź�������   


void initTransactionSet()       //��ȡ����   
{
	int n;
	ifstream ifs("name_cat_out.txt");
	//cout << "���������񼯵ĸ���:" << endl;
	ifs >> n;
	ifs.get();
	//cout << "����������:" << endl;
	while (n--)
	{
		char szLine[200];
		ifs.getline(szLine, 200);          //�����������ÿ��Ԫ���Կո����,����ֻ����������   
		T.push_back(string(szLine));
	}
	//cout << "��������С֧�ֶȺ����Ŷ�:" << endl;      //֧�ֶȺ����Ŷ�ΪС����ʾ��ʽ   
	//cin >> minSup >> minConf;
	minSup = 3e-2;
	minConf = 5e-1;
}


vector<string> split(string str, char ch)
{
	vector<string> v;
	v.reserve(30);//����v�ı��ÿռ�Ϊ30�������ڴ�������
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

void genarateOneFrequenceSet()          //����1-Ƶ����Ŀ��   
{
	int i, j;
	vector<string> f;                 //�洢1-Ƶ����Ŀ��   
	for (i = 0; i<T.size(); i++)
	{
		string t = T[i];
		vector<string> v = split(t, ' ');       //����������񼯽����з֣�������1 2 3���зֵõ�"1","2","3"   
		for (j = 0; j<v.size(); j++)             //ͳ��ÿ��Ԫ�س��ֵĴ�����ע��mapĬ�ϰ���key����������   
		{
			if (v[j] != "nan")
			{
				mp[v[j]]++;
			}
		}
	}

	int AbsMinSup = minSup * T.size();
	for (map<string, int>::iterator it = mp.begin(); it != mp.end(); it++)    //�޳���������С֧�ֶ�Ҫ����   
	{
		if ((*it).second >= AbsMinSup)
		{
			f.push_back((*it).first);
		}
	}
	F.push_back(T);                 //������F[1]��ʾ1-Ƶ����Ŀ��   
	if (f.size() != 0)
	{
		F.push_back(f);
	}
}

bool judgeItem(vector<string> v1, vector<string> v2) //�ж�v1��v2�Ƿ�ֻ�����һ�ͬ   
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

bool judgeSubset(vector<string> v, vector<string> f)        //�ж�v������k-1�Ӽ��Ƿ���f��   
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

int calculateSupportCount(vector<string> v)      //����֧�ֶȼ���   
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


bool judgeSupport(vector<string> v)                  //�ж�һ�����֧�ֶ��Ƿ�����Ҫ��   
{
	int count = calculateSupportCount(v);
	if (count >= ceil(minSup*T.size()))
		return true;
	return false;
}


void generateKFrequenceSet()        //����k-Ƶ����Ŀ��   
{
	int k;
	for (k = 2; k <= mp.size(); k++)
	{
		if (F.size()< k)           //���Fk-1Ϊ�գ����˳�   
			break;
		else                      //����Fk-1����Ck��ѡ�      
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
					if (judgeItem(v1, v2))       //���v1��v2ֻ�����һ�ͬ�����������   
					{
						vector<string> tempVector = v1;
						tempVector.push_back(v2[v2.size() - 1]);
						sort(tempVector.begin(), tempVector.end());     //��Ԫ�����򣬷����ж��Ƿ��������   

																		//��֦�Ĺ���   
																		//�ж� v1��(k-1)���Ӽ��Ƿ���Fk-1���Լ��Ƿ��������֧�ֶ�   
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



vector<string> removeItemFromSet(vector<string> v1, vector<string> v2)    //��v1���޳�v2   
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

string getStr(vector<string> v1, vector<string> v2)        //����ǰ���ͺ���õ�����  
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
	vector< vector<string> > H;                   //������еĺ��   
	int fCount = calculateSupportCount(v);         //f��֧�ֶȼ���   

	for (i = 0; i<v.size(); i++)                    //������1-�����������   
	{
		vector<string> temp = v;
		temp.erase(temp.begin() + i);

		int aCount = calculateSupportCount(temp);

		if (fCount >= ceil(aCount*minConf))        //����������Ŷ�Ҫ��   
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

	for (k = 2; k<v.size(); k++)              //����k-�����������   
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
					tempVector.push_back(v2[v2.size() - 1]);                        //�õ��������   
					sort(tempVector.begin(), tempVector.end());

					vector<string> filterV = removeItemFromSet(v, tempVector);      //�õ�ǰ������   
					int aCount = calculateSupportCount(filterV);                   //����ǰ��֧�ֶȼ���   
					if (fCount >= ceil(aCount*minConf))                           //����������Ŷ�Ҫ��   
					{
						string rStr = getStr(filterV, tempVector);                  //����ǰ���ͺ���õ�����   

						string hStr;
						for (int s = 0; s<tempVector.size(); s++)
							hStr += tempVector[s] + " ";
						hStr = hStr.substr(0, hStr.size() - 1);
						addH.push_back(hStr);                                    //�õ�һ���µĺ������   

						R.push_back(rStr);
					}
				}
			}
		}
		if (addH.size() != 0)                                                       //�����е�k-������ϼ��뵽H��   
			H.push_back(addH);
	}
}


void generateRules()               //���ɹ�������   
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


void outputFrequenceSet()          //���Ƶ����Ŀ��   
{
	int i, k;
	if (F.size() == 1)
	{
		cout << "��Ƶ����Ŀ��!" << endl;
		return;
	}
	for (k = 1; k<F.size(); k++)
	{
		cout << k << "-Ƶ����Ŀ��:" << endl;
		vector<string> f = F[k];
		for (i = 0; i<f.size(); i++)
			cout << f[i] << endl;
	}
}

void outputRules()              //�����������   
{
	int i;
	cout << "��������:" << endl;
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