#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Registration Function
void registerUser()
{
    string username, password;

    cout << "\n===== Registration =====\n";
    cout << "Enter Username: ";
    cin >> username;

    cout << "Enter Password: ";
    cin >> password;


    ifstream file("users.txt");
    string user, pass;

    while (file >> user >> pass)
    {
        if (user == username)
        {
            cout << "Error: Username already exists!\n";
            file.close();
            return;
        }
    }

    file.close();

   
    ofstream outfile("users.txt", ios::app);
    outfile << username << " " << password << endl;
    outfile.close();

    cout << "Registration successful!\n";
}



void loginUser()
{
    string username, password;
    string user, pass;

    cout << "\n===== Login =====\n";
    cout << "Enter Username: ";
    cin >> username;

    cout << "Enter Password: ";
    cin >> password;


    ifstream file("users.txt");

    while (file >> user >> pass)
    {
        if (user == username && pass == password)
        {
            cout << "Login successful! Welcome " << username << endl;
            file.close();
            return;
        }
    }

    file.close();

    cout << "Login failed! Invalid username or password.\n";
}



int main()
{
    int choice;

    do
    {
        cout << "\n============================";
        cout << "\n Login & Registration System";
        cout << "\n============================";
        cout << "\n1. Register";
        cout << "\n2. Login";
        cout << "\n3. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;


        switch(choice)
        {
            case 1:
                registerUser();
                break;

            case 2:
                loginUser();
                break;

            case 3:
                cout << "Program exited...\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }

    }while(choice != 3);


    return 0;
}