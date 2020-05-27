#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<cstdlib>
using namespace std;

#define nbyn 5
#define PATTERN 8 // ��, ��, ��, ��, �� 5���� �̱⿡ 5
#define NEURON 25 // 5*5 ũ��� �迭�� ��������Ƿ� 1���� �迭 25�ٷ� ǥ��
#define HIDELAYER 3 // ������ ��
#define eta 0.1
#define errorrange 0.001
#define offset 0.8

vector<vector<int> > p(PATTERN, vector<int>(NEURON));//���������� �迭
vector<vector<int> > signal(PATTERN, vector<int>(PATTERN, 0)); // �����ȣ ����� �迭

vector<vector<double> > hide_w(NEURON, vector<double>(HIDELAYER));//�Է� -> ���� ����ġ ������ ������ �迭
vector<vector<double> > hide_w_after(NEURON, vector<double>(HIDELAYER)); // �Է�->���� ����ġ ��ȭ ��
vector<vector<double> > hide_w_gap(NEURON, vector<double>(HIDELAYER)); // �Է�->���� ����ġ ��, �� ��
vector<double> hide(HIDELAYER); // ������ ���� ������ �迭
vector<double> hide_delta(HIDELAYER); // ������ �������(��Ÿ)

vector<vector<double> > prnt_w(HIDELAYER, vector<double>(PATTERN));//����->��� ����ġ ������ ������ �迭
vector<vector<double> > prnt_w_after(HIDELAYER, vector<double>(PATTERN)); // ����->��� ����ġ ��ȭ ��
vector<vector<double> > prnt_w_gap(HIDELAYER, vector<double>(PATTERN)); // ����->��� ����ġ ��, �� ��
vector<double> prnt(PATTERN); // ����� ���� ������ �迭
vector<double> prnt_delta(PATTERN); // ����� �������(��Ÿ)

vector<int> testcase(NEURON);
vector<double> testhide(HIDELAYER);
vector<double> testprnt(PATTERN);

double sigmoid(double x) {
	return 1 / (1 + exp(-x));
}

int main() {
	int epoch = 0, ans, t, check = 0;
	double sum, m = 0;
	p[0] = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };//��
	p[1] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1 };//��
	p[2] = { 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0 };//��
	p[3] = { 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1 };//��
	p[4] = { 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1 };//��
	p[5] = { 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1 };//��
	p[6] = { 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1 };//��
	p[7] = { 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1 };//��

	for (int i = 0; i < NEURON; i++) for (int j = 0; j < HIDELAYER; j++) hide_w[i][j] = ((rand() % 10) + 1) * 0.01;
	for (int i = 0; i < HIDELAYER; i++) for (int j = 0; j < PATTERN; j++) prnt_w[i][j] = ((rand() % 10) + 1) * 0.01;

	for (int i = 0; i < PATTERN; i++) signal[i][i] = 1; //�����ȣ ����

	//�н�����
	while (true) {
		epoch++;
		printf("%d\n", epoch);
		for (int n = 0; n < PATTERN; n++) {
			check = 0;
			for (int i = 0; i < HIDELAYER; i++) {// �������� ��°� ����
				sum = 0;
				for (int j = 0; j < NEURON; j++) sum += p[n][j] * hide_w[j][i];
				hide[i] = sigmoid(sum);
			}

			for (int i = 0; i < PATTERN; i++) {// ������� ��°� ����
				sum = 0;
				for (int j = 0; j < HIDELAYER; j++) sum += hide[j] * prnt_w[j][i];
				prnt[i] = sigmoid(sum);
			}

			// ����� �������(��Ÿ)
			for (int i = 0; i < PATTERN; i++) prnt_delta[i] = prnt[i] * (1 - prnt[i]) * (signal[n][i] - prnt[i]);

			for (int i = 0; i < HIDELAYER; i++) { // ������ �������(��Ÿ)
				sum = 0;
				for (int j = 0; j < PATTERN; j++) sum += prnt_delta[j] * prnt_w[i][j];
				hide_delta[i] = hide[i] * (1 - hide[i]) * sum;
			}

			for (int i = 0; i < HIDELAYER; i++) { // ������. ����ġ ����(������->�����)
				for (int j = 0; j < PATTERN; j++) {
					prnt_w_after[i][j] = prnt_w[i][j] + eta * prnt_delta[j] * hide[i];//����
					prnt_w_gap[i][j] = abs(prnt_w_after[i][j] - prnt_w[i][j]);//����
					prnt_w[i][j] = prnt_w_after[i][j];//����
					if (prnt_w_gap[i][j] > errorrange) check++;
				}
			}

			for (int i = 0; i < NEURON; i++) { // ����ġ ����(�Է���->������)
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
		for (int i = 0; i < NEURON; i++) cin >> testcase[i]; // �׽�Ʈ�� �� �Է¹ޱ�

		for (int i = 0; i < NEURON; i++) {
			if (testcase[i] == 1) cout << "��"; // 1�̸� �Ͼ�׸��
			else cout << "��"; // -1�̸� �����׸�� ���
			if (i % nbyn == nbyn-1) cout << endl; // ����� ���� ���� 5��°���� �ٶ��
		}
		cout << "\n\n";
		for (int i = 0; i < HIDELAYER; i++) {// �Է¹��� �� �Է���->������ Ȯ��
			sum = 0;
			for (int j = 0; j < NEURON; j++) sum += testcase[j] * hide_w[j][i];
			testhide[i] = sigmoid(sum);
		}
		m = 0;
		for (int i = 0; i < PATTERN; i++) {// ������ -> ����� Ȯ��
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
			if (p[ans][i] == 1) cout << "��"; // 1�̸� �Ͼ�׸��
			else cout << "��"; // -1�̸� �����׸�� ���
			if (i % nbyn == nbyn-1) cout << endl; // ����� ���� ���� 5��°���� �ٶ��
		}
	}
}