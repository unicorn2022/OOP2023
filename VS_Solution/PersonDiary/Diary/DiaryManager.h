#pragma once
#include "Diary.h"

class DiaryManager {
public:
	static DiaryManager& GetInstance() {
		static DiaryManager instance;
		return instance;
	}

public:
	void InputNewDiary();
	void OutputAllDiary(Date L, Date R);

private:
	void InputAllDiary();
	void StoreDiary();

private:
	DiaryManager();
	~DiaryManager();

	std::vector<Diary> diary;
};

