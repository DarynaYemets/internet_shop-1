#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cctype>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

//  КАТАЛОГ ТОВАРІВ 

const int MAX_PRODUCTS = 100;

int    ids[MAX_PRODUCTS];
string namesArr[MAX_PRODUCTS];
double pricesArr[MAX_PRODUCTS];
string categoriesArr[MAX_PRODUCTS];
int    quantitiesArr[MAX_PRODUCTS];

int productCount = 0;

void pressEnterToContinue() {
    cout << "\nНатисніть Enter, щоб продовжити...";
    cin.ignore(10000, '\n');
    cin.get();
}

int generateNextId() {
    int maxId = 0;
    for (int i = 0; i < productCount; ++i) {
        if (ids[i] > maxId) maxId = ids[i];
    }
    return maxId + 1;
}

void printProductCard(int index) {
    cout << "ID: " << ids[index] << "\n"
        << "Назва: " << namesArr[index] << "\n"
        << "Категорія: " << categoriesArr[index] << "\n"
        << "Ціна: " << fixed << setprecision(2) << pricesArr[index] << " грн\n"
        << "Кількість на складі: " << quantitiesArr[index] << "\n"
        << "-----------------------------\n";
}

void printCatalog() {
    if (productCount == 0) {
        cout << "Каталог порожній.\n";
        return;
    }
    for (int i = 0; i < productCount; ++i) {
        printProductCard(i);
    }
}

void loadCatalogFromFile(const string& filename) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Файл \"" << filename << "\" не знайдено. "
            << "Каталог буде створено з нуля.\n";
        productCount = 0;
        return;
    }

    productCount = 0;
    string line;
    while (getline(fin, line) && productCount < MAX_PRODUCTS) {
        if (line.empty()) continue;

        stringstream ss(line);
        string idStr, priceStr, qtyStr;

        if (!getline(ss, idStr, ';')) continue;
        if (!getline(ss, namesArr[productCount], ';')) continue;
        if (!getline(ss, priceStr, ';')) continue;
        if (!getline(ss, categoriesArr[productCount], ';')) continue;
        if (!getline(ss, qtyStr, ';')) continue;

        try {
            ids[productCount] = stoi(idStr);
            pricesArr[productCount] = stod(priceStr);
            quantitiesArr[productCount] = stoi(qtyStr);
        }
        catch (...) {
            continue;
        }

        productCount++;
    }

    cout << "Каталог завантажено. Кількість товарів: "
        << productCount << "\n";
}

void saveCatalogToFile(const string& filename) {
    ofstream fout(filename);
    if (!fout.is_open()) {
        cout << "Помилка: не вдалося відкрити файл для запису: "
            << filename << "\n";
        return;
    }

    for (int i = 0; i < productCount; ++i) {
        fout << ids[i] << ';'
            << namesArr[i] << ';'
            << pricesArr[i] << ';'
            << categoriesArr[i] << ';'
            << quantitiesArr[i] << '\n';
    }

    cout << "Каталог збережено у файл: " << filename << "\n";
}

int findIndexById(int id) {
    for (int i = 0; i < productCount; ++i) {
        if (ids[i] == id) return i;
    }
    return -1;
}

void addProduct() {
    if (productCount >= MAX_PRODUCTS) {
        cout << "Каталог заповнений (досягнуто MAX_PRODUCTS).\n";
        return;
    }

    int index = productCount;

    ids[index] = generateNextId();
    cout << "Додавання нового товару\n";
    cout << "ID призначено автоматично: " << ids[index] << "\n";

    cin.ignore(10000, '\n');

    cout << "Введіть назву товару: ";
    getline(cin, namesArr[index]);

    cout << "Введіть категорію: ";
    getline(cin, categoriesArr[index]);

    cout << "Введіть ціну: ";
    cin >> pricesArr[index];

    cout << "Введіть кількість: ";
    cin >> quantitiesArr[index];

    productCount++;
    cout << "Товар успішно додано!\n";
}

bool deleteProductById(int id) {
    int index = findIndexById(id);
    if (index == -1) return false;

    for (int i = index; i < productCount - 1; ++i) {
        ids[i] = ids[i + 1];
        namesArr[i] = namesArr[i + 1];
        pricesArr[i] = pricesArr[i + 1];
        categoriesArr[i] = categoriesArr[i + 1];
        quantitiesArr[i] = quantitiesArr[i + 1];
    }

    productCount--;
    return true;
}

bool editProductById(int id) {
    int index = findIndexById(id);
    if (index == -1) return false;

    cout << "Редагування товару з ID " << id << "\n";
    printProductCard(index);

    cin.ignore(10000, '\n');

    string input;

    cout << "Нова назва (залиште порожнім, щоб не змінювати): ";
    getline(cin, input);
    if (!input.empty()) namesArr[index] = input;

    cout << "Нова категорія (залиште порожнім, щоб не змінювати): ";
    getline(cin, input);
    if (!input.empty()) categoriesArr[index] = input;

    cout << "Нова ціна (введіть -1, щоб не змінювати): ";
    double newPrice;
    cin >> newPrice;
    if (newPrice >= 0) pricesArr[index] = newPrice;

    cout << "Нова кількість (введіть -1, щоб не змінювати): ";
    int newQty;
    cin >> newQty;
    if (newQty >= 0) quantitiesArr[index] = newQty;

    cout << "Товар оновлено.\n";
    return true;
}

// Пошук по фрагменту назви
void searchByNameSimple() {
    cin.ignore(10000, '\n');
    cout << "Введіть фрагмент назви для пошуку: ";
    string pattern;
    getline(cin, pattern);

    if (pattern.empty()) {
        cout << "Порожній запит.\n";
        return;
    }

    bool found = false;
    for (int i = 0; i < productCount; ++i) {
        if (namesArr[i].find(pattern) != string::npos) {
            printProductCard(i);
            found = true;
        }
    }

    if (!found) {
        cout << "Товари з такою назвою не знайдено.\n";
    }
}

// ПОШУК ЗА ФІЛЬТРАМИ ДЛЯ КОРИСТУВАЧА

void inputFilters(string& name, string& category, double& minPrice, double& maxPrice) {
    cout << "Введіть назву (або '-' для пропуску): ";
    cin >> name;

    cout << "Введіть категорію (або '-' для пропуску): ";
    cin >> category;

    cout << "Мінімальна ціна: ";
    cin >> minPrice;

    cout << "Максимальна ціна: ";
    cin >> maxPrice;
}

bool matchProductFilters(int idx,
    const string& name,
    const string& category,
    double minP,
    double maxP) {
    bool nameOk = (name == "-" || namesArr[idx].find(name) != string::npos);
    bool catOk = (category == "-" || categoriesArr[idx] == category);
    bool priceOk = (pricesArr[idx] >= minP && pricesArr[idx] <= maxP);
    return nameOk && catOk && priceOk;
}

void printProductRow(int idx) {
    cout << ids[idx] << "\t"
        << namesArr[idx] << "\t"
        << categoriesArr[idx] << "\t"
        << pricesArr[idx] << " грн\t"
        << (quantitiesArr[idx] > 0 ? "В наявності" : "Немає") << endl;
}

// ===================== МОДУЛЬ ОПЛАТИ ======================

enum class PayMethod { Card, CashOnDelivery };

struct CardData {
    string number;   // тільки цифри
    string expiry;   // MM/YY
    string cvv;      // 3 цифри
    string holder;   // ім'я
};

struct PaymentResult {
    bool ok;
    string code;
    string message;
    string maskedCard;
};

// допоміжні функції для string
string digitsOnly(const string& s) {
    string r;
    for (char ch : s) if (isdigit((unsigned char)ch)) r.push_back(ch);
    return r;
}

string trim(const string& s) {
    int i = 0, j = (int)s.size() - 1;
    while (i <= j && isspace((unsigned char)s[i])) i++;
    while (j >= i && isspace((unsigned char)s[j])) j--;
    if (i > j) return "";
    return s.substr(i, j - i + 1);
}

bool luhnValid(const string& digits) {
    int sum = 0;
    bool alt = false;
    for (int i = (int)digits.size() - 1; i >= 0; --i) {
        if (!isdigit((unsigned char)digits[i])) return false;
        int d = digits[i] - '0';
        if (alt) {
            d *= 2;
            if (d > 9) d -= 9;
        }
        sum += d;
        alt = !alt;
    }
    return (sum % 10) == 0;
}

bool expiryValid(const string& mmYY) {
    if (mmYY.size() != 5 || mmYY[2] != '/') return false;
    if (!isdigit((unsigned char)mmYY[0]) ||
        !isdigit((unsigned char)mmYY[1]) ||
        !isdigit((unsigned char)mmYY[3]) ||
        !isdigit((unsigned char)mmYY[4])) return false;

    int mm = (mmYY[0] - '0') * 10 + (mmYY[1] - '0');
    int yy = (mmYY[3] - '0') * 10 + (mmYY[4] - '0');
    if (mm < 1 || mm > 12) return false;

    time_t t = time(nullptr);
    tm now{};
#ifdef _WIN32
    localtime_s(&now, &t);
#else
    now = *localtime(&t);
#endif
    int curMM = now.tm_mon + 1;
    int curYY = (now.tm_year % 100);

    if (yy < curYY) return false;
    if (yy == curYY && mm < curMM) return false;
    return true;
}

bool cvvValid(const string& cvv) {
    if (cvv.size() != 3) return false;
    return isdigit((unsigned char)cvv[0]) &&
        isdigit((unsigned char)cvv[1]) &&
        isdigit((unsigned char)cvv[2]);
}

bool holderValid(const string& h) {
    string s = trim(h);
    if (s.empty()) return false;
    int letters = 0;
    for (char c : s)
        if (isalpha((unsigned char)c)) ++letters;
    return letters >= 2;
}

string maskCard(const string& digits) {
    if (digits.size() < 4) return "****";
    string last4 = digits.substr(digits.size() - 4);
    return "**** **** **** " + last4;
}

bool restrictedBIN(const string& digits) {
    if (digits.size() < 6) return false;
    string bin6 = digits.substr(0, 6);
    const string bad[] = { "000000", "123456", "111111" };
    for (const string& s : bad)
        if (bin6 == s) return true;
    return false;
}

PaymentResult authorize(double amount, const CardData& cd) {
    if (amount >= 50000.0) {
        return { false, "DECLINED",
                 "Недостатньо коштів або ліміт перевищено.",
                 maskCard(cd.number) };
    }
    if (restrictedBIN(cd.number)) {
        return { false, "DECLINED",
                 "Обмежений BIN. Операцію відхилено.",
                 maskCard(cd.number) };
    }
    return { true, "APPROVED",
             "Оплата карткою успішна.",
             maskCard(cd.number) };
}

PayMethod readPayMethod() {
    cout << "\nОберіть спосіб оплати:\n"
        << "1 — Банківська картка\n"
        << "2 — Готівка при отриманні\n";
    while (true) {
        cout << "Введіть 1 або 2: ";
        int x;
        cin >> x;
        if (x == 1) return PayMethod::Card;
        if (x == 2) return PayMethod::CashOnDelivery;
        cout << "Невірне значення. Спробуйте ще раз.\n";
    }
}

CardData readCard() {
    CardData cd{};
    cin.ignore(10000, '\n');

    cout << "Номер картки (можна з пробілами): ";
    string tmp;
    getline(cin, tmp);
    cd.number = digitsOnly(tmp);

    cout << "Строк дії (MM/YY): ";
    getline(cin, cd.expiry);
    cd.expiry = trim(cd.expiry);

    cout << "CVV (3 цифри): ";
    getline(cin, cd.cvv);
    cd.cvv = trim(cd.cvv);

    cout << "Ім'я та прізвище власника: ";
    getline(cin, cd.holder);
    cd.holder = trim(cd.holder);

    return cd;
}

PaymentResult processPayment(double amount, PayMethod method, const CardData& cardIfAny) {
    if (!(amount > 0.0 && amount <= 1'000'000.0))
        return { false, "INVALID_INPUT",
                 "Сума повинна бути більшою за 0 і не перевищувати 1 000 000 грн.",
                 "" };

    if (method == PayMethod::CashOnDelivery) {
        return { true, "APPROVED",
                 "Оплата при отриманні. Замовлення підтверджено.",
                 "" };
    }

    if (method == PayMethod::Card) {
        if (cardIfAny.number.size() < 12 || cardIfAny.number.size() > 19)
            return { false, "INVALID_INPUT", "Невірний номер картки (довжина).", "" };
        if (!luhnValid(cardIfAny.number))
            return { false, "INVALID_INPUT", "Номер картки не проходить Luhn-перевірку.", "" };
        if (!expiryValid(cardIfAny.expiry))
            return { false, "INVALID_INPUT", "Некоректний строк дії картки.", "" };
        if (!cvvValid(cardIfAny.cvv))
            return { false, "INVALID_INPUT", "CVV має містити рівно 3 цифри.", "" };
        if (!holderValid(cardIfAny.holder))
            return { false, "INVALID_INPUT", "Порожнє або некоректне ім'я власника.", "" };

        return authorize(amount, cardIfAny);
    }

    return { false, "INVALID_INPUT", "Непідтримуваний спосіб оплати.", "" };
}

// ОФОРМЛЕННЯ ЗАМОВЛЕННЯ 

double calculateSum(double price, int quantity) {
    return price * quantity;
}

void makeOrderForProduct(int prodIndex) {
    if (prodIndex < 0 || prodIndex >= productCount) return;

    cout << "\n=== ОФОРМЛЕННЯ ЗАМОВЛЕННЯ ===\n";
    cout << "Обраний товар: " << namesArr[prodIndex]
        << " (" << pricesArr[prodIndex] << " грн)\n";
    cout << "Доступна кількість: " << quantitiesArr[prodIndex] << "\n";

    int qty;
    cout << "Введіть кількість (1.." << quantitiesArr[prodIndex] << "): ";
    cin >> qty;

    if (qty <= 0 || qty > quantitiesArr[prodIndex]) {
        cout << "Некоректна кількість.\n";
        return;
    }

    double sum = calculateSum(pricesArr[prodIndex], qty);

    string customerName, customerAddress;
    cin.ignore(10000, '\n');
    cout << "Ім'я покупця: ";
    getline(cin, customerName);
    cout << "Адреса доставки: ";
    getline(cin, customerAddress);

    cout << "\n=== ПІДСУМКОВИЙ ЧЕК (без оплати) ===\n";
    cout << namesArr[prodIndex] << " — "
        << qty << " шт × " << pricesArr[prodIndex]
        << " грн = " << sum << " грн\n";
    cout << "Покупець: " << customerName << "\n";
    cout << "Адреса: " << customerAddress << "\n";

    // Оплата
    PayMethod method = readPayMethod();
    CardData cd{};
    if (method == PayMethod::Card) {
        cd = readCard();
    }

    PaymentResult r = processPayment(sum, method, cd);

    cout << "\n--- РЕЗУЛЬТАТ ОПЛАТИ ---\n";
    cout << "Статус: " << (r.ok ? "УСПІХ" : "ПОМИЛКА") << "\n";
    cout << "Код: " << r.code << "\n";
    cout << "Повідомлення: " << r.message << "\n";
    if (!r.maskedCard.empty())
        cout << "Картка: " << r.maskedCard << "\n";
    cout << "------------------------\n";

    if (r.ok) {
        quantitiesArr[prodIndex] -= qty;
        cout << "Замовлення підтверджено, товар зарезервовано на складі.\n";
    }
    else {
        cout << "Замовлення скасовано через помилку оплати.\n";
    }
}

// СЦЕНАРІЇ ДЛЯ КОРИСТУВАЧА ЧИ АДМІНА

void userShoppingFlow() {
    if (productCount == 0) {
        cout << "Каталог порожній.\n";
        return;
    }

    string nameFilter, categoryFilter;
    double minPrice, maxPrice;

    inputFilters(nameFilter, categoryFilter, minPrice, maxPrice);

    cout << "\nРезультати пошуку:\n";
    cout << "ID\tНазва\tКатегорія\tЦіна\tСтатус\n";

    bool found = false;
    int foundIds[MAX_PRODUCTS];
    int foundCount = 0;

    for (int i = 0; i < productCount; ++i) {
        if (matchProductFilters(i, nameFilter, categoryFilter, minPrice, maxPrice)) {
            printProductRow(i);
            found = true;
            foundIds[foundCount++] = ids[i];
        }
    }

    if (!found) {
        cout << "\nНічого не знайдено.\n";
        return;
    }

    int selectedID;
    cout << "\nВведіть ID товару для оформлення замовлення: ";
    cin >> selectedID;

    int idx = findIndexById(selectedID);
    if (idx == -1) {
        cout << "Товар з таким ID не знайдено.\n";
        return;
    }
    if (quantitiesArr[idx] <= 0) {
        cout << "Цей товар відсутній на складі.\n";
        return;
    }

    makeOrderForProduct(idx);
}

void adminCatalogMenu(const string& filename) {
    while (true) {
        cout << "\n=== АДМІН-МЕНЮ КАТАЛОГУ ===\n";
        cout << "1 - Показати всі товари\n";
        cout << "2 - Пошук товарів за назвою\n";
        cout << "3 - Додати товар\n";
        cout << "4 - Видалити товар за ID\n";
        cout << "5 - Редагувати товар за ID\n";
        cout << "0 - Вихід у головне меню (зберегти каталог)\n";
        cout << "Ваш вибір: ";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            printCatalog();
            pressEnterToContinue();
            break;
        case 2:
            searchByNameSimple();
            pressEnterToContinue();
            break;
        case 3:
            addProduct();
            pressEnterToContinue();
            break;
        case 4: {
            cout << "Введіть ID товару для видалення: ";
            int id;
            cin >> id;
            if (deleteProductById(id))
                cout << "Товар видалено.\n";
            else
                cout << "Товар з таким ID не знайдено.\n";
            pressEnterToContinue();
            break;
        }
        case 5: {
            cout << "Введіть ID товару для редагування: ";
            int id;
            cin >> id;
            if (!editProductById(id)) {
                cout << "Товар з таким ID не знайдено.\n";
            }
            pressEnterToContinue();
            break;
        }
        case 0:
            saveCatalogToFile(filename);
            return;
        default:
            cout << "Невірний пункт меню. Спробуйте ще раз.\n";
            pressEnterToContinue();
            break;
        }
    }
}

// ===================== MAIN ======================

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
#endif

    const string filename = "products.txt";
    loadCatalogFromFile(filename);

    while (true) {
        cout << "\n=========== ІНТЕРНЕТ-МАГАЗИН ===========\n";
        cout << "1 - Переглянути весь каталог\n";
        cout << "2 - Пошук товарів та оформлення замовлення (user)\n";
        cout << "3 - Адмін-режим (керування каталогом)\n";
        cout << "0 - Вихід\n";
        cout << "Ваш вибір: ";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            printCatalog();
            pressEnterToContinue();
            break;
        case 2:
            userShoppingFlow();
            pressEnterToContinue();
            break;
        case 3:
            adminCatalogMenu(filename);
            break;
        case 0:
            saveCatalogToFile(filename);
            cout << "Вихід з програми.\n";
            return 0;
        default:
            cout << "Невірний пункт меню.\n";
            pressEnterToContinue();
            break;
        }
    }

    return 0;
}
