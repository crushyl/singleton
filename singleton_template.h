#pragma once
#include <atomic>

template<typename T>
class SingletonTemplate {
 public:
  struct Init {
    Init() {
      auto& count = refCount();
      auto ori = count.fetch_add(1);
      if (ori == 0) {
        T* ptr = SingletonTemplate::Ptr();
        new (ptr) T();
      }
    }

    ~Init() {
      auto& count = refCount();
      auto ori = count.fetch_sub(1);
      if (ori == 1) {
        T* ptr = SingletonTemplate::Ptr();
        ptr->~T();
      }
    }

    static auto& refCount() {
      static std::atomic<unsigned> count{0};
      return count;
    }

    Init(const Init&) = delete;
    Init& opreator= (const Init&) = delete;
  };
 protected:
  SingletonTemplate() = default;
  ~SingletonTemplate() = default;
  SingletonTemplate(const SingletonTemplate&) = delete;
  SingletonTemplate& operator= (const SingletonTemplate&) = delete;

 public:
  static T* Ptr() {
    alignas(T) static char singBuf[sizeof(T)];
    return reinterpret_cast<T*>(sing_buf);
  }
};