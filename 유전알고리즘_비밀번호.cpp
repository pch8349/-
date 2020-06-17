#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

#define SIZE 100
#define max_len 8
#define min_len 2// 비밀번호 최소, 최대길이
#define best_sample 20
#define lucky_sample 20
const string password = "pch8349"; // 비밀번호
vector<int> spell = { 48,49,50,51,52,53,54,55,56,57,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122 };
vector<string> child(SIZE); // 자식 세대 저장할 배열
vector<double> child_score(SIZE); // 자식 세대의 점수 배열
vector<string> survivechild(best_sample + lucky_sample); // 살아남은 아이들
vector<bool> child_check(SIZE); // 중복을 막기 위한 check 배열

random_device rd;
mt19937_64 rng(rd()); // 의사 난수 생성 엔진의 시드 초기화로 random_device 값을 준다.
uniform_int_distribution<_int64> dis(0, 62); // 글자 생성용 랜덤

void Qsort(int st, int end) { // child 배열과 child_score 배열을 똑같이 정렬한다. 오름차순이므로 정답인게 뒤에 가있음
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

string Randword(int length) { // 랜덤하게 단어 생성하는 함수
	string s;
	char c;
	for (int i = 0; i < length; i++) {
		c = spell[dis(rng)]; // 랜덤 넣는 방법
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
	score += 0.5;
	for (int i = 0; i < password.size(); i++) {
		if (password[i] == test_word[i]) score++;
	}
	return (score / password.size() + 0.5) * 100;
}

void Sortby_score() { // 적합도 점수 순으로 정렬(오름차순으로됨)
	for (int i = 0; i < SIZE; i++) child_score[i] = Fitness(child[i]);
	Qsort(0, SIZE - 1);
}

void Select_survivors() { // 이전 세대에서 살아남을 아이들 설정 20개, 20개
	uniform_int_distribution<_int64> the(0, SIZE - best_sample - 1);
	int a;
	for (int i = 0; i < best_sample; i++) {
		survivechild[i] = child[SIZE - i - 1]; // 우수한 세대 20개
		child_check[SIZE - i - 1] = true;
	}
	for (int i = best_sample; i < best_sample + lucky_sample; i++) {
		while (true) {
			a = dis(the);
			if (child_check[a] == false) {
				survivechild[i] = child[a]; // 랜덤한 세대 20개
				child_check[a] == true;
				break;
			}
		}
	}
	random_device rd; // 벡터 랜덤셔플하는 방법
	mt19937 g(rd());
	shuffle(survivechild.begin(), survivechild.end(), g); // 살아남은 아이들 섞어두기
}

void Create_Generate(int a, int b) {
	string s;
	int maxlen = max(survivechild[a].size(), survivechild[b].size());
	uniform_int_distribution<_int64> the(0, 1);
	int check = dis(the);
	for (int i = 0; i < maxlen; i++) {
		if (i <= survivechild[a].size() && i <= survivechild[b].size()) { // 지정 범위보다 두 단어가 짧지 않을때
			if (check == 0) s.push_back(survivechild[a][i]);
			else s.push_back(survivechild[b][i]);
		}
		else { // 지정 범위보다 길어지면?

		}
	}
}

int main() {

}