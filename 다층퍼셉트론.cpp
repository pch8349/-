#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<cstdlib>
using namespace std;

#define nbyn 5
#define PATTERN 8 // ㄱ, ㄴ, ㅇ, ㅈ, ㅅ 5가지 이기에 5
#define NEURON 25 // 5*5 크기로 배열이 만들어지므로 1차원 배열 25줄로 표현
#define HIDELAYER 3 // 은닉층 수
#define eta 0.1
#define errorrange 0.001
#define offset 0.8

vector<vector<int> > p(PATTERN, vector<int>(NEURON));//패턴저장할 배열
vector<vector<int> > signal(PATTERN, vector<int>(PATTERN, 0)); // 교사신호 저장된 배열

vector<vector<double> > hide_w(NEURON, vector<double>(HIDELAYER));//입력 -> 은닉 가중치 집합을 저장할 배열
vector<vector<double> > hide_w_after(NEURON, vector<double>(HIDELAYER)); // 입력->은닉 가중치 변화 후
vector<vector<double> > hide_w_gap(NEURON, vector<double>(HIDELAYER)); // 입력->은닉 가중치 전, 후 차
vector<double> hide(HIDELAYER); // 은닉층 값을 저장할 배열
vector<double> hide_delta(HIDELAYER); // 은닉층 오차계산(델타)

vector<vector<double> > prnt_w(HIDELAYER, vector<double>(PATTERN));//은닉->출력 가중치 집합을 저장할 배열
vector<vector<double> > prnt_w_after(HIDELAYER, vector<double>(PATTERN)); // 은닉->출력 가중치 변화 후
vector<vector<double> > prnt_w_gap(HIDELAYER, vector<double>(PATTERN)); // 은닉->출력 가중치 전, 후 차
vector<double> prnt(PATTERN); // 출력층 값을 저장할 배열
vector<double> prnt_delta(PATTERN); // 출력층 오차계산(델타)

vector<int> testcase(NEURON);
vector<double> testhide(HIDELAYER);
vector<double> testprnt(PATTERN);

double sigmoid(double x) {
	return 1 / (1 + exp(-x));
}

int main() {
	int epoch = 0, ans, t, check = 0;
	double sum, m = 0;
	p[0] = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };//ㄱ
	p[1] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1 };//ㄴ
	p[2] = { 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0 };//ㅇ
	p[3] = { 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1 };//ㅈ
	p[4] = { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1 };//ㅅ
	p[5] = { 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1 };//ㅁ
	p[6] = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1 };//ㄹ
	p[7] = { 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1 };//ㄷ

	for (int i = 0; i < NEURON; i++) for (int j = 0; j < HIDELAYER; j++) hide_w[i][j] = ((rand() % 10) + 1) * 0.01;
	for (int i = 0; i < HIDELAYER; i++) for (int j = 0; j < PATTERN; j++) prnt_w[i][j] = ((rand() % 10) + 1) * 0.01;

	for (int i = 0; i < PATTERN; i++) signal[i][i] = 1; //교사신호 설정

	//학습시작
	while (true) {
		epoch++;
		printf("%d\n", epoch);
		for (int n = 0; n < PATTERN; n++) {
			check = 0;
			for (int i = 0; i < HIDELAYER; i++) {// 은닉층의 출력값 저장
				sum = 0;
				for (int j = 0; j < NEURON; j++) sum += p[n][j] * hide_w[j][i];
				hide[i] = sigmoid(sum);
			}

			for (int i = 0; i < PATTERN; i++) {// 출력층의 출력값 저장
				sum = 0;
				for (int j = 0; j < HIDELAYER; j++) sum += hide[j] * prnt_w[j][i];
				prnt[i] = sigmoid(sum);
			}

			// 출력층 오차계산(델타)
			for (int i = 0; i < PATTERN; i++) prnt_delta[i] = prnt[i] * (1 - prnt[i]) * (signal[n][i] - prnt[i]);

			for (int i = 0; i < HIDELAYER; i++) { // 은닉층 오차계산(델타)
				sum = 0;
				for (int j = 0; j < PATTERN; j++) sum += prnt_delta[j] * prnt_w[i][j];
				hide_delta[i] = hide[i] * (1 - hide[i]) * sum;
			}

			for (int i = 0; i < HIDELAYER; i++) { // 역전파. 가중치 수정(은닉층->출력층)
				for (int j = 0; j < PATTERN; j++) {
					prnt_w_after[i][j] = prnt_w[i][j] + eta * prnt_delta[j] * hide[i];//수정
					prnt_w_gap[i][j] = abs(prnt_w_after[i][j] - prnt_w[i][j]);//차이
					prnt_w[i][j] = prnt_w_after[i][j];//갱신
					if (prnt_w_gap[i][j] > errorrange) check++;
				}
			}

			for (int i = 0; i < NEURON; i++) { // 가중치 수정(입력층->은닉층)
				for (int j = 0; j < HIDELAYER; j++) {
					hide_w_after[i][j] = hide_w[i][j] + eta * hide_delta[j] * p[n][i];
					hide_w_gap[i][j] = abs(hide_w_after[i][j] - hide_w[i][j]);
					hide_w[i][j] = hide_w_after[i][j];
					if (hide_w_gap[i][j] > errorrange) check++;
				}
			}
			printf("check = %d\n", check);
			if (check < 2) break;
		}
		if (check < 2) break;
	}
	cout << "epoch : " << epoch << endl;
	int ask;
	while (true) {
		cin >> ask;
		if (ask == 0) break;
		for (int i = 0; i < NEURON; i++) cin >> testcase[i]; // 테스트할 값 입력받기

		for (int i = 0; i < NEURON; i++) {
			if (testcase[i] == 1) cout << "■"; // 1이면 하얀네모로
			else cout << "□"; // -1이면 검은네모로 출력
			if (i % nbyn == nbyn-1) cout << endl; // 모양을 보기 위해 5번째마다 줄띄움
		}
		cout << "\n\n";
		for (int i = 0; i < HIDELAYER; i++) {// 입력받은 값 입력층->은닉층 확인
			sum = 0;
			for (int j = 0; j < NEURON; j++) sum += testcase[j] * hide_w[j][i];
			testhide[i] = sigmoid(sum);
		}
		m = 0;
		for (int i = 0; i < PATTERN; i++) {// 은닉층 -> 출력층 확인
			sum = 0;
			for (int j = 0; j < HIDELAYER; j++) sum += testhide[j] * prnt_w[j][i];
			testprnt[i] = sigmoid(sum);
			cout << testprnt[i] << endl;
			if (m < testprnt[i]) {
				m = testprnt[i];
				ans = i;
			}
		}

		for (int i = 0; i < NEURON; i++) {
			if (p[ans][i] == 1) cout << "■"; // 1이면 하얀네모로
			else cout << "□"; // -1이면 검은네모로 출력
			if (i % nbyn == nbyn-1) cout << endl; // 모양을 보기 위해 5번째마다 줄띄움
		}
	}
}