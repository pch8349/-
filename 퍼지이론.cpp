#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

double POWER_0(double x) { // (온도, 습도) low, low / low, mid 인 경우
	if (x < 10) return 1;
	else if (x >= 10 && x < 30) return ((-x) + 30) / 20;
	else return 0;
}
double POWER_40(double x) { // (온도, 습도) mid, low / low, high
	if (x > 0 && x < 20) return x / 20;
	else if (x = 20 && x <= 40) return 1;
	else if (x > 40 && x < 60) return ((-x) + 60) / 20;
	else return 0;
}
double POWER_70(double x) { // (온도, 습도) mid, mid / high, low
	if (x > 30 && x < 50) return (x - 30) / 20;
	else if (x >= 50 && x <= 70) return 1;
	else if (x > 70 && x < 90) return ((-x) + 90) / 20;
	else return 0;
}
double POWER_100(double x) {// (온도, 습도)mid, high / high, high/ high, mid
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
	double temp, humidity;
	cin >> temp >> humidity;
	
}