#include <iostream>
#include <string>
#include <memory>
using namespace std;

class Student {
    int id;
    string name;
public:
    Student(int id, string name) : id(id), name(name) {}
	~Student() { cout << "Student " << id << " is destructed." << endl; }
};

int main() {
    shared_ptr<Student> p = make_shared<Student>(1, "s1");
    shared_ptr<Student> p1 = p;
    printf("p:%d p1:%d\n", p.use_count(), p1.use_count());
    shared_ptr<Student> p2 = p;
    printf("p:%d p1:%d p2:%d\n", p.use_count(), p1.use_count(), p2.use_count());
    p = make_shared<Student>(2, "s1");
    printf("p:%d p1:%d p2:%d\n", p.use_count(), p1.use_count(), p2.use_count());

    return 0;
}