#pragma once

#include <string>
#include <Windows.h>

enum class Color {
	Red,
	Blue,
	Green,
	Yellow,
	Clear
};

static void SetColor(Color color) {
	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
	if (color == Color::Red)SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_INTENSITY);
	else if (color == Color::Blue)SetConsoleTextAttribute(hdl, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	else if (color == Color::Green)SetConsoleTextAttribute(hdl, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	else if (color == Color::Yellow)SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	else if (color == Color::Clear)SetConsoleTextAttribute(hdl, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}