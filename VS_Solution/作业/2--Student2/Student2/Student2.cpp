//剩余课程平均成绩没写
#pragma warning(disable:4996)
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <set>
using namespace std;
const int maxn = 100 + 10;
const int Inf = 1e9;
class StudentRecord {
	int maxCourse;
	struct Course {
		int courseID;
		string courseName;
		int score;
		int maxScore,minScore,studentCnt;
		double sumScore;
		Course() {};
		Course(int id, string name, int s) {
			courseID = id;
			courseName = name;
			score = s;
			if (s != 0)maxScore = minScore = s;
			else maxScore = -Inf, minScore = Inf;
			studentCnt = 1;
			sumScore = s;
		}
		void insert(int s) {
			maxScore = max(maxScore, s);
			minScore = min(minScore, s);
			studentCnt++;
			sumScore += s;
		}
		bool operator<(const Course& A)const {
			return courseID < A.courseID;
		}
		bool operator==(const Course& A)const {
			return courseID == A.courseID;
		}
		bool operator>(const Course& A)const {
			return courseID > A.courseID;
		}
	};
	struct Student {
		int studentID;
		string studentName;
		set<Course>studentCourse;
		Student() {};
		Student(int id, string name) {
			studentID = id;
			studentName = name;
		}
		bool insertCourse(Course now) {
			if (studentCourse.find(now) != studentCourse.end()) {
				cout << "already input this course " << now.courseName << ",Please input another\n";
				return false;
			}
			studentCourse.insert(now);
			return true;
		}
		void outputCourse() {
			cout << "The student " << studentName << " 's scores are as bellow\n";
			cout.flags(ios::left);
			for (set<Course>::iterator it = studentCourse.begin(); it != studentCourse.end(); it++) {
				cout << "ID:";
				cout << setw(5) << it->courseID;
				cout << "name:";
				cout << it->courseName;
				cout << "score:";
				cout << it->score << endl;
			}
			cout << endl;
		}
		void Print() {//输出当前学生的课程及成绩
			cout.flags(ios::left);
			cout << "No." << studentID << " ";
			cout << studentName << ":" << endl;
			//输出课程名称(表头)
			for (set<Course>::iterator it = studentCourse.begin(); it != studentCourse.end(); it++) {
				cout << setw(8) << it->courseName;
			}
			cout << setw(8) << "average";
			cout << endl;
			//输出该学生的得分
			int cnt = 0;
			double sum = 0;
			for (set<Course>::iterator it = studentCourse.begin(); it != studentCourse.end(); it++) {
				cout << setw(8) << it->score;
				cnt++; sum += it->score;
			}
			cout << setw(8) << sum/cnt;
			cout << endl;
		}
		bool operator<(const Student& A)const {
			return studentID < A.studentID;
		}
		bool operator==(const Student& A)const {
			return studentID == A.studentID;
		}
		bool operator>(const Student& A)const {
			return studentID > A.studentID;
		}
	};
	set<Student>totalStudent;
	set<Course>totalCourse;
public:
	//输入所有课程
	void InputCourse() {
		while (1) {
			int id;
			string courseName;
			cout << "Please input the course ID (end with -1) and its name (don't have space):\n";
			cin >> id;
			if (id == -1) {
				cout << "Input all courses\n\n";
				break;
			}
			//cout << "Please input the course's name :\n";
			cin >> courseName;

			if (totalCourse.find(Course(id,"",0)) != totalCourse.end()) {
				cout << "Already input this course\n\n";
				continue;
			}

			totalCourse.insert(Course(id, courseName, 0));
			cout << "Input course " << courseName << " Complete\n\n";
		}
	}
	//输出所有课程
	void OutputAllCourse() {
		cout << "All Course are as behind\n";
		cout.flags(ios::left);
		for (set<Course>::iterator it = totalCourse.begin(); it != totalCourse.end(); it++) {
			cout << "ID:";
			cout << setw(5) << it->courseID;
			cout << "name:";
			cout << it->courseName << endl;
		}
		cout << endl;
	}
	//输入一个学生的信息
	void InputStudent(int No, string Name) {
		cout << "\nPlease input the student's course\n";
		OutputAllCourse();
		Student nowStudent = Student(No, Name);
		while (1) {
			int id, score;
			cout << "Please input the course ID (end with -1) and the student's score:\n";
			cin >> id;
			if (id == -1) {
				cout << "Input all of the student's courses\n\n";
				break;
			}
			//cout << "Please input the student's score\n";
			cin >> score;
			set<Course>::iterator it = totalCourse.find(Course(id, "", score));

			if (totalCourse.find(Course(id, "", 0)) == totalCourse.end()) {
				cout << "Don't exist this course, please input another\n\n";
				continue;
			}

			Course nowCourse = *it;
			nowCourse.score = score;
			bool flag = nowStudent.insertCourse(nowCourse);
			if (flag) {//插入成功，更新课程
				totalCourse.erase(it);
				nowCourse.insert(score);
				totalCourse.insert(nowCourse);
			}
		}
		totalStudent.insert(nowStudent);
		cout << "Input the student " << Name << " Complete\n\n";
	}
	//输出所有学生,课程
	void OutputAll() {
		cout.flags(ios::left);
		cout << "All students are as behind\n";
		for (set<Student>::iterator it = totalStudent.begin(); it != totalStudent.end(); it++) {
			Student now = *it;
			now.Print();
		}
		cout << endl;
		cout << "All Courses are as behind\n";
		for (set<Course>::iterator it = totalCourse.begin(); it != totalCourse.end(); it++) {
			cout << "ID:";
			cout << setw(5) << it->courseID;
			cout << "name:";
			cout << setw(10) << it->courseName;
			cout << "max:";
			cout << setw(5) << it->maxScore;
			cout << "min:";
			cout << setw(5) << it->minScore;
			cout << "average:";
			cout << setw(5) << it->sumScore/it->studentCnt;
			cout << endl;
		}
		cout << endl;
	}
};
StudentRecord studentRecord;
int main()
{
	cout << "Input 1 for File input, 0 for KeyBoard input\n";
	int op; cin >> op;
	if(op==1)freopen("in.txt", "r", stdin);
	cout << "Please input all the course with ID and name\n";
	studentRecord.InputCourse();
	studentRecord.OutputAllCourse();
	while (1) {
		cout << "Please input a student\n";
		cout << "Please input the student's ID (end with -1) and its name:\n";
		int id;
		string name;
		cin >> id;
		if (id == -1) {
			cout << "Input all students\n\n";
			break;
		}
		//cout << "Please input the student's name (don't have space):\n";
		cin >> name;
		studentRecord.InputStudent(id, name);
	}
	studentRecord.OutputAll();
	return 0;
}