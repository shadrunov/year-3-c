#include <string>
#include <utility>
#include "kitchen.h"

class Restaurant : public Kitchen
{
public:
    static Restaurant *getInstance()
    {
        if (!p_instance)
            p_instance = new Restaurant();
        return p_instance;
    }

    // other methods
    enum restaurantStatus
    {
        open = 1,
        closed = 2
    };

    void openRestaurant()
    {
        if (this->restaurantState != restaurantStatus::open)
        {
            this->restaurantState = restaurantStatus::open;
            std::cout << "Restaurant opened" << std::endl;
        }
    }

    void closeRestaurant()
    {
        if (this->restaurantState != restaurantStatus::closed)
        {
            this->restaurantState = restaurantStatus::closed;
            std::cout << "Restaurant closed" << std::endl;
        }
    }

private:
    static Restaurant *p_instance;
    int restaurantState;
    Restaurant(){};                            // Private constructor
    ~Restaurant(){};                           // Private destructor
    Restaurant(const Restaurant &);            // Prevent copy constructor
    Restaurant &operator=(const Restaurant &); // Prevent assignment
};

Restaurant *Restaurant::p_instance = nullptr;
