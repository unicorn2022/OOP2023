/*
pdlist:
	按照日期的顺序，列出diary.txt中的日记实体
	输入：如果通过命令行参数，输入了开始日期和结束日期，则只输出在这两个日期之间的日记
	输出：stdout
*/
#pragma warning(disable:4996)
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <Windows.h>
#include <ctime>
#include <iostream>
#include <fstream>
using namespace std;
//设置输出的颜色
void SetColor(std::string color) {
	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
	if (color == "Red")SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_INTENSITY);
	else if (color == "Blue")SetConsoleTextAttribute(hdl, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	else if (color == "Green")SetConsoleTextAttribute(hdl, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	else if (color == "Yellow")SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}
class Date {
	int year, month, day;
public:
	Date() {
		year = month = day = 0;
	}
	Date(int Y, int M, int D) {
		year = Y;
		month = M;
		day = D;
	}
	bool input() {
		SetColor("Blue");
		printf("请输入日记的日期：年,月,日\n");
		SetColor("Red");
		cin >> year >> month >> day;
		if (year == -1)return false;
		printf("%4d %02d %02d\n",year,month,day);
		return true;
	}
	bool input(ifstream& fin) {
		fin >> year >> month >> day;
		if (year == -1)return false;
		return true;
	}

	void output() {
		printf("%04d/%02d/%02d\n", year, month, day);
	}
	void output(ofstream& fout) {
		fout << year << " " << month << " " << day << endl;
	}

	//日期为-1/-1/-1的是最大的(放在文件结尾)
	bool operator==(const Date& A)const {
		return year == A.year && month == A.month && day == A.day;
	}
	bool operator<(const Date& A)const {
		if (year == -1)return false;
		if (A.year == -1)return true;

		if (year != A.year)return year < A.year;
		if (month != A.month)return month < A.month;
		return day < A.day;
	}
	bool operator>(const Date& A)const {
		if (year == -1)return true;
		if (A.year == -1)return false;

		if (year != A.year)return year > A.year;
		if (month != A.month)return month > A.month;
		return day > A.day;
	}
};
class Diary {
	Date date;
	vector<string>content;
public:
	bool input() {
		if (date.input() == false)return false;
		SetColor("Blue");
		printf("请逐行输入日记的内容,以.或EOF结束\n");
		string s;
		getline(cin, s);
		while (1) {
			SetColor("Yellow");
			printf("%2d:", (int)content.size() + 1);
			SetColor("Red");
			getline(cin, s);
			cout<< s << endl;
			if (s == "")break;
			if (s == ".")break;
			content.push_back(s);
		}
		content.push_back(".");
		SetColor("Blue");
		printf("日记输入成功\n");
		return true;
	}
	bool input(ifstream& fin) {
		if (date.input(fin) == false)return false;
		string s;
		getline(fin, s);
		while (1) {
			getline(fin, s);
			if (s == "")break;
			if (s == ".")break;
			content.push_back(s);
		}
		content.push_back(".");
		return true;
	}

	void output() {
		date.output();
		SetColor("Blue");
		for (int i = 0; i < content.size(); i++)
			cout << content[i] << endl;
	}
	void output(ofstream& fout) {
		date.output(fout);
		for (int i = 0; i < content.size(); i++)
			fout << content[i] << endl;
	}

	void outputDate() {
		date.output();
	}
	Date GetDate() {
		return date;
	}
	bool operator==(const Diary& A)const {
		return date == A.date;
	}
	bool operator<(const Diary& A)const {
		return date < A.date;
	}
	bool operator>(const Diary& A)const {
		return date > A.date;
	}
};
vector<Diary>diary;
void InputAllDiary() {
	ifstream fin("diary.txt");
	diary.push_back(Diary());
	int now = diary.size() - 1;
	while (diary[now].input(fin)) {
		diary.push_back(Diary());
		now = diary.size() - 1;
	}
	fin.close();
}
void OutputAllDiary() {
	int num = 0; 
	for (int i = 0; i < diary.size() - 1; i++) {
		SetColor("Yellow"); printf("No.%d: ", ++num);
		SetColor("Blue"); diary[i].outputDate();
	}
	if(num == 0){
		SetColor("Red");
		printf("现在没有日记\n");
	}
}
void OutputAllDiary(Date L, Date R) {
	int num = 0;
	for (int i = 0; i < diary.size() - 1; i++) {
		if (diary[i].GetDate() > R)continue;
		if (diary[i].GetDate() < L)continue;
		SetColor("Yellow"); printf("No.%d: ", ++num);
		SetColor("Blue"); diary[i].outputDate();
	}
	if(num == 0){
		SetColor("Red");
		printf("该时间段内没有日记\n");
	}
}
bool CheckFormat(int n, char* s[]) {
	if (n != 3)return false;
	int len = strlen(s[1]);
	int cnt = 0;
	for (int i = 0; i < len; i++) {
		if (s[1][i] == '/')cnt++;
		else if (s[1][i] > '9' || s[1][i] < '0')return false;
	}
	if (cnt != 2)return false;

	len = strlen(s[2]);
	cnt = 0;
	for (int i = 0; i < len; i++) {
		if (s[2][i] == '/')cnt++;
		else if (s[2][i] > '9' || s[2][i] < '0')return false;
	}
	if (cnt != 2)return false;
	return true;
}
int main(int argc, char* argv[]) {
	//freopen("in.txt", "r", stdin);
	InputAllDiary();
	if (argc == 1) {
		OutputAllDiary();
		SetColor("Blue");
	}
	else {
		if (!CheckFormat(argc, argv)) {
			SetColor("Red");
			printf("请输入正确的参数格式:xxxx/xx/xx xxxx/xx/xx\n");
		}
		else {
			int LYear = 0, LMonth = 0, LDay = 0;
			int RYear = 0, RMonth = 0, RDay = 0;
			sscanf(argv[1], "%d/%d/%d", &LYear, &LMonth, &LDay);
			sscanf(argv[2], "%d/%d/%d", &RYear, &RMonth, &RDay);
			OutputAllDiary(Date(LYear, LMonth, LDay), Date(RYear, RMonth, RDay));
			SetColor("Blue");
		}
	}
	
	return 0;
}
