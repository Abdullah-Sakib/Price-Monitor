#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

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
    string name;
    double price;
    string category;
};

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
void saveProduct(const Product &product);
void loadProducts(vector<Product> &products);

int main()
{
    preSignInMenu();
    return 0;
}

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
            cout << "Exiting the system. Goodbye!\n";
            exitSystem = true;
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (!exitSystem);
}

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
            cout << "3. Sign Out\n";
            cout << "4. Exit System\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();

            if (choice == 1)
            {
                addProduct();
            }
            else if (choice == 2)
            {
                showProducts();
            }
            else if (choice == 3)
            {
                cout << "Signed out successfully!\n";
                return false;
            }
            else if (choice == 4)
            {
                cout << "Exiting the system.\n";
                exitSystem = true;
            }
            else
            {
                cout << "Invalid choice. Please try again.\n";
            }
        }
        else if (role == "User")
        {
            cout << "\n--- User Menu ---\n";
            cout << "1. View Products\n";
            cout << "2. Sign Out\n";
            cout << "3. Exit System\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();

            if (choice == 1)
            {
                showProducts();
            }
            else if (choice == 2)
            {
                cout << "Signed out successfully!\n";
                return false;
            }
            else if (choice == 3)
            {
                cout << "Exiting the system.\n";
                exitSystem = true;
            }
            else
            {
                cout << "Invalid choice. Please try again.\n";
            }
        }
    } while (!exitSystem);

    return exitSystem;
}

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

bool signIn(string &role)
{
    string email, password;
    cout << "\n--- Sign In ---\n";
    cout << "Enter your Email: ";
    getline(cin, email);
    cout << "Enter your Password: ";
    getline(cin, password);

    if (validateCredentials(email, password, role))
    {
        cout << "Sign in successful! Welcome!\n";
        return true;
    }
    else
    {
        cout << "Invalid email or password. Please try again.\n";
        return false;
    }
}

void saveUser(const User &user)
{
    ofstream outfile("user.txt", ios::app);
    if (outfile.is_open())
    {
        outfile << user.name << "," << user.email << "," << user.password << "," << user.role << "\n";
        outfile.close();
    }
    else
    {
        cout << "Error: Unable to save user data.\n";
    }
}

bool isUserExist(const string &email)
{
    vector<User> users;
    loadUsers(users);

    for (const auto &user : users)
    {
        if (user.email == email)
        {
            return true;
        }
    }
    return false;
}

void loadUsers(vector<User> &users)
{
    ifstream infile("user.txt");
    string line;

    if (infile.is_open())
    {
        while (getline(infile, line))
        {
            User user;
            size_t pos = 0;
            pos = line.find(",");
            user.name = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find(",");
            user.email = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find(",");
            user.password = line.substr(0, pos);
            line.erase(0, pos + 1);

            user.role = line;

            users.push_back(user);
        }
        infile.close();
    }
}

bool validateCredentials(const string &email, const string &password, string &role)
{
    vector<User> users;
    loadUsers(users);

    for (const auto &user : users)
    {
        if (user.email == email && user.password == password)
        {
            role = user.role; // Set the role for the signed-in user
            return true;
        }
    }
    return false;
}

// Admin:
void addProduct()
{
    Product product;
    cout << "\n--- Add Product ---\n";
    cout << "Enter Product Name: ";
    getline(cin, product.name);
    cout << "Enter Product Price: ";
    cin >> product.price;
    cin.ignore();
    cout << "Enter Product Category: ";
    getline(cin, product.category);

    saveProduct(product);
    cout << "Product added successfully!\n";
}

void saveProduct(const Product &product)
{
    ofstream outfile("products.txt", ios::app);
    if (outfile.is_open())
    {
        outfile << product.name << "," << product.price << "," << product.category << "\n";
        outfile.close();
    }
    else
    {
        cout << "Error: Unable to save product data.\n";
    }
}

// User:
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
        cout << left << setw(20) << "Product Name" << setw(15) << "Price" << setw(20) << "Category" << endl;
        cout << "-----------------------------------------------------\n";
        for (const auto &product : products)
        {
            cout << left << setw(20) << product.name << setw(15) << product.price << setw(20) << product.category << endl;
        }
    }
}

void loadProducts(vector<Product> &products)
{
    ifstream infile("products.txt");
    string line;

    if (infile.is_open())
    {
        while (getline(infile, line))
        {
            Product product;
            size_t pos = 0;
            pos = line.find(",");
            product.name = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find(",");
            product.price = stod(line.substr(0, pos));
            line.erase(0, pos + 1);

            product.category = line;

            products.push_back(product);
        }
        infile.close();
    }
}
