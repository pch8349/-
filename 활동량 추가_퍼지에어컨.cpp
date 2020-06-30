#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

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

double MOVE_low(double x) {
	if (x <= 20) return 1;
	else if (x > 20 && x < 40) return ((-x) + 40) / 20;
	else return 0;
}
double MOVE_mid(double x) {
	if (x > 20 && x < 40) return (x - 20) / 20;
	else if (x >= 40 && x <= 60) return 1;
	else if (x > 60 && x < 80) return ((-x) + 80) / 20;
	else return 0;
}
double MOVE_high(double x) {
	if (x > 60 && x < 70) return (x - 60) / 10;
	else if (x >= 70) return 1;
	else return 0;
}

int main() {
	double temp, humidity, move, sum_top, sum_bottom = 0, sum0 = 0, sum40 = 0, sum70 = 0, sum100 = 0;
	cout << "현재 온도(0 ~ 40) : ";
	cin >> temp;
	cout << "현재 습도(0 ~ 100) : ";
	cin >> humidity;
	cout << "사용자의 현재 활동량(수면상태 = 0. 격력한 운동중 상태 = 100) : ";
	cin >> move;
	vector<double> temp_(3);
	vector<double> humidity_(3);
	vector<double> move_(3);
	double a, b, c, x, y, z;
	temp_[0] = TEMP_low(temp);
	temp_[1] = TEMP_mid(temp);
	temp_[2] = TEMP_high(temp);
	humidity_[0] = HUMID_low(humidity);
	humidity_[1] = HUMID_mid(humidity);
	humidity_[2] = HUMID_high(humidity);
	move_[0] = MOVE_low(move);
	move_[1] = MOVE_mid(move);
	move_[2] = MOVE_high(move);
	// 온도 습도 활동
	//  0% : 000, 001, 002, 010, 011, 012, 020, 021, 100
	// 40% : 022, 101, 102, 110
	// 70% : 111, 120, 200, 210
	//100% : 121, 122, 112, 201, 202, 211, 212, 220, 221, 222

	for (int i = 0; i < 3; i++) sum0 += min(min(temp_[0], humidity_[0]), move_[i]) + min(min(temp_[0], humidity_[1]), move_[i]);
	sum0 += min(min(temp_[0], humidity_[2]), move_[0]) + min(min(temp_[0], humidity_[2]), move_[1]) + min(min(temp_[2], humidity_[0]), move_[0]);
	sum40 += min(min(temp_[0], humidity_[2]), move_[2]) + min(min(temp_[1], humidity_[0]), move_[1]) + min(min(temp_[1], humidity_[1]), move_[2]) + min(min(temp_[1], humidity_[1]), move_[0]);
	sum70 += min(min(temp_[1], humidity_[1]), move_[1]) + min(min(temp_[1], humidity_[2]), move_[0]) + min(min(temp_[2], humidity_[0]), move_[0]) + min(min(temp_[2], humidity_[1]), move_[0]);
	for (int i = 0; i < 3; i++) sum100 += min(min(temp_[2], humidity_[i]), move_[1]) + min(min(temp_[2], humidity_[i]), move_[2]);
	sum100 += min(min(temp_[1], humidity_[2]), move_[1]) + min(min(temp_[1], humidity_[2]), move_[2]) + min(min(temp_[1], humidity_[1]), move_[2]);

	cout << "\n0% 비율 : " << sum0 << "\n40% 비율 : " << sum40 << "\n70% 비율 : " << sum70 << "\n100% 비율 : " << sum100 << "\n";
	sum_top = sum0 * 0 + sum40 * 40 + sum70 * 70 + sum100 * 100;
	sum_bottom = sum0 + sum40 + sum70 + sum100;

	cout << "\n\n\n";
	cout << "에어컨을 " << sum_top / sum_bottom << "%로 출력합니다." << endl;
}