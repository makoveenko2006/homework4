cmake_minimum_required(VERSION 3.10)
project(CollatzHypothesis)

# Встановлюємо стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Оптимізація для продакшену
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -pthread")

# Додаємо виконуваний файл
add_executable(collatz src/collatz.cpp)

# Підключаємо pthread
find_package(Threads REQUIRED)
target_link_libraries(collatz Threads::Threads)

# Встановлюємо властивості
set_target_properties(collatz PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}"
)