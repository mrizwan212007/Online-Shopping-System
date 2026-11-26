#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <iomanip>
#include <limits>
#include <utility>

using namespace std;

struct Product
{
    int id;
    string name;
    string category;
    float price;
    int stock;
    Product* next;
};

struct Cart
{
    int id;
    string name;
    float price;
    int quantity;
    Cart* next;
};

struct OrderItem
{
    int id;
    string name;
    float price;
    int quantity;
};

struct Order
{
    int orderID;
    int itemCount;
    OrderItem items[50];
    float total;
};


Product* head = NULL;
Cart* cartHead = NULL;

queue<Order> orders;

int nextOrderID = 1;

void clearInput()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

Product* findProduct(int id)
{
    Product* temp = head;

    while (temp != NULL)
    {
        if (temp->id == id)
        {
            return temp;
        }

        temp = temp->next;
    }

    return NULL;
}


void saveProducts()
{
    ofstream file("products.txt");

    Product* temp = head;

    while (temp != NULL)
    {
        file << temp->id << " "
             << temp->price << " "
             << temp->stock << endl;

        file << temp->name << endl;
        file << temp->category << endl;

        temp = temp->next;
    }

    file.close();
}

void loadProducts()
{
    ifstream file("products.txt");

    if (!file)
    {
        return;
    }

    while (true)
    {
        Product* p = new Product;

        if (!(file >> p->id >> p->price >> p->stock))
        {
            delete p;
            break;
        }

        file.ignore(numeric_limits<streamsize>::max(), '\n');

        if (!getline(file, p->name) ||
            !getline(file, p->category))
        {
            delete p;
            break;
        }

        p->next = NULL;

        if (head == NULL)
        {
            head = p;
        }
        else
        {
            Product* temp = head;

            while (temp->next != NULL)
            {
                temp = temp->next;
            }

            temp->next = p;
        }
    }

    file.close();
}

void addProduct()
{
    Product* p = new Product;

    cout << "\nEnter Product ID: ";

    if (!(cin >> p->id) || p->id <= 0)
    {
        cout << "Invalid Product ID!\n";
        clearInput();
        delete p;
        return;
    }

    if (findProduct(p->id) != NULL)
    {
        cout << "Duplicate Product ID! Try another ID.\n";
        delete p;
        return;
    }

    clearInput();

    cout << "Enter Product Name: ";
    getline(cin, p->name);

    if (p->name.empty())
    {
        cout << "Product name cannot be empty!\n";
        delete p;
        return;
    }

    cout << "Enter Category: ";
    getline(cin, p->category);

    if (p->category.empty())
    {
        cout << "Category cannot be empty!\n";
        delete p;
        return;
    }

    cout << "Enter Price: ";

    if (!(cin >> p->price) || p->price <= 0)
    {
        cout << "Invalid Price!\n";
        clearInput();
        delete p;
        return;
    }

    cout << "Enter Stock: ";

    if (!(cin >> p->stock) || p->stock < 0)
    {
        cout << "Invalid Stock!\n";
        clearInput();
        delete p;
        return;
    }

    p->next = NULL;

    if (head == NULL)
    {
        head = p;
    }
    else
    {
        Product* temp = head;

        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = p;
    }

    saveProducts();

    cout << "\nProduct Added Successfully!\n";
}


void displayProducts()
{
    if (head == NULL)
    {
        cout << "\nNo Products Available!\n";
        return;
    }

    Product* temp = head;

    cout << "\nPRODUCT LIST\n";

    cout << left
         << setw(10) << "ID"
         << setw(20) << "Name"
         << setw(15) << "Category"
         << setw(12) << "Price"
         << "Stock\n";

    cout << "\n";

    while (temp != NULL)
    {
        cout << left
             << setw(10) << temp->id
             << setw(20) << temp->name
             << setw(15) << temp->category
             << setw(12) << fixed << setprecision(2) << temp->price
             << temp->stock << endl;

        temp = temp->next;
    }
}

void searchProduct()
{
    if (head == NULL)
    {
        cout << "\nNo Products Available!\n";
        return;
    }

    int choice;

    cout << "\n1. Search by ID";
    cout << "\n2. Search by Name";
    cout << "\n3. Search by Category";
    cout << "\n4. Search by Price";
    cout << "\nEnter Choice: ";

    if (!(cin >> choice))
    {
        cout << "Invalid Input!\n";
        clearInput();
        return;
    }

    Product* temp = head;
    bool found = false;

    if (choice == 1)
    {
        int id;

        cout << "Enter Product ID: ";
        cin >> id;

        temp = findProduct(id);

        if (temp != NULL)
        {
            found = true;
        }
    }

    else if (choice == 2 || choice == 3)
    {
        string value;

        clearInput();

        cout << "Enter Name or Category: ";
        getline(cin, value);

        while (temp != NULL)
        {
            if ((choice == 2 && temp->name == value) ||
                (choice == 3 && temp->category == value))
            {
                cout << "\nID: " << temp->id
                     << "\nName: " << temp->name
                     << "\nCategory: " << temp->category
                     << "\nPrice: " << temp->price
                     << "\nStock: " << temp->stock << endl;

                found = true;
            }

            temp = temp->next;
        }
    }

    else if (choice == 4)
    {
        float price;

        cout << "Enter Price: ";
        cin >> price;

        while (temp != NULL)
        {
            if (temp->price == price)
            {
                cout << "\nID: " << temp->id
                     << "\nName: " << temp->name
                     << "\nPrice: " << temp->price << endl;

                found = true;
            }

            temp = temp->next;
        }
    }

    else
    {
        cout << "Invalid Choice!\n";
        return;
    }

    if (choice == 1 && found)
    {
        cout << "\nProduct Found!"
             << "\nID: " << temp->id
             << "\nName: " << temp->name
             << "\nCategory: " << temp->category
             << "\nPrice: " << temp->price
             << "\nStock: " << temp->stock << endl;
    }

    if (!found)
    {
        cout << "\nProduct Not Found!\n";
    }
}

void updateProduct()
{
    int id;

    cout << "\nEnter Product ID: ";
    cin >> id;

    Product* temp = findProduct(id);

    if (temp == NULL)
    {
        cout << "Product Not Found!\n";
        return;
    }

    float price;
    int stock;

    cout << "Enter New Price: ";

    if (!(cin >> price) || price <= 0)
    {
        cout << "Invalid Price!\n";
        clearInput();
        return;
    }

    cout << "Enter New Stock: ";

    if (!(cin >> stock) || stock < 0)
    {
        cout << "Invalid Stock!\n";
        clearInput();
        return;
    }

    temp->price = price;
    temp->stock = stock;

    saveProducts();

    cout << "\nProduct Updated Successfully!\n";
}

void deleteProduct()
{
    int id;

    cout << "\nEnter Product ID to Delete: ";
    cin >> id;

    Product* temp = head;
    Product* previous = NULL;

    while (temp != NULL && temp->id != id)
    {
        previous = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        cout << "Product Not Found!\n";
        return;
    }

    if (previous == NULL)
    {
        head = temp->next;
    }
    else
    {
        previous->next = temp->next;
    }

    delete temp;

    saveProducts();

    cout << "\nProduct Deleted Successfully!\n";
}

void sortProducts()
{
    if (head == NULL)
    {
        cout << "No Products Available!\n";
        return;
    }

    int choice;

    cout << "\n1. Sort by ID";
    cout << "\n2. Sort by Name";
    cout << "\n3. Sort by Category";
    cout << "\n4. Sort by Price";
    cout << "\nEnter Choice: ";

    if (!(cin >> choice))
    {
        cout << "Invalid Input!\n";
        clearInput();
        return;
    }

    if (choice < 1 || choice > 4)
    {
        cout << "Invalid Choice!\n";
        return;
    }

    for (Product* i = head; i != NULL; i = i->next)
    {
        for (Product* j = head; j->next != NULL; j = j->next)
        {
            bool condition = false;

            if (choice == 1 && j->id > j->next->id)
                condition = true;

            else if (choice == 2 && j->name > j->next->name)
                condition = true;

            else if (choice == 3 && j->category > j->next->category)
                condition = true;

            else if (choice == 4 && j->price > j->next->price)
                condition = true;

            if (condition)
            {
                swap(j->id, j->next->id);
                swap(j->name, j->next->name);
                swap(j->category, j->next->category);
                swap(j->price, j->next->price);
                swap(j->stock, j->next->stock);
            }
        }
    }

    saveProducts();

    cout << "\nProducts Sorted Successfully!\n";

    displayProducts();
}

Cart* findCartItem(int id)
{
    Cart* temp = cartHead;

    while (temp != NULL)
    {
        if (temp->id == id)
            return temp;

        temp = temp->next;
    }

    return NULL;
}

void addToCart()
{
    int id;
    int quantity;

    cout << "\nEnter Product ID: ";
    cin >> id;

    Product* p = findProduct(id);

    if (p == NULL)
    {
        cout << "Product Unavailable!\n";
        return;
    }

    cout << "Enter Quantity: ";

    if (!(cin >> quantity) || quantity <= 0)
    {
        cout << "Invalid Quantity!\n";
        clearInput();
        return;
    }

    Cart* existing = findCartItem(id);

    int alreadyAdded = 0;

    if (existing != NULL)
    {
        alreadyAdded = existing->quantity;
    }

    if (alreadyAdded + quantity > p->stock)
    {
        cout << "Insufficient Stock!\n";
        return;
    }

    if (existing != NULL)
    {
        existing->quantity += quantity;
    }
    else
    {
        Cart* c = new Cart;

        c->id = p->id;
        c->name = p->name;
        c->price = p->price;
        c->quantity = quantity;
        c->next = NULL;

        if (cartHead == NULL)
        {
            cartHead = c;
        }
        else
        {
            Cart* temp = cartHead;

            while (temp->next != NULL)
            {
                temp = temp->next;
            }

            temp->next = c;
        }
    }

    cout << "\nProduct Added to Cart Successfully!\n";
}

void removeFromCart()
{
    if (cartHead == NULL)
    {
        cout << "\nShopping Cart is Empty!\n";
        return;
    }

    int id;

    cout << "Enter Product ID to Remove: ";
    cin >> id;

    Cart* temp = cartHead;
    Cart* previous = NULL;

    while (temp != NULL && temp->id != id)
    {
        previous = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        cout << "Product Not Found in Cart!\n";
        return;
    }

    if (previous == NULL)
    {
        cartHead = temp->next;
    }
    else
    {
        previous->next = temp->next;
    }

    delete temp;

    cout << "\nProduct Removed from Cart!\n";
}

void displayCart()
{
    if (cartHead == NULL)
    {
        cout << "\nShopping Cart is Empty!\n";
        return;
    }

    Cart* temp = cartHead;
    float total = 0;

    cout << "\nSHOPPING CART\n";

    while (temp != NULL)
    {
        float amount = temp->price * temp->quantity;

        cout << "\nProduct: " << temp->name
             << "\nQuantity: " << temp->quantity
             << "\nPrice: " << temp->price
             << "\nAmount: " << amount << endl;

        total += amount;

        temp = temp->next;
    }

    cout << "\nTotal Purchase Amount: Rs. "
         << fixed << setprecision(2) << total << endl;
}

void savePendingOrders()
{
    ofstream file("pending_orders.txt");

    queue<Order> temp = orders;

    while (!temp.empty())
    {
        Order o = temp.front();
        temp.pop();

        file << o.orderID << " "
             << o.itemCount << " "
             << o.total << endl;

        for (int i = 0; i < o.itemCount; i++)
        {
            file << o.items[i].id << " "
                 << o.items[i].price << " "
                 << o.items[i].quantity << endl;

            file << o.items[i].name << endl;
        }
    }

    file.close();
}

void loadOrders()
{
    ifstream file("pending_orders.txt");

    if (!file)
    {
        return;
    }

    Order o;

    while (file >> o.orderID >> o.itemCount >> o.total)
    {
        if (o.itemCount < 1 || o.itemCount > 50)
            break;

        file.ignore(numeric_limits<streamsize>::max(), '\n');

        bool valid = true;

        for (int i = 0; i < o.itemCount; i++)
        {
            if (!(file >> o.items[i].id
                       >> o.items[i].price
                       >> o.items[i].quantity))
            {
                valid = false;
                break;
            }

            file.ignore(numeric_limits<streamsize>::max(), '\n');

            if (!getline(file, o.items[i].name))
            {
                valid = false;
                break;
            }
        }

        if (!valid)
            break;

        orders.push(o);

        if (o.orderID >= nextOrderID)
        {
            nextOrderID = o.orderID + 1;
        }
    }

    file.close();
}

void placeOrder()
{
    if (cartHead == NULL)
    {
        cout << "\nCannot Place Order! Cart is Empty.\n";
        return;
    }

    Order o;

    o.orderID = nextOrderID++;
    o.itemCount = 0;
    o.total = 0;

    Cart* temp = cartHead;

    while (temp != NULL)
    {
        if (o.itemCount >= 50)
        {
            cout << "Cart contains too many different products!\n";
            nextOrderID--;
            return;
        }

        Product* p = findProduct(temp->id);

        if (p == NULL || p->stock < temp->quantity)
        {
            cout << "Order cannot be placed! Stock is unavailable.\n";
            nextOrderID--;
            return;
        }

        o.items[o.itemCount].id = temp->id;
        o.items[o.itemCount].name = temp->name;
        o.items[o.itemCount].price = p->price;
        o.items[o.itemCount].quantity = temp->quantity;

        o.total += p->price * temp->quantity;

        o.itemCount++;

        temp = temp->next;
    }

    orders.push(o);

    while (cartHead != NULL)
    {
        Cart* temp = cartHead;
        cartHead = cartHead->next;
        delete temp;
    }

    savePendingOrders();

    cout << "\nOrder Placed Successfully!"
         << "\nOrder ID: " << o.orderID
         << "\nTotal Amount: Rs. "
         << fixed << setprecision(2) << o.total << endl;
}

void processOrder()
{
    if (orders.empty())
    {
        cout << "\nNo Pending Orders to Process!\n";
        return;
    }

    Order o = orders.front();
    for (int i = 0; i < o.itemCount; i++)
    {
        Product* p = findProduct(o.items[i].id);

        if (p == NULL || p->stock < o.items[i].quantity)
        {
            cout << "\nOrder " << o.orderID
                 << " cannot be processed due to insufficient stock!\n";

            orders.pop();
            savePendingOrders();
            return;
        }
    }
    for (int i = 0; i < o.itemCount; i++)
    {
        Product* p = findProduct(o.items[i].id);

        p->stock -= o.items[i].quantity;
    }

    saveProducts();
    ofstream file("order_history.txt", ios::app);

    file << "\nOrder ID: " << o.orderID
         << "\nTotal Amount: Rs. " << o.total << endl;

    for (int i = 0; i < o.itemCount; i++)
    {
        file << "Product: " << o.items[i].name
             << " | Quantity: " << o.items[i].quantity
             << " | Price: " << o.items[i].price << endl;
    }

    file << "\n";

    file.close();

    cout << "\nOrder Processed Successfully!"
         << "\nOrder ID: " << o.orderID
         << "\nTotal Amount: Rs. "
         << fixed << setprecision(2) << o.total << endl;

    orders.pop();

    savePendingOrders();
}

int main()
{
    loadProducts();
    loadOrders();

    int choice = 0;

    do
    {
        cout << "\n\n";
        cout << "\n ONLINE SHOPPING AND ORDER SYSTEM";
        cout << "\n";

        cout << "\n\n1. Add Product";
        cout << "\n2. Display Products";
        cout << "\n3. Search Product";
        cout << "\n4. Update Product";
        cout << "\n5. Delete Product";
        cout << "\n6. Sort Products";
        cout << "\n7. Add Product to Cart";
        cout << "\n8. Remove Product from Cart";
        cout << "\n9. Display Shopping Cart";
        cout << "\n10. Place Order";
        cout << "\n11. Process Next Order";
        cout << "\n12. Exit";

        cout << "\n\nEnter Your Choice: ";

        if (!(cin >> choice))
        {
            cout << "\nInvalid Input! Enter a Number.\n";
            clearInput();
            choice = 0;
            continue;
        }

        switch (choice)
        {
        case 1:
            addProduct();
            break;

        case 2:
            displayProducts();
            break;

        case 3:
            searchProduct();
            break;

        case 4:
            updateProduct();
            break;

        case 5:
            deleteProduct();
            break;

        case 6:
            sortProducts();
            break;

        case 7:
            addToCart();
            break;

        case 8:
            removeFromCart();
            break;

        case 9:
            displayCart();
            break;

        case 10:
            placeOrder();
            break;

        case 11:
            processOrder();
            break;

        case 12:
            saveProducts();
            savePendingOrders();
            cout << "\nThank You for Using Our System!\n";
            break;

        default:
            cout << "\nInvalid Choice! Try Again.\n";
        }

    } while (choice != 12);

    while (head != NULL)
    {
        Product* temp = head;
        head = head->next;
        delete temp;
    }

    while (cartHead != NULL)
    {
        Cart* temp = cartHead;
        cartHead = cartHead->next;
        delete temp;
    }

    return 0;
}


