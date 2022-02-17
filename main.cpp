#include <iostream>
#include <chrono>
#include <vector>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

#define TIMESCALE std::chrono::microseconds

inline TIMESCALE MemcpyTest(vector<int> &a, vector<ivec4> &b)
{
    assert(sizeof(ivec4) == 4 * sizeof(int));//, "The memory of ivec4 is no longer aligned with 4 * int")

    auto start = chrono::high_resolution_clock::now();

    // 1000 rows, 4000 columns in a
    const int cols = 4000;
    const int rows = 1000;

    for(int i = 0; i < rows; ++i)
    {
        memcpy(b.data() + i * cols/4, a.data() + (rows-1-i) * cols, cols * sizeof(int));
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

    const int cols = 4000;
    const int rows = 1000;

    for(int i = rows-1; i >= 0; --i)
    {
        for(int j = 0; j < cols; j+=4)
        {
            int startIndex = (rows - 1 - i) * cols + j;
            b[i * cols/4 + j/4] = ivec4 {
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
    vector<int> a = vector<int>(4'000'000);
    vector<ivec4> b = vector<ivec4>(1'000'000);

    for(int i = 0; i < 4'000'000; ++i)
    {
        a[i] = i;
    }


    auto memcpyDuration = MemcpyTest(a,b);
    cout << "Takes " << memcpyDuration << " us" << endl;
    auto forLoopDuration = ForLoopTest(a,b);
    cout << "Takes " << forLoopDuration << " us" << endl;

    cout << "Memcpy is " << forLoopDuration.count() / (float)memcpyDuration.count() << "x faster" << endl;

    return 0;
}
