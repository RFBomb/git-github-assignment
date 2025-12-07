//
// Robert Brenckman
// Charter Oak - ITE217 ( Fall 2025 )
// 11/6/2025
//

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <limits>

///////////////////////////////////////////////////////////////////////////////////////////////////////
//                               Observer Class
///////////////////////////////////////////////////////////////////////////////////////////////////////

class Observer
{

private:
    std::string color;

public:

    Observer(const std::string& colorCode);

    const std::string& getColor() const;

    void notify(const std::string& message) const;
};

// ---------------- 

inline Observer::Observer(const std::string& colorCode) : color(colorCode) {}

inline const std::string& Observer::getColor() const { return color; }

inline void Observer::notify(const std::string& message) const
{
    std::cout << color << message << "\033[0m\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//                               Subject Class
///////////////////////////////////////////////////////////////////////////////////////////////////////

class Subject
{

private:
    std::vector<std::shared_ptr<Observer>> observers;

public:

    void subscribe(std::shared_ptr<Observer> obs)
    {
        observers.push_back(obs);
    }

    void unsubscribe(size_t index) {
        if (index < observers.size())
            observers.erase(observers.begin() + index);
    }

    void notifyAll(const std::string& message) const
    {
        if (observers.empty())
        {
            std::cout << "No observers.\n";
            return;
        }
        std::cout << "--------------------\n";
        for (auto& obs : observers)
            obs->notify(message);
        std::cout << "--------------------\n";
    }

    void clearSubscribers()
    {
        observers.clear();
    }

    void listObservers() const
    {
        if (observers.empty()) {
            std::cout << "No observers.\n";
            return;
        }

        std::cout << "\n\nCurrent Observers:\n";

        for (size_t i = 0; i < observers.size(); ++i)
        {
            std::cout << observers[i]->getColor() << "Observer @ Index : " << i << "\033[0m\n";
        }
    }

    size_t count() const { return observers.size(); }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////
//                               Global Declarations
///////////////////////////////////////////////////////////////////////////////////////////////////////

const std::vector<std::string> colors = {
    "\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m",
    "\033[36m", "\033[37m", "\033[90m", "\033[91m", "\033[92m"
};

Subject subject; // for the purposes of this task, we create a single subject to observe and interact with.

///////////////////////////////////////////////////////////////////////////////////////////////////////
//                               Menu System
///////////////////////////////////////////////////////////////////////////////////////////////////////


void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

//----------------------------------------------------
//                Observer Menu
//----------------------------------------------------
void subscriberMenu()
{
    int choice = 0;
    do {
        std::cout << "\nObserver Management:\n1. Add Observer\n2. Remove Observer\n3. List Observer\n4. Clear all observers\n0. Back\nChoice: ";
        std::cin >> choice;
        clearInput();
        switch (choice) 
        {
            // Add a Observer
        case 1: 
        {
            std::cout << "Select a color:\n";
            for (size_t i = 0; i < colors.size(); ++i) {
                std::cout << i << ": " << colors[i] << "Color\033[0m\n";
            }
            int colorChoice = -1;
            std::cin >> colorChoice;
            clearInput();
            if (colorChoice >= 0 && colorChoice < (int)colors.size()) {
                std::shared_ptr<Observer> obs = std::make_shared<Observer>(colors[colorChoice]);
                subject.subscribe(obs);
                obs->notify("Observer added!\n");
            }
            else {
                std::cout << "Invalid color choice.\n";
            }
            break;
        }

              // Remove a Observer
        case 2: {
            if (subject.count() == 0) {
                std::cout << "No observers to remove.\n";
                break;
            }
            subject.listObservers();
            std::cout << "Enter index to remove: ";
            int idx;
            std::cin >> idx;
            clearInput();
            if (idx >= 0 && idx < (int)subject.count()) {
                subject.unsubscribe(idx);
                std::cout << "Observer removed!\n";
            }
            else {
                std::cout << "Invalid index.\n";
            }
            break;
        }

              // List all Observer
        case 3:
            subject.listObservers();
            break;

            // clear all
        case 4:
            subject.clearSubscribers();
            std::cout << "Observers cleared.\n";
            break;
        }
    } while (choice != 0);
}

//----------------------------------------------------
//                Publisher Loop
//----------------------------------------------------
void publisherLoop()
{
    std::string message;
    while (true)
    {
        std::cout << "\nEnter a message to publish (empty line to return to main menu)\nMessage : ";
        std::getline(std::cin, message);
        if (message == "") return;
        subject.notifyAll(message);
    };
}


//----------------------------------------------------
//                Main Menu
//----------------------------------------------------
int main() {
    


    int mainChoice = 0;

    std::cout << "\nThis console app creates a single subject that can have observers attached to it.\n"
        << "Use the menu system to add observers to the collection, then publish messages to all observers.\n";

    do 
    {
        std::cout << "\nMain Menu:\n1. Observer Management\n2. Publish Message\n0. Exit\nChoice: ";
        std::cin >> mainChoice;
        clearInput();
        switch (mainChoice) 
        {
            case 1:
                subscriberMenu();
                break;
            case 2: 
                publisherLoop();
                break;
            case 3:
                break;
        }
    } while (mainChoice != 0);

    std::cout << "Exiting...\n";
    return 0;
}




