#include <iostream>

template<typename T>
class circular_buffer {
private:
    T *data_; // Массив
    size_t size_; // Размер
    size_t end_, start_; // Указатели на конец и начало
public:
    explicit circular_buffer(size_t size = 1, const T &value = 0)
            : size_(size), data_(new T[size]), end_(0), start_(0) {
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
        if (pos >= size_){
            exit(1);
        }
        return data_[pos];
    }

    const T &operator[](size_t pos) const {
        if (pos >= size_){
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
    }

    void delete_end() {
        end_--;
        if (end_ == -1)
            end_ = size_ - 1;
        data_[end_] = 0;
    }

    void place_start(const T &value) {
        start_--;
        if (start_ == -1) {
            start_ = size_ - 1;
        }
        data_ = value;
    }

    void delete_start() {
        data_[start_] = 0;
        start_++;
        if (start_ == size_)
            start_ = 0;
    }

    T &start() {
        return data_[start_];
    }

    T &end() {
        return data_[end_];
    }

    void resize(const size_t &new_size) {
        T *tmp = data_;
        delete [] data_;
        data_ = new T[new_size];
        size_t current = start_;
        size_t capacity;
        if (end_ > start_){
            capacity = end_ - start_;
        } else {
            capacity = end_ + size_ - start_;
        }
        for (size_t i = 0; i < new_size && i < capacity; i++, current++) {
            data_[i] = tmp[current];
        }
        delete [] tmp;
    }
};


int main() {
    circular_buffer<int> a(2,1);
    a.resize(4);
    for (size_t i = 0; i < a.size(); i++)
        std::cout << a[i] << " ";
    return 0;
}
