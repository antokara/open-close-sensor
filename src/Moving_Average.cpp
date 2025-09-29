// Moving_Average.cpp
// Implementations for Moving_Average

#include "Moving_Average.h"
#include <stdlib.h>

Moving_Average::Moving_Average(size_t window_size)
    : capacity_(window_size > 0 ? window_size : 1), count_(0), idx_(0), buffer_(nullptr), sum_(0.0f)
{
    buffer_ = new float[capacity_];
    for (size_t i = 0; i < capacity_; ++i)
        buffer_[i] = 0.0f;
}

Moving_Average::~Moving_Average()
{
    delete[] buffer_;
}

void Moving_Average::reset()
{
    for (size_t i = 0; i < capacity_; ++i)
        buffer_[i] = 0.0f;
    count_ = 0;
    idx_ = 0;
    sum_ = 0.0f;
}

void Moving_Average::add(float value)
{
    if (count_ < capacity_)
    {
        // window not yet full
        buffer_[idx_] = value;
        sum_ += value;
        ++count_;
        idx_ = (idx_ + 1) % capacity_;
    }
    else
    {
        // overwrite oldest
        float &slot = buffer_[idx_];
        sum_ += value - slot;
        slot = value;
        idx_ = (idx_ + 1) % capacity_;
    }
}

float Moving_Average::get() const
{
    if (count_ == 0)
        return 0.0f;
    return sum_ / static_cast<float>(count_);
}

size_t Moving_Average::capacity() const { return capacity_; }
size_t Moving_Average::count() const { return count_; }
bool Moving_Average::isFull() const { return count_ == capacity_; }
