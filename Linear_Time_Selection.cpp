#include <iostream>
#include <ctime>
#include <algorithm>
#include <windows.h>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <sstream>

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

// 分区操作
int partition(int* a, int left, int right, int pivotIndex) {
    int pivotValue = a[pivotIndex];
    // 将pivot值移动到数组的最右边
    swap(a[pivotIndex], a[right]);
    int storeIndex = left;
    
    // 对数组进行分区，所有小于pivotValue的元素移到左边，大于pivotValue的移到右边
    for (int i = left; i < right; i++) {
        if (a[i] < pivotValue) {
            swap(a[i], a[storeIndex]);
            storeIndex++;
        }
    }
    
    // 将pivot值移到正确的位置
    swap(a[storeIndex], a[right]);
    return storeIndex;
}

// 找到数组中第k小的元素，使用二次取中方法
int medianOfMedians(int* a, int left, int right) {
    int n = right - left + 1;

    // 如果数组只有五个或更少元素，直接排序并返回中位数
    if (n <= 5) {
        sort(a + left, a + right + 1);
        return (left + right) / 2;
    }

    // 将数组分为大小为5的小组，并对每个小组排序，找到每个小组的中位数
    for (int i = 0; i < n / 5; i++) {
        int subLeft = left + i * 5;
        int subRight = subLeft + 4;
        sort(a + subLeft, a + subRight + 1);
        swap(a[left + i], a[subLeft + 2]); // 将每个小组的中位数放到数组前面
    }

    // 处理剩余的小组（如果有的话）
    int numMedians = (n + 4) / 5;
    if (numMedians == 1) {
        return left + (n / 2); // 只有一个中位数
    } else {
        return medianOfMedians(a, left, left + numMedians - 1); // 递归寻找中位数的中位数
    }
}

// 线性时间选择算法（通过二次取中）
int linearSelect(int* a, int left, int right, int k) {
    if (left == right) {
        return a[left]; // 如果只有一个元素，直接返回
    }

    // 计算二次取中的枢轴位置
    int pivotIndex = medianOfMedians(a, left, right);
    
    // 每次计算中位数后暂停1000毫秒
    Sleep(1000);  // 每次暂停1秒

    // 将枢轴元素移动到末尾
    pivotIndex = partition(a, left, right, pivotIndex);

    // 找到第k小元素
    if (k == pivotIndex) {
        return a[k];
    } else if (k < pivotIndex) {
        return linearSelect(a, left, pivotIndex - 1, k);
    } else {
        return linearSelect(a, pivotIndex + 1, right, k);
    }
}

// 将搜索时间结果写入CSV文件
void writeResultsToCSV(string filename, int test_size, double times[5]) {
    ofstream outFile(filename, ios::app);  // 追加模式写入文件
    if (outFile.is_open()) {
        // 将每次搜索时间写入CSV文件
        outFile << test_size;  // 数据量
        for (int i = 0; i < 5; i++) {
            outFile << "," << fixed << setprecision(4) << times[i];  // 时间
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
    int test_sizes[] = {100000, 1000000, 10000000, 100000000};

    // 创建并初始化CSV文件，写入表头
    ofstream outFile("linear_select_results.csv");
    if (outFile.is_open()) {
        outFile << "Data Size,Test 1,Test 2,Test 3,Test 4,Test 5\n";  // CSV 表头
        outFile.close();
    } else {
        cout << "无法创建CSV文件!" << endl;
        return 1;
    }

    // 遍历每个测试数据量
    for (int i = 0; i < 4; i++) {
        int n = test_sizes[i]; // 当前测试数据量
        cout << "正在测试数据量: " << n << endl;

        // 动态分配数组空间
        a = new int[n];

        // 生成n个随机数并将其存入数组a[]
        generateRandomArray(a, n);

        // 将生成的随机数写入文件，文件名包含数据量n
        writeRandomArrayToFile(a, n);

        // 排序数组，以便进行二次取中选择
        sort(a, a + n);

        // 保存五次的选择时间
        double times[5];

        // 五次测试
        for (int j = 0; j < 5; j++) {
            // 随机选择第k小元素
            int k = rand() % n; // 随机选择一个索引作为目标元素的排名（0到n-1）
            cout << "正在寻找第 " << k + 1 << " 小的元素..." << endl;

            // 调用线性时间选择算法
            start_time = clock(); // 设置开始时间
            ans = linearSelect(a, 0, n - 1, k); // 调用线性时间选择算法
            end_time = clock(); // 设置结束时间
            double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
            times[j] = time_taken;

            cout << "第 " << k + 1 << " 小的元素为: " << ans << endl;

            // 为了便于调试，避免每次测试之间的冲突，稍微暂停
            Sleep(500);  // 暂停500毫秒
        }

        // 将该数据量的五次测试结果写入CSV文件
        writeResultsToCSV("linear_select_results.csv", n, times);

        // 释放动态分配的内存
        delete[] a;

        cout << endl; // 分隔不同的数据量测试
    }

    cout << "所有测试完成，结果已保存至 'linear_select_results.csv' 文件中。" << endl;

    return 0;
}
