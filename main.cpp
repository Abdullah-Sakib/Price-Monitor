#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <algorithm>

using namespace std;

struct User
{
    string name;
    string email;
    string password;
    string role;
};

struct Product
{
    int id;
    string name;
    double price;
    string category;
};

struct PriceLog
{
    int productId;
    double previousPrice;
    double newPrice;
    string updateDate;
};

// Function declarations
void signUp();
bool signIn(string &role);
void saveUser(const User &user);
bool isUserExist(const string &email);
void loadUsers(vector<User> &users);
bool validateCredentials(const string &email, const string &password, string &role);
void preSignInMenu();
bool postSignInMenu(const string &role);
void addProduct();
void showProducts();
void updateProductPrice();
void saveProduct(const Product &product);
void loadProducts(vector<Product> &products);
int getNextProductId();
void saveAllProducts(const vector<Product> &products);
void loadPriceLog(int productId, vector<PriceLog> &logs);
void savePriceLog(const PriceLog &log);
void showPriceUpdateHistory();

int main()
{
    preSignInMenu();
    return 0;
}

// Pre-SignIn Menu
void preSignInMenu()
{
    int choice;
    string role;
    bool exitSystem = false;

    do
    {
        cout << "\n--- Welcome to Price Monitor ---\n";
        cout << "1. Sign Up\n";
        cout << "2. Sign In\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            signUp();
            break;
        case 2:
            if (signIn(role))
            {
                exitSystem = postSignInMenu(role);
            }
            break;
        case 3:
            cout << "Exiting the system.\n";
            exitSystem = true;
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (!exitSystem);
}

// Post-SignIn Menu
bool postSignInMenu(const string &role)
{
    int choice;
    bool exitSystem = false;

    do
    {
        if (role == "Admin")
        {
            cout << "\n--- Admin Menu ---\n";
            cout << "1. Add Product\n";
            cout << "2. View Products\n";
            cout << "3. Update Product Price\n";
            cout << "4. View Price Update History\n";
            cout << "5. Sign Out\n";
            cout << "6. Exit System\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice)
            {
            case 1:
                addProduct();
                break;
            case 2:
                showProducts();
                break;
            case 3:
                updateProductPrice();
                break;
            case 4:
                showPriceUpdateHistory();
                break;
            case 5:
                cout << "Signed out successfully!\n";
                return false;
            case 6:
                cout << "Exiting the system.\n";
                exitSystem = true;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        }
        else if (role == "User")
        {
            cout << "\n--- User Menu ---\n";
            cout << "1. View Products\n";
            cout << "2. View Price Update History\n";
            cout << "3. Sign Out\n";
            cout << "4. Exit System\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice)
            {
            case 1:
                showProducts();
                break;
            case 2:
                showPriceUpdateHistory();
                break;
            case 3:
                cout << "Signed out successfully!\n";
                return false;
            case 4:
                cout << "Exiting the system.\n";
                exitSystem = true;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        }
    } while (!exitSystem);

    return exitSystem;
}

// Sign Up
void signUp()
{
    User user;
    cout << "\n--- Sign Up ---\n";
    cout << "Enter your Name: ";
    getline(cin, user.name);
    cout << "Enter your Email: ";
    getline(cin, user.email);
    cout << "Enter your Password: ";
    getline(cin, user.password);
    cout << "Enter your Role (Admin/User): ";
    getline(cin, user.role);

    if (isUserExist(user.email))
    {
        cout << "User with this email already exists. Try signing in.\n";
        return;
    }

    saveUser(user);
    cout << "Sign up successful! You can now sign in.\n";
}

// Sign In
bool signIn(string &role)
{
    string email, password;
    cout << "\n--- Sign In ---\n";
    cout << "Enter your Email: ";
    cin >> email;
    cout << "Enter your Password: ";
    cin >> password;
    return validateCredentials(email, password, role);
}

// Save User
void saveUser(const User &user)
{
    ofstream file("users.txt", ios::app);
    file << user.name << "," << user.email << "," << user.password << "," << user.role << "\n";
    file.close();
}

// Check if User Exists
bool isUserExist(const string &email)
{
    vector<User> users;
    loadUsers(users);
    for (const auto &user : users)
    {
        if (user.email == email)
            return true;
    }
    return false;
}

// Load Users
void loadUsers(vector<User> &users)
{
    ifstream file("users.txt");
    string line;
    while (getline(file, line))
    {
        User user;
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        user.name = line.substr(0, pos1);
        user.email = line.substr(pos1 + 1, pos2 - pos1 - 1);
        user.password = line.substr(pos2 + 1, pos3 - pos2 - 1);
        user.role = line.substr(pos3 + 1);
        users.push_back(user);
    }
    file.close();
}

// Validate Credentials
bool validateCredentials(const string &email, const string &password, string &role)
{
    vector<User> users;
    loadUsers(users);
    for (const auto &user : users)
    {
        if (user.email == email && user.password == password)
        {
            role = user.role;
            return true;
        }
    }
    cout << "Invalid credentials!\n";
    return false;
}

// Add Product
void addProduct()
{
    Product product;
    product.id = getNextProductId();

    cout << "\n--- Add Product ---\n";
    cout << "Enter Product Name: ";
    getline(cin, product.name);
    cout << "Enter Product Price: ";
    cin >> product.price;
    cin.ignore();
    cout << "Enter Product Category: ";
    getline(cin, product.category);

    saveProduct(product);
    cout << "Product added successfully with ID: " << product.id << "!\n";
}

// Show Products
void showProducts()
{
    vector<Product> products;
    loadProducts(products);

    if (products.empty())
    {
        cout << "No products available.\n";
    }
    else
    {
        cout << "\n--- Product List ---\n";
        cout << left << setw(5) << "ID" << setw(20) << "Name" << setw(10) << "Price" << setw(15) << "Category" << "\n";
        cout << "------------------------------------------------------\n";
        for (const auto &product : products)
        {
            cout << left << setw(5) << product.id << setw(20) << product.name << setw(10) << product.price << setw(15) << product.category << "\n";
        }
    }
}

// Update Product Price
void updateProductPrice()
{
    vector<Product> products;
    loadProducts(products);

    int productId;
    double newPrice;
    bool found = false;

    cout << "\n--- Update Product Price ---\n";
    cout << "Enter Product ID: ";
    cin >> productId;

    for (auto &product : products)
    {
        if (product.id == productId)
        {
            double oldPrice = product.price;
            cout << "Current Price of " << product.name << " is " << oldPrice << "\n";
            cout << "Enter New Price: ";
            cin >> newPrice;
            product.price = newPrice;
            found = true;

            // Create a price log entry
            PriceLog log;
            log.productId = productId;
            log.previousPrice = oldPrice;
            log.newPrice = newPrice;

            // Get current date and time
            time_t now = time(0);
            char *dt = ctime(&now);
            log.updateDate = dt;

            savePriceLog(log);
            cout << "Product price updated successfully!\n";
            break;
        }
    }

    if (!found)
    {
        cout << "Product ID not found.\n";
    }
    else
    {
        saveAllProducts(products);
    }
}

// Comparison function to compare PriceLog objects by updateDate
bool comparePriceLogsDescending(const PriceLog &a, const PriceLog &b)
{
    return a.updateDate > b.updateDate; // Sort in descending order
}

void showPriceUpdateHistory()
{
    int productId;
    cout << "\n--- Price Update History ---\n";
    cout << "Enter Product ID: ";
    cin >> productId;
    vector<PriceLog> logs;
    loadPriceLog(productId, logs);

    if (logs.empty())
    {
        cout << "No price updates available for this product.\n";
    }
    else
    {
        // Sort logs using the comparison function
        sort(logs.begin(), logs.end(), comparePriceLogsDescending);

        cout << "\nPrice Update Log for Product ID: " << productId << "\n";
        cout << left << setw(30) << "Date" << setw(15) << "Old Price" << setw(15) << "New Price" << "\n";
        cout << "---------------------------------------------------------\n";
        for (const auto &log : logs)
        {
            cout << left << setw(30) << log.updateDate << setw(15) << log.previousPrice << setw(15) << log.newPrice << "\n";
        }
    }
}

// Helper Functions for File Operations
void saveProduct(const Product &product)
{
    ofstream file("products.txt", ios::app);
    file << product.id << "," << product.name << "," << product.price << "," << product.category << "\n";
    file.close();
}

void loadProducts(vector<Product> &products)
{
    ifstream file("products.txt");
    string line;
    while (getline(file, line))
    {
        Product product;
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        product.id = stoi(line.substr(0, pos1));
        product.name = line.substr(pos1 + 1, pos2 - pos1 - 1);
        product.price = stod(line.substr(pos2 + 1, pos3 - pos2 - 1));
        product.category = line.substr(pos3 + 1);
        products.push_back(product);
    }
    file.close();
}

int getNextProductId()
{
    vector<Product> products;
    loadProducts(products);
    return products.empty() ? 1 : products.back().id + 1;
}

void saveAllProducts(const vector<Product> &products)
{
    ofstream file("products.txt", ios::trunc);
    for (const auto &product : products)
    {
        file << product.id << "," << product.name << "," << product.price << "," << product.category << "\n";
    }
    file.close();
}

void savePriceLog(const PriceLog &log)
{
    ofstream file("price_logs.txt", ios::app);
    file << log.productId << "," << log.previousPrice << "," << log.newPrice << "," << log.updateDate;
    file.close();
}

void loadPriceLog(int productId, vector<PriceLog> &logs)
{
    ifstream file("price_logs.txt");
    string line;
    while (getline(file, line))
    {
        PriceLog log;
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        log.productId = stoi(line.substr(0, pos1));
        if (log.productId == productId)
        {
            log.previousPrice = stod(line.substr(pos1 + 1, pos2 - pos1 - 1));
            log.newPrice = stod(line.substr(pos2 + 1, pos3 - pos2 - 1));
            log.updateDate = line.substr(pos3 + 1);
            logs.push_back(log);
        }
    }
    file.close();
}
