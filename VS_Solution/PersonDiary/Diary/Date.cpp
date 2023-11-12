#pragma warning(disable:4996)
#include "Date.h"

bool Date::input() {
	SetColor("Blue");
	printf("请输入日记的日期：年 月 日\n");
	SetColor("Red");
	std::cin >> year >> month >> day;
	if (year == -1)return false;
	printf("%4d %02d %02d\n", year, month, day);
	return true;
}

bool Date::input(std::ifstream& fin) {
	fin >> year >> month >> day;
	if (year == -1)return false;
	return true;
}

void Date::output() const {
	printf("%04d/%02d/%02d\n", year, month, day);
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
