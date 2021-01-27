#include <iostream>
#include <bitset>
#include <array>
#include <limits>

using namespace std;
using T = char;
using BinaryStorage = array<bool, 8 * sizeof(T)>;

//----------------------------Converter--------------------------//

BinaryStorage convert_to_binary_num(T num) {
    T mask;
    BinaryStorage binary_num;
    const int SIZE = binary_num.size();

    cout << "Number in binary radix: ";

    switch (sizeof(T)) {
        case 1 : //mask for char(8)
            mask = 0100;
            break;

        case 2 : //mask for short int(16)
            mask = 040000;
            break;

        case 4 : //mask for int(32)
            mask = 010000000000;
            break;
    }

    binary_num[0] = (num < 0 ? 1 : 0);
    for (int i = 1; i < SIZE; ++i) {
        binary_num[i] = (num & mask ? 1 : 0);
        mask >>= 1;
    }
    for (int i = 0; i < SIZE; i++) {
        cout << binary_num[i];
    }

    cout << endl;
    return binary_num;
}

//----------------------------Binary Number-------------------------------//
template<typename T>
class BinaryNumber {
    const BinaryStorage binary_num_;

public:
    BinaryNumber() :
            binary_num_({}) {}

    explicit BinaryNumber(T num) :
            binary_num_(convert_to_binary_num(num)) {
    }

    explicit BinaryNumber(BinaryStorage binary_num) :
            binary_num_(binary_num) {}

    BinaryNumber operator+(const BinaryNumber &other) const {
        BinaryStorage res;
        BinaryStorage carries;
        const auto size = binary_num_.size();

        bool carry = false;
        for (int i = size - 1; i >= 0; i--) {
            auto a = binary_num_[i];
            auto b = other.binary_num_[i];
            res[i] = ((a ^ b) ^ carry);
            carry = ((a & b) | (a & carry)) | (b & carry);
            carries[i] = carry;
        }
        if ((!carries[0] && carries[1]) || (!carries[1] && carries[0])) {
            throw std::runtime_error("Overflow error");
        }
        return BinaryNumber(res);
    }

    BinaryNumber<T> operator*(const BinaryNumber<T> &other) const {
        BinaryStorage res;
    }

    friend ostream &operator<<(ostream &stream, const BinaryNumber &binary_num) {
        for (auto element: binary_num.binary_num_) {
            stream << element;
        }
        stream << endl;
        return stream;
    }
};


//-----------------------------Parsing expression ------------------------------------//
class Expression {
public:
    const T number1;
    const T number2;
    const char oper;

    Expression(T number1, T number2, char oper):
        number1(number1), number2(number2), oper(oper){}

    auto calculate() {
        BinaryNumber<T> binary_num1(number1);

        if (oper == '+') {
            BinaryNumber<T> binary_num2(number2);
            return binary_num1 + binary_num2;
        } else if (oper == '-') {
            BinaryNumber<T> binary_num2(-number2);
            return binary_num1+binary_num2;
        } else if (oper == '*') {
            BinaryNumber<T> binary_num2(number2);
            return binary_num1 * binary_num2;
        } else {
            throw runtime_error("This is not operator");
        }
    }

};


void check_segment_entery(int number) {

    if (number < numeric_limits<T>::min() || number > numeric_limits<T>::max()) {
        throw runtime_error("number is out of range");
    }

}



//---------------------------Enter String-------------------------//
Expression getNumberFromConsole() {
    int num1;
    int num2;
    char oper;
    cout << "Enter the expression" << endl;
    cout << "Number 1: ";
    cin >> num1;
    check_segment_entery(num1);
    cout << "Operator: ";
    cin >> oper;
    cout << "Number 2: ";
    cin >> num2;
    check_segment_entery(num2);
    return Expression(num1, num2, oper);
}

int main() {
    auto expression = getNumberFromConsole();
    cout << "Check your expression: " << int(expression.number1) << expression.oper << int(expression.number2)<< endl;
    auto res = expression.calculate();
    cout << "Result is: " << res << endl;

}