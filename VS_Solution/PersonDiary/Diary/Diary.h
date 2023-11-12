#pragma once
#include <vector>
#include "Date.h"

class Diary {
public:
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

	/*
	* @brief 输出日期到控制台
	*/
	void outputDate() const;

public:
	Date GetDate() const { return date; }

public:
	bool operator==(const Diary& A) const;
	bool operator<(const Diary& A) const;
	bool operator>(const Diary& A) const;

private:
	Date date;
	std::vector<std::string> content;
};

