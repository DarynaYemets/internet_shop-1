#define _CRT_SECURE_NO_WARNINGS               // Директива, щоб Visual C++ не сварився на старі функції типу scanf/strcpy
#include <iostream>                           // Підключаємо потік введення/виведення
#include <string>                             // Підключаємо клас std::string
#include <fstream>                            // Підключаємо роботу з файлами (ifstream/ofstream)
#include <sstream>                            // Підключаємо stringstream для розбору рядків
#include <iomanip>                            // Підключаємо маніпулятори форматування (setprecision тощо)
#include <ctime>                              // Підключаємо роботу з часом (time, tm)
#include <cctype>                             // Підключаємо функції роботи з символами (isdigit, isspace, isalpha)

#ifdef _WIN32                                 // Якщо компілюємо під Windows
#include <windows.h>                          // Підключаємо заголовок Windows API для налаштування кодування консолі
#endif

using namespace std;                          // Щоб не писати std:: перед кожним ім’ям з стандартної бібліотеки

// ===================== КАТАЛОГ ТОВАРІВ ======================

const int MAX_PRODUCTS = 100;                 // Максимальна кількість товарів у каталозі

int    ids[MAX_PRODUCTS];                     // Масив ID товарів
string namesArr[MAX_PRODUCTS];                // Масив назв товарів
double pricesArr[MAX_PRODUCTS];               // Масив цін товарів
string categoriesArr[MAX_PRODUCTS];           // Масив категорій товарів
int    quantitiesArr[MAX_PRODUCTS];           // Масив кількостей товару на складі

int productCount = 0;                         // Поточна кількість товарів у каталозі

void pressEnterToContinue() {                 // Функція, яка чекає натискання Enter для продовження
    cout << "\nНатисніть Enter, щоб продовжити...";  // Повідомлення користувачу
    cin.ignore(10000, '\n');                  // Ігноруємо залишки в буфері до символу нового рядка
    cin.get();                                // Чекаємо, поки користувач натисне Enter
}

int generateNextId() {                        // Функція для генерації наступного унікального ID
    int maxId = 0;                            // Змінна для збереження максимального знайденого ID
    for (int i = 0; i < productCount; ++i) {  // Проходимо по всіх товарах
        if (ids[i] > maxId) maxId = ids[i];   // Якщо поточний ID більший за maxId — оновлюємо maxId
    }
    return maxId + 1;                         // Повертаємо наступний ID (на 1 більший за максимальний)
}

void printProductCard(int index) {            // Вивід картки товару за індексом у масиві
    cout << "ID: " << ids[index] << "\n"      // Виводимо ID товару
        << "Назва: " << namesArr[index] << "\n"       // Виводимо назву товару
        << "Категорія: " << categoriesArr[index] << "\n" // Виводимо категорію товару
        << "Ціна: " << fixed << setprecision(2)        // Встановлюємо формат з 2 знаками після коми
        << pricesArr[index] << " грн\n"       // Виводимо ціну товару
        << "Кількість на складі: " << quantitiesArr[index] << "\n" // Виводимо кількість на складі
        << "-----------------------------\n";  // Роздільна лінія
}

void printCatalog() {                         // Функція для виводу всього каталогу
    if (productCount == 0) {                  // Якщо товарів ще немає
        cout << "Каталог порожній.\n";        // Виводимо повідомлення
        return;                               // Виходимо з функції
    }
    for (int i = 0; i < productCount; ++i) {  // Інакше проходимо по всіх товарах
        printProductCard(i);                  // І виводимо кожну картку товару
    }
}

void loadCatalogFromFile(const string& filename) { // Завантаження каталогу з текстового файлу
    ifstream fin(filename);                  // Відкриваємо вхідний файл
    if (!fin.is_open()) {                    // Якщо не вдалося відкрити файл
        cout << "Файл \"" << filename << "\" не знайдено. "  // Повідомлення про відсутність файлу
            << "Каталог буде створено з нуля.\n";           // Інформація про створення пустого каталогу
        productCount = 0;                    // Обнуляємо кількість товарів
        return;                              // Виходимо з функції
    }

    productCount = 0;                        // Якщо файл відкрився — починаємо з нуля
    string line;                             // Рядок для читання з файлу
    while (getline(fin, line) && productCount < MAX_PRODUCTS) { // Читаємо файл пострічково до кінця або до межі масиву
        if (line.empty()) continue;          // Пропускаємо порожні рядки

        stringstream ss(line);               // Обгортаємо рядок у stringstream для розбиття по ';'
        string idStr, priceStr, qtyStr;      // Тимчасові рядки для ID, ціни, кількості

        if (!getline(ss, idStr, ';')) continue;                 // Читаємо ID до ';'
        if (!getline(ss, namesArr[productCount], ';')) continue; // Читаємо назву до ';'
        if (!getline(ss, priceStr, ';')) continue;              // Читаємо ціну до ';'
        if (!getline(ss, categoriesArr[productCount], ';')) continue; // Читаємо категорію до ';'
        if (!getline(ss, qtyStr, ';')) continue;                // Читаємо кількість до ';'

        try {                                 // Пробуємо конвертувати рядкові значення у числа
            ids[productCount] = stoi(idStr);  // Перетворення ID у int
            pricesArr[productCount] = stod(priceStr);        // Перетворення ціни у double
            quantitiesArr[productCount] = stoi(qtyStr);      // Перетворення кількості у int
        }
        catch (...) {                         // Якщо сталася будь-яка помилка перетворення
            continue;                         // Пропускаємо цей рядок
        }

        productCount++;                       // Збільшуємо лічильник товарів
    }

    cout << "Каталог завантажено. Кількість товарів: " // Повідомляємо про успішне завантаження
        << productCount << "\n";              // Виводимо кількість товарів
}

void saveCatalogToFile(const string& filename) { // Збереження каталогу у файл
    ofstream fout(filename);                // Відкриваємо вихідний файл
    if (!fout.is_open()) {                  // Якщо не вдалося відкрити
        cout << "Помилка: не вдалося відкрити файл для запису: " // Повідомляємо про помилку
            << filename << "\n";            // Виводимо ім’я файлу
        return;                             // Виходимо з функції
    }

    for (int i = 0; i < productCount; ++i) { // Проходимо по всіх товарах каталогу
        fout << ids[i] << ';'               // Записуємо ID та роздільник
            << namesArr[i] << ';'           // Записуємо назву та роздільник
            << pricesArr[i] << ';'          // Записуємо ціну та роздільник
            << categoriesArr[i] << ';'      // Записуємо категорію та роздільник
            << quantitiesArr[i] << '\n';    // Записуємо кількість та кінець рядка
    }

    cout << "Каталог збережено у файл: " << filename << "\n"; // Повідомляємо, що усе записано
}

int findIndexById(int id) {                 // Пошук індексу товару в масивах за його ID
    for (int i = 0; i < productCount; ++i) { // Перебираємо всі товари
        if (ids[i] == id) return i;         // Якщо ID співпадає — повертаємо індекс
    }
    return -1;                              // Якщо не знайшли — повертаємо -1
}

void addProduct() {                         // Додавання нового товару до каталогу
    if (productCount >= MAX_PRODUCTS) {     // Якщо масив уже заповнений
        cout << "Каталог заповнений (досягнуто MAX_PRODUCTS).\n"; // Повідомлення користувачу
        return;                             // Виходимо
    }

    int index = productCount;               // Індекс нового товару — поточний розмір каталогу

    ids[index] = generateNextId();          // Генеруємо наступний унікальний ID
    cout << "Додавання нового товару\n";    // Повідомляємо про операцію
    cout << "ID призначено автоматично: " << ids[index] << "\n"; // Виводимо новий ID

    cin.ignore(10000, '\n');                // Очищаємо буфер введення від попередніх чисел

    cout << "Введіть назву товару: ";       // Просимо ввести назву
    getline(cin, namesArr[index]);          // Зчитуємо її рядком

    cout << "Введіть категорію: ";          // Просимо ввести категорію
    getline(cin, categoriesArr[index]);     // Зчитуємо як рядок

    cout << "Введіть ціну: ";               // Просимо ввести ціну
    cin >> pricesArr[index];                // Зчитуємо ціну в змінну масиву

    cout << "Введіть кількість: ";          // Просимо ввести кількість
    cin >> quantitiesArr[index];            // Зчитуємо кількість

    productCount++;                         // Збільшуємо кількість товарів
    cout << "Товар успішно додано!\n";      // Повідомляємо, що товар доданий
}

bool deleteProductById(int id) {            // Видалення товару з каталогу за його ID
    int index = findIndexById(id);          // Знаходимо індекс товару
    if (index == -1) return false;          // Якщо не знайшли — повертаємо false

    for (int i = index; i < productCount - 1; ++i) { // Зсуваємо всі елементи після видаленого
        ids[i] = ids[i + 1];               // Пересуваємо ID
        namesArr[i] = namesArr[i + 1];     // Пересуваємо назву
        pricesArr[i] = pricesArr[i + 1];   // Пересуваємо ціну
        categoriesArr[i] = categoriesArr[i + 1]; // Пересуваємо категорію
        quantitiesArr[i] = quantitiesArr[i + 1]; // Пересуваємо кількість
    }

    productCount--;                         // Зменшуємо лічильник товарів
    return true;                            // Повертаємо, що видалення успішне
}

bool editProductById(int id) {              // Редагування товару за ID
    int index = findIndexById(id);          // Знаходимо індекс товару
    if (index == -1) return false;          // Якщо товар не знайдено — повертаємо false

    cout << "Редагування товару з ID " << id << "\n"; // Повідомлення
    printProductCard(index);                // Виводимо поточні дані про товар

    cin.ignore(10000, '\n');                // Очищаємо буфер від попередніх чисел

    string input;                           // Тимчасовий рядок для нового значення

    cout << "Нова назва (залиште порожнім, щоб не змінювати): "; // Питаємо нову назву
    getline(cin, input);                    // Зчитуємо введення
    if (!input.empty()) namesArr[index] = input;   // Якщо рядок не порожній — оновлюємо назву

    cout << "Нова категорія (залиште порожнім, щоб не змінювати): "; // Питаємо нову категорію
    getline(cin, input);                    // Зчитуємо
    if (!input.empty()) categoriesArr[index] = input; // Якщо не порожньо — змінюємо категорію

    cout << "Нова ціна (введіть -1, щоб не змінювати): "; // Питаємо нову ціну
    double newPrice;                        // Змінна для нової ціни
    cin >> newPrice;                        // Зчитуємо значення
    if (newPrice >= 0) pricesArr[index] = newPrice; // Якщо ціна невід’ємна — оновлюємо

    cout << "Нова кількість (введіть -1, щоб не змінювати): "; // Питаємо нову кількість
    int newQty;                             // Змінна для нової кількості
    cin >> newQty;                          // Зчитуємо
    if (newQty >= 0) quantitiesArr[index] = newQty; // Якщо кількість невід’ємна — оновлюємо

    cout << "Товар оновлено.\n";            // Повідомляємо про успішне оновлення
    return true;                            // Повертаємо успіх
}

// Пошук по фрагменту назви
void searchByNameSimple() {                 // Функція простого пошуку товару за фрагментом назви
    cin.ignore(10000, '\n');                // Очищаємо буфер від попередніх числових введень
    cout << "Введіть фрагмент назви для пошуку: "; // Просимо ввести фрагмент
    string pattern;                         // Змінна для пошукового шаблону
    getline(cin, pattern);                  // Зчитуємо рядок

    if (pattern.empty()) {                  // Якщо користувач нічого не ввів
        cout << "Порожній запит.\n";        // Виводимо повідомлення
        return;                             // Виходимо з функції
    }

    bool found = false;                     // Прапорець, чи знайдено хоч один товар
    for (int i = 0; i < productCount; ++i) { // Переглядаємо всі товари
        if (namesArr[i].find(pattern) != string::npos) { // Якщо назва містить введений фрагмент
            printProductCard(i);            // Виводимо картку товару
            found = true;                   // Встановлюємо прапорець
        }
    }

    if (!found) {                           // Якщо нічого не знайшли
        cout << "Товари з такою назвою не знайдено.\n"; // Повідомляємо про це
    }
}

// ПОШУК ЗА ФІЛЬТРАМИ ДЛЯ КОРИСТУВАЧА

void inputFilters(string& name, string& category, double& minPrice, double& maxPrice) { // Ввід фільтрів для пошуку
    cout << "Введіть назву (або '-' для пропуску): "; // Просимо ввести назву або '-'
    cin >> name;                           // Зчитуємо слово (без пробілів)

    cout << "Введіть категорію (або '-' для пропуску): "; // Просимо ввести категорію або '-'
    cin >> category;                       // Зчитуємо категорію

    cout << "Мінімальна ціна: ";           // Просимо мінімальну ціну
    cin >> minPrice;                       // Зчитуємо мінімальну ціну

    cout << "Максимальна ціна: ";          // Просимо максимальну ціну
    cin >> maxPrice;                       // Зчитуємо максимальну ціну
}

bool matchProductFilters(int idx,           // Перевіряємо, чи товар з індексом idx підходить під фільтри
    const string& name,
    const string& category,
    double minP,
    double maxP) {
    bool nameOk = (name == "-" || namesArr[idx].find(name) != string::npos); // Якщо ім’я '-' або міститься в назві
    bool catOk = (category == "-" || categoriesArr[idx] == category);        // Якщо категорія '-' або точний збіг
    bool priceOk = (pricesArr[idx] >= minP && pricesArr[idx] <= maxP);       // Якщо ціна в межах [minP; maxP]
    return nameOk && catOk && priceOk;   // Повертаємо true, якщо всі три умови виконуються
}

void printProductRow(int idx) {             // Вивід товару в один рядок (для таблиці)
    cout << ids[idx] << "\t"                // Виводимо ID
        << namesArr[idx] << "\t"            // Виводимо назву
        << categoriesArr[idx] << "\t"       // Виводимо категорію
        << pricesArr[idx] << " грн\t"       // Виводимо ціну та текст "грн"
        << (quantitiesArr[idx] > 0 ? "В наявності" : "Немає") << endl; // Виводимо статус товару
}

// ===================== МОДУЛЬ ОПЛАТИ ======================

enum class PayMethod { Card, CashOnDelivery }; // Перелік варіантів оплати: картка або післяплата

struct CardData {                            // Структура для збереження даних картки
    string number;   // тільки цифри         // Номер картки
    string expiry;   // MM/YY                // Термін дії у форматі ММ/РР
    string cvv;      // 3 цифри              // CVV код з 3 цифр
    string holder;   // ім'я                 // Ім’я власника картки
};

struct PaymentResult {                       // Структура результату оплати
    bool ok;                                 // Чи успішна операція
    string code;                             // Код результату (APPROVED/DECLINED/INVALID_INPUT)
    string message;                          // Людське повідомлення
    string maskedCard;                       // Замаскований номер картки для логів / чеку
};

// допоміжні функції для string
string digitsOnly(const string& s) {         // Функція повертає лише цифри з рядка
    string r;                                // Результуючий рядок
    for (char ch : s)                        // Перебираємо всі символи
        if (isdigit((unsigned char)ch))      // Якщо це цифра
            r.push_back(ch);                 // Додаємо до результату
    return r;                                // Повертаємо рядок з цифрами
}

string trim(const string& s) {               // Функція обрізає пробіли з початку і кінця рядка
    int i = 0, j = (int)s.size() - 1;        // Індекси початку і кінця
    while (i <= j && isspace((unsigned char)s[i])) i++; // Зсуваємо i вправо, поки натрапляємо на пробіли
    while (j >= i && isspace((unsigned char)s[j])) j--; // Зсуваємо j вліво, поки пробіли
    if (i > j) return "";                    // Якщо рядок повністю з пробілів — повертаємо порожній
    return s.substr(i, j - i + 1);           // Вирізаємо "чисту" частину
}

bool luhnValid(const string& digits) {       // Перевірка номера картки за алгоритмом Луна
    int sum = 0;                             // Сума контрольних цифр
    bool alt = false;                        // Прапорець, чи подвоювати наступну цифру
    for (int i = (int)digits.size() - 1; i >= 0; --i) { // Йдемо з кінця рядка
        if (!isdigit((unsigned char)digits[i])) return false; // Якщо зустріли не цифру — помилка
        int d = digits[i] - '0';            // Перетворюємо символ у число
        if (alt) {                          // Якщо прапорець увімкнений
            d *= 2;                         // Подвоюємо цифру
            if (d > 9) d -= 9;              // Якщо більше 9 — віднімаємо 9
        }
        sum += d;                            // Додаємо до суми
        alt = !alt;                          // Перемикаємо прапорець
    }
    return (sum % 10) == 0;                  // Номер валідний, якщо сума кратна 10
}

bool expiryValid(const string& mmYY) {       // Перевірка строку дії картки у форматі MM/YY
    if (mmYY.size() != 5 || mmYY[2] != '/') return false; // Якщо формат не "??/??" — помилка
    if (!isdigit((unsigned char)mmYY[0]) ||  // Перевіряємо, що всі позиції з цифрами — саме цифри
        !isdigit((unsigned char)mmYY[1]) ||
        !isdigit((unsigned char)mmYY[3]) ||
        !isdigit((unsigned char)mmYY[4])) return false;

    int mm = (mmYY[0] - '0') * 10 + (mmYY[1] - '0'); // Обчислюємо місяць
    int yy = (mmYY[3] - '0') * 10 + (mmYY[4] - '0'); // Обчислюємо рік (останнi двi цифри)
    if (mm < 1 || mm > 12) return false;             // Місяць має бути від 1 до 12

    time_t t = time(nullptr);            // Поточний час у секундах з епохи
    tm now{};                            // Структура для локального часу
#ifdef _WIN32                             // Для Windows
    localtime_s(&now, &t);               // Безпечне перетворення time_t у tm
#else                                     // Для інших систем
    now = *localtime(&t);                // Стандартне перетворення
#endif
    int curMM = now.tm_mon + 1;          // Поточний місяць (tm_mon рахується з 0)
    int curYY = (now.tm_year % 100);     // Поточний рік (останні дві цифри)

    if (yy < curYY) return false;        // Якщо рік уже минув — картка прострочена
    if (yy == curYY && mm < curMM) return false; // Якщо рік поточний, але місяць уже минув — теж прострочена
    return true;                         // Інакше строк дії валідний
}

bool cvvValid(const string& cvv) {        // Перевірка CVV коду
    if (cvv.size() != 3) return false;    // CVV має містити рівно 3 символи
    return isdigit((unsigned char)cvv[0]) && // Усі три мають бути цифрами
        isdigit((unsigned char)cvv[1]) &&
        isdigit((unsigned char)cvv[2]);
}

bool holderValid(const string& h) {       // Перевірка імені власника картки
    string s = trim(h);                   // Обрізаємо пробіли по краях
    if (s.empty()) return false;          // Якщо після обрізки рядок порожній — помилка
    int letters = 0;                      // Лічильник букв
    for (char c : s)                      // Перебираємо всі символи
        if (isalpha((unsigned char)c)) ++letters; // Рахуємо тільки буквені символи
    return letters >= 2;                  // Вважаємо валідним, якщо є хоча б 2 букви
}

string maskCard(const string& digits) {   // Маскування номера картки для відображення
    if (digits.size() < 4) return "****"; // Якщо номер надто короткий — повертаємо 4 зірочки
    string last4 = digits.substr(digits.size() - 4); // Беремо останні 4 цифри
    return "**** **** **** " + last4;     // Формуємо маску типу "**** **** **** 1234"
}

bool restrictedBIN(const string& digits) { // Перевірка на заборонені BIN (перші 6 цифр)
    if (digits.size() < 6) return false;  // Якщо менше 6 цифр — не можемо перевірити
    string bin6 = digits.substr(0, 6);    // Беремо перші 6 цифр
    const string bad[] = { "000000", "123456", "111111" }; // Список "поганих" BINів
    for (const string& s : bad)           // Перебираємо всі заборонені варіанти
        if (bin6 == s) return true;       // Якщо збіглося — BIN заборонений
    return false;                         // Інакше BIN дозволений
}

PaymentResult authorize(double amount, const CardData& cd) { // Імітація авторизації платежу
    if (amount >= 50000.0) {              // Якщо сума занадто велика
        return { false, "DECLINED",       // Відхиляємо операцію
                 "Недостатньо коштів або ліміт перевищено.", // Повідомлення
                 maskCard(cd.number) };   // Маскований номер картки
    }
    if (restrictedBIN(cd.number)) {       // Якщо BIN картки із забороненого списку
        return { false, "DECLINED",       // Теж відхиляємо
                 "Обмежений BIN. Операцію відхилено.", // Повідомлення
                 maskCard(cd.number) };   // Маскований номер
    }
    return { true, "APPROVED",            // Якщо все добре — платіж схвалено
             "Оплата карткою успішна.",   // Текст успіху
             maskCard(cd.number) };       // Маскований номер
}

// >>> ЄДИНИЙ ВИПРАВЛЕНИЙ БАГ: стабільне читання способу оплати <<<

PayMethod readPayMethod() {                // Функція вибору способу оплати користувачем
    cout << "\nОберіть спосіб оплати:\n"   // Виводимо меню варіантів оплати
         << "1 — Банківська картка\n"
         << "2 — Готівка при отриманні\n";

    while (true) {                         // Безкінечний цикл, доки не буде введено коректний варіант
        cout << "Введіть 1 або 2: ";       // Просимо користувача ввести 1 або 2
        int x;                             // Змінна для введеного числа
        if (!(cin >> x)) {                 // Якщо введення не є числом (наприклад, букви)
            cin.clear();                   // Скидаємо стан помилки потоку
            cin.ignore(10000, '\n');       // Викидаємо "сміття" до кінця рядка
            cout << "Невірне значення. Спробуйте ще раз.\n"; // Просимо повторити ввід
            continue;                      // Переходимо на наступну ітерацію циклу
        }

        if (x == 1) return PayMethod::Card;        // Якщо введено 1 — повертаємо оплату карткою
        if (x == 2) return PayMethod::CashOnDelivery; // Якщо 2 — повертаємо оплату при отриманні

        cout << "Невірне значення. Спробуйте ще раз.\n"; // Якщо введено інше число — повторюємо
    }
}

CardData readCard() {                      // Зчитування даних картки з консолі
    CardData cd{};                         // Створюємо порожню структуру CardData
    cin.ignore(10000, '\n');               // Очищаємо буфер після попередніх числових вводів

    cout << "Номер картки (можна з пробілами): ";  // Просимо ввести номер картки
    string tmp;                            // Тимчасовий рядок
    getline(cin, tmp);                     // Зчитуємо повний рядок
    cd.number = digitsOnly(tmp);           // Зберігаємо тільки цифри з введеного рядка

    cout << "Строк дії (MM/YY): ";         // Просимо строк дії
    getline(cin, cd.expiry);               // Зчитуємо строк дії
    cd.expiry = trim(cd.expiry);           // Обрізаємо пробіли

    cout << "CVV (3 цифри): ";             // Просимо CVV
    getline(cin, cd.cvv);                  // Зчитуємо CVV
    cd.cvv = trim(cd.cvv);                 // Обрізаємо пробіли

    cout << "Ім'я та прізвище власника: "; // Просимо ім’я власника
    getline(cin, cd.holder);               // Зчитуємо ім’я
    cd.holder = trim(cd.holder);           // Обрізаємо зайві пробіли

    return cd;                             // Повертаємо заповнену структуру
}

PaymentResult processPayment(double amount, PayMethod method, const CardData& cardIfAny) { // Загальна обробка оплати
    if (!(amount > 0.0 && amount <= 1'000'000.0)) // Перевіряємо, що сума в адекватному діапазоні
        return { false, "INVALID_INPUT",  // Якщо ні — повертаємо помилку
                 "Сума повинна бути більшою за 0 і не перевищувати 1 000 000 грн.",
                 "" };

    if (method == PayMethod::CashOnDelivery) { // Якщо обрано оплату при отриманні
        return { true, "APPROVED",       // Просто підтверджуємо замовлення
                 "Оплата при отриманні. Замовлення підтверджено.",
                 "" };
    }

    if (method == PayMethod::Card) {     // Якщо оплата карткою
        if (cardIfAny.number.size() < 12 || cardIfAny.number.size() > 19) // Перевіряємо довжину номера
            return { false, "INVALID_INPUT", "Невірний номер картки (довжина).", "" };
        if (!luhnValid(cardIfAny.number))      // Перевіряємо номер за алгоритмом Луна
            return { false, "INVALID_INPUT", "Номер картки не проходить Luhn-перевірку.", "" };
        if (!expiryValid(cardIfAny.expiry))    // Перевіряємо строк дії
            return { false, "INVALID_INPUT", "Некоректний строк дії картки.", "" };
        if (!cvvValid(cardIfAny.cvv))          // Перевіряємо CVV
            return { false, "INVALID_INPUT", "CVV має містити рівно 3 цифри.", "" };
        if (!holderValid(cardIfAny.holder))    // Перевіряємо ім’я власника
            return { false, "INVALID_INPUT", "Порожнє або некоректне ім'я власника.", "" };

        return authorize(amount, cardIfAny);   // Якщо всі перевірки успішні — викликаємо authorize
    }

    return { false, "INVALID_INPUT", "Непідтримуваний спосіб оплати.", "" }; // Запасний випадок
}

// ОФОРМЛЕННЯ ЗАМОВЛЕННЯ 

double calculateSum(double price, int quantity) { // Обчислюємо вартість позиції: ціна * кількість
    return price * quantity;                      // Повертаємо результат
}

void makeOrderForProduct(int prodIndex) {         // Оформлення замовлення для одного товару
    if (prodIndex < 0 || prodIndex >= productCount) return; // Перевіряємо коректність індексу

    cout << "\n=== ОФОРМЛЕННЯ ЗАМОВЛЕННЯ ===\n"; // Заголовок блоку
    cout << "Обраний товар: " << namesArr[prodIndex]   // Виводимо назву товару
        << " (" << pricesArr[prodIndex] << " грн)\n";  // І його ціну
    cout << "Доступна кількість: " << quantitiesArr[prodIndex] << "\n"; // Показуємо залишок на складі

    int qty;                                  // Кількість, яку хоче купити користувач
    cout << "Введіть кількість (1.." << quantitiesArr[prodIndex] << "): "; // Просимо ввести кількість
    cin >> qty;                               // Зчитуємо кількість

    if (qty <= 0 || qty > quantitiesArr[prodIndex]) { // Якщо кількість некоректна
        cout << "Некоректна кількість.\n";   // Повідомляємо про помилку
        return;                              // Виходимо
    }

    double sum = calculateSum(pricesArr[prodIndex], qty); // Обчислюємо суму до оплати

    string customerName, customerAddress;     // Змінні для ПІБ покупця та адреси
    cin.ignore(10000, '\n');                  // Очищаємо буфер
    cout << "Ім'я покупця: ";                 // Просимо ввести ім’я
    getline(cin, customerName);               // Зчитуємо ім’я
    cout << "Адреса доставки: ";              // Просимо адресу
    getline(cin, customerAddress);            // Зчитуємо адресу

    cout << "\n=== ПІДСУМКОВИЙ ЧЕК (без оплати) ===\n";   // Виводимо проміжний чек
    cout << namesArr[prodIndex] << " — "      // Назва товару
        << qty << " шт × " << pricesArr[prodIndex]        // Кількість і ціна за одиницю
        << " грн = " << sum << " грн\n";     // Загальна сума
    cout << "Покупець: " << customerName << "\n";         // Виводимо ім’я покупця
    cout << "Адреса: " << customerAddress << "\n";        // Виводимо адресу доставки

    // Оплата
    PayMethod method = readPayMethod();       // Зчитуємо вибір способу оплати (ТУТ ВИПРАВЛЕНИЙ БАГ)
    CardData cd{};                            // Створюємо структуру для даних картки
    if (method == PayMethod::Card) {          // Якщо користувач обрав оплату карткою
        cd = readCard();                      // Зчитуємо реквізити картки
    }

    PaymentResult r = processPayment(sum, method, cd); // Обробляємо оплату і отримуємо результат

    cout << "\n--- РЕЗУЛЬТАТ ОПЛАТИ ---\n";   // Виводимо результат
    cout << "Статус: " << (r.ok ? "УСПІХ" : "ПОМИЛКА") << "\n"; // Показуємо статус
    cout << "Код: " << r.code << "\n";       // Код результату
    cout << "Повідомлення: " << r.message << "\n"; // Текстове повідомлення
    if (!r.maskedCard.empty())               // Якщо є замаскований номер картки
        cout << "Картка: " << r.maskedCard << "\n"; // Виводимо його
    cout << "------------------------\n";    // Роздільна лінія

    if (r.ok) {                              // Якщо оплата пройшла успішно
        quantitiesArr[prodIndex] -= qty;     // Зменшуємо залишок товару на складі
        cout << "Замовлення підтверджено, товар зарезервовано на складі.\n"; // Підтвердження
    }
    else {                                   // Якщо оплата не пройшла
        cout << "Замовлення скасовано через помилку оплати.\n"; // Повідомляємо про скасування
    }
}

// СЦЕНАРІЇ ДЛЯ КОРИСТУВАЧА ЧИ АДМІНА

void userShoppingFlow() {                    // Сценарій покупки для звичайного користувача
    if (productCount == 0) {                 // Якщо каталог порожній
        cout << "Каталог порожній.\n";       // Повідомляємо
        return;                              // Виходимо
    }

    string nameFilter, categoryFilter;       // Фільтри по назві та категорії
    double minPrice, maxPrice;               // Мінімальна та максимальна ціна

    inputFilters(nameFilter, categoryFilter, minPrice, maxPrice); // Зчитуємо фільтри від користувача

    cout << "\nРезультати пошуку:\n";        // Заголовок списку результатів
    cout << "ID\tНазва\tКатегорія\tЦіна\tСтатус\n"; // Шапка таблиці

    bool found = false;                      // Прапорець, чи щось знайдено
    int foundIds[MAX_PRODUCTS];              // Масив ID знайдених товарів
    int foundCount = 0;                      // Кількість знайдених товарів

    for (int i = 0; i < productCount; ++i) { // Перебираємо всі товари каталогу
        if (matchProductFilters(i, nameFilter, categoryFilter, minPrice, maxPrice)) { // Перевіряємо фільтри
            printProductRow(i);              // Виводимо рядок з інформацією про товар
            found = true;                    // Встановлюємо прапорець
            foundIds[foundCount++] = ids[i]; // Запам’ятовуємо ID знайденого товару
        }
    }

    if (!found) {                            // Якщо нічого не знайдено
        cout << "\nНічого не знайдено.\n";   // Виводимо повідомлення
        return;                              // Виходимо
    }

    int selectedID;                          // ID товару, який користувач хоче купити
    cout << "\nВведіть ID товару для оформлення замовлення: "; // Просимо ввести ID
    cin >> selectedID;                       // Зчитуємо ID

    int idx = findIndexById(selectedID);     // Знаходимо індекс у масиві за ID
    if (idx == -1) {                         // Якщо не знайшли
        cout << "Товар з таким ID не знайдено.\n"; // Повідомляємо
        return;                              // Виходимо
    }
    if (quantitiesArr[idx] <= 0) {           // Якщо товар відсутній на складі
        cout << "Цей товар відсутній на складі.\n"; // Повідомляємо
        return;                              // Виходимо
    }

    makeOrderForProduct(idx);                // Запускаємо оформлення замовлення
}

void adminCatalogMenu(const string& filename) { // Меню адміністратора для роботи з каталогом
    while (true) {                         // Цикл до виходу з адмін-меню
        cout << "\n=== АДМІН-МЕНЮ КАТАЛОГУ ===\n"; // Заголовок меню
        cout << "1 - Показати всі товари\n";       // Пункт 1: показати каталог
        cout << "2 - Пошук товарів за назвою\n";   // Пункт 2: простий пошук
        cout << "3 - Додати товар\n";              // Пункт 3: додати товар
        cout << "4 - Видалити товар за ID\n";      // Пункт 4: видалити товар
        cout << "5 - Редагувати товар за ID\n";    // Пункт 5: редагувати товар
        cout << "0 - Вихід у головне меню (зберегти каталог)\n"; // Пункт 0: вихід і збереження
        cout << "Ваш вибір: ";                     // Запит вибору

        int choice;                                // Змінна для вибору меню
        cin >> choice;                             // Зчитуємо пункт

        switch (choice) {                          // Обробляємо вибір
        case 1:                                    // Якщо обрали 1
            printCatalog();                        // Виводимо всі товари
            pressEnterToContinue();                // Чекаємо, поки користувач натисне Enter
            break;                                 // Повертаємось до меню
        case 2:                                    // Якщо обрали 2
            searchByNameSimple();                  // Виконуємо пошук за фрагментом назви
            pressEnterToContinue();                // Чекаємо Enter
            break;                                 // Повертаємось до меню
        case 3:                                    // Якщо обрали 3
            addProduct();                          // Додаємо новий товар
            pressEnterToContinue();                // Чекаємо Enter
            break;                                 // Повертаємось до меню
        case 4: {                                  // Якщо обрали 4 (блок у фігурних дужках для змінних)
            cout << "Введіть ID товару для видалення: "; // Просимо ввести ID
            int id;                                // Змінна для ID
            cin >> id;                             // Зчитуємо ID
            if (deleteProductById(id))             // Якщо видалення успішне
                cout << "Товар видалено.\n";       // Повідомляємо про видалення
            else
                cout << "Товар з таким ID не знайдено.\n"; // Якщо ID не знайдено
            pressEnterToContinue();                // Чекаємо Enter
            break;                                 // Повертаємось до меню
        }
        case 5: {                                  // Якщо обрали 5
            cout << "Введіть ID товару для редагування: "; // Просимо ID
            int id;                                // Змінна для ID
            cin >> id;                             // Зчитуємо
            if (!editProductById(id)) {            // Якщо редагування не відбулося (ID не знайдений)
                cout << "Товар з таким ID не знайдено.\n"; // Повідомляємо
            }
            pressEnterToContinue();                // Чекаємо Enter
            break;                                 // Повертаємось до меню
        }
        case 0:                                    // Якщо обрали 0
            saveCatalogToFile(filename);           // Зберігаємо каталог у файл
            return;                                // Виходимо з адмін-меню в головне
        default:                                   // Будь-який інший ввід
            cout << "Невірний пункт меню. Спробуйте ще раз.\n"; // Повідомляємо про помилку
            pressEnterToContinue();                // Чекаємо Enter
            break;                                 // Повертаємось до меню
        }
    }
}

// ===================== MAIN ======================

int main() {                                      // Точка входу в програму
#ifdef _WIN32                                     // Якщо Windows
    SetConsoleOutputCP(1251);                     // Встановлюємо кодування виводу консолі на Windows-1251
    SetConsoleCP(1251);                           // Встановлюємо кодування введення теж на 1251
#endif

    const string filename = "products.txt";       // Ім’я файлу каталогу
    loadCatalogFromFile(filename);                // Завантажуємо каталог з файлу

    while (true) {                                // Головний цикл програми
        cout << "\n=========== ІНТЕРНЕТ-МАГАЗИН ===========\n"; // Заголовок головного меню
        cout << "1 - Переглянути весь каталог\n";              // Пункт 1: показати каталог
        cout << "2 - Пошук товарів та оформлення замовлення (user)\n"; // Пункт 2: сценарій покупця
        cout << "3 - Адмін-режим (керування каталогом)\n";     // Пункт 3: адмін-меню
        cout << "0 - Вихід\n";                                // Пункт 0: вихід з програми
        cout << "Ваш вибір: ";                                // Запит вибору

        int choice;                                           // Змінна для вибору
        cin >> choice;                                        // Зчитуємо вибір

        switch (choice) {                                     // Обробляємо вибір
        case 1:                                               // Якщо 1
            printCatalog();                                   // Показуємо весь каталог
            pressEnterToContinue();                           // Чекаємо Enter
            break;                                            // Повертаємось до меню
        case 2:                                               // Якщо 2
            userShoppingFlow();                               // Запускаємо сценарій користувача
            pressEnterToContinue();                           // Чекаємо Enter
            break;                                            // Повертаємось до меню
        case 3:                                               // Якщо 3
            adminCatalogMenu(filename);                       // Переходимо в адмін-меню
            break;                                            // Після виходу з адмін-меню повертаємось
        case 0:                                               // Якщо 0
            saveCatalogToFile(filename);                      // Перед виходом зберігаємо каталог
            cout << "Вихід з програми.\n";                    // Повідомляємо про завершення
            return 0;                                         // Повертаємо 0 — успішне завершення
        default:                                              // Якщо введено щось інше
            cout << "Невірний пункт меню.\n";                 // Повідомляємо про помилку
            pressEnterToContinue();                           // Чекаємо Enter
            break;                                            // Повертаємось до меню
        }
    }

    return 0;                                                 // Запасне повернення (фактично недосяжне)
}
