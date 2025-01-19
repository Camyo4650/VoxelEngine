#include <queue>
#include <mutex>
#include <condition_variable>

namespace Game::Util {
    template<typename T>
    class ThreadSafeQueue {
    private:
        std::queue<T> queue;
        mutable std::mutex mtx;
        std::condition_variable cv;

    public:
        void push(T value) {
            std::lock_guard<std::mutex> lock(mtx);
            queue.push(std::move(value));
            cv.notify_one();
        }

        T pop() {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]() { return !queue.empty(); });
            T value = std::move(queue.front());
            queue.pop();
            return value;
        }

        bool empty() const {
            std::lock_guard<std::mutex> lock(mtx);
            return queue.empty();
        }

        // Get the size of the queue
        size_t size() const {
            std::lock_guard<std::mutex> lock(mtx);
            return queue.size();
        }
    };
}