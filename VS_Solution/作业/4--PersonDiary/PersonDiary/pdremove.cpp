/*
pdremove:
	������������ڣ�ɾ��diary.txt�еĶ�Ӧ�ռ�
	���룺stdin
	�����stdout
	����ֵ��0��ʾɾ���ɹ���1��ʾɾ��ʧ��
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
		printf("�������ռǵ����ڣ���,��,��\n");
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
		SetColor("Red");
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
bool StoreDiary(Date now) {
	ofstream fout("diary.txt", ios::out);
	bool flag = false;
	//���ռǰ������ڴ�С��������
	for (int i = 0; i < diary.size(); i++) {
		if (diary[i].GetDate() == now) {
			flag = true;
			continue;
		}
		diary[i].output(fout);
	}
	fout.close();
	SetColor("Blue");
	if (flag) printf("ɾ���ɹ�\n");
	else printf("ɾ��ʧ��,Ŀ���ռǲ�����\n");
	SetColor("Blue");
	return flag;
}
int main(int argc, char* argv[]) {
	if(argc == 2){
		freopen(argv[1], "r", stdin);
	} 
	//freopen("in.txt", "r", stdin);
	Date now;
	now.input();
	SetColor("Blue");
	InputAllDiary();
	//OutputAllDiary();
	if (StoreDiary(now))return 0;
	else return 1;
}
