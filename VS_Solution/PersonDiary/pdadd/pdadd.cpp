#include <iostream>
#include <fstream>
#include "../Diary/Diary.h"

using namespace std;
vector<Diary> diary;

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

void StoreDiary() {
	ofstream fout("diary.txt", ios::out);
	//将日记按照日期从小到大排列
	for (int i = 1; i < diary.size(); i++) {
		if (diary[i] == diary[0]) {
			diary[0].output(fout);
		}
		else if (diary[i] > diary[0] && diary[i - 1] < diary[0]) {
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
	if (argc == 2) {
		freopen(argv[1], "r", stdin);
	}

	InputNewDiary();
	InputOldDiary();
	StoreDiary();
	SetColor("Blue");
	return 0;
}
