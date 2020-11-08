#include "Decimal-class.h"

namespace ara {

	Decimal::Decimal() {
		// default blank constructor
	}

	Decimal::Decimal(int whole, long long decimal) : m_Whole{ whole } {
		// the full accurate int/long long constructor
		setDecimal(decimal);
	}

	Decimal::Decimal(int whole) : m_Whole{ whole } {
		// the only int constructor that can be used
		setDecimal(0);
	}

	Decimal::Decimal(float d) {
		// cover all floating point types.
		loadDecimal((long double)d);
	}

	Decimal::Decimal(double d) {
		// cover all floating point types.
		loadDecimal((long double)d);
	}

	Decimal::Decimal(long double d) {
		// cover all floating point types.
		loadDecimal(d);
	}

	void Decimal::loadDecimal(long double d) {
		// load for all float types. converts the floating point into closest available number. 
		// despite best efforts, this may still give inaccurate values.
		m_Whole = (int)floor(d);
		m_Decimal = (long long)round((d - m_Whole) * power());
		checkNegative();
	}

	void Decimal::loadDecimal(int whole, long long decimal) {
		// load with accurate int numbers, without worrying about program breaking down number for me.
		m_Whole = whole;
		setDecimal(decimal);
		checkNegative();
	}

	Decimal::~Decimal() {
		// default destructor, will do nothing.
	}

	void Decimal::checkNegative() {
		// if either value is negative, then assume that both are meant to be. ensure both are positive;
		if (m_Whole < 0 || m_Decimal < 0)
			m_Negative = -1;
		else
			m_Negative = 1;

		m_Whole = abs(m_Whole);
		m_Decimal = llabs(m_Decimal);
	}

	long double Decimal::power() {
		// return the power value of max decimal places
		return pow(10.0, m_MaxDecimalPlaces);
	}

	void Decimal::setMaxDecPlaces(int maximum) {
		// set the maximum visible places. This will not define the places for the variable, 
		// since this is a static call. All future definitions will check that. 
		m_MaxDecimalPlaces = maximum;
	}

	void Decimal::setWhole(int whole) {
		// set the whole number only, leaving decimal intact. 
		m_Whole = whole;
		checkNegative();
	}

	void Decimal::loadDecimal(long long decimal) {
		// separate the 2 values for the whole and decimal.
		m_Whole = (int)floor(decimal / power());
		m_Decimal = (long long)(decimal - floor(getWhole() * power()));

		checkNegative();
	}

	void Decimal::setDecimal(long long decimal) {
		// set the decimal number only, then check for remaining spaces required. 
		m_Decimal = decimal;

		// we need to convert mto allow the maximum decimals places
		long long decCalc = m_Decimal;
		int places = 1;
		while (decCalc > 10) {
			// check and count total places, we'll add zeros afterwards.
			decCalc /= 10;
			++places;
		}
		std::cout << m_MaxDecimalPlaces << ' ' << places << '\n';
		if (places > m_MaxDecimalPlaces) {
			// truncate the number, this is too long

			m_Decimal = (long long)round(m_Decimal / pow(10.0, places - m_MaxDecimalPlaces));

		} else if (places < m_MaxDecimalPlaces) {
			// multiply in, there is too few places
			m_Decimal = (long long)round(m_Decimal * pow(10.0, m_MaxDecimalPlaces - places));

		} 
		checkNegative();
	}

	void Decimal::print() {
		// print the current number, inlcude the negative on whole.
		std::cout << (m_Whole * m_Negative) << "." << leadingZeros() << m_Decimal << '\n';
	}

	int Decimal::getMaxDecPlaces() {
		// get the mamximum decimal places and return. 
		return m_MaxDecimalPlaces;
	}

	int Decimal::getWhole() {
		// get only the absolute whole number. 
		return m_Whole;
	}

	long long Decimal::getDecimal() {
		// get only the decimal places.
		return m_Decimal;
	}

	std::string Decimal::leadingZeros() {
		// add the possible leading zeros that would be otherwise missed by the decimal system. 
		std::string ret{ "" };
		for (long long i{ (long long)power() / 10 }; i > m_Decimal; i /= 10) {
			// for each level of 10, add a 0 to the string. 
			ret += "0";
		}
		return ret;

	}

	long double Decimal::toLongDouble() {
		// degrade the decimal to a long double for use in other functions, if needed.
		return (long double)((m_Whole + (m_Decimal / power())) * m_Negative);
	}

	double Decimal::toDouble() {
		// degrade the decimal to a double for use in other functions if needed. 
		return (double)((m_Whole + (m_Decimal / power())) * m_Negative);
	}

	float Decimal::toFloat() {
		// degrade the decimal to a float for use in other functions if needed. 
		return (float)((m_Whole + (m_Decimal / power())) * m_Negative);
	}

	// operator overloads
	Decimal operator+(Decimal& d1, Decimal& d2) {  // TEST THIS
		Decimal d;

		// add the 2 values together
		long long fullsize = (long long)(d1.m_Whole * d.power() + d1.m_Decimal) * d1.m_Negative + (long long)(d2.m_Whole * d.power() + d2.m_Decimal) * d2.m_Negative;

		d.loadDecimal(fullsize);
		// return the decimal result.
		return d;
	}

	Decimal operator-(Decimal& d1, Decimal& d2) { // TEST THIS
		Decimal d;
		
		// d1 - d2 always. 
		long long fullsize = (long long)(d1.m_Whole * d.power() + d1.m_Decimal) * d1.m_Negative - (long long)(d2.m_Whole * d.power() + d2.m_Decimal) * d2.m_Negative;

		d.loadDecimal(fullsize);

		return d;
	}

	Decimal operator*(Decimal& d1, Decimal& d2) { // TEST THIS
		Decimal d;
		
		// values are mutiplied, therefore off by a factor of power()
		long long fullsize = (long long)((d1.m_Whole * d.power() + d1.m_Decimal) * d1.m_Negative) * (long long)((d2.m_Whole * d.power() + d2.m_Decimal) * d2.m_Negative);

		// reduce to normal size
		fullsize = (long long)round(fullsize / d.power());

		d.loadDecimal(fullsize);

		return d;
	}

	Decimal operator/(Decimal& d1, Decimal& d2) { // TEST THIS
		Decimal d;
		
		// values are mutiplied, therefore off by a factor of power() d1 / d2;
		long long fullsize = (long long)round(((d1.m_Whole * d.power() + d1.m_Decimal) * d1.m_Negative) / ((d2.m_Whole * d.power() + d2.m_Decimal) * d2.m_Negative) * d.power());

		d.loadDecimal(fullsize);

		return d;
	}

	Decimal operator%(Decimal& d1, Decimal& d2) { // TEST THIS
		Decimal d;
	
		// values are mutiplied, therefore off by a factor of power() d1 / d2;
		long long fullsize = (long long)((d1.m_Whole * d.power() + d1.m_Decimal) * d1.m_Negative) % (long long)((d2.m_Whole * d.power() + d2.m_Decimal) * d2.m_Negative);

		d.loadDecimal(fullsize);

		return d;
	}

	Decimal Decimal::operator-() {
		return Decimal(-m_Whole, m_Decimal);
	}

	bool operator==(Decimal& d1, Decimal& d2) {
		return (d1.m_Whole == d2.m_Whole && d1.m_Decimal == d2.m_Decimal && d1.m_Negative == d2.m_Negative);
	}

	bool operator!=(Decimal& d1, Decimal& d2) {
		return !(d1 == d2);
	}

	bool operator>(Decimal& d1, Decimal& d2) {
		return (long long)((d1.m_Whole * d1.power() + d1.m_Decimal) * d1.m_Negative) > (long long)((d2.m_Whole * d2.power() + d2.m_Decimal) * d2.m_Negative);
	}

	bool operator<(Decimal& d1, Decimal& d2) {
		return (long long)((d1.m_Whole * d1.power() + d1.m_Decimal) * d1.m_Negative) < (long long)((d2.m_Whole * d2.power() + d2.m_Decimal) * d2.m_Negative);
	}

	bool operator<=(Decimal& d1, Decimal& d2) {
		return !(d1 > d2);
	}

	bool operator>=(Decimal& d1, Decimal& d2) {
		return !(d1 < d2);
	}

	Decimal& Decimal::operator++() {
		++m_Whole;
		return *this;
	}

	Decimal& Decimal::operator--() {
		--m_Whole;
		return *this;
	}

	Decimal& Decimal::operator+=(Decimal d) {

		// add the 2 values together
		long long fullsize = (long long)(m_Whole * power() + m_Decimal) * m_Negative + (long long)(d.m_Whole * d.power() + d.m_Decimal) * d.m_Negative;

		loadDecimal(fullsize);

		return *this;
	}

	Decimal& Decimal::operator-=(Decimal d) {

		long long fullsize = (long long)(m_Whole * power() + m_Decimal) * m_Negative - (long long)(d.m_Whole * d.power() + d.m_Decimal) * d.m_Negative;

		loadDecimal(fullsize);

		return *this;
	}

	Decimal& Decimal::operator*=(Decimal d) {

		long long fullsize = (long long)(m_Whole * power() + m_Decimal) * m_Negative * (long long)(d.m_Whole * d.power() + d.m_Decimal) * d.m_Negative;

		// reduce to normal size
		fullsize = (long long)round(fullsize / d.power());

		loadDecimal(fullsize);

		return *this;
	}

	Decimal& Decimal::operator/=(Decimal d) {

		long long fullsize = (long long)round((m_Whole * power() + m_Decimal) * m_Negative / (d.m_Whole * d.power() + d.m_Decimal) * d.m_Negative * power());

		loadDecimal(fullsize);

		return *this;
	}

	Decimal& Decimal::operator%=(Decimal d) { // TEST THIS

		// values are mutiplied, therefore off by a factor of power() d1 / d2;
		long long fullsize = (long long)((m_Whole * power() + m_Decimal) * m_Negative) % (long long)((d.m_Whole * power() + d.m_Decimal) * d.m_Negative);

		loadDecimal(fullsize);

		return *this;
	}

	std::ostream& operator<<(std::ostream& out, const Decimal& decimal) {

		Decimal d = decimal;

		std::string s = d.leadingZeros();

		out << (decimal.m_Whole * decimal.m_Negative) << "." << s << decimal.m_Decimal;

		return out;
	}

	std::istream& operator>>(std::istream& in, Decimal& decimal) {

		in >> decimal.m_Whole;

		in.ignore(std::numeric_limits<std::streamsize>::max(), '.');

		in >> decimal.m_Decimal;

		decimal.checkNegative();

		return in;
	}
}