#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

// Вспомогательная функция для проверки, есть ли уже такой тип сладости в подарке
static bool is_type_in_gift(ChristmasGift* gift, const char* type_name) {
    GiftNode* current = gift->head;
    while (current != NULL) {
        if (strcmp(current->type_name, type_name) == 0) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Вспомогательная функция для добавления сладости в список конкретного подарка
static void add_sweet_to_gift_list(ChristmasGift* gift, const char* type_name, const char* sweet_name) {
    // Проверяем, есть ли уже точно такая же сладость, чтобы просто увеличить счётчик
    GiftNode* current = gift->head;
    while (current != NULL) {
        if (strcmp(current->sweet_name, sweet_name) == 0) {
            current->count++;
            return;
        }
        current = current->next;
    }

    // Если сладость новая, создаем узел списка
    GiftNode* new_node = (GiftNode*)malloc(sizeof(GiftNode));
    if (!new_node) return;

    strcpy(new_node->type_name, type_name);
    strcpy(new_node->sweet_name, sweet_name);
    new_node->count = 1;
    new_node->next = gift->head; // Добавляем в начало списка
    gift->head = new_node;
}

// Функция сравнения подарков по суммарному сахару (для qsort)
static int comp_gifts_by_sugar(const void* a, const void* b) {
    const ChristmasGift* g1 = (const ChristmasGift*)a;
    const ChristmasGift* g2 = (const ChristmasGift*)b;
    if (g1->total_sugar < g2->total_sugar) return -1;
    if (g1->total_sugar > g2->total_sugar) return 1;
    return 0;
}


// Функция 8. Расформирование ВСЕХ сладостей по новогодним подаркам
// =========================================================================
void CreateChristmasGifts(TypeWrapper* types, int typeCount, SweetsWrapper* sweets, int sweetCount, ChristmasGift** gifts, int* giftCount) {
    // Если до этого были сформированы подарки, очищаем старую память
    if (*gifts != NULL) {
        clear_GiftList(*gifts, *giftCount);
        *gifts = NULL;
    }
    *giftCount = 0;

    if (sweets == NULL || sweetCount == 0) {
        printf("[!] База сладостей пуста. Нечего расформировывать.\n");
        return;
    }

    double max_weight, max_price;
    int max_types;

    printf("\n--- НАСТРОЙКА КРИТЕРИЕВ ДЛЯ НОВОГОДНИХ ПОДАРКОВ ---\n");
    printf("Введите максимальный вес одного подарка (кг): ");
    if (scanf("%lf", &max_weight) != 1 || max_weight <= 0) return;
    printf("Введите максимальную стоимость одного подарка (руб): ");
    if (scanf("%lf", &max_price) != 1 || max_price <= 0) return;
    printf("Введите макс. количество уникальных типов сладостей в подарке: ");
    if (scanf("%d", &max_types) != 1 || max_types <= 0) return;

    int capacity = 5; // Стартовая емкость динамического массива подарков
    *gifts = (ChristmasGift*)malloc(sizeof(ChristmasGift) * capacity);
    if (!*gifts) return;

    // Цикл по всем сладостям из базы данных
    for (int i = 0; i < sweetCount; i++) {
        if (sweets[i].is_deleted) continue;

        if (sweets[i].data.weight > max_weight || sweets[i].data.price > max_price) {
            printf("[!] Предупреждение: Сладость \"%s\" слишком тяжелая/дорогая и не поместится ни в один пустой подарок. Пропуск.\n", sweets[i].data.sweet_name);
            continue;
        }

        // Ищем название типа для этой сладости
        const char* current_type_name = "Неизвестный тип";
        for (int t = 0; t < typeCount; t++) {
            if (!types[t].is_deleted && types[t].data.code == sweets[i].data.type_code) {
                current_type_name = types[t].data.type_name;
                break;
            }
        }

        bool distributed = false;

        for (int g = 0; g < *giftCount; g++) {
            ChristmasGift* cand = &((*gifts)[g]);

            // Проверяем лимит веса и цены
            if (cand->total_weight + sweets[i].data.weight > max_weight) continue;
            if (cand->total_price + sweets[i].data.price > max_price) continue;

            // Проверяем лимит уникальных типов
            bool type_already_exists = is_type_in_gift(cand, current_type_name);
            if (!type_already_exists && cand->unique_types_count >= max_types) continue;

            // добавляем сладость в этот подарок
            if (!type_already_exists) {
                cand->unique_types_count++;
            }
            add_sweet_to_gift_list(cand, current_type_name, sweets[i].data.sweet_name);
            cand->total_weight += sweets[i].data.weight;
            cand->total_price += sweets[i].data.price;
            cand->total_sugar += (sweets[i].data.weight * (sweets[i].data.amount_of_sugar / 100.0)); // Считаем чистый сахар в кг

            distributed = true;
            break; 
        }

        // Если ни в один существующий подарок сладость не влезла — открываем новый подарок
        if (!distributed) {
            if (*giftCount >= capacity) {
                capacity *= 2;
                ChristmasGift* temp = (ChristmasGift*)realloc(*gifts, sizeof(ChristmasGift) * capacity);
                if (!temp) return;
                *gifts = temp;
            }

            ChristmasGift* new_gift = &((*gifts)[*giftCount]);
            new_gift->id = *giftCount + 1;
            new_gift->head = NULL;
            new_gift->total_weight = sweets[i].data.weight;
            new_gift->total_price = sweets[i].data.price;
            new_gift->unique_types_count = 1;
            new_gift->total_sugar = (sweets[i].data.weight * (sweets[i].data.amount_of_sugar / 100.0));

            add_sweet_to_gift_list(new_gift, current_type_name, sweets[i].data.sweet_name);
            (*giftCount)++;
        }
    }

    // Сортировка созданных подарков по возрастанию суммарного содержания сахара
    if (*giftCount > 1) {
        qsort(*gifts, *giftCount, sizeof(ChristmasGift), comp_gifts_by_sugar);
    }

    printf("[+] Расформирование завершено. Все сладости распределены по %d подаркам.\n", *giftCount);
    printf("[i] Подарки успешно отсортированы по возрастанию содержания сахара.\n");
}

