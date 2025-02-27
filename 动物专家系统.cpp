#include<stdio.h>
#include<stdlib.h>
#pragma warning (disable:4996)

#define NUM 32

int expr[][2][4] = { {{1, -1, -1, -1}, { 21 }},
					 {{2, -1, -1, -1}, { 21 }},
					 {{3, -1, -1, -1}, { 22 }},
					 {{4, 5, -1, -1}, { 22 }},
					 {{7, 8, 9, -1}, { 23 }},
					 {{21, 9, -1, -1}, { 24 }},
					 {{21, 10, -1, -1}, { 24 }},
					 {{22, 18, 19, 17}, { 25 }},
					 {{22, 20, -1, -1}, { 26 }},
					 {{22, 15, 16, 17}, { 27 }},
					 {{24, 14, -1, -1}, { 28 }},//1 9 14 .
					 {{24, 15, 16, 13}, { 29 }},
					 {{23, 12, 14, -1}, { 30 }},
					 {{23, 12, 13, -1}, { 31 }},//7 8 9 12 13 .			 
					 {{4, 17, -1, -1}, { 32 }} };

char features[NUM][16] = { "有毛", "产奶", "有羽毛", "会飞", "会下蛋",//1
							"吃肉", "有犬齿", "有爪", "眼睛盯前方", "有蹄",//6
							"反刍", "黄色", "有斑点", "有黑色条纹", "长脖",//11
							"长腿", "不会飞", "会游泳", "黑白两色", "善飞",//16
							"哺乳类", "鸟类", "肉食类", "蹄类", "企鹅",//21
							"燕子", "鸵鸟", "斑马", "长颈鹿", "狮虎",//26
							"雪豹","矛盾体" };//31


int count_num = 0;					//记录输入特征个数
int rule_Max = 4;					//规则前件最大为4
int rule_numberR = -1;				//匹配规则标号
char(*dbL)[16] = features;
int dbR[100];
int result;			//记录结论
int countNum;
int condition_chose[24];			//选择的动物特征
int count_numL = 0;					//记录输入特征个数
int rule_MaxL = 4;					//规则前件最大为4
int rule_numberL = -1;				//匹配规则标号

//获取数据的个数
int getNUM() {
	int num = 0;
	while (*dbL[num] != NULL) {
		num++;
	}
	return num;
}


//获取规则个数
int getrule_NUM() {
	int num = 0;
	num = sizeof(expr) / sizeof(expr[0]);
	return num;
}


void menuL(char* a) {
	//数据库选择列表
	printf("----------------------------数据库----------------------------\n");
	int count = NUM;
	for (int i = count - 7; i < count; i++) {
		printf("%-2d.%-15s", i, dbL[i - 1]);
		if (i % 4 == 0) {
			printf("\n");
		}
	}
	if (a != NULL) {
		count++;
		printf("%d.%s ", count, a);
	}
	printf("\n");
	printf("--------------------------------------------------------------\n");
	printf("请输入选择的动物数字:");
}

//匹配规则左部分
bool matchL(int* p) {
	int k;
	int j;
	int count_elem = 0;
	int flag = 0;
	for (rule_numberL = 0; rule_numberL < getrule_NUM(); rule_numberL++) {
		count_elem = 0;
		for (int j = 0; j < count_numL; j++) {
			if (expr[rule_numberL][1][0] != -1) {
				count_elem++;
			}
			if (p[j] == expr[rule_numberL][1][0]) {
				flag++;
			}
		}
		if (flag == (count_elem / count_numL)) {
			return true;
		}
		flag = 0;
	}
	return false;
}

//找到结论
void inferenceL(int* p) {
	int u;
	int l;
	int result_count = 0;
	//	int count = 1;
	int result1;
	int result[4];			//记录结论
	while (matchL(p)) {
		u = 0;
		for (int j = 0; j < rule_MaxL; j++) {
			result[j] = expr[rule_numberL][0][j];
		}
		result1 = expr[rule_numberL][1][0];
		for (int i = 0; i < count_numL; i++) {
			if (expr[rule_numberL][1][0] == -1) {
				u++;
			}
			if (p[i] == expr[rule_numberL][1][0]) {
				p[i] = -2;				//删除已经用过的结论
			}
		}
		printf("\n----------------------------规则库----------------------------\n");
		printf("%d：规则%d IF ", ++countNum, rule_numberL);
		printf("%s", features[expr[rule_numberL][0][0] - 1]);
		for (int i = 1; expr[rule_numberL][0][i] != -1 && i < 4; i++) {
			printf(" ∧ %s", features[expr[rule_numberL][0][i] - 1]);
		}
		printf(" THEN %s\n", features[result1 - 1]);
		printf("--------------------------------------------------------------\n");
		//推理得到的前件加入到p中
		for (int k = count_numL; k < count_numL + 4; k++) {
			p[k] = result[result_count];
			result_count++;
		}
		count_numL += 4;
		result_count = 0;


		printf("\n----------------------------数据库----------------------------\n");
		for (int o = 0; o < count_numL; o++) {
			if (p[o] != -1 && p[o] != -2) {
				printf("%s\t", features[p[o] - 1]);
			}
		}
		printf("\n--------------------------------------------------------------\n");

		printf("\n");
		if (matchL(p) == false) {
			printf("\n---------------------------匹配结果---------------------------\n");
			printf("匹配到的动物基本特征：\n");
			for (int o = 0; o < count_numL; o++) {
				if (p[o] != -1 && p[o] != -2) {
					printf("%s\n", features[p[o] - 1]);
				}
			}
			printf("\n");
			return;
		}
	}
	printf("没有此动物特征\n");
}

void menuR(int a) {
	//事实库选择列表
	printf("----------------------------数据库----------------------------\n");
	int count = NUM;
	if (a != NULL) {
		for (int i = 1; dbR[i] != 0; i++) {
			if (dbR[i] == -2) {
				continue;
			}
			printf("%s\t", features[dbR[i] - 1]);
		}
		printf("\n--------------------------------------------------------------\n");
		return;
	}
	for (int i = 1; i <= count - 8; i++) {
		printf("%-2d.%-15s", i, features[i - 1]);
		if (i % 4 == 0) {
			printf("\n");
		}
	}

	printf("\n--------------------------------------------------------------\n\n");
	printf("请输入选择的特征数字：");
}

//匹配规则右部分
bool matchR(int* p) {
	int k;
	int j;
	int count_elem = 0;//前件部分中存在的特征条件的数量
	int flag = 0;//已经匹配的特征条件的数量
	for (rule_numberR = 0; rule_numberR < getrule_NUM(); rule_numberR++) {
		count_elem = 0;
		for (j = 1; dbR[j] != 0; j++) {
			if (dbR[j] == -2) {
				continue;
			}
			for (k = 0; k < 4; k++) {
				if (expr[rule_numberR][0][k] != -1) {
					count_elem++;
				}
				if (dbR[j] == expr[rule_numberR][0][k]) {
					flag++;
				}
			}
		}
		if (flag == (count_elem / count_num)) {
			int u;//前件部分中未匹配的特征条件数量
			int l;//未匹配的特征条件的数量
			u = 0;
			result = expr[rule_numberR][1][0];
			for (int i = 1; dbR[i] != 0; i++) {
				if (dbR[i] == -2) {
					continue;
				}
				for (int j = 0; j < 4; j++) {
					if (expr[rule_numberR][0][j] == -1) {
						u++;
					}
					if (dbR[i] == expr[rule_numberR][0][j]) {
						dbR[i] = -2;				//删除已经用过的前件
						count_num--;
					}
				}
			}
			if (count_num == 0) {
				l = 2;
			}
			else {
				l = rule_Max - (u / count_num);
			}
			printf("\n----------------------------规则库----------------------------\n");
			printf("%d：规则%d IF ", ++countNum, rule_numberR);
			printf("%s", features[expr[rule_numberR][0][0] - 1]);
			for (int i = 1; i <= l; i++) {
				printf(" ∧ %s", features[expr[rule_numberR][0][i] - 1]);
			}
			printf(" THEN %s\n", features[result - 1]);
			printf("--------------------------------------------------------------\n");
			//count++;							//规则计数器
			dbR[++dbR[0]] = result;
			count_num++;
			menuR(dbR[0]);
			return true;
		}
		flag = 0;
	}
	return false;
}

//找到结论
void inferenceR(int* p) {
	while (true) {
		bool success = matchR(dbR);
		if (success == false && result <= 24) {
			//			printf("---------------------------------------------------\n");
			printf("\n---------------------------匹配结果---------------------------\n");
			printf("没有匹配到动物\n");
			return;
		}
		else if (result > 24) {
			printf("\n---------------------------匹配结果---------------------------\n");
			printf("匹配到的动物是:%s\n", features[result - 1]);
			printf("\n");
			return;
		}
	}
	printf("没有匹配到动物\n");
}

int main() {
	int method = 0;//推理方向

	int chose;						//记录输入的特征
	int k = 0;					    //控制condition_chose数组
	printf("请输入推理方向：\n1：正向推理\n2：逆向推理\n");
	scanf("%d", &method);
	if (method == 1) {
		menuR(NULL);	//界面
		while (scanf("%d", &chose)) {
			dbR[++k] = chose;
		}
		count_num = dbR[0] = k;
		menuR(dbR[0]);
		inferenceR(dbR);
	}
	else if (method == 2) {
		int* p = condition_chose;		//操作选项
		menuL(NULL);				//界面
		while (scanf("%d", &chose)) {
			condition_chose[k++] = chose;
			count_numL++;
		}
		inferenceL(p);
	}
	else {
		printf("未知方向，程序结束\n");
	}
	system("pause");
	return 0;
}