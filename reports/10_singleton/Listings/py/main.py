from enum import Enum


class Kitchen:
    class kitchenStatus(Enum):
        busy = 1
        free = 2
        dayOff = 3

    def __init__(self) -> None:
        self.kitchenState: Kitchen.kitchenStatus = Kitchen.kitchenStatus.free
        self.currentOrder: str = ""
        self.orderNumber: int = 0

    def startKitchen(self):
        if self.kitchenState != Kitchen.kitchenStatus.free:
            self.kitchenState = Kitchen.kitchenStatus.free
            print("Kitchen started, state: free")

    def closeKitchen(self):
        if self.kitchenState == Kitchen.kitchenStatus.free:
            self.kitchenState = Kitchen.kitchenStatus.dayOff
            print("Kitchen closed, state: dayOff")
            print("Orders number:", self.orderNumber)
            self.orderNumber = 0

    def acceptOrder(self, order: str):
        if self.kitchenState != Kitchen.kitchenStatus.free:
            print("Can't accept order, kitchen is already cooking")
        else:
            print("Accepted order:", order)
            self.kitchenState = Kitchen.kitchenStatus.busy
            self.currentOrder = order

    def finishOrder(self):
        if self.kitchenState != Kitchen.kitchenStatus.busy:
            print("Can't finish order, kitchen currently is not cooking")
            return ""
        _currentOrder = self.currentOrder
        self.kitchenState = Kitchen.kitchenStatus.free
        self.currentOrder = ""
        print("Finished order:", _currentOrder)
        self.orderNumber += 1
        return _currentOrder

    def getCurrentOrder(self):
        if self.currentOrder:
            return self.currentOrder
        return "-- nothing --"


class Restaurant(Kitchen):
    def __new__(cls):
        if not hasattr(cls, "instance"):
            cls.instance = super(Restaurant, cls).__new__(cls)
        return cls.instance

    def __init__(self) -> None:
        super().__init__()
        self.restaurantState = Restaurant.restaurantStatus.closed

    class restaurantStatus(Enum):
        open = 1
        closed = 2

    def openRestaurant(self):
        if self.restaurantState != Restaurant.restaurantStatus.open:
            self.restaurantState = Restaurant.restaurantStatus.open
            print("Restaurant opened")

    def closeRestaurant(self):
        if self.restaurantState != Restaurant.restaurantStatus.closed:
            self.restaurantState = Restaurant.restaurantStatus.closed
            print("Restaurant closed")


if __name__ == "__main__":
    print("Python Singleton")
    rest: Restaurant = Restaurant()

    rest.openRestaurant()
    rest.startKitchen()

    rest.acceptOrder("Apple")
    print("Current order: ", rest.getCurrentOrder())
    rest.finishOrder()
    print("Current order: ", rest.getCurrentOrder())

    rest.acceptOrder("Banana")
    print("Current order: ", rest.getCurrentOrder())
    rest.finishOrder()
    print("Current order: ", rest.getCurrentOrder())

    rest.closeKitchen()
    rest.closeRestaurant()
