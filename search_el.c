#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "functions.h"


void search_sweets_with_filters(SweetsWrapper* sweets, int s_size, TypeWrapper* types, int t_size) {
    // Переменные для хранения настроек фильтров
    char name_filter[100] = "";
    bool has_name_filter = false;

    double min_price = -1.0, max_price = -1.0;
    double min_weight = -1.0, max_weight = -1.0;
    double min_sugar = -1.0, max_sugar = -1.0;

    int type_code_filter = -1;

    int choice;
    do {
        printf("\n===================================================\n");
        printf("  Конструктор фильтров для СЛАДОСТЕЙ\n");
        printf("===================================================\n");
        printf("0) Назад в меню выбора списков поиска\n");
        printf("Текущие активные фильтры:\n");
        printf("1) Название содержит:    %s\n", has_name_filter ? name_filter : "[НЕ ЗАДАНО]");
        printf("2) Диапазон цен (руб):   ");
        if (min_price >= 0 || max_price >= 0) printf("%.2f - %.2f\n", min_price >= 0 ? min_price : 0.0, max_price >= 0 ? max_price : 999999.0);
        else printf("[НЕ ЗАДАНО]\n");

        printf("3) Диапазон веса (кг):   ");
        if (min_weight >= 0 || max_weight >= 0) printf("%.3f - %.3f\n", min_weight >= 0 ? min_weight : 0.0, max_weight >= 0 ? max_weight : 999.0);
        else printf("[НЕ ЗАДАНО]\n");

        printf("4) Диапазон сахара (%%):  ");
        if (min_sugar >= 0 || max_sugar >= 0) printf("%.1f%% - %.1f%%\n", min_sugar >= 0 ? min_sugar : 0.0, max_sugar >= 0 ? max_sugar : 100.0);
        else printf("[НЕ ЗАДАНО]\n");

        printf("5) Код типа сладости:    ");
        if (type_code_filter >= 0) printf("%d\n", type_code_filter);
        else printf("[НЕ ЗАДАНО]\n");

        printf("---------------------------------------------------\n");
        printf("6) [СБРОСИТЬ ВСЕ ФИЛЬТРЫ]\n");
        printf("7) Начать поиск с текущими фильтрами\n");
        printf("---------------------------------------------------\n");
        printf("Ваш выбор: ");

        scanf("%d", &choice);

        while (choice > 7 || choice < 0) {
            printf("Неверный ввод. Попробуйте снова.\n");
            scanf("%d", &choice);
        }

        switch (choice) {
        case 1:
            printf("Введите подстроку для поиска в названии: ");
            while (getchar() != '\n');
            if (scanf(" %99[^\n]", name_filter) == 1) has_name_filter = true;
            break;
        case 2:
            printf("Введите минимальную цену (или -1 для пропуска): ");
            scanf("%lf", &min_price);
            printf("Введите максимальную цену (или -1 для пропуска): ");
            scanf("%lf", &max_price);
            break;
        case 3:
            printf("Введите минимальный вес (или -1 для пропуска): ");
            scanf("%lf", &min_weight);
            printf("Введите максимальный вес (или -1 для пропуска): ");
            scanf("%lf", &max_weight);
            break;
        case 4:
            printf("Введите мин. сахар %% (или -1 для пропуска): ");
            scanf("%lf", &min_sugar);
            printf("Введите макс. сахар %% (или -1 для пропуска): ");
            scanf("%lf", &max_sugar);
            break;
        case 5:
            printf("Введите код типа для фильтрации (или -1 для пропуска): ");
            scanf("%d", &type_code_filter);
            break;
        case 6:
            has_name_filter = false;
            min_price = max_price = min_weight = max_weight = min_sugar = max_sugar = -1.0;
            type_code_filter = -1;
            printf("[+] Все фильтры сладостей сброшены.\n");
            break;
        case 7: {
            printf("\nРезультаты фильтрации сладостей:\n");
            int match_count = 0;
            bool header_printed = false;

            for (int i = 0; i < s_size; i++) {
                if (sweets[i].is_deleted) continue;
                Sweets* s = &(sweets[i].data);

                // Проверка фильтра по названию
                if (has_name_filter && strstr(s->sweet_name, name_filter) == NULL) continue;

                // Проверка фильтра по цене
                if (min_price >= 0 && s->price < min_price) continue;
                if (max_price >= 0 && s->price > max_price) continue;

                // Проверка фильтра по весу
                if (min_weight >= 0 && s->weight < min_weight) continue;
                if (max_weight >= 0 && s->weight > max_weight) continue;

                // Проверка фильтра по сахару
                if (min_sugar >= 0 && s->amount_of_sugar < min_sugar) continue;
                if (max_sugar >= 0 && s->amount_of_sugar > max_sugar) continue;

                // Проверка фильтра по коду типа
                if (type_code_filter >= 0 && s->type_code != type_code_filter) continue;

                // Если все проверки пройдены, выводим элемент
                if (!header_printed) {
                    print_header_sweets();
                    header_printed = true;
                }

                const char* t_name = "Не указан";
                for (int j = 0; j < t_size; j++) {
                    if (!types[j].is_deleted && types[j].data.code == s->type_code) {
                        t_name = types[j].data.type_name;
                        break;
                    }
                }

                printf("|| %-3d | %-48s | %-28s | %-8.2f | %-7.3f | %-7.1f ||\n",
                    s->id, s->sweet_name, t_name, s->price, s->weight, s->amount_of_sugar);
                match_count++;
            }

            if (match_count > 0) {
                printf("-----------------------------------------------------------------------------------------\n");
                printf("[i] Найдено элементов по фильтрам: %d шт.\n", match_count);
            }
            else {
                printf("[i] Нет активных сладостей, соответствующих заданным критериям фильтрации.\n");
            }
            break;
        }
        }
    } while (choice != 0);
}

void search_types_with_filters(TypeWrapper* types, int t_size) {
    char name_filter[100] = "";
    bool has_name_filter = false;

    int min_count = -1, max_count = -1;

    int choice;
    do {
        printf("\n===================================================\n");
        printf("  Конструктор фильтров для ТИПОВ СЛАДОСТЕЙ\n");
        printf("===================================================\n");
        printf("0) Назад в меню выбора списков поиска\n");
        printf("Текущие активные фильтры:\n");
        printf("1) Название содержит:       %s\n", has_name_filter ? name_filter : "[НЕ ЗАДАНО]");
        printf("2) Количество сладостей:     ");
        if (min_count >= 0 || max_count >= 0) printf("%d - %d\n", min_count >= 0 ? min_count : 0, max_count >= 0 ? max_count : 9999);
        else printf("[НЕ ЗАДАНО]\n");

        printf("---------------------------------------------------\n");
        printf("3) [СБРОСИТЬ ВСЕ ФИЛЬТРЫ]\n");
        printf("4) Начать поиск с текущими фильтрами\n");
        printf("---------------------------------------------------\n");
        printf("Ваш выбор: ");

        scanf("%d", &choice);

        while (choice > 4 || choice < 0) {
            printf("Неверный ввод. Попробуйте снова.\n");
            scanf("%d", &choice);
        }

        switch (choice) {
        case 1:
            printf("Введите подстроку для поиска в названии типа: ");
            while (getchar() != '\n');
            if (scanf(" %99[^\n]", name_filter) == 1) has_name_filter = true;
            break;
        case 2:
            printf("Введите минимальное кол-во сладостей (или -1 для пропуска): ");
            scanf("%d", &min_count);
            printf("Введите максимальное кол-во сладостей (или -1 для пропуска): ");
            scanf("%d", &max_count);
            break;
        case 3:
            has_name_filter = false;
            min_count = max_count = -1;
            printf("[+] Все фильтры типов сброшены.\n");
            break;
        case 4: {
            printf("\nРезультаты фильтрации типов:\n");
            int match_count = 0;
            bool header_printed = false;

            for (int i = 0; i < t_size; i++) {
                if (types[i].is_deleted) continue;
                Type* t = &(types[i].data);

                // Фильтр по названию подстроки
                if (has_name_filter && strstr(t->type_name, name_filter) == NULL) continue;

                // Фильтр по количеству элементов внутри типа
                if (min_count >= 0 && t->sweets_count < min_count) continue;
                if (max_count >= 0 && t->sweets_count > max_count) continue;

                if (!header_printed) {
                    print_header_types();
                    header_printed = true;
                }

                printf("|| %-3d | %-6d | %-27s | %-8d ||\n",
                    t->id, t->code, t->type_name, t->sweets_count);
                match_count++;
            }

            if (match_count > 0) {
                printf("-----------------------------------------------------------\n");
                printf("[i] Найдено типов по фильтрам: %d шт.\n", match_count);
            }
            else {
                printf("[i] Нет активных типов, соответствующих заданным критериям фильтрации.\n");
            }
            break;
        }
        }
    } while (choice != 0);
}

