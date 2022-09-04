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

void lengthComparison(double x1, double y1, double x2, double y2);
void lengthComparison_Struct(Point& a, Point& b);
void isTriangleComparison(double a, double b, double c);
void isTriangleComparison_Struct(Triangle& t);

int main() {
	double coordinates[6];
	Point points[3];

	for (short i = 0; i < sizeof(coordinates) / sizeof(coordinates[0]); i++) {
		cout << (char)('X' + i % 2) << " of point " << (char)('A' + i / 2) << ": ";
		double tmp;
		string wrongInput;
		while (scanf_s("%lf", &tmp) == 0) {
			char clear;
			while ((clear = fgetc(stdin)) != '\n' && clear != EOF) {
				wrongInput += clear;
			}
			cout << "ERROR: '"<< wrongInput << "' is not a proper coordinate. Please put in a correct floating point number" << endl;
			cout << (char)('X' + i % 2) << " of point " << (char)('A' + i / 2) << ": ";
		}
		coordinates[i] = tmp;
	}
	for (short i = 0; i < sizeof(coordinates) / sizeof(coordinates[0]); i+=2) {
		cout << "Point " << (char)('A' + i / 2) << "(" << coordinates[i] << "," << coordinates[i + 1] << ")" << endl;
		points[i / 2] = { coordinates[i], coordinates[i + 1] };
	}
	Triangle t = { points[0], points[1], points[2] };
	double lengths[3] = { getStructLength(points[0], points[1]), getStructLength(points[0], points[2]), getStructLength(points[1], points[2])};

	cout << "+++ Calculating length between 2 points +++" << endl;
	cout << endl << "Between A and B:" << endl;
	lengthComparison(coordinates[0], coordinates[1], coordinates[2], coordinates[3]);
	lengthComparison_Struct(points[0], points[1]);
	cout << endl << "Between A and C:" << endl;
	lengthComparison(coordinates[0], coordinates[1], coordinates[4], coordinates[5]);
	lengthComparison_Struct(points[0], points[2]);
	cout << endl << "Between B and C:" << endl;
	lengthComparison(coordinates[2], coordinates[3], coordinates[4], coordinates[5]);
	lengthComparison_Struct(points[1], points[2]);
	cout << endl << "+++ Checking if 3 side lengths are a triangle +++" << endl;
	isTriangleComparison(lengths[0], lengths[1], lengths[2]);
	isTriangleComparison_Struct(t);
}


void lengthComparison(double x1, double y1, double x2, double y2) {
	cout << "C++:" << endl;
	printExecTime_Length(x1, y1, x2, y2, getLength);
	cout << "ASM:" << endl;
	printExecTime_Length(x1, y1, x2, y2, getLengthASM);
}

void lengthComparison_Struct(Point& a, Point& b) {
	cout  << "C++ struct:" << endl;
	printExecTime_LengthStruct(a, b, getStructLength);
	cout  <<  "ASM struct:" << endl;
	printExecTime_LengthStruct(a, b, getStructLengthASM);
}

void isTriangleComparison(double a, double b, double c) {
	cout << "C++:" << endl;
	printExecTime_Triangle(a, b, c, isTriangle);
	cout  << "ASM:" << endl;
	printExecTime_Triangle(a, b, c, isTriangleASM);
}

void isTriangleComparison_Struct(Triangle& t) {
	cout << "C++ struct:" << endl;
	printExecTime_TriangleStruct(t, isTriangleStruct);
	cout << "ASM struct:" << endl;
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