#pragma once
#include "Diary.h"
#include <set>

class DiaryManager {
public:
	static DiaryManager& GetInstance() {
		static DiaryManager instance;
		return instance;
	}

public:
	void InputNewDiary();
	void OutputAllDiary(Date L = DATE_MIN, Date R = DATE_MAX);
	void RemoveDiary(Date now);

private:
	void InputAllDiary();
	void StoreDiary();

private:
	DiaryManager();
	~DiaryManager();

	std::set<Diary> diary;
};

