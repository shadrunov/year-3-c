#include <iostream>
#include "kitchen.h"
#include "restaurant.h"

int main()
{
    std::cout << "Meyer's Singleton" << std::endl;

    Restaurant *rest = Restaurant::getInstance(); // Указатель на синглтон
    rest->openRestaurant();
    rest->startKitchen();

    rest->acceptOrder("Apple");
    std::cout << "Current order: " << rest->getCurrentOrder() << std::endl;
    rest->finishOrder();
    std::cout << "Current order: " << rest->getCurrentOrder() << std::endl;

    rest->acceptOrder("Banana");
    std::cout << "Current order: " << rest->getCurrentOrder() << std::endl;
    rest->finishOrder();
    std::cout << "Current order: " << rest->getCurrentOrder() << std::endl;

    rest->closeKitchen();
    rest->closeRestaurant();

    Restaurant *rest1 = Restaurant::getInstance();
    printf("\n---------------------------\n");
    printf("First object address: %p\n", (void *)rest);
    printf("Second object address: %p\n", (void *)rest1);

    return 0;
}
