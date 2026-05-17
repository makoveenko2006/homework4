# Використовуємо офіційний образ Ubuntu з комілятором C++
FROM ubuntu:22.04

# Встановлюємо необхідні пакети
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    git \
    && rm -rf /var/lib/apt/lists/*

# Створюємо робочу директорію
WORKDIR /app

# Копіюємо вихідний код
COPY . /app

# Створюємо директорію для збірки
RUN mkdir -p /app/build

# Компілюємо програму
RUN cd /app/build && \
    cmake .. && \
    make

# За замовчуванням запускаємо програму
CMD ["/app/build/collatz"]