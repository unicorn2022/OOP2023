#pragma warning(disable:4996)
#include "Date.h"

bool Date::input() {
	printf("请输入日记的日期：年 月 日\n");
	SetColor(Color::Red);
	std::cin >> year >> month >> day;
	SetColor(Color::Blue);
	if (year == -1)return false;
	return true;
}

bool Date::input(std::ifstream& fin) {
	if (fin.eof()) return false;
	fin >> year >> month >> day;
	if (year == -1)return false;
	return true;
}

void Date::output() const {
	SetColor(Color::Red);
	printf("%04d/%02d/%02d\n", year, month, day);
	SetColor(Color::Blue);
}

void Date::output(std::ostream& fout) const {
	fout << year << " " << month << " " << day << std::endl;
}

bool Date::operator==(const Date& A) const {
	return year == A.year && month == A.month && day == A.day;
}

bool Date::operator<(const Date& A) const {
	if (year == -1)return false;
	if (A.year == -1)return true;

	if (year != A.year)return year < A.year;
	if (month != A.month)return month < A.month;
	return day < A.day;
}

bool Date::operator>(const Date& A) const {
	return !(*this < A) && !(*this == A);
}
