#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <map>
#include <fstream>
#include <iomanip>
#include <time.h>
using namespace std;
int count = 0, maxLengthOfName = 0;
struct Food
{
    int id;
    float id_checkFloat;
    string name;
    double cost;
};
struct OrderFood
{
    int id, amount;
    float id_checkFloat, amount_checkFloat;
}; 
struct discountFoodOfDay
{
    int id, discount;
    float id_checkFloat;
};
void standardized(string &s, int maxLength) // Standardize dish names
{
    int size = s.length();
    while (size <= maxLength)
    {
        s.push_back(' ');
        size++;
    }
}
void deleteAElementOfArray(OrderFood o[], int &m, int pos) // delete a element of array at position
{
    for (int i = pos; i <= m-1; i++)
    {
        o[i] = o[i+1];
    }
    --m;
}
void deleteArray(OrderFood o[], int &m, map<int, int> &dpp) // delete the same element of Oder array  
{
    int temp;
    int i = 1, j;
    while (i <= m)
    {
        if (dpp[o[i].id] != 1)
        {
            temp = i;
            j = i + 1;
            while (j <= m)
            {
                if (o[j].id == o[temp].id)
                {
                    o[temp].amount += o[j].amount;
                    dpp[o[temp].id]--;
                    deleteAElementOfArray(o,m,j);
                }
                else j++;
            }
        }
        i++;
    }
}
void swap(Food *a, Food *b)
{
    Food temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
int partition(Food f[], int left, int right)
{
    int pivot = f[right].id;
    int l = left, r = right - 1, d = 0;
    while(d == 0)
    {
        while (l <= r && f[l].id < pivot) l++;
        while (l <= r && f[r].id > pivot) r--;
        if (l >= r) break;
        swap(&f[l], &f[r]);
        l--;
        r++;
    }
    swap(&f[l], &f[right]);
    return l;
}
void quickSort(Food f[], int left, int right)
{
    if (left < right)
    {
        int pi = partition(f, left, right);
        quickSort(f, left, pi-1);
        quickSort(f, pi+1, right);
    }
}
void getMenu(ifstream &inputMenu, Food f[], int &n) // Get menu from the file
{
    int i = 0, distance = 0;
    bool check;
    while(!inputMenu.eof())
    {   
        i++;
        inputMenu >> f[i].id;
        if (i != 1 && (f[i].id - f[i-1].id) != 1) check = true;
        getline(inputMenu, f[i].name, '-');
        if (maxLengthOfName < f[i].name.length()) maxLengthOfName = f[i].name.length();
        inputMenu >> f[i].cost;
        inputMenu.ignore();
    }
    n =  i;
    for (int i = 1; i <= n ;i++)
        standardized(f[i].name, maxLengthOfName);
    if (check == true) quickSort(f, 1, n);
}
void printMenu(Food f[], int n)
{
    int checkLength = f[1].name.length();
    if (checkLength < maxLengthOfName)
        for (int i = 1; i <= n; i++)
            standardized(f[i].name, maxLengthOfName);
    cout << "\n        ========== MENU ==========" << endl;
        for (int i = 1; i<= n; i++)
            cout << "        " << f[i].id << ". " << f[i].name << "   " << f[i].cost << "d" <<  endl;
    cout << "        ==========================" << endl;
}
void printMenuToFile(ofstream &output, Food f[], int n)
{
    output << "\n        ========== MENU ==========" << endl;
        for (int i = 1; i<= n; i++)
            output << "        " << f[i].id << ". " << f[i].name << "   " << f[i].cost << "d" <<  endl;
    output << "        ==========================" << endl;
}
void printBill(ofstream &output, Food f[], OrderFood o[], discountFoodOfDay discountOfDay, int m, string codeOfDay, double &totalOfDay, map<int, int> &dpp)  // print Bill
{
    deleteArray(o, m, dpp);
    double totalBill = 0, discount;
    if(count == 0) output << "Code Day: " << codeOfDay << endl;
    count++;
    cout << "\n======================== BILL ========================\n"
         << "STT       Ten San Pham       So Luong       Thanh Tien\n";
    output << "\n======================= BILL " << count << " =======================\n"
           << "STT       Ten San Pham       So Luong       Thanh Tien\n";
    for (int i = 1; i <= m; i++)
    {
        if (o[i].id == 0 && o[i].amount == 0) break;
        else
        {   
            double cost = o[i].amount *f[o[i].id].cost;
            if (o[i].id == discountOfDay.id) 
            {
                cost -= cost * discountOfDay.discount / 100;
                cout << i << "	  "
                     << f[o[i].id].name << "       "
                     << o[i].amount << "              "
                     << setprecision(20) << cost  << "d (discounted)" << endl;
                output << i << "         "
                       << f[o[i].id].name << "       "
                       << o[i].amount << "              "
                       << setprecision(20) << cost  << "d (discounted)" << endl;
            }
            else
            {
                cout << i << "	  "
                     << f[o[i].id].name << "       "
                     << o[i].amount << "              "
                     << setprecision(20) << cost  << "d" << endl;
                output << i << "         "
                       << f[o[i].id].name << "       "
                       << o[i].amount << "              "
                       << setprecision(20) << cost  << "d" << endl;
            }
            totalBill += cost;
        }
    }
    cout << "------------------------------------------------------\n";
    cout << "Total: " << setprecision(20) << totalBill << "d" << endl;
    cout << "Discount(%): ";
    output << "------------------------------------------------------\n";
    output << "Total: " << setprecision(20) << totalBill << "d" << endl;
    if (totalBill >= 2000000)
    {
        cin >> discount;
        totalBill -= totalBill *discount / 100;
    }
    else 
    {
        cout << "0\n";
        discount = 0;
    }
    cout << "TOTAL: " << setprecision(20) <<  totalBill << "d" << endl; 
    cout << "======================================================\n\n";
    output << "Discount(%): " << setprecision(5) << discount;
    output << "\nTOTAL: " << setprecision(20) << totalBill << "d" << endl;
    output << "======================================================\n\n";   
    totalOfDay += totalBill;
}
void addFood(Food f[], OrderFood o[], int &m, int n, map<int, int> &dpp)
{
    int i = m;
    if (i == 20 && o[i].id != 0) cout << "      YOU CAN'T ORDER MORE FOOD!\n";
    else
    {
        cout << "\n       Order more food(Food__Amount):\n";
        while(i <= 20)
        {
            m = i;
            cout << "         "; cin >> o[i].id_checkFloat >> o[i].amount_checkFloat;
            while ((o[i].id_checkFloat < 0 || o[i].id_checkFloat > n) || (o[i].id_checkFloat == 0 && o[i].amount_checkFloat != 0) || (o[i].id_checkFloat != 0 && o[i].amount_checkFloat == 0) || (o[i].id_checkFloat != trunc(o[i].id_checkFloat)) || (o[i].amount_checkFloat != trunc(o[i].amount_checkFloat)))
            {
                cout << "         ERROR! Enter again: ";
                cin >> o[i].id_checkFloat >> o[i].amount_checkFloat; 
            }
            o[i].id = trunc(o[i].id_checkFloat);
            o[i].amount = trunc(o[i].amount_checkFloat);
            if (o[i].id == 0 && o[i].amount == 0) break;
            dpp[o[i].id]++;
            i++;
        }
    }
    if (i > 20) cout << "       YOU HAVE ENTERED ENOUGH AMOUNT ALLOWED!\n";
}
void reduceFood(Food f[], OrderFood o[], int &m, int n, map<int, int> &dpp)
{  
    deleteArray(o, m, dpp);
    int ID, amount, temp;
    float ID_checkFloat, amount_checkFloat;
    bool check = false;
    cout << "       Enter the food to need reduce(Food__Amount):\n";
    cout << "          ID: "; cin >> ID_checkFloat;
    while (check == false || ID_checkFloat <= 0 || ID_checkFloat > n || ID_checkFloat != trunc(ID_checkFloat))
    {
        if (ID_checkFloat != trunc(ID_checkFloat))
        {
            cout << "           + ID must be a integer!\n";
            cout << "           + Enter again: "; cin >> ID_checkFloat;
        }
        else
        {
            for (int i = 1; i <= m; i++)
                if ((ID_checkFloat == o[i].id) && (o[i].id != 0)) 
                { 
                    check = true; 
                    temp = i; 
                    break; 
                }
            if (ID_checkFloat <= 0 || ID_checkFloat > n)
            {
                cout << "           + The food isn't in the menu!\n";
                cout << "           + Enter again: "; cin >> ID_checkFloat;
            }
            else if (check == false) 
            {
                cout << "           + The food hasn't been ordered yet!\n";
                cout << "           + Enter again: "; cin >> ID_checkFloat;
            } 
        }        
    }
    ID = trunc(ID_checkFloat);
    cout << "          Amount: "; cin >> amount_checkFloat;
    while ((amount_checkFloat > o[temp].amount) || (amount_checkFloat < 0) ||(amount_checkFloat != trunc(amount_checkFloat)))
    {
        if (amount_checkFloat != trunc(amount_checkFloat))
        {
            cout << "           + Amount must be a integer!\n";
            cout << "           + Enter again: "; cin >> amount_checkFloat;
        }
        else if (amount_checkFloat > o[temp].amount )
        {
            cout << "           + Not enough amount to reduce!\n";
            cout << "           + Enter again: "; cin >> amount_checkFloat;
        }
        else 
        {
            cout << "           + Amount must be a positive number!\n";
            cout << "           + Enter again: "; cin >> amount_checkFloat;
        }
    }
    amount = trunc(amount_checkFloat);
    o[temp].amount -= amount;
    if (o[temp].amount == 0) 
    {
        for (int i = temp; i <= m-1; i++)
        {
            o[i] = o[i+1];
        }
        --m;
    }
}
void changeMenu(ifstream &inputMenu, Food f[], int &n)
{
    int sl;
    float sl_checkFloat;
    bool check;
    cout << "\n       ========== LIST OF CHANGE ==========\n"
         << "       1. Add more food to the menu\n"
         << "       2. Remove food from the menu\n"
         << "       3. Edit food in the menu\n"
         << "       4. Print the menu\n"
         << "       5. Exit\n"
         << "       ====================================\n";
        
    do  
    {
        cout << "\n       _Choose to change the menu: "; cin >> sl_checkFloat;
        while (sl_checkFloat < 1 || sl_checkFloat > 5 || sl_checkFloat != trunc(sl_checkFloat)) 
        {
            if (sl_checkFloat != trunc(sl_checkFloat))
            {
                cout << "         + The change must a integer!\n";
                cout << "         + _Choose again: "; cin >> sl_checkFloat;
            }
            else
            {
                cout << "         + Don't have this selection!\n";
                cout << "         + _Choose again: "; 
                cin >> sl_checkFloat;
            }
        }       
        sl = sl_checkFloat;
        switch(sl)
        {
            case 1: 
                {
                    cout << "       Enter the food which you want to add to the menu:\n";
                    f[++n].id = n;
                    cout << "          Name of food: "; cin.ignore(); getline(cin, f[n].name);
                    if (f[n].name.length() > maxLengthOfName) maxLengthOfName = f[n].name.length();
                    else standardized(f[n].name, maxLengthOfName);
                    cout << "          Cost: "; cin >> f[n].cost;
                    check = false;
                    break;
                }
            case 2: 
                {
                    int ID;
                    float ID_checkFloat;
                    cout << "       Enter the food which you want to remove from the menu:\n";
                    cout << "          ID: "; cin >> ID_checkFloat;
                    while ((ID_checkFloat > n) || (ID_checkFloat <= 0) || (ID_checkFloat != trunc(ID_checkFloat)))
                    {
                        if (ID_checkFloat != trunc(ID_checkFloat))
                        {
                            cout << "           + ID must be a integer! Enter again: ";
                            cin >> ID_checkFloat;
                        }
                        else
                        {
                            cout << "           + This food isn't in the menu! Enter again: ";
                            cin >> ID_checkFloat;
                        }
                    }
                    ID = trunc(ID_checkFloat);
                    for (int i = ID; i <= n-1; i++)
                    {
                        f[i] = f[i+1];
                        f[i].id--;
                    }
                    --n;
                    check = false;
                    break;
                }
            case 3: 
            {
                int ID, i;
                float ID_checkFloat, i_checkFloat;
                cout << "       Enter the food which you want to edit in the menu:\n";
                cout << "          ID: "; cin >> ID_checkFloat;
                while ((ID_checkFloat <= 0) || (ID_checkFloat > n) || (ID_checkFloat != trunc(ID_checkFloat)))
                {
                    if (ID_checkFloat != trunc(ID_checkFloat))
                    {
                        cout << "           + ID must be a integer! Enter again: ";
                        cin >> ID_checkFloat;
                    }
                    else
                    {
                        cout << "           + This food isn't in the menu! Enter again: ";
                        cin >> ID_checkFloat;
                    }
                }
                ID = trunc(ID_checkFloat);
                cout << "          ---------------------\n"
                     << "            1. Name\n"
                     << "            2. Cost\n"
                     << "            3. Name and cost\n"
                     << "          ---------------------\n";
                cout << "          You want to edit: "; cin >> i_checkFloat;
                while (i_checkFloat < 1 || i_checkFloat > 3 || i_checkFloat != trunc(i_checkFloat)) 
                {
                    if (i_checkFloat != trunc(i_checkFloat))
                    {
                        cout << "           + The selection is a integer!\n";
                        cout << "           + _Choose again: "; cin >> i_checkFloat;
                    }
                    else
                    {
                        cout << "           + ERROR! Choose again: "; 
                        cin >> i_checkFloat;
                    }
                }
                i = trunc(i_checkFloat);
                switch (i)
                    {
                    case 1:
                    {
                        cout << "       Edit name of food: "; cin.ignore(); getline(cin, f[ID].name);
                        if (f[n].name.length() > maxLengthOfName) maxLengthOfName = f[n].name.length();
                        else standardized(f[n].name, maxLengthOfName);
                        break;
                    }
                    case 2:
                    {
                        cout << "       Edit cost of food: "; cin >> f[ID].cost;
                        break;
                    }
                    default: 
                    {
                        cout << "       Edit name of food: "; cin.ignore(); getline(cin, f[ID].name);
                        if (f[n].name.length() > maxLengthOfName) maxLengthOfName = f[n].name.length();
                        else standardized(f[n].name, maxLengthOfName);
                        cout << "       Edit cost of food: "; cin >> f[ID].cost;
                    }
                    }
                check = false;
                break;
            }
            case 4:
            {
                printMenu(f, n);
                check = true;
                break;
            }
            default: 
                if (check == false) printMenu(f, n); 
        }
    }while (sl != 5);
}
void order(ofstream &output, Food f[], OrderFood o[], discountFoodOfDay discountOfDay, int &m, int &n, map<int, int> &dpp, string codeOfDay, double &totalOfDay)  // Order food
{
    cout << "\n       Order food(Food__Amount):\n";
    int i = 1, sl;
    float sl_checkFloat;
    while(i <= 20)
    {
        m = i;
        cout << "         "; cin >> o[i].id_checkFloat >> o[i].amount_checkFloat;
        while ((o[i].id_checkFloat < 0 || o[i].id_checkFloat > n) || (o[i].id_checkFloat == 0 && o[i].amount_checkFloat != 0) || (o[i].id_checkFloat != 0 && o[i].amount_checkFloat == 0) || (o[i].id_checkFloat != trunc(o[i].id_checkFloat)) || (o[i].amount_checkFloat != trunc(o[i].amount_checkFloat)))
        {
            cout << "         ERROR! Enter again: ";
            cin >> o[i].id_checkFloat >> o[i].amount_checkFloat; 
        }
        o[i].id = trunc(o[i].id_checkFloat);
        o[i].amount = trunc(o[i].amount_checkFloat);
        if (o[i].id == 0 && o[i].amount == 0) break;
        dpp[o[i].id]++;
        i++;
    }
    if (i > 20) cout << "       YOU HAVE ENTERED ENOUGH AMOUNT ALLOWED!\n";
    do
    {
        cout << "\n       ========== SELECTION ==========\n" 
             << "       1. Order more food\n"
             << "       2. Reduce food\n"
             << "       3. Print bill\n"
             << "       4. Exit\n"
             << "       ===============================\n";
        cout << "\n       _Choose selection: "; cin >> sl_checkFloat;
        while (sl_checkFloat < 1 || sl_checkFloat > 4 || sl_checkFloat != trunc(sl_checkFloat))
        {
            if (sl_checkFloat != trunc(sl_checkFloat))
            {
                cout << "         + The selection must a integer!\n";
                cout << "         + _Choose again: "; cin >> sl_checkFloat;
            }
            else
            {
                cout << "         + Don't have this selection\n";
                cout << "         + _Choose selection again: "; cin >> sl_checkFloat;
            }
        }
        sl = trunc(sl_checkFloat);
        switch (sl)
        {
            case 1: addFood(f, o, m, n, dpp); break;
            case 2: reduceFood(f, o, m, n, dpp); break;
            case 3: 
            {
                if (o[1].id != 0 ) printBill(output, f, o, discountOfDay, m, codeOfDay, totalOfDay, dpp);
            }
        }
    }while(sl != 3 && sl != 4);
}
void option(ifstream &inputMenu, ofstream &output, Food f[], OrderFood o[], discountFoodOfDay &discountOfDay)
{
    int n, m, sl;
    float sl_checkFloat;
    double totalOfDay = 0;
    map<int, int> dpp;
    string code_begin, code_finish = "\0";
    cout << "Enter code: "; cin >> code_begin;
    getMenu(inputMenu, f, n); printMenu(f, n);
    cout << "\nDiscounted food of the day is: "; cin >> discountOfDay.id_checkFloat;
    while (discountOfDay.id_checkFloat > n || discountOfDay.id_checkFloat != trunc(discountOfDay.id_checkFloat))
    {
        if (discountOfDay.id_checkFloat != trunc(discountOfDay.id_checkFloat))
        {
            cout << "  ID must be a integer!\n   Enter again: ";
            cin >> discountOfDay.id_checkFloat;
        }
        else
        {
            cout << "  This food isn't in the menu\n   Enter again: ";
            cin >> discountOfDay.id_checkFloat;
        }
    }
    if (discountOfDay.id_checkFloat == 0) discountOfDay.discount = 0;
    else { cout << "Discount(%): "; cin >> discountOfDay.discount; }
    discountOfDay.id = trunc(discountOfDay.id_checkFloat);
    while(code_finish != code_begin)
    { 
        cout << "\n        ========= OPTION ==========\n"
             << "        1. Order the food\n"
             << "        2. Change the menu\n"
             << "        ===========================\n";
        cout << "        _Choose option: "; cin >> sl_checkFloat;
        while (sl_checkFloat < 1 || sl_checkFloat > 2 || sl_checkFloat != trunc(sl_checkFloat))
        {
            if (sl_checkFloat != trunc(sl_checkFloat))
            {
                cout << "         + The option must be a integer!\n";
                cout << "         + _Choose again: "; cin >> sl_checkFloat;
            }
            else
            {
                cout << "         + Don't have this option\n";
                cout << "         + _Choose option again: "; cin >> sl_checkFloat;
            }
        }
        sl = trunc(sl_checkFloat);
        switch(sl)
        {
            case 1:
                order(output, f, o, discountOfDay, m, n, dpp, code_begin, totalOfDay);
                break;
            case 2:
                changeMenu(inputMenu, f, n);
        }
        cout << "\n*.* Press Enter to continue! ";
        cin.ignore();
        getline(cin,code_finish);
        cout << "\n------------------------------------------------------------------------------\n";
    }
    printMenuToFile(output, f, n);
    output << "TOTAL OF DAY: " << setprecision(20) << totalOfDay << "d" << endl;
    output << "------------------------------------------------------------------------------\n";
}
int main()
{
    Food f[100];
    OrderFood o[100];
    discountFoodOfDay discountOfDay;
    ifstream inputMenu; inputMenu.open("menu.txt");
    ofstream output("output.txt", ios::app);
    option(inputMenu, output, f, o, discountOfDay);
    output.close();
    inputMenu.close();
    return 0;
}
