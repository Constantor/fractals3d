#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <chrono>
#include <QImage>

class Recorder {
public:
	enum class Mode {
		kInstantStop,
		kWaitProcessing
	};

	Recorder(Mode mode);

	virtual ~Recorder() noexcept;

	void start();

	void stop();

	void push_back(std::pair<QImage, QString> &&data);

private:
	void run();

private:
	const bool instantStop;
	std::atomic_bool running = false;
	std::thread worker;

	std::queue<std::pair<QImage, QString>> queue;
	std::mutex queueMutex;
	std::condition_variable queueCondition;
};
