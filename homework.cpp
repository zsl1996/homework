
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
using namespace std;
string topworld[10];
int wnumtop[10] = { 0 };
int allch = 0; //the number of char
int allline = 0; //the num of line
bitset<128> q = 0;//��־word��һλ�Ǵ�д
void GetAllFiles(string path, vector<string>& files)
{

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
int ReadByChar(string file, int &fch, int &line, unordered_map<string, int> &word, unordered_map<string, int> &phrase, unordered_map<string, bitset<128>> &wordd)
{

	fstream openbychar;
	char c;
	int flag = 0;//��־�Ƿ��ǵ�һ����
	openbychar.open(file, ios::in);
	string sword = "";
	string sphrase = "";
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
		if ((c < 91 && c>64) || (c < 123 && c>96)) {
			flag++;
			if (c < 91 && c>64) // ��Χ�����֪��λ�����11111111111111111111111111111111
			{
				c = c + 32;
				sword = sword + c;
				q.set(flag - 1);
			}
			else sword = sword + c;
		}
		//������ĸ
		else if (flag > 3)
		{
			word[sword]++;
			wordd[sword] = bitcmp(q,wordd[sword]); 
			q = 0;
			//ͳ�ƴ���
			sphrase = sphrase + " " + sword;
			phrase[sphrase]++;
			sphrase = sword;
			flag = 0;
			sword = "";
		}
		else
		{
			flag = 0;
			sword = "";
		}
	}
	if (sword.length() > 3) { //��ĩ���һ������
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
	unordered_map<string, int>::iterator it = word.begin();
	unordered_map<string, int>::iterator end = word.end();
	int min = getwmin();
	for (; it != end; it++) { //��Ϊָ��Ӧ�ñȵ�������1111111111111111111111111111111111111111111111
		if (it->second > wnumtop[min]) {
			topworld[min] = "";
			topworld[min] = it->first;
			wnumtop[min] = it->second;
			min = getwmin();
		}
	}

}
void writewordtxt(string filepath, unordered_map<string, bitset<128>>& wordd) { //�����txt
	fstream writebychar;
	char c;
	int max = 0;
	writebychar.open(filepath, ios::app);
	for (int i = 0; i < 10; i++)
	{
		max = getmax();
		bitset<128> q = wordd[topworld[max]];
		int f = topworld[max].length();
		for (int j = 0; j < f; j++)
		{
			topworld[max][j] = topworld[max][j] - 32 * q.test(j);
		}
		writebychar << topworld[max];
		writebychar << " ";
		writebychar << wnumtop[max] << "\n";
		wnumtop[max] = 0;
	}
	writebychar << "\n\n";
	writebychar.close();
}
void writeworddtxt(string filepath) { //�����txt
	fstream writebychar;
	char c;
	int max = 0;
	writebychar.open(filepath, ios::app);
	for (int i = 0; i < 10; i++)
	{
		max = getmax();

		writebychar << topworld[max];
		writebychar << " ";
		writebychar << wnumtop[max] << "\n";
		wnumtop[max] = 0;

	}
	writebychar << "\n\n";
	writebychar.close();
}
void writecharnum(string path, int wordnum) {
	fstream write;
	char c;
	int max = 0;
	write.open(path, ios::out);
	write << "char_number : ";
	write << allch << "\n";
	write << "line_number : ";
	write << allline << "\n";
	write << " word_number: ";
	write << wordnum << "\n \n";;
	write.close();
}
int main()
{
	string filepath = "C:/Users/zsl/Desktop/1";
	string writepath = "C:/Users/zsl/Desktop/result.txt";
	unordered_map<string, int> word; //
	unordered_map<string, bitset<128>> wordd;
	unordered_map<string, int> phrase;
	vector<string> files;
	GetAllFiles(filepath, files);
	int size = files.size();
	for (int i = 0; i < size; i++)
	{
		int fch = 0;
		int line = 0;
		ReadByChar(files[i], fch, line, word, phrase, wordd);
		allch += fch;
		allline += line;
	}
	writecharnum(writepath, word.size());
	if (allch != 0) {
		top(word);
		writewordtxt(writepath, wordd);
		top(phrase);
		writeworddtxt(writepath);
	}
 	return 0;
}