#pragma once

/**
 *
 * The decimal class is designed for holding a number to x decimal places while safely storing accuracy and 
 * maintaining large numbers that can be easily compared down to the most minute digit. This can also be used\
 * to avoid the .1 + .1 + .1 issue and getting the accurate .3.
 * 
 * This class should have most of the required overloads to be used like any other fundamental type. Decimal can 
 * be set with any floating point type, or by int, long long.
 * 
 * To make sure that this class does not improperly interact with any other Decimal class that likely exists, it 
 * lives within the namespace ara::
 * 
 */

#include <math.h>									// for floor, round
#include <iostream>									// for std::cin and std::cout, likely called but better to not assume. 
#include <string>									// for std::string.

namespace ara {

	class Decimal
	{
	private:
		inline static int m_MaxDecimalPlaces{ 10 };		// defaults decimal to 10 places, can be increased or lowered as needed.

		int m_Whole = 0;								// the whole number component of the decimal
		long long m_Decimal = 0;						// the decimal number component of the decimal, long long should allow for signifint digits. 
		int m_Negative = 1;								// the positive or negative value of the number defined as +1 or -1 

	protected:
		long double power();							// power function, returns 10^decimal places for calculation later.
		void checkNegative();							// check if the number is negative or not. 
		std::string leadingZeros();						// get the leading zeros for the decimal.

	public:
		Decimal();										// default blank constructor
		Decimal(int whole, long long decimal);			// the dual int-based constructor that can be used, since both use the limits of class
		Decimal(int whole);								// for initializing whole numbers, without decimal places.
		Decimal(float d);								// float constructor, will be innacurate
		Decimal(double d);								// double constructor, better
		Decimal(long double d);							// long double constructor, ideal if integer assignment cannot be used.
		~Decimal();										// default destructor, will likely do nothing.

		static void setMaxDecPlaces(int maximum);		// maximum visible decimal places for universal usage.
		void setWhole(int whole);						// whole number overload
		void setDecimal(long long decimal);				// set decimal overload

		void loadDecimal(long long decimal);			// load by long long value, will break down by decimal places to proper numbers
		void loadDecimal(long double d);				// load by long double, works as an effective overload for most types. 
		void loadDecimal(int whole, long long decimal); // load by int and long long, for absolute definition.

		long double toLongDouble();						// degrade the decimal to a long double for use in other functions, if needed.
		double toDouble();								// degrade the decimal to a double for use in other functions if needed. 	
		float toFloat();								// degrade the decimal to a float for use in other functions if needed. 

		int getMaxDecPlaces();							// get max visible places, defined as an int.
		int getWhole();									// get whole number only.
		long long getDecimal();							// get decimal only.

		void print();									// print in cout. use << overload for inline call

		// operator overloads
		// mathmatical operators. + - * / %
		friend Decimal operator+(Decimal& d1, Decimal& d2);
		friend Decimal operator-(Decimal& d1, Decimal& d2);
		friend Decimal operator*(Decimal& d1, Decimal& d2);
		friend Decimal operator/(Decimal& d1, Decimal& d2);
		friend Decimal operator%(Decimal& d1, Decimal& d2);

		// unary operators -
		Decimal operator-();

		// comparison opertators == != < > <= >=
		friend bool operator==(Decimal& d1, Decimal& d2);
		friend bool operator!=(Decimal& d1, Decimal& d2);
		friend bool operator>(Decimal& d1, Decimal& d2);
		friend bool operator<(Decimal& d1, Decimal& d2);
		friend bool operator<=(Decimal& d1, Decimal& d2);
		friend bool operator>=(Decimal& d1, Decimal& d2);

		// increment operators: += -= *= /= %=  ++ --
		Decimal& operator++();
		Decimal& operator--();
		Decimal& operator+=(Decimal d);
		Decimal& operator-=(Decimal d);
		Decimal& operator*=(Decimal d);
		Decimal& operator/=(Decimal d);
		Decimal& operator%=(Decimal d);

		// in/out operators. << >>
		friend std::ostream& operator<< (std::ostream& out, const Decimal& decimal);
		friend std::istream& operator>> (std::istream& in, Decimal& decimal);
	};

}