#ifndef _SINGLETON_HPP_
#define _SINGLETON_HPP_

template <typename T>
class Singleton {
public:
    static T& get_instance() {
        static T instance;
        return instance;
    }
private:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton(const Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(const Singleton&&) = delete;
};

#endif //!_SINGLETON_HPP_
