#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <random>
using namespace std;

const string password = "UPe29Q4"; // ��й�ȣ

#define SIZE 100
#define max_len 8
#define min_len 2// ��й�ȣ �ּ�, �ִ����
#define best_sample 20
#define lucky_sample 20
#define child_birth 4
#define MaxMutation 3 // �� ���뿡 �� �������� �ִ����

vector<int> spell = { 48,49,50,51,52,53,54,55,56,57,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122 };
vector<string> child(SIZE); // �ڽ� ���� ������ �迭
vector<double> child_score(SIZE); // �ڽ� ������ ���� �迭
vector<string> survivechild(best_sample + lucky_sample); // ��Ƴ��� ���̵�
vector<bool> child_check(SIZE); // �ߺ��� ���� ���� check �迭

random_device rd;

void Qsort(int st, int end) { // child �迭�� child_score �迭�� �Ȱ��� �����Ѵ�. ���������̹Ƿ� �����ΰ� �ڿ� ������
	if (st >= end) return;
	int pivot = st, a = pivot + 1, b = end;

	while (a <= b) {
		while (a <= end && child_score[a] < child_score[pivot]) a++;
		while (b > st && child_score[b] >= child_score[pivot]) b--;
		if (a > b) {
			swap(child_score[b], child_score[pivot]);
			swap(child[b], child[pivot]);
		}
		else {
			swap(child_score[a], child_score[b]);
			swap(child[a], child[b]);
		}
	}
	Qsort(st, b - 1);
	Qsort(b + 1, end);
}

string Randword(int length) { // �����ϰ� �ܾ� �����ϴ� �Լ�
	string s;
	char c;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, 61);
	for (int i = 0; i < length; i++) {
		c = spell[dis(gen)]; // ���� �ִ� ���
		s.push_back(c);
	}
	return s;
}
void Generate_population() { // Randword �Լ��� �̿��� �ڽĳ�忡 ���� �Լ� ����
	int length;
	for (int i = 0; i < SIZE; i++) {
		length = i % (max_len - min_len + 1) + min_len;
		child[i] = Randword(length);
	}
}

double Fitness(string test_word) { // ���յ� ��� �Լ�
	double score = 0;
	if (password.size() != test_word.size()) return score;
	score += 0.5;
	for (int i = 0; i < password.size(); i++) {
		if (password[i] == test_word[i]) score++;
	}
	return (score / (password.size() + 0.5)) * 100;
}

int Sortby_score() { // ���յ� ���� ������ ����(�����������ε�)
	for (int i = 0; i < SIZE; i++) child_score[i] = Fitness(child[i]);
	Qsort(0, SIZE - 1);
	if (child_score[SIZE - 1] == 100) return 1;
	else return 0;
}

void Select_survivors() { // ���� ���뿡�� ��Ƴ��� ���̵� ���� 20��, 20��
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, best_sample + lucky_sample - 1);
	int a;
	child_check.clear();
	child_check.resize(SIZE);
	for (int i = 0; i < best_sample; i++) {
		survivechild[i] = child[SIZE - i - 1]; // ����� ���� 20��
		child_check[SIZE - i - 1] = true; // �ߺ��� ������ ���� ���� ���õƴ� ����� ǥ���صд�.
	}
	for (int i = best_sample; i < best_sample + lucky_sample; i++) {
		while (true) {
			a = dis(gen);
			if (child_check[a] == false) {
				survivechild[i] = child[a]; // ������ ���� 20��
				child_check[a] = true;
				break;
			}
		}
	}
	for (int i = 0; i < best_sample; i++) child[i] = survivechild[i]; // ���� �� 20�� �״�� ����
	random_device rda; // ���� ���������ϴ� ���
	mt19937 g(rda());
	shuffle(survivechild.begin(), survivechild.end(), g); // ��Ƴ��� ���̵� ����α�
}

string Sex_and_baby(int a, int b) { // �θ𳢸� ���� �Լ�
	string s;
	int maxlen = max(survivechild[a].size(), survivechild[b].size());
	int check;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, 1);
	for (int i = 0; i < maxlen; i++) {
		check = dis(gen);
		if (i < survivechild[a].size() && i < survivechild[b].size()) { // ���� �������� �� �ܾ ª�� ������
			if (check == 0) s.push_back(survivechild[a][i]); // �׳� �� �θ� �� �������� ���� �־��ش�.
			else s.push_back(survivechild[b][i]);
		}
		else if (survivechild[a].size() <= i) s.push_back(survivechild[b][i]); // ���� �������� �θ� ���̰� ª���� ������?
		else if (survivechild[b].size() <= i) s.push_back(survivechild[a][i]); // ������ ��ַ� ä������
	}
	return s;
}

void Create_child() { // �� �κθ��� ���� �ڽ��� ���� ���ΰ�.
	int t = best_sample;
	for (int i = 0; i < best_sample; i++) {
		for (int j = 0; j < child_birth; j++) {
			child[t++] = Sex_and_baby(i, i + best_sample);
		}
	}
}

void Mutation() { // �������� ���� �Լ�
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, 99);
	for (int i = 0; i < SIZE / 10; i++) {
		int a = dis(gen);
		int t = child[a].size();
		for (int j = 0; j < dis(gen) % 3 + 1; j++) {
			child[a][dis(gen) % t] = spell[dis(gen) % 62];
		}
	}
}

int main() {
	int epoch = 1;
	int a;
	Generate_population();
	while (true) {
		printf("epoch : %d\n", epoch);
		epoch++;
		a = Sortby_score();
		if (a == 1) break;
		for (int i = 0; i < child[SIZE - 1].size(); i++) printf("%c", child[SIZE - 1][i]);
		printf("\n score : %f", child_score[SIZE - 1]);
		printf("\n");
		Select_survivors();
		Create_child();
		Mutation();
	}
	cout << "��й�ȣ : ";
	for (int i = 0; i < child[SIZE - 1].size(); i++) cout << child[SIZE - 1][i];
	cout << endl;
}