#include <string>
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <thread>
#include <algorithm>


enum class ConsoleColor {
	Clear,	// 原色
	White,	// 白色
	Red,	// 红色
	Green,	// 绿色
	Blue,	// 蓝色
	Yellow,	// 黄色
	Pink,	// 粉色
	Cyan	// 青色
};

static char SetConsoleColor(ConsoleColor color) {
	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
	switch (color) {
	case ConsoleColor::Clear:
		SetConsoleTextAttribute(hdl, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		break;
	case ConsoleColor::White:
		SetConsoleTextAttribute(hdl, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case ConsoleColor::Red:
		SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case ConsoleColor::Green:
		SetConsoleTextAttribute(hdl, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case ConsoleColor::Blue:
		SetConsoleTextAttribute(hdl, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case ConsoleColor::Yellow:
		SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case ConsoleColor::Pink:
		SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	case ConsoleColor::Cyan:
		SetConsoleTextAttribute(hdl, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	default:
		break;
	}
	return '\0';
}

static char PrintPercent(int now, int total, int average) {
	int width = log10(total);

	SetConsoleColor(ConsoleColor::Pink);
	std::cout << std::setw(width) << now;
	SetConsoleColor(ConsoleColor::Clear);
	std::cout << "/" << std::setw(width) << total << " ";

	std::cout << "(";
	if (now == total) {
		SetConsoleColor(ConsoleColor::Green);
		std::cout << "AC";
		SetConsoleColor(ConsoleColor::Clear);
	}
	else {
		int percent = now * 100 / total;
		if (percent > average) SetConsoleColor(ConsoleColor::Red);
		else if (percent == average) SetConsoleColor(ConsoleColor::Blue);
		else SetConsoleColor(ConsoleColor::Green);
		std::cout << percent << "%";
		SetConsoleColor(ConsoleColor::Clear);
	}
	std::cout << ") ";
	return ' ';
}

/*
* @param t: 单位为 ms
*/
static char PrintLastTime(int t) {
	if (t == 0) {
		SetConsoleColor(ConsoleColor::Green);
		std::cout << "success";
		SetConsoleColor(ConsoleColor::Clear);
		return ' ';
	}

	std::cout << " ";
	int hour = t / 3600000;
	t %= 3600000;
	int minute = t / 60000;
	t %= 60000;
	float second = t / 1000.0f;
	if (hour > 0) {
		SetConsoleColor(ConsoleColor::Cyan);
		std::cout << hour;
		SetConsoleColor(ConsoleColor::Clear);
		std::cout << "h";
	}
	if (minute > 0) {
		SetConsoleColor(ConsoleColor::Cyan);
		std::cout << minute;
		SetConsoleColor(ConsoleColor::Clear);
		std::cout << "min";
	}
	if (second > 0) {
		SetConsoleColor(ConsoleColor::Cyan);
		std::cout << second;
		SetConsoleColor(ConsoleColor::Clear);
		std::cout << "s";
	}
	return ' ';
}