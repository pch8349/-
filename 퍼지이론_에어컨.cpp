#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

double POWER_0(double x) { // (온도, 습도) low, low / low, mid 인 경우./ ax, ay
	if (x < 10) return 1;
	else if (x >= 10 && x < 30) return ((-x) + 30) / 20;
	else return 0;
}
double POWER_40(double x) { // (온도, 습도) mid, low / low, high / bx, az
	if (x > 0 && x < 20) return x / 20;
	else if (x > 40 && x < 60) return ((-x) + 60) / 20;
	else return 0;
}
double POWER_70(double x) { // (온도, 습도) mid, mid / mid, high / by, bz
	if (x > 30 && x < 50) return (x - 30) / 20;
	else if (x >= 50 && x <= 70) return 1;
	else if (x > 70 && x < 90) return ((-x) + 90) / 20;
	else return 0;
}
double POWER_100(double x) {// (온도, 습도)high, low / high, high/ high, mid / cx, cy, cz
	if (x > 70 && x < 90) return (x - 70) / 20;
	else return 1;
}

double TEMP_low(double x) { // 온도
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

double HUMID_low(double x) { // 습도
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
	double a, b, c, x, y, z; // abc는 온도 low,mid,high 순, xyz는 습도low,mid,high순
	a = TEMP_low(temp);
	b = TEMP_mid(temp);
	c = TEMP_high(temp);
	x = HUMID_low(humidity);
	y = HUMID_mid(humidity);
	z = HUMID_high(humidity);

	sum_top = (min(a, x) + min(a, y)) * 0 + (min(b, x) + min(a, z)) * 40 + (min(b, y) + min(b, z)) * 70 + (min(c, x) + min(c, y) + min(c, z)) * 100;
	sum_bottom = min(a, x) + min(a, y) + min(b, x) + min(a, z) + min(b, y) + min(b, z) + min(c, x) + min(c, y) + min(c, z);
	cout << sum_top / sum_bottom << endl;
}