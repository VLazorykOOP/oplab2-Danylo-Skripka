#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <thread>
#include <chrono>
#include <mutex>

// Оголошення класу Car для представлення машини
class Car {
public:
    float x, y;           // Поточні координати машини
    float targetX, targetY; // Кінцева точка, до якої рухається машина
    float speed;          // Швидкість машини
    bool isMoving;        // Статус руху машини

    // Конструктор для ініціалізації об'єкта машини
    Car(float startX, float startY, float speed, float targetX, float targetY)
        : x(startX), y(startY), speed(speed), targetX(targetX), targetY(targetY), isMoving(true) {}

    // Метод для переміщення машини до цільової точки
    void move() {
        if (isMoving) {
            float directionX = targetX - x;          // Обчислення напрямку по осі X
            float directionY = targetY - y;          // Обчислення напрямку по осі Y
            float length = std::sqrt(directionX * directionX + directionY * directionY); // Довжина вектора напрямку
            if (length <= speed) {                   // Якщо відстань до цілі менше або дорівнює швидкості
                x = targetX;                         // Задаємо кінцеву координату X
                y = targetY;                         // Задаємо кінцеву координату Y
                isMoving = false;                    // Зупиняємо машину
            } else {
                x += directionX / length * speed;    // Переміщуємо машину по осі X
                y += directionY / length * speed;    // Переміщуємо машину по осі Y
            }
        }
    }
};

// Функція для генерації випадкового числа в діапазоні [min, max]
float getRandomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

// Функція для симуляції руху машини
void simulateCarMovement(Car& car, std::mutex& io_mutex) {
    while (car.isMoving) {
        car.move();
        {
            std::lock_guard<std::mutex> lock(io_mutex); // Блокування для захисту виводу
            std::cout << "Car at (" << car.x << ", " << car.y << ")\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Імітація затримки
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));           // Ініціалізація генератора випадкових чисел

    const float w = 100.0f;                          // Ширина області симуляції
    const float h = 100.0f;                          // Висота області симуляції
    const float speed = 1.0f;                        // Швидкість машин

    std::vector<Car> trucks;                         // Вектор для зберігання вантажівок
    std::vector<Car> cars;                           // Вектор для зберігання легкових машин

    // Генерація вантажівок у верхній лівій чверті
    for (int i = 0; i < 10; ++i) {
        float startX = getRandomFloat(0, w / 2);     // Випадкова початкова координата X
        float startY = getRandomFloat(0, h / 2);     // Випадкова початкова координата Y
        float targetX = getRandomFloat(0, w / 2);    // Випадкова цільова координата X
        float targetY = getRandomFloat(0, h / 2);    // Випадкова цільова координата Y
        trucks.emplace_back(startX, startY, speed, targetX, targetY); // Додаємо вантажівку у вектор
    }

    // Генерація легкових машин у нижній правій чверті
    for (int i = 0; i < 10; ++i) {
        float startX = getRandomFloat(w / 2, w);     // Випадкова початкова координата X
        float startY = getRandomFloat(h / 2, h);     // Випадкова початкова координата Y
        float targetX = getRandomFloat(w / 2, w);    // Випадкова цільова координата X
        float targetY = getRandomFloat(h / 2, h);    // Випадкова цільова координата Y
        cars.emplace_back(startX, startY, speed, targetX, targetY); // Додаємо легкову машину у вектор
    }

    // Вектор потоків
    std::vector<std::thread> threads;
    std::mutex io_mutex; // М'ютекс для синхронізації виводу

    // Запуск потоків для вантажівок
    for (auto& truck : trucks) {
        threads.emplace_back(simulateCarMovement, std::ref(truck), std::ref(io_mutex));
    }

    // Запуск потоків для легкових машин
    for (auto& car : cars) {
        threads.emplace_back(simulateCarMovement, std::ref(car), std::ref(io_mutex));
    }

    // Очікування завершення всіх потоків
    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "Simulation ended. All cars and trucks have stopped.\n"; // Повідомлення про закінчення симуляції

    return 0; // Завершення програми
}
