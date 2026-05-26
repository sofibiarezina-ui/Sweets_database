#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

// Вспомогательная функция для обновления динамических связей между типами и сладостями
void relink_data(SweetsWrapper* sweets, int sweetCount, TypeWrapper* types, int typeCount) {
    // Используем функцию очистки из free.c
    clear_type_links(types, typeCount);

    for (int i = 0; i < typeCount; i++) {
        if (types[i].is_deleted) continue;

        int current_code = types[i].data.code;
        int found = 0;

        // Считаем активные сладости этого типа
        for (int j = 0; j < sweetCount; j++) {
            if (!sweets[j].is_deleted && sweets[j].data.type_code == current_code) {
                found++;
            }
        }

        types[i].data.sweets_count = found;

        if (found > 0) {
            types[i].data.sweets_of_type = (Sweets**)malloc(sizeof(Sweets*) * found);
            if (types[i].data.sweets_of_type == NULL) {
                printf("[!] Ошибка выделения памяти при связывании типов.\n");
                return;
            }

            int k = 0;
            for (int j = 0; j < sweetCount; j++) {
                if (!sweets[j].is_deleted && sweets[j].data.type_code == current_code) {
                    types[i].data.sweets_of_type[k] = &(sweets[j].data);
                    k++;
                }
            }
        }
        else {
            types[i].data.sweets_of_type = NULL;
        }
    }
}

// Добавление нового типа сладостей
void add_type(TypeWrapper** types, int* size, int* capacity, int* next_id) {
    // Если массив заполнен, удваиваем емкость
    if (*size >= *capacity) {
        int new_capacity = (*capacity == 0) ? 4 : (*capacity * 2);
        TypeWrapper* temp = (TypeWrapper*)realloc(*types, sizeof(TypeWrapper) * new_capacity);
        if (temp == NULL) {
            printf("\n[!] Ошибка: не удалось выделить память для расширения списка типов.\n");
            return;
        }
        *types = temp;
        *capacity = new_capacity;
    }

    int code;
    char name[100];

    printf("\n===================================================\n");
    printf("  ДОБАВЛЕНИЕ НОВОГО ТИПА СЛАДОСТЕЙ\n");
    printf("===================================================\n");

    printf("Введите уникальный код типа (целое число): ");
    if (scanf("%d", &code) != 1) {
        printf("[!] Ошибка ввода. Возврат в меню.\n");
        while (getchar() != '\n');
        return;
    }

    // Проверка уникальности кода среди живых типов
    for (int i = 0; i < *size; i++) {
        if (!(*types)[i].is_deleted && (*types)[i].data.code == code) {
            printf("[!] Ошибка: Тип с кодом %d уже существует (\"%s\").\n", code, (*types)[i].data.type_name);
            return;
        }
    }

    printf("Введите название типа: ");
    while (getchar() != '\n'); // Очистка буфера от '\n'
    if (scanf(" %99[^\n]", name) != 1) {
        printf("[!] Ошибка ввода названия.\n");
        return;
    }

    // Инициализация новой записи
    TypeWrapper* new_type = &((*types)[*size]);
    new_type->data.id = *next_id;
    new_type->data.code = code;
    strcpy(new_type->data.type_name, name);
    new_type->data.sweets_count = 0;
    new_type->data.sweets_of_type = NULL;
    new_type->is_deleted = false;

    (*size)++;
    (*next_id)++;

    printf("\n[+] Тип успешно добавлен! (ID: %d, Код: %d, Название: %s)\n", new_type->data.id, code, name);
}

// Добавление новой сладости
void add_sweet(SweetsWrapper** sweets, int* size, int* capacity, int* next_id, TypeWrapper* types, int type_size) {
    if (*size >= *capacity) {
        int new_capacity = (*capacity == 0) ? 4 : (*capacity * 2);
        SweetsWrapper* temp = (SweetsWrapper*)realloc(*sweets, sizeof(SweetsWrapper) * new_capacity);
        if (temp == NULL) {
            printf("\n[!] Ошибка: не удалось выделить память для расширения списка сладостей.\n");
            return;
        }
        *sweets = temp;
        *capacity = new_capacity;
    }

    char name[100];
    int type_code;
    double price, weight, sugar;

    printf("\n===================================================\n");
    printf("  ДОБАВЛЕНИЕ НОВОЙ СЛАДОСТИ\n");
    printf("===================================================\n");

    printf("Введите название сладости: ");
    while (getchar() != '\n'); // Очистка буфера
    if (scanf(" %99[^\n]", name) != 1) {
        printf("[!] Ошибка ввода названия.\n");
        return;
    }

    printf("Введите код существующего типа: ");
    if (scanf("%d", &type_code) != 1) {
        printf("[!] Ошибка ввода кода типа.\n");
        while (getchar() != '\n');
        return;
    }

    // Валидация: существует ли такой тип сладости?
    bool type_found = false;
    for (int i = 0; i < type_size; i++) {
        if (!types[i].is_deleted && types[i].data.code == type_code) {
            type_found = true;
            break;
        }
    }

    if (!type_found) {
        printf("[!] Ошибка: Тип с кодом %d не найден в базе данных. Сначала добавьте этот тип.\n", type_code);
        return;
    }

    printf("Введите цену (руб.): ");
    if (scanf("%lf", &price) != 1 || price < 0) {
        printf("[!] Ошибка: некорректная цена.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Введите вес (кг): ");
    if (scanf("%lf", &weight) != 1 || weight <= 0) {
        printf("[!] Ошибка: некорректный вес.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Введите количество сахара (%%): ");
    if (scanf("%lf", &sugar) != 1 || sugar < 0 || sugar > 100) {
        printf("[!] Ошибка: некорректный процент сахара.\n");
        while (getchar() != '\n');
        return;
    }

    // Заполнение структуры
    SweetsWrapper* new_sweet = &((*sweets)[*size]);
    new_sweet->data.id = *next_id;
    strcpy(new_sweet->data.sweet_name, name);
    new_sweet->data.type_code = type_code;
    new_sweet->data.price = price;
    new_sweet->data.weight = weight;
    new_sweet->data.amount_of_sugar = sugar;
    new_sweet->is_deleted = false;

    (*size)++;
    (*next_id)++;

    printf("\n[+] Сладость успешно добавлена! (ID: %d, Название: %s)\n", new_sweet->data.id, name);
}
