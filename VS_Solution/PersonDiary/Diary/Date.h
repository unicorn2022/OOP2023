#pragma once
#include <iostream>
#include <fstream>
#include "Utils.h"

class Date {
public:
	Date() : year(0), month(0), day(0) {}
	Date(int year, int month, int day) : year(year), month(month), day(day) {}

	/*
	* @brief 从控制台读取日期
	*/
	bool input();

	/*
	* @brief 从文件读取日期
	* @param fin 文件输入流
	*/
	bool input(std::ifstream& fin);

	/*
	* @brief 输出日期到控制台
	*/
	void output() const;

	/*
	* @brief 输出日期到文件
	* @param fout 文件输出流
	*/
	void output(std::ostream& fout) const;

public:
	bool operator==(const Date& A) const;
	bool operator<(const Date& A) const;
	bool operator>(const Date& A) const;

private:
	int year, month, day;
};

