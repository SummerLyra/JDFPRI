#include<iostream>
#include<iomanip>
#include<climits>
#include<list>
#include<algorithm>

using namespace std;

const int insStream = 320;
const int pageSize = 1024;
const int minPageNum = 4;
const int maxPageNum = 32;
const int maxAddr = 32766;
const int unused = 65535;

struct Page
{
	int num;
	int count;

	Page(int n = unused, int c = 0) :num(n), count(c) {};
	bool operator==(const Page& p);
	bool operator<(const Page& p);
};

bool Page::operator==(const Page& p)
{
	return num == p.num;
}

bool Page::operator<(const Page& p)
{
	return count < p.count;
}

list<int> seq;
double hit = 0;

void initial()
{
	int count = 0;
	hit = 0;
	seq.clear();
	srand((int)time(0));

	while (true)
	{
		int m = rand() % maxAddr + 1;
		seq.push_back(m);
		count++;
		if (count >= insStream)
		{
			break;
		}

		seq.push_back(m + 1);
		count++;
		if (count >= insStream)
		{
			break;
		}

		int m1 = rand() % m;
		seq.push_back(m1);
		count++;
		if (count >= insStream)
		{
			break;
		}

		seq.push_back(m1 + 1);
		count++;
		if (count >= insStream)
		{
			break;
		}

		int m2 = rand() % (maxAddr - 1 - m1) + (m1 + 2);
		seq.push_back(m2);
		count++;
		if (count >= insStream)
		{
			break;
		}

		seq.push_back(m2 + 1);
		count++;
		if (count >= insStream)
		{
			break;
		}
	}

	for (list<int>::iterator it = seq.begin(); it != seq.end(); it++)
	{
		*it /= pageSize;
	}
}

void fifo(int available)
{
	list<int> memory(available, unused);
	hit = 0;

	for (list<int>::iterator it = seq.begin(); it != seq.end(); it++)
	{
		list<int>::iterator f = find(memory.begin(), memory.end(), *it);
		if (f != memory.end())
		{
			hit++;
		}
		else
		{
			memory.pop_front();
			memory.push_back(*it);
		}
	}

	hit /= (double)seq.size();
	hit *= 100;
	cout << setw(7) << hit << " %    ";
}

void lru(int available)
{
	list<Page> memory(available);
	hit = 0;

	for (list<int>::iterator it = seq.begin(); it != seq.end(); it++)
	{
		for (list<Page>::iterator tr = memory.begin(); tr != memory.end(); tr++)
		{
			tr->count++;
		}

		Page n(*it);
		list<Page>::iterator f = find(memory.begin(), memory.end(), n);
		if (f != memory.end())
		{
			f->count = 0;
			hit++;
		}
		else
		{
			memory.sort();
			memory.pop_back();
			memory.push_front(n);
		}
	}

	hit /= (double)seq.size();
	hit *= 100;
	cout << setw(7) << hit << " %    ";
}

void opt(int available)
{
	list<int> memory;
	hit = 0;

	for (list<int>::iterator it = seq.begin(); it != seq.end(); it++)
	{
		list<int>::iterator f = find(memory.begin(), memory.end(), *it);
		if (f != memory.end())
		{
			hit++;
		}
		else
		{
			if (memory.size() < available)
			{
				memory.push_back(*it);
			}
			else
			{
				list<int> part(it, seq.end());
				part.pop_front();
				list<Page> timeDist;

				for (list<int>::iterator tr = memory.begin(); tr != memory.end(); tr++)
				{
					list<int>::iterator ftest = find(part.begin(), part.end(), *tr);
					if (ftest == part.end())
					{
						Page n(*tr, unused);
						timeDist.push_back(n);
						continue;
					}

					int i = 0;
					for (list<int>::iterator fi = part.begin(); fi != part.end(); fi++)
					{
						if (*fi == *tr)
						{
							Page n(*tr, i);
							timeDist.push_back(n);
							break;
						}
						else
						{
							i++;
						}
					}
				}

				timeDist.sort();
				list<int>::iterator del = find(memory.begin(), memory.end(), timeDist.back().num);
				memory.erase(del);
				memory.push_back(*it);
			}
		}
	}

	hit /= (double)seq.size();
	hit *= 100;
	cout << setw(7) << hit << " %    ";
}

int main()
{
	int choice = 0;
	cout.setf(ios::left);

	do
	{
		cout << "--------------------" << endl;
		cout << "THE HIT RATE IS:" << endl;
		cout << setw(13) << "FIFO" << setw(13) << "LRU" << setw(13) << "OPT" << "AVAILABLE" << endl;

		initial();
		for (int avail = minPageNum; avail <= maxPageNum; avail++)
		{
			fifo(avail);
			lru(avail);
			opt(avail);
			cout << avail << endl;
		}

		cout << "--------------------" << endl;
		cout << "PRESS 0 TO CALCULATE A NEW SERIES OF RESULTS." << endl;
		cout << "--------------------" << endl;

		cin >> choice;
	}
	while (choice == 0);

	return 0;
}
