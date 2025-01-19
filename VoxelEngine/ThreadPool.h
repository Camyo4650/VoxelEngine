#include <thread>
#include <vector>
#include <functional>
#include <future>
#include <queue>

namespace Game::Util {
    class ThreadPool {
    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex mtx;
        std::condition_variable cv;
        bool stop = false;

    public:
        ThreadPool(size_t threads) {
            for (size_t i = 0; i < threads; ++i) {
                workers.emplace_back([this]() {
                    while (true) {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(this->mtx);
                            this->cv.wait(lock, [this]() { return this->stop || !this->tasks.empty(); });

                            if (this->stop && this->tasks.empty()) return;

                            task = std::move(this->tasks.front());
                            this->tasks.pop();
                        }
                        task();
                    }
                    });
            }
        }

        template<class F>
        void enqueue(F&& task) {
            {
                std::lock_guard<std::mutex> lock(mtx);
                tasks.emplace(std::forward<F>(task));
            }
            cv.notify_one();
        }

        ~ThreadPool() {
            {
                std::lock_guard<std::mutex> lock(mtx);
                stop = true;
            }
            cv.notify_all();
            for (std::thread& worker : workers) {
                worker.join();
            }
        }
    };
}
