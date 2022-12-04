//Version 1.0
#include<iostream>
#include<sstream>
#include<string>
#include<fstream>
#include<algorithm>
#include<cctype>
using namespace std;

void storeData(){
    ofstream fout;
    fout.open("store.txt", ios::app);
    char choice;
    string website, pass;
    do{
        cout << "Enter the name of the website: ";
        cin >> website;
        cout << "Enter the password: ";
        cin >> pass;
        fout << website << " : " << pass << endl;

        cout << "Want to enter more data: Y/y for yes anything else to exit: ";
        cin >> choice;
        choice = tolower(choice);
    } while (choice == 'y');
    fout.close();
}

void readData(){
    ifstream fin;
    fin.open("store.txt", fstream::in);

    string line, platform, word;

    cout << "Enter the name of the platform: ";
    cin >> platform;
    transform(platform.begin(), platform.end(), platform.begin(), ::tolower);
    getline(fin, line);
    while (getline(fin, line)) {
        istringstream sin(line);
        sin >> word;
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        int j = 0;
        string trimmed = "";
        for (int i = 0; i < word.size(); i++) {   //Instead of trimming like this I could use getline(sin, word, ',')
            if ((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z'))
                trimmed = trimmed + word[i];
        }
        cout << trimmed << "    " << platform << endl;
        if (trimmed == platform) {
            cout << "+----------------------------------------------------------" << endl;
            cout << "| " << line << "\t" << endl;
            cout << "+----------------------------------------------------------";
            break;
        }
    }

    fin.close();
}

char increment(char c) { return c + 3; }

void encrypt(string &username, string &password){
    transform(username.begin(), username.end(), username.begin(), increment);
    transform(password.begin(), password.end(), password.begin(), increment);
}

void changePassword(int *passCount){
    ifstream fin("store.txt", ios::app | ios::ate);
    ofstream fout("Copy.txt");
    int newCount = 0; 
    string username, password, line;
    cout << "Enter new username: ";
    cin >> username;
    cout << endl
         << "Enter new password: ";
    cin >> password;
    encrypt(username, password);
    newCount = newCount + username.length() + password.length() + 1;
    fout << username << " " << password;
    fin.seekg(*passCount, ios::beg);
    while(getline(fin, line)){
        fout << line << endl;
    }
    *passCount = newCount;
    fin.close();
    fout.close();
    remove("store.txt");
    rename("Copy.txt", "store.txt");
}

int main() {
    string username, password, un, pass, line;
    int passwordCount = 0;
    cout << "Enter the username: ";
    cin >> username;
    cout << endl
         << "Enter the password: ";
    cin >> password;

    ifstream fin("store.txt");
    getline(fin, line);
    istringstream sin(line);
    sin >> un >> pass;
    passwordCount = un.length() + pass.length()+1;
    encrypt(username, password);
    fin.close();
    char choice;
    if(username == un && password == pass){
        while(1){
            cout << "\nEnter your choice: E: Enter data,  R: Read data,  P: To reset Password,  Press any other to exit ----> ";
            cin >> choice;
            choice = tolower(choice);
            if(choice == 'e')
                storeData();
            else if(choice == 'r')
                readData();
            else if(choice == 'p')
                changePassword(&passwordCount);
            else
                break;
        }
    }
    

    return 0;
}

