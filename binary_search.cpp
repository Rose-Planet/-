#include <iostream>
#include <ctime>
#include <algorithm>
#include <windows.h>
#include <fstream> // 引入文件流库
#include <sstream> // 引入字符串流库
#include <cstdlib>  // 用于 rand() 函数
#include <iomanip>  // 用于设置输出格式

using namespace std;

clock_t start_time, end_time;

// 将随机生成的数组写入文件，文件名为 "random_array_n.txt"
void writeRandomArrayToFile(int* a, int n) {
    stringstream filename;
    filename << "random_array_" << n << ".txt";  // 文件名格式为 "random_array_n.txt"

    ofstream outFile(filename.str(), ios::out); // 打开文件
    if (outFile.is_open()) {
        for (int i = 0; i < n; i++) {
            outFile << a[i] << " "; // 写入每个随机生成的元素
        }
        outFile << endl; // 写入换行符
        outFile.close(); // 关闭文件
        cout << "生成的随机数组已写入文件 " << filename.str() << endl;
    } else {
        cout << "无法打开文件写入数据!" << endl;
    }
}

// 生成n个随机数并返回数组
void generateRandomArray(int* a, int n) {
    for (int i = 0; i < n; i++) {
        a[i] = rand(); // 生成随机数
    }
}

// 非递归二分搜索函数
int binarySearchIterative(int* a, int x, int n)
{
    int left = 0, right = n - 1;
    while (left <= right) // 采用分治思想，将数组依次进行二分，用非递归方法搜索x
    {
        Sleep(1000); // 每次执行二分搜索时暂停 1000 毫秒（即 1 秒），用于计时
        int mid = left + (right - left) / 2; // 防止越界
        if (x == a[mid]) 
            return mid; // 找到目标值
        if (x > a[mid]) 
            left = mid + 1; // 调整左边界
        else 
            right = mid - 1; // 调整右边界
    }
    return -1; // 没找到
}

// 递归二分搜索函数
int binarySearchRecursive(int* a, int left, int right, int x)
{
    if (left > right) {
        return -1; // 没找到
    }

    Sleep(1000); // 每次执行二分搜索时暂停 1000 毫秒（即 1 秒）
    int mid = left + (right - left) / 2; // 防止越界

    if (a[mid] == x) {
        return mid; // 找到目标值
    }
    else if (a[mid] < x) {
        return binarySearchRecursive(a, mid + 1, right, x); // 递归查找右半部分
    }
    else {
        return binarySearchRecursive(a, left, mid - 1, x); // 递归查找左半部分
    }
}

// 将搜索时间结果写入CSV文件
void writeResultsToCSV(string filename, int test_size, double timesIterative[5], double timesRecursive[5]) {
    ofstream outFile(filename, ios::app);  // 追加模式写入文件
    if (outFile.is_open()) {
        // 将每次搜索时间写入CSV文件
        outFile << test_size;  // 数据量
        for (int i = 0; i < 5; i++) {
            outFile << "," << fixed << setprecision(4) << timesIterative[i];  // 非递归版本的时间
        }
        for (int i = 0; i < 5; i++) {
            outFile << "," << fixed << setprecision(4) << timesRecursive[i];  // 递归版本的时间
        }
        outFile << endl;  // 换行
        outFile.close();  // 关闭文件
    } else {
        cout << "无法打开文件写入结果!" << endl;
    }
}

// 主函数
int main() {
    int* a; // 使用指针来动态分配内存
    int ans;

    srand(time(NULL)); // 随机数种子

    // 定义测试数据量
    int test_sizes[] = {100, 1000, 10000, 100000, 1000000};

    // 创建并初始化CSV文件，写入表头
    ofstream outFile("binary_search_results.csv");
    if (outFile.is_open()) {
        outFile << "Data Size,Iterative Test 1,Iterative Test 2,Iterative Test 3,Iterative Test 4,Iterative Test 5,Recursive Test 1,Recursive Test 2,Recursive Test 3,Recursive Test 4,Recursive Test 5\n";  // CSV 表头
        outFile.close();
    } else {
        cout << "无法创建CSV文件!" << endl;
        return 1;
    }

    // 遍历每个测试数据量
    for (int i = 0; i < 5; i++) {
        int n = test_sizes[i]; // 当前测试数据量
        cout << "正在测试数据量: " << n << endl;

        // 动态分配数组空间
        a = new int[n];

        // 生成n个随机数并将其存入数组a[]
        generateRandomArray(a, n);

        // 将生成的随机数写入文件，文件名包含数据量n
        writeRandomArrayToFile(a, n);

        // 排序数组，以便进行二分搜索
        sort(a, a + n);

        // 保存五次的搜索时间
        double timesIterative[5];
        double timesRecursive[5];

        // 五次测试
        for (int j = 0; j < 5; j++) {
            // 随机选取一个目标值
            int targetIndex = rand() % n; // 随机选择一个索引
            int target = a[targetIndex]; // 使用该索引作为目标值
            cout << "随机选取的目标值是: " << target << endl;

            // 调用非递归二分搜索
            start_time = clock(); // 设置开始时间
            ans = binarySearchIterative(a, target, n); // 调用非递归二分搜索函数
            end_time = clock(); // 设置结束时间
            double iterative_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
            timesIterative[j] = iterative_time;

            if (ans == -1) // 判断是否找到
                cout << "待查找的元素不存在 (Iterative)" << endl;
            else
                cout << "待查找元素的位置为 (Iterative): " << ans + 1 << endl;

            // 调用递归二分搜索
            start_time = clock(); // 设置开始时间
            ans = binarySearchRecursive(a, 0, n - 1, target); // 调用递归二分搜索函数
            end_time = clock(); // 设置结束时间
            double recursive_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
            timesRecursive[j] = recursive_time;

            if (ans == -1) // 判断是否找到
                cout << "待查找的元素不存在 (Recursive)" << endl;
            else
                cout << "待查找元素的位置为 (Recursive): " << ans + 1 << endl;

            // 为了便于调试，避免每次测试之间的冲突，稍微暂停
            Sleep(500);  // 暂停500毫秒
        }

        // 将该数据量的五次测试结果写入CSV文件
        writeResultsToCSV("binary_search_results.csv", n, timesIterative, timesRecursive);

        // 释放动态分配的内存
        delete[] a;

        cout << endl; // 分隔不同的数据量测试
    }

    cout << "所有测试完成，结果已保存至 'binary_search_results.csv' 文件中。" << endl;

    return 0;
}
