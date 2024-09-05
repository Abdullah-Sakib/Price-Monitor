#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// User Structure to hold user data
struct User
{
    string name;
    string email;
    string password;
    string role;
};

// Function Prototypes
void signUp();
bool signIn();
void saveUser(const User &user);
bool isUserExist(const string &email);
void loadUsers(vector<User> &users);
bool validateCredentials(const string &email, const string &password);
void menu();

int main()
{
    menu();
    return 0;
}

// Main menu to choose actions
void menu()
{
    int choice;
    bool signedIn = false;

    do
    {
        cout << "\n--- Welcome to Price Monitor ---\n";
        cout << "1. Sign Up\n";
        cout << "2. Sign In\n";
        cout << "3. Sign Out\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear the newline character from the buffer

        switch (choice)
        {
        case 1:
            signUp();
            break;
        case 2:
            signedIn = signIn();
            break;
        case 3:
            if (signedIn)
            {
                cout << "Signed out successfully!\n";
                signedIn = false;
            }
            else
            {
                cout << "No user is currently signed in.\n";
            }
            break;
        case 4:
            cout << "Exiting the system. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);
}

// Function to handle user sign-up
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

// Function to handle user sign-in
bool signIn()
{
    string email, password;
    cout << "\n--- Sign In ---\n";
    cout << "Enter your Email: ";
    getline(cin, email);
    cout << "Enter your Password: ";
    getline(cin, password);

    if (validateCredentials(email, password))
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

// Function to save user details to the file
void saveUser(const User &user)
{
    ofstream outfile("user.txt", ios::app); // Open file in append mode
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

// Function to check if a user with the given email already exists
bool isUserExist(const string &email)
{
    vector<User> users;
    loadUsers(users);

    for (const auto &user : users)
    {
        if (user.email == email)
        {
            return true; // User exists
        }
    }
    return false; // User does not exist
}

// Function to load all users from the file into a vector
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

// Function to validate user credentials for sign-in
bool validateCredentials(const string &email, const string &password)
{
    vector<User> users;
    loadUsers(users);

    for (const auto &user : users)
    {
        if (user.email == email && user.password == password)
        {
            return true; // Credentials are valid
        }
    }
    return false; // Invalid credentials
}
