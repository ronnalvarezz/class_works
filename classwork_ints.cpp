#include <cstddef>
#include <limits>
#include <stdexcept>
#include <utility>

struct p_t {
    int x;
    int y;
};

class Ints {
public:
    Ints();
    Ints(size_t size, int value);
    Ints(const int* arr, size_t arr_size);
    Ints(const Ints& rhs);
    Ints(const size_t size);
    Ints(Ints&& rhs) noexcept;

    Ints& operator=(const Ints& rhs);
    Ints& operator=(Ints&& rhs) noexcept;
    Ints operator+(const Ints& rhs) const;

    ~Ints();

    size_t getSize() const noexcept;

    void set(size_t id, int i);
    int get(size_t id) const noexcept;

    int getSafe(size_t id) const;
    void setSafe(size_t id, int i);

    Ints append(int i) const;
    Ints append(const Ints& b) const;

private:
    size_t size;
    size_t capacity;
    int* data;
};


Ints::Ints():
    capacity(0), size(0), data(nullptr)
{}

Ints::Ints(const size_t size, const int value):
    capacity(size), size(size), data(new int[size])
{
    for (size_t i = 0; i < size; i++) {
        data[i] = value;
    }
}

Ints::Ints(const int* arr, const size_t arr_size):
    size(arr_size), capacity(arr_size), data(nullptr)
{
    if (arr_size > 0 && arr == nullptr) {
        throw std::invalid_argument("Null array");
    }

    data = new int[capacity];

    for (size_t i = 0; i < size; i++) {
        data[i] = arr[i];
    }
}

Ints::Ints(const size_t size):
    size(size), capacity(size), data(new int[size])
{}

Ints::Ints(const Ints& rhs):
    size(rhs.size), capacity(rhs.capacity), data(new int[capacity])
{
    for (size_t i = 0; i < size; i++) {
        data[i] = rhs.data[i];
    }
}

Ints::Ints(Ints&& rhs) noexcept:
    size(rhs.size), capacity(rhs.capacity), data(rhs.data)
{
    rhs.size = 0;
    rhs.capacity = 0;
    rhs.data = nullptr;
}

Ints& Ints::operator=(const Ints& rhs)
{
    if (this == &rhs) {
        return *this;
    }

    Ints temp(rhs);

    std::swap(size, temp.size);
    std::swap(capacity, temp.capacity);
    std::swap(data, temp.data);

    return *this;
}

Ints& Ints::operator=(Ints&& rhs) noexcept
{
    if (this != &rhs) {
        delete[] data;

        size = rhs.size;
        capacity = rhs.capacity;
        data = rhs.data;

        rhs.size = 0;
        rhs.capacity = 0;
        rhs.data = nullptr;
    }
    return *this;
}

Ints Ints::operator+(const Ints& rhs) const
{
    if (size != rhs.size) {
        throw std::invalid_argument("Arrays have different sizes");
    }
    Ints result(size, 0);
    for (size_t i = 0; i < size; i++) {
        long long sum = static_cast<long long>(data[i]) + rhs.data[i];
        if (sum > std::numeric_limits<int>::max() ||
            sum < std::numeric_limits<int>::min()) {
            throw std::overflow_error("Overflow");
        }
        result.data[i] = static_cast<int>(sum);
    }

    return result;
}

Ints::~Ints()
{
    delete[] data;
}

void Ints::set(const size_t id, const int i)
{
    data[id] = i;
}

int Ints::get(const size_t id) const noexcept
{
    return data[id];
}

void Ints::setSafe(const size_t id, const int i)
{
    if (id >= size) {
        throw std::logic_error("Out of range");
    }
    set(id, i);
}

int Ints::getSafe(const size_t id) const
{
    if (id >= size) {
        throw std::logic_error("Out of range");
    }
    return get(id);
}

size_t Ints::getSize() const noexcept
{
    return size;
}

Ints Ints::append(const int i) const
{
    if (size == std::numeric_limits<size_t>::max()) {
        throw std::overflow_error("Overflow");
    }
    Ints temp(size + 1);
    for (size_t p = 0; p < size; p++) {
        temp.data[p] = data[p];
    }
    temp.data[size] = i;
    return temp;
}

Ints Ints::append(const Ints& b) const
{
    if (b.size > std::numeric_limits<size_t>::max() - size) {
        throw std::overflow_error("Overflow");
    }
    Ints temp(size + b.size);
    for (size_t i = 0; i < size; i++) {
        temp.data[i] = data[i];
    }
    for (size_t i = 0; i < b.size; i++) {
        temp.data[size + i] = b.data[i];
    }
    return temp;
}

class Points {
    Ints data_points;

public:
    Points() :
        data_points()
    {}

    Points(const Ints& data) :
        data_points(data)
    {
        if (data.getSize() % 2 != 0) {
            throw std::invalid_argument("Wrong points data");
        }
    }

    ~Points() = default;

    void set(const size_t id, const p_t p)
    {
        data_points.set(id * 2, p.x);
        data_points.set(id * 2 + 1, p.y);
    }

    p_t get(const size_t id) const
    {
        p_t res;
        res.x = data_points.get(id * 2);
        res.y = data_points.get(id * 2 + 1);
        return res;
    }

    Points append(const p_t p) const
    {
        size_t const size = data_points.getSize();
        if (size > std::numeric_limits<size_t>::max() - 2) {
            throw std::overflow_error("Overflow");
        }
        Ints tmp = data_points.append(p.x).append(p.y);
        return Points(tmp);
    }
};

class Mtx {
public:
    Mtx();
    Mtx(size_t m, size_t n);
    Mtx(size_t m, size_t n, const Ints& data);

    ~Mtx() = default;

    Mtx sum(const Mtx& rhs) const;
    Mtx mul(const Mtx& rhs) const;

    Ints getCol(size_t id) const;
    Ints getRow(size_t id) const;

    Mtx addRow(const int pasta) const;

    Mtx transpose() const;
    int det() const;
private:
    size_t m;
    size_t n;
    Ints data_mtx;
};

Mtx::Mtx() :
    m(0), n(0), data_mtx()
{}

Mtx::Mtx(const size_t m, const size_t n):
    m(m), n(n), data_mtx()
{
    if (n != 0 && m > std::numeric_limits<size_t>::max() / n) {
        throw std::overflow_error("Matrix size overflow");
    }
    data_mtx = Ints(m * n, 0);
}

Mtx::Mtx(const size_t m, const size_t n, const Ints& data):
    m(m), n(n), data_mtx(data)
{
    if (n != 0 && m > std::numeric_limits<size_t>::max() / n) {
        throw std::overflow_error("Matrix size overflow");
    }
    if (data.getSize() != m * n) {
        throw std::invalid_argument("Wrong matrix size");
    }
}

Mtx Mtx::sum(const Mtx& rhs) const
{
    if (m != rhs.m || n != rhs.n) {
        throw std::invalid_argument("Different matrix sizes");
    }
    Mtx result(m, n);
    for (size_t i = 0; i < m * n; i++) {
        long long sum = static_cast<long long>(data_mtx.get(i)) + rhs.data_mtx.get(i);
        if (sum > std::numeric_limits<int>::max() ||
            sum < std::numeric_limits<int>::min()) {
            throw std::overflow_error("Overflow");
        }
        result.data_mtx.set(i, static_cast<int>(sum));
    }
    return result;
}

Ints Mtx::getCol(const size_t id) const
{
    if (id >= n) {
        throw std::logic_error("Out of range");
    }
    Ints res;
    for (size_t i = 0; i < m; i++) {
        res = res.append(data_mtx.get(i * n + id));
    }
    return res;
}

Ints Mtx::getRow(const size_t id) const
{
    if (id >= m) {
        throw std::logic_error("Out of range");
    }
    Ints res;
    for (size_t i = 0; i < n; i++) {
        res = res.append(data_mtx.get(id * n + i));
    }
    return res;
}

Mtx Mtx::mul(const Mtx& rhs) const
{
    if (n != rhs.m) {
        throw std::invalid_argument("Wrong matrix sizes");
    }

    Mtx res(m, rhs.n);

    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < rhs.n; j++) {
            long long sm = 0;
            for (size_t k = 0; k < n; k++) {
                sm += static_cast<long long>(data_mtx.get(i * n + k))
                    * rhs.data_mtx.get(k * rhs.n + j);
            }
            if (sm > std::numeric_limits<int>::max() ||
                sm < std::numeric_limits<int>::min()) {
                throw std::overflow_error("Overflow");
            }
            res.data_mtx.set(i * rhs.n + j, static_cast<int>(sm));
        }
    }
    return res;
}

Mtx Mtx::addRow(const int pasta) const
{
    if (m == std::numeric_limits<size_t>::max()) {
        throw std::overflow_error("Overflow");
    }
    Mtx res(m + 1, n);
    for (size_t i = 0; i < m * n; i++) {
        res.data_mtx.set(i, data_mtx.get(i));
    }
    for (size_t i = 0; i < n; i++) {
        res.data_mtx.set(m * n + i, pasta);
    }
    return res;
}

Mtx Mtx::transpose() const
{
    Mtx res(n, m);
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            res.data_mtx.set(j * m + i, data_mtx.get(i * n + j));
        }
    }
    return res;
}

int Mtx::det() const
{
    if (m != n) {
        throw std::logic_error("Matrix must be square");
    }

    if (m == 0) {
        return 0;
    }

    if (m == 1) {
        return data_mtx.get(0);
    }

    if (m == 2) {
        long long res = static_cast<long long>(data_mtx.get(0)) * data_mtx.get(3) - static_cast<long long>(data_mtx.get(1)) * data_mtx.get(2);

        if (res > std::numeric_limits<int>::max() ||
            res < std::numeric_limits<int>::min()) {
            throw std::overflow_error("Determinant overflow");
        }

        return static_cast<int>(res);
    }

    long long res = 0;

    for (size_t i = 0; i < n; i++) {
        Mtx minor(m - 1, n - 1);

        size_t p = 0;

        for (size_t row = 1; row < m; row++) {
            for (size_t col = 0; col < n; col++) {
                if (col == i) {
                    continue;
                }
                minor.data_mtx.set(
                    p++, data_mtx.get(row * n + col)
                );
            }
        }
        int sign = (i % 2 == 0) ? 1 : -1;
        res += static_cast<long long>(sign)
             * data_mtx.get(i)
             * minor.det();
    }
    if (res > std::numeric_limits<int>::max() || res < std::numeric_limits<int>::min()) {
        throw std::overflow_error("Determinant overflow");
    }
    return static_cast<int>(res);
}

class Plot {
public:
    void set(int x, int y);
    int calc(int x) const;
    int solve(Ints& buf, int y) const;
    Points append(p_t p) const;

private:
    Ints data_plot;
};

void Plot::set(const int x, const int y)
{
    for (size_t i = 0; i < data_plot.getSize(); i += 2) {
        if (data_plot.get(i) == x) {
            data_plot.set(i + 1, y);
            return;
        }
    }
    data_plot = data_plot.append(x);
    data_plot = data_plot.append(y);
}

int Plot::calc(const int x) const
{
    for (size_t i = 0; i < data_plot.getSize(); i += 2) {
        if (data_plot.get(i) == x) {
            return data_plot.get(i + 1);
        }
    }
    throw std::logic_error("Point not found");
}

int Plot::solve(Ints& buf, const int y) const
{
    int cnt = 0;

    for (size_t i = 1; i < data_plot.getSize(); i += 2) {
        if (y == data_plot.get(i)) {
            buf = buf.append(data_plot.get(i - 1));
            cnt++;
        }
    }

    return cnt;
}

Points Plot::append(const p_t p) const
{
    Ints tmp = data_plot.append(p.x).append(p.y);

    return Points(tmp);
}

struct Mtxs {
    size_t k;
    Mtx * mtxs;

    Mtxs():
        k(0), mtxs(nullptr)
    {}
    Mtxs(size_t k):
        k(k), mtxs(new Mtx[k])
    {}
    Mtxs(const Mtxs&rhs):
        k(rhs.k), mtxs(nullptr)
    {
        mtxs = new Mtx[k];

        try {
            for (size_t i = 0; i < k; i++) {
                mtxs[i] = rhs.mtxs[i];
            }
        }
        catch (...) {
            delete[] mtxs;
            mtxs = nullptr;
            k = 0;
            throw;
        }
    }

    Mtxs(Mtxs&& rhs) noexcept:
        k(rhs.k), mtxs(rhs.mtxs)
    {
        rhs.k = 0;
        rhs.mtxs = nullptr;
    }

    Mtxs& operator=(const Mtxs& rhs) {
        if (this == &rhs) {
            return *this;
        }
        Mtxs temp(rhs);
        std::swap(k, temp.k);
        std::swap(mtxs, temp.mtxs);

        return *this;
    }

    Mtxs& operator=(Mtxs&& rhs) noexcept {
        if (this != &rhs) {
            delete[] mtxs;
            k = rhs.k;
            mtxs = rhs.mtxs;
            rhs.k = 0;
            rhs.mtxs = nullptr;
        }
        return *this;
    }

    ~Mtxs() {
        delete[] mtxs;
    }
};

Mtx conv(Mtx m, Mtx(*f)(Mtx, Mtx)) {
    Mtx res = m;
    return res;
}

struct Plots { //????
    size_t k;
    Plot plots;
};

Plot calc(...) {} //??????

int main()
{
    int arr[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    Ints a(arr, 9);
    Mtx mtx1(3, 3, a);

    return 0;
}