#include <iostream>
#include <vector>
using namespace std;

/* ================= PRODUCT ================= */

class Product {
protected:
    int id;
    string name;
public:
    Product(int i, string n) : id(i), name(n) {}
    virtual void display() = 0;
    int getId() { return id; }
};

class MenClothing : public Product {
public:
    MenClothing(int i, string n) : Product(i, n) {}
    void display() {
        cout << id << ". " << name << endl;
    }
};

/* ================= LINKED LIST ================= */

class Node {
public:
    int productId;
    Node* next;
    Node(int id) {
        productId = id;
        next = NULL;
    }
};

class PurchaseHistory {
    Node* head;
public:
    PurchaseHistory() { head = NULL; }

    void addProduct(int id) {
        Node* n = new Node(id);
        n->next = head;
        head = n;
    }

    bool hasProduct(int id) {
        Node* temp = head;
        while (temp) {
            if (temp->productId == id)
                return true;
            temp = temp->next;
        }
        return false;
    }
};

/* ================= USER ================= */

class User {
public:
    string username;
    string password;
    PurchaseHistory history;

    User(string u, string p) {
        username = u;
        password = p;
    }
};

/* ================= RECOMMENDATION ENGINE ================= */

class RecommendationEngine {
public:
    static int similarity(User& u1, User& u2, int totalProducts) {
        int score = 0;
        for (int i = 1; i <= totalProducts; i++) {
            if (u1.history.hasProduct(i) &&
                u2.history.hasProduct(i))
                score++;
        }
        return score;
    }

    static void recommend(User& target,
                          vector<User>& users,
                          vector<Product*>& products) {

        int bestScore = 0;
        int bestUser = -1;

        for (int i = 0; i < users.size(); i++) {
            if (&users[i] == &target) continue;

            int score = similarity(target, users[i], products.size());
            if (score > bestScore) {
                bestScore = score;
                bestUser = i;
            }
        }

        cout << "\nRecommended Products For You:\n";

        if (bestUser == -1 || bestScore == 0) {
            cout << "No recommendations yet.\n";
            return;
        }

        for (int i = 0; i < products.size(); i++) {
            int pid = products[i]->getId();
            if (users[bestUser].history.hasProduct(pid) &&
                !target.history.hasProduct(pid)) {
                products[i]->display();
            }
        }
    }
};

/* ================= MAIN ================= */

int main() {

    vector<Product*> products = {
        new MenClothing(1, "Shalwar Kameez"),
        new MenClothing(2, "Kurta Pajama"),
        new MenClothing(3, "Waistcoat"),
        new MenClothing(4, "Shawl"),
        new MenClothing(5, "Khussa"),
        new MenClothing(6, "Hoodie"),
        new MenClothing(7, "Sweater"),
        new MenClothing(8, "Denim Jeans"),
        new MenClothing(9, "Shirts"),
        new MenClothing(10, "Muffler")
    };

    vector<User> users;

    // Existing users (data for collaborative filtering)
    users.push_back(User("ahmed", "111"));
    users.push_back(User("usman", "222"));

    users[0].history.addProduct(1);
    users[0].history.addProduct(2);
    users[0].history.addProduct(3);

    users[1].history.addProduct(2);
    users[1].history.addProduct(4);
    users[1].history.addProduct(6);

    /* ===== MAIN SYSTEM LOOP ===== */
    while (true) {
        cout << "\n===== E-Commerce Recommendation Engine =====\n";
        cout << "1. Signup\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Choose option: ";

        int option;
        cin >> option;

        if (option == 3) {
            cout << "Program exited.\n";
            break;
        }

        User* currentUser = NULL;

        /* ===== SIGNUP ===== */
        if (option == 1) {
            string u, p;
            cout << "Create Username: ";
            cin >> u;
            cout << "Create Password: ";
            cin >> p;

            users.push_back(User(u, p));
            currentUser = &users.back();

            cout << "Signup Successful!\n";
        }

        /* ===== LOGIN ===== */
        else if (option == 2) {
            string u, p;
            cout << "Username: ";
            cin >> u;
            cout << "Password: ";
            cin >> p;

            for (int i = 0; i < users.size(); i++) {
                if (users[i].username == u &&
                    users[i].password == p) {
                    currentUser = &users[i];
                    break;
                }
            }

            if (!currentUser) {
                cout << "Invalid Login!\n";
                continue;
            }
        }
        else {
            cout << "Invalid option!\n";
            continue;
        }

        /* ===== USER SHOPPING SESSION ===== */
        while (true) {
            cout << "\nMen Clothing Products:\n";
            for (auto p : products) p->display();

            cout << "\nEnter Product ID to Buy (0 = logout): ";
            int pid;
            cin >> pid;

            if (pid == 0) {
                cout << "Logged out successfully.\n";
                break;   // ?? FIX: returns to login/signup menu
            }

            currentUser->history.addProduct(pid);
            cout << "Purchase Successful!\n";

            RecommendationEngine::recommend(
                *currentUser, users, products
            );
        }
    }

    return 0;
}

