#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <ctime>

using namespace std;

void atomic_counter(int numTasks, int numThreads, unsigned int duration) {
	atomic<int> globalCount = 0;
	vector<int> values(numTasks, 0);
	vector<thread> threads;
	threads.reserve(numThreads);

	long start = clock();
	for (int i = 0; i < numThreads; i++) {
		threads.push_back((thread)[&] {
			int local_count = globalCount.fetch_add(1);
			while (local_count < numTasks) {
				values.at(local_count) = 1;
				local_count = globalCount.fetch_add(1);
				if (duration) {
					this_thread::sleep_for(chrono::nanoseconds(duration));
				}
			}
			});
	}
	for (thread& thread : threads) {
		if (thread.joinable()) {
			thread.join();
		}
	}

	long stop = clock();
	long time = stop - start;

	cout << "Время на функции с " << numThreads << " потоками" << ": "<< time << " миллисекунд ";
	if (all_of(values.cbegin(), values.cend(), [](int i) {
		return i == 1;
		})) {
		cout << "+" << endl;
	}
	else {
		cout << "-" << endl;
	}
}

int main() {
	int numTasks = 1024 * 1024;

	setlocale(LC_ALL, "Russian");

	atomic_counter(numTasks, 4, 0);
	atomic_counter(numTasks, 8, 0);
	atomic_counter(numTasks, 16, 0);
	atomic_counter(numTasks, 32, 0);

	atomic_counter(numTasks, 4, 10);
	atomic_counter(numTasks, 8, 10);
	atomic_counter(numTasks, 16, 10);
	atomic_counter(numTasks, 32, 10);

	return 0;
}
