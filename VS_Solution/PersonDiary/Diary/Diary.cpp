#pragma warning(disable:4996)
#include "Diary.h"
using namespace std;

bool Diary::input() {
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
		cout << s << endl;
		if (s == "")break;
		if (s == ".")break;
		content.push_back(s);
	}
	content.push_back(".");
	SetColor("Blue");
	printf("日记输入成功\n");
	return true;
}

bool Diary::input(std::ifstream& fin) {
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

void Diary::output() const {
	date.output();
	SetColor("Blue");
	for (int i = 0; i < content.size(); i++)
		cout << content[i] << endl;
}

void Diary::output(std::ostream& fout) const {
	date.output(fout);
	for (int i = 0; i < content.size(); i++)
		fout << content[i] << endl;
}

void Diary::outputDate() const {
	date.output();
}

bool Diary::operator==(const Diary& A) const {
	return date == A.date;
}

bool Diary::operator<(const Diary& A) const {
	return date < A.date;
}

bool Diary::operator>(const Diary& A) const {
	return date > A.date;
}
