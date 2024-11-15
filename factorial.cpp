#include <iostream>
#include <ctime>
#include <windows.h> // for Sleep
using namespace std;

// 使用递归计算阶乘
int factorial(int n) {
    Sleep(1000); // 模拟耗时操作, 1秒
    // 基础情况：0! 和 1! 都等于 1
    if (n <= 1) {
        return 1;
    }
    // 递归调用，分治思想计算 n * (n-1)!
    return n * factorial(n - 1);
}

//测试阶乘运行时间
void testFactorial(int n) {
    // 记录开始时间
    clock_t start = clock();
    // 计算阶乘
    unsigned long long result = factorial(n);
    // 记录结束时间
    clock_t end = clock();
    double duration = double(end - start) / CLOCKS_PER_SEC;
    cout << "Factorial of " << n << " is " << result << ", Time taken: " << duration << " seconds" << endl;
}

int main() {
    int n;
    cout << "请输入一个数字来计算它的阶乘: ";
    cin >> n;

    if (n < 0) {
        cout << "阶乘没有为负数定义." << endl;
    } else {
        unsigned long long result = factorial(n);
        cout  << n << " 的阶乘是 " << result << endl;
        testFactorial(n); // 测试阶乘运行时间
    }
    return 0;
}
