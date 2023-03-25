#pragma once
#include <string>
#include <iostream>

class Kitchen
{
public:
    enum kitchenStatus
    {
        busy = 1,
        free = 2,
        dayOff = 3
    };

    void startKitchen()
    {
        if ((this->kitchenState != kitchenStatus::busy) && (this->kitchenState != kitchenStatus::dayOff))
        {
            this->kitchenState = kitchenStatus::free;
            std::cout << "Kitchen started, state: free" << std::endl;
        }
    }

    void closeKitchen()
    {
        if (this->kitchenState == kitchenStatus::free)
        {
            this->kitchenState = kitchenStatus::dayOff;
            std::cout << "Kitchen closed, state: dayOff" << std::endl;
            std::cout << "Orders number: " << this->orderNumber << std::endl;
            this->orderNumber = 0;
        }
    }

    void acceptOrder(std::string order)
    {
        if (this->kitchenState != kitchenStatus::free)
            std::cout << "Can't accept order, kitchen is already cooking" << std::endl;
        else
        {
            std::cout << "Accepted order " << order << std::endl;
            this->kitchenState = kitchenStatus::busy;
            this->currentOrder = order;
        }
    }

    std::string finishOrder()
    {
        if (this->kitchenState != kitchenStatus::busy)
        {
            std::cout << "Can't finish order, kitchen currently is not cooking" << std::endl;
            return nullptr;
        }
        std::string _currentOrder = this->currentOrder;
        this->kitchenState = kitchenStatus::free;
        this->currentOrder = "";
        std::cout << "Finished order " << _currentOrder << std::endl;
        this->orderNumber++;
        return _currentOrder;
    }

    std::string getCurrentOrder() const
    {
        if (currentOrder != "")
            return currentOrder;
        return "-- nothing --";
    }

protected:
    kitchenStatus kitchenState;
    std::string currentOrder;
    int orderNumber;
    Kitchen()
    {
        this->kitchenState = kitchenStatus::free;
        this->currentOrder = "";
        this->orderNumber = 0;
    };
    ~Kitchen() = default;
};
