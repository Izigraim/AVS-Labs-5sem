#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <numeric>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <ctime>
#include "dynamicMutexQueue.cpp"
#include "fixedQueueMutex.cpp"
#include "fixedQueueAtomic.cpp"

template <typename T>
void Task(int size) {
	vector<int> consumerNum = { 1, 2, 4 };
	vector<int> produserNum = { 1, 2, 4 };
	int taskNum = 4 * 1024 * 1024;
	
	T q(size);

	for (int producerI : produserNum) {
		for (int consumerI : consumerNum) {

			atomic<int> sum = 0;
			cout << "Производителей: " << producerI << " Потребителей: " << consumerI << endl;
			int ii = 0;
			int jj = 0;

			auto producer = [&]() {
				auto start = clock();
				for (int i = 0; i < taskNum; i++) {
					q.push(1);
				}
				auto end = clock();
				printf("Производитель %d - %f \n", ++ii, (double)(end - start) / 1000);
			};

			auto consumer = [&]() {
				auto start = clock();
				for (int i = 0; i < taskNum * producerI / consumerI; i++) {
					uint8_t val = 0;
					while (!q.pop(val));
					sum += val;
				}
				auto end = clock();
				printf("Потребитель %d - %f \n", ++jj, (double)(end - start) / 1000);
			};

			vector<thread> threads;;
			for (int i = 0; i < producerI; i++)
				threads.push_back(thread(producer));
			for (int i = 0; i < consumerI; i++)
				threads.push_back(thread(consumer));
			for (int i = 0; i < consumerI + producerI; i++)
				threads[i].join();

			if (sum == taskNum * producerI)
			{
				cout << "+" << endl << endl;
			}
			else
			{
				cout << "-" << endl;
				cout << "Суммы не совпали" << endl;
				exit(0);
			}
		}
	}
}


void main()
{
	setlocale(LC_ALL, "Russian");

	Task<Queue>(0);

	vector<int> queueSize = { 1, 4, 16 };

	for (int i : queueSize)
	{
		Task<FixedQueue>(i);
	}
	

	for (int i : queueSize)
	{
		Task<FixedQueueMutex>(i);
	}
}