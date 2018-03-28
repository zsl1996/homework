// homework.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <io.h>  
#include <fstream>  
#include <string>  
#include <vector>  
#include <iostream>  
#include<unordered_map>
#include <valarray>  
#include<iterator>
using namespace std;
string topworld[10];
int wnumtop[10] = { 0 };
int allch = 0; //the number of char
int allline = 0; //the num of line
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
int ReadByChar(string file, int &fch, int &line, unordered_map<string, int> &word, unordered_map<string, int> &phrase)
{

	fstream openbychar;
	char c;
	int flag = 0;//标志是否是第一个词
	openbychar.open(file, ios::in);
	string sword;
	string sphrase = "";
	if (openbychar.fail()) {
		return 0; //可能是目录
	}
	line = 1;//文件末尾没有换行
	while (openbychar.get(c))
	{
		if (c >= 32 && c < 127)  //有待商榷
			fch++;
		if (c == '\n') {
			line++;
		}
		//统计单词
		if ((c < 91 && c>64) || (c < 123 && c>96)) {
			flag++;
			if (c < 91 && c>64) // 范围大概已知用位运算简化11111111111111111111111111111111
			{
				c = c + 32;
				sword = sword + c;
			}
			sword = sword + c;
		}
		//不是字母
		else if (flag > 3)
		{
			word[sword]++;
//统计词组
			sphrase = sphrase +" "+ sword;
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
	openbychar.close();
	return 0;
}
int  getwmin() { //获取最小值下标志
	int minnum = 2147483647;
	int min= 0;
	for (int i = 0; i < 10; i++) {
		if (wnumtop[i] < minnum) {
			minnum = wnumtop[i];
			min = i;
		}
	}
		return min;
}
void top(unordered_map<string, int> & word) {
	unordered_map<string, int>::iterator it = word.begin();
	unordered_map<string, int>::iterator end = word.end();
	int min = getwmin();
	for (; it != end; it++) { //改为指针应该比迭代器快1111111111111111111111111111111111111111111111
		if (it->second > wnumtop[min]) {
			topworld[min] = "";
			topworld[min] = it->first;
			wnumtop[min] = it->second;
			min = getwmin();
		}
	}

}
void writetxt(string filepath) { //输出到txt
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
void writecharnum(string path,int wordnum) {
	fstream write;
	char c;
	int max = 0;
	write.open(path, ios::out);
		write << "char_number : ";
		write << allch<<"\n";
		write << "line_number : ";
		write << allline << "\n";
		write << " word_number: ";
		write << wordnum <<"\n \n";;
	write.close();
}
int main()
{
	string filepath = "C:/Users/zsl/Desktop/test/newsample";
	string writepath = "C:/Users/zsl/Desktop/result.txt";
	unordered_map<string, int> word;
	unordered_map<string, int> phrase;
	vector<string> files;
	GetAllFiles(filepath, files);
	int size = files.size();
	for (int i = 0; i < size; i++)
	{
		int fch = 0; 
		int line = 0;
		ReadByChar(files[i],fch,line,word,phrase);
		allch += fch;
		allline += line;
	}
	writecharnum(writepath,word.size());
	top(word);
	writetxt(writepath);
	top(phrase);
	writetxt(writepath);
	return 0;
}