#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

// Функция редактирования типа сладостей
void edit_type(TypeWrapper* types, int typeCount, SweetsWrapper* sweets, int sweetCount) {
    int target_id;
    printf("Введите ID типа для редактирования: ");
    
    scanf("%d", &target_id);

    int index = -1;
    for (int i = 0; i < typeCount; i++) {
        if (!types[i].is_deleted && types[i].data.id == target_id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("[!] Ошибка: Активный тип с ID %d не найден.\n", target_id);
        return;
    }

    Type* t = &(types[index].data);
    int choice;
    do {
        printf("\nТекущие данные типа (ID: %d):\n", t->id);
        printf("1) Код типа: %d\n", t->code);
        printf("2) Название типа: %s\n", t->type_name);
        printf("0) Завершить редактирование этого типа\n");
        printf("Выберите пункт для изменения: ");

        scanf("%d", &choice);

        while (choice > 2 || choice < 0) {
            printf("Неверный ввод. Попробуйте снова.\n");
            scanf("%d", &choice);
        }

        if (choice == 1) {
            int new_code;
            printf("Введите новый код типа: ");
            if (scanf("%d", &new_code) != 1) {
                printf("[!] Ошибка ввода кода.\n");
                while (getchar() != '\n');
                continue;
            }

            // Проверка уникальности нового кода среди остальных ЖИВЫХ типов
            bool is_unique = true;
            for (int i = 0; i < typeCount; i++) {
                if (!types[i].is_deleted && i != index && types[i].data.code == new_code) {
                    is_unique = false;
                    break;
                }
            }

            if (!is_unique) {
                printf("[!] Ошибка: Тип с кодом %d уже существует в базе данных.\n", new_code);
            }
            else {
                int old_code = t->code;
                t->code = new_code;

                // КАСКАДНОЕ ОБНОВЛЕНИЕ: меняем код типа у всех привязанных к нему сладостей
                int cascade_counter = 0;
                for (int j = 0; j < sweetCount; j++) {
                    if (!sweets[j].is_deleted && sweets[j].data.type_code == old_code) {
                        sweets[j].data.type_code = new_code;
                        cascade_counter++;
                    }
                }
                printf("[+] Код успешно изменен с %d на %d.\n", old_code, new_code);
                if (cascade_counter > 0) {
                    printf("[i] Каскадно обновлен код типа у %d связанных сладостей.\n", cascade_counter);
                }
            }
        }
        else if (choice == 2) {
            char new_name[100];
            printf("Введите новое название типа: ");
            while (getchar() != '\n'); // Очистка буфера
            if (scanf(" %99[^\n]", new_name) == 1) {
                strcpy(t->type_name, new_name);
                printf("[+] Название типа успешно изменено.\n");
            }
            else {
                printf("[!] Ошибка ввода названия.\n");
            }
        }
    } while (choice != 0);
}

// Функция редактирования конкретной сладости
void edit_sweet(SweetsWrapper* sweets, int sweetCount, TypeWrapper* types, int typeCount) {
    int target_id;
    printf("Введите ID сладости для редактирования: ");
    if (scanf("%d", &target_id) != 1) {
        printf("[!] Ошибка ввода ID.\n");
        while (getchar() != '\n');
        return;
    }

    int index = -1;
    for (int i = 0; i < sweetCount; i++) {
        if (!sweets[i].is_deleted && sweets[i].data.id == target_id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("[!] Ошибка: Активная сладость с ID %d не найдена.\n", target_id);
        return;
    }

    Sweets* s = &(sweets[index].data);
    int choice;
    do {
        printf("\nТекущие данные сладости (ID: %d):\n", s->id);
        printf("1) Название: %s\n", s->sweet_name);
        printf("2) Код типа (категория): %d\n", s->type_code);
        printf("3) Цена: %.2f руб.\n", s->price);
        printf("4) Вес: %.3f кг\n", s->weight);
        printf("5) Содержание сахара: %.1f%%\n", s->amount_of_sugar);
        printf("0) Завершить редактирование этой сладости\n");
        printf("Выберите поле для изменения: ");

        scanf("%d", &choice);

        while (choice > 5 || choice < 0) {
            printf("[!] Неверный ввод. Попробуйте снова.\n");
            scanf("%d", &choice);
        }

        if (choice == 1) {
            char new_name[100];
            printf("Введите новое название: ");
            while (getchar() != '\n');
            if (scanf(" %99[^\n]", new_name) == 1) {
                strcpy(s->sweet_name, new_name);
                printf("[+] Название успешно изменено.\n");
            }
        }
        else if (choice == 2) {
            int new_type_code;
            printf("Введите новый код существующего типа: ");
            if (scanf("%d", &new_type_code) != 1) {
                printf("[!] Ошибка ввода.\n");
                while (getchar() != '\n');
                continue;
            }

            // Валидация: проверяем, существует ли тип с таким кодом
            bool type_exists = false;
            for (int i = 0; i < typeCount; i++) {
                if (!types[i].is_deleted && types[i].data.code == new_type_code) {
                    type_exists = true;
                    break;
                }
            }

            if (!type_exists) {
                printf("[!] Ошибка: Тип с кодом %d не найден в базе данных. Сначала создайте этот тип.\n", new_type_code);
            }
            else {
                s->type_code = new_type_code;
                printf("[+] Категория сладости успешно изменена.\n");
            }
        }
        else if (choice == 3) {
            double new_price;
            printf("Введите новую цену (руб.): ");
            if (scanf("%lf", &new_price) == 1 && new_price >= 0) {
                s->price = new_price;
                printf("[+] Цена обновлена.\n");
            }
            else {
                printf("[!] Ошибка: некорректная цена.\n");
                while (getchar() != '\n');
            }
        }
        else if (choice == 4) {
            double new_weight;
            printf("Введите новый вес (кг): ");
            if (scanf("%lf", &new_weight) == 1 && new_weight > 0) {
                s->weight = new_weight;
                printf("[+] Вес обновлен.\n");
            }
            else {
                printf("[!] Ошибка: некорректный вес.\n");
                while (getchar() != '\n');
            }
        }
        else if (choice == 5) {
            double new_sugar;
            printf("Введите новое количество сахара (%%): ");
            if (scanf("%lf", &new_sugar) == 1 && new_sugar >= 0 && new_sugar <= 100) {
                s->amount_of_sugar = new_sugar;
                printf("[+] Процент сахара обновлен.\n");
            }
            else {
                printf("[!] Ошибка: некорректное значение сахара (должно быть от 0 до 100).\n");
                while (getchar() != '\n');
            }
        }
    } while (choice != 0);
}

