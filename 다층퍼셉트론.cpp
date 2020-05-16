#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
using namespace std;

#define PATTERN 5 // ��, ��, ��, ��, �� 5���� �̱⿡ 5
#define NEURON 25 // 5*5 ũ��� �迭�� ��������Ƿ� 1���� �迭 25�ٷ� ǥ��
#define HIDELAYER 3 // ������ ��
#define eta 0.3
#define offset 0
#define w_first 0.7
#define errorrange 0.000001

vector<vector<int> > p(PATTERN, vector<int>(NEURON));//���������� �迭
vector<vector<int> > signal(PATTERN, vector<int>(PATTERN, 0)); // �����ȣ ����� �迭
vector<vector<double> > hide_w(NEURON, vector<double>(HIDELAYER, w_first));//�Է� -> ���� ����ġ ������ ������ �迭
vector<vector<double> > prnt_w(HIDELAYER, vector<double>(PATTERN, w_first));//����->��� ����ġ ������ ������ �迭
vector<vector<double> > hide_w_after(NEURON, vector<double>(HIDELAYER)); // �Է�->���� ����ġ ��ȭ ��
vector<vector<double> > prnt_w_after(HIDELAYER, vector<double>(PATTERN)); // ����->��� ����ġ ��ȭ ��
vector<vector<double> > hide_w_gap(NEURON, vector<double>(HIDELAYER)); // �Է�->���� ����ġ ��, �� ��
vector<vector<double> > prnt_w_gap(HIDELAYER, vector<double>(PATTERN)); // ����->��� ����ġ ��, �� ��
vector<double> hide(HIDELAYER); // ������ ���� ������ �迭
vector<double> hide_delta(HIDELAYER); // ������ �������(��Ÿ)
vector<double> prnt(PATTERN); // ����� ���� ������ �迭
vector<double> prnt_delta(PATTERN); // ����� �������(��Ÿ)

vector<int> testcase(NEURON);
vector<double> testhide(HIDELAYER);
vector<double> testprnt(PATTERN);

double sigmoid(double x) {
	return 1 / (1 + std::exp(-x));
}

int main() {
	int epoch = 0, ans;
	double sum, m = 0;
	p[0] = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };//��
	p[1] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1 };//��
	p[2] = { 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0 };//��
	p[3] = { 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1 };//��
	p[4] = { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1 };//��

	for (int i = 0; i < PATTERN; i++) { // �����ȣ ����
		for (int j = 0; j < PATTERN; j++) {
			if (i == j) signal[i][j] = 1;
		}
	}

	while (true) {
		epoch++;
		for (int i = 0; i < PATTERN; i++) {
			for (int j = 0; j < HIDELAYER; j++) {// �������� ��°� ����
				sum = 0;
				for (int k = 0; k < NEURON; k++) sum += p[i][k] * hide_w[k][j];
				hide[j] = sigmoid(sum);
			}

			for (int j = 0; j < PATTERN; j++) {// ������� ��°� ����
				sum = 0;
				for (int k = 0; k < HIDELAYER; k++) sum += hide[k] * prnt_w[k][j];
				prnt[j] = sigmoid(sum);
			}

			// ����� �������(��Ÿ)
			for (int j = 0; j < PATTERN; j++) prnt_delta[j] = prnt[j] * (1 - prnt[j]) * (signal[i][j] - prnt[j]);

			for (int j = 0; j < HIDELAYER; j++) { // ������ �������(��Ÿ)
				sum = 0;
				for (int k = 0; k < PATTERN; k++) {
					sum += prnt_delta[k] * prnt_w[j][k];
				}
				hide_delta[j] = hide[j] * (1 - hide[j]) * sum;
			}

			for (int j = 0; j < HIDELAYER; j++) { // ����ġ ����(������->�����)
				for (int k = 0; k < PATTERN; k++) {
					prnt_w_after[j][k] = prnt_w[j][k] + eta * prnt_delta[k] * hide_delta[j];
					prnt_w_gap[j][k] = prnt_w_after[j][k] - prnt_w[j][k];
					if (prnt_w_gap[j][k] < 0) prnt_w_gap[j][k] = 0 - prnt_w_gap[j][k];
				}
			}

			for (int j = 0; j < NEURON; j++) { // ����ġ ����(�Է���->������)
				for (int k = 0; k < HIDELAYER; k++) {
					hide_w_after[j][k] = hide_w[j][k] + eta * hide_delta[k] * p[i][j];
					hide_w_gap[j][k] = hide_w_after[j][k] - hide_w[j][k];
					if (hide_w_gap[j][k] < 0) hide_w_gap[j][k] = 0 - hide_w_gap[j][k];
				}
			}
		}
		int t = 0;
		for (int i = 0; i < HIDELAYER; i++) { // �Էµ� ���� 1epoch ���� �� �������. ��� 0.00001 ���ϸ� ����
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
	cout <<"epoch : " << epoch << endl;

	for (int i = 0; i < NEURON; i++) cin >> testcase[i]; // �׽�Ʈ�� �� �Է¹ޱ�

	for (int i = 0; i < NEURON; i++) {
		if (testcase[i] == 1) cout << "��"; // 1�̸� �Ͼ�׸��
		else cout << "��"; // -1�̸� �����׸�� ���
		if (i % 5 == 4) cout << endl; // ����� ���� ���� 5��°���� �ٶ��
	}
	cout << "\n\n";
	for (int i = 0; i < HIDELAYER; i++) {// �������� ��°� ����
		sum = 0;
		for (int j = 0; j < NEURON; j++) sum += testcase[j] * hide_w_after[j][i];
		testhide[i] = sigmoid(sum);
	}

	for (int i = 0; i < PATTERN; i++) {// ������� ��°� ����
		sum = 0;
		for (int j = 0; j < HIDELAYER; j++) sum += testhide[j] * prnt_w_after[j][i];
		testprnt[i] = sigmoid(sum);
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