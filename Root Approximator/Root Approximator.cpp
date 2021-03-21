#include <iostream>
#include <math.h>
#include <cmath>
#include <string>
#include <iomanip>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <sstream>

#include "exprtk.hpp"

using namespace std;

const int MAX_ITS = 20;

// Global string to hold the user-defined function
string equation;

template<typename T>
struct myFunc : public exprtk::ifunction<T>
{
	myFunc() : exprtk::ifunction<T>(2)
	{
		exprtk::disable_has_side_effects(*this);
	}

	inline T operator()(const T& v1, const T& v2)
	{
		return T(1) + (v1 * v2) / T(3);
	}
};

template<typename T>
T custom_function(T num)
{
	typedef exprtk::symbol_table<T> symbol_table_t;
	typedef exprtk::expression<T>	expression_t;
	typedef exprtk::parser<T>		parser_t;

	std::string expression_string = equation;

	T x = num;

	myFunc<T> mf;

	symbol_table_t symbol_table;
	symbol_table.add_variable("x", x);
	symbol_table.add_function("myFunc", mf);
	symbol_table.add_constants();

	expression_t expression;
	expression.register_symbol_table(symbol_table);

	parser_t parser;
	parser.compile(expression_string, expression);

	T result = expression.value();
	
	return result;
}

// This evaluates the given function's derivative of x
float funcDeriv(float x)
{
	// Change the equation here as desired
	return -(4 / x) + 1;
}

float convertError(string err)
{
	cout << fixed;

	float realError = 0;
	string temp;
	float exp;

	// Extract exponent from string, then convert it to int
	size_t pos = err.find('^') + 1;
	temp = err.substr(pos);
	exp = stof(temp);

	// Convert err to real float (for example: 10^-6 -> .000001)
	realError = static_cast<float>(1 * pow(10, exp));

	cout << "Real Error: " << realError << endl << endl;

	return realError;
}

float newtonMethod(float x)
{
	return x - (custom_function(x) / funcDeriv(x));
}

float secantMethod(float x, float prevX, float fOfX, float fOfPrevX)
{
	return x - (fOfX * ((x - prevX) / (fOfX - fOfPrevX)));
}

void newtonApproximation(float midpt, float err)
{
	int count = 1;
	float newX;

	cout << "\ni\tx\t\tf(x)\t\tf'(x)" << endl;
	cout << "---\t---\t\t---\t\t---" << endl << endl;

	cout << 0 << "\t" << midpt << "\t" << custom_function(midpt) << "\t" << funcDeriv(midpt) << endl;

	for (int i = 0; i < MAX_ITS; i++)
	{
		// Calculate next x value
		newX = newtonMethod(midpt);
		cout << count << "\t" << newX << "\t" << custom_function(newX) << "\t" << funcDeriv(newX) << endl;

		// Check for completion (newX - midpt < err)
		if (abs(newX - midpt) < err)
		{
			cout << "Approximation: " << newX << endl << endl;
			return;
		}

		// Get the next x value
		midpt = newX;

		count++;
	}

	cout << "Error: Newton Approximation function exceeded max iterations." << endl;
	exit(0);
}

void secantApproximation(float x1, float x2, float err)
{
	int count = 2;
	float newX;

	cout << "\ni\tx\t\tf(x)" << endl << endl;

	cout << 0 << "\t" << x1 << "\t" << custom_function(x1) << endl;
	cout << 1 << "\t" << x2 << "\t" << custom_function(x2) << endl;

	for (int i = 0; i < MAX_ITS; i++)
	{
		// Calculate the next x value
		newX = secantMethod(x2, x1, custom_function(x2), custom_function(x1));
		cout << count << "\t" << newX << "\t" << custom_function(newX) << endl;

		//Base case, check if within error tolerance
		if (abs(newX - x2) < err)
		{
			cout << "Approximation: " << newX << endl << endl;
			return;
		}

		// Perform simple swap to get next values to be calculated
		x1 = x2;
		x2 = newX;

		count++;
	}

	cout << "Error: Secant Approximation function exceeded max iterations" << endl;
	exit(0);
}

int main()
{
	float x1, x2, realErr, midpt;
	int choice;
	char uIn;
	string err;

	cout << setprecision(7);


	while (true)
	{
		cout << "Use previous/pre-defined function? (Y/N)" << endl;
		cin >> uIn;

		if (uIn == 'n' || uIn == 'N')
		{
			cin.ignore();

			cout << "Input function (Ensure all terms are separated by a whitespace): ";
			getline(cin, equation);
		}


		cout << "\n\nChoose method (1 = Newton's Method, 2 = Secant Method, 3 = Exit): ";
		cin >> choice;

		switch (choice)
		{
			case 1:
			{
				cout << "x1: ";
				cin >> x1;

				cout << "\nx2: ";
				cin >> x2;

				cout << "\nError Tolerance: Input as '10^x', where x = exponent (for example: '10^-6'): ";
				cin >> err;

				midpt = (x1 + x2) / 2;

				realErr = convertError(err);

				newtonApproximation(midpt, realErr);

				system("pause");

				return 0;
			}

			case 2:
			{
				cout << "x1: ";
				cin >> x1;
	
				cout << "\nx2: ";
				cin >> x2;

				cout << "\nError Tolerance: Input as '10^x', where x = exponent (for example: '10^-6'): ";
				cin >> err;

				realErr = convertError(err);

				secantApproximation(x1, x2, realErr);

				system("pause");

				return 0;
			}

			case 3:
			{
				cout << "Goodbye!" << endl;
				return 0;
			}

			default:
			{
				cout << "Error: Invalid choice. Choose again.\n\n";
			}
		}
	}
}

