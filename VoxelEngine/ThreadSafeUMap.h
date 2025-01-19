#include <unordered_map>
#include <mutex>

namespace Game::Util {
    template<typename K, typename V>
    class ThreadSafeUMap {
    private:
        std::unordered_map<K, V> map;
        mutable std::mutex mtx;

    public:
        // Check if a key exists
        bool contains(const K& key) const {
            std::lock_guard<std::mutex> lock(mtx);
            return map.find(key) != map.end();
        }

        // Insert a key-value pair
        void insert(const K& key, const V& value) {
            std::lock_guard<std::mutex> lock(mtx);
            map[key] = value;
        }

        // Retrieve a value (throws if key does not exist)
        V get(const K& key) const {
            std::lock_guard<std::mutex> lock(mtx);
            return map.at(key);
        }

        // Erase a key
        void erase(const K& key) {
            std::lock_guard<std::mutex> lock(mtx);
            map.erase(key);
        }

        // Access the internal map for custom operations
        template<typename Func>
        void access(Func&& func) {
            std::lock_guard<std::mutex> lock(mtx);
            func(map);
        }
    };
}
