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
        return data_[(pos + start_) % size_];
    }

    const T &operator[](size_t pos) const {
        return data_[(start_ + pos) % size_];
    }

    size_t size() const {
        size_t currentSize;
        if (end_ > start_) {
            currentSize = end_ - start_;
        } else {
            currentSize = end_ + size_ - start_;
        }
        return currentSize;
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

        Iterator &operator++() {
            ptr_++;
            if (ptr_ == ptrEnd_) {
                ptr_ = ptrStart_;
            }
            return *this;
        }

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
        std::cout << "True" << std::endl;
    else
        std::cout << "False" << std::endl;
}

template<typename T, typename Pred>
bool all_of(T begin, T end, Pred pred) {
    while (begin != end) {
        if (!pred(*begin)) {
            return false;
        }
        begin++;
    }
    return true;
}

template<typename T, typename Pred>
bool any_of(T begin, T end, Pred pred) {
    while (begin != end) {
        if (pred(*begin)) {
            return true;
        }
        begin++;
    }
    return false;
}

template<typename T, typename Pred>
bool None_of(T begin, T end, Pred pred) {
    while (begin != end) {
        if (pred(*begin)) {
            return false;
        }
        begin++;
    }
    return true;
}

template<typename T, typename Pred>
bool one_of(T begin, T end, Pred pred) {
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
bool is_sorted(T begin, T end, Pred pred) {
    while (begin != end - 1) {
        if (!pred(*begin, *(begin + 1))) {
            return false;
        }
        begin++;
    }
    return true;
}

template<typename T, typename Pred>
bool is_partitioned(T begin, T end, Pred pred) {
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

template<typename T, typename U>
constexpr T find_not(T begin, T end, const U &element) {
    while (begin != end) {
        if (*begin != element)
            return begin;
        begin++;
    }
    return end;
}

template<typename T, typename U>
constexpr T find_backwards(T begin, T end, const U &element) {
    T it = end - 1;
    while (begin != it) {
        if (*end == element)
            return it;
        it--;
    }
    return end;
}

template<typename T, typename Pred>
bool is_palindrome(T begin, T end, Pred pred) {
    int i = 0;
    while (begin + i < end - i) {
        if (!pred(*(begin + i), *(end - 1 - i))) {
            return false;
        }
        i++;
    }
    return true;
}

template<typename T>
void Print_Arr(T &arr) {
    for (auto aa: arr)
        std::cout << aa << " ";
    std::cout << std::endl;
}

struct Student {
    std::string surname;
    int grade;
};

int main() {
    circular_buffer<int> a(2);
    a.resize(4);

    a.place_end(1);
    Print_Arr(a);

    a.place_end(2);
    Print_Arr(a);

    a.place_end(3);
    Print_Arr(a);

    a.place_end(4);
    Print_Arr(a);

    a.place_end(5);
    Print_Arr(a);

    a.place_end(6);
    Print_Arr(a);

    a.delete_end();
    Print_Arr(a);

    a.place_start(10);
    Print_Arr(a);

    a.place_start(11);
    Print_Arr(a);

    a.place_start(12);
    Print_Arr(a);

    a.place_start(13);
    Print_Arr(a);

    std::vector<int> A(5);
    A[0] = 1;
    A[1] = 2;
    A[2] = 3;
    A[3] = -2;
    A[4] = -1;

    std::cout << std::endl;
    Print_Bool(one_of(A.begin(), A.end(), [](int a) { return a == 1; }));


    Print_Bool(is_palindrome(A.begin(), A.end(), [](int a, int b) { return a == -b; }));


    auto result = find_not(A.begin(), A.end(), 1);
    Print_Bool(result != A.end());

    std::vector<Student> students(4);
    students[0] = {"Ivanov", 3};
    students[1] = {"Tsarev", 5};
    students[2] = {"Krylov", 4};
    students[3] = {"Trek", 2};
    Print_Bool(None_of(students.begin(), students.end(),
                       [](const Student &a) { return a.grade == 5; }));

    return 0;
}