#ifndef ADVANCEDDB2022COURSEWORK1_H
#define ADVANCEDDB2022COURSEWORK1_H
#define LONG_ATTRIBUTE_INDEX 0
#define DOUBLE_ATTRIBUTE_INDEX 1
#define STRING_ATTRIBUTE_INDEX 2

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
 * 0 is long, 1 is double, 2 is a c-string
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
                if (getAttributeValueType(this->large1[i][j]) == LONG_ATTRIBUTE_INDEX) {
                    large1DSM[j].push_back(getLongValue(this->large1[i][j]));
                } else if (getAttributeValueType(this->large1[i][j]) == DOUBLE_ATTRIBUTE_INDEX) {
                    large1DSM[j].push_back(getdoubleValue(this->large1[i][j]));
                } else if (getAttributeValueType(this->large1[i][j]) == STRING_ATTRIBUTE_INDEX) {
                    large1DSM[j].push_back(getStringValue(this->large1[i][j]));
                }
            }
        }

        for (int i = 0; i < getNumberOfTuplesInRelation(this->large2); i++) {
            for (int j = 0; j < getNumberOfValuesInTuple(this->large2[i]); j++) {
                if (getAttributeValueType(this->large2[i][j]) == LONG_ATTRIBUTE_INDEX) {
                    large2DSM[j].push_back(getLongValue(this->large2[i][j]));
                } else if (getAttributeValueType(this->large2[i][j]) == DOUBLE_ATTRIBUTE_INDEX) {
                    large2DSM[j].push_back(getdoubleValue(this->large2[i][j]));
                } else if (getAttributeValueType(this->large2[i][j]) == STRING_ATTRIBUTE_INDEX) {
                    large2DSM[j].push_back(getStringValue(this->large2[i][j]));
                }
            }
        }

        for (int i = 0; i < getNumberOfTuplesInRelation(this->small); i++) {
            for (int j = 0; j < getNumberOfValuesInTuple(this->small[i]); j++) {
                if (getAttributeValueType(this->small[i][j]) == LONG_ATTRIBUTE_INDEX) {
                    smallDSM[j].push_back(getLongValue(this->small[i][j]));
                } else if (getAttributeValueType(this->small[i][j]) == DOUBLE_ATTRIBUTE_INDEX) {
                    smallDSM[j].push_back(getdoubleValue(this->small[i][j]));
                } else if (getAttributeValueType(this->small[i][j]) == STRING_ATTRIBUTE_INDEX) {
                    smallDSM[j].push_back(getStringValue(this->small[i][j]));
                }
            }
        }

    }

    long runQuery(long threshold = 9) {
        auto sum = 0L;
        // You should add your implementation here...
        sort(large1DSM);
        sort(large2DSM);

        std::vector<Column> res;

        for (int i = 0; i < 5; i++) {
            std::vector<AttributeValue> temp;
            res.push_back(temp);
        }

        mergeJoin(res, large1DSM, large2DSM);

//        for (int i = 0; i < res[0].size(); i++) {
//            for (int j = 0; j < 5; j++) {
//                std::cout << res[j][i] << ", ";
//            }
//            std::cout << "\n";
//        }

        return sum;
    }

private:
    void mergeJoin(std::vector<Column> &res, std::vector<Column> &a, std::vector<Column> &b) {
        auto leftI = 0;
        auto rightI = 0;

        // passing over the nulls
        while (getAttributeValueType(a[0][leftI]) == 2 && getStringValue(a[0][leftI]) == getStringValue(nullptr)) {
            leftI++;
        }

        // passing over the nulls
        while (getAttributeValueType(b[0][rightI]) == 2 && getStringValue(b[0][rightI]) == getStringValue(nullptr)) {
            rightI++;
        }

        while (leftI < a[0].size() && rightI < b[0].size()) {
            if (getAttributeValueType(a[0][leftI]) == getAttributeValueType(b[0][rightI])) {

                if (getAttributeValueType(a[0][leftI]) == 1) {
                    int aInt = (int) getdoubleValue(a[leftI][0]);
                    int bInt = (int) getdoubleValue(b[rightI][0]);

                    if (aInt == bInt) {
                        res[0].push_back(a[0][leftI]);
                        res[1].push_back(a[1][leftI]);
                        res[2].push_back(a[2][leftI]);
                        res[3].push_back(b[1][rightI]);
                        res[4].push_back(b[2][rightI]);

                        auto tempL = leftI + 1;

                        // check for dupes in left side
                        while (tempL < a[0].size() && rightI < b[0].size()
                               && getAttributeValueType(a[0][tempL]) == 1 &&
                               getAttributeValueType(b[0][rightI]) == 1) {
                            int tempA = (int) getdoubleValue(a[0][tempL]);
                            int tempB = (int) getdoubleValue(b[0][rightI]);
                            if (tempA == tempB) {
                                res[0].push_back(a[0][tempL]);
                                res[1].push_back(a[1][tempL]);
                                res[2].push_back(a[2][tempL]);
                                res[3].push_back(b[1][rightI]);
                                res[4].push_back(b[2][rightI]);
                                tempL++;
                            } else {
                                rightI++;
                                if (b[0][rightI] > a[0][leftI]) {
                                    leftI = tempL;
                                    break;
                                }
                            }

                        }

                    } else {
                        if (aInt < bInt) {
                            leftI++;
                        } else {
                            rightI++;
                        }
                    }

                } else {
                    if (a[0][leftI] == b[0][rightI]) {
                        res[0].push_back(a[0][leftI]);
                        res[1].push_back(a[1][leftI]);
                        res[2].push_back(a[2][leftI]);
                        res[3].push_back(b[1][rightI]);
                        res[4].push_back(b[2][rightI]);

                        auto tempL = leftI + 1;

                        // check for dupes in left side
                        while (tempL < a[0].size() && rightI < b[0].size()) {
                            if (a[0][tempL] == b[0][rightI]) {
                                res[0].push_back(a[0][tempL]);
                                res[1].push_back(a[1][tempL]);
                                res[2].push_back(a[2][tempL]);
                                res[3].push_back(b[1][rightI]);
                                res[4].push_back(b[2][rightI]);
                                tempL++;
                            } else {
                                rightI++;
                                if (b[0][rightI] > a[0][leftI]) {
                                    leftI = tempL;
                                    break;
                                }
                            }

                        }
                    } else {
                        if (a[0][leftI] < b[0][rightI]) {
                            leftI++;
                        } else {
                            rightI++;
                        }
                    }


                }

            } else {
                if (getAttributeValueType(a[0][leftI]) < getAttributeValueType(b[0][rightI])) {
                    leftI++;
                } else {
                    rightI++;
                }
            }
        }


    }

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
            } else {
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
