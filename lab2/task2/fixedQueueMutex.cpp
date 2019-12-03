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

class FixedQueueMutex
{
	uint8_t* q;
	mutex mtx;
	int size;
	condition_variable cond_push, cond_pop;
	int tail, head, count;

public:
	FixedQueueMutex(int size)
	{
		this->size = size;
		q = new uint8_t[size];
		for (int i = 0; i < size; i++)
			q[i] = 0;
		count = 0;
		tail = 0;
		head = 0;
	}

	~FixedQueueMutex()
	{
		delete[] q;
	}

	void push(uint8_t val)
	{
		unique_lock<mutex> lock(mtx);
		while (count == size)
			cond_push.wait(lock);
		q[head++ % size] = val;
		count++;
		cond_pop.notify_one();
	}

	bool pop(uint8_t& val)
	{
		unique_lock<mutex> lock(mtx);
		while (count == 0)
			cond_pop.wait(lock);
		val = q[tail % size];
		q[tail++ % size] = 0;
		count--;
		cond_push.notify_one();
		return true;
	}
};