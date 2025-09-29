// Moving_Average.h
// Declaration for a simple moving average window filter for embedded/Arduino use.

#ifndef MOVING_AVERAGE_H
#define MOVING_AVERAGE_H

#include <stddef.h>

class Moving_Average
{
public:
    // Create a moving average with the given window size (must be >= 1).
    explicit Moving_Average(size_t window_size = 10);

    // Non-copyable (avoid accidental heavy copies); movable operations are not provided.
    Moving_Average(const Moving_Average &) = delete;
    Moving_Average &operator=(const Moving_Average &) = delete;

    ~Moving_Average();

    // Reset the filter to empty (all zeros internally).
    void reset();

    // Push a new sample into the window.
    // Maintains running sum and overwrites the oldest sample when full.
    void add(float value);

    // Return the current average. If no samples yet, returns 0.0f.
    float get() const;

    // Return the configured window size (capacity).
    size_t capacity() const;

    // Return the current number of samples stored (<= capacity()).
    size_t count() const;

    // True when the window is full.
    bool isFull() const;

private:
    size_t capacity_;
    size_t count_;
    size_t idx_; // index to write next (oldest element)
    float *buffer_;
    float sum_;
};

#endif // MOVING_AVERAGE_H
