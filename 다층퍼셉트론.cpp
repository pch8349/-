#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
using namespace std;

#define PATTERN 8 // ㄱ, ㄴ, ㅇ, ㅈ, ㅅ, ㅁ, ㄹ, ㄷ 8가지 이기에 8
#define NEURON 25 // 5*5 크기로 배열이 만들어지므로 1차원 배열 25줄로 표현
#define HIDELAYER 4 // 은닉층 수
#define eta 0.3 // 학습률
#define w_first 0.6 // 초기 가중치값
#define bias 1.0 // bias 값
#define errorrange 0.00001 // 최소오차범위

vector<vector<double> > p(PATTERN, vector<double>(NEURON + 1));//패턴저장할 배열
vector<vector<int> > signal(PATTERN, vector<int>(PATTERN, 0)); // 교사신호 저장된 배열
//bias는 epoch가 계속 돌아도 값이 바뀌지 않고, bias의 가중치 선의 값이 바뀌는 식으로 계산한다.
vector<vector<double> > hide_w(NEURON + 1, vector<double>(HIDELAYER, w_first));//입력 -> 은닉 가중치 집합을 저장할 배열. +1을 하는 이유는 bias의 가중치 값도 저장하기 위함
vector<vector<double> > prnt_w(HIDELAYER + 1, vector<double>(PATTERN, w_first));//은닉->출력 가중치 집합을 저장할 배열
vector<vector<double> > hide_w_after(NEURON + 1, vector<double>(HIDELAYER)); // 입력->은닉 가중치 변화 후
vector<vector<double> > prnt_w_after(HIDELAYER + 1, vector<double>(PATTERN)); // 은닉->출력 가중치 변화 후
vector<vector<double> > hide_w_gap(NEURON + 1, vector<double>(HIDELAYER)); // 입력->은닉 가중치 전, 후 차
vector<vector<double> > prnt_w_gap(HIDELAYER + 1, vector<double>(PATTERN)); // 은닉->출력 가중치 전, 후 차
vector<double> hide(HIDELAYER + 1); // 은닉층 값을 저장할 배열
vector<double> hide_sum(HIDELAYER);
vector<double> prnt(PATTERN); // 출력층 값을 저장할 배열
vector<double> prnt_sum(PATTERN);
vector<double> prnt_delta(PATTERN); // 은닉층 가중치값과 출력층 가중치 값에서 공통으로 사용되는 부분 저장할 배열

vector<int> testcase(NEURON + 1);
vector<double> testhide(HIDELAYER + 1);
vector<double> testprnt(PATTERN);

double sigmoid(double x) {
	return 1 / (1 + std::exp(-x));
}

double sigmoid_differential(double x) {
	return std::exp(-x) / pow((1 - std::exp(-x)), 2);
}

void cac(double sum) {
	for (int i = 0; i < PATTERN; i++) {
		for (int j = 0; j < hide_w[j].size(); j++) {// 은닉층의 출력값 저장. y = f(가중치w X 입력노드p + bias)
			sum = 0;
			for (int k = 0; k < hide_w.size(); k++) sum += p[i][k] * hide_w[k][j];
			hide_sum[j] = sum;
			hide[j] = sigmoid(sum);
		}


		for (int j = 0; j < PATTERN; j++) {// 출력층의 출력값 저장. y = f(가중치w X 은닉노드hide + bias)
			sum = 0;
			for (int k = 0; k < prnt_w.size(); k++) sum += hide[k] * prnt_w[k][j];
			prnt_sum[j] = sum;
			prnt[j] = sigmoid(sum);
		}

		for (int j = 0; j < PATTERN; j++) { // 가중치 수정(은닉층->출력층)
			prnt_delta[j] = sigmoid_differential(prnt_sum[j]) * (prnt[j] - signal[i][j]);//은닉층 가중치 수정할 때 공통으로 사용돼서 저장.
			for (int k = 0; k < prnt_w.size(); k++) { // w_update = w - eta * (오차값 가중치로 미분한 값).  E(오차) = 1/2 * (y_target - y)^2
				prnt_w_after[k][j] = prnt_w[k][j] - eta * prnt_delta[j] * hide[k];
				prnt_w_gap[k][j] = max(prnt_w_after[k][j], prnt_w[k][j]) - min(prnt_w_after[k][j], prnt_w[k][j]);
				prnt_w[k][j] = prnt_w_after[k][j];
			}
		}

		for (int j = 0; j < HIDELAYER; j++) { // 가중치 수정(입력층->은닉층)
			sum = 0;
			for (int t = 0; t < PATTERN; t++) sum += prnt_w[j][t] * prnt_delta[t];
			sum *= sigmoid_differential(hide_sum[j]);
			for (int k = 0; k < hide_w.size(); k++) {//위와 식은 같음.
				hide_w_after[k][j] = hide_w[k][j] - eta * sum * p[i][k];
				hide_w_gap[k][j] = max(hide_w_after[k][j], hide_w[k][j]) - min(hide_w_after[k][j], hide_w[k][j]);
				hide_w[k][j] = hide_w_after[k][j];
			}
		}
	}
}

double re(double x) {
	return x > 0.0 ? x : 0.01 * x;
}
int main() {
	int epoch = 0, ans, t;
	double sum = 0, m = 0;
	hide[HIDELAYER] = bias;
	p[0] = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, bias };//ㄱ
	p[1] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, bias };//ㄴ
	p[2] = { 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, bias };//ㅇ
	p[3] = { 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, bias };//ㅈ
	p[4] = { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, bias };//ㅅ
	p[5] = { 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, bias };//ㅁ
	p[6] = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, bias };//ㄹ
	p[7] = { 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, bias };//ㄷ

	for (int i = 0; i < PATTERN; i++) for (int j = 0; j < PATTERN; j++) if (i == j) signal[i][j] = 1;// 교사신호 설정
	 
	while (true) {
		epoch++;
		printf("%d\n", epoch);
		
		cac(sum);
		t = 0;
		for (int i = 0; i < HIDELAYER; i++) { // 입력된 패턴 1epoch 수행 시 오차계산. 모두 0.00001 이하면 종료
			for (int j = 0; j < PATTERN; j++) {
				printf("%d   ", prnt_w_gap[i][j]);
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
					printf("%d    ", hide_w_gap[i][j]);
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
		for (int j = 0; j < testcase.size(); j++) sum += testcase[j] * hide_w[j][i];
		testhide[i] = sigmoid(sum);
	}

	for (int i = 0; i < PATTERN; i++) {// 은닉층 -> 출력층 확인
		sum = 0;
		for (int j = 0; j < testhide.size(); j++) sum += testhide[j] * prnt_w[j][i];
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