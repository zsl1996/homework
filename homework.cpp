
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
string worddtostr(unordered_map<string, bitset<20>> &wordd, string s) {//根据位信息将单词改为大写待更改！！！！！！！！！！！！！！！！！！！！！！
	bitset<20> q = wordd[s];
	int f = s.length();
	for (int j = 0; j < f; j++)
	{
		s[j] = s[j] - 32 * q.test(j);
	}
	return s;
}
#ifdef _WIN32
#include <io.h>
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
#endif
#ifdef __linux__
#include <dirent.h>
#include <sys/stat.h>
void GetAllFiles(string path, vector<string>& files)
{
	string name;
	DIR* dir = opendir(path.c_str());
	dirent* p = NULL;
	while ((p = readdir(dir)) != NULL)
	{
		if (p->d_name[0] != '.')
		{
			string name = path + "/" + string(p->d_name);
			files.push_back(name);
			//cout << name << endl;
			if (p->d_type == 4) {
				GetAllFiles(name, files);
			}
		}

	}
	closedir(dir);

}
#endif



bitset<20> bitcmp(bitset<20> a, bitset<20> b) { //按字典顺序返回正确的词
	for (int j = 0; j < 20; j++) {
		if (a.test(j) > b.test(j)) {
			return a;
		}
		if (a.test(j)< b.test(j)) {
			return b;
		}
	}
	return a;
}

int ReadByChar(string file, int &fch, int &line, unordered_map<string, int> &word, unordered_map<string, int> &phrase, unordered_map<string, bitset<20>> &wordd, unordered_map<string, string>& wordend)
{
	bitset<20> q = 0;
	fstream openbychar;
	char c;
	int flag = 0;//标志是否是第一个词
	openbychar.open(file, ios::in);
	string sword = "";
	string sphrase = "";
	string num = "";
	unordered_map<string, string>::iterator it;
	if (openbychar.fail()) {
		return 0; //可能是目录
	}
	line = 1;//文件末尾没有换行
	while (openbychar.get(c))
	{
		if (c >= 32 && c < 127) fch++; //统计字符数
		if (c == '\n') {
			line++;
		}
		//统计单词
		if ((c < 91 && c>64) || (c < 123 && c>96)) { //字母
			flag++;
			if (c < 91 && c>64) // 范围大概已知用位运算简化11111111111111111111111111111111
			{
				c = c + 32;
				sword = sword + c;
				if (flag<20) q.set(flag - 1);
			}
			//			else sword = sword + c; //  优化！！！！！！！
			else sword.push_back(c); //  优化！！！！！！！
		}
		else if (c > 47 && c < 58) { //数字   !!!!!!!!!!!file123 与file
			if (flag > 3) {
				num = num + c;
				sword = sword + c;
				flag++;
			}
			else {
				flag = 0;
				sword = "";
				q = 0;
				num = "";
			}
		}
		else {   //特殊字符
			if (flag > 3) {
				if (sword[sword.length() - 1] < 123 && sword[sword.length() - 1]>96) //最后是字母
				{
					word[sword]++;
				 bitset<20> a = bitcmp(q, wordd[sword]);
					if (q == a) wordend[sword] = "";//有纯单词，且按字典顺序靠前，只需要存后缀为空
					wordd[sword] = a;
					sphrase = sphrase + " " + sword; //第一个词组有点bug，可以输出的时候去掉！！！！！！！！！
					phrase[sphrase]++;
					sphrase = sword;
				}
				else { //后缀为数字

					sword = sword.substr(0, strlen(sword.c_str()) - strlen(num.c_str())); // 优化！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
					word[sword]++;
				 bitset<20> c = wordd[sword];
					if (q == c) {
						if (wordend[sword] != "") {
							wordend[sword] = wordend[sword].compare(num) > 0 ? num : wordend[sword];
						}
					}
					else if(q == bitcmp(q,c))
					{
						wordend[sword] = num;
						wordd[sword] = q;
					}	
					sphrase = sphrase + " " + sword; //第一个词组有点bug，可以输出的时候去掉！！！！！！！！！！！
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
	if (sword.length() > 3) { //文末最后一个单词//bug!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (flag > 3) {
			if (sword[sword.length() - 1] < 123 && sword[sword.length() - 1]>96) //最后是字母
			{
				word[sword]++;
				 bitset<20> d = bitcmp(q, wordd[sword]);
				if (q == d ) wordend[sword] = "";//有纯单词，且按字典顺序靠前，只需要存后缀为空
				wordd[sword] = d;
				sphrase = sphrase + " " + sword; //第一个词组有点bug，可以输出的时候去掉！！！！！！！！！
				phrase[sphrase]++;
				sphrase = sword;
			}
			else { //后缀为数字

				sword = sword.substr(0, strlen(sword.c_str()) - strlen(num.c_str())); // 优化！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
				word[sword]++;
				 bitset<20> e = wordd[sword];
				if (q == e) {
					if (wordend[sword] != "") {
						wordend[sword] = wordend[sword].compare(num) > 0 ? num : wordend[sword];
					}
				}
				else if (q == bitcmp(q, e))
				{
					wordend[sword] = num;
					wordd[sword] = q;
				}
				sphrase = sphrase + " " + sword; //第一个词组有点bug，可以输出的时候去掉！！！！！！！！！！！
				phrase[sphrase]++;
				sphrase = sword;
			}
			sword = "";
			num = "";
			q = 0;
			flag = 0;
		}
		sword = "";
		num = "";
		q = 0;
		flag = 0;
	}
	q = 0;
	openbychar.close();
	if (fch == 0) line = 0;
	return 0;
}

void SplitString(string& s, vector<std::string>& v)

{
	const std::string& c = " ";
	//优化！！！！！！！！！！！！！！！！！！！！！！
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

int  getwmin() { //获取最小值下标志
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
	//优化！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
	unordered_map<string, int>::iterator it = word.begin();
	unordered_map<string, int>::iterator end = word.end();
	int min = getwmin();
	for (; it != end; it++) { //改为指针应该比迭代器快1111111111111111111111111111111111111111111111
		if (it->second > wnumtop[min]) {

			//pworld[min] = "";
			topworld[min] = it->first;
			wnumtop[min] = it->second;
			min = getwmin();
		}
	}

}

void writewordtxt(string filepath, unordered_map<string, bitset<20> >& wordd, unordered_map<string, string> wordend) { //输出到txt
	fstream writebychar;
	int max = 0;
	writebychar.open(filepath, ios::app);
	for (int i = 0; i < 10; i++)
	{
		max = getmax();
		string s = worddtostr(wordd, topworld[max]);
		writebychar << (s + wordend[topworld[max]]); //优化！！！！！！！！！！！！！！！！！！！！
		writebychar << " ";
		writebychar << wnumtop[max] << "\n";
		wnumtop[max] = 0;
	}
	writebychar << "\n\n";
	writebychar.close();
}

void writeworddtxt(string filepath, unordered_map<string, bitset<20>> wordd, unordered_map<string, string> &wordend) { // 等待修改！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
	fstream writebychar;
	int max = 0;
	writebychar.open(filepath, ios::app);
	for (int i = 0; i < 10; i++)
	{
		max = getmax();
		vector<string> s;
		SplitString(topworld[max], s);
		writebychar << (worddtostr(wordd, s[0]) + wordend[s[0]]) << " " << (worddtostr(wordd, s[1]) + wordend[s[1]]);
		writebychar << " ";
		writebychar << wnumtop[max] << "\n";
		wnumtop[max] = 0;

	}
	writebychar << "\n\n";
	writebychar.close();
}

void writecharnum(string path, unordered_map<string, int> word) {
	fstream write;
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

int main(int argc, char *argv[])
{	//"C:/Users/zsl/Desktop/test/newsample"
	unordered_map<string, int> word; //单词 数目
	unordered_map<string, bitset<20>> wordd;//单词 对应应该大写的位
	unordered_map<string, int> phrase; //词组 数目
	unordered_map<string, string> wordend; //词 可能有的单词后缀
	vector<string> files;
	string writepath = "";
	string filepath = "";
	//string filepath = "C:/Users/zsl/Desktop/test/newsample";
	string s = argv[1];
	writepath = s + "/result.txt";
	
	if (s.find(".") == string::npos) {
		filepath = s;
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
		ReadByChar(s, allch, allline, word, phrase, wordd, wordend);
	}

	writecharnum(writepath, word);
	if (allch != 0) {
		top(word);
		writewordtxt(writepath, wordd, wordend);
		top(phrase);
		writeworddtxt(writepath, wordd, wordend);
	}
	return 0;
}
