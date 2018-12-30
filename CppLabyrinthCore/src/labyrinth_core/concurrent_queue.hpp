#ifndef SRC_LABYRINTH_CORE_CONCURRENT_QUEUE_HPP_
#define SRC_LABYRINTH_CORE_CONCURRENT_QUEUE_HPP_

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

namespace multithread {

template <class T>
class ConcurrentQueue {

	std::queue<T> queue;
	std::mutex mutex;
	std::condition_variable cond_var;

public:
	T pop() {
		std::unique_lock<std::mutex> lock (mutex);
		while (queue.empty()) {
			cond_var.wait(lock);
		}
		T toreturn = queue.front();
		queue.pop();
		return toreturn;
	}

	void push(const T& item) {
		std::unique_lock<std::mutex> lock (mutex);
		queue.push(item);
		lock.unlock();
		cond_var.notify_one();
	}

	void push(T&& item) {
		std::unique_lock<std::mutex> lock (mutex);
		queue.push(item);
		lock.unlock();
		cond_var.notify_one();
	}

	bool empty() const {
		return queue.empty();
	}

};

}

#endif /* SRC_LABYRINTH_CORE_CONCURRENT_QUEUE_HPP_ */
