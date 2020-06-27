#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

double TEMP_low(double x) { // �µ�
	if (x < 20) return 1;
	else if (x >= 20 && x < 25) return ((-x) + 25) / 5;
	else return 0;
}
double TEMP_mid(double x) {
	if (x > 20 && x < 25) return (x - 20) / 5;
	else if (x >= 25 && x <= 30) return ((-x) + 30) / 5;
	else return 0;
}
double TEMP_high(double x) {
	if (x > 25 && x < 30) return (x - 25) / 5;
	else if (x >= 30) return 1;
	else return 0;
}

double HUMID_low(double x) { // ����
	if (x <= 20) return 1;
	else if (x > 20 && x < 40) return ((-x) + 40) / 20;
	else return 0;
}
double HUMID_mid(double x) {
	if (x > 20 && x < 40) return (x - 20) / 20;
	else if (x >= 40 && x <= 60) return 1;
	else if (x > 60 && x < 80) return ((-x) + 80) / 20;
	else return 0;
}
double HUMID_high(double x) {
	if (x > 60 && x < 80) return (x - 60) / 20;
	else if (x >= 80) return 1;
	else return 0;
}

int main() {
	double temp, humidity, sum_top, sum_bottom;
	cin >> temp >> humidity;
	double a, b, c, x, y, z; // abc�� �µ� low,mid,high ��, xyz�� ����low,mid,high��
	a = TEMP_low(temp);
	b = TEMP_mid(temp);
	c = TEMP_high(temp);
	x = HUMID_low(humidity);
	y = HUMID_mid(humidity);
	z = HUMID_high(humidity);

	//  0% : ax, ay
	// 40% : bx, ax
	// 70% : by, bz
	//100% : cx, cy, cz

	sum_top = (min(a, x) + min(a, y)) * 0 + (min(b, x) + min(a, z)) * 40 + (min(b, y) + min(b, z)) * 70 + (min(c, x) + min(c, y) + min(c, z)) * 100;
	sum_bottom = min(a, x) + min(a, y) + min(b, x) + min(a, z) + min(b, y) + min(b, z) + min(c, x) + min(c, y) + min(c, z);
	cout << sum_top / sum_bottom << endl;
}