#include <iostream>
#include <vector>
#include <random>
using namespace std;

#define SIZE 100
#define max_len 8
#define min_len 2// 비밀번호 최소, 최대길이
#define best_sample 10
#define lucky_sample 10
const string password = "pch8349"; // 비밀번호
vector<int> spell = { 48,49,50,51,52,53,54,55,56,57,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122 };
vector<string> child(SIZE); // 자식 세대 저장할 배열
vector<double> child_score(SIZE);

random_device rd;
mt19937_64 rng(rd()); // 의사 난수 생성 엔진의 시드 초기화로 random_device 값을 준다.
uniform_int_distribution<_int64> dis(0, 62); // 글자 생성용 랜덤
uniform_int_distribution<_int64> the(0, max_len); // 부모 분할지점 정하기용 랜덤

void Qsort(int st, int end) { // child 배열과 child_score 배열을 똑같이 정렬한다. 오름차순이므로 정답인게 뒤에 가있음
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

string Randword(int length) { // 랜덤하게 단어 생성하는 함수
	string s;
	char c;
	for (int i = 0; i < length; i++) {
		c = spell[dis(rng)];
		s.push_back(c);
	}
	return s;
}
string Generate_population() { // randword 함수를 이용해 자식노드에 랜덤 함수 저장
	int length;
	for (int i = 0; i < SIZE; i++) {
		length = i % (max_len - min_len + 1) + min_len;
		child[i] = Randword(length);
	}
}

double Fitness(string test_word) { // 적합도 계산 함수
	double score = 0;
	if (password.size() == test_word.size()) return score;
	score+=0.5;
	for (int i = 0; i < password.size(); i++) {
		if (password[i] == test_word[i]) score++;
	}
	return (score / password.size() + 0.5) * 100;
}

void Sortby_score() { // 적합도 점수 순으로 정렬
	for (int i = 0; i < SIZE; i++) child_score[i] = Fitness(child[i]);
	Qsort(0, SIZE - 1);
}

void Select_survivors() {

}

int main() {

}