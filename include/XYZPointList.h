#ifndef XYZ_POINT_LIST_H
#define XYZ_POINT_LIST_H

#include <cstddef>

class XYZPointList
{
public:
    // Construct with maximum number of points to store (must be > 0)
    explicit XYZPointList(size_t maxPoints = 30);
    ~XYZPointList();

    XYZPointList(const XYZPointList &) = delete;
    XYZPointList &operator=(const XYZPointList &) = delete;

    // Add a point (x,y,z) into the list. Overwrites oldest when full.
    void add(float x, float y, float z);

    // Remove all stored points.
    void clear();

    // Find a point matching (x,y,z) with fuzzy threshold starting at startIndex.
    // Returns the logical index (0..size()-1) if found, otherwise -1.
    int find(float x, float y, float z, size_t startIndex, float threshold) const;

    // Number of stored points (<= capacity()).
    size_t size() const;

    // Capacity (max number of points)
    size_t capacity() const;

    // Get point by logical index (0 = oldest). If index out of range, returns zeros.
    void get(size_t index, float &x, float &y, float &z) const;

private:
    float *m_x;
    float *m_y;
    float *m_z;
    size_t m_head;     // next write position (physical index)
    size_t m_count;    // number stored
    size_t m_capacity; // allocated capacity

    size_t oldestIndex() const;
};

#endif // XYZ_POINT_LIST_H
