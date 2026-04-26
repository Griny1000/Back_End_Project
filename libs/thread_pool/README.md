#ThreadPool

Простая, но эффективная реализация пула потоков на C++17. Поддерживает задачи с возвращаемым значением через `std::future`, автоматическое управление потоками и graceful shutdown.

## Возможности

- Фиксированное количество рабочих потоков (по умолчанию `std::thread::hardware_concurrency()`).
- Постановка задач с любыми аргументами и возвращаемым значением.
- Получение результата через `std::future`.
- Корректная обработка исключений, брошенных внутри задачи.
- Безопасное завершение: деструктор дожидается окончания всех задач.

## Требования

- Компилятор с поддержкой C++17 (GCC 7+, Clang 5+, MSVC 2017+).
- CMake 3.15+.

## Сборка и подключение

### Как часть большего проекта (рекомендуется)

```bash
git clone https://github.com/Griny1000/Back_End_Project.git
cd Back_End_Project
mkdir build && cd build
cmake ..
make
```
### Отдельная сборка ThreadPool

```bash
cd libs/thread_pool
mkdir build && cd build
cmake ..
make
```
### Базовый пример: задача без возвращаемого значения

```cpp
#include "ThreadPool.hpp"

int main() {
    ThreadPool pool(4);
    for (int i = 0; i < 10; ++i) {
        pool.enqueue([i]() {
            std::cout << "Task " << i << " executed by thread " 
                      << std::this_thread::get_id() << '\n';
        });
    }
    // Деструктор pool дождётся завершения всех задач
    return 0;
}
```

### Пример с получением результата
```cpp
#include "ThreadPool.hpp"
#include <iostream>

int main() {
    ThreadPool pool(4);
    auto future = pool.enqueue([](int a, int b) { return a + b; }, 5, 7);
    std::cout << "Result: " << future.get() << '\n'; // 12
    return 0;
}
```

### Обработка исключений
```cpp
auto bad_future = pool.enqueue([]() {
    throw std::runtime_error("Oops");
    return 0;
});
try {
    bad_future.get();
} catch (const std::exception& e) {
    std::cerr << "Caught: " << e.what() << '\n';
}
```
## Тестирование

### Проект использует Google Test для модульных тестов. Запустите:
```bash
cd build
ctest
```

## Бенчмарки
### Для измерения производительности используется Google Benchmark. Соберите и запустите:

```bash
cd build
make thread_pool_benchmark
./libs/thread_pool/thread_pool_benchmark
```
## Лицензия

Этот проект распространяется под лицензией MIT. 

