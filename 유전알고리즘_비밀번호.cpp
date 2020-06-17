#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

#define SIZE 100
#define max_len 8
#define min_len 2// ��й�ȣ �ּ�, �ִ����
#define best_sample 20
#define lucky_sample 20
const string password = "pch8349"; // ��й�ȣ
vector<int> spell = { 48,49,50,51,52,53,54,55,56,57,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122 };
vector<string> child(SIZE); // �ڽ� ���� ������ �迭
vector<double> child_score(SIZE); // �ڽ� ������ ���� �迭
vector<string> survivechild(best_sample + lucky_sample); // ��Ƴ��� ���̵�
vector<bool> child_check(SIZE); // �ߺ��� ���� ���� check �迭

random_device rd;
mt19937_64 rng(rd()); // �ǻ� ���� ���� ������ �õ� �ʱ�ȭ�� random_device ���� �ش�.
uniform_int_distribution<_int64> dis(0, 62); // ���� ������ ����

void Qsort(int st, int end) { // child �迭�� child_score �迭�� �Ȱ��� �����Ѵ�. ���������̹Ƿ� �����ΰ� �ڿ� ������
	if (st >= end) return;
	int pivot = st, a = pivot + 1, b = end;

	while (a <= b) {
		while (a <= end && child_score[a] < child_score[pivot]) a++;
		while (b > st&& child_score[b] >= child_score[pivot]) b--;
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
	for (int i = 0; i < length; i++) {
		c = spell[dis(rng)]; // ���� �ִ� ���
		s.push_back(c);
	}
	return s;
}
string Generate_population() { // randword �Լ��� �̿��� �ڽĳ�忡 ���� �Լ� ����
	int length;
	for (int i = 0; i < SIZE; i++) {
		length = i % (max_len - min_len + 1) + min_len;
		child[i] = Randword(length);
	}
}

double Fitness(string test_word) { // ���յ� ��� �Լ�
	double score = 0;
	if (password.size() == test_word.size()) return score;
	score += 0.5;
	for (int i = 0; i < password.size(); i++) {
		if (password[i] == test_word[i]) score++;
	}
	return (score / password.size() + 0.5) * 100;
}

void Sortby_score() { // ���յ� ���� ������ ����(�����������ε�)
	for (int i = 0; i < SIZE; i++) child_score[i] = Fitness(child[i]);
	Qsort(0, SIZE - 1);
}

void Select_survivors() { // ���� ���뿡�� ��Ƴ��� ���̵� ���� 20��, 20��
	uniform_int_distribution<_int64> the(0, SIZE - best_sample - 1);
	int a;
	for (int i = 0; i < best_sample; i++) {
		survivechild[i] = child[SIZE - i - 1]; // ����� ���� 20��
		child_check[SIZE - i - 1] = true;
	}
	for (int i = best_sample; i < best_sample + lucky_sample; i++) {
		while (true) {
			a = dis(the);
			if (child_check[a] == false) {
				survivechild[i] = child[a]; // ������ ���� 20��
				child_check[a] == true;
				break;
			}
		}
	}
	random_device rd; // ���� ���������ϴ� ���
	mt19937 g(rd());
	shuffle(survivechild.begin(), survivechild.end(), g); // ��Ƴ��� ���̵� ����α�
}

void Create_Generate(int a, int b) {
	string s;
	int maxlen = max(survivechild[a].size(), survivechild[b].size());
	uniform_int_distribution<_int64> the(0, 1);
	int check = dis(the);
	for (int i = 0; i < maxlen; i++) {
		if (i <= survivechild[a].size() && i <= survivechild[b].size()) { // ���� �������� �� �ܾ ª�� ������
			if (check == 0) s.push_back(survivechild[a][i]);
			else s.push_back(survivechild[b][i]);
		}
		else { // ���� �������� �������?

		}
	}
}

int main() {

}