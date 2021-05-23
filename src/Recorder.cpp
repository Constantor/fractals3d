#include "Recorder.hpp"

Recorder::Recorder(Mode mode) : instantStop(mode == Mode::kInstantStop) {}

Recorder::~Recorder() noexcept {
	stop();
}

void Recorder::start() {
	if(!running) {
		running = true;
		worker = std::thread(&Recorder::run, this);
	}
}

void Recorder::stop() {
	if(!running)
		return;
	{
		std::unique_lock<std::mutex> lock(queueMutex);
		running = false;
		queueCondition.notify_all();
	}
	worker.join();
}

void Recorder::push_back(std::pair<QImage, QString> &&data) {
	std::unique_lock<std::mutex> lock(queueMutex);
	queue.emplace(std::move(data));
	queueCondition.notify_all();
}

void Recorder::run() {
	while(running || !queue.empty()) {
		std::unique_lock<std::mutex> lock(queueMutex);
		queueCondition.wait(lock, [this] { return !queue.empty() || !running; });
		if(!running && instantStop)
			break;
		if(queue.empty())
			continue;

		auto data = std::move(queue.front());
		queue.pop();
		lock.unlock();

		data.first.save(data.second);
	}
}
