#ifndef ADVANCEDDB2022COURSEWORK1_H
#define ADVANCEDDB2022COURSEWORK1_H

#include <array>
#include <cstdlib>
#include <tuple>
#include <variant>
#include <vector>
#include <iostream>
// YOU MAY NOT ADD ANY OTHER INCLUDES!!!
using AttributeValue = std::variant<long, double, char const *>;
using Tuple = std::vector<AttributeValue>;
using Relation = std::vector<Tuple>;
using Column = std::vector<AttributeValue>;

/**
 * 0 islong, 1 is double, 2 is a c-string
 */
inline size_t getAttributeValueType(AttributeValue const &value) { return value.index(); }

inline long getLongValue(AttributeValue const &value) { return std::get<long>(value); }

inline double getdoubleValue(AttributeValue const &value) { return std::get<double>(value); }

inline char const *getStringValue(AttributeValue const &value) {
    return std::get<char const *>(value);
}

inline size_t getNumberOfValuesInTuple(Tuple const &t) { return t.size(); }

inline size_t getNumberOfTuplesInRelation(Relation const &t) { return t.size(); }

/**
 * DBMS shall implement the following query in the query function:
 *
 * select sum(large1.c * large2.c * small.c) from large1, large2, small where
 * large1.a = large2.a and large2.a = small.a and large1.b + large2.b + small.b
 * > 9;
 */
class DBMSImplementationForMarks { // you may edit anything inside this class
    // but nothing else
    Relation large1;
    Relation large2;
    Relation small;

    std::vector<Column> large1DSM;
    std::vector<Column> large2DSM;
    std::vector<Column> smallDSM;

public:
    void loadData(Relation const *large1,
                  Relation const *large2, // NOLINT(bugprone-easily-swappable-parameters)
                  Relation const *small)  // NOLINT(bugprone-easily-swappable-parameters)
    {

        std::cout << "hello";
        this->large1 = *large1; // you can copy the relation or just keep a pointer
        this->large2 = *large2;
        this->small = *small;

        // here is some example code for the exatraction of values feel free to
        // access the data any way you like (without changing the function
        // signature, of course)

        // You should add your implementation here...

        std::cout << "hello" << "\n";

        for (int j = 0; j < getNumberOfValuesInTuple(this->large1[0]); j++) {
            std::vector<AttributeValue> temp;
            large1DSM.push_back(temp);
        }

        for (int j = 0; j < getNumberOfValuesInTuple(this->large2[0]); j++) {
            std::vector<AttributeValue> temp;
            large2DSM.push_back(temp);
        }

        for (int j = 0; j < getNumberOfValuesInTuple(this->small[0]); j++) {
            std::vector<AttributeValue> temp;
            smallDSM.push_back(temp);
        }

        for (int i = 0; i < getNumberOfTuplesInRelation(this->large1); i++) {
            for (int j = 0; j < getNumberOfValuesInTuple(this->large1[i]); j++) {
                if (getAttributeValueType(this->large1[i][j]) == 0) {
                    large1DSM[j].push_back(getLongValue(this->large1[i][j]));
                } else if (getAttributeValueType(this->large1[i][j]) == 1) {
                    large1DSM[j].push_back(getdoubleValue(this->large1[i][j]));
                } else if (getAttributeValueType(this->large1[i][j]) == 2) {
                    large1DSM[j].push_back(getStringValue(this->large1[i][j]));
                } else {
                    // Null Pointer
                    large1DSM[j].push_back(this->large1[i][j]);
                }
            }
        }

        for (int i = 0; i < getNumberOfTuplesInRelation(this->large2); i++) {
            for (int j = 0; j < getNumberOfValuesInTuple(this->large2[i]); j++) {
                if (getAttributeValueType(this->large2[i][j]) == 0) {
                    large2DSM[j].push_back(getLongValue(this->large2[i][j]));
                } else if (getAttributeValueType(this->large2[i][j]) == 1) {
                    large2DSM[j].push_back(getdoubleValue(this->large2[i][j]));
                } else if (getAttributeValueType(this->large2[i][j]) == 2) {
                    large2DSM[j].push_back(getStringValue(this->large2[i][j]));
                } else {
                    // Null Pointer
                    large2DSM[j].push_back(this->large2[i][j]);
                }
            }
        }

        for (int i = 0; i < getNumberOfTuplesInRelation(this->small); i++) {
            for (int j = 0; j < getNumberOfValuesInTuple(this->small[i]); j++) {
                if (getAttributeValueType(this->small[i][j]) == 0) {
                    smallDSM[j].push_back(getLongValue(this->small[i][j]));
                } else if (getAttributeValueType(this->small[i][j]) == 1) {
                    smallDSM[j].push_back(getdoubleValue(this->small[i][j]));
                } else if (getAttributeValueType(this->small[i][j]) == 2) {
                    smallDSM[j].push_back(getStringValue(this->small[i][j]));
                } else {
                    // Null Pointer
                    smallDSM[j].push_back(this->small[i][j]);
                }
            }
        }


        for (int i = 0; i < smallDSM.size(); i++) {
            for (int j = 0; j < getNumberOfValuesInTuple(this->smallDSM[i]); j++) {
                if (getAttributeValueType(this->smallDSM[i][j]) == 0) {
                    std::cout << (getLongValue(this->smallDSM[i][j])) << "\n";
                } else if (getAttributeValueType(this->smallDSM[i][j]) == 1) {
                    std::cout << (getdoubleValue(this->smallDSM[i][j])) << "\n";
                } else if (getAttributeValueType(this->smallDSM[i][j]) == 2) {
                    std::cout << (getStringValue(this->smallDSM[i][j])) << "\n";
                } else {
                    // Null Pointer
                }
            }
        }

        sort(smallDSM);


        for (int i = 0; i < smallDSM.size(); i++) {
            for (int j = 0; j < getNumberOfValuesInTuple(this->smallDSM[i]); j++) {
                if (getAttributeValueType(this->smallDSM[i][j]) == 0) {
                    std::cout << (getLongValue(this->smallDSM[i][j])) << "\n";
                } else if (getAttributeValueType(this->smallDSM[i][j]) == 1) {
                    std::cout << (getdoubleValue(this->smallDSM[i][j])) << "\n";
                } else if (getAttributeValueType(this->smallDSM[i][j]) == 2) {
                    std::cout << (getStringValue(this->smallDSM[i][j])) << "\n";
                } else {
                    // Null Pointer
                }
            }
        }


    }

    long runQuery(long threshold = 9) {
        auto sum = 0L;
        // You should add your implementation here...


        return sum;
    }

private:
    void sort(std::vector<Column> &data) {
        mergeSort(data, 0, data[0].size() - 1);
    }

    void mergeSort(std::vector<Column> &data, int start, int end) {
        if (start >= end)
            return;

        auto mid = start + (end - start) / 2;
        mergeSort(data, start, mid);
        mergeSort(data, mid + 1, end);
        merge(data, start, mid, end);
    }

    void merge(std::vector<Column> &data, int start, int mid, int end) {
        auto const subArrayOne = mid - start + 1;
        auto const subArrayTwo = end - mid;

        std::vector<Tuple> leftArray;
        std::vector<Tuple> rightArray;

        leftArray.reserve(subArrayOne);
        for (auto i = 0; i < subArrayOne; i++)
            leftArray.push_back(Tuple{data[0][start + i], data[1][start + i], data[2][start + i]});
        rightArray.reserve(subArrayTwo);
        for (auto j = 0; j < subArrayTwo; j++)
            rightArray.push_back(Tuple{data[0][mid + 1 + j], data[1][mid + 1 + j], data[2][mid + 1 + j]});

        auto indexOfSubArrayOne = 0,
        indexOfSubArrayTwo = 0;
        int indexOfMergedArray = start;

        while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
            if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) {
                data[0][indexOfMergedArray] = leftArray[indexOfSubArrayOne][0];
                data[1][indexOfMergedArray] = leftArray[indexOfSubArrayOne][1];
                data[2][indexOfMergedArray] = leftArray[indexOfSubArrayOne][2];
                indexOfSubArrayOne++;
            }
            else {
                data[0][indexOfMergedArray] = rightArray[indexOfSubArrayTwo][0];
                data[1][indexOfMergedArray] = rightArray[indexOfSubArrayTwo][1];
                data[2][indexOfMergedArray] = rightArray[indexOfSubArrayTwo][2];
                indexOfSubArrayTwo++;
            }
            indexOfMergedArray++;
        }

        while (indexOfSubArrayOne < subArrayOne) {
            data[0][indexOfMergedArray] = leftArray[indexOfSubArrayOne][0];
            data[1][indexOfMergedArray] = leftArray[indexOfSubArrayOne][1];
            data[2][indexOfMergedArray] = leftArray[indexOfSubArrayOne][2];
            indexOfSubArrayOne++;
            indexOfMergedArray++;
        }

        while (indexOfSubArrayTwo < subArrayTwo) {
            data[0][indexOfMergedArray] = rightArray[indexOfSubArrayTwo][0];
            data[1][indexOfMergedArray] = rightArray[indexOfSubArrayTwo][1];
            data[2][indexOfMergedArray] = rightArray[indexOfSubArrayTwo][2];
            indexOfSubArrayTwo++;
            indexOfMergedArray++;
        }
    }
};

class DBMSImplementationForCompetition : public DBMSImplementationForMarks {
public:
    static constexpr char const *teamName =
            nullptr; // set this to your team name if you mean to compete
};

#endif /* ADVANCEDDB2022COURSEWORK1_H */
