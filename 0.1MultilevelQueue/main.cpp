#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>

using namespace std;

struct PCB
{
	string name;
	int need; //运行时间
	int wait; //已等待时间
	int turn; //周转时间
	PCB* next;
};

int myclock = 0;
const int q = 7;
PCB* RQ1 = new PCB();
PCB* RQ2 = new PCB();
PCB* Finish = nullptr;

int main()
{
	string iname;
	int ineed, iwait;
	PCB* p = nullptr;
	PCB* t = nullptr;

	//从文件读入
	ifstream read;
	read.open("rq.txt");

	PCB* q1 = RQ1;
	read >> iname >> ineed >> iwait;
	q1->name = iname;
	q1->need = ineed;
	q1->wait = iwait;
	for (int i = 2; i < 6; i++)
	{
		read >> iname >> ineed >> iwait;
		p = new PCB();
		p->name = iname;
		p->need = ineed;
		p->wait = iwait;
		q1->next = p;
		p = p->next;
		q1 = q1->next;
	}

	PCB* q2 = RQ2;
	read >> iname >> ineed >> iwait;
	q2->name = iname;
	q2->need = ineed;
	q2->wait = iwait;
	for (int i = 7; i < 11; i++)
	{
		read >> iname >> ineed >> iwait;
		p = new PCB();
		p->name = iname;
		p->need = ineed;
		p->wait = iwait;
		q2->next = p;
		p = p->next;
		q2 = q2->next;
	}

	read.close();

	//处理RQ1
	while (RQ1 != nullptr)
	{
		p = RQ1;

		//运行时间大于q
		if (RQ1->need > q)
		{
			myclock += q;
			RQ1->need -= q;
			RQ1 = RQ1->next;
			for (t = p; t->next != nullptr; t = t->next) {}
			p->next = nullptr;
			t->next = p;
		}

		//运行时间不大于q
		else
		{
			myclock += RQ1->need;
			RQ1->turn = myclock + RQ1->wait;
			RQ1 = RQ1->next;
			p->next = nullptr;
			if (Finish == nullptr)
			{
				Finish = p;
			}
			else
			{
				for (t = Finish; t->next != nullptr; t = t->next) {}
				t->next = p;
			}
		}
	}

	//处理RQ2
	while (RQ2 != nullptr)
	{
		p = RQ2;
		PCB* m = new PCB();
		PCB* f = RQ2;
		int min = RQ2->need;
		//寻找运行时间最短的进程
		for (t = p; t->next != nullptr; t = t->next)
		{
			if (t->next->need < min)
			{
				min = t->next->need;
				m = t->next;
				f = t;
			}
		}
		if (RQ2->next == nullptr)
		{
			m = RQ2;
			RQ2 = nullptr;
		}

		myclock += m->need;
		m->turn = myclock + m->wait;
		f->next = m->next;
		m->next = nullptr;
		if (Finish == nullptr)
		{
			Finish = m;
		}
		else
		{
			for (t = Finish; t->next != nullptr; t = t->next) {}
			t->next = m;
		}
	}

	//输出进程的名字与周转时间
	cout << setw(4) << "NAME" << setw(6) << "TURN" << endl;
	for (t = Finish; t != nullptr; t = t->next)
	{
		cout << setw(4) << t->name << setw(6) << t->turn << endl;
	}

	return 0;
}
