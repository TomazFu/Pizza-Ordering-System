#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>


using namespace std;

// Struct datatypes
// Defining Pizza with members of name and price
// Also defining a function (constructor) named print()
struct Pizza{
	string name;
	double price;
	
	void print(int i){
		cout << left << i << " [" << setw(18) << name << ": RM" << price << "]" << endl;
	}
};

// Defining Order with members of orderNo, date, pizza[]
struct Order {
	int orderNo;
	string date;
	Pizza pizza[8];
};

//The order size of pizza is 8(constant) 
const int PIZZA_ORDER_SIZE = 8;
//The total orders quantity is 10(constant)
const int ORDER_SIZE = 10;

// Declare functions prototypes
void showMainPage();
int enterOption();
void showMenu();
void processOption(int option, int quantity[][PIZZA_ORDER_SIZE], Order orderList[], int & i, int & j);
bool checkUserInput(int pizzaType);
void processOrder(int pizzaType, int i, Order & order);
void recordOrder(string name, int i, Order & order);
void printOrder(Order order, int quantity[][PIZZA_ORDER_SIZE], int i);
int findQuantitySize(int quantity[][PIZZA_ORDER_SIZE], int i);
void saveOrder(Order order, int quantity[][PIZZA_ORDER_SIZE], int i, ofstream& outfile);
int readOrderNo();
void incrementOrderNo(int currentOrderNo);

//Main function (Select option)
int main(){
	int option = 0;
	int quantity[ORDER_SIZE][PIZZA_ORDER_SIZE]; //Declare a 2D quantity array to record quantity
	Order orderList[ORDER_SIZE]; //Declare a Order list to store orders
	int i, j;
	i = j = 0;
	memset(quantity, 0, sizeof(quantity)); // initialize all empty elements of quantity list into 0
	
	do { // Loops until option == 3
		showMainPage();
		option = enterOption();
		processOption(option, quantity, orderList, i, j); //option processed in this function
	} while (option != 3);
	
	return 0;
}

//Function that shows the main user interface
void showMainPage() {
	cout << "\nPizza Ordering System \n\n\n";
    cout << "1 [Order Pizza]\n";
    cout << "2 [View Order ]\n";
    cout << "3 [Exit       ]\n\n";
}

//Function that records your option choices and return a int value
int enterOption() {
    int option;
    cout << "Enter your option: ";
    cin >> option;
	return option;
}

//Function that shows the pizza list by opening and accessing Pizza.txt file
void showMenu(){
	cout << "\n\nMain->Order Pizza\n\n";
    cout << "<<Pizza Menu>>\n\n";
    ifstream file("Pizza.txt");
    string line;
    Pizza pizza;
    int i = 1;
    string temp1, temp2 = "";

    if (file.is_open()) {
        do {
			getline(file, temp1, '\n');
			getline(file, temp2, '\n');
			
			pizza.name = temp1;
			pizza.price = stod(temp2);
			pizza.print(i);
			i++;
		} while (!file.eof());
        file.close();
    } else {
        cout << "\nUnable to open menu file.\n" << endl;
    }
}

// Require option choice, quantity of pizzas, orderList, and 2 variable to identify pizza quantity in the 2D quantity array
void processOption(int option, int quantity[][PIZZA_ORDER_SIZE], Order orderList[], int & i, int & j) {
	
	// Switch-case statement starts
	switch (option) {
		// if option == 1
		case 1: {
			// initialize variables
			int pizzaType = 0;
			int pizzaQty = 0;
			string date = "";
			char continueFlag = ' ';
			Order order;
			// Enter date manually
			cout << "\nEnter Date(d/m/yy) : ";
		    cin >> date;
			// Check if number of pizzas ordered is more than the array max capacity
			while (j < PIZZA_ORDER_SIZE){
				// Shows menu by extracting data from Pizza.txt
				showMenu(); 
				// User selects pizza number
				cout << "\nSelect Pizza : ";
		    	cin >> pizzaType;
		    	// Check if input is valid
			    if (!checkUserInput(pizzaType)){
			    	cout << "\nInvalid input. Please try again.\n";
					continue;
				}
				// User enters pizza quantity
			    cout << "\nEnter Quantity : ";
		    	cin >> pizzaQty;
		    	// Pizza quantity is stored into a specific location in quantity array
		    	quantity[i][j] = pizzaQty;
		    	// Order is processed through processOrder() function
		    	processOrder(pizzaType, j, order);
		    	// Increment index by 1
		    	j++;
		    	// Ask if continue to order the next pizza   	
				cout << "\nContinue next pizza [Y/N]: ";
	            cin >> continueFlag;
	            // If no, save details and print out order by printOrder() function
	            if(continueFlag == 'N' || continueFlag == 'n'){
	            	order.date = date;
	            	order.orderNo = i + 1;
					printOrder(order, quantity, i);
					j = 0;
					orderList[i] = order;
					cout << "\nOrder details saved." << endl;
					break;
	            // If yes, continue the loop
	            } else if (continueFlag == 'Y' || continueFlag == 'y'){
					continue;
				// If invalid input, return to the top of the loop and run again. Order record will be saved.
	            } else {
	                cout << "\nInvalid input. Please try again.\n";
	                continue;	
	            }		
			}
			// Prompt a message if pizza ordered is full for a single receipt
			if (j >= PIZZA_ORDER_SIZE){
				cout << "Order is full for a single receipt now.\n" << endl;
			}	
			// Increment index by 1
			i++;
			break;
		}
		// If user's option == 2
		case 2: {
			int targetOrderNo = 0;
			// Prompt user to enter the order no desired
			cout << "Please enter the order No: ";
			cin >> targetOrderNo;
			// Check if input is valid
			if (targetOrderNo > i || targetOrderNo < 1){
				cout << "Order not found in the system." << endl;
			} else {
				// Print order if input is valid
				printOrder(orderList[targetOrderNo - 1], quantity, targetOrderNo - 1);
			}
			break;
		}
		// If user's option == 3
		case 3: {
			// Create a new file to record order details
			// File named as order followed by the last order number
			// Last order number is read from readOrderNo() function
			string filename = "order" + to_string(readOrderNo()) + ".txt";
		    ofstream file(filename, ios::app);
		    // If file exists
		    if (file.is_open()) {
		    	// Save all orders in orderList through saveOrder() function and for loops
		    	for(int count = 0; count < i; count++){
		    		saveOrder(orderList[count], quantity, count, file);
				}
		        
		        file.close();
		        cout << "\nOrder details saved in file: " << filename << endl;
		        // increase the number by 1 and save it back to the file for the next order number execution
				incrementOrderNo(readOrderNo() + 1);
		    } else {
		    	// If file does not exist
		        cout << "\nError saving order details to file." << endl;
		    }
		    // print out thank you at last
			cout << "Thank you and have a nice day!" << endl;
			break;
		}
		// If invalid option, show error message
		default: {
			cout << "Invalid option!" << endl;
			cout << "Please try again..." << endl;
		}
	}
}
// function to check if the user input is valid
bool checkUserInput(int pizzaType){
	// Pizza.txt file is opened and read
	ifstream file("Pizza.txt");
	string line = "";
	int currentLine = 1;
	if (file.is_open()) {
		// Count total numbers of pizza in menu
        while (getline(file, line)) {
        	// save into variable named currentLine
        	currentLine++;
		}
        file.close();
    } else {
    	// Pizza.txt file corrupts or missing, then print error message
        cout << "\nError occured. Please try again.\n" << endl;
    }
    // Uses formula to calculate if user input is valid
    if(pizzaType > ((currentLine-1)/2) || pizzaType < 1){
    	// return false if invalid
    	return false;
	} else {
		// return true if valid
		return true;
	}
}
// function to process the pizza number
void processOrder(int pizzaType, int i, Order & order){
	// Pizza.txt file is opened and read
	ifstream file("Pizza.txt");
	string line = "";
	int currentLine = 1;
	int currentName = 1;
	if (file.is_open()) {
        while (getline(file, line)) {
        	if (currentLine % 2 == 0){
        		// ignore the price of pizza as Pizza.txt consists of pizza name and pizza price
        		currentLine++;
        		continue;
			}
		    if (currentName == pizzaType) {
		    	// if index found same as user's input then proceed to record the order
				recordOrder(line, i, order);
		        break;
		    }
        	currentLine++;
		    currentName++;
		}
		
        file.close();
    } else {
    	// If Pizza.txt corrupts or missing, Error message prompts
        cout << "\nError occured. Please try again.\n" << endl;
    }
}
// function to record order details into Order
void recordOrder(string name, int i, Order & order){
	// Insert pizza name into order
	order.pizza[i].name = name;
	// Pizza.txt is opened and read
	ifstream file("Pizza.txt");
    string line;
    Pizza pizza;
    string temp1, temp2 = "";
	// if Pizza.txt available then save pizza price into order
    if (file.is_open()) {
        do {
			getline(file, temp1, '\n');
			getline(file, temp2, '\n');
			
			if(order.pizza[i].name == temp1){
				// stod() is to convert string to double as information extracted from file is a string type
				order.pizza[i].price = stod(temp2);
				break;
			}

		} while (!file.eof());
        file.close();
         cout << "\nOrder recorded." << endl;
    } else {
    	// If Pizza.txt corrupts or missing, prompt error message
        cout << "\nUnable to open menu file.\n" << endl;
    }
}
// function to print order details in receipt form
void printOrder(Order order, int quantity[][PIZZA_ORDER_SIZE], int i){
	double total = 0;
	// findQuantitySize() functions' purpose is to set limit to looping the print order in receipt
	int quantitySize = findQuantitySize(quantity, i);
	// print receipt with the format specified
	cout << fixed << showpoint << setprecision(2) << left <<
	 "Order No: " << setw(10) << order.orderNo << "Date: " << order.date << endl;
	cout << left << setw(10) << "No " << setw(20) << "Pizza" << setw(10) << right << "Quantity" << setw(10) << "Total" << endl;
	cout << "------------------------------------------------------" << endl;
	for(int j = 0; j < quantitySize; j++){
		// print name, price, quantity and total of pizza ordered
		cout << left << setw(10) << j+1 << setw(20) << order.pizza[j].name << setw(10) << right << quantity[i][j] << setw(10) << order.pizza[j].price * quantity[i][j] << endl;
		total += (order.pizza[j].price * quantity[i][j]);
	}
	cout << "------------------------------------------------------" << endl;
	// calculate to print tax and total
	cout << setw(40) << "Tax (5%)" << setw(10) << total * .05 << endl;
	cout << setw(40) << "TOTAL" << setw(10) << total * 1.05 << endl;
	cout << "------------------------------------------------------" << endl;
}
// function to find the total quantity size of the quantity array
int findQuantitySize(int quantity[][PIZZA_ORDER_SIZE], int i){
	int quantitySize = 0;
	for(int j = 0; j < PIZZA_ORDER_SIZE; j++){
		if(quantity[i][j] > 0){
			// Since unused memory of quantity array is assigned to 0, if elements > 0, then quantity size can be determined
			quantitySize += 1;
		}
	}
	// Quantity size is returned
	return quantitySize;
}
// function to save order details in receipt format into txt file
// The only thing that differs from saveOrder() and printOrder() is saveOrder uses keyword outfile, which saves the details into file instead of cout, which prints in the CLI
void saveOrder(Order order, int quantity[][PIZZA_ORDER_SIZE], int i, ofstream& outfile){
	double total = 0;
	int quantitySize = findQuantitySize(quantity, i);
	outfile << fixed << showpoint << setprecision(2) << left <<
	 "Order No: " << setw(10) << order.orderNo << "Date: " << order.date << endl;
	outfile << left << setw(10) << "No " << setw(20) << "Pizza" << setw(10) << right << "Quantity" << setw(10) << "Total" << endl;
	outfile << "------------------------------------------------------" << endl;
	for(int j = 0; j < quantitySize; j++){
		outfile << left << setw(10) << j+1 << setw(20) << order.pizza[j].name << setw(10) << right << quantity[i][j] << setw(10) << order.pizza[j].price * quantity[i][j] << endl;
		total += (order.pizza[j].price * quantity[i][j]);
	}
	outfile << "------------------------------------------------------" << endl;
	outfile << setw(40) << "Tax (5%)" << setw(10) << total * .05 << endl;
	outfile << setw(40) << "TOTAL" << setw(10) << total * 1.05 << endl;
	outfile << "------------------------------------------------------" << endl;
	outfile << endl;
}
// function that reads the number of orderNo.txt file and returns it back to the program
int readOrderNo() {
	int currentOrderNo;
    ifstream file("orderNo.txt");
    // if file not found, return 1 back as no record was found and this is the first order number
    if (!file) {
        return 1;
    }

    file >> currentOrderNo;
    file.close();
    return currentOrderNo;
}
// function that increases orderNo by 1 and write it into orderNo.txt file
void incrementOrderNo(int currentOrderNo) {
    ofstream file("orderNo.txt");
    // Prompts error message if file is corrupted or missing
    if (!file) {
        cout << "Error opening file." << endl;
        return;
    }

    file << currentOrderNo;
    file.close();
}
