#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <numeric>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <ctime>

using namespace std;

class Queue
{
	queue<uint8_t> q;
	mutex mtx;

public:
	Queue(int size)
	{

	}

	void push(uint8_t val)
	{
		mtx.lock();
		q.push(val);
		mtx.unlock();
	}

	bool pop(uint8_t& val)
	{
		mtx.lock();
		if (q.empty()) {
			mtx.unlock();
			this_thread::sleep_for(chrono::milliseconds(1));
			return false;
		}
		val = q.front();
		q.pop();
		mtx.unlock();
		return true;
	}
};