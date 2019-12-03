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


class FixedQueue {
	atomic<uint8_t>* q;
	int size;
	atomic<int> head, tail;

public:
	FixedQueue(int size) {
		head = 0;
		tail = 0;
		this->size = size;
		q = new atomic<uint8_t>[size];
		for (int i = 0; i < size; i++)
			q[i] = 0;
	}

	~FixedQueue()
	{
		delete[] q;
	}

	void push(uint8_t val) {
		while (true) {
			int cur = head.load();
			if (cur == tail.load() + size) {
				continue;
			}

			uint8_t a = q[cur % size];
			if (a != 0) continue;

			if (head.compare_exchange_strong(cur, cur + 1)) {
				if (q[cur % size].compare_exchange_strong(a, val))
					return;
			}
		}
	}

	bool pop(uint8_t& val) {
		int cur = tail.load();
		if (cur == head.load()) {
			return false;
		}

		uint8_t a = q[cur % size];
		if (a == 0) return false;
		if (tail.compare_exchange_strong(cur, cur + 1)) {

			if (q[cur % size].compare_exchange_strong(a, 0)) {
				val = a;
				return true;
			}
		}
		return false;
	}
};