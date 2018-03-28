#include "stdafx.h"
#include <io.h>  
#include <fstream>  
#include <string>  
#include <vector>  
#include <iostream>  
#include<unordered_map>
#include <valarray>  
#include<iterator>
#include<bitset>
#include<string.h>
using namespace std;
string topworld[10];
int wnumtop[10] = { 0 };
int allch = 0; //the number of char
int allline = 0; //the num of line
string worddtostr(unordered_map<string, bitset<128>> &wordd,string s){//����λ��Ϣ�����ʸ�Ϊ��д�����ģ�������������������������������������������
	bitset<128> q = wordd[s];
	int f = s.length();
	for (int j = 0; j < f; j++)
	{
		s[j] = s[j] - 32 * q.test(j);
	}
	return s;
}

void GetAllFiles(string path, vector<string>& files)
{
#ifdef _WIN32
	long   hFile = 0;
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					GetAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}

		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
#else
#endif
}

bitset<128> bitcmp(bitset<128> a, bitset<128> b) { //���ֵ�˳�򷵻���ȷ�Ĵ�
	for (int j = 0; j < 128; j++) {
		if (a.test(j) > b.test(j)) {
			return a;
		}
		if (a.test(j)< b.test(j)) {
			return b;
		}
	}
	return a;
}

int ReadByChar(string file, int &fch, int &line, unordered_map<string, int> &word, unordered_map<string, int> &phrase, unordered_map<string, bitset<128>> &wordd, unordered_map<string, string>& wordend)
{
	bitset<128> q = 0;
	fstream openbychar;
	char c;
	int flag = 0;//��־�Ƿ��ǵ�һ����
	openbychar.open(file, ios::in);
	string sword = "";
	string sphrase = "";
	string num = "";
	unordered_map<string, string>::iterator it;
	if (openbychar.fail()) {
		return 0; //������Ŀ¼
	}
	line = 1;//�ļ�ĩβû�л���
	while (openbychar.get(c))
	{
		if (c >= 32 && c < 127) fch++; //ͳ���ַ���
		if (c == '\n') {
			line++;
		}
		//ͳ�Ƶ���
		if ((c < 91 && c>64) || (c < 123 && c>96)) { //��ĸ
			flag++;
			if (c < 91 && c>64) // ��Χ�����֪��λ�����11111111111111111111111111111111
			{
				c = c + 32;
				sword = sword +num + c;
				if(flag<128) q.set(flag - 1);
			}
//			else sword = sword + c; //  �Ż���������������
			else sword.push_back(c); //  �Ż���������������
		}
		else if (c > 47 && c < 58) { //����   !!!!!!!!!!!file123 ��file
			if (flag > 3) {
				num = num + c;
				flag++;
			}
			else {
				flag = 0;
				sword = "";
				q = 0;
				num = "";
			}
		}
		else {   //�����ַ�
			if (flag > 3) {
				if (sword[sword.length() - 1] < 123 && sword[sword.length() - 1]>96) //�������ĸ
				{
					word[sword]++;
					wordd[sword] = bitcmp(q, wordd[sword]);
					wordend[sword] = "";//�д����ʣ����ֵ�˳��ֻ��Ҫ���׺Ϊ��
					sphrase = sphrase + " " + sword; //��һ�������е�bug�����������ʱ��ȥ�����������������
					phrase[sphrase]++;
					sphrase = sword;
				}
				else { //��׺Ϊ����
				
					sword=sword.substr(0, strlen(sword.c_str()) - strlen(num.c_str())); // �Ż���������������������������������������������������������������
					word[sword]++;
					it = wordend.find(sword);
					if (it == wordend.end()) { //˵��������û��������ֺ�׺�ĵ���
						wordend[sword] = num;
					}
					else
					{
						wordend[sword] = wordend[sword].compare(num) > 0 ? num: wordend[sword];
					}
					wordd[sword] = bitcmp(q, wordd[sword]);
					sphrase = sphrase + " " + sword; //��һ�������е�bug�����������ʱ��ȥ���������������������
					phrase[sphrase]++;
					sphrase = sword;
				}
				sword = "";
				num = "";
				q = 0;
				flag = 0;
			}
			else {
				flag = 0;
				sword = ""; 
				num = "";
				q = 0;
			}
		}
	}
	if (sword.length() > 3) { //��ĩ���һ������//bug!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		word[sword]++;
		wordd[sword] = bitcmp(q,wordd[sword]); 
		q = 0;
		sphrase = sphrase + " " + sword;
		phrase[sphrase]++;
	}
	q = 0;
	openbychar.close();
	if (fch == 0) line = 0;
	return 0;
}

void SplitString(string& s,vector<std::string>& v) 

{
	const std::string& c = " ";
	//�Ż���������������������������������������������
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

int  getwmin() { //��ȡ��Сֵ�±�־
	int minnum = 2147483647;
	int min = 0;
	for (int i = 0; i < 10; i++) {
		if (wnumtop[i] < minnum) {
			minnum = wnumtop[i];
			min = i;
		}
	}
	return min;
}

int getmax() {
	int maxnum = 0;
	int max = 0;
	for (int i = 0; i < 10; i++) {
		if (wnumtop[i] > maxnum) {
			maxnum = wnumtop[i];
			max = i;
		}
	}
	return max;
}

void top(unordered_map<string, int> & word) {
	//�Ż�����������������������������������������������������������������������������
	unordered_map<string, int>::iterator it = word.begin();
	unordered_map<string, int>::iterator end = word.end();
	int min = getwmin();
	for (; it != end; it++) { //��Ϊָ��Ӧ�ñȵ�������1111111111111111111111111111111111111111111111
		if (it->second > wnumtop[min]) {
			
			//pworld[min] = "";
			topworld[min] = it->first;
			wnumtop[min] = it->second;
			min = getwmin();
		}
	}

}

void writewordtxt(string filepath, unordered_map<string, bitset<128>>& wordd,unordered_map<string, string> wordend) { //�����txt
	fstream writebychar;
	int max = 0;
	writebychar.open(filepath, ios::app);
	for (int i = 0; i < 10; i++)
	{
		max = getmax();
		string s = worddtostr(wordd, topworld[max]);
		writebychar << (s + wordend[topworld[max]]); //�Ż�����������������������������������������
		writebychar << " ";
		writebychar << wnumtop[max] << "\n";
		wnumtop[max] = 0;
	}
	writebychar << "\n\n";
	writebychar.close();
}

void writeworddtxt(string filepath, unordered_map<string, bitset<128>> wordd, unordered_map<string, string> &wordend) { // �ȴ��޸ģ���������������������������������������������������������������������
	fstream writebychar;
	char c;
	int max = 0;
	writebychar.open(filepath, ios::app);
	for (int i = 0; i < 10; i++)
	{
		max = getmax();
		vector<string> s;
		SplitString(topworld[max], s);
		writebychar << (worddtostr(wordd,s[0])+ wordend[s[0]] )<<" "<< (worddtostr(wordd, s[1])+ wordend[s[1]]);
		writebychar << " ";
		writebychar << wnumtop[max] << "\n";
		wnumtop[max] = 0;

	}
	writebychar << "\n\n";
	writebychar.close();
}

void writecharnum(string path, unordered_map<string, int> word) {
	fstream write;
	char c;
	int max = 0;
	unordered_map<string, int>::iterator it = word.begin();
	unordered_map<string, int>::iterator end = word.end();
	int wordnum = 0;
	for (; it != end; it++) {
		wordnum = wordnum + it->second;
	}
	write.open(path, ios::out);
	write << "char_number : ";
	write << allch << "\n";
	write << "line_number : ";
	write << allline << "\n";
	write << "word_number:";
	write << wordnum << "\n \n";;
	write.close();
}

int main(int argc,char *argv[])
{	//"C:/Users/zsl/Desktop/test/newsample"
	unordered_map<string, int> word; //���� ��Ŀ
	unordered_map<string, bitset<128>> wordd;//���� ��ӦӦ�ô�д��λ
	unordered_map<string, int> phrase; //���� ��Ŀ
	unordered_map<string, string> wordend; //�� �����еĵ��ʺ�׺
	vector<string> files;
	string writepath="";
	string filepath="";
	//string filepath = "C:/Users/zsl/Desktop/test/newsample";
	writepath = "C:/Users/zsl/Desktop/result.txt";
	string s = argv[1];
	if (s.find(".") == string::npos) {
		filepath =s;
		GetAllFiles(filepath, files);

		int size = files.size();
		for (int i = 0; i < size; i++)
		{
			int fch = 0;
			int line = 0;
			ReadByChar(files[i], fch, line, word, phrase, wordd, wordend);
			allch += fch;
			allline += line;
		}
	}
	else {
		ReadByChar(s , allch, allline, word, phrase, wordd, wordend);
	}

	writecharnum(writepath,word);
	if (allch != 0) {
		top(word);
		writewordtxt(writepath, wordd,wordend);
		top(phrase);
		writeworddtxt(writepath, wordd, wordend);
	}
 	return 0;
}