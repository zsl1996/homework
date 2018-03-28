
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
int ReadByChar(string file,int &fch,int &line, unordered_map<string, int> &word)
{

	fstream openbychar;
	char c;
	int flag = 0;
	openbychar.open(file, ios::in);
	string s;
	if (openbychar.fail()) {
		return 0; //可能是目录
	}
	line = 1;//文件末尾没有换行
	while (openbychar.get(c))
	{
		if(c>=32&&c<127)  //有待商榷
		fch++;
		if (c == '\n') {
			line++;
		}
	//统计单词
		if ((c < 91 && c>64 )|| (c<123&&c>96) ) {
			flag++;
			if (c < 91 && c>64) // 范围大概已知用位运算简化11111111111111111111111111111111
			{
				c = c + 32;
				s = s + c;
			}
			s = s + c;
		}
		//不是字母
		else if (flag >3)
		{
			word[s]++;
			flag = 0;
			s = "";
		}
		else
		{
			flag = 0;
			s = "";
		}
	//
		
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
int main()
{
	unordered_map<string, int> word;
	string filepath = "C:/Users/zsl/Desktop/test/newsample";
	vector<string> files;
	GetAllFiles(filepath, files);
	int size = files.size();
	int allch = 0; //the number of char
	int allline =0 ; //the num of line
	for (int i = 0; i < size; i++)
	{
		int fch = 0; 
		int line = 0;
		ReadByChar(files[i],fch,line,word);
		allch += fch;
		allline += line;
	}
	top(word);


	return 0;
}