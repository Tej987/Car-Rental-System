#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

class Car {
private:
    int carID;
    string model;
    string brand;
    double rentalPrice;
    bool isRented;

public:
    Car(int id, string m, string b, double price, bool rented = false) {
        carID = id;
        model = m;
        brand = b;
        rentalPrice = price;
        isRented = rented;
    }

    int getCarID() const { return carID; }
    string getModel() const { return model; }
    string getBrand() const { return brand; }
    double getRentalPrice() const { return rentalPrice; }
    bool getIsRented() const { return isRented; }

    void rentCar() {
        if (!isRented) {
            isRented = true;
            cout << "Car rented successfully!\n";
        } else {
            cout << "Car is already rented!\n";
        }
    }

    void returnCar() {
        if (isRented) {
            isRented = false;
            cout << "Car returned successfully!\n";
        } else {
            cout << "This car was not rented!\n";
        }
    }

    void displayCar() const {
        cout << "Car ID: " << carID << " | Model: " << model << " | Brand: " << brand
             << " | Price per day: ?" << fixed << setprecision(2) << rentalPrice
             << " | Status: " << (isRented ? "Rented" : "Available") << endl;
    }

    void saveToFile(ofstream &outfile) const {
        outfile << carID << "," << model << "," << brand << "," << rentalPrice << "," << isRented << endl;
    }

    static Car loadFromFile(ifstream &infile) {
        int id;
        string model, brand;
        double price;
        bool rented;
        infile >> id;
        infile.ignore();
        getline(infile, model, ',');
        getline(infile, brand, ',');
        infile >> price >> rented;
        return Car(id, model, brand, price, rented);
    }
};

bool isCarIDUnique(vector<Car> &cars, int id) {
    for (const auto &c : cars) {
        if (c.getCarID() == id) {
            return false;
        }
    }
    return true;
}

Car* findCar(vector<Car> &cars, int id) {
    for (auto &c : cars) {
        if (c.getCarID() == id) {
            return &c;
        }
    }
    return nullptr;
}

void saveCarsToFile(vector<Car> &cars) {
    ofstream outfile("cars.txt", ios::trunc);
    for (const auto &c : cars) {
        c.saveToFile(outfile);
    }
    outfile.close();
}

void loadCarsFromFile(vector<Car> &cars) {
    ifstream infile("cars.txt");
    if (!infile) return;

    while (!infile.eof()) {
        int id;
        string model, brand;
        double price;
        bool rented;

        infile >> id;
        infile.ignore();
        if (getline(infile, model, ',') && getline(infile, brand, ',') && infile >> price >> rented) {
            cars.push_back(Car(id, model, brand, price, rented));
        }
    }
    infile.close();
}

void deleteCar(vector<Car> &cars, int id) {
    for (auto it = cars.begin(); it != cars.end(); ++it) {
        if (it->getCarID() == id) {
            cars.erase(it);
            cout << "Car deleted successfully!\n";
            saveCarsToFile(cars);
            return;
        }
    }
    cout << "Car not found!\n";
}

int main() {
    vector<Car> cars;
    loadCarsFromFile(cars);

    int choice;
    while (true) {
        cout << "\n===== Car Rental System =====\n";
        cout << "1. Add Car\n";
        cout << "2. View Available Cars\n";
        cout << "3. Rent a Car\n";
        cout << "4. Return a Car\n";
        cout << "5. Search Car by ID\n";
        cout << "6. Delete Car\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int id;
            string model, brand;
            double price;
            cout << "Enter Car ID: ";
            cin >> id;
            cin.ignore();

            if (!isCarIDUnique(cars, id)) {
                cout << "Error! Car ID already exists.\n";
                continue;
            }

            cout << "Enter Model: ";
            getline(cin, model);
            cout << "Enter Brand: ";
            getline(cin, brand);
            cout << "Enter Rental Price per day: ?";
            cin >> price;

            cars.push_back(Car(id, model, brand, price));
            saveCarsToFile(cars);
            cout << "Car added successfully!\n";

        } else if (choice == 2) {
            cout << "\n===== Available Cars =====\n";
            for (const auto &c : cars) {
                if (!c.getIsRented()) c.displayCar();
            }

        } else if (choice == 3) {
            int id;
            cout << "Enter Car ID to rent: ";
            cin >> id;
            Car* car = findCar(cars, id);

            if (car) {
                car->rentCar();
                saveCarsToFile(cars);
            } else {
                cout << "Car not found!\n";
            }

        } else if (choice == 4) {
            int id;
            cout << "Enter Car ID to return: ";
            cin >> id;
            Car* car = findCar(cars, id);

            if (car) {
                car->returnCar();
                saveCarsToFile(cars);
            } else {
                cout << "Car not found!\n";
            }

        } else if (choice == 5) {
            int id;
            cout << "Enter Car ID to search: ";
            cin >> id;
            Car* car = findCar(cars, id);

            if (car) {
                car->displayCar();
            } else {
                cout << "Car not found!\n";
            }

        } else if (choice == 6) {
            int id;
            cout << "Enter Car ID to delete: ";
            cin >> id;
            deleteCar(cars, id);

        } else if (choice == 7) {
            cout << "Exiting Car Rental System. Thank you!\n";
            break;
        } else {
            cout << "Invalid choice! Try again.\n";
        }
    }
    return 0;
}
