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
// YOU MAY NOT ADD ANY OTHER INCLUDES!!!
using AttributeValue = std::variant<long, double, char const *>;
using Tuple = std::vector<AttributeValue>;
using Relation = std::vector<Tuple>;
using Column = std::vector<AttributeValue>;

/**
 * 0 is long, 1 is double, 2 is a c-string
 */
inline size_t getAttributeValueType(AttributeValue const &value) { return value.index(); }

inline long getLongValue(AttributeValue const &value) {
    return std::get<long>(value);
}

inline double getdoubleValue(AttributeValue const &value) {
    return std::get<double>(value);
}

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
class DBMSImplementationForMarks {
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

        this->large1 = *large1;
        this->large2 = *large2;
        this->small = *small;

        /* Pre-sort large1 and large2 to use for sort-merge-join later on. */
        std::qsort(&this->large1[0],
                   this->large1.size(),
                   sizeof(Tuple),
                   compareTuples);
        std::qsort(&this->large2[0],
                   this->large2.size(),
                   sizeof(Tuple),
                   compareTuples);

        /* Initialise DSM representations of relations. */
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

        /* Build DSM representations. */
        for (int i = 0; i < getNumberOfTuplesInRelation(this->large1); i++) {
            for (int j = 0; j < getNumberOfValuesInTuple(this->large1[i]);
                 j++) {
                if (getAttributeValueType(this->large1[i][j])
                    == LONG_ATTRIBUTE_INDEX) {
                    large1DSM[j].push_back(getLongValue(this->large1[i][j]));
                } else if (getAttributeValueType(this->large1[i][j])
                    == DOUBLE_ATTRIBUTE_INDEX) {
                    large1DSM[j].push_back(getdoubleValue(this->large1[i][j]));
                } else if (getAttributeValueType(this->large1[i][j])
                    == STRING_ATTRIBUTE_INDEX) {
                    large1DSM[j].push_back(getStringValue(this->large1[i][j]));
                }
            }
        }

        for (int i = 0; i < getNumberOfTuplesInRelation(this->large2); i++) {
            for (int j = 0; j < getNumberOfValuesInTuple(this->large2[i]);
                 j++) {
                if (getAttributeValueType(this->large2[i][j])
                    == LONG_ATTRIBUTE_INDEX) {
                    large2DSM[j].push_back(getLongValue(this->large2[i][j]));
                } else if (getAttributeValueType(this->large2[i][j])
                    == DOUBLE_ATTRIBUTE_INDEX) {
                    large2DSM[j].push_back(getdoubleValue(this->large2[i][j]));
                } else if (getAttributeValueType(this->large2[i][j])
                    == STRING_ATTRIBUTE_INDEX) {
                    large2DSM[j].push_back(getStringValue(this->large2[i][j]));
                }
            }
        }

        for (int i = 0; i < getNumberOfTuplesInRelation(this->small); i++) {
            for (int j = 0; j < getNumberOfValuesInTuple(this->small[i]); j++) {
                if (getAttributeValueType(this->small[i][j])
                    == LONG_ATTRIBUTE_INDEX) {
                    smallDSM[j].push_back(getLongValue(this->small[i][j]));
                } else if (getAttributeValueType(this->small[i][j])
                    == DOUBLE_ATTRIBUTE_INDEX) {
                    smallDSM[j].push_back(getdoubleValue(this->small[i][j]));
                } else if (getAttributeValueType(this->small[i][j])
                    == STRING_ATTRIBUTE_INDEX) {
                    smallDSM[j].push_back(getStringValue(this->small[i][j]));
                }
            }
        }

    }

    long runQuery(long threshold = 9) {
        auto sum = 0L;

        /* Initialise DSM representation of intermediate states.
         * Since we know we are only concerned with the first 3 attributes
         * of the table, we can hardcode the size of the DSM. */
        std::vector<Column> res; // {a, b1, c1, b3, c3}
        std::vector<Column> res2; // {a, b1, c1, b3, c3, b2, c2}

        for (int i = 0; i < 5; i++) {
            std::vector<AttributeValue> temp;
            res.push_back(temp);
        }
        for (int i = 0; i < 7; i++) {
            std::vector<AttributeValue> temp;
            res2.push_back(temp);
        }

        /* Sort-merge join the large tables and hash-join the smaller table. */
        mergeJoin(res, large1DSM, large2DSM);
        hashJoin(res2, res, smallDSM);

        /* Calculate result of query. */
        for (size_t i = 0; i < res2[0].size(); i++) {
            long current_b_sum = 0;
            for (size_t j = 1; j < res2.size(); j += 2)
                current_b_sum += getLongValue(res2[j][i]);

            if (current_b_sum > threshold) {
                long current_c_multiple = 1;
                for (size_t j = 2; j < res2.size(); j += 2)
                    current_c_multiple *= getLongValue(res2[j][i]);
                sum += current_c_multiple;
            }
        }

        return sum;
    }

 private:

    /* Rounds a double to a long. */
    static inline long doubleToLong(double n) {
        return (long) (n > 0 ? n + 0.5 : n - 0.5);
    }

    /* Comparator function used to sort N-ary storage relations. */
    static inline int compareTuples(const void *t1, const void *t2) {
        AttributeValue a = (*(Tuple *) t1)[0];
        AttributeValue b = (*(Tuple *) t2)[0];

        return compareAttributes(a, b);
    }

    /* Compares equality of two attributes, taking into account their types.
     * Note that this function is very similar to compareAttributes, but is
     * faster since it only has to check for equality and no need to check for
     * an ordering. */
    static inline bool attributesAreEqual(AttributeValue a, AttributeValue b) {
        size_t typeA = getAttributeValueType(a);
        size_t typeB = getAttributeValueType(b);

        if (typeA != typeB) return false;

        switch (typeA) {
            case LONG_ATTRIBUTE_INDEX:
                return getLongValue(a) == getLongValue(b);
            case DOUBLE_ATTRIBUTE_INDEX:
                return doubleToLong(getdoubleValue(a))
                    == doubleToLong(getdoubleValue(b));
            case STRING_ATTRIBUTE_INDEX:
                return getStringValue(a) == getStringValue(b);
        }

        return false;
    }

    /* Compares two attributes, taking into account their types.
     * Returns -1 if a < b, 0 if a == b, 1 if a > b */
    static inline int compareAttributes(AttributeValue a, AttributeValue b) {
        size_t typeA = getAttributeValueType(a);
        size_t typeB = getAttributeValueType(b);

        if (typeA < typeB) return -1;
        if (typeA > typeB) return 1;

        switch (typeA) {
            case LONG_ATTRIBUTE_INDEX:
                if (getLongValue(a) < getLongValue(b))return -1;
                if (getLongValue(a) == getLongValue(b)) return 0;
                if (getLongValue(a) > getLongValue(b)) return 1;
            case DOUBLE_ATTRIBUTE_INDEX:
                if (doubleToLong(getdoubleValue(a))
                    < doubleToLong(getdoubleValue(b)))
                    return -1;
                if (doubleToLong(getdoubleValue(a))
                    == doubleToLong(getdoubleValue(b)))
                    return 0;
                if (doubleToLong(getdoubleValue(a))
                    > doubleToLong(getdoubleValue(b)))
                    return 1;
            case STRING_ATTRIBUTE_INDEX:auto strA = getStringValue(a);
                auto strB = getStringValue(b);

                if (strA < strB) return -1;
                if (strA == strB) return 0;
                if (strA > strB) return 1;
        }

        return 1;
    }

    /* Hash function that returns -1 if input is invalid. */
    static inline long hash(AttributeValue input, unsigned long hashtableSize) {
        long hashValue;

        /* Calculate modulo by using bitwise AND. Hashtable size must be
         * multiple of two! */
        switch (getAttributeValueType(input)) {
            case LONG_ATTRIBUTE_INDEX:
                hashValue = getLongValue(input) & (hashtableSize - 1);
                break;
            case DOUBLE_ATTRIBUTE_INDEX:
                hashValue = doubleToLong(getdoubleValue(input))
                    & (hashtableSize - 1);
                break;
            case STRING_ATTRIBUTE_INDEX:
                if (getStringValue(input) == getStringValue(nullptr)) {
                    return -1; // NULL entry
                }
                hashValue =
                    ((long) *getStringValue(input)) & (hashtableSize - 1);
                break;
            default:hashValue = -1;
        }

        return hashValue;
    }

    /* Calculates the smallest power of two larger than size provided. */
    static inline int calculateHashtableSize(int n) {
        int count = 0;
        while (n > 0) {
            n = n >> 1;
            count += 1;
        }
        return 1 << count;
    }

    /* Hashes relation b and joins with relation a, then puts result in res. */
    static void hashJoin(std::vector<Column> &res,
                         std::vector<Column> &a,
                         std::vector<Column> &b) {
        // Over-allocate hashtable by 4
        int hashtableSize = calculateHashtableSize(b[0].size()) << 2;

        // Stores tuple of <Slot occupied, Index in original table>
        std::vector<std::tuple<bool, int>> hashtable(hashtableSize);

        // Initialise hashtable
        for (size_t i = 0; i < hashtableSize; i++)
            hashtable[i] = std::tuple<bool, int>(false, -1);

        // Build hashtable on the first column
        for (size_t i = 0; i < b[0].size(); i++) {

            AttributeValue input = b[0][i];
            long hashValue = hash(input, hashtableSize);
            if (hashValue == -1) {
                continue;
            }

            while (std::get<0>(hashtable[hashValue]))
                hashValue = (++hashValue & (hashtableSize - 1));
            hashtable[hashValue] =
                std::tuple<bool, int>(true, i); // Store index of column
        }

        // Probe hashtable
        for (size_t i = 0; i < a[0].size(); i++) {

            AttributeValue probeInput = a[0][i];
            long hashValue = hash(probeInput, hashtableSize);
            if (hashValue == -1) {
                continue;
            }

            // Only stop once we reach an empty slot, since there could be
            // duplicates further down the chain.
            while (std::get<0>(hashtable[hashValue])) {
                int index = std::get<1>(hashtable[hashValue]);

                if (attributesAreEqual(b[0][index], probeInput)) {
                    res[0].push_back(a[0][i]); // res.a
                    res[1].push_back(a[1][i]); // res.b1
                    res[2].push_back(a[2][i]); // res.c1
                    res[3].push_back(a[3][i]); // res.b2
                    res[4].push_back(a[4][i]); // res.c2
                    res[5].push_back(b[1][index]); // res.b3
                    res[6].push_back(b[2][index]); // res.c3
                }

                hashValue = (++hashValue & (hashtableSize - 1));
            }
        }
    }

    /* Joins relations a and b, assuming both are sorted. */
    static void mergeJoin(std::vector<Column> &res,
                          std::vector<Column> &a,
                          std::vector<Column> &b) {
        long leftI = 0;
        long rightI = 0;

        if (a[0].empty() || b[0].empty()) {
            return;
        }

        // Pass over nulls
        while (getAttributeValueType(a[0][leftI]) == 2
            && getStringValue(a[0][leftI]) == getStringValue(nullptr)) {
            leftI++;
        }

        while (getAttributeValueType(b[0][rightI]) == 2
            && getStringValue(b[0][rightI]) == getStringValue(nullptr)) {
            rightI++;
        }

        // Step through both relations to try and merge them
        while (leftI < a[0].size() && rightI < b[0].size()) {

            AttributeValue leftInput = a[0][leftI];
            AttributeValue rightInput = b[0][rightI];

            switch (compareAttributes(leftInput, rightInput)) {
                case -1: // a < b
                    leftI++;
                    break;
                case 1: // a > b
                    rightI++;
                    break;
                case 0:
                    // Add to results
                    res[0].push_back(a[0][leftI]); // a
                    res[1].push_back(a[1][leftI]); // b1
                    res[2].push_back(a[2][leftI]); // c1
                    res[3].push_back(b[1][rightI]); // b2
                    res[4].push_back(b[2][rightI]); // c2

                    auto tempL = leftI + 1;

                    // Check for duplicates in left side
                    while (tempL < a[0].size() && rightI < b[0].size()) {
                        if (attributesAreEqual(a[0][tempL], b[0][rightI])) {
                            res[0].push_back(a[0][tempL]);
                            res[1].push_back(a[1][tempL]);
                            res[2].push_back(a[2][tempL]);
                            res[3].push_back(b[1][rightI]); // b2
                            res[4].push_back(b[2][rightI]); // c2
                            tempL++;
                        } else {
                            // Here, all duplicates in the left has been traversed
                            // so we advance the right pointer and continue.
                            rightI++;
                            if (b[0][rightI] > a[0][leftI]) {
                                break;
                            }
                        }
                    }
                    leftI = tempL;
                    break;
            }
        }
    }
};

class DBMSImplementationForCompetition : public DBMSImplementationForMarks {
 public:
    static constexpr char const *teamName =
        nullptr; // set this to your team name if you mean to compete
};

#endif /* ADVANCEDDB2022COURSEWORK1_H */
