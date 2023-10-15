#pragma warning(disable:4996)
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <iostream>
#include <cmath>
#include <Windows.h>
using namespace std;
const int INF = 0x3f3f3f3f;
typedef long long LL;
class Fraction {
	LL numerator;  //分子
	LL denominator;//分母
	//计算A和B的最大公约数
	static LL gcd(LL A, LL B) {
		if (A < B)return gcd(B, A);
		if (B == 0)return A;
		return gcd(B, A % B);
	}
	//最小公倍数
	static LL lcm(LL A, LL B) {
		return 1ll * A * B / gcd(A, B);
	}
	//判断浮点数是否为整数
	static bool isInteger(double A) {
		return ceil(A) == floor(A);
	}
public:
	//将分数化简
	void Simplify() {
		if (numerator == 0)return;
		int GCD = gcd(numerator, denominator);
		numerator /= GCD;
		denominator /= GCD;
	}
	//设置分子
	void setNumerator(LL numerator) {
		this->numerator = numerator;
	}
	//设置分母
	void setDenominator(LL denominator) {
		this->denominator = denominator;
	}
	//获取分子
	LL getNumerator() {
		return this->numerator;
	}
	//获取分母
	LL getDenominator() {
		return this->denominator;
	}
	
	//缺省构造函数
	Fraction() {
		numerator = 0;
		denominator = 1;
	}
	//有两个整数作为参数的构造函数
	Fraction(LL numerator, LL denominator) {
		this->numerator = numerator;
		this->denominator = denominator;
		this->Simplify();
	}
	//拷贝构造函数
	Fraction(const Fraction &A) {
		this->numerator = A.numerator;
		this->denominator = A.denominator;
		this->Simplify();
	}

	//算术运算符重载
	const Fraction operator+(const Fraction& A)const {
		Fraction ans;
		ans.denominator = lcm(A.denominator, this->denominator);
		ans.numerator = ans.denominator / this->denominator * this->numerator;
		ans.numerator += ans.denominator / A.denominator * A.numerator;
		ans.Simplify();
		return ans;
	}
	const Fraction operator-(const Fraction& A)const {
		Fraction ans;
		ans.denominator = lcm(A.denominator, this->denominator);
		ans.numerator = ans.denominator / this->denominator * this->numerator;
		ans.numerator -= ans.denominator / A.denominator * A.numerator;
		ans.Simplify();
		return ans;
	}
	const Fraction operator*(const Fraction& A)const {
		Fraction ans;
		ans.denominator =  this->denominator * A.denominator;
		ans.numerator = this->numerator * A.numerator;
		ans.Simplify();
		return ans;
	}
	const Fraction operator/(const Fraction& A)const {
		Fraction ans;
		ans.denominator = this->denominator * A.numerator;
		ans.numerator = this->numerator * A.denominator;
		ans.Simplify();
		return ans;
	}

	//关系运算符重载
	bool operator<(const Fraction& A)const {
		int LCM = lcm(this->denominator, A.denominator);
		int this_numrator = LCM / this->denominator * this->numerator;
		int A_numerator = LCM / A.denominator * A.numerator;
		return this_numrator < A_numerator;
	}
	bool operator>(const Fraction& A)const {
		return A < *this;
	}
	bool operator<=(const Fraction& A)const {
		return !(*this > A);
	}
	bool operator>=(const Fraction& A)const {
		return !(*this < A);
	}
	bool operator==(const Fraction& A)const {
		return this->numerator == A.numerator && this->denominator == A.denominator;
	}
	bool operator!=(const Fraction& A)const {
		return !(*this == A);
	}

	//类型转化为双精度
	operator double() const {
		return (double)this->numerator / (double)this->denominator;
	}

	//转化为字符串
	string toString() {
		char s[100];
		sprintf(s, "%d/%d", this->numerator, this->denominator);
		string ans(s);
		return ans;
	}

	//从一个有限精度的浮点数字符串转化为Fraction:1.414
	void conversion(string value) {
		double val = 0;
		sscanf(value.c_str(), "%lf", &val);
		denominator = 1;
		while (!isInteger(val * denominator))denominator *= 10;
		numerator = val * denominator;
		Simplify();
	}
};
//从输入流中读取,格式:分子 分母
static istream& operator>>(istream& is, Fraction& A) {
	LL numerator = 0, denominator = 1;
	is >> numerator >> denominator;
	A.setNumerator(numerator);
	A.setDenominator(denominator);
	A.Simplify();
	return is;
}
//向输出流中输出
static ostream& operator<<(ostream& os, Fraction& A) {
	return os << A.getNumerator() << '/' << A.getDenominator();
}
//更改颜色
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
int main() {
	Fraction A;
	cout << "default ctor(A):" << red << A << blue << endl << endl;
	
	cout << "please input two integer" << endl;
	cout << yellow;
	cin >> A;
	cout << blue;
	cout << "ctor takes two integers as parameters(A):" << red << A << blue << endl << endl;

	Fraction B(A);
	cout << "copy ctor(B):" << red << A << blue << endl << endl;

	cout << "arithmetical operators:" << endl;
	Fraction C = A + B;
	cout << "A + B : " << red << C << blue << endl;
	C = A - B;
	cout << "A - B : " << red << C << blue << endl;
	C = A * B;
	cout << "A * B : " << red << C << blue << endl;
	C = A / B;
	cout << "A / B : " << red << C << blue << endl;
	cout << endl;

	cout << "relational operators:" << endl;
	cout << "A <  B : " << red << (A <  B ? "true" : "false") << blue << endl;
	cout << "A <= B : " << red << (A <= B ? "true" : "false") << blue << endl;
	cout << "A == B : " << red << (A == B ? "true" : "false") << blue << endl;
	cout << "A != B : " << red << (A != B ? "true" : "false") << blue << endl;
	cout << "A >= B : " << red << (A >= B ? "true" : "false") << blue << endl;
	cout << "A >  B : " << red << (A >  B ? "true" : "false") << blue << endl;
	cout << endl;

	cout << "type cast to double:" << red << (double)A << blue << endl << endl;

	cout << "to string(A):" << red << A.toString() << blue << endl << endl;

	cout << "please input a finite decimal string like: 1.414" << endl;
	cout << yellow;
	string temp;
	cin >> temp;
	cout << blue;
	A.conversion(temp);
	cout << "conversion from a finite decimal string(A):" << red << A << blue << endl;
	return 0;
}