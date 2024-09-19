#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Structure to represent a term in the polynomial
typedef struct
{
    int coefficient; // Coefficient (e.g., 5 in 5x^3)
    int power;       // Power of x (e.g., 3 in 5x^3)
} Term;

// Structure to represent a polynomial function
typedef struct
{
    Term terms[10]; // Maximum number of terms in the polynomial (can be adjusted)
    int termCount;  // Number of terms in the polynomial
} Function;

// Function prototypes
void GetFunctionFromUser(Function *foo);
void PrintFunction(Function *foo);
void Derivative(Function *foo, Function *deriv);
void FindExtrema(Function *deriv);
void FindCuts(Function *foo);
void DetermineIncreasingDecreasing(Function *deriv, int range_min, int range_max);
void DeterminePositiveNegative(Function *foo, int range_min, int range_max);
int EvaluatePolynomial(Function *foo, int x);

// Main function
int main()
{
    Function f;
    Function df; // Derivative

    GetFunctionFromUser(&f);

    printf("\nOriginal function:\n");
    PrintFunction(&f);

    // Compute the derivative
    Derivative(&f, &df);
    printf("\nDerivative:\n");
    PrintFunction(&df);

    // Output additional details
    printf("\nDetails:\n");
    printf("+-----------------------------+--------------------------+\n");
    printf("| %-27s | %-24s |\n", "Detail", "Value");
    printf("+-----------------------------+--------------------------+\n");

    // Find extrema points
    printf("| %-27s | %-24s |\n", "Extrema", "Check output");
    FindExtrema(&df);

    // Find intersections with X and Y axes
    printf("| %-27s | %-24s |\n", "Y-axis intersection", "Check output");
    FindCuts(&f);

    printf("| %-27s | %-24s |\n", "X-axis intersections", "Check output");
    FindXIntersections(&f);

    // Determine where the function is increasing or decreasing
    printf("| %-27s | %-24s |\n", "Increasing/Decreasing", "Check output");
    int range_min = -10, range_max = 10;
    DetermineIncreasingDecreasing(&df, range_min, range_max);

    // Determine where the function is positive or negative
    printf("| %-27s | %-24s |\n", "Positive/Negative", "Check output");
    DeterminePositiveNegative(&f, range_min, range_max);

    printf("+-----------------------------+--------------------------+\n");

    return 0;
}

// Function to get the polynomial from the user
void GetFunctionFromUser(Function *foo)
{
    printf("Enter the number of terms in the polynomial (including constant term): ");
    scanf("%d", &foo->termCount);

    for (int i = 0; i < foo->termCount; i++)
    {
        printf("Enter term %d (e.g., 5 3 for 5x^3 or 7 0 for 7): ", i + 1);
        int coefficient;
        int power;

        scanf("%d %d", &coefficient, &power);

        foo->terms[i].coefficient = coefficient;
        foo->terms[i].power = power;
    }
}

// Function to print a polynomial
void PrintFunction(Function *foo)
{
    int first = 1; // Flag to handle the first term

    for (int i = 0; i < foo->termCount; i++)
    {
        if (foo->terms[i].coefficient == 0)
            continue; // Skip zero coefficients

        // Handle the sign of the term
        if (first)
        {
            if (foo->terms[i].coefficient < 0)
                printf("-");
            first = 0;
        }
        else
        {
            if (foo->terms[i].coefficient > 0)
                printf(" + ");
            else if (foo->terms[i].coefficient < 0)
                printf(" - ");
        }

        // Handle the coefficient
        if (abs(foo->terms[i].coefficient) != 1 || foo->terms[i].power == 0)
            printf("%d", abs(foo->terms[i].coefficient));

        // Handle the power of x
        if (foo->terms[i].power > 0)
        {
            printf("x");
            if (foo->terms[i].power > 1)
                printf("^%d", foo->terms[i].power);
        }
    }

    if (first) // If all coefficients were zero
        printf("0");

    printf("\n");
}

// Function to compute the derivative of a polynomial
void Derivative(Function *foo, Function *deriv)
{
    deriv->termCount = 0;
    for (int i = 0; i < foo->termCount; i++)
    {
        // Ignore terms with power 0, as their derivative is 0
        if (foo->terms[i].power != 0)
        {
            deriv->terms[deriv->termCount].coefficient = foo->terms[i].coefficient * foo->terms[i].power;
            deriv->terms[deriv->termCount].power = foo->terms[i].power - 1;
            deriv->termCount++;
        }
    }
}

// Function to find extrema points (local maxima and minima)
void FindExtrema(Function *deriv)
{
    int min_x = -10;
    int max_x = 10;
    int prev_value = EvaluatePolynomial(deriv, min_x);

    for (int x = min_x + 1; x <= max_x; x++)
    {
        int derivative_value = EvaluatePolynomial(deriv, x);

        if (prev_value == 0 && derivative_value != 0)
        {
            printf("| %-27s | x = %-20d with value = %-20d |\n", "Local Min/Max", x, EvaluatePolynomial(deriv, x));
        }

        prev_value = derivative_value;
    }
}

// Function to find intersections with X and Y axes
void FindCuts(Function *foo)
{
    // Intersection with Y-axis occurs at x = 0
    int y_cut = EvaluatePolynomial(foo, 0);
    printf("| %-27s | (0, %-20d) |\n", "Y-axis intersection", y_cut);
}

// Function to find X-axis intersections
void FindXIntersections(Function *foo)
{
    printf("| %-27s | %-24s |\n", "X-axis intersections", "Check output");

    int min_x = -10;
    int max_x = 10;

    for (int x = min_x; x <= max_x; x++)
    {
        int value = EvaluatePolynomial(foo, x);
        if (value == 0)
        {
            printf("| %-27s | x = %-20d |\n", "Intersection", x);
        }
    }
}

// Function to evaluate a polynomial at a given x value
int EvaluatePolynomial(Function *foo, int x)
{
    int result = 0;
    for (int i = 0; i < foo->termCount; i++)
    {
        result += foo->terms[i].coefficient * pow(x, foo->terms[i].power);
    }
    return result;
}

// Function to determine where the function is increasing or decreasing
void DetermineIncreasingDecreasing(Function *deriv, int range_min, int range_max)
{
    int prev_sign = 0; // 0 = no sign yet, 1 = increasing, -1 = decreasing
    int start = range_min;

    for (int x = range_min; x <= range_max; x++)
    {
        int derivative_value = EvaluatePolynomial(deriv, x);
        int current_sign = (derivative_value > 0) ? 1 : (derivative_value < 0) ? -1 : 0;

        if (prev_sign == 0) // First value to determine direction
        {
            prev_sign = current_sign;
        }
        else if (current_sign != prev_sign) // Direction changed
        {
            if (prev_sign > 0)
                printf("| %-27s | x = %-20d to x = %-20d |\n", "Increasing", start, x - 1);
            else if (prev_sign < 0)
                printf("| %-27s | x = %-20d to x = %-20d |\n", "Decreasing", start, x - 1);

            start = x;
            prev_sign = current_sign;
        }
    }

    // Print the last range
    if (prev_sign > 0)
        printf("| %-27s | x = %-20d to x = %-20d |\n", "Increasing", start, range_max);
    else if (prev_sign < 0)
        printf("| %-27s | x = %-20d to x = %-20d |\n", "Decreasing", start, range_max);
}

// Function to determine where the function is positive or negative
void DeterminePositiveNegative(Function *foo, int range_min, int range_max)
{
    int prev_sign = 0; // 0 = no sign yet, 1 = positive, -1 = negative
    int start = range_min;

    for (int x = range_min; x <= range_max; x++)
    {
        int function_value = EvaluatePolynomial(foo, x);
        int current_sign = (function_value > 0) ? 1 : (function_value < 0) ? -1 : 0;

        if (prev_sign == 0) // First value to determine sign
        {
            prev_sign = current_sign;
        }
        else if (current_sign != prev_sign) // Sign changed
        {
            if (prev_sign > 0)
                printf("| %-27s | x = %-20d to x = %-20d |\n", "Positive", start, x - 1);
            else if (prev_sign < 0)
                printf("| %-27s | x = %-20d to x = %-20d |\n", "Negative", start, x - 1);

            start = x;
            prev_sign = current_sign;
        }
    }

    // Print the last range
    if (prev_sign > 0)
        printf("| %-27s | x = %-20d to x = %-20d |\n", "Positive", start, range_max);
    else if (prev_sign < 0)
        printf("| %-27s | x = %-20d to x = %-20d |\n", "Negative", start, range_max);
}
