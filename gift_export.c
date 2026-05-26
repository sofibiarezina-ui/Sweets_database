#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

// функция выгрузки подарка в текстовый файл
void ExportToTextFile(const char* filename, ChristmasGift* gifts, int giftCount) {
    if (gifts == NULL || giftCount == 0) {
        printf("[!] Ошибка: Массив подарков пуст, нечего экспортировать.\n");
        return;
    }

    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("[!] Ошибка создания файла %s\n", filename);
        return;
    }

    
    for (int i = 0; i < giftCount; i++) {
        ChristmasGift* g = &gifts[i];
        fprintf(fp, ">>> ПОДАРОК №%d <<<\n", g->id);
        fprintf(fp, "-------------------------------------------------------------------------\n");
        fprintf(fp, "| %-30s | %-25s | %-10s |\n", "Категория (Тип)", "Название сладости", "Кол-во (шт)");
        fprintf(fp, "-------------------------------------------------------------------------\n");

        GiftNode* curr = g->head;
        while (curr != NULL) {
            fprintf(fp, "| %-30s | %-25s | %-10d |\n", curr->type_name, curr->sweet_name, curr->count);
            curr = curr->next;
        }

        fprintf(fp, "-------------------------------------------------------------------------\n");
        fprintf(fp, " Итоговые параметры подарка №%d:\n", g->id);
        fprintf(fp, "  - Общий вес:         %.3f кг\n", g->total_weight);
        fprintf(fp, "  - Общая стоимость:   %.2f руб.\n", g->total_price);
        fprintf(fp, "  - Уникальных типов:  %d\n", g->unique_types_count);
        fprintf(fp, "  - Чистый сахар:      %.4f кг\n", g->total_sugar);
        fprintf(fp, "=========================================================================\n\n");
    }

    fclose(fp);
    printf("[+] Полный отчет по %d подаркам сохранен в файл: \"%s\"\n", giftCount, filename);
}

// Координатор для вызова из главного меню 
void process_export_gift(ChristmasGift* gifts, int giftCount, bool was_formed) {
    if (!was_formed || gifts == NULL || giftCount == 0) {
        printf("[!] Ошибка: Подарки еще не были расформированы! Выполните пункт 8.\n");
        return;
    }

    char filename[150];
    printf("\nВведите имя текстового файла для сохранения отчета: ");
    while (getchar() != '\n');
    if (scanf(" %149[^\n]", filename) != 1) {
        strcpy(filename, "all_gifts_report.txt");
    }

    ExportToTextFile(filename, gifts, giftCount);
}
