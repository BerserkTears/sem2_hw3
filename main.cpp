#include <iostream>
#include <vector>
#include "iterator"
#include "cstddef"

// Часть 1

template<typename T>
class circular_buffer {
protected:
    T *data_; // Массив
    size_t size_; // Размер
    size_t end_, start_; // Указатели на конец и начало
public:
    explicit circular_buffer(size_t size = 1, const T &value = 0)
            : size_(size + 1), data_(new T[size + 1]), end_(0), start_(0) {
        for (size_t i = 0; i < size_; i++)
            data_[i] = value;
    }

    circular_buffer(const circular_buffer &another)
            : size_(another.size_), data_(new T[size_]),
              end_(another.end_), start_(another.start_) {
        for (size_t i; i < size_; i++)
            data_[i] = another.data_[i];
    }

    ~circular_buffer() {
        delete[] data_;
    }

    circular_buffer &operator=(circular_buffer const &another) {
        if (&another != this) {
            size_ = another.size_;
            end_ = another.end_;
            data_ = new T[size_];
            for (size_t i = 0; i < another.size_; i++)
                data_[i] = another.data_[i];
        }
        return *this;
    }

    T &operator[](size_t pos) {
        if (pos >= size_) {
            exit(1);
        }
        return data_[pos];
    }

    const T &operator[](size_t pos) const {
        if (pos >= size_) {
            exit(1);
        }
        return data_[pos];
    }

    size_t size() const {
        return size_;
    }

    void place_end(const T &value) {
        data_[end_] = value;
        end_++;
        if (end_ == size_)
            end_ = 0;
        if (end_ == start_)
            start_++;
        if (start_ == size_)
            start_ = 0;
    }

    void delete_end() {
        if (end_ != start_) {
            end_--;
            data_[end_] = 0;
            if (end_ == -1)
                end_ = size_ - 1;
        }
    }

    void place_start(const T &value) {
        start_--;
        if (start_ == -1) {
            start_ = size_ - 1;
        }
        data_[start_] = value;
        if (start_ == end_) {
            end_--;
            if (end_ == -1)
                end_ = size_ - 1;
        }

    }

    void delete_start() {
        if (end_ != start_) {
            data_[start_] = 0;
            start_++;
            if (start_ == size_)
                start_ = 0;
        }
    }

    class Iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        Iterator(pointer ptr, pointer ptrEnd, pointer ptrStart) :
                ptr_(ptr), ptrEnd_(ptrEnd), ptrStart_(ptrStart) {}

        reference operator*() const { return *ptr_; }

        pointer operator->() { return ptr_; }

        // Prefix increment
        Iterator &operator++() {
            ptr_++;
            if (ptr_ == ptrEnd_) {
                ptr_ = ptrStart_;
            }
            return *this;
        }

        // Postfix increment
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const Iterator &a, const Iterator &b) { return a.ptr_ == b.ptr_; };

        friend bool operator!=(const Iterator &a, const Iterator &b) { return a.ptr_ != b.ptr_; };
    private:
        pointer ptr_;
        pointer ptrEnd_, ptrStart_;
    };

    Iterator begin() {
        return Iterator(&data_[start_], &data_[size_], &data_[0]);
    }

    Iterator end() {
        return Iterator(&data_[end_], &data_[size_], &data_[0]);
    }

    void resize(const size_t &new_size) {
        T *tmp = new T[new_size + 1];
        size_t current = start_;
        size_t capacity;
        if (end_ > start_) {
            capacity = end_ - start_;
        } else {
            capacity = end_ + size_ - start_;
        }
        for (size_t i = 0; i < new_size + 1 && i < capacity; i++, current++) {
            tmp[i] = data_[current];
            if (current == size_) {
                current = 0;
            }
        }
        delete[] data_;
        size_ = new_size + 1;
        data_ = tmp;
    }
};

// Часть 2

void Print_Bool(bool B) {
    if (B)
        std::cout << "True";
    else
        std::cout << "False";
}

template<typename T, typename Pred>
bool all_of(T *begin, T *end, Pred pred) {
    while (begin != end) {
        if (!pred(*begin)) {
            return false;
        }
        begin++;
    }
    return true;
}

template<typename T, typename Pred>
bool any_of(T *begin, T *end, Pred pred) {
    while (begin != end) {
        if (pred(*begin)) {
            return true;
        }
        begin++;
    }
    return false;
}

template<typename T, typename Pred>
bool none_of(T *begin, T *end, Pred pred) {
    while (begin != end) {
        if (pred(*begin)) {
            return false;
        }
        begin++;
    }
    return true;
}

template<typename T, typename Pred>
bool one_of(T *begin, T *end, Pred pred) {
    int count = 0;
    while (begin != end) {
        if (pred(*begin)) {
            count++;
        }
        begin++;
    }
    if (count == 1)
        return true;
    else
        return false;
}

template<typename T, typename Pred>
bool is_sorted(T *begin, T *end, Pred pred) {
    while (begin != end - 1) {
        if (!pred(*begin, *(begin + 1))) {
            return false;
        }
        begin++;
    }
    return true;
}

template<typename T, typename Pred>
bool is_partitioned(T *begin, T *end, Pred pred) {
    int amount = 0;
    while (begin != end - 1) {
        if (pred(*begin) != pred(*(begin + 1))) {
            amount++;
        }
        begin++;
    }
    if (amount == 1)
        return true;
    else
        return false;
}

template<typename T>
T &find_not(T *begin, T *end, T element) {
    while (begin != end) {
        if (*begin != element)
            return &begin;
        begin++;
    }
    return nullptr;
}

template<typename T>
T &find_backwards(T *begin, T *end, T element) {
    while (begin != end) {
        if (*end == element)
            return &end;
        end--;
    }
    return nullptr;
}

template<typename T, typename Pred>
bool is_palindrome(T *begin, T *end, Pred pred) {
    int i = 0;
    while (begin + i < end - i) {
        if (pred(*(begin + i)) != pred(*(end - i))) {
            return false;
        }
        i++;
    }
    return true;
}


int main() {
    circular_buffer<int> a(2);
    a.resize(4);
    a.place_end(1);
//    for (auto aa: a)
//        std::cout << aa << " ";
//    std::cout << std::endl;
    a.place_end(2);
//    for (auto aa: a)
//        std::cout << aa << " ";
//    std::cout << std::endl;
    a.place_end(3);
//    for (auto aa: a)
//        std::cout << aa << " ";
//    std::cout << std::endl;

    a.place_end(4);
//    for (auto aa: a)
//        std::cout << aa << " ";
//    std::cout << std::endl;

    a.place_end(5);
//    for (auto aa: a)
//        std::cout << aa << " ";
//    std::cout << std::endl;
    a.place_end(6);
//    for (auto aa: a)
//        std::cout << aa << " ";
//    std::cout << std::endl;
    a.delete_end();
    a.place_start(10);
    a.place_start(11);
    a.place_start(12);
    a.place_start(13);
    for (auto aa: a)
        std::cout << aa << " ";

    std::vector<int> A(5);
    A[0] = 10;
    A[1] = 9;
    A[2] = 8;
    A[3] = 7;
    A[4] = 6;

    std::cout << std::endl;
    Print_Bool(any_of(A.begin(), A.end(), [](int a) { return a > 0; }));

    std::cout << std::endl;
    Print_Bool(is_sorted(A.begin(), A.end(), [](int a, int b) { return a > b; }));

    return 0;
}
