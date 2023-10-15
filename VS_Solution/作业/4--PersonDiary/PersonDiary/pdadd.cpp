/*
pdadd:
	���ռ��ļ�dairy.txt�����һ��ʵ��,�����ռǵ�ʱ�䡢����
	ʱ�䣺����ռǵ�ʱ����dairy.txt�е������ռ��ظ�,��Ὣԭ�����ռ��滻��
	���ݣ���stdin�����ж�ȡ�ռ����ݣ�ֱ��������һ��'.'/EOF��ֹ
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
//�����������ɫ
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
		printf("�������ռǵ����ڣ��� �� ��\n");
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

	//����Ϊ-1/-1/-1��������(�����ļ���β)
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
		printf("�����������ռǵ�����,��.��EOF����\n");
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
		printf("�ռ�����ɹ�\n");
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
void InputNewDiary() {
	diary.push_back(Diary());
	diary[0].input();
}
void InputOldDiary() {
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
	SetColor("Blue");
	printf("all diary:\n");
	for (int i = 0; i < diary.size(); i++) {
		SetColor("Yellow"); printf("No.%d\n", i);
		diary[i].output();
	}
}
void StoreDiary() {
	ofstream fout("diary.txt",ios::out);
	//���ռǰ������ڴ�С��������
	for (int i = 1; i < diary.size(); i++) {
		if (diary[i] == diary[0]) {
			diary[0].output(fout);
		}
		else if (diary[i] > diary[0] && diary[i-1] < diary[0]) {
			diary[0].output(fout);
			diary[i].output(fout);
		}
		else {
			diary[i].output(fout);
		}
	}
	fout.close();
}
int main(int argc, char* argv[]) {
	if(argc == 2){
		freopen(argv[1], "r", stdin);
	} 
	
	InputNewDiary();
	InputOldDiary();
	//OutputAllDiary();
	StoreDiary();
	SetColor("Blue");
	return 0;
}
