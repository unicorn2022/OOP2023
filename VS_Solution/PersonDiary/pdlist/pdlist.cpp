#include "../Diary/Diary.h"
using namespace std;
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
	if (num == 0) {
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
	if (num == 0) {
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
