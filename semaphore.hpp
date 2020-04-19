#include <mutex>
#include <condition_variable>

class Semaphore
{

private:
    std::mutex mutex_;
    std::condition_variable condition_;
    unsigned long long count_;

public:

    Semaphore(){
        count_ =1;

    }
    
    void signal() {
        std::lock_guard<decltype(mutex_)> lock(mutex_);
        ++count_;
        condition_.notify_one();
    }

    void wait() {
        std::unique_lock<decltype(mutex_)> lock(mutex_);
        while(!count_) // Handle spurious wake-ups.
            condition_.wait(lock);
        --count_;
    }

    bool try_wait() {
        std::lock_guard<decltype(mutex_)> lock(mutex_);
        if(count_) {
            --count_;
            return true;
        }
        return false;
    }
};