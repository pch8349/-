#include <stdio.h>
double MAX(double a, double b, double c) {
	if (a >= b && a >= c)
		return a;
	else if (b >= c && b >= a)
		return b;
	else if (c >= a && c >= b)
		return c;
	else
		return 0;
}
int main() {
	double n[6][2001] = { 0, };
	double sum, sum2, result;
	int j;
	int input;
	printf("������ �Է��� �ּ���.\nNULL:1, ZERO:2, SMALL:3, MEDIUM:4, LARGE : 5, VERY LARGE : 6\n�Է� : ");
	scanf_s("%d", &input);
	//RULE #1 ��к�
	for (j = 400; j <= 2000; j += 200)
		if (j >= 1600)
			n[0][j] = (double)j / 400 - 4;
		else
			n[0][j] = 0;
	//RULE #2 ��к�
	for (j = 400; j <= 2000; j += 200)
		if (j >= 1200 && j <= 1600)
			n[1][j] = (double)j / 400 - 3;
		else if (j > 1600 && j <= 2000)
			n[1][j] = (double)j / (-400) + 5;
		else
			n[1][j] = 0;
	//RULE #3 ��к�
	for (j = 400; j <= 2000; j += 200)
		if (j >= 800 && j <= 1200)
			n[2][j] = (double)j / 400 - 2;
		else if (j > 1200 && j <= 1600)
			n[2][j] = (double)j / (-400) + 4;
		else
			n[2][j] = 0;
	//RULE #4 ��к�
	for (j = 400; j <= 2000; j += 200)
		if (j >= 400 && j <= 800)
			n[3][j] = (double)j / 400 - 1;
		else if (j > 800 && j <= 1200)
			n[3][j] = (double)j / (-400) + 3;
		else
			n[3][j] = 0;
	//RULE #5 ��к�
	for (j = 400; j <= 2000; j += 200)
		if (j <= 800)
			n[4][j] = (double)j / (-400) + 2;
		else
			n[4][j] = 0;
	//RULE #6 ��к�
	for (j = 400; j <= 2000; j += 200)
		if (j <= 800)
			n[5][j] = (double)j / (-400) + 2;
		else
			n[5][j] = 0;
	sum = 0;
	sum2 = 0;
	// printf("%lf\n",MAX(n[0][400], n[1][400], 0.00));
	// printf("n[0][1600]->%lf\nn[1][1600]->%lf\n",n[0][1600], n[1][1600]);
	// printf("n[3][600]=%lf", MAX(n[3][600], n[4][600], n[5][600]));
	if (input == 1)
		for (j = 400; j <= 2000; j += 200) {
			if (n[1][j] > 0.5)
				n[1][j] = 0.5; //��ġ��(����)�������� �ڸ���
			sum = sum + MAX(n[0][j], n[1][j], 0.00); //�и� ���
			sum2 = sum2 + j * MAX(n[0][j], n[1][j], 0.00); //���ڰ� ���
			printf("\n%d -> %lf %lf\n", j, MAX(n[0][j], n[1][j], 0.00), j * MAX(n[0][j], n[1][j], 0.00));
		}
	else if (input == 2)
		for (j = 400; j <= 2000; j += 200) {
			if (n[0][j] > 0.5)
				n[0][j] = 0.5;
			if (n[2][j] > 0.5)
				n[2][j] = 0.5;
			sum = sum + MAX(n[0][j], n[1][j], n[2][j]);
			sum2 = sum2 + (double)j * MAX(n[0][j], n[1][j], n[2][j]);
			printf("\n%d -> %lf %lf\n", j, MAX(n[0][j], n[1][j], n[2][j]), j * MAX(n[0][j], n[1][j], n[2][j]));
		}
	else if (input == 3)
		for (j = 400; j <= 2000; j += 200) {
			if (n[1][j] > 0.5)
				n[1][j] = 0.5;
			if (n[3][j] > 0.5)
				n[3][j] = 0.5;
			sum = sum + MAX(n[1][j], n[2][j], n[3][j]);
			sum2 = sum2 + (double)j * MAX(n[1][j], n[2][j], n[3][j]);
			printf("\n%d -> %lf %lf\n", j, MAX(n[1][j], n[2][j], n[3][j]), j * MAX(n[1][j], n[2][j], n[3][j]));
		}
	else if (input == 4)
		for (j = 400; j <= 2000; j += 200) {
			if (n[2][j] > 0.5)
				n[2][j] = 0.5;
			if (n[4][j] > 0.5)
				n[4][j] = 0.5;
			sum = sum + MAX(n[2][j], n[3][j], n[4][j]);
			sum2 = sum2 + (double)j * MAX(n[2][j], n[3][j], n[4][j]);
			printf("\n%d -> %lf %lf\n", j, MAX(n[2][j], n[3][j], n[4][j]), j * MAX(n[2][j], n[3][j], n[4][j]));
		}
	else if (input == 5)
		for (j = 400; j <= 2000; j += 200) {
			if (n[3][j] > 0.5)
				n[3][j] = 0.5;
			sum = sum + MAX(n[3][j], n[4][j], n[5][j]);
			sum2 = sum2 + (double)j * MAX(n[3][j], n[4][j], n[5][j]);
			printf("\n%d -> %lf %lf\n", j, MAX(n[3][j], n[4][j], n[5][j]),
				j * MAX(n[3][j], n[4][j], n[5][j]));
		}
	else if (input == 6)
		for (j = 400; j <= 2000; j += 200) {
			sum = sum + MAX(n[4][j], n[5][j], 0);
			sum2 = sum2 + (double)j * MAX(n[4][j], n[5][j], 0);
			printf("\n%d -> %lf %lf\n", j, MAX(0, n[4][j], n[5][j]),
				j * MAX(0, n[4][j], n[5][j]));
		}
	result = sum2 / sum; //�����߽ɹ�
	printf("\n������ ���� �Է��� ");
	if (input == 1)
		printf("NULL");
	else if (input == 2)
		printf("ZERO");
	else if (input == 3)
		printf("SMALL");
	else if (input == 4)
		printf("MEDIUM");
	else if (input == 5)
		printf("LARGE");
	else
		printf("VERY LARGE");
	printf(" �ϰ�� ȸ���ӵ��� %lf �Դϴ�.\n", result);
	return 0;
}