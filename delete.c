#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

// Функция удаления сладости по ID
void delete_sweet(SweetsWrapper* sweets, int sweetCount) {
    int target_id;
    printf("Введите ID сладости для удаления: ");

    if (scanf("%d", &target_id) != 1) {
        printf("[!] Ошибка ввода ID.\n");
        while (getchar() != '\n');
        return;
    }

    bool found = false;
    for (int i = 0; i < sweetCount; i++) {
        if (!sweets[i].is_deleted && sweets[i].data.id == target_id) {
            sweets[i].is_deleted = true;
            printf("[+] Сладость \"%s\" (ID: %d) успешно удалена.\n", sweets[i].data.sweet_name, target_id);
            found = true;
            break;
        }
    }

    if (!found) {
        printf("[!] Ошибка: Активная сладость с ID %d не найдена.\n", target_id);
    }
}

// Функция удаления типа по ID (с каскадным удалением сладостей)
void delete_type(TypeWrapper* types, int typeCount, SweetsWrapper* sweets, int sweetCount) {
    int target_id;
    printf("Введите ID типа для удаления: ");

    if (scanf("%d", &target_id) != 1) {
        printf("[!] Ошибка ввода ID.\n");
        while (getchar() != '\n');
        return;
    }

    bool found = false;
    for (int i = 0; i < typeCount; i++) {
        if (!types[i].is_deleted && types[i].data.id == target_id) {
            int target_code = types[i].data.code;

            // 1. Помечаем сам тип как удаленный
            types[i].is_deleted = true;
            printf("[+] Тип \"%s\" (ID: %d, Код: %d) успешно удален.\n", types[i].data.type_name, target_id, target_code);

            // 2. Каскадное удаление всех сладостей этого типа
            int cascade_count = 0;
            for (int j = 0; j < sweetCount; j++) {
                if (!sweets[j].is_deleted && sweets[j].data.type_code == target_code) {
                    sweets[j].is_deleted = true;
                    cascade_count++;
                }
            }

            if (cascade_count > 0) {
                printf("[i] Каскадно удалено сладостей этого типа: %d шт.\n", cascade_count);
            }

            found = true;
            break;
        }
    }

    if (!found) {
        printf("[!] Ошибка: Активный тип с ID %d не найден.\n", target_id);
    }
}

