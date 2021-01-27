#include <iostream>
#include <array>
#include <limits>

using namespace std;
using T = char;
using BinaryStorage = array<bool, 8 * sizeof(T)>;


//----------------------------Binary Number-------------------------------//
template<typename T>
class BinaryNumber {
    BinaryStorage binary_num_;

public:
    BinaryNumber() :
            binary_num_({}) {}

    explicit BinaryNumber(T num) :
            binary_num_(convert_to_binary_num(num)) {
    }

    explicit BinaryNumber(BinaryStorage binary_num) :
            binary_num_(binary_num) {}


    size_t size() const {
        return  binary_num_.size();
    }
//-------------------------Converting------------------------------------//

    BinaryStorage convert_to_binary_num(T num) {
        T mask;
        BinaryStorage binary_num;
        const int SIZE = binary_num.size();

        printf("Number %d in binary radix: ",num);

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
            binary_num[i] = (abs(num) & mask ? 1 : 0);
            mask >>= 1;
        }
        for (int i = 0; i < SIZE; i++) {
            cout << binary_num[i];
        }

        cout << endl;
        return binary_num;
    }

    BinaryNumber complimentary() const {
        BinaryNumber res = *this;
        bool temp = res.binary_num_[0];
        for(int idx = 0; idx < size(); idx++) {
            res.binary_num_[idx] = !res.binary_num_[idx];
        }
        BinaryNumber comp;
        comp.binary_num_[size()-1] = true;
        res = res + comp;
        res.binary_num_[0] = temp;
        return res;
    }
    //--------------------Overloading----------------------//
    //sum
    BinaryNumber operator+(const BinaryNumber &other) const {
        BinaryStorage res;
        BinaryStorage carries;

        BinaryNumber A = *this;
        BinaryNumber B = other;

        if (A.binary_num_[0]) {
            A = A.complimentary();
        }

        if (B.binary_num_[0]) {
            B = B.complimentary();
        }

        bool carry = false;
        for (int i = size() - 1; i >= 0; i--) {
            auto a = A.binary_num_[i];
            auto b = B.binary_num_[i];
            res[i] = ((a ^ b) ^ carry);
            carry = ((a & b) | (a & carry)) | (b & carry);
            carries[i] = carry;
        }
        if ((!carries[0] && carries[1]) || (!carries[1] && carries[0])) {
            throw std::runtime_error("Overflow error");
        }
        return BinaryNumber(res);
    }
    //mul
    BinaryNumber shift_left() {
        BinaryNumber res = *this;
        auto temp = res.binary_num_[0];
        int idx = 0;
        for (idx = 0; idx < size()-1; idx++) {
            res.binary_num_[idx] = res.binary_num_[idx+1];
        }
        res.binary_num_[idx] = temp;
        return res;
    }

    BinaryNumber operator*(const BinaryNumber<T> &other) const {
        auto sign = binary_num_[0] ^ other.binary_num_[0];
        BinaryNumber acc;
        BinaryNumber multiplicand = *this;
        BinaryNumber multiplier = other;
        multiplicand.binary_num_[0] = false;
        multiplier.binary_num_[0] = false;

        for (int idx = size()-1; idx > 0; idx--) {
            if (multiplier.binary_num_[idx]) {
                acc = acc + multiplicand;
            }
            multiplicand = multiplicand.shift_left();
        }
        acc.binary_num_[0] = sign;
        if (sign) {
            return acc.complimentary();
        } else {
            return  acc;
        }

    }

    T convert_to_decimal_num(){
        const int n = size();
        int pow2=1;
        int dec_num = 0;

        for(int i=n-1;i>=0;i--){
            if (binary_num_[i]== 1)
                dec_num+=pow2;
            pow2*=2;
        }

        return dec_num;
    }

    friend ostream &operator<<(ostream &stream, const BinaryNumber &binary_num) {
        for (auto element: binary_num.binary_num_) {
            stream << element;
        }
        stream << endl;
        return stream;
    }
};



class Expression {
public:
    const T number1;
    const T number2;
    const char oper;

    Expression(T number1, T number2, char oper):
        number1(number1), number2(number2), oper(oper){}

    auto calculate() const {
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

void check_range(int number) {
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
    check_range(num1);
    cout << "Operator: ";
    cin >> oper;
    cout << "Number 2: ";
    cin >> num2;
    check_range(num2);
    return Expression(num1, num2, oper);
}

int main() {
    while (true) {
        auto expression = getNumberFromConsole();
        cout << "Check your expression: " << int(expression.number1) << expression.oper << int(expression.number2)<< endl;
        auto res = expression.calculate();
        cout << "Result in binary is: " << res;
        cout << "Result in decimal:" << int(res.convert_to_decimal_num()) << endl;
    }

}