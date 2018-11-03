#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<list>

using namespace std;

struct DiskAccess
{
	string name;
	int track;
	int dist;

	DiskAccess() :name(""), track(0), dist(0) {};
	bool operator==(const DiskAccess& dt);
};

bool DiskAccess::operator==(const DiskAccess& dt)
{
	return (name == dt.name);
}

int pos = 90;
list<DiskAccess> d;
double avgDist = 0;

bool compareByTrack(const DiskAccess& d1, const DiskAccess& d2)
{
	return d1.track < d2.track;
}

bool compareByDist(const DiskAccess& d1, const DiskAccess& d2)
{
	return d1.dist < d2.dist;
}

void fcfs()
{
	list<DiskAccess> tqueue = d;
	pos = 90;
	avgDist = 0;
	cout << "NAME" << " " << "TRACK" << " " << "DISTANCE" << endl;

	for (list<DiskAccess>::iterator it = tqueue.begin(); it != tqueue.end(); it++)
	{
		it->dist = abs(pos - it->track);
		avgDist += it->dist;
		pos = it->track;

		cout << setw(4) << it->name << setw(6) << it->track << setw(9) << it->dist << endl;
	}

	avgDist /= (double)d.size();
	cout << "AVERAGE DISTANCE = " << avgDist << endl;
	cout << "--------------------" << endl;
}

void sstf()
{
	list<DiskAccess> tqueue = d;
	pos = 90;
	avgDist = 0;
	cout << "NAME" << " " << "TRACK" << " " << "DISTANCE" << endl;

	while (tqueue.empty() == false)
	{
		for (list<DiskAccess>::iterator it = tqueue.begin(); it != tqueue.end(); it++)
		{
			it->dist = abs(pos - it->track);
		}
		tqueue.sort(compareByDist);
		avgDist += tqueue.front().dist;
		pos = tqueue.front().track;

		cout << setw(4) << tqueue.front().name << setw(6) << tqueue.front().track << setw(9) << tqueue.front().dist << endl;

		tqueue.pop_front();
	}

	avgDist /= (double)d.size();
	cout << "AVERAGE DISTANCE = " << avgDist << endl;
	cout << "--------------------" << endl;
}

void scan()
{
	list<DiskAccess> tqueue = d;
	pos = 90;
	avgDist = 0;
	cout << "NAME" << " " << "TRACK" << " " << "DISTANCE" << endl;

	list<DiskAccess> minor, major;
	for (list<DiskAccess>::iterator it = tqueue.begin(); it != tqueue.end(); it++)
	{
		if (it->track < pos)
		{
			minor.push_back(*it);
		}
		else
		{
			major.push_back(*it);
		}
	}

	major.sort(compareByTrack);
	minor.sort(compareByTrack);
	minor.reverse();
	major.splice(major.end(), minor);
	tqueue = major;

	for (list<DiskAccess>::iterator it = tqueue.begin(); it != tqueue.end(); it++)
	{
		it->dist = abs(pos - it->track);
		avgDist += it->dist;
		pos = it->track;

		cout << setw(4) << it->name << setw(6) << it->track << setw(9) << it->dist << endl;
	}

	avgDist /= (double)d.size();
	cout << "AVERAGE DISTANCE = " << avgDist << endl;
	cout << "--------------------" << endl;
}

int main()
{
	ifstream read;
	read.open("/home/dy/2/data");
	if (!read)
	{
		cout << "DATA FILE DOES NOT EXIST." << endl;
		return 0;
	}
	while (true)
	{
		DiskAccess t;
		read >> t.name >> t.track;

		if (t.name == "")
		{
			break;
		}

		d.push_back(t);
	}
	read.close();

	int choice = 1;
	while (choice != 0)
	{
		cout << "PLEASE CHOOSE:" << endl;
		cout << "1.FIRST COME FIRST SERVED" << endl;
		cout << "2.SHORTEST SEEK TIME FIRST" << endl;
		cout << "3.ELEVATOR ALGORITHM" << endl;
		cout << "0.EXIT" << endl;
		cout << "--------------------" << endl;

		cin >> choice;
		switch (choice)
		{
			case 1:
				fcfs();
				break;
			case 2:
				sstf();
				break;
			case 3:
				scan();
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
