#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctime>

#pragma warning (disable:4996)
#define M 4//棋盘大小
int N;//码数
int test;

// 棋盘
typedef struct State {
	int board[M][M];
	int blankX, blankY;
	int cost, level;
	struct State* parent;
} State;

// 用于A*算法的优先队列节点
typedef struct Node {
	State* state;
	int priority;
} Node;

int method;//启发式
int front = 0, rear = 0;
int runn = 0;//记录执行次数

// 创建一个新状态
State* create_state(int board[M][M], int blankX, int blankY, int level, State* parent) {
	State* state = (State*)malloc(sizeof(State));
	state->blankX = blankX;
	state->blankY = blankY;
	state->level = level;
	state->parent = parent;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			state->board[i][j] = board[i][j];
		}
	}
	return state;
}

// 交换两个位置上的数字
void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

// 检查两个状态是否相同
bool is_equal(State* state1, State* state2) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (state1->board[i][j] != state2->board[i][j]) {
				return false;
			}
		}
	}
	return true;
}

// 检查状态是否有效
bool is_valid(int x, int y) {
	return (x >= 0 && x < N && y >= 0 && y < N);
}

// 启发式函数(错误数字个数)
int heuristic_WrongDigitalNum(State* state, State* target) {
	int h = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			runn++;
			if (state->board[i][j] != 0) {
				if (state->board[i][j] != target->board[i][j]) {
					h++;
				}
			}
		}
	}
	return h;
}

// 启发式函数(错误数字距离)
int heuristic_WrongDigitalLay(State* state, State* target) {
	int h = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (state->board[i][j] != 0) {
				for (int a = 0; a < N; a++) {
					for (int b = 0; b < N; b++) {
						runn++;
						if (state->board[i][j] == target->board[a][b])
							h += abs(a - i) + abs(b - j);
					}
				}
			}
		}
	}
	return h;
}

// 启发式函数(线性冲突)
int heuristic_LinearConflict(State* state, State* target) {
	int h = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (state->board[i][j] != 0) {
				// 检查水平线性冲突
				for (int k = j + 1; k < N; k++) {
					runn++;
					if (state->board[i][k] != 0 &&
						target->board[i][j] != 0 &&
						target->board[i][k] != 0 &&
						state->board[i][j] == target->board[i][k] &&
						state->board[i][k] == target->board[i][j]) {
						h++; // 每个水平线性冲突额外增加代价
						break;
					}
				}
				// 检查垂直线性冲突
				for (int k = i + 1; k < N; k++) {
					runn++;
					if (state->board[k][j] != 0 &&
						target->board[i][j] != 0 &&
						target->board[k][j] != 0 &&
						state->board[i][j] == target->board[k][j] &&
						state->board[k][j] == target->board[i][j]) {
						h++; // 每个垂直线性冲突额外增加代价
						break;
					}
				}
			}
		}
	}
	return h;
}

// 启发式函数(纵横匹配)
int heuristic_Custom(State* state, State* target) {
	int h = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (state->board[i][j] != 0 && state->board[i][j] != target->board[i][j]) {
				h += 2;
				for (int k = 1; k < N; k++) {
					runn++;
					if (state->board[i][j] == target->board[i][(j + k) % N] || state->board[i][j] == target->board[(i + k) % N][j]) {
						h--;
						break;
					}
				}
			}
		}
	}
	return h;
}

// 打印状态
void print_state(State* state) {

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf("%2d ", state->board[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

//打印结果
void print_res(State* state) {
	if (state != NULL) {
		print_res(state->parent);
		print_state(state);
	}
}

// A*算法主函数
void solve_puzzle(State* initial_state, State* target_state) {
	clock_t start, end;
	start = clock();

	rear = 0, front = 0;
	// 创建一个优先队列
	Node queue[_I16_MAX];//16位int最大值32767


	// 将初始状态插入队列
	queue[rear].state = initial_state;
	queue[rear].priority = 0;
	rear++;

	// A*算法主循环
	while (front < rear && rear < _I16_MAX) {
		end = clock();
		if ((double)(end - start) / CLK_TCK >= 3.0) {
			printf("单次运行时间超过3秒，程序停止\n");
			test = -3;
			return;
		}
		// 从队列中取出优先级最高的状态
		State* current_state = queue[front].state;
		int current_priority = queue[front].priority;
		front++;

		// 如果当前状态与目标状态相同，找到解
		if (is_equal(current_state, target_state)) {
			if (method < 0)
				return;
			// 递归调用state打印解路径
			printf("解路径:\n");
			print_res(current_state);
			return;
		}

		// 生成可能的下一步状态
		int dx[] = { 1, -1, 0, 0 };
		int dy[] = { 0, 0, 1, -1 };

		for (int i = 0; i < 4; i++) {
			int new_x = current_state->blankX + dx[i];
			int new_y = current_state->blankY + dy[i];

			if (is_valid(new_x, new_y)) {
				State* new_state = create_state(current_state->board, new_x, new_y, current_state->level + 1, current_state);
				new_state->board[current_state->blankX][current_state->blankY] = new_state->board[new_x][new_y];
				new_state->board[new_x][new_y] = 0;

				// 计算新状态的启发式函数值
				int new_priority = 0;
				switch (abs(method))
				{
				case 1:
					new_priority = new_state->level + heuristic_WrongDigitalNum(new_state, target_state);
					break;
				case 2:
					new_priority = new_state->level + heuristic_WrongDigitalLay(new_state, target_state);
					break;
				case 3:
					new_priority = new_state->level + heuristic_LinearConflict(new_state, target_state);
					break;
				case 4:
					new_priority = new_state->level + heuristic_Custom(new_state, target_state);
					break;
				default:
					printf("未知启发式\n");
					return;
				}


				// 检查新状态是否已经在队列中
				bool in_queue = false;
				for (int j = 0; j < rear; j++) {
					if (is_equal(new_state, queue[j].state)) {
						in_queue = true;
						break;
					}
				}
				if (!in_queue) {
					// 将新状态插入队列
					queue[rear].state = new_state;
					queue[rear].priority = new_priority;
					rear++;
				}
			}
		}

		// 根据优先级重新排序队列
		for (int i = front; i < rear; i++) {
			for (int j = i + 1; j < rear; j++) {
				if (queue[i].priority > queue[j].priority) {
					Node temp = queue[i];
					queue[i] = queue[j];
					queue[j] = temp;
				}
			}
		}
		//		print_state(current_state);
	}

	printf("无解\n");
	exit(999);
}

int main() {
	State* initial_state, * target_state;
	printf("请输入码数：\n3： 8数码\n4：15数码\n");
	scanf("%d", &N);
	if (N == 3) {
		int target[M][M] = { {1, 2, 3}, {8, 0, 4}, {7, 6, 5} };
		target_state = create_state(target, 1, 1, 0, NULL);
		//测试集1
		/*int initial[M][M] = {{1, 0, 3}, {7, 2, 4}, {6, 8, 5}};
		initial_state = create_state(initial, 0, 1, 0, NULL);*/


		//测试集2
		int initial[M][M] = {{2, 8, 3}, {1, 6, 4}, {7, 0, 5}};
		initial_state = create_state(initial, 2, 1, 0, NULL);
	}
	else if (N == 4) {
		int target[M][M] = { {1, 2, 3,4}, {5, 6, 7,8}, {9, 10, 11,12},{13,14,15,0} };
		target_state = create_state(target, 3, 3, 0, NULL);
		//测试集1
		/*int initial[M][M] = {{5,1,2,4}, {9,6,3,8}, {13,15,10,11},{14,0,7,12}};
		initial_state = create_state(initial, 3, 1, 0, NULL);*/


		//测试集2
		int initial[M][M] = {{1, 2, 3,4}, {5, 6, 7,8}, {9, 10, 11,12},{0,13,14,15}};
		initial_state = create_state(initial, 3, 0, 0, NULL);
	}
	else {
		printf("未知码数，程序结束\n");
		return -1;
	}


	printf("初始状态:\n");
	print_state(initial_state);
	printf("目标状态:\n");
	print_state(target_state);
	printf("请输入使用的启发式：\n1：错误数字个数\n2：错误数字距离\n3：线性冲突\n4：纵横匹配(自创)\n");
	scanf("%d", &method);
	//按照method规定的启发式求解
	solve_puzzle(initial_state, target_state);
	//是否进行性能分析
	printf("若要进行性能分析，请输入0：");
	scanf("%d", &method);
	int t = 0;
	if (N <= 3) {
		t = 100000;
	}
	else {
		t = 10000;
	}
	if (method == 0) {
		test = t;
		printf("%d次运行情况如下\n", test);
		for (method = -1; method >= -4; method--, test = t) {

			runn = 0;
			clock_t start, end;
			start = clock();

			while (test--) {
				solve_puzzle(initial_state, target_state);
				if (test == -3) {
					break;
				}
			}
			if (test == -3) {
				continue;
			}

			end = clock();
			printf("启发式%d：深度 = %d\t启发式循环总计 = %7d万次\t耗时 = %lf秒\n", abs(method), rear, runn / 10000, (double)(end - start) / CLK_TCK);
		}
	}
	printf("程序运行结束\n");
	return 0;
}
