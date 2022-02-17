#include <iostream>
#include <chrono>
#include <vector>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

#define TIMESCALE std::chrono::nanoseconds

#define ROWS 100
#define COLS 4'000'000


inline TIMESCALE MemcpyTest(vector<int> &a, vector<ivec4> &b)
{
    assert(sizeof(ivec4) == 4 * sizeof(int));//, "The memory of ivec4 is no longer aligned with 4 * int")

    auto start = chrono::high_resolution_clock::now();

    for(int i = 0; i < ROWS; ++i)
    {
        memcpy(b.data() + i * COLS/4, a.data() + (ROWS-1-i) * COLS, COLS * sizeof(int));
    }

//    for (int i = 0; i < 10; ++i)
//    {
//        cout << b[i].x << " " << b[i].y << " " << b[i].z << " " << b[i].w << endl;
//    }

    auto stop = chrono::high_resolution_clock::now();
    return chrono::duration_cast<TIMESCALE>(stop-start);
}

inline TIMESCALE ForLoopTest(vector<int> &a, vector<ivec4> &b)
{
    auto start = chrono::high_resolution_clock::now();

    for(int i = ROWS-1; i >= 0; --i)
    {
        for(int j = 0; j < COLS; j+=4)
        {
            int startIndex = (ROWS - 1 - i) * COLS + j;
            b[i * COLS/4 + j/4] = ivec4 {
                a[startIndex],
                a[startIndex + 1],
                a[startIndex + 2],
                a[startIndex + 3]
            };
        }
    }

//    for (int i = 0; i < 10; ++i)
//    {
//        cout << b[i].x << " " << b[i].y << " " << b[i].z << " " << b[i].w << endl;
//    }

    auto stop = chrono::high_resolution_clock::now();
    return chrono::duration_cast<TIMESCALE>(stop-start);
}


int main()
{
    vector<int> a = vector<int>(ROWS * COLS);
    vector<ivec4> b = vector<ivec4>(ROWS * COLS / 4);

    for(int i = 0; i < ROWS * COLS; ++i)
    {
        a[i] = i;
    }


    auto memcpyDuration = MemcpyTest(a,b);
    cout << "Memcpy Takes " << memcpyDuration.count() << " ns" << endl;
    auto forLoopDuration = ForLoopTest(a,b);
    cout << "Simple Assignment Takes " << forLoopDuration.count() << " ns" << endl;

    cout << "Memcpy is " << forLoopDuration.count() / (float)memcpyDuration.count() << "x faster" << endl;

    return 0;
}

// Takes 1731 us
// Takes 15753 us
// Memcpy is 9.10052x faster