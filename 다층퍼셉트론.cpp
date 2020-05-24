#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
using namespace std;

#define PATTERN 8 // ��, ��, ��, ��, ��, ��, ��, �� 8���� �̱⿡ 8
#define NEURON 25 // 5*5 ũ��� �迭�� ��������Ƿ� 1���� �迭 25�ٷ� ǥ��
#define HIDELAYER 4 // ������ ��
#define eta 0.3 // �н���
#define w_first 0.6 // �ʱ� ����ġ��
#define bias 1.0 // bias ��
#define errorrange 0.00001 // �ּҿ�������

vector<vector<double> > p(PATTERN, vector<double>(NEURON + 1));//���������� �迭
vector<vector<int> > signal(PATTERN, vector<int>(PATTERN, 0)); // �����ȣ ����� �迭
//bias�� epoch�� ��� ���Ƶ� ���� �ٲ��� �ʰ�, bias�� ����ġ ���� ���� �ٲ�� ������ ����Ѵ�.
vector<vector<double> > hide_w(NEURON + 1, vector<double>(HIDELAYER, w_first));//�Է� -> ���� ����ġ ������ ������ �迭. +1�� �ϴ� ������ bias�� ����ġ ���� �����ϱ� ����
vector<vector<double> > prnt_w(HIDELAYER + 1, vector<double>(PATTERN, w_first));//����->��� ����ġ ������ ������ �迭
vector<vector<double> > hide_w_after(NEURON + 1, vector<double>(HIDELAYER)); // �Է�->���� ����ġ ��ȭ ��
vector<vector<double> > prnt_w_after(HIDELAYER + 1, vector<double>(PATTERN)); // ����->��� ����ġ ��ȭ ��
vector<vector<double> > hide_w_gap(NEURON + 1, vector<double>(HIDELAYER)); // �Է�->���� ����ġ ��, �� ��
vector<vector<double> > prnt_w_gap(HIDELAYER + 1, vector<double>(PATTERN)); // ����->��� ����ġ ��, �� ��
vector<double> hide(HIDELAYER + 1); // ������ ���� ������ �迭
vector<double> hide_sum(HIDELAYER);
vector<double> prnt(PATTERN); // ����� ���� ������ �迭
vector<double> prnt_sum(PATTERN);
vector<double> prnt_delta(PATTERN); // ������ ����ġ���� ����� ����ġ ������ �������� ���Ǵ� �κ� ������ �迭

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
		for (int j = 0; j < hide_w[j].size(); j++) {// �������� ��°� ����. y = f(����ġw X �Է³��p + bias)
			sum = 0;
			for (int k = 0; k < hide_w.size(); k++) sum += p[i][k] * hide_w[k][j];
			hide_sum[j] = sum;
			hide[j] = sigmoid(sum);
		}


		for (int j = 0; j < PATTERN; j++) {// ������� ��°� ����. y = f(����ġw X ���г��hide + bias)
			sum = 0;
			for (int k = 0; k < prnt_w.size(); k++) sum += hide[k] * prnt_w[k][j];
			prnt_sum[j] = sum;
			prnt[j] = sigmoid(sum);
		}

		for (int j = 0; j < PATTERN; j++) { // ����ġ ����(������->�����)
			prnt_delta[j] = sigmoid_differential(prnt_sum[j]) * (prnt[j] - signal[i][j]);//������ ����ġ ������ �� �������� ���ż� ����.
			for (int k = 0; k < prnt_w.size(); k++) { // w_update = w - eta * (������ ����ġ�� �̺��� ��).  E(����) = 1/2 * (y_target - y)^2
				prnt_w_after[k][j] = prnt_w[k][j] - eta * prnt_delta[j] * hide[k];
				prnt_w_gap[k][j] = max(prnt_w_after[k][j], prnt_w[k][j]) - min(prnt_w_after[k][j], prnt_w[k][j]);
				prnt_w[k][j] = prnt_w_after[k][j];
			}
		}

		for (int j = 0; j < HIDELAYER; j++) { // ����ġ ����(�Է���->������)
			sum = 0;
			for (int t = 0; t < PATTERN; t++) sum += prnt_w[j][t] * prnt_delta[t];
			sum *= sigmoid_differential(hide_sum[j]);
			for (int k = 0; k < hide_w.size(); k++) {//���� ���� ����.
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
	p[0] = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, bias };//��
	p[1] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, bias };//��
	p[2] = { 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, bias };//��
	p[3] = { 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, bias };//��
	p[4] = { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, bias };//��
	p[5] = { 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, bias };//��
	p[6] = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, bias };//��
	p[7] = { 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, bias };//��

	for (int i = 0; i < PATTERN; i++) for (int j = 0; j < PATTERN; j++) if (i == j) signal[i][j] = 1;// �����ȣ ����
	 
	while (true) {
		epoch++;
		printf("%d\n", epoch);
		
		cac(sum);
		t = 0;
		for (int i = 0; i < HIDELAYER; i++) { // �Էµ� ���� 1epoch ���� �� �������. ��� 0.00001 ���ϸ� ����
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

	for (int i = 0; i < NEURON; i++) cin >> testcase[i]; // �׽�Ʈ�� �� �Է¹ޱ�

	for (int i = 0; i < NEURON; i++) {
		if (testcase[i] == 1) cout << "��"; // 1�̸� �Ͼ�׸��
		else cout << "��"; // -1�̸� �����׸�� ���
		if (i % 5 == 4) cout << endl; // ����� ���� ���� 5��°���� �ٶ��
	}
	cout << "\n\n";
	for (int i = 0; i < HIDELAYER; i++) {// �Է¹��� �� �Է���->������ Ȯ��
		sum = 0;
		for (int j = 0; j < testcase.size(); j++) sum += testcase[j] * hide_w[j][i];
		testhide[i] = sigmoid(sum);
	}

	for (int i = 0; i < PATTERN; i++) {// ������ -> ����� Ȯ��
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
		if (p[ans][i] == 1) cout << "��"; // 1�̸� �Ͼ�׸��
		else cout << "��"; // -1�̸� �����׸�� ���
		if (i % 5 == 4) cout << endl; // ����� ���� ���� 5��°���� �ٶ��
	}
}