#pragma warning(disable:4996)
#include "../Diary/DiaryManager.h"
using namespace std;

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
	SetColor(Color::Blue);
	if (argc == 1) {
		DiaryManager::GetInstance().OutputAllDiary();
	}
	else {
		if (!CheckFormat(argc, argv)) {
			SetColor(Color::Red);
			printf("请输入正确的参数格式:xxxx/xx/xx xxxx/xx/xx\n");
		}
		else {
			int LYear = 0, LMonth = 0, LDay = 0;
			int RYear = 0, RMonth = 0, RDay = 0;
			sscanf(argv[1], "%d/%d/%d", &LYear, &LMonth, &LDay);
			sscanf(argv[2], "%d/%d/%d", &RYear, &RMonth, &RDay);
			DiaryManager::GetInstance().OutputAllDiary(Date(LYear, LMonth, LDay), Date(RYear, RMonth, RDay));
		}
	}

	return 0;
}
