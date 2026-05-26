#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "functions.h"

int display_menu_start(void) {
	int choise;

	printf("---------------------------------------------------\n");
	printf("| Меню:                                           |\n");
	printf("---------------------------------------------------\n");
	printf("| 1) Чтение данных из файла                       |\n");
	printf("| 9) Выход из программы без сохранения изменений  |\n");
	printf("---------------------------------------------------\n");

	printf("  Выберите пункт меню:\t");
	scanf("%d", &choise);

	while (choise != 1 && choise != 9) {
		printf("[!] Данного пункта не существует или он недоступен, попробуйте еще раз.\n");
		printf("    Выберите пункт меню:\t");
		scanf("%d", &choise);
	}

	return choise;
}

int display_menu(void) {
	int choise;

	printf("---------------------------------------------------\n");
	printf("| Меню:                                           |\n");
	printf("---------------------------------------------------\n");
	printf("| 1) Чтение данных из файла                       |\n");
	printf("| 2) Просмотр всего списка                        |\n");
	printf("| 3) Сортировка данных                            |\n");
	printf("| 4) Поиск данных с использованием фильтров       |\n");
	printf("| 5) Добавление данных в список                   |\n");
	printf("| 6) Удаление данных из списка                    |\n");
	printf("| 7) Редактирование данных                        |\n");
	printf("| 8) Формирование новогодних подарков             |\n");
	printf("| 9) Выход из программы без сохранения изменений  |\n");
	printf("| 10) Выход с сохранением изменений               |\n");
	printf("---------------------------------------------------\n");

	printf("  Выберите пункт меню:\t");
	scanf("%d", &choise);

	while (choise > 10 && choise < 1) {
		printf("[!] Данного пункта не существует, попробуйте еще раз.");
		printf("    Выберите пункт меню:\t");
		scanf("%d", &choise);
	}

	return choise;
}

void display_menu_lists(TypeWrapper* types, int typeCount, SweetsWrapper* sweets, int sweetCount, bool was_formed,
	ChristmasGift* gift, int gift_count) {

	int choice;

	do {
		printf("\n---------------------------------------------------\n");
		printf("| Просмотр данных:                                |\n");
		printf("---------------------------------------------------\n");
		printf("| 0) Назад в главное меню                         |\n");
		printf("| 1) Список типов сладостей                       |\n");
		printf("| 2) Список всех сладостей                        |\n");
		if (was_formed) {
			printf("| 3) Сформированные новогодние подарки            |\n");
		}
		printf("---------------------------------------------------\n");
		printf("  Ваш выбор: ");
		scanf("%d", &choice);

		int boader;
		if (was_formed) {
			boader = 3;
		}
		else {
			boader = 2;
		}
		
		while (choice < 0 || choice > boader) {
			printf("\n[!] Ошибка: Неверный пункт меню. Попробуйте ещё раз.\n");
			printf("    Ваш выбор: ");
			scanf("%d", &choice);
		}

		switch (choice) {
		case 1:
			view_types(types, typeCount);
			break;
		case 2:
			view_sweets(sweets, sweetCount, types, typeCount);
			break;
		case 3:
			if (was_formed) {
				ViewChristmasGifts(gift, gift_count, was_formed);
			}
			else {
				printf("[!] Подарки еще не были сформированы!\n");
			}
			break;
		case 0:
			printf("Возврат...\n");
			break;
		}

		if (choice != 0) {
			printf("\nНажмите Enter, чтобы продолжить...");
			fflush(stdin); // Очистка буфера
			getchar(); getchar();
		}

	} while (choice != 0);
}


void display_sorted_lists_menu(SweetsSortContainer sweets_sorts, TypesSortContainer types_sorts, TypeWrapper* types, int typeCount) {
	int main_choice, sub_choice;

	do {
		printf("\n---------------------------------------------------\n");
		printf("| Просмотр отсортированных списков:               |\n");
		printf("---------------------------------------------------\n");
		printf("| 0) Назад в главное меню                         |\n");
		printf("| 1) Выбрать сортировку сладостей                 |\n");
		printf("| 2) Выбрать сортировку типов                     |\n");
		printf("---------------------------------------------------\n");
		printf("  Ваш выбор: ");
		scanf("%d", &main_choice);

		if (main_choice == 1) {
			printf("\nДоступные сортировки сладостей:\n");
			printf("1) По ID\n2) По названию\n3) По цене\n4) По весу\n5) По сахару\nВыбор: ");
			scanf("%d", &sub_choice);

			switch (sub_choice) {
			case 1: view_sorted_sweets(sweets_sorts.id, types, typeCount); break;
			case 2: view_sorted_sweets(sweets_sorts.name, types, typeCount); break;
			case 3: view_sorted_sweets(sweets_sorts.price, types, typeCount); break;
			case 4: view_sorted_sweets(sweets_sorts.weight, types, typeCount); break;
			case 5: view_sorted_sweets(sweets_sorts.sugar, types, typeCount); break;
			default: printf("[!] Неверный выбор.\n");
			}
		}
		else if (main_choice == 2) {
			printf("\nДоступные сортировки типов:\n");
			printf("1) По ID\n2) По коду\n3) По названию\n4) По количеству сладостей\nВыбор: ");
			scanf("%d", &sub_choice);

			switch (sub_choice) {
			case 1: view_sorted_types(types_sorts.id); break;
			case 2: view_sorted_types(types_sorts.code); break;
			case 3: view_sorted_types(types_sorts.name); break;
			case 4: view_sorted_types(types_sorts.count); break;
			default: printf("[!] Неверный выбор.\n");
			}
		}

		if (main_choice != 0) {
			printf("\nНажмите Enter, чтобы продолжить...");
			fflush(stdin);
			getchar(); getchar();
		}

	} while (main_choice != 0);
}

// Меню-координатор для добавления элементов ===================================================================================
void display_add_menu(SweetsWrapper** sweets, int* s_size, int* s_capacity, int* s_next_id,
	TypeWrapper** types, int* t_size, int* t_capacity, int* t_next_id,
	SweetsSortContainer sweets_sorts, TypesSortContainer types_sorts) {
	int choice;
	do {
		printf("\n---------------------------------------------------\n");
		printf("| Блок добавления элементов:                      |\n");
		printf("---------------------------------------------------\n");
		printf("| 0) Назад в главное меню                         |\n");
		printf("| 1) Добавить новый тип сладостей                 |\n");
		printf("| 2) Добавить новую сладость                      |\n");
		printf("---------------------------------------------------\n");
		printf("  Ваш выбор: ");

		scanf("%d", &choice);

		while (choice > 2 || choice < 0) {
			printf("[!] Неверный ввод. Попробуйте снова.\n");
			scanf("%d", &choice);
		}

		if (choice == 1) {
			add_type(types, t_size, t_capacity, t_next_id);
			// Перестраиваем связи и обновляем сортировочные списки типов
			relink_data(*sweets, *s_size, *types, *t_size);
			UpdateAllSortsTypes(*types, *t_size, types_sorts.id, types_sorts.code, types_sorts.name, types_sorts.count);
		}
		else if (choice == 2) {
			add_sweet(sweets, s_size, s_capacity, s_next_id, *types, *t_size);
			// Перестраиваем связи и обновляем сортировочные списки сладостей и типов
			relink_data(*sweets, *s_size, *types, *t_size);
			UpdateAllSortsSweets(*sweets, *s_size, sweets_sorts.id, sweets_sorts.name, sweets_sorts.price, sweets_sorts.weight, sweets_sorts.sugar);
			UpdateAllSortsTypes(*types, *t_size, types_sorts.id, types_sorts.code, types_sorts.name, types_sorts.count);
		}
	} while (choice != 0);
}


// Меню для блока удаления  ==========================================================================================================
void display_delete_menu(SweetsWrapper* sweets, int s_size, TypeWrapper* types, int t_size,
	SweetsSortContainer sweets_sorts, TypesSortContainer types_sorts) {
	int choice;
	do {
		printf("\n---------------------------------------------------\n");
		printf("| Блок удаления элементов:                        |\n");
		printf("---------------------------------------------------\n");
		printf("| 0) Назад в главное меню                         |\n");
		printf("| 1) Удалить тип сладостей (и его сладости)       |\n");
		printf("| 2) Удалить конкретную сладость                  |\n");
		printf("---------------------------------------------------\n");
		printf("  Ваш выбор: ");

		scanf("%d", &choice);

		while (choice > 2 || choice < 0) {
			printf("[!] Неверный ввод. Попробуйте снова.\n");
			scanf("%d", &choice);
		}

		if (choice == 1) {
			delete_type(types, t_size, sweets, s_size);
			// Перестраиваем связи и полностью обновляем все сортировочные списки
			relink_data(sweets, s_size, types, t_size);
			UpdateAllSortsSweets(sweets, s_size, sweets_sorts.id, sweets_sorts.name, sweets_sorts.price, sweets_sorts.weight, sweets_sorts.sugar);
			UpdateAllSortsTypes(types, t_size, types_sorts.id, types_sorts.code, types_sorts.name, types_sorts.count);
		}
		else if (choice == 2) {
			delete_sweet(sweets, s_size);
			// Перестраиваем связи и полностью обновляем все сортировочные списки
			relink_data(sweets, s_size, types, t_size);
			UpdateAllSortsSweets(sweets, s_size, sweets_sorts.id, sweets_sorts.name, sweets_sorts.price, sweets_sorts.weight, sweets_sorts.sugar);
			UpdateAllSortsTypes(types, t_size, types_sorts.id, types_sorts.code, types_sorts.name, types_sorts.count);
		}
	} while (choice != 0);
}


// Меню для блока редактирования ============================================================================================
void display_edit_menu(SweetsWrapper* sweets, int s_size, TypeWrapper* types, int t_size,
	SweetsSortContainer sweets_sorts, TypesSortContainer types_sorts) {
	int choice;
	do {
		printf("\n---------------------------------------------------\n");
		printf("| Блок редактирования элементов:                  |\n");
		printf("---------------------------------------------------\n");
		printf("| 0) Назад в главное меню                         |\n");
		printf("| 1) Редактировать тип сладостей                  |\n");
		printf("| 2) Редактировать конкретную сладость            |\n");
		printf("---------------------------------------------------\n");
		printf("  Ваш выбор: ");

		scanf("%d", &choice);

		while (choice > 2 || choice < 0) {
			printf("[!] Неверный ввод. Попробуйте снова.\n");
			scanf("%d", &choice);
		}

		if (choice == 1) {
			edit_type(types, t_size, sweets, s_size);
			// Перестраиваем внутренние связи и полностью обновляем все индексы сортировки
			relink_data(sweets, s_size, types, t_size);
			UpdateAllSortsSweets(sweets, s_size, sweets_sorts.id, sweets_sorts.name, sweets_sorts.price, sweets_sorts.weight, sweets_sorts.sugar);
			UpdateAllSortsTypes(types, t_size, types_sorts.id, types_sorts.code, types_sorts.name, types_sorts.count);
		}
		else if (choice == 2) {
			edit_sweet(sweets, s_size, types, t_size);
			// Перестраиваем внутренние связи и полностью обновляем все индексы сортировки
			relink_data(sweets, s_size, types, t_size);
			UpdateAllSortsSweets(sweets, s_size, sweets_sorts.id, sweets_sorts.name, sweets_sorts.price, sweets_sorts.weight, sweets_sorts.sugar);
			UpdateAllSortsTypes(types, t_size, types_sorts.id, types_sorts.code, types_sorts.name, types_sorts.count);
		}
	} while (choice != 0);
}

//Меню  для поиска по фильтрам ===================================================================================
void display_search_menu(SweetsWrapper* sweets, int s_size, TypeWrapper* types, int t_size) {
	int choice;
	do {
		printf("\n---------------------------------------------------\n");
		printf("| Поиск данных с использованием фильтра            |\n");
		printf("---------------------------------------------------\n");
		printf("| 0) Назад в главное меню                         |\n");
		printf("| 1) Искать типы по фильтрам                      |\n");
		printf("| 2) Искать сладости по фильтрам                  |\n");
		printf("---------------------------------------------------\n");
		printf("  Ваш выбор: ");

		scanf("%d", &choice);

		while (choice > 2 || choice < 0) {
			printf("[!] Неверный ввод. Попробуйте снова.\n");
			scanf("%d", &choice);
		}

		if (choice == 2) {
			search_sweets_with_filters(sweets, s_size, types, t_size);
		}
		else if (choice == 1) {
			search_types_with_filters(types, t_size);
		}
	} while (choice != 0);
}