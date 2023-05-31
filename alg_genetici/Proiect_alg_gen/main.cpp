#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

ifstream f("date.in");
ofstream g("date.out");

int N;
int A, B;
int coefA, coefB, coefC;
int precision;
double p_crossover;
double p_mutation;
int nr_iterations;

int binaryStringLength;
double discreteStepValue;

struct Chromosone {
    string binaryRepresentation;
    double x;
    double fValue; // fitness-ul
    double selectionProbability;
    int index;
};

vector < Chromosone > population;
vector < double > selectionProbabilityInterval;
vector < Chromosone > selectedIndividuals;
vector < Chromosone > individualsWhichWillBeRecombined;
vector < Chromosone > firstThreeBestIndividuals;

void readData() {
    f >> N;
    f >> A >> B;
    f >> coefA >> coefB >> coefC;
    f >> precision;
    f >> p_crossover;
    f >> p_mutation;
    f >> nr_iterations;
}

int binaryToDecimal(string bString) {
    int decimal = 0;
    for (int i = 0; i < bString.size(); ++i) {
        decimal += (bString[i] - '0') * pow(2, bString.size() - i - 1);
    }

    return decimal;

}

string decimalToBinary(int decimal) {
    string s = "";
    while (decimal > 0) {
        s = (decimal % 2 == 0 ? "0" : "1") + s;
        decimal /= 2;
    }

    return s;
}

void generateInitialPopulation() {
    srand(time(nullptr));

    for (int i = 0; i < N; ++i) {
        Chromosone individual;

        // generate a random number in the interval [A, B] so we have a diverse population
        individual.x = A + (double) rand() / RAND_MAX * (B - A);

        double leftBound = A;
        double rightBound = A + discreteStepValue;
        int count = 0;

        while (individual.x > rightBound) { // algoritmul de codificare
            leftBound += discreteStepValue;
            rightBound += discreteStepValue;
            ++count;
        }

        string decimalToBinaryString = decimalToBinary(count);
        int decimalToBinaryStringLength =  decimalToBinaryString.length();
        for (int _i = 0; _i < binaryStringLength - decimalToBinaryStringLength; ++_i) {
            decimalToBinaryString.insert(0, "0");
        }

        individual.binaryRepresentation = decimalToBinaryString;
        individual.fValue = coefA * individual.x * individual.x + coefB * individual.x + coefC;

        population.push_back(individual);
    }
}

void writeInitialPopulation() {
    g << "Initial population:" << '\n';
    for (int i = 0; i < N; ++i) {
        g << "    " << i << ". ";
        g << population[i].binaryRepresentation << " x = ";
        g << fixed << setprecision(precision) << population[i].x << " f = "; // fixed = sets the floatin point format to fixed-point notation
        g << fixed << setprecision(20) << population[i].fValue << '\n';
    }
}

void calculateDiscreteStepValue() {
    discreteStepValue = (double) (B - A) / pow(2, binaryStringLength);
    g << "Discrete step value: " << discreteStepValue << '\n';
}

void calculateBinaryStringLength() {
    binaryStringLength = ceil(log2((B - A) * pow(10, precision)));
    g << "Binary string length: " << binaryStringLength << '\n';
}

void newLine() {
    g << '\n';
}

void calculateSelectionProbability() {
    double sum = 0;
    for (int i = 0; i < N; ++i) {
        sum += population[i].fValue;
    }

    for (int i = 0; i < N; ++i) {
        population[i].selectionProbability = population[i].fValue / sum;
    }
}

void writeSelectionProbability() {
    g << "Selection probabilities:" << '\n';
    for (int i = 0; i < N; ++i) {
        g << "    " << i << ". ";
        g << fixed << setprecision(15) << population[i].selectionProbability << '\n';
    }
}

void calculateSelectionProbabilityIntervals() { // transform the probability to an interval (direct proportional)
    double sum = 0;
    for (int i = 0; i < N; ++i) {
        sum += population[i].selectionProbability;
        selectionProbabilityInterval.push_back(sum);
    }

    selectionProbabilityInterval.push_back(sum);
}

void writeSelectionProbabilityIntervals() {
    g << "Selection probability intervals:" << '\n';
    for (int i = 0; i < N; ++i) {
        g << "    " << i << ". ";
        g << fixed << setprecision(15) << selectionProbabilityInterval[i] << '\n';
    }
}

void calculateSelectedIndividual(bool showDebugInfo = false) {
    if (showDebugInfo) {
        g << "Random uniform numbers and selected individuals:" << '\n';
    }

    selectedIndividuals.resize(3);
    selectedIndividuals[0] = population[0];
    selectedIndividuals[1] = population[1];
    selectedIndividuals[2] = population[2];
    for (int i = 2; i < N; ++i) {
        double randomNumber = (double) rand() / RAND_MAX;
        if (showDebugInfo) {
            g << "    " << i << ". u = ";
            g << fixed << setprecision(15) << randomNumber << " ";
        }

        // search for the first interval that contains the random number using binary search
        int left = 0;
        int right = N - 1;
        int middle = (left + right) / 2;
        int selectedIndividualIndex = -1;
        while (left <= right) {
            if (randomNumber < selectionProbabilityInterval[middle]) {
                right = middle - 1;
            } else {
                left = middle + 1;
            }

            middle = (left + right) / 2;
        }

        selectedIndividualIndex = left;
        selectedIndividuals.push_back(population[selectedIndividualIndex]);

        if (showDebugInfo) {
            g << "-> Selected individual = " << selectedIndividualIndex << " | Interval number = " << selectedIndividualIndex + 1 << " | Interval = [" << selectionProbabilityInterval[selectedIndividualIndex - 1] << ", " << selectionProbabilityInterval[selectedIndividualIndex] << ")" << '\n';
        }
    }
}

void writeSelectedIndividuals() {
    g << "Selected individuals:" << '\n';
    for (int i = 0; i < N; ++i) {
        g << "    " << i << ". ";
        g << selectedIndividuals[i].binaryRepresentation << " x = ";
        g << fixed << setprecision(precision) << selectedIndividuals[i].x << " f = ";
        g << fixed << setprecision(20) << selectedIndividuals[i].fValue << '\n';
    }
}

void calculateAndWriteIndividualsWhichWillBeRecombined(bool showDebugInfo = false) {
    if (showDebugInfo) {
        g << "Selected individuals for recombination:" << '\n';
    }
    for (int i = 0; i < N; ++i) {
        double randomNumber = (double) rand() / RAND_MAX;

        if (showDebugInfo) {
            g << "    " << i << ". ";
            g << selectedIndividuals[i].binaryRepresentation << " x = ";
            g << fixed << setprecision(precision) << selectedIndividuals[i].x << " f = ";
            g << fixed << setprecision(20) << selectedIndividuals[i].fValue << " u = ";
            g << fixed << setprecision(15) << randomNumber;
        }
        if (randomNumber < p_crossover && i > 2) { //if it's in the first 25%, in the example p_cross = 25%
            if (showDebugInfo) {
                g << " < " << p_crossover << " -> SELECTED" << '\n';
            }
            selectedIndividuals[i].index = i;
            individualsWhichWillBeRecombined.push_back(selectedIndividuals[i]);
        } else {
            if (showDebugInfo) {
                g << '\n';
            }
        }
    }
}

void calculateAndWriteRecombinationResults(bool showDebugInfo = false) { // algoritmul de incrucisare
    if (showDebugInfo) {
        g << "Recombination process:" << '\n';
    }

    int individualsWhichWillBeRecombinedSize = individualsWhichWillBeRecombined.size();
    for (int i = 0; i < individualsWhichWillBeRecombinedSize; i += 2) {
        if (i + 1 >= individualsWhichWillBeRecombinedSize) {
            break;
        }
        int randomIndex = rand() % binaryStringLength;

        if (showDebugInfo) {
            g << "    Recombine individual " << individualsWhichWillBeRecombined[i].index << " with individual " << individualsWhichWillBeRecombined[i + 1].index << " at position " << randomIndex << '\n';
            g << "    Before recombination: " << individualsWhichWillBeRecombined[i].binaryRepresentation << " " << individualsWhichWillBeRecombined[i + 1].binaryRepresentation << '\n';
        }

        string firstIndividualBinaryRepresentation = individualsWhichWillBeRecombined[i].binaryRepresentation;
        string secondIndividualBinaryRepresentation = individualsWhichWillBeRecombined[i + 1].binaryRepresentation;

        for (int j = 0; j < randomIndex; ++j) { // for-ul de la incrucisare
            swap(firstIndividualBinaryRepresentation[j], secondIndividualBinaryRepresentation[j]);
        }

        selectedIndividuals[individualsWhichWillBeRecombined[i].index].binaryRepresentation = firstIndividualBinaryRepresentation;
        selectedIndividuals[individualsWhichWillBeRecombined[i + 1].index].binaryRepresentation = secondIndividualBinaryRepresentation;

        int decimal = binaryToDecimal(firstIndividualBinaryRepresentation);

        double leftBound = A;
        double rightBound = A + discreteStepValue;
        int count = 0;

        while (count < decimal) {
            leftBound += discreteStepValue;
            rightBound += discreteStepValue;
            ++count;
        }

        selectedIndividuals[individualsWhichWillBeRecombined[i].index].x = leftBound;

        decimal = binaryToDecimal(secondIndividualBinaryRepresentation);
        leftBound = A;
        rightBound = A + discreteStepValue;
        count = 0;

        while (count < decimal) {
            leftBound += discreteStepValue;
            rightBound += discreteStepValue;
            ++count;
        }

        selectedIndividuals[individualsWhichWillBeRecombined[i + 1].index].x = leftBound;

        selectedIndividuals[individualsWhichWillBeRecombined[i].index].fValue = coefA * pow(selectedIndividuals[individualsWhichWillBeRecombined[i].index].x, 2) + coefB * selectedIndividuals[individualsWhichWillBeRecombined[i].index].x + coefC;
        selectedIndividuals[individualsWhichWillBeRecombined[i + 1].index].fValue = coefA * pow(selectedIndividuals[individualsWhichWillBeRecombined[i + 1].index].x, 2) + coefB * selectedIndividuals[individualsWhichWillBeRecombined[i + 1].index].x + coefC;

        if (showDebugInfo) {
            g << "    After recombination:  " << firstIndividualBinaryRepresentation << " "
              << secondIndividualBinaryRepresentation << '\n';
            newLine();
        }
    }
}

void writePopulationAfterRecombination() {
    g << "Population after recombination:" << '\n';
    for (int i = 0; i < N; ++i) {
        g << "    " << i << ". ";
        g << selectedIndividuals[i].binaryRepresentation << " x = ";
        g << fixed << setprecision(precision) << selectedIndividuals[i].x << " f = ";
        g << fixed << setprecision(20) << selectedIndividuals[i].fValue << '\n';
    }
}

void calculateAndWriteMutations(bool showDebugInfo = false) {
    if (showDebugInfo) {
        g << "Mutation process:" << '\n';
    }

    for (int i = 0; i < N; ++i) {
        double randomNumber = (double) rand() / RAND_MAX;

        if (showDebugInfo) {
            g << "    " << i << ". ";
            g << selectedIndividuals[i].binaryRepresentation << " x = ";
            g << fixed << setprecision(precision) << selectedIndividuals[i].x << " f = ";
            g << fixed << setprecision(20) << selectedIndividuals[i].fValue << " u = ";
            g << fixed << setprecision(15) << randomNumber;
        }

        if (randomNumber < p_mutation && i > 2) {
            if (showDebugInfo) {
                g << " < " << p_mutation << " -> MUTATED" << '\n';
            }

            int randomIndex = rand() % binaryStringLength;
            if (selectedIndividuals[i].binaryRepresentation[randomIndex] == '0') {
                selectedIndividuals[i].binaryRepresentation[randomIndex] = '1';
            } else {
                selectedIndividuals[i].binaryRepresentation[randomIndex] = '0';
            }

            int decimal = binaryToDecimal(selectedIndividuals[i].binaryRepresentation);
            double leftBound = A;
            double rightBound = A + discreteStepValue;
            int count = 0;

            while (count < decimal) {
                leftBound += discreteStepValue;
                rightBound += discreteStepValue;
                ++count;
            }

            selectedIndividuals[i].x = leftBound;
            selectedIndividuals[i].fValue = coefA * pow(selectedIndividuals[i].x, 2) + coefB * selectedIndividuals[i].x + coefC;
        } else {
            if (showDebugInfo) {
                g << '\n';
            }
        }
    }

    if (showDebugInfo) {
        newLine();
        g << "Population after mutation:" << '\n';

        for (int i = 0; i < N; ++i) {
            g << "    " << i << ". ";
            g << selectedIndividuals[i].binaryRepresentation << " x = ";
            g << fixed << setprecision(precision) << selectedIndividuals[i].x << " f = ";
            g << fixed << setprecision(20) << selectedIndividuals[i].fValue << '\n';
        }
    }
}

void calculateAndWriteMaxAndMeanFitness(int generation = 1) {
    double maxFitness = selectedIndividuals[0].fValue;
    double meanFitness = 0;

    for (int i = 0; i < N; ++i) {
        if (selectedIndividuals[i].fValue > maxFitness) {
            maxFitness = selectedIndividuals[i].fValue;
        }

        meanFitness += selectedIndividuals[i].fValue;
    }

    vector < Chromosone > sortedSelectedIndividuals = selectedIndividuals;
    sort(sortedSelectedIndividuals.begin(), sortedSelectedIndividuals.end(), [](const Chromosone & a, const Chromosone & b) {
        return a.fValue > b.fValue;
    });

    firstThreeBestIndividuals[0] = sortedSelectedIndividuals[0];
    firstThreeBestIndividuals[1] = sortedSelectedIndividuals[1];
    firstThreeBestIndividuals[2] = sortedSelectedIndividuals[2];
    for (auto const& item: firstThreeBestIndividuals) {
        cout << item.fValue << " ";
    }
    cout << '\n';

    meanFitness /= N;

    g << "Generation " << generation << '\n';
    g << "Max fitness: " << fixed << setprecision(20) << maxFitness << '\n';
    g << "Mean fitness: " << fixed << setprecision(20) << meanFitness << '\n';
    newLine();
}

int main() {
    firstThreeBestIndividuals.resize(3);
    readData();
    calculateBinaryStringLength();
    calculateDiscreteStepValue();
    newLine();
    generateInitialPopulation();
    writeInitialPopulation();
    newLine();
    calculateSelectionProbability();
    writeSelectionProbability();
    newLine();
    calculateSelectionProbabilityIntervals();
    writeSelectionProbabilityIntervals();
    newLine();
    calculateSelectedIndividual(true);
    newLine();
    writeSelectedIndividuals();
    newLine();
    calculateAndWriteIndividualsWhichWillBeRecombined(true);
    newLine();
    calculateAndWriteRecombinationResults(true);
    newLine();
    writePopulationAfterRecombination();
    newLine();
    calculateAndWriteMutations(true);
    newLine();
    calculateAndWriteMaxAndMeanFitness();

    for (int i = 1; i < nr_iterations; ++i) {
        population = selectedIndividuals;
        population[0] = firstThreeBestIndividuals[0];
        population[1] = firstThreeBestIndividuals[1];
        population[2] = firstThreeBestIndividuals[2];
        calculateSelectionProbability();
        calculateSelectionProbabilityIntervals();
        calculateSelectedIndividual();
        calculateAndWriteIndividualsWhichWillBeRecombined();
        calculateAndWriteRecombinationResults();
        calculateAndWriteMutations();
        calculateAndWriteMaxAndMeanFitness(i + 1);
    }
}