#pragma once

#include <QImage>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

#include <chrono>

class Recorder {
public:
	enum class Mode {
		kInstantStop,
		kWaitProcessing
	};

	Recorder(Mode mode) : instant_stop_(mode == Mode::kInstantStop) {}
	virtual ~Recorder() {
		stop();
	}

	void start() {
		if(!running_) {
			running_ = true;
			worker_ = std::thread(&Recorder::run, this);
		}
	}

	void stop() {
		if(!running_)
			return;

		{
			std::unique_lock<std::mutex> lock(queue_mutex_);
			running_ = false;
			queue_condition_.notify_all();
		}
		worker_.join();
	}

	void push_back(std::pair<QImage, QString> &&data) {
		std::unique_lock<std::mutex> lock(queue_mutex_);
		queue_.emplace(std::move(data));
		queue_condition_.notify_all();
	}

private:
	void run() {
		while(running_ || !queue_.empty()) {
			std::unique_lock<std::mutex> lock(queue_mutex_);
			queue_condition_.wait(lock, [this] { return !queue_.empty() || !running_; });
			if(!running_ && instant_stop_) {
				break;
			}
			if(queue_.empty()) {
				continue;
			}

			auto data = std::move(queue_.front());
			queue_.pop();
			lock.unlock();

			data.first.save(data.second);
		}
	}

private:
	const bool instant_stop_;
	std::atomic_bool running_ = false;
	std::thread worker_;

	std::queue<std::pair<QImage, QString>> queue_;
	std::mutex queue_mutex_;
	std::condition_variable queue_condition_;
};
