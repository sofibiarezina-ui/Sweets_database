#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdbool.h>


//-----------------------------Структура реализации сладостей
typedef struct {
	int id;
	char sweet_name[100];
	int type_code;
	double price;
	double weight;
	double amount_of_sugar;
} Sweets;

typedef struct {
	Sweets data;
	bool is_deleted;
} SweetsWrapper;

typedef struct {
	Sweets** items;     // Массив указателей на оригинальные данные
	int count;          // Количество элементов
	char sort_type[40]; // Метка: "по цене", "по названию" и т.д.
} SortedSweetsList;

typedef struct {
	SortedSweetsList* id;
	SortedSweetsList* name;
	SortedSweetsList* price;
	SortedSweetsList* weight;
	SortedSweetsList* sugar;
} SweetsSortContainer;


//-----------------------------Структура реализации типов
typedef struct {
	int id;
	int code;
	char type_name[100];
	
	int sweets_count;  //Количество сладостей данного типа
	Sweets** sweets_of_type;
} Type;

typedef struct {
	Type data;
	bool is_deleted;
} TypeWrapper;

typedef struct {
	Type** items;     // Массив указателей на оригинальные данные
	int count;          // Количество элементов
	char sort_type[40]; // Метка: "по цене", "по названию" и т.д.
} SortedTypeList;

typedef struct {
	SortedTypeList* id;
	SortedTypeList* code;
	SortedTypeList* name;
	SortedTypeList* count;
} TypesSortContainer;

//-----------------------------Структура реализации подарков

// Элемент односвязного списка внутри конкретного подарка
typedef struct GiftNode {
	char type_name[100];
	char sweet_name[100];
	int count;
	struct GiftNode* next;
} GiftNode;

// Структура самого новогоднего подарка
typedef struct {
	int id;                 // Номер подарка
	GiftNode* head;         // Указатель на начало списка сладостей
	double total_weight;    // Текущий суммарный вес подарка
	double total_price;     // Текущая общая стоимость подарка
	int unique_types_count; // Количество уникальных типов сладостей в подарке
	double total_sugar;     // Суммарное содержание сахара (для сортировки)
} ChristmasGift;

// Чтение данных из файла
bool load_binary_sweets(SweetsWrapper** sweets, int* size, int* capacity, int* next_id);
bool load_binary_types(TypeWrapper** types, int* size, int* capacity, int* next_id);
void link_sweets_to_types(TypeWrapper* types, int typeCount, SweetsWrapper* sweets, int sweetCount);
bool complete_all_stuctures(
	SweetsWrapper** sweets, TypeWrapper** types,
	int* sweets_size, int* type_size,
	int* sweets_capacity, int* type_capacity,
	int* sweets_next_id, int* type_next_id);


// Сохранение изменений (Запись в text файлы)
void ExportToTextFile(const char* filename, ChristmasGift* gifts, int giftCount);
void process_export_gift(ChristmasGift* gifts, int giftCount, bool was_formed);


// Выход из программы (Очистка ресурсов)
void clear_type_links(TypeWrapper* types, int typeCount);
bool terminate_program(TypeWrapper** types, int* typeCount, SweetsWrapper** sweets, int* sweetCount);
void clear_GiftList(ChristmasGift* gifts, int giftCount);


// Поиск данных с использованием фильтров
void search_sweets_with_filters(SweetsWrapper* sweets, int s_size, TypeWrapper* types, int t_size);
void search_types_with_filters(TypeWrapper* types, int t_size);


// Добавление данных в список
void relink_data(SweetsWrapper* sweets, int sweetCount, TypeWrapper* types, int typeCount);
void add_type(TypeWrapper** types, int* size, int* capacity, int* next_id);
void add_sweet(SweetsWrapper** sweets, int* size, int* capacity, int* next_id, TypeWrapper* types, int type_size);



// Удаление данных из списка
void delete_type(TypeWrapper* types, int typeCount, SweetsWrapper* sweets, int sweetCount);
void delete_sweet(SweetsWrapper* sweets, int sweetCount);


// Редактирование данных
void edit_type(TypeWrapper* types, int typeCount, SweetsWrapper* sweets, int sweetCount);
void edit_sweet(SweetsWrapper* sweets, int sweetCount, TypeWrapper* types, int typeCount);


// Формирование новогодних подарков 
static bool is_type_in_gift(ChristmasGift* gift, const char* type_name);
static void add_sweet_to_gift_list(ChristmasGift* gift, const char* type_name, const char* sweet_name);
static int comp_gifts_by_sugar(const void* a, const void* b);
void CreateChristmasGifts(TypeWrapper* types, int typeCount, SweetsWrapper* sweets, int sweetCount, ChristmasGift** gifts, int* giftCount);



// Сортировка данных
int comp_sweet_id(const void* a, const void* b);
int comp_sweet_name(const void* a, const void* b);
int comp_sweet_price(const void* a, const void* b);
int comp_sweet_weight(const void* a, const void* b);
int comp_sweet_sugar(const void* a, const void* b);

int comp_type_id(const void* a, const void* b);
int comp_type_name(const void* a, const void* b);
int comp_type_code(const void* a, const void* b);
int comp_type_count(const void* a, const void* b);


void RefreshSortedSweetsList(SortedSweetsList* list, SweetsWrapper* source, int sourceCount, int (*compar)(const void*, const void*));
void RefreshSortedTypeList(SortedTypeList* list, TypeWrapper* source, int sourceCount, int (*compar)(const void*, const void*));
void UpdateAllSortsSweets(SweetsWrapper* sweets, int s_size, SortedSweetsList* id,
	SortedSweetsList* n, SortedSweetsList* p,
	SortedSweetsList* w, SortedSweetsList* s);
void UpdateAllSortsTypes(TypeWrapper* types, int t_size,
	SortedTypeList* id, SortedTypeList* c,
	SortedTypeList* n, SortedTypeList* s_c);


// Просмотр списка
void display_menu_lists(TypeWrapper* types, int typeCount, SweetsWrapper* sweets, int sweetCount, bool was_formed,
	ChristmasGift* gift, int gift_count);


// Вывод меню/подменю
int display_menu_start(void);
int display_menu(void);
void display_sorted_lists_menu(SweetsSortContainer sweets_sorts, TypesSortContainer types_sorts, TypeWrapper* types, int typeCount);
void display_add_menu(SweetsWrapper** sweets, int* s_size, int* s_capacity, int* s_next_id,
	TypeWrapper** types, int* t_size, int* t_capacity, int* t_next_id,
	SweetsSortContainer sweets_sorts, TypesSortContainer types_sorts);
void display_delete_menu(SweetsWrapper* sweets, int s_size, TypeWrapper* types, int t_size,
	SweetsSortContainer sweets_sorts, TypesSortContainer types_sorts);
void display_edit_menu(SweetsWrapper* sweets, int s_size, TypeWrapper* types, int t_size,
	SweetsSortContainer sweets_sorts, TypesSortContainer types_sorts);
void display_search_menu(SweetsWrapper* sweets, int s_size, TypeWrapper* types, int t_size);


// Табличный вывод данных в консоль
void print_header_sweets();
void print_header_types();
void view_types(TypeWrapper* types, int typeCount);
void view_sweets(SweetsWrapper* sweets, int sweetCount, TypeWrapper* types, int typeCount);
void view_sorted_sweets(SortedSweetsList* list, TypeWrapper* types, int typeCount);
void view_sorted_types(SortedTypeList * list);
void ViewChristmasGifts(ChristmasGift* gifts, int giftCount, bool was_formed);




#endif