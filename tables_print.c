#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

// Заголовки
void print_header_sweets() {
    printf("\n%s\n", "----------------------------------------------------------------------------------------------------------");
    printf("|| %-3s | %-30s | %-26s | %-8s | %-7s | %-9s ||\n", "ID", "Name", "Type", "Price,р", "Weight,kg", "Sugar,%");
    printf("%s\n", "----------------------------------------------------------------------------------------------------------");
}

void print_header_types() {
    printf("\n%s\n", "-----------------------------------------------------------");
    printf("|| %-3s | %-6s | %-27s | %-8s ||\n", "ID", "Code", "Type name", "Amount");
    printf("%s\n", "-----------------------------------------------------------");
}

// Просмотр типов
void view_types(TypeWrapper* types, int typeCount) {

    print_header_types();
    for (int i = 0; i < typeCount; i++) {
        if (!types[i].is_deleted) {
            printf("|| %-3d | %-6d | %-27s | %-8d ||\n",
                types[i].data.id,
                types[i].data.code,
                types[i].data.type_name,
                types[i].data.sweets_count);
        }
    }
    printf("-----------------------------------------------------------\n");
}

// Просмотр сладостей (с поиском имени типа по коду)
void view_sweets(SweetsWrapper* sweets, int sweetCount, TypeWrapper* types, int typeCount) {
   
    print_header_sweets();
    for (int i = 0; i < sweetCount; i++) {
        if (!sweets[i].is_deleted) {
            // текстовое название типа по коду для наглядности
            const char* t_name = "No info";
            for (int j = 0; j < typeCount; j++) {
                if (!types[j].is_deleted && types[j].data.code == sweets[i].data.type_code) {
                    t_name = types[j].data.type_name;
                    break;
                }
            }

            printf("|| %-3d | %-30s | %-26s | %8.2f | %9.2f | %9.2f ||\n",
                sweets[i].data.id,
                sweets[i].data.sweet_name,
                t_name,
                sweets[i].data.price,
                sweets[i].data.weight,
                sweets[i].data.amount_of_sugar);
        }
    }
    printf("%s\n", "----------------------------------------------------------------------------------------------------------");
}


// Печать отсортированного списка сладостей по указателям
void view_sorted_sweets(SortedSweetsList* list, TypeWrapper* types, int typeCount) {
    if (list == NULL || list->items == NULL || list->count == 0) {
        printf("\n[!] Список сладостей (%s) пуст.\n", list->sort_type);
        return;
    }

    // Используем уже настроенный нами ранее заголовок
    print_header_sweets();

    for (int i = 0; i < list->count; i++) {
        Sweets* s = list->items[i];

        // Ищем имя типа для красивого вывода
        const char* t_name = "Не указан";
        for (int j = 0; j < typeCount; j++) {
            if (!types[j].is_deleted && types[j].data.code == s->type_code) {
                t_name = types[j].data.type_name;
                break;
            }
        }

        // Строгое выравнивание без обрезки по точке
        printf("| %-3d | %-30s | %-25s | %-6.2f | %-6.3f | %-6.1f |\n",
            s->id, s->sweet_name, t_name, s->price, s->weight, s->amount_of_sugar);
    }
    printf("----------------------------------------------------------------------------------------------\n");
}

// Печать отсортированного списка типов по указателям
void view_sorted_types(SortedTypeList* list) {
    if (list == NULL || list->items == NULL || list->count == 0) {
        printf("\n[!] Список типов (%s) пуст.\n", list->sort_type);
        return;
    }

    print_header_types();
    for (int i = 0; i < list->count; i++) {
        Type* t = list->items[i];
        printf("| %-3d | %-8d | %-25s | %-7d |\n", t->id, t->code, t->type_name, t->sweets_count);
    }
    printf("----------------------------------------------------------\n");
}




// Функция отображения массива списков сформированных подарков в консоли
void ViewChristmasGifts(ChristmasGift* gifts, int giftCount, bool was_formed) {
    printf("\n=========================================================================\n");
    printf("                  ПРОСМОТР СФОРМИРОВАННЫХ ПОДАРКОВ                       \n");
    printf("         (Отсортировано по возрастанию содержания сахара)               \n");
    printf("=========================================================================\n");

    if (!was_formed || gifts == NULL || giftCount == 0) {
        printf("[!] Ошибка: Подарки еще не были расформированы.\n");
        printf("[i] Сначала выберите пункт формирования подарков в меню (Пункт 8).\n");
        printf("=========================================================================\n");
        return;
    }

    // Проходим циклом по всему массиву подарков
    for (int i = 0; i < giftCount; i++) {
        ChristmasGift* g = &gifts[i];

        printf("\n>>> ПОДАРОК №%d (Сахар: %.4f кг) <<<\n", g->id, g->total_sugar);
        printf("-------------------------------------------------------------------------\n");
        printf("|| %-30s | %-25s | %-10s ||\n", "Категория (Тип)", "Название сладости", "Кол-во (шт)");
        printf("-------------------------------------------------------------------------\n");

        // Обход односвязного списка сладостей внутри текущего подарка
        GiftNode* current_node = g->head;
        if (current_node == NULL) {
            printf("|| %-70s ||\n", "Подарок пуст (нет элементов)");
        }

        while (current_node != NULL) {
            printf("|| %-30s | %-25s | %-10d ||\n",
                current_node->type_name,
                current_node->sweet_name,
                current_node->count);
            current_node = current_node->next; // Переход к следующему элементу списка
        }

        // Подвал с характеристиками для каждого подарка отдельно
        printf("-------------------------------------------------------------------------\n");
        printf("   Итоговые параметры подарка №%d:\n", g->id);
        printf("   - Общий вес:         %-10.3f кг\n", g->total_weight);
        printf("   - Общая стоимость:   %-10.2f руб.\n", g->total_price);
        printf("   - Количество типов:  %-10d\n", g->unique_types_count);
        printf("=========================================================================\n");
    }
}

