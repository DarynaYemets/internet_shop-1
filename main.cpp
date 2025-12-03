#define _CRT_SECURE_NO_WARNINGS                             // Дозволяємо деякі функції CRT на Windows
#include <iostream>                                         // Підключаємо стандартний ввід/вивід
#include <string>                                           // Підключаємо рядки std::string
#include <fstream>                                          // Підключаємо файлові потоки
#include <sstream>                                          // Підключаємо stringstream для парсингу
#include <iomanip>                                          // Підключаємо маніпулятори форматування
#include <ctime>                                            // Підключаємо роботу з часом
#include <cctype>                                           // Підключаємо функції для символів (isdigit тощо)

#ifdef _WIN32                                               // Якщо компілюємо під Windows
#include <windows.h>                                        // Підключаємо Windows API для кодувань консолі
#endif

using namespace std;                                        // Щоб не писати std:: перед кожним іменем

// ===================== КАТАЛОГ ТОВАРІВ ======================

const int MAX_PRODUCTS = 100;                               // Максимальна кількість товарів у каталозі

int    ids[MAX_PRODUCTS];                                   // Масив ID товарів
string namesArr[MAX_PRODUCTS];                              // Масив назв товарів
double pricesArr[MAX_PRODUCTS];                             // Масив цін
string categoriesArr[MAX_PRODUCTS];                         // Масив категорій
int    quantitiesArr[MAX_PRODUCTS];                         // Масив кількостей на складі

int productCount = 0;                                       // Поточна кількість товарів

void pressEnterToContinue() {                               // Функція для паузи після дій
    cout << "\nНатисніть Enter, щоб продовжити...";         // Повідомлення користувачу
    cin.ignore(10000, '\n');                                // Скидаємо все до \n у буфері
    cin.get();                                              // Чекаємо натискання Enter
}

int generateNextId() {                                      // Генерація наступного доступного ID
    int maxId = 0;                                          // Початкове максимальне значення
    for (int i = 0; i < productCount; ++i) {                // Проходимо по всіх товарах
        if (ids[i] > maxId) maxId = ids[i];                 // Шукаємо найбільший ID
    }
    return maxId + 1;                                       // Повертаємо наступний ID
}

void printProductCard(int index) {                          // Вивід інформації про один товар
    cout << "ID: " << ids[index] << "\n"                    // Виводимо ID
        << "Назва: " << namesArr[index] << "\n"             // Виводимо назву
        << "Категорія: " << categoriesArr[index] << "\n"    // Виводимо категорію
        << "Ціна: " << fixed << setprecision(2)             // Фіксований формат з 2 знаками
        << pricesArr[index] << " грн\n"                     // Виводимо ціну
        << "Кількість на складі: " << quantitiesArr[index]  // Виводимо кількість
        << "\n-----------------------------\n";              // Роздільник
}

void printCatalog() {                                       // Вивід усього каталогу
    if (productCount == 0) {                                // Якщо каталог порожній
        cout << "Каталог порожній.\n";                      // Пишемо про це
        return;                                             // Виходимо
    }
    for (int i = 0; i < productCount; ++i) {                // Інакше проходимо по всіх товарах
        printProductCard(i);                                // І виводимо кожен
    }
}

void loadCatalogFromFile(const string& filename) {          // Завантаження каталогу з текстового файлу
    ifstream fin(filename);                                 // Відкриваємо файл для читання
    if (!fin.is_open()) {                                   // Якщо файл не відкрився
        cout << "Файл \"" << filename << "\" не знайдено. " // Повідомляємо про відсутність файлу
            << "Каталог буде створено з нуля.\n";           // Кажемо, що починаємо з пустого каталогу
        productCount = 0;                                   // Обнуляємо кількість товарів
        return;                                             // Виходимо з функції
    }

    productCount = 0;                                       // Обнуляємо лічильник перед читанням
    string line;                                            // Змінна для рядка з файлу

    while (getline(fin, line) && productCount < MAX_PRODUCTS) { // Читаємо файл пострічково
        if (line.empty()) continue;                         // Пропускаємо порожні рядки

        stringstream ss(line);                              // Створюємо stringstream для парсингу
        string idStr, priceStr, qtyStr;                     // Тимчасові змінні для полів

        if (!getline(ss, idStr, ';')) continue;             // Зчитуємо ID
        if (!getline(ss, namesArr[productCount], ';')) continue;   // Зчитуємо назву
        if (!getline(ss, priceStr, ';')) continue;          // Зчитуємо ціну
        if (!getline(ss, categoriesArr[productCount], ';')) continue;// Зчитуємо категорію
        if (!getline(ss, qtyStr, ';')) continue;            // Зчитуємо кількість

        try {                                               // Пробуємо конвертувати строки в числа
            ids[productCount] = stoi(idStr);                // Перетворюємо ID у int
            pricesArr[productCount] = stod(priceStr);       // Перетворюємо ціну у double
            quantitiesArr[productCount] = stoi(qtyStr);     // Перетворюємо кількість у int
        }
        catch (...) {                                       // Якщо сталася помилка конвертації
            continue;                                       // Просто пропускаємо цей рядок
        }

        productCount++;                                     // Збільшуємо кількість товарів
    }

    cout << "Каталог завантажено. Кількість товарів: "      // Повідомляємо скільки прочитали
        << productCount << "\n";                            // Виводимо число
}

void saveCatalogToFile(const string& filename) {            // Збереження каталогу у файл
    ofstream fout(filename);                                // Відкриваємо файл для запису
    if (!fout.is_open()) {                                  // Якщо файл не відкрився
        cout << "Помилка: не вдалося відкрити файл для запису: "
            << filename << "\n";                            // Виводимо повідомлення про помилку
        return;                                             // Виходимо
    }

    for (int i = 0; i < productCount; ++i) {                // Проходимо всі товари
        fout << ids[i] << ';'                               // Пишемо ID
            << namesArr[i] << ';'                           // Пишемо назву
            << pricesArr[i] << ';'                          // Пишемо ціну
            << categoriesArr[i] << ';'                      // Пишемо категорію
            << quantitiesArr[i] << '\n';                    // Пишемо кількість і кінець рядка
    }

    cout << "Каталог збережено у файл: " << filename << "\n"; // Інформаційне повідомлення
}

int findIndexById(int id) {                                 // Пошук індекса товару по ID
    for (int i = 0; i < productCount; ++i) {                // Переглядаємо всі товари
        if (ids[i] == id) return i;                         // Якщо ID співпав — повертаємо індекс
    }
    return -1;                                              // Якщо товар не знайдено — повертаємо -1
}

void addProduct() {                                         // Додавання нового товару
    if (productCount >= MAX_PRODUCTS) {                     // Якщо каталог вже заповнений
        cout << "Каталог заповнений (досягнуто MAX_PRODUCTS).\n"; // Повідомляємо про неможливість додати
        return;                                             // Виходимо
    }

    int index = productCount;                               // Новий товар піде в позицію productCount

    ids[index] = generateNextId();                          // Генеруємо новий ID
    cout << "Додавання нового товару\n";                    // Інфо-повідомлення
    cout << "ID призначено автоматично: " << ids[index] << "\n"; // Показуємо ID

    cin.ignore(10000, '\n');                                // Очищаємо буфер після попереднього вводу

    cout << "Введіть назву товару: ";                       // Питаємо назву
    getline(cin, namesArr[index]);                          // Зчитуємо повністю рядок

    cout << "Введіть категорію: ";                          // Питаємо категорію
    getline(cin, categoriesArr[index]);                     // Зчитуємо рядок

    cout << "Введіть ціну: ";                               // Питаємо ціну
    cin >> pricesArr[index];                                // Зчитуємо double

    cout << "Введіть кількість: ";                          // Питаємо кількість
    cin >> quantitiesArr[index];                            // Зчитуємо int

    productCount++;                                         // Збільшуємо кількість товарів
    cout << "Товар успішно додано!\n";                      // Підтвердження
}

bool deleteProductById(int id) {                            // Видалення товару по ID
    int index = findIndexById(id);                          // Шукаємо індекс
    if (index == -1) return false;                          // Якщо не знайшли — повертаємо false

    for (int i = index; i < productCount - 1; ++i) {        // Зсуваємо всі елементи після видаленого
        ids[i] = ids[i + 1];                                // Переписуємо ID
        namesArr[i] = namesArr[i + 1];                      // Переписуємо назву
        pricesArr[i] = pricesArr[i + 1];                    // Переписуємо ціну
        categoriesArr[i] = categoriesArr[i + 1];            // Переписуємо категорію
        quantitiesArr[i] = quantitiesArr[i + 1];            // Переписуємо кількість
    }

    productCount--;                                         // Зменшуємо кількість товарів
    return true;                                            // Повертаємо успішний результат
}

bool editProductById(int id) {                              // Редагування товару по ID
    int index = findIndexById(id);                          // Шукаємо індекс
    if (index == -1) return false;                          // Якщо не знайшли — повертаємо false

    cout << "Редагування товару з ID " << id << "\n";       // Інфо-повідомлення
    printProductCard(index);                                // Показуємо поточні дані

    cin.ignore(10000, '\n');                                // Очищаємо буфер

    string input;                                           // Тимчасовий рядок

    cout << "Нова назва (залиште порожнім, щоб не змінювати): "; // Питаємо нову назву
    getline(cin, input);                                    // Зчитуємо
    if (!input.empty()) namesArr[index] = input;            // Якщо не порожня — міняємо

    cout << "Нова категорія (залиште порожнім, щоб не змінювати): "; // Питаємо категорію
    getline(cin, input);                                    // Зчитуємо
    if (!input.empty()) categoriesArr[index] = input;       // Якщо не порожня — міняємо

    cout << "Нова ціна (введіть -1, щоб не змінювати): ";   // Питаємо ціну
    double newPrice;                                       // Нова ціна
    cin >> newPrice;                                        // Зчитуємо
    if (newPrice >= 0) pricesArr[index] = newPrice;         // Якщо >=0 — оновлюємо

    cout << "Нова кількість (введіть -1, щоб не змінювати): "; // Питаємо кількість
    int newQty;                                            // Нова кількість
    cin >> newQty;                                         // Зчитуємо
    if (newQty >= 0) quantitiesArr[index] = newQty;         // Якщо >=0 — оновлюємо

    cout << "Товар оновлено.\n";                            // Підтвердження
    return true;                                            // Повертаємо успіх
}

void searchByNameSimple() {                                 // Пошук товарів за фрагментом назви
    cin.ignore(10000, '\n');                                // Очищаємо буфер перед getline
    cout << "Введіть фрагмент назви для пошуку: ";          // Просимо ввести фрагмент
    string pattern;                                        // Рядок-патерн
    getline(cin, pattern);                                  // Зчитуємо повністю

    if (pattern.empty()) {                                  // Якщо нічого не ввели
        cout << "Порожній запит.\n";                        // Повідомляємо
        return;                                             // Виходимо
    }

    bool found = false;                                     // Прапорець, чи щось знайдено
    for (int i = 0; i < productCount; ++i) {                // Перебираємо всі товари
        if (namesArr[i].find(pattern) != string::npos) {    // Якщо підрядок знайдений у назві
            printProductCard(i);                            // Виводимо товар
            found = true;                                   // Ставимо прапорець
        }
    }

    if (!found) {                                           // Якщо нічого не знайшли
        cout << "Товари з такою назвою не знайдено.\n";     // Повідомляємо
    }
}

// ===================== ПОШУК ЗА ФІЛЬТРАМИ (МОДУЛЬ ПОШУКУ) ======================

void inputFilters(string& name, string& category, double& minPrice, double& maxPrice) {
    cin.ignore(10000, '\n');                                // ОЧИЩЕННЯ БУФЕРА ПЕРЕД getline (ВИПРАВЛЕНИЙ БАГ №1)

    cout << "Введіть назву (або '-' для пропуску): ";       // Просимо ввести назву або '-'
    getline(cin, name);                                     // Читаємо повний рядок з пробілами
    if (name.empty()) {                                     // Якщо залишили порожнім
        name = "-";                                         // Вважаємо це пропуском фільтра
    }

    cout << "Введіть категорію (або '-' для пропуску): ";   // Просимо ввести категорію
    getline(cin, category);                                 // Читаємо рядок
    if (category.empty()) {                                 // Якщо пусто
        category = "-";                                     // Також пропуск фільтра
    }

    cout << "Мінімальна ціна: ";                            // Питаємо мінімальну ціну
    cin >> minPrice;                                        // Зчитуємо double

    cout << "Максимальна ціна: ";                           // Питаємо максимальну ціну
    cin >> maxPrice;                                        // Зчитуємо double
}

bool matchProductFilters(int idx,                           // Перевірка чи товар idx проходить фільтри
    const string& name,
    const string& category,
    double minP,
    double maxP) 
{
    bool skipName = (name == "-" || name == "_" || name.empty());   // Чи пропускаємо фільтр по назві (ВИПРАВЛЕНИЙ БАГ №2)
    bool skipCat  = (category == "-" || category == "_" || category.empty()); // Чи пропускаємо фільтр по категорії

    bool nameOk = (skipName || namesArr[idx].find(name) != string::npos); // Якщо не пропускаємо — шукаємо підрядок
    bool catOk  = (skipCat  || categoriesArr[idx] == category);           // Якщо не пропускаємо — порівнюємо категорію
    bool priceOk = (pricesArr[idx] >= minP && pricesArr[idx] <= maxP);    // Перевіряємо попадання в діапазон цін

    return nameOk && catOk && priceOk;                                   // Повертаємо результат перевірки всіх фільтрів
}

void printProductRow(int idx) {                               // Короткий вивід товару в один рядок
    cout << ids[idx] << "\t"                                  // Виводимо ID
        << namesArr[idx] << "\t"                              // Виводимо назву
        << categoriesArr[idx] << "\t"                         // Виводимо категорію
        << pricesArr[idx] << " грн\t"                         // Виводимо ціну
        << (quantitiesArr[idx] > 0 ? "В наявності" : "Немає") // Статус на складі
        << endl;                                              // Перехід рядка
}

// ===================== МОДУЛЬ ОПЛАТИ ======================

enum class PayMethod { Card, CashOnDelivery };           // Перелік способів оплати

struct CardData {                                        // Структура для даних картки
    string number;                                       // Номер картки (тільки цифри)
    string expiry;                                       // Строк дії MM/YY
    string cvv;                                          // CVV 3 цифри
    string holder;                                       // Ім’я власника
};

struct PaymentResult {                                   // Результат операції оплати
    bool ok;                                             // true, якщо успішно
    string code;                                         // Код результату
    string message;                                      // Текстове повідомлення
    string maskedCard;                                   // Маскований номер картки
};

string digitsOnly(const string& s) {                     // Повертає тільки цифри з рядка
    string r;                                            // Результат
    for (char ch : s)                                    // Перебираємо всі символи
        if (isdigit((unsigned char)ch))                  // Якщо це цифра
            r.push_back(ch);                             // Додаємо до результату
    return r;                                            // Повертаємо тільки цифри
}

string trim(const string& s) {                           // Обрізає пробіли з початку і кінця
    int i = 0;                                           // Індекс зліва
    int j = (int)s.size() - 1;                           // Індекс справа
    while (i <= j && isspace((unsigned char)s[i])) i++;   // Рухаємось справа наліво доки пробіл
    while (j >= i && isspace((unsigned char)s[j])) j--;   // Рухаємось зліва направо доки пробіл
    if (i > j) return "";                                // Якщо рядок пустий
    return s.substr(i, j - i + 1);                       // Повертаємо обрізану частину
}

bool luhnValid(const string& digits) {                   // Перевірка номера картки алгоритмом Луна
    int sum = 0;                                         // Сума для перевірки
    bool alt = false;                                    // Прапорець через один символ
    for (int i = (int)digits.size() - 1; i >= 0; --i) {  // Йдемо з кінця
        if (!isdigit((unsigned char)digits[i])) return false; // Якщо не цифра — помилка
        int d = digits[i] - '0';                         // Перетворюємо символ у число
        if (alt) {                                       // Якщо потрібно подвоїти
            d *= 2;                                      // Подвоюємо
            if (d > 9) d -= 9;                           // Якщо більше 9 — віднімаємо 9
        }
        sum += d;                                        // Додаємо до суми
        alt = !alt;                                      // Перемикаємо прапорець
    }
    return (sum % 10) == 0;                              // Правильний номер, якщо сума кратна 10
}

bool expiryValid(const string& mmYY) {                   // Перевірка строку дії картки
    if (mmYY.size() != 5 || mmYY[2] != '/') return false; // Формат має бути MM/YY
    if (!isdigit((unsigned char)mmYY[0]) ||              // Перевіряємо всі цифри
        !isdigit((unsigned char)mmYY[1]) ||
        !isdigit((unsigned char)mmYY[3]) ||
        !isdigit((unsigned char)mmYY[4])) return false;

    int mm = (mmYY[0] - '0') * 10 + (mmYY[1] - '0');     // Місяць
    int yy = (mmYY[3] - '0') * 10 + (mmYY[4] - '0');     // Рік (2 останні цифри)
    if (mm < 1 || mm > 12) return false;                 // Місяць має бути 1..12

    time_t t = time(nullptr);                            // Поточний час у секундах
    tm now{};                                            // Структура дати/часу

#ifdef _WIN32                                            // Для Windows використовуємо localtime_s
    localtime_s(&now, &t);
#else                                                     // Для інших ОС — localtime
    now = *localtime(&t);
#endif

    int curMM = now.tm_mon + 1;                          // Поточний місяць (0..11 → 1..12)
    int curYY = (now.tm_year % 100);                     // Поточний рік (2 цифри)

    if (yy < curYY) return false;                        // Якщо рік менший — карта прострочена
    if (yy == curYY && mm < curMM) return false;         // Якщо той самий рік, але менший місяць — теж
    return true;                                         // Інакше строк дії валідний
}

bool cvvValid(const string& cvv) {                       // Перевірка CVV
    if (cvv.size() != 3) return false;                   // Має бути рівно 3 символи
    return isdigit((unsigned char)cvv[0]) &&             // Усі три мають бути цифрами
        isdigit((unsigned char)cvv[1]) &&
        isdigit((unsigned char)cvv[2]);
}

bool holderValid(const string& h) {                      // Перевірка імені власника
    string s = trim(h);                                  // Обрізаємо пробіли
    if (s.empty()) return false;                         // Порожнє ім’я — не ок
    int letters = 0;                                     // Лічильник літер
    for (char c : s)                                     // Перебираємо символи
        if (isalpha((unsigned char)c)) ++letters;        // Якщо літера — додаємо
    return letters >= 2;                                 // Має бути принаймні 2 літери
}

string maskCard(const string& digits) {                  // Маскуємо номер картки
    if (digits.size() < 4) return "****";                // Якщо менше 4 цифр — просто ****
    string last4 = digits.substr(digits.size() - 4);     // Беремо останні 4 цифри
    return "**** **** **** " + last4;                    // Повертаємо маскований формат
}

bool restrictedBIN(const string& digits) {               // Перевірка «поганих» BIN-ів
    if (digits.size() < 6) return false;                 // Якщо менше 6 цифр — не перевіряємо
    string bin6 = digits.substr(0, 6);                   // Беремо перші 6 цифр
    const string bad[] = { "000000", "123456", "111111" };// Масив заборонених
    for (const string& s : bad)                          // Перебираємо всі
        if (bin6 == s) return true;                      // Якщо збіг — BIN заборонений
    return false;                                        // Інакше — ОК
}

PaymentResult authorize(double amount, const CardData& cd) { // Імітація авторизації платежу
    if (amount >= 50000.0) {                             // Якщо сума занадто велика
        return { false, "DECLINED",                     // Відхиляємо
                 "Недостатньо коштів або ліміт перевищено.",
                 maskCard(cd.number) };                 // Повертаємо масковану картку
    }
    if (restrictedBIN(cd.number)) {                     // Якщо BIN заборонений
        return { false, "DECLINED",                     // Відхиляємо
                 "Обмежений BIN. Операцію відхилено.",
                 maskCard(cd.number) };                 // Повертаємо масковану картку
    }
    return { true, "APPROVED",                          // Інакше — схвалено
             "Оплата карткою успішна.",
             maskCard(cd.number) };                     // Маскований номер картки
}

PayMethod readPayMethod() {                             // Зчитування способу оплати (ВИПРАВЛЕНИЙ БАГ №3)
    cout << "\nОберіть спосіб оплати:\n"                 // Виводимо меню
         << "1 — Банківська картка\n"
         << "2 — Готівка при отриманні\n";

    while (true) {                                      // Нескінченний цикл поки не введемо коректно
        cout << "Введіть 1 або 2: ";                    // Запитуємо значення
        int x;                                          // Змінна для вводу
        if (!(cin >> x)) {                              // Якщо ввели не число (наприклад abc)
            cin.clear();                                // Скидаємо стан потоку
            cin.ignore(10000, '\n');                    // Викидаємо сміття з буфера
            cout << "Невірне значення. Спробуйте ще раз.\n"; // Пояснюємо помилку
            continue;                                   // Переходимо до наступної спроби
        }

        if (x == 1) return PayMethod::Card;             // Якщо 1 — оплата карткою
        if (x == 2) return PayMethod::CashOnDelivery;   // Якщо 2 — готівка при отриманні

        cout << "Невірне значення. Спробуйте ще раз.\n"; // Якщо інше число — неправильний вибір
    }
}

CardData readCard() {                                   // Зчитування даних картки з консолі
    CardData cd{};                                      // Створюємо структуру
    cin.ignore(10000, '\n');                            // Очищаємо буфер після попереднього вводу

    cout << "Номер картки (можна з пробілами): ";       // Просимо номер картки
    string tmp;                                         // Тимчасовий рядок
    getline(cin, tmp);                                  // Зчитуємо повністю
    cd.number = digitsOnly(tmp);                        // Залишаємо тільки цифри

    cout << "Строк дії (MM/YY): ";                      // Просимо строк дії
    getline(cin, cd.expiry);                            // Зчитуємо
    cd.expiry = trim(cd.expiry);                        // Обрізаємо пробіли

    cout << "CVV (3 цифри): ";                          // Просимо CVV
    getline(cin, cd.cvv);                               // Зчитуємо
    cd.cvv = trim(cd.cvv);                              // Обрізаємо пробіли

    cout << "Ім'я та прізвище власника: ";              // Просимо ім’я власника
    getline(cin, cd.holder);                            // Зчитуємо
    cd.holder = trim(cd.holder);                        // Обрізаємо пробіли

    return cd;                                          // Повертаємо структуру з даними
}

PaymentResult processPayment(double amount, PayMethod method, const CardData& cardIfAny) {
    if (!(amount > 0.0 && amount <= 1'000'000.0))        // Перевіряємо адекватність суми
        return { false, "INVALID_INPUT",                 // Якщо ні — помилка вводу
                 "Сума повинна бути більшою за 0 і не перевищувати 1 000 000 грн.",
                 "" };

    if (method == PayMethod::CashOnDelivery) {           // Якщо обрана оплата при отриманні
        return { true, "APPROVED",                       // Вважаємо, що все ок
                 "Оплата при отриманні. Замовлення підтверджено.",
                 "" };
    }

    if (method == PayMethod::Card) {                     // Якщо оплата карткою
        if (cardIfAny.number.size() < 12 || cardIfAny.number.size() > 19)
            return { false, "INVALID_INPUT",             // Перевіряємо довжину номера
                     "Невірний номер картки (довжина).",
                     "" };
        if (!luhnValid(cardIfAny.number))                // Перевіряємо алгоритмом Луна
            return { false, "INVALID_INPUT",
                     "Номер картки не проходить Luhn-перевірку.",
                     "" };
        if (!expiryValid(cardIfAny.expiry))              // Перевіряємо строк дії
            return { false, "INVALID_INPUT",
                     "Некоректний строк дії картки.",
                     "" };
        if (!cvvValid(cardIfAny.cvv))                    // Перевіряємо CVV
            return { false, "INVALID_INPUT",
                     "CVV має містити рівно 3 цифри.",
                     "" };
        if (!holderValid(cardIfAny.holder))              // Перевіряємо ім’я власника
            return { false, "INVALID_INPUT",
                     "Порожнє або некоректне ім'я власника.",
                     "" };

        return authorize(amount, cardIfAny);             // Якщо всі перевірки ОК — авторизуємо
    }

    return { false, "INVALID_INPUT",                     // Сюди попадемо, якщо method невідомий
             "Непідтримуваний спосіб оплати.",
             "" };
}

// ===================== ОФОРМЛЕННЯ ЗАМОВЛЕННЯ ======================

double calculateSum(double price, int quantity) {         // Обчислення суми за товар
    return price * quantity;                              // Просто множимо ціну на кількість
}

void makeOrderForProduct(int prodIndex) {                 // Оформлення замовлення на конкретний товар
    if (prodIndex < 0 || prodIndex >= productCount) return; // Захист від некоректного індекса

    cout << "\n=== ОФОРМЛЕННЯ ЗАМОВЛЕННЯ ===\n";          // Заголовок
    cout << "Обраний товар: " << namesArr[prodIndex]     // Показуємо назву
        << " (" << pricesArr[prodIndex] << " грн)\n";    // Показуємо ціну
    cout << "Доступна кількість: " << quantitiesArr[prodIndex] << "\n"; // Показуємо залишок

    int qty;                                             // Змінна для кількості
    cout << "Введіть кількість (1.." << quantitiesArr[prodIndex] << "): "; // Просимо кількість
    cin >> qty;                                          // Зчитуємо

    if (qty <= 0 || qty > quantitiesArr[prodIndex]) {    // Перевіряємо коректність
        cout << "Некоректна кількість.\n";               // Повідомляємо про помилку
        return;                                          // Виходимо
    }

    double sum = calculateSum(pricesArr[prodIndex], qty); // Обчислюємо суму

    string customerName, customerAddress;                // Дані клієнта
    cin.ignore(10000, '\n');                             // Очищаємо буфер
    cout << "Ім'я покупця: ";                            // Питаємо ім’я
    getline(cin, customerName);                          // Зчитуємо
    cout << "Адреса доставки: ";                         // Питаємо адресу
    getline(cin, customerAddress);                       // Зчитуємо

    cout << "\n=== ПІДСУМКОВИЙ ЧЕК (без оплати) ===\n";  // Виводимо попередній чек
    cout << namesArr[prodIndex] << " — "                 // Назва
        << qty << " шт × " << pricesArr[prodIndex]       // Кількість і ціна
        << " грн = " << sum << " грн\n";                 // Загальна сума
    cout << "Покупець: " << customerName << "\n";        // Ім’я покупця
    cout << "Адреса: " << customerAddress << "\n";       // Адреса

    PayMethod method = readPayMethod();                  // Читаємо спосіб оплати
    CardData cd{};                                       // Порожні дані картки
    if (method == PayMethod::Card) {                     // Якщо оплата карткою
        cd = readCard();                                 // Зчитуємо дані картки
    }

    PaymentResult r = processPayment(sum, method, cd);   // Обробляємо оплату

    cout << "\n--- РЕЗУЛЬТАТ ОПЛАТИ ---\n";              // Заголовок результату
    cout << "Статус: " << (r.ok ? "УСПІХ" : "ПОМИЛКА") << "\n"; // Показуємо, успіх чи ні
    cout << "Код: " << r.code << "\n";                   // Код результату
    cout << "Повідомлення: " << r.message << "\n";       // Текстове повідомлення
    if (!r.maskedCard.empty())                           // Якщо є маскований номер картки
        cout << "Картка: " << r.maskedCard << "\n";      // Виводимо його
    cout << "------------------------\n";                 // Роздільник

    if (r.ok) {                                          // Якщо оплата пройшла успішно
        quantitiesArr[prodIndex] -= qty;                 // Зменшуємо кількість на складі
        cout << "Замовлення підтверджено, товар зарезервовано на складі.\n"; // Повідомляємо
    }
    else {                                               // Якщо оплата неуспішна
        cout << "Замовлення скасовано через помилку оплати.\n"; // Повідомляємо
    }
}

// ===================== СЦЕНАРІЇ ДЛЯ КОРИСТУВАЧА ТА АДМІНА ======================

void userShoppingFlow() {                                // Потік роботи для звичайного користувача
    if (productCount == 0) {                             // Якщо каталог порожній
        cout << "Каталог порожній.\n";                   // Повідомляємо
        return;                                          // Виходимо
    }

    string nameFilter, categoryFilter;                   // Рядки для фільтрів
    double minPrice, maxPrice;                           // Межі ціни

    inputFilters(nameFilter, categoryFilter, minPrice, maxPrice); // Зчитуємо фільтри

    cout << "\nРезультати пошуку:\n";                    // Заголовок результатів
    cout << "ID\tНазва\tКатегорія\tЦіна\tСтатус\n";      // Шапка таблиці

    bool found = false;                                  // Чи знайшли хоч щось
    int foundIds[MAX_PRODUCTS];                          // Масив ID знайдених товарів
    int foundCount = 0;                                  // Кількість знайдених

    for (int i = 0; i < productCount; ++i) {             // Перебираємо всі товари
        if (matchProductFilters(i, nameFilter, categoryFilter, minPrice, maxPrice)) { // Перевіряємо фільтри
            printProductRow(i);                          // Виводимо рядок
            found = true;                                // Позначаємо, що щось знайшли
            foundIds[foundCount++] = ids[i];             // Запам’ятовуємо ID знайденого
        }
    }

    if (!found) {                                        // Якщо нічого не знайдено
        cout << "\nНічого не знайдено.\n";               // Повідомляємо
        return;                                          // Виходимо
    }

    int selectedID;                                      // Змінна для ID, який обере користувач
    cout << "\nВведіть ID товару для оформлення замовлення: "; // Просимо вибрати ID
    cin >> selectedID;                                   // Зчитуємо ID

    int idx = findIndexById(selectedID);                 // Знаходимо індекс товару
    if (idx == -1) {                                     // Якщо такий ID не існує
        cout << "Товар з таким ID не знайдено.\n";       // Повідомляємо
        return;                                          // Виходимо
    }
    if (quantitiesArr[idx] <= 0) {                       // Якщо товару нема на складі
        cout << "Цей товар відсутній на складі.\n";      // Повідомляємо
        return;                                          // Виходимо
    }

    makeOrderForProduct(idx);                            // Запускаємо оформлення замовлення
}

void adminCatalogMenu(const string& filename) {          // Меню адміністратора
    while (true) {                                       // Нескінченний цикл поки не вийдемо
        cout << "\n=== АДМІН-МЕНЮ КАТАЛОГУ ===\n";       // Заголовок меню
        cout << "1 - Показати всі товари\n";             // Пункт 1
        cout << "2 - Пошук товарів за назвою\n";         // Пункт 2
        cout << "3 - Додати товар\n";                    // Пункт 3
        cout << "4 - Видалити товар за ID\n";            // Пункт 4
        cout << "5 - Редагувати товар за ID\n";          // Пункт 5
        cout << "0 - Вихід у головне меню (зберегти каталог)\n"; // Пункт 0
        cout << "Ваш вибір: ";                           // Запитуємо вибір

        int choice;                                      // Змінна для вибору
        cin >> choice;                                   // Зчитуємо

        switch (choice) {                                // Обробляємо вибір
        case 1:                                          // Якщо 1
            printCatalog();                              // Показуємо каталог
            pressEnterToContinue();                      // Чекаємо Enter
            break;                                       // Виходимо зі switch
        case 2:                                          // Якщо 2
            searchByNameSimple();                        // Пошук за назвою
            pressEnterToContinue();                      // Пауза
            break;                                       // Далі
        case 3:                                          // Якщо 3
            addProduct();                                // Додаємо товар
            pressEnterToContinue();                      // Пауза
            break;                                       // Далі
        case 4: {                                        // Якщо 4 (блок з власними змінними)
            cout << "Введіть ID товару для видалення: "; // Просимо ID
            int id;                                      // Змінна для ID
            cin >> id;                                   // Зчитуємо
            if (deleteProductById(id))                   // Прагнемо видалити
                cout << "Товар видалено.\n";             // Якщо успіх
            else
                cout << "Товар з таким ID не знайдено.\n"; // Якщо не знайшли
            pressEnterToContinue();                      // Пауза
            break;                                       // Вихід з case
        }
        case 5: {                                        // Якщо 5
            cout << "Введіть ID товару для редагування: "; // Просимо ID
            int id;                                      // Змінна для ID
            cin >> id;                                   // Зчитуємо
            if (!editProductById(id)) {                  // Якщо не вдалося знайти/редагувати
                cout << "Товар з таким ID не знайдено.\n"; // Повідомляємо
            }
            pressEnterToContinue();                      // Пауза
            break;                                       // Вихід з case
        }
        case 0:                                          // Якщо 0
            saveCatalogToFile(filename);                 // Зберігаємо каталог
            return;                                      // Виходимо з меню
        default:                                         // Якщо ввели інше
            cout << "Невірний пункт меню. Спробуйте ще раз.\n"; // Повідомляємо
            pressEnterToContinue();                      // Пауза
            break;                                       // Далі
        }
    }
}

// ===================== MAIN ======================

int main() {                                             // Головна функція програми
#ifdef _WIN32                                            // Якщо Windows
    SetConsoleOutputCP(1251);                            // Встановлюємо кодування виводу 1251
    SetConsoleCP(1251);                                  // Встановлюємо кодування вводу 1251
#endif

    const string filename = "products.txt";              // Ім’я файлу каталогу
    loadCatalogFromFile(filename);                       // Завантажуємо каталог з файлу

    while (true) {                                       // Основний цикл програми
        cout << "\n=========== ІНТЕРНЕТ-МАГАЗИН ===========\n"; // Заголовок меню
        cout << "1 - Переглянути весь каталог\n";        // Пункт 1
        cout << "2 - Пошук товарів та оформлення замовлення (user)\n"; // Пункт 2
        cout << "3 - Адмін-режим (керування каталогом)\n"; // Пункт 3
        cout << "0 - Вихід\n";                           // Пункт 0
        cout << "Ваш вибір: ";                           // Запитуємо вибір

        int choice;                                      // Змінна для вибору
        cin >> choice;                                   // Зчитуємо вибір

        switch (choice) {                                // Обробляємо вибір
        case 1:                                          // Якщо 1
            printCatalog();                              // Виводимо каталог
            pressEnterToContinue();                      // Пауза
            break;                                       // Далі
        case 2:                                          // Якщо 2
            userShoppingFlow();                          // Запускаємо сценарій користувача
            pressEnterToContinue();                      // Пауза
            break;                                       // Далі
        case 3:                                          // Якщо 3
            adminCatalogMenu(filename);                  // Запускаємо адмін-меню
            break;                                       // Повертаємось після виходу з адмін-меню
        case 0:                                          // Якщо 0
            saveCatalogToFile(filename);                 // Зберігаємо каталог перед виходом
            cout << "Вихід з програми.\n";               // Повідомляємо
            return 0;                                    // Завершуємо програму
        default:                                         // Якщо ввели щось інше
            cout << "Невірний пункт меню.\n";            // Повідомляємо
            pressEnterToContinue();                      // Пауза
            break;                                       // Далі
        }
    }

    return 0;                                            // Формальний повернений код (сюди не дійдемо)
}
