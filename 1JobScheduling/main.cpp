#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<list>

using namespace std;

struct JCB
{
	string name;
	int arrival;
	int run;
	int wait;
	int finish;
	double turn;
	double response;

	JCB() :name(""), arrival(0), run(0), wait(0), finish(0), turn(0), response(0) {};
	bool operator==(const JCB& j);
};

bool JCB::operator==(const JCB& j)
{
	return (name == j.name);
}

int clock1 = 0;
int clock2 = 0;
list<JCB> q;
double avgTurnTime = 0;
double avgWeightTurnTime = 0.0;

bool compareByArrival(const JCB& j1, const JCB& j2)
{
	return j1.arrival < j2.arrival;
}

bool compareByRun(const JCB& j1, const JCB& j2)
{
	return j1.run < j2.run;
}

bool compareByResponse(const JCB& j1, const JCB& j2)
{
	return ((double)j1.wait / j1.run) < ((double)j2.wait / j2.run);
}

void scheduling(list<JCB> q)
{
	clock1 = clock2 = 0;
	avgTurnTime = avgWeightTurnTime = 0;
	cout << "NAME" << " " << "FINISH" << " " << "TURN" << " " << "WEIGHTED" << endl;

	for (list<JCB>::iterator it = q.begin(); it != q.end(); it++)
	{
		if (clock1 < clock2)
		{
			if (it->arrival < clock1)
			{
				it->finish = clock1 + it->run;
				clock1 = it->finish;
				it->turn = it->finish - it->arrival;
				it->response = it->turn / it->run;
				avgTurnTime += it->turn;
				avgWeightTurnTime += it->response;
			}
			else
			{
				it->finish = it->arrival + it->run;
				clock1 = it->finish;
				it->turn = it->run;
				it->response = it->turn / it->run;
				avgTurnTime += it->turn;
				avgWeightTurnTime += it->response;
			}
		}

		else
		{
			if (it->arrival < clock2)
			{
				it->finish = clock2 + it->run;
				clock2 = it->finish;
				it->turn = it->finish - it->arrival;
				it->response = it->turn / it->run;
				avgTurnTime += it->turn;
				avgWeightTurnTime += it->response;
			}
			else
			{
				it->finish = it->arrival + it->run;
				clock2 = it->finish;
				it->turn = it->run;
				it->response = it->turn / it->run;
				avgTurnTime += it->turn;
				avgWeightTurnTime += it->response;
			}
		}

		cout << setw(4) << it->name << setw(7) << it->finish << setw(5) << it->turn << setw(9) << it->response << endl;
	}

	avgTurnTime /= (double)q.size();
	avgWeightTurnTime /= (double)q.size();
	cout << "AVERAGE TURN TIME = " << avgTurnTime << endl;
	cout << "AVERAGE WEIGHTED TURN TIME = " << avgWeightTurnTime << endl;
	cout << "--------------------" << endl;
}

void fcfs()
{
	list<JCB> jqueue = q;

	jqueue.sort(compareByArrival);

	scheduling(jqueue);
}

void sjf()
{
	list<JCB> jqueue;
	list<JCB> qr = q;
	list<JCB> q1, q2;
	clock1 = clock2 = 0;

	while (qr.empty() == false)
	{
		if (clock1 < clock2)
		{
			for (list<JCB>::iterator it = qr.begin(); it != qr.end(); it++)
			{
				if (it->arrival <= clock1)
				{
					q1.push_back(*it);
				}
			}
			if (q1.empty() == false)
			{
				q1.sort(compareByRun);
				jqueue.push_back(q1.front());
				qr.remove(q1.front());
				clock1 += q1.front().run;
				q1.clear();
			}
			else
			{
				list<JCB> qt = qr;
				qt.sort(compareByArrival);
				jqueue.push_back(qt.front());
				qr.remove(qt.front());
				clock1 = qt.front().arrival + qt.front().run;
			}

			for (list<JCB>::iterator it = qr.begin(); it != qr.end(); it++)
			{
				if (it->arrival < clock2)
				{
					q2.push_back(*it);
				}
			}
			if (q2.empty() == false)
			{
				q2.sort(compareByRun);
				jqueue.push_back(q2.front());
				qr.remove(q2.front());
				clock2 += q2.front().run;
				q2.clear();
			}
			else
			{
				list<JCB> qt = qr;
				qt.sort(compareByArrival);
				jqueue.push_back(qt.front());
				qr.remove(qt.front());
				clock2 = qt.front().arrival + qt.front().run;
			}
		}

		else
		{
			for (list<JCB>::iterator it = qr.begin(); it != qr.end(); it++)
			{
				if (it->arrival > 0 && it->arrival <= clock2)
				{
					q2.push_back(*it);
				}
			}
			if (q2.empty() == false)
			{
				q2.sort(compareByRun);
				jqueue.push_back(q2.front());
				qr.remove(q2.front());
				clock2 += q2.front().run;
				q2.clear();
			}
			else
			{
				list<JCB> qt = qr;
				qt.sort(compareByArrival);
				jqueue.push_back(qt.front());
				qr.remove(qt.front());
				clock2 = qt.front().arrival + qt.front().run;
			}

			for (list<JCB>::iterator it = qr.begin(); it != qr.end(); it++)
			{
				if (it->arrival < clock1)
				{
					q1.push_back(*it);
				}
			}
			if (q1.empty() == false)
			{
				q1.sort(compareByRun);
				jqueue.push_back(q1.front());
				qr.remove(q1.front());
				clock1 += q1.front().run;
				q1.clear();
			}
			else
			{
				list<JCB> qt = qr;
				qt.sort(compareByArrival);
				jqueue.push_back(qt.front());
				qr.remove(qt.front());
				clock1 = qt.front().arrival + qt.front().run;
			}
		}
	}

	scheduling(jqueue);
}

void hrrn()
{
	list<JCB> jqueue;
	list<JCB> qr = q;
	list<JCB> q1, q2;
	clock1 = clock2 = 0;

	while (qr.empty() == false)
	{
		if (clock1 < clock2)
		{
			for (list<JCB>::iterator it = qr.begin(); it != qr.end(); it++)
			{
				if (it->arrival <= clock1)
				{
					it->wait = clock1 - it->arrival;
					q1.push_back(*it);
				}
			}
			if (q1.empty() == false)
			{
				q1.sort(compareByResponse);
				jqueue.push_back(q1.back());
				qr.remove(q1.back());
				clock1 += q1.back().run;
				q1.clear();
			}
			else
			{
				list<JCB> qt = qr;
				qt.sort(compareByArrival);
				jqueue.push_back(qt.front());
				qr.remove(qt.front());
				clock1 = qt.front().arrival + qt.front().run;
			}

			for (list<JCB>::iterator it = qr.begin(); it != qr.end(); it++)
			{
				if (it->arrival < clock2)
				{
					it->wait = clock2 - it->arrival;
					q2.push_back(*it);
				}
			}
			if (q2.empty() == false)
			{
				q2.sort(compareByResponse);
				jqueue.push_back(q2.back());
				qr.remove(q2.back());
				clock2 += q2.back().run;
				q2.clear();
			}
			else
			{
				list<JCB> qt = qr;
				qt.sort(compareByArrival);
				jqueue.push_back(qt.front());
				qr.remove(qt.front());
				clock2 = qt.front().arrival + qt.front().run;
			}
		}

		else
		{
			for (list<JCB>::iterator it = qr.begin(); it != qr.end(); it++)
			{
				if (it->arrival > 0 && it->arrival <= clock2)
				{
					it->wait = clock2 - it->arrival;
					q2.push_back(*it);
				}
			}
			if (q2.empty() == false)
			{
				q2.sort(compareByResponse);
				jqueue.push_back(q2.back());
				qr.remove(q2.back());
				clock2 += q2.back().run;
				q2.clear();
			}
			else
			{
				list<JCB> qt = qr;
				qt.sort(compareByArrival);
				jqueue.push_back(qt.front());
				qr.remove(qt.front());
				clock2 = qt.front().arrival + qt.front().run;
			}

			for (list<JCB>::iterator it = qr.begin(); it != qr.end(); it++)
			{
				if (it->arrival < clock1)
				{
					it->wait = clock1 - it->arrival;
					q1.push_back(*it);
				}
			}
			if (q1.empty() == false)
			{
				q1.sort(compareByResponse);
				jqueue.push_back(q1.back());
				qr.remove(q1.back());
				clock1 += q1.back().run;
				q1.clear();
			}
			else
			{
				list<JCB> qt = qr;
				qt.sort(compareByArrival);
				jqueue.push_back(qt.front());
				qr.remove(qt.front());
				clock1 = qt.front().arrival + qt.front().run;
			}
		}
	}

	scheduling(jqueue);
}

int main()
{
	ifstream read;
	read.open("data");
	if (!read)
	{
		cout << "DATA FILE DOES NOT EXIST." << endl;
		return 0;
	}
	while (true)
	{
		JCB n;
		read >> n.name >> n.arrival >> n.run;

		if (n.name == "")
		{
			break;
		}

		q.push_back(n);
	}
	read.close();

	int choice = 1;
	while (choice != 0)
	{
		cout << "PLEASE CHOOSE:" << endl;
		cout << "1.FIRST COME FIRST SERVED" << endl;
		cout << "2.SHORTEST JOB FIRST" << endl;
		cout << "3.HIGHEST RESPONSE RATIO NEXT" << endl;
		cout << "0.EXIT" << endl;
		cout << "--------------------" << endl;

		cin >> choice;
		switch (choice)
		{
		case 1:
			fcfs();
			break;
		case 2:
			sjf();
			break;
		case 3:
			hrrn();
			break;
		case 0:
			return 0;
		default:
			cout << "ERROR!" << endl;
			break;
		}
	}

	return 0;
}
