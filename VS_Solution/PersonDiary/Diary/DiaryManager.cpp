#include "DiaryManager.h"
using namespace std;

void DiaryManager::InputNewDiary() {
	Diary tmp;
	tmp.input();
	diary.insert(tmp);
}

void DiaryManager::OutputAllDiary(Date L, Date R) {
	auto it1 = diary.lower_bound(Diary(L));
	auto it2 = diary.upper_bound(Diary(R));
	SetColor(Color::Yellow);
	if (it1 == it2) {
		cout << "No diary in this period!\n";
	}
	else {
		cout << "Diary is as following:\n";
		for (auto it = it1; it != it2; it++) {
			it->output();
		}
	}
	SetColor(Color::Clear);
}

void DiaryManager::RemoveDiary(Date now) {
	SetColor(Color::Yellow);
	if(diary.find(Diary(now)) != diary.end()) {
		diary.erase(Diary(now));
		cout << "Remove success" << endl;
	}
	else {
		cout << "No diary on this day!" << endl;
	}
	SetColor(Color::Clear);

}

void DiaryManager::InputAllDiary() {
	ifstream fin("C://Users//Admin//Desktop//diary.txt");
	while (true) {
		Diary tmp;
		if (!tmp.input(fin)) break;
		diary.insert(tmp);
	}
	fin.close();
}

void DiaryManager::StoreDiary() {
	ofstream fout("C://Users//Admin//Desktop//diary.txt", ios::out);
	for(auto it = diary.begin(); it != diary.end(); it++) {
		it->output(fout);
	}
	fout.close();
}

DiaryManager::DiaryManager() {
	InputAllDiary();
}

DiaryManager::~DiaryManager() {
	StoreDiary();
}
