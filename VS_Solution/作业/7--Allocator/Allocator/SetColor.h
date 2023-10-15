#pragma once
#include <Windows.h>
#include <iostream>
using namespace std;
//¸ü¸ÄÑÕÉ«
static void SetColor(string color) {
	HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
	if (color == "Red")SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_INTENSITY);
	else if (color == "Blue")SetConsoleTextAttribute(hdl, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	else if (color == "Green")SetConsoleTextAttribute(hdl, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	else if (color == "Yellow")SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}
static ostream& red(ostream& os) {
	SetColor("Red");
	return os;
}
static ostream& blue(ostream& os) {
	SetColor("Blue");
	return os;
}
static ostream& yellow(ostream& os) {
	SetColor("Yellow");
	return os;
}