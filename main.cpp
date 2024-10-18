#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

struct Pair
{
    int x1, y1, x2, y2;
};

// Function to print all pairs
void printPairs(const Pair *pairs, int size)
{
    cout << "\nYou entered the following pairs: " << endl;
    for (unsigned int i = 0; i < size; i++)
    {
        cout << "Pair " << i + 1 << ": "
             << pairs[i].x1 << " " << pairs[i].y1 << " "
             << pairs[i].x2 << " " << pairs[i].y2 << "\n";
    }
}

// Function to detect pairs where there is a sign change between x1 and x2
int *xSignChange(const Pair *pairs, int size, int &count)
{
    // Dynamically allocate memory for storing the indices of pairs with sign changes
    int *xs = new int[size]; // At most all pairs could have a sign change
    count = 0;               // Initialize count to track how many pairs have sign changes

    for (int i = 0; i < size; ++i)
    {
        // Check if x1 and x2 have different signs
        if ((pairs[i].x1 < 0 && pairs[i].x2 >= 0) || (pairs[i].x1 >= 0 && pairs[i].x2 < 0))
        {
            xs[count] = i; // Store the index of the pair with a sign change
            count++;
        }
    }

    return xs;
}

// Function to calculate the slope (m) and y-intercept (b) from valid pairs
double calculateYIntercept(const Pair &p, double &m)
{
    // Calculate slope (m)
    if (p.x2 - p.x1 != 0)
    {
        m = static_cast<double>(p.y2 - p.y1) / (p.x2 - p.x1);
    }
    else
    {
        std::cerr << "Error: Division by zero when calculating slope." << std::endl;
        m = 0; // Just to avoid undefined behavior, but logically this shouldn't happen.
    }

    // Calculate y-intercept (b) using the formula: b = y1 - m * x1
    double b = p.y1 - m * p.x1;
    return b;
}

// Function to sort the dynamic array of y-intercepts using simple selection sort
void sortYIntercepts(double *yIntercepts, int count)
{
    for (int i = 0; i < count - 1; ++i)
    {
        int minIdx = i;
        for (int j = i + 1; j < count; ++j)
        {
            if (yIntercepts[j] < yIntercepts[minIdx])
            {
                minIdx = j;
            }
        }
        // Swap the minimum element with the first element
        double temp = yIntercepts[i];
        yIntercepts[i] = yIntercepts[minIdx];
        yIntercepts[minIdx] = temp;
    }
}

int main(int argc, char **argv)
{
    int n;
    cout << "Enter number of pairs: ";
    cin >> n;
    cin.ignore(); // cin leaves the new line character if we entered " -1 -1 -1 -1 ENTER" the enter character is left in the buffer

    // Dynamic allocation for the struct pair based on size n
    Pair *pairs = new Pair[n];

    char input[50]; // buffer for input streams, 50 is large but I'm being safe

    for (unsigned int i = 0; i < n; i++)
    {
        cout << "Enter a pair of coordinates: (x1 y1 x2 y2) ";
        cin.getline(input, 50);

        // Tokenize the input and convert strings to integers using atoi
        char *token = strtok(input, " ");
        pairs[i].x1 = atoi(token);

        token = strtok(NULL, " ");
        pairs[i].y1 = atoi(token);

        token = strtok(NULL, " ");
        pairs[i].x2 = atoi(token);

        token = strtok(NULL, " ");
        pairs[i].y2 = atoi(token);
    }

    // Detect a sign change between pairs of x values
    int count = 0;
    int *signChangedIndices = xSignChange(pairs, n, count);

    // If no sign changes found, return -1.0
    if (count == 0)
    {
        cout << "-1.0\n";
    }
    else
    {
        // Dynamically allocate an array to store y-intercepts
        double *yIntercepts = new double[count];

        // Loop through each pair with a sign change and calculate the y-intercept
        for (int i = 0; i < count; ++i)
        {
            int idx = signChangedIndices[i];
            double slope = 0;
            yIntercepts[i] = calculateYIntercept(pairs[idx], slope);
        }

        // Sort the y-intercepts
        sortYIntercepts(yIntercepts, count);

        // Output the smallest y-intercept (first element in the sorted array)
        cout << "\nSmallest y-intercept: " << yIntercepts[0] << "\n";

        // Clean up the dynamically allocated y-intercepts array
        delete[] yIntercepts;
    }

    // Clean up dynamically allocated memory
    delete[] pairs;
    delete[] signChangedIndices;

    return 0;
}