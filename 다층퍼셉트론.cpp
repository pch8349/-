#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
using namespace std;

#define PATTERN 5 // ㄱ, ㄴ, ㅇ, ㅈ, ㅅ 5가지 이기에 5
#define NEURON 25 // 5*5 크기로 배열이 만들어지므로 1차원 배열 25줄로 표현
#define HIDELAYER 3 // 은닉층 수
#define eta 0.3
#define w_first 0.6
#define errorrange 0.00001

vector<vector<int> > p(PATTERN, vector<int>(NEURON));//패턴저장할 배열
vector<vector<int> > signal(PATTERN, vector<int>(PATTERN, 0)); // 교사신호 저장된 배열
vector<vector<double> > hide_w(NEURON, vector<double>(HIDELAYER, w_first));//입력 -> 은닉 가중치 집합을 저장할 배열
vector<vector<double> > prnt_w(HIDELAYER, vector<double>(PATTERN, w_first));//은닉->출력 가중치 집합을 저장할 배열
vector<vector<double> > hide_w_after(NEURON, vector<double>(HIDELAYER)); // 입력->은닉 가중치 변화 후
vector<vector<double> > prnt_w_after(HIDELAYER, vector<double>(PATTERN)); // 은닉->출력 가중치 변화 후
vector<vector<double> > hide_w_gap(NEURON, vector<double>(HIDELAYER)); // 입력->은닉 가중치 전, 후 차
vector<vector<double> > prnt_w_gap(HIDELAYER, vector<double>(PATTERN)); // 은닉->출력 가중치 전, 후 차
vector<double> hide(HIDELAYER); // 은닉층 값을 저장할 배열
vector<double> hide_delta(HIDELAYER); // 은닉층 오차계산(델타)
vector<double> prnt(PATTERN); // 출력층 값을 저장할 배열
vector<double> prnt_delta(PATTERN); // 출력층 오차계산(델타)

vector<int> testcase(NEURON);
vector<double> testhide(HIDELAYER);
vector<double> testprnt(PATTERN);

double sigmoid(double x) {
	return 1 / (1 + std::exp(-x));
}

int main() {
	int epoch = 0, ans, t;
	double sum, m = 0;
	p[0] = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };//ㄱ
	p[1] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1 };//ㄴ
	p[2] = { 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0 };//ㅇ
	p[3] = { 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1 };//ㅈ
	p[4] = { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1 };//ㅅ

	for (int i = 0; i < PATTERN; i++) { // 교사신호 설정
		for (int j = 0; j < PATTERN; j++) if (i == j) signal[i][j] = 1;
	}

	while (true) {
		epoch++;
		for (int i = 0; i < PATTERN; i++) {
			for (int j = 0; j < HIDELAYER; j++) {// 은닉층의 출력값 저장
				sum = 0;
				for (int k = 0; k < NEURON; k++) sum += p[i][k] * hide_w[k][j];
				hide[j] = sigmoid(sum);
			}

			for (int j = 0; j < PATTERN; j++) {// 출력층의 출력값 저장
				sum = 0;
				for (int k = 0; k < HIDELAYER; k++) sum += hide[k] * prnt_w[k][j];
				prnt[j] = sigmoid(sum);
			}

			// 출력층 오차계산(델타)
			for (int j = 0; j < PATTERN; j++) prnt_delta[j] = prnt[j] * (1 - prnt[j]) * (signal[i][j] - prnt[j]);

			for (int j = 0; j < HIDELAYER; j++) { // 은닉층 오차계산(델타)
				sum = 0;
				for (int k = 0; k < PATTERN; k++) sum += prnt_delta[k] * prnt_w[j][k];
				hide_delta[j] = hide[j] * (1 - hide[j]) * sum;
			}

			for (int j = 0; j < HIDELAYER; j++) { // 가중치 수정(은닉층->출력층)
				for (int k = 0; k < PATTERN; k++) {
					prnt_w_after[j][k] = prnt_w[j][k] + eta * prnt_delta[k] * hide_delta[j];
					prnt_w_gap[j][k] = abs(prnt_w_after[j][k] - prnt_w[j][k]);
					prnt_w[j][k] = prnt_w_after[j][k];
				}
			}

			for (int j = 0; j < NEURON; j++) { // 가중치 수정(입력층->은닉층)
				for (int k = 0; k < HIDELAYER; k++) {
					hide_w_after[j][k] = hide_w[j][k] + eta * hide_delta[k] * p[i][j];
					hide_w_gap[j][k] = abs(hide_w_after[j][k] - hide_w[j][k]);
					hide_w[j][k] = hide_w_after[j][k];
				}
			}
		}
		t = 0;
		for (int i = 0; i < HIDELAYER; i++) { // 입력된 패턴 1epoch 수행 시 오차계산. 모두 0.00001 이하면 종료
			for (int j = 0; j < PATTERN; j++) {
				if (prnt_w_gap[i][j] < errorrange)
					continue;
				else {
					t = 1;
					break;
				}
			}
			if (t == 1) break;
		}
		if (t == 0) {
			for (int i = 0; i < NEURON; i++) {
				for (int j = 0; j < HIDELAYER; j++) {
					if (hide_w_gap[i][j] < errorrange)
						continue;
					else {
						t = 1;
						break;
					}
				}
				if (t == 1)break;
			}
		}
		if (t == 0) break;
	}
	cout << "epoch : " << epoch << endl;

	for (int i = 0; i < NEURON; i++) cin >> testcase[i]; // 테스트할 값 입력받기

	for (int i = 0; i < NEURON; i++) {
		if (testcase[i] == 1) cout << "■"; // 1이면 하얀네모로
		else cout << "□"; // -1이면 검은네모로 출력
		if (i % 5 == 4) cout << endl; // 모양을 보기 위해 5번째마다 줄띄움
	}
	cout << "\n\n";
	for (int i = 0; i < HIDELAYER; i++) {// 입력받은 값 입력층->은닉층 확인
		sum = 0;
		for (int j = 0; j < NEURON; j++) sum += testcase[j] * hide_w[j][i];
		testhide[i] = sigmoid(sum);
	}

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
		if (i % 5 == 4) cout << endl; // 모양을 보기 위해 5번째마다 줄띄움
	}
}