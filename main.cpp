#include <iostream>   // Підключення бібліотеки для вводу/виводу
#include <vector>     // Підключення бібліотеки для використання контейнеру vector
#include <cstdlib>    // Підключення бібліотеки для генерації випадкових чисел
#include <ctime>      // Підключення бібліотеки для роботи з часом
#include <cmath>      // Підключення бібліотеки для математичних операцій
#include <thread>     // Підключення бібліотеки для роботи з потоками (для імітації затримки)
#include <chrono>     // Підключення бібліотеки для роботи з часом (для імітації затримки)

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
        if (startX == targetX && startY == targetY) { // Якщо початкова точка співпадає з цільовою
            targetX = startX;                        // Залишаємо початкову координату X
            targetY = startY;                        // Залишаємо початкову координату Y
        }
        trucks.emplace_back(startX, startY, speed, targetX, targetY); // Додаємо вантажівку у вектор
    }

    // Генерація легкових машин у нижній правій чверті
    for (int i = 0; i < 10; ++i) {
        float startX = getRandomFloat(w / 2, w);     // Випадкова початкова координата X
        float startY = getRandomFloat(h / 2, h);     // Випадкова початкова координата Y
        float targetX = getRandomFloat(w / 2, w);    // Випадкова цільова координата X
        float targetY = getRandomFloat(h / 2, h);    // Випадкова цільова координата Y
        if (startX == targetX && startY == targetY) { // Якщо початкова точка співпадає з цільовою
            targetX = startX;                        // Залишаємо початкову координату X
            targetY = startY;                        // Залишаємо початкову координату Y
        }
        cars.emplace_back(startX, startY, speed, targetX, targetY); // Додаємо легкову машину у вектор
    }

    // Симуляція руху
    bool allStopped = false;                         // Прапорець для перевірки зупинки всіх машин
    while (!allStopped) {                            // Поки не зупинились всі машини
        allStopped = true;                           // Припускаємо, що всі машини зупинились

        for (auto& truck : trucks) {                 // Для кожної вантажівки
            truck.move();                            // Рухаємо вантажівку
            if (truck.isMoving) {                    // Якщо вантажівка ще рухається
                allStopped = false;                  // Скидаємо прапорець
            }
        }

        for (auto& car : cars) {                     // Для кожної легкової машини
            car.move();                              // Рухаємо легкову машину
            if (car.isMoving) {                      // Якщо легкова машина ще рухається
                allStopped = false;                  // Скидаємо прапорець
            }
        }

        // Виведення позицій (необов'язково)
        std::cout << "Truck positions:\n";           // Виведення заголовку для позицій вантажівок
        for (const auto& truck : trucks) {           // Для кожної вантажівки
            std::cout << "Truck at (" << truck.x << ", " << truck.y << ")\n"; // Виведення позиції вантажівки
        }

        std::cout << "Car positions:\n";             // Виведення заголовку для позицій легкових машин
        for (const auto& car : cars) {               // Для кожної легкової машини
            std::cout << "Car at (" << car.x << ", " << car.y << ")\n"; // Виведення позиції легкової машини
        }

        // Імітація затримки для часу кроку симуляції
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Затримка на 100 мілісекунд
    }

    std::cout << "Simulation ended. All cars and trucks have stopped.\n"; // Повідомлення про закінчення симуляції

    return 0; // Завершення програми
}
