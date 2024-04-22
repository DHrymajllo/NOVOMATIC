#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

class TicketMachine {
public:
    TicketMachine() {
        currentBalance = 0;
        state = State::WELCOMING;
        welcomeMessage();
    }

    void processInput(const std::string& input) {
        if (input == "Stan poprzedni") {
            cancel();
            return;
        }

        if (state == State::WELCOMING) {
            std::istringstream iss(replaceCommasWithSpaces(input));
            int coin;
            while (iss >> coin) {
                insertMoney(coin);
            }
            displayBalanceAndTickets();
        }
        else if (state == State::CHOOSING_TICKET) {
            try {
                int ticketPrice = std::stoi(input);
                chooseTicket(ticketPrice);
            }
            catch (const std::invalid_argument&) {
                std::cout << "Nieprawidlowe dane. Proszê sprobowac ponownie." << std::endl;
            }
        }
    }

private:
    enum class State {
        WELCOMING,
        CHOOSING_TICKET
    };

    int currentBalance;
    int selectedTicketPrice;
    State state;

    void welcomeMessage() {
        std::cout << "Witaj, z tej strony automat biletowy miasta Krakow, oczekuje wrzucenia do mnie pieniedzy" << std::endl;
    }

    void insertMoney(int coin) {
        currentBalance += coin;
    }

    void chooseTicket(int ticketPrice) {
        if (ticketPrice <= currentBalance) {
            currentBalance -= ticketPrice;
            selectedTicketPrice = ticketPrice;
            printTicket();
            giveChange();
            thankYou();
            reset();
        }
        else {
            std::cout << "Niewystarczajace saldo." << std::endl;
        }
    }

    void displayBalanceAndTickets() {
        std::cout << "Twoje saldo wynosi: " << currentBalance << ", wybierz bilet ktory chcesz kupic: 3, 4, 8, 10, 15" << std::endl;
        state = State::CHOOSING_TICKET;
    }

    void printTicket() {
        std::cout << "Trwa proces drukowania biletow *BRRR* *BRRR* *BRRR*" << std::endl;
        std::cout << "Bilet zostal wydrukowany ";
    }

    void giveChange() {
        if (currentBalance > 0) {
            std::cout << "oto Twoja reszta: ";
            std::vector<int> denominations = { 5, 2, 1 };
            for (int i = 0; i < denominations.size(); ++i) {
                int count = currentBalance / denominations[i];
                std::cout << count << "*" << denominations[i] << " zl";
                if (i < denominations.size() - 1) {
                    std::cout << ", ";
                }
                currentBalance -= count * denominations[i];
            }
            std::cout << std::endl;
        }
    }

    void thankYou() {
        std::cout << "Zycze milego dnia Twoj bilecik " << selectedTicketPrice << " oczekuje na odbior" << std::endl;
    }

    void cancel() {
        std::cout << "Oddaje Twoje monety" << std::endl;
        reset();
    }

    void reset() {
        currentBalance = 0;
        state = State::WELCOMING;
        welcomeMessage();
    }

    std::string replaceCommasWithSpaces(const std::string& input) {
        std::string output = input;
        std::replace(output.begin(), output.end(), ',', ' ');
        return output;
    }
};
#ifndef UNIT_TESTING
int main() {
    TicketMachine machine;
    std::string input;
    while (true) {
        std::getline(std::cin, input);
        if (input == "exit") break;
        machine.processInput(input);
    }
    return 0;
}
#endif
