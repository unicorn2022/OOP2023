﻿#pragma warning(disable:4996)
#include "../Diary/DiaryManager.h"
using namespace std;

int main(int argc, char* argv[]) {
	SetColor(Color::Blue);
	if (argc == 2) {
		freopen(argv[1], "r", stdin);
	}

	DiaryManager::GetInstance().InputNewDiary();
	return 0;
}
