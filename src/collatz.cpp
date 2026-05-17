//Makoveienko Veronika KI-43
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include <iomanip>
#include <mutex>

class CollatzParallel {
private:
    const long long TOTAL_NUMBERS = 10'000'000;
    std::atomic<long long> totalSteps{0};
    
    int collatzSteps(long long n) {
        int steps = 0;
        // Додаємо захист від переповнення
        while (n != 1 && steps < 10000) {
            if (n % 2 == 0) {
                n = n / 2;
            } else {
                // Перевірка на переповнення
                if (n > (LLONG_MAX - 1) / 3) {
                    return steps; // Безпечний вихід
                }
                n = 3 * n + 1;
            }
            steps++;
        }
        return steps;
    }
    
    void processRange(int start, int end) {
        long long localSteps = 0;
        for (int i = start; i <= end; i++) {
            localSteps += collatzSteps(i);
        }
        totalSteps += localSteps;
    }
    
public:
    void run(int numThreads) {
        std::cout << "\n========================================" << std::endl;
        std::cout << "   ГІПОТЕЗА КОЛАТЦА - ПАРАЛЕЛЬНІ ОБЧИСЛЕННЯ" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Числовий діапазон: 1 .. " << TOTAL_NUMBERS << std::endl;
        std::cout << "Кількість потоків: " << numThreads << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        
        auto startTime = std::chrono::steady_clock::now();
        
        // Розподіл роботи між потоками
        std::vector<std::thread> threads;
        int numbersPerThread = TOTAL_NUMBERS / numThreads;
        int remainder = TOTAL_NUMBERS % numThreads;
        
        int currentStart = 1;
        for (int i = 0; i < numThreads; i++) {
            int currentEnd = currentStart + numbersPerThread - 1;
            if (i < remainder) currentEnd++;
            
            threads.emplace_back(&CollatzParallel::processRange, this, currentStart, currentEnd);
            currentStart = currentEnd + 1;
        }
        
        // Очікування завершення всіх потоків
        for (auto& thread : threads) {
            thread.join();
        }
        
        auto endTime = std::chrono::steady_clock::now();
        auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        
        double averageSteps = (double)totalSteps / TOTAL_NUMBERS;
        
        std::cout << "\nРЕЗУЛЬТАТИ ОБЧИСЛЕНЬ:" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Час виконання:        " << elapsedMs << " мс" << std::endl;
        std::cout << "Всього оброблено:     " << TOTAL_NUMBERS << " чисел" << std::endl;
        std::cout << "Сумарна кількість кроків: " << totalSteps << std::endl;
        std::cout << "Середня кількість кроків: " << std::fixed << std::setprecision(2) << averageSteps << std::endl;
        std::cout << "========================================" << std::endl;
        
        // Демонстрація для перших чисел
        std::cout << "\nПеревірка (перші 10 чисел):" << std::endl;
        for (int i = 1; i <= 10; i++) {
            std::cout << "  " << std::setw(3) << i << " -> " 
                      << std::setw(3) << collatzSteps(i) << " кроків" << std::endl;
        }
        std::cout << std::endl;
    }
};

int main() {
    // Автоматичне визначення кількості потоків
    int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;
    
    try {
        CollatzParallel calculator;
        calculator.run(numThreads);
    } catch (const std::exception& e) {
        std::cerr << "Помилка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}