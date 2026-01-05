#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

// ANSI color codes
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define YELLOW "\033[1;33m"
#define CYAN "\033[1;36m"
#define PINK "\033[1;35m"
#define RESET "\033[0m"

//Basis Klasse
class Character {
protected:
    string name;
    int HP;
    int AD;
    int MP;

public:
    Character(string n, int h, int a, int m) : name(n), HP(h), AD(a), MP(m) {}

    // Polymorphie
    virtual void attack(Character& target) {
        target.HP -= AD;
        cout << GREEN << name << " hits " << target.getName() << " for " << AD << " damage!" << RESET << endl;
    }

    virtual void castMagic(Character& target) {
        target.HP -= MP;
        cout << BLUE << name << " casts magic on " << target.getName() << " for " << MP << " damage!" << RESET << endl;
    }

    string getName() const { return name; }
    int getHP() const { return HP; }
    bool isAlive() const { return HP > 0; }

    virtual void showStats() = 0;
};


// Klasse enemy erbt vom Character
class Enemy : public Character {
public:
    Enemy(string n, int h, int a, int m) : Character(n,h,a,m) {}

    // Override showStats for Enemy
    void showStats() override {
        cout << name << " - HP: " << HP << " | AD: " << AD << " | MP: " << MP << endl;
    }
};

//klasse cat erbt acuch vom character

class Cat : public Character {
private:
    int XP;
    int level;
    int fishes;

public:
    Cat(string n, int h, int a, int m) : Character(n,h,a,m), XP(0), level(1), fishes(0) {}

    void gainXP(int amount) {
        XP += amount;
        cout << PINK << name << " gains " << amount << " XP!" << RESET << endl;
        levelUp();
    }

    void levelUp() {
        while(XP >= 20) {
            level++;
            XP -= 20;
            int choice;
            cout << "\n" << PINK << "Level up! You are now level " << level << "!" << RESET << endl;
            cout << "Add 5 points to:\n1. HP\n2. AD\n3. MP\nChoice: ";
            cin >> choice;
            switch(choice) {
                case 1: HP += 5; break;
                case 2: AD += 5; break;
                case 3: MP += 5; break;
                default: cout << "Invalid choice, no points added.\n";
            }
        }
    }

    void addFishes(int amount) {
        fishes += amount;
        cout << YELLOW << name << " receives " << amount << " fishes. Total: " << fishes << RESET << endl;
    }

    void shop() {
        int choice;
        cout << "\n" << CYAN << "Welcome to the shop! Fishes: " << YELLOW << fishes << RESET << endl;
        cout << "1. Buy 15 HP for 10 fishes\n2. Buy 4 MP for 15 fishes\n3. Buy 4 AD for 15 fishes\n4. Exit\nChoice: ";
        cin >> choice;

        switch(choice) {
            case 1:
                if(fishes >= 10) { HP += 15; fishes -= 10; cout << GREEN << "Bought 15 HP. Current HP: " << HP << RESET << endl; }
                else cout << RED << "Not enough fishes!" << RESET << endl;
                break;
            case 2:
                if(fishes >= 15) { MP += 4; fishes -= 15; cout << BLUE << "Bought 4 MP. Current MP: " << MP << RESET << endl; }
                else cout << RED << "Not enough fishes!" << RESET << endl;
                break;
            case 3:
                if(fishes >= 15) { AD += 4; fishes -= 15; cout << YELLOW << "Bought 4 AD. Current AD: " << AD << RESET << endl; }
                else cout << RED << "Not enough fishes!" << RESET << endl;
                break;
            default: cout << CYAN << "Exiting shop." << RESET << endl;
        }
    }

    void showStats() override {
        cout << PINK << name << " - Level " << level << " | HP: " << HP << " | AD: " << AD << " | MP: " << MP 
             << " | XP: " << XP << " | Fishes: " << YELLOW << fishes << RESET << endl;
    }
};

// Generate random enemy
Enemy generateEnemy() {
    vector<string> names = {"Goblin", "Minion", "Dog", "Skeleton", "Wolf"};
    string enemyName = names[rand() % names.size()];
    int hp = 50 + rand() % 31;
    int ad = 5 + rand() % 6;
    int mp = rand() % 6;
    return Enemy(enemyName, hp, ad, mp);
}


bool battle(Cat& player) {
    Enemy enemy = generateEnemy();
    cout << "\n" << BLUE << "A wild enemy appears!" << RESET << endl;
    enemy.showStats();

    while(player.isAlive() && enemy.isAlive()) {
        int choice;
        cout << "\n1. Do Attack Damage \n 2. Do Magic Damage\nChoose action: ";
        cin >> choice;

        if(choice == 1) player.attack(enemy);    // AD attack → green
        else if(choice == 2) player.castMagic(enemy); // Magic attack → blue
        else cout << "Invalid choice! You lose your turn.\n";

        if(enemy.isAlive()) enemy.attack(player);
    }

    if(player.isAlive()) {
        cout << PINK << "You won !" << RESET << endl;
        player.gainXP(10);
        player.addFishes(5);
        return true;
    } else {
        cout << PINK << "You lost !" << RESET << endl;
        return false;
    }
}

int main() {
    srand(time(NULL));

    // Choose Cat
    cout << CYAN << "Choose your cat:\n1. Warrior Cat\n2. Mage Cat\n3. Balanced Cat\nChoice: " << RESET;
    int choice; cin >> choice;
    Cat player = (choice == 1) ? Cat("Warrior Cat", 120, 15, 5) :
                 (choice == 2) ? Cat("Mage Cat", 80, 5, 20) :
                                 Cat("Balanced Cat", 100, 10, 10);

    // Main game loop
    while(player.isAlive()) {
        cout << "\nCommands:\n";
        cout << BLUE << "1. Battle Area\n" 
             << PINK << "2. Show Stats\n" 
             << YELLOW << "3. Fish Shop\n" 
             << RED << "4. Exit\n" << RESET
             << "Choice: ";
        cin >> choice;

        if(choice == 1) battle(player);
        else if(choice == 2) player.showStats();
        else if(choice == 3) player.shop();
        else if(choice == 4) break;
        else cout << "Invalid command.\n";
    }

    cout << RED << "Game over!" << RESET << endl;
    return 0;
}


// Console Battler – Kurzbeschreibung:

//  Du wählst eine von drei Katzen mit unterschiedlichen Werten (HP, AD, MP) '
// und kämpfst gegen zufällige Gegner in Area 1. Kämpfe sind rundenbasiert:
//  Du kannst normal angreifen oder Magie einsetzen. Gewonnene Kämpfe bringen XP und Währung. 
// Bei genügend XP kannst du aufleveln und Punkte auf HP, AD oder MP verteilen. Gegner lassen manchmal 
// kleine Loots fallen, die deine HP kurzfristig erhöhen. Ziel ist es, stärker zu werden, XP zu sammeln 
// und deine Währung zu erhöhen.