//
//  main.cpp
//  BigInt
//
//  Created by Zhalgas Baibatyr on 5/19/2016.
//  Copyright © 2018 Zhalgas Baibatyr. All rights reserved.
//

#include <iostream>
#include <string.h>
#include <chrono>

/* Big Integer class */
class BigInt
{
public:
    
    /* Default constructor: */
    BigInt()
    :   numberOfDigits(0),
    value(nullptr),
    isNegative(false)
    {
    }
    
    /* Copy constructor: */
    BigInt(const BigInt& bigInt)
    :   numberOfDigits(bigInt.numberOfDigits),
    value(new char[numberOfDigits]),
    isNegative(bigInt.isNegative)
    {
        for (size_t i = 0; i < numberOfDigits; ++i)
        {
            value[i] = bigInt[i];
        }
    }
    
    /* Move constructor: */
    BigInt(BigInt &&bigInt)
    :   numberOfDigits(bigInt.numberOfDigits),
    value(bigInt.value),
    isNegative(bigInt.isNegative)
    {
        bigInt.value = nullptr;
    }
    
    /* Constructor with initial value: */
    BigInt(const char *str)
    {
        *this = str;
    }
    
    /* Destructor: */
    ~BigInt()
    {
        delete[] value;
    }
    
    /* Copy assignment operator: */
    BigInt & operator= (const BigInt &bigInt)
    {
        *this = BigInt(bigInt);
        
        return *this;
    }
    
    /* Move assignment operator: */
    BigInt & operator= (BigInt &&bigInt)
    {
        delete[] value;
        numberOfDigits = bigInt.numberOfDigits;
        value = bigInt.value;
        isNegative = bigInt.isNegative;
        
        bigInt.value = nullptr;
        
        return *this;
    }
    
    /* Assign value: */
    BigInt & operator= (const char *str)
    {
        if (str == NULL)
        {
            throw std::invalid_argument("NULL");
        }
        
        if (*str == 45)
        {
            numberOfDigits = strlen(str) - 1;
            isNegative = true;
            ++str;
        }
        else
        {
            numberOfDigits = strlen(str);
            isNegative = false;
        }
        
        if (numberOfDigits == 0)
        {
            throw std::invalid_argument(str);
        }
        
        value = new char[numberOfDigits];
        for (int i = int(numberOfDigits - 1); i > -1; --i)
        {
            if (*str < 48 || *str > 57)
            {
                throw std::invalid_argument(str - numberOfDigits + i + 1);
            }
            
            value[i] = *str - 48;
            ++str;
        }
        
        return *this;
    }
    
    /* Set as negative: */
    BigInt operator- () const
    {
        BigInt bigInt(*this);
        if (bigInt.isNegative)
        {
            bigInt.isNegative = false;
        }
        else if (!(bigInt.numberOfDigits == 1 && bigInt[0] == 0))
        {
            bigInt.isNegative = true;
        }
        
        return bigInt;
    }
    
    /* Addition operator: */
    BigInt operator+ (const BigInt &bigInt) const
    {
        if (this->isNegative != bigInt.isNegative)
        {
            if (this->isNegative)
            {
                BigInt diff(*this);
                diff.isNegative = false;
                diff -= bigInt;
                diff.isNegative = !diff.isNegative;
                
                return diff;
            }
            else
            {
                BigInt diff(bigInt);
                diff.isNegative = false;
                diff -= *this;
                diff.isNegative = !diff.isNegative;
                
                return diff;
            }
        }
        
        size_t N1 = numberOfDigits;
        size_t N2 = bigInt.numberOfDigits;
        const BigInt *pBigInt;
        if (N1 < N2)
        {
            N1 = bigInt.numberOfDigits;
            N2 = numberOfDigits;
            pBigInt = &bigInt;
        }
        else
        {
            pBigInt = this;
        }
        
        BigInt sum(N1 + 1);
        sum.isNegative = isNegative;
        char tmpVal;
        size_t i = 0;
        
        while (i < N2)
        {
            tmpVal = value[i] + bigInt[i] + sum[i];
            sum[i] = tmpVal % 10;
            sum[i + 1] = tmpVal / 10;
            ++i;
        }
        
        while (i < N1)
        {
            tmpVal = pBigInt->value[i] + sum[i];
            sum[i] = tmpVal % 10;
            sum[i + 1] = tmpVal / 10;
            ++i;
        }
        
        while (sum.numberOfDigits > 1 && sum[sum.numberOfDigits - 1] == 0)
        {
            --sum.numberOfDigits;
        }
        
        return sum;
    }
    
    /* "Increment by value" operator: */
    BigInt & operator+= (const BigInt &bigInt)
    {
        *this = *this + bigInt;
        
        return *this;
    }
    
    /* "Pre increment" operator: */
    BigInt & operator++ ()
    {
        *this = *this + BigInt("1");
        
        return *this;
    }
    
    /* "Post increment" operator: */
    BigInt operator++ (int)
    {
        BigInt bigInt(*this);
        ++(*this);
        
        return bigInt;
    }
    
    /* "Difference" operator: */
    BigInt operator- (const BigInt &bigInt) const
    {
        if (this->isNegative != bigInt.isNegative)
        {
            if (this->isNegative)
            {
                BigInt negSum(*this);
                negSum.isNegative = false;
                negSum += bigInt;
                negSum.isNegative = true;
                
                return negSum;
            }
            else
            {
                BigInt sum(bigInt);
                sum.isNegative = false;
                sum += *this;
                
                return sum;
            }
        }
        
        const size_t &N1 = numberOfDigits;
        const size_t &N2 = bigInt.numberOfDigits;
        
        size_t N;
        BigInt diff;
        const BigInt *pBigInt;
        if (N1 > N2)
        {
            N = N2;
            diff = *this;
            pBigInt = &bigInt;
        }
        else if (N1 < N2)
        {
            N = N1;
            diff = bigInt;
            pBigInt = this;
            diff.isNegative = true;
        }
        else
        {
            N = N1;
            size_t i = N - 1;
            while (i > 0 && value[i] == bigInt[i])
            {
                --i;
            }
            
            if (value[i] < bigInt[i])
            {
                diff = bigInt;
                pBigInt = this;
                diff.isNegative = true;
            }
            else
            {
                diff = *this;
                pBigInt = &bigInt;
            }
        }
        
        for (size_t i = 0, j; i < N; ++i)
        {
            diff[i] -= pBigInt->value[i];
            j = i;
            while (diff[j] < 0)
            {
                diff[j] += 10;
                ++j;
                --diff[j];
            }
        }
        
        while (diff.numberOfDigits > 1 && diff[diff.numberOfDigits - 1] == 0)
        {
            --diff.numberOfDigits;
        }
        
        if (diff.numberOfDigits == 1 && diff[0] == 0)
        {
            diff.isNegative = false;
        }
        
        return diff;
    }
    
    /* "Decrement by value" operator: */
    BigInt & operator-= (const BigInt &bigInt)
    {
        *this = *this - bigInt;
        
        return *this;
    }
    
    /* "Pre decrement" operator: */
    BigInt & operator-- ()
    {
        *this = *this - BigInt("1");
        
        return *this;
    }
    
    /* "Post decrement" operator: */
    BigInt operator-- (int)
    {
        BigInt bigInt(*this);
        --(*this);
        
        return bigInt;
    }
    
    /* "Multiplication" operator: */
    BigInt operator* (const BigInt &bigInt) const
    {
        const size_t &N1 = numberOfDigits;
        const size_t &N2 = bigInt.numberOfDigits;
        
        BigInt product(N1 + N2);
        
        char tmpVal;
        for (size_t i = 0; i < N2; ++i)
        {
            for (size_t j = 0; j < N1; ++j)
            {
                tmpVal = value[j] * bigInt[i] + product[i + j];
                for (size_t k = j; k < N1 + 1; ++k)
                {
                    product[i + k] = tmpVal % 10;
                    
                    tmpVal /= 10;
                    if (tmpVal == 0)
                    {
                        break;
                    }
                    
                    tmpVal += product[i + k + 1];
                }
            }
        }
        
        while (product.numberOfDigits > 1 && product[product.numberOfDigits - 1] == 0)
        {
            --product.numberOfDigits;
        }
        
        if (!(product.numberOfDigits == 1 && product[0] == 0))
        {
            product.isNegative = (this->isNegative != bigInt.isNegative);
        }
        
        return product;
    }
    
    /* "Multiplication assignment" operator: */
    BigInt & operator*= (const BigInt &bigInt)
    {
        *this = *this * bigInt;
        
        return *this;
    }
    
    /* "Division" operator: */
    BigInt operator/ (const BigInt &bigInt) const
    {
        /* Check if divisor is equal to zero or one: */
        if (bigInt.numberOfDigits == 1)
        {
            if (bigInt[0] == 0)
            {
                throw std::runtime_error("Division by zero!");
            }
            else if (bigInt[0] == 1)
            {
                return (this->isNegative == bigInt.isNegative) ? *this : -*this;
            }
        }
        
        /* Check if dividend is equal to zero or divident is less than divisor: */
        if ((numberOfDigits == 1 && *value == 0) ||
            (this->isNegative ? -*this : *this) < (bigInt.isNegative ? -bigInt : bigInt))
        {
            return BigInt("0");
        }
        
        const size_t &N1 = numberOfDigits;
        const size_t &N2 = bigInt.numberOfDigits;
        
        BigInt minuend(N2);
        for (size_t i = N2; i > 0; --i)
        {
            minuend[N2 - i] = value[N1 - i];
        }
        
        const BigInt subtrahend(bigInt.isNegative ? -bigInt : bigInt);
        BigInt quotient(N1 == N2 ? 1 : N1 - N2 + (minuend < subtrahend ? 0 : 1));
        quotient.isNegative = (this->isNegative != bigInt.isNegative);
        
        char lower;
        char upper;
        BigInt middle(1);
        int j = int(N1 - N2);
        size_t nPulls = 0;
        size_t k = 1;
        do
        {
            if (minuend < subtrahend)
            {
                nPulls = subtrahend.numberOfDigits - minuend.numberOfDigits;
                if (minuend.numberOfDigits == 1 && minuend[0] == 0)
                {
                    ++nPulls;
                    if (j < nPulls)
                    {
                        break;
                    }
                    
                    for (size_t i = 1; i < nPulls; ++i, ++k)
                    {
                        quotient[quotient.numberOfDigits - k] = 0;
                    }
                    
                    minuend = BigInt(nPulls);
                }
                else
                {
                    BigInt tmpBigInt((!nPulls ? ++nPulls : nPulls) + 1);
                    for (size_t i = 1; i < nPulls; ++i, ++k)
                    {
                        quotient[quotient.numberOfDigits - k] = 0;
                    }
                    
                    tmpBigInt[nPulls] = 1;
                    minuend *= tmpBigInt;
                }
                
                for (size_t i = nPulls; i > 0; --i)
                {
                    minuend[nPulls - i] = value[j - i];
                }
            }
            
            lower = 0;
            upper = 10;
            middle[0] = 4;
            while (true)
            {
                if (minuend >= (middle * subtrahend))
                {
                    lower = middle[0];
                }
                else
                {
                    upper = middle[0];
                }
                
                middle[0] = (lower + upper) / 2;
                if (upper - lower < 2)
                {
                    minuend -= (middle * subtrahend);
                    break;
                }
            }
            
            if (quotient.numberOfDigits >= k)
            {
                quotient[quotient.numberOfDigits - k] = middle[0];
            }
            
            j -= nPulls;
            ++k;
        }
        while (j > 0);
        
        return quotient;
    }
    
    /* "Division assignment" operator: */
    BigInt & operator/= (const BigInt &bigInt)
    {
        *this = *this / bigInt;
        
        return *this;
    }
    
    /* "Modulo" operator: */
    BigInt operator% (const BigInt &bigInt) const
    {
        BigInt reminder = *this - bigInt * (*this / bigInt);
        
        return reminder;
    }
    
    /* "Modulo assignment" operator: */
    BigInt & operator%= (const BigInt &bigInt)
    {
        
        *this -= bigInt * (*this / bigInt);
        
        return *this;
        
    }
    
    /* Calculate power by value: */
    BigInt operator^ (BigInt exp) const
    {
        if (exp == "0")
        {
            return "1";
        }
        
        BigInt power(*this);
        BigInt extraValue("1");
        while (exp > "1")
        {
            if (exp % "2" == "1") {
                extraValue *= power;
            }
            exp /= "2";
            power *= power;
        }
        
        if (extraValue != "1")
        {
            power *= extraValue;
        }
        
        return power;
    }
    
    /* "Equal to" operator: */
    bool operator== (const BigInt &bigInt) const
    {
        if (numberOfDigits != bigInt.numberOfDigits || isNegative != bigInt.isNegative)
        {
            return false;
        }
        
        for (int i = int(numberOfDigits - 1); i > -1; --i)
        {
            if (value[i] != bigInt[i])
            {
                return false;
            }
        }
        
        return true;
    }
    
    /* "Not equal to" operator: */
    inline bool operator!= (const BigInt &bigInt) const
    {
        return !(*this == bigInt);
    }
    
    /* "Greater than" operator: */
    bool operator> (const BigInt &bigInt) const
    {
        bool isOpposite = false;
        if (isNegative)
        {
            if (bigInt.isNegative)
            {
                isOpposite = true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (bigInt.isNegative)
            {
                return true;
            }
        }
        
        if (numberOfDigits > bigInt.numberOfDigits)
        {
            return isOpposite ? false : true;
        }
        else if (numberOfDigits == bigInt.numberOfDigits)
        {
            for (int i = int(numberOfDigits - 1); i > -1; --i)
            {
                if (value[i] > bigInt[i])
                {
                    return isOpposite ? false : true;
                }
                else if (value[i] < bigInt[i])
                {
                    return isOpposite ? true : false;
                }
            }
        }
        
        return false;
    }
    
    /* "Greater than or equal to" operator: */
    inline bool operator>= (const BigInt &bigInt) const
    {
        return !(*this < bigInt);
    }
    
    /* "Less than" operator: */
    bool operator< (const BigInt &bigInt) const
    {
        bool isOpposite = false;
        if (isNegative)
        {
            if (bigInt.isNegative)
            {
                isOpposite = true;
            }
            else
            {
                return true;
            }
        }
        else
        {
            if (bigInt.isNegative)
            {
                return false;
            }
        }
        
        if (numberOfDigits < bigInt.numberOfDigits)
        {
            return isOpposite ? false : true;
        }
        else if (numberOfDigits == bigInt.numberOfDigits)
        {
            for (int i = int(numberOfDigits - 1); i > -1; --i)
            {
                if (value[i] < bigInt[i])
                {
                    return isOpposite ? false : true;
                }
                else if (value[i] > bigInt[i])
                {
                    return isOpposite ? true : false;
                }
            }
        }
        
        return false;
    }
    
    /* "Less than or equal to" operator: */
    inline bool operator<= (const BigInt &bigInt) const
    {
        return !(*this > bigInt);
    }
    
    /* Get number of digits:  */
    inline size_t size() const
    {
        return numberOfDigits;
    }
    
    /* Input stream: */
    friend std::istream & operator>> (std::istream &in, BigInt &bigInt)
    {
        std::string str;
        in >> str;
        bigInt = str.c_str();
        
        return in;
    }
    
    /* Output stream: */
    friend std::ostream & operator<< (std::ostream &out, const BigInt &bigInt)
    {
        if (bigInt.isNegative == true)
        {
            out << '-';
        }
        
        for (int i = int(bigInt.numberOfDigits - 1); i > -1; --i)
        {
            out << char(bigInt[i] + 48);
        }
        
        return out;
    }
    
    
private:
    
    /* Stored data: */
    size_t numberOfDigits;
    char *value;
    bool isNegative;
    
    /* Create BigInt with initial size: */
    BigInt(const size_t initSize)
    :   numberOfDigits(initSize),
    value(new char[numberOfDigits]),
    isNegative(false)
    {
        for (size_t i = 0; i < numberOfDigits; ++i)
        {
            value[i] = 0;
        }
    }
    
    /* Digit access by value: */
    inline char operator[] (const size_t i) const
    {
        return value[i];
    }
    
    /* Digit access by reference: */
    inline char & operator[] (const size_t i)
    {
        return value[i];
    }
};

int main(int argc, const char * argv[])
{
    BigInt bigInt1;
    BigInt bigInt2;
    std::cin >> bigInt1 >> bigInt2;
    
    auto start = std::chrono::steady_clock::now();
    std::cout << (bigInt1 ^ bigInt2) - (bigInt2 ^ bigInt1) << std::endl;
    auto end = std::chrono::steady_clock::now();
    
    auto diff = end - start;
    std::cout << std::chrono::duration<double, std::milli> (diff).count() << std::endl;
    
    return EXIT_SUCCESS;
}
