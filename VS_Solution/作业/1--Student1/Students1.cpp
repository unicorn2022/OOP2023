#include <iostream>
#include <iomanip>
#include <string>
using namespace std;
const int maxn = 100 + 10;
const int Inf = 1e9;
struct Student {
	int no;
	string name;
	int score1, score2, score3;
	double average;
	void set(int No, string Name, int Score1, int Score2, int Score3) {
		no = No;
		name = Name;
		score1 = Score1;
		score2 = Score2;
		score3 = Score3;
		average = (double)(Score1 + Score2 + Score3) / 3.0;
	}
	void PrintName() {
		cout.flags(ios::left);
		cout << setw(8) << "no";
		cout << setw(8) << "name";
		cout << setw(8) << "score1";
		cout << setw(8) << "score2";
		cout << setw(8) << "score3";
		cout << setw(8) << "average";
		cout << endl;
	}
	void Print() {
		cout.flags(ios::left);
		cout << setw(8) << no;
		cout << setw(8) << name;
		cout << setw(8) << score1;
		cout << setw(8) << score2;
		cout << setw(8) << score3;
		cout << setw(8) << average;
		cout << endl;
	}
}st[maxn];
void Init();
void Output(string, double, double, double);
int main()
{
	Init();
	st[1].PrintName();
	double sumScore1 = 0, sumScore2 = 0, sumScore3 = 0;
	int minScore1 = Inf, minScore2 = Inf, minScore3 = Inf;
	int maxScore1 = -Inf, maxScore2 = -Inf, maxScore3 = -Inf;

	for (int i = 1; i <= 10; i++) {
		st[i].Print();
		sumScore1 += st[i].score1;
		sumScore2 += st[i].score2;
		sumScore3 += st[i].score3;
		minScore1 = min(minScore1, st[i].score1);
		minScore2 = min(minScore2, st[i].score2);
		minScore3 = min(minScore3, st[i].score3);
		maxScore1 = max(maxScore1, st[i].score1);
		maxScore2 = max(maxScore2, st[i].score2);
		maxScore3 = max(maxScore3, st[i].score3);
	}
	Output("average", sumScore1 / 10.0, sumScore2 / 10.0, sumScore3 / 10.0);
	Output("min", minScore1, minScore2, minScore3);
	Output("max", maxScore1, maxScore2, maxScore3);
		
	return 0;
}
void Init() {
	st[1].set(1, "K.Weng", 5, 5, 5);
	st[2].set(2, "T.Dixon", 4, 3, 2);
	st[3].set(3, "V.Chu", 3, 4, 5);
	st[4].set(4, "L.Tsom", 4, 3, 4);
	st[5].set(5, "L.Lee", 3, 4, 3);
	st[6].set(6, "I.Young", 4, 2, 5);
	st[7].set(7, "K.Hiro", 4, 2, 1);
	st[8].set(8, "G.Ping", 4, 4, 4);
	st[9].set(9, "H.Gu", 2, 3, 4);
	st[10].set(10, "J.Jon", 5, 4, 3);
}
void Output(string name, double x, double y, double z) {
	cout << setw(8) << " ";
	cout << setw(8) << name;
	cout << setw(8) << x;
	cout << setw(8) << y;
	cout << setw(8) << z;
	cout << endl;
}