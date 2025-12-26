#include "XYZPointList.h"
#include <cmath>
#include <cstring>

XYZPointList::XYZPointList(size_t maxPoints)
    : m_x(nullptr), m_y(nullptr), m_z(nullptr), m_head(0), m_count(0), m_capacity(maxPoints)
{
    if (m_capacity == 0)
    {
        m_x = m_y = m_z = nullptr;
        return;
    }
    m_x = new float[m_capacity];
    m_y = new float[m_capacity];
    m_z = new float[m_capacity];
    // Optional: initialize memory
    std::memset(m_x, 0, m_capacity * sizeof(float));
    std::memset(m_y, 0, m_capacity * sizeof(float));
    std::memset(m_z, 0, m_capacity * sizeof(float));
}

XYZPointList::~XYZPointList()
{
    delete[] m_x;
    delete[] m_y;
    delete[] m_z;
}

void XYZPointList::add(float x, float y, float z)
{
    if (m_capacity == 0)
        return;
    m_x[m_head] = x;
    m_y[m_head] = y;
    m_z[m_head] = z;
    m_head = (m_head + 1) % m_capacity;
    if (m_count < m_capacity)
        ++m_count;
}

void XYZPointList::clear()
{
    m_head = 0;
    m_count = 0;
}

size_t XYZPointList::oldestIndex() const
{
    return (m_count < m_capacity) ? 0 : m_head;
}

int XYZPointList::find(float x, float y, float z, size_t startIndex, float threshold) const
{
    if (startIndex >= m_count || m_capacity == 0)
        return -1;
    float thr2 = threshold * threshold;
    size_t oldest = oldestIndex();
    for (size_t i = startIndex; i < m_count; ++i)
    {
        size_t phys = (oldest + i) % m_capacity;
        float dx = m_x[phys] - x;
        float dy = m_y[phys] - y;
        float dz = m_z[phys] - z;
        float dist2 = dx * dx + dy * dy + dz * dz;
        if (dist2 <= thr2)
            return (int)i;
    }
    return -1;
}

size_t XYZPointList::size() const
{
    return m_count;
}

size_t XYZPointList::capacity() const
{
    return m_capacity;
}

void XYZPointList::get(size_t index, float &x, float &y, float &z) const
{
    if (index >= m_count || m_capacity == 0)
    {
        x = y = z = 0.0f;
        return;
    }
    size_t phys = (oldestIndex() + index) % m_capacity;
    x = m_x[phys];
    y = m_y[phys];
    z = m_z[phys];
}
