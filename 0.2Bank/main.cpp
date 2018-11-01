#include<iostream>
#include<fstream>

#define N 5 //进程个数
#define M 3 //资源种类

using namespace std;

int available[M]; //可提供资源向量
int alloc[N][M]; //已分配资源矩阵
int need[N][M]; //需求量矩阵

int main()
{
	int request[M]; //请求向量

	//从文件读入
	ifstream read;
	read.open("bank.txt");

	for (int i = 0; i < M; i++)
	{
		read >> available[i];
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			read >> alloc[i][j];
		}
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			read >> need[i][j];
		}
	}

	read.close();

	int flag = 1;
	while (flag == 1)
	{
		int p;
		cout << "->PLEASE INPUT THE PROCESS NUMBER FROM 1 TO " << N << ":" << endl;
		cin >> p;
		if (p < 1 || p > N)
		{
			cout << "->PROCESS DOES NOT EXIST. PLEASE INPUT AGAIN CORRECTLY." << endl;
			continue;
		}
		cout << "->PLEASE INPUT THE " << M << " REQUEST OF THIS PROCESS:" << endl;
		for (int i = 0; i < M; i++)
		{
			cin >> request[i];
		}

		//合法性测试
		int isIllegal = 0;
		for (int i = 0; i < M; i++)
		{
			if (request[i] > need[p - 1][i])
			{
				isIllegal = 1;
				cout << "->ILLEGAL REQUEST FOUND. PLEASE INPUT AGAIN CORRECTLY." << endl;
				break;
			}
		}
		for (int i = 0; i < M; i++)
		{
			if (request[i] > available[i])
			{
				isIllegal = 1;
				cout << "->UNAVAILABLE REQUEST FOUND. PLEASE INPUT THE NEXT REQUEST." << endl;
				break;
			}
		}
		if (isIllegal == 1)
		{
			continue;
		}

		//试探性分配
		int work[M];
		int preAlloc[N][M];
		int preNeed[N][M];
		int finish[N] = { 0 };

		for (int i = 0; i < M; i++)
		{
			work[i] = available[i] - request[i];
		}
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < M; j++)
			{
				preAlloc[i][j] = alloc[i][j];
				preNeed[i][j] = need[i][j];
			}
		}
		for (int i = 0; i < M; i++)
		{
			preAlloc[p - 1][i] += request[i];
			preNeed[p - 1][i] -= request[i];
		}

		//安全性测试
		for (int n = 0; n < N; n++)
		{
			for (int i = 0; i < N; i++)
			{
				if (finish[i] == 0)
				{
					int sat = 1;
					for (int j = 0; j < M; j++)
					{
						if (preNeed[i][j] <= work[j])
						{
							sat *= 1;
						}
						else
						{
							sat *= 0;
						}
					}
					if (sat == 1)
					{
						finish[i] = 1;
						for (int j = 0; j < M; j++)
						{
							work[j] += preAlloc[i][j];
						}
						cout << (i + 1) << " ";
					}
				}
			}
		}

		int isFinish = 1;
		for (int i = 0; i < N; i++)
		{
			isFinish *= finish[i];
		}
		if (isFinish == 1)
		{
			cout << "IS THE SAFETY LIST." << endl;
			cout << "->REQUEST ACCEPTED." << endl;
			int noNeed = 0;
			for (int i = 0; i < M; i++)
			{
				available[i] -= request[i];
				alloc[p - 1][i] += request[i];
				need[p - 1][i] -= request[i];
				noNeed += need[p - 1][i];
			}
			if (noNeed == 0)
			{
				for (int i = 0; i < M; i++)
				{
					available[i] += alloc[p - 1][i];
					alloc[p - 1][i] = 0;
				}
			}
		}
		else
		{
			cout << "->REQUEST REFUSED FOR UNSAFETY. PLEASE INPUT THE NEXT REQUEST." << endl;
			continue;
		}

		//输出系统当前资源状态
		cout << "********************************" << endl;
		cout << "THE ALLOCATION MATRIX NOW IS:" << endl;
		for (int i = 0; i < N; i++)
		{
			cout << "P" << (i + 1) << "\t";
			for (int j = 0; j < M; j++)
			{
				cout << alloc[i][j] << " ";
			}
			cout << endl;
		}
		cout << "THE NEED MATRIX NOW IS:" << endl;
		for (int i = 0; i < N; i++)
		{
			cout << "P" << (i + 1) << "\t";
			for (int j = 0; j < M; j++)
			{
				cout << need[i][j] << " ";
			}
			cout << endl;
		}
		cout << "THE AVAILABLE VECTOR NOW IS:" << endl;
		cout << "  \t";
		for (int i = 0; i < M; i++)
		{
			cout << available[i] << " ";
		}
		cout << endl;
		cout << "********************************" << endl;

		cout << "->IS THERE ANY REQUEST ELSE? 1->YES 0->NO:" << endl;
		cin >> flag;
	}

	return 0;
}
