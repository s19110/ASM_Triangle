#include <iostream>
#include <math.h>
#include <chrono>

using namespace std;

#pragma pack(1)
struct Point
{
	double x;
	double y;
};

#pragma pack(1)
struct Triangle
{
	Point a;
	Point b;
	Point c;
};

extern "C" double getLengthASM(double x1, double y1, double x2, double y2);
extern "C" bool isTriangleASM(double a, double b, double c);
extern "C" double getStructLengthASM(Point &a, Point &b);
extern "C" bool isTriangleStructASM(Triangle &t);

double getLength(double x1, double y1, double x2, double y2);
double getStructLength(Point &a,  Point &b);
bool isTriangle(double a, double b, double c);
bool isTriangleStruct(Triangle &t);

void printExecTime_Length(double x1, double y1, double x2, double y2, double (*func)(double, double, double, double));
void printExecTime_LengthStruct( Point& a,  Point& b, double (*func)(Point&, Point&));
void printExecTime_Triangle(double a, double b, double c, bool (*func)(double, double, double));
void printExecTime_TriangleStruct(Triangle& t, bool (*func)(Triangle&));

int main() {
	Point a = {1.0,1.0};
	Point b = { 2.0,2.0 };
	Triangle t = { {0.0, 0.0}, {3.0,0.0}, {0.0,5.0} };

	cout << "+++ Calculating length between 2 points +++" << endl;
	cout << endl << "C++:" << endl;
	printExecTime_Length(1.0, 1.0, 2.0, 2.0, getLength);
	cout << endl << "ASM:" << endl;
	printExecTime_Length(1.0, 1.0, 2.0, 2.0, getLengthASM);
	cout << endl << "C++ struct:" << endl;
	printExecTime_LengthStruct(a, b, getStructLength);
	cout << endl <<  "ASM struct:" << endl;
	printExecTime_LengthStruct(a, b, getStructLengthASM);
	cout << endl << "+++ Checking if 3 side lengths are a triangle +++" << endl;
	cout << endl << "C++:" << endl;
	printExecTime_Triangle(3.0, 4.0, 5.0, isTriangle);
	cout << endl << "ASM:" << endl;
	printExecTime_Triangle(3.0, 4.0, 5.0, isTriangleASM);
	cout << endl << "C++ struct:" << endl;
	printExecTime_TriangleStruct(t, isTriangleStruct);
	cout << endl << "ASM struct:" << endl;
	printExecTime_TriangleStruct(t, isTriangleStructASM);
}

void printExecTime_Length(double x1, double y1, double x2, double y2, double (*func)(double, double, double, double)) {
	auto start = chrono::system_clock::now();
	cout << "Length: " << func(x1, y1, x2, y2);
	auto end = chrono::system_clock::now();
	cout << "\t time to execute: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds" << endl;
}

void printExecTime_LengthStruct( Point& a,  Point& b, double (*func)(Point&, Point&)) {
	auto start = chrono::system_clock::now();
	cout << "Length: " << func(a, b);
	auto end = chrono::system_clock::now();
	cout << "\t time to execute: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds" << endl;
}

void printExecTime_Triangle(double a, double b, double c, bool (*func)(double, double, double)) {
	auto start = chrono::system_clock::now();
	cout << "Is triangle: " << (func(a, b, c) ? "yes" : "no");
	auto end = chrono::system_clock::now();
	cout << "\t time to execute: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds" << endl;
}

void printExecTime_TriangleStruct(Triangle &t, bool (*func)(Triangle&)) {
	auto start = chrono::system_clock::now();
	cout << "Is triangle: " << (func(t) ? "yes" : "no");
	auto end = chrono::system_clock::now();
	cout << "\t time to execute: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds" << endl;
}

double getLength(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

double getStructLength( Point& a,  Point& b) {
	return sqrt(pow(a.x - b.x,2) + pow(a.y - b.y, 2));
}

bool isTriangle(double a, double b, double c) {
	return a + b > c && a + c > b && b + c > a;
}

bool isTriangleStruct(Triangle& t) {
	double a = getStructLength(t.a, t.b);
	double b = getStructLength(t.b, t.c);
	double c = getStructLength(t.a, t.c);
	return isTriangle(a, b, c);
}