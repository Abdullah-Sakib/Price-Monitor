#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <algorithm>

using namespace std;

// Structure to represent a User in the system
struct User
{
    string name;     // User's full name
    string email;    // User's email address (used for identification)
    string password; // User's password
    string role;     // User's role (either "Admin" or "User")
};

// Structure to represent a Product in the system
struct Product
{
    int id;          // Unique product ID
    string name;     // Name of the product
    double price;    // Price of the product
    string category; // Category to which the product belongs
};

// Structure to represent a log entry for price updates
struct PriceLog
{
    int productId;        // ID of the product whose price was updated
    double previousPrice; // The price of the product before the update
    double newPrice;      // The updated price of the product
    string updateDate;    // Date of the update (as a string)
};

// Function declarations
void signUp();                                                                       // Handles user registration
bool signIn(string &role);                                                           // Handles user login and returns the role
void saveUser(const User &user);                                                     // Saves a user to the users.txt file
bool isUserExist(const string &email);                                               // Checks if a user exists based on email
void loadUsers(vector<User> &users);                                                 // Loads all users from the users.txt file into a vector
bool validateCredentials(const string &email, const string &password, string &role); // Validates login credentials
void preSignInMenu();                                                                // Displays the pre-sign-in menu
bool postSignInMenu(const string &role);                                             // Displays the menu based on the user's role (Admin/User)
void addProduct();                                                                   // Allows Admin to add a new product
void showProducts();                                                                 // Displays all products in the system
void updateProductPrice();                                                           // Allows Admin to update a product's price
void saveProduct(const Product &product);                                            // Saves a product to the products.txt file
void loadProducts(vector<Product> &products);                                        // Loads all products from the products.txt file into a vector
int getNextProductId();                                                              // Gets the next available product ID
void saveAllProducts(const vector<Product> &products);                               // Saves all products back to the products.txt file
void loadPriceLog(int productId, vector<PriceLog> &logs);                            // Loads price logs for a specific product
void savePriceLog(const PriceLog &log);                                              // Saves a price update log entry
void showPriceUpdateHistory();                                                       // Displays the price update history for all products

int main()
{
    preSignInMenu(); // Entry point of the program
    return 0;
}

// Displays the pre-sign-in menu where users can choose to sign up, sign in, or exit
void preSignInMenu()
{
    int choice;              // Variable to store user choice
    string role;             // Variable to store the role of the signed-in user
    bool exitSystem = false; // Flag to indicate if the system should exit

    do
    {
        cout << "\n--- Welcome to Price Monitor ---\n";
        cout << "1. Sign Up\n";
        cout << "2. Sign In\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear the input buffer

        switch (choice)
        {
        case 1:
            signUp(); // Call the sign-up function
            break;
        case 2:
            if (signIn(role)) // Attempt to sign in
            {
                exitSystem = postSignInMenu(role); // If successful, navigate to the post-sign-in menu
            }
            break;
        case 3:
            cout << "Exiting the system.\n";
            exitSystem = true; // Set the exit flag to true
            break;
        default:
            cout << "Invalid choice. Please try again.\n"; // Handle invalid input
        }
    } while (!exitSystem); // Loop until the user chooses to exit
}

// Displays the post-sign-in menu based on the role of the user
bool postSignInMenu(const string &role)
{
    int choice;              // Variable to store user choice
    bool exitSystem = false; // Flag to indicate if the system should exit

    do
    {
        if (role == "Admin") // If the user is an Admin
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
                addProduct(); // Add a new product
                break;
            case 2:
                showProducts(); // Display all products
                break;
            case 3:
                updateProductPrice(); // Update the price of a product
                break;
            case 4:
                showPriceUpdateHistory(); // Display price update history
                break;
            case 5:
                cout << "Signed out successfully!\n";
                return false; // Sign out and return to the pre-sign-in menu
            case 6:
                cout << "Exiting the system.\n";
                exitSystem = true; // Exit the program
                break;
            default:
                cout << "Invalid choice. Please try again.\n"; // Handle invalid input
            }
        }
        else if (role == "User") // If the user is a regular User
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
                showProducts(); // Display all products
                break;
            case 2:
                showPriceUpdateHistory(); // Display price update history
                break;
            case 3:
                cout << "Signed out successfully!\n";
                return false; // Sign out and return to the pre-sign-in menu
            case 4:
                cout << "Exiting the system.\n";
                exitSystem = true; // Exit the program
                break;
            default:
                cout << "Invalid choice. Please try again.\n"; // Handle invalid input
            }
        }
    } while (!exitSystem); // Loop until the user chooses to exit

    return exitSystem; // Return the exit flag
}

// Handles user registration by collecting their information and saving it
void signUp()
{
    User user; // Create a new User object
    cout << "\n--- Sign Up ---\n";
    cout << "Enter your Name: ";
    getline(cin, user.name); // Get the user's name
    cout << "Enter your Email: ";
    getline(cin, user.email); // Get the user's email
    cout << "Enter your Password: ";
    getline(cin, user.password); // Get the user's password
    cout << "Enter your Role (Admin/User): ";
    getline(cin, user.role); // Get the user's role

    if (isUserExist(user.email)) // Check if the user already exists
    {
        cout << "User with this email already exists. Try signing in.\n";
        return;
    }

    saveUser(user); // Save the new user's details
    cout << "Sign up successful! You can now sign in.\n";
}

// Handles user login by validating credentials
bool signIn(string &role)
{
    string email, password;
    cout << "\n--- Sign In ---\n";
    cout << "Enter your Email: ";
    cin >> email;
    cout << "Enter your Password: ";
    cin >> password;
    return validateCredentials(email, password, role); // Validate the user's credentials
}

// Saves user information to the users.txt file
void saveUser(const User &user)
{
    ofstream file("users.txt", ios::app);                                                       // Open the file in append mode
    file << user.name << "," << user.email << "," << user.password << "," << user.role << "\n"; // Write user data
    file.close();                                                                               // Close the file
}

// Checks if a user with the given email already exists in the system
bool isUserExist(const string &email)
{
    vector<User> users;            // Create a vector to store all users
    loadUsers(users);              // Load all users from the file
    for (const auto &user : users) // Iterate through each user
    {
        if (user.email == email) // Check if the email matches
            return true;         // Return true if the user exists
    }
    return false; // Return false if the user does not exist
}

// Loads all users from the users.txt file into a vector
void loadUsers(vector<User> &users)
{
    ifstream file("users.txt"); // Open the file in read mode
    string line;
    while (getline(file, line)) // Read each line from the file
    {
        User user; // Create a new User object
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        user.name = line.substr(0, pos1);                       // Extract the name
        user.email = line.substr(pos1 + 1, pos2 - pos1 - 1);    // Extract the email
        user.password = line.substr(pos2 + 1, pos3 - pos2 - 1); // Extract the password
        user.role = line.substr(pos3 + 1);                      // Extract the role
        users.push_back(user);                                  // Add the user to the vector
    }
    file.close(); // Close the file
}

// Validate user credentials during sign-in
// Checks if the provided email and password match any user in the system
// If valid, assigns the user's role and returns true
bool validateCredentials(const string &email, const string &password, string &role)
{
    vector<User> users;
    loadUsers(users); // Load all users from the file
    for (const auto &user : users)
    {
        if (user.email == email && user.password == password) // Check email and password match
        {
            role = user.role; // Assign the user's role
            return true;      // Credentials are valid
        }
    }
    cout << "Invalid credentials!\n"; // Notify invalid credentials
    return false;                     // Credentials are invalid
}

// Add a new product to the system
// Prompts the admin to enter product details and saves them to the file
void addProduct()
{
    Product product;
    product.id = getNextProductId(); // Generate the next product ID

    cout << "\n--- Add Product ---\n";
    cout << "Enter Product Name: ";
    getline(cin, product.name); // Input product name
    cout << "Enter Product Price: ";
    cin >> product.price; // Input product price
    cin.ignore();         // Ignore leftover newline
    cout << "Enter Product Category: ";
    getline(cin, product.category); // Input product category

    saveProduct(product); // Save the product details
    cout << "Product added successfully with ID: " << product.id << "!\n";
}

// Display the list of products available in the system
// Fetches and prints product details in a tabular format
void showProducts()
{
    vector<Product> products;
    loadProducts(products); // Load all products from the file

    if (products.empty())
    {
        cout << "No products available.\n"; // Notify if no products are found
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

// Update the price of an existing product
// Allows the admin to specify a product ID and change its price
void updateProductPrice()
{
    vector<Product> products;
    loadProducts(products); // Load all products from the file

    int productId;
    double newPrice;
    bool found = false;

    cout << "\n--- Update Product Price ---\n";
    cout << "Enter Product ID: ";
    cin >> productId; // Input product ID

    for (auto &product : products)
    {
        if (product.id == productId) // Check if product ID matches
        {
            double oldPrice = product.price;
            cout << "Current Price of " << product.name << " is " << oldPrice << "\n";
            cout << "Enter New Price: ";
            cin >> newPrice;          // Input the new price
            product.price = newPrice; // Update the price
            found = true;

            // Create a price log entry
            PriceLog log;
            log.productId = productId;
            log.previousPrice = oldPrice;
            log.newPrice = newPrice;

            // Get current date and time
            time_t now = time(0);
            char *dt = ctime(&now);
            log.updateDate = dt; // Store the update date

            savePriceLog(log); // Save the price log
            cout << "Product price updated successfully!\n";
            break;
        }
    }

    if (!found)
    {
        cout << "Product ID not found.\n"; // Notify if product ID is not found
    }
    else
    {
        saveAllProducts(products); // Save updated product details
    }
}

// Comparison function to sort price logs by date in descending order
bool comparePriceLogsDescending(const PriceLog &a, const PriceLog &b)
{
    return a.updateDate > b.updateDate; // Sort logs by date
}

// Display the price update history for a specific product
// Fetches and displays all updates for a given product ID
void showPriceUpdateHistory()
{
    int productId;
    cout << "\n--- Price Update History ---\n";
    cout << "Enter Product ID: ";
    cin >> productId; // Input product ID
    vector<PriceLog> logs;
    loadPriceLog(productId, logs); // Load price logs for the product

    if (logs.empty())
    {
        cout << "No price updates available for this product.\n"; // Notify if no logs found
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

// Save a single product to the file
void saveProduct(const Product &product)
{
    ofstream file("products.txt", ios::app); // Open file in append mode
    file << product.id << "," << product.name << "," << product.price << "," << product.category << "\n";
    file.close();
}

// Load all products from the file into a vector
void loadProducts(vector<Product> &products)
{
    ifstream file("products.txt"); // Open file for reading
    string line;
    while (getline(file, line)) // Read each line
    {
        Product product;
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        product.id = stoi(line.substr(0, pos1));                      // Extract product ID
        product.name = line.substr(pos1 + 1, pos2 - pos1 - 1);        // Extract product name
        product.price = stod(line.substr(pos2 + 1, pos3 - pos2 - 1)); // Extract product price
        product.category = line.substr(pos3 + 1);                     // Extract product category
        products.push_back(product);                                  // Add product to the vector
    }
    file.close();
}

// Generate the next product ID
// Returns the next ID based on the last product in the file
int getNextProductId()
{
    vector<Product> products;
    loadProducts(products);                               // Load all products
    return products.empty() ? 1 : products.back().id + 1; // Generate ID
}

// Save all products back to the file
void saveAllProducts(const vector<Product> &products)
{
    ofstream file("products.txt", ios::trunc); // Open file in truncate mode
    for (const auto &product : products)
    {
        file << product.id << "," << product.name << "," << product.price << "," << product.category << "\n";
    }
    file.close();
}

// Save a price update log to the file
void savePriceLog(const PriceLog &log)
{
    ofstream file("price_logs.txt", ios::app);                                                         // Open file in append mode
    file << log.productId << "," << log.previousPrice << "," << log.newPrice << "," << log.updateDate; // Write log details
    file.close();
}

// Load price logs for a specific product ID
// Fetches logs from the file and adds them to the vector
void loadPriceLog(int productId, vector<PriceLog> &logs)
{
    ifstream file("price_logs.txt"); // Open file for reading
    string line;
    while (getline(file, line)) // Read each line
    {
        PriceLog log;
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        log.productId = stoi(line.substr(0, pos1)); // Extract product ID
        if (log.productId == productId)             // Check if log matches product ID
        {
            log.previousPrice = stod(line.substr(pos1 + 1, pos2 - pos1 - 1)); // Extract old price
            log.newPrice = stod(line.substr(pos2 + 1, pos3 - pos2 - 1));      // Extract new price
            log.updateDate = line.substr(pos3 + 1);                           // Extract update date
            logs.push_back(log);                                              // Add log to the vector
        }
    }
    file.close();
}
