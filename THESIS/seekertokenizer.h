#ifndef SEEKERTOKENIZER_H
#define SEEKERTOKENIZER_H

#include <QRegularExpression>
#include <operatorpriority.h>
#include <QMap>
#include <QVector>
/**
 * @brief The SeekerTokenizer class
 * \note it splices, removes, checks: string, operation, typos, etc... most of the method are based on QRegularExpression
 */
class SeekerTokenizer
{
public:
    /**
     * @brief SeekerTokenizer
     * @param pattern
     */
    explicit SeekerTokenizer();

    ~SeekerTokenizer();
    /**
     * @brief init fills the operations map based of their priority
     */
    void init();
    /**
     * @brief singleOperatorMatcher
     * @param parenthesisContent
     * @return QString
     * \note this recursive method returns the most nested parenthesis
     */
    QString singleOperatorMatcher(const QString &parenthesisContent);
    /**
     * @brief multiOperatorMatcher
     * @param baseString
     * @return QVector
     * \note I will change this method for returning a match better structed, a vector of QRegularExpressionMatch
     */
    QVector<QRegularExpressionMatch> multiOperatorMatcher(const QString &baseString);
    /**
     * @brief operatorsNumberChecker
     * @param baseString
     * @return int
     * \note it checkes is the part of the query passed contains more than one operator
     */
    int operatorsNumberChecker(const QString &baseString);
    /**
     * @brief parenthesisMatcher
     * @param query
     * @param wordRef
     * @return QString
     * \note this recursive method returns the most nested strings in the parenthesis
     */
    QString parenthesisMatcher(const QString &query, QString &wordRef);
    /**
     * @brief parenthesisChecker
     * @param query
     * @return bool
     * \note check the order the parenthesis are collected and the odd even count
     */
    bool parenthesisChecker(QString query);
    /**
     * @brief multiOperatorChecker
     * @param baseString
     * @return bool
     * \note checks if the operators are correct and if there is a value in between of them
     */
    bool multiOperatorChecker(QString baseString);
    /**
     * @brief operatorMap
     * @return the map
     * \deprecated
     */
    const QMap<QString, OperatorPriority> &operatorMap() const;
    /**
     * @brief checkOperatorPriority
     * @param op
     * @return Operator Priority
     * \note return the operator priority
     */
    OperatorPriority checkOperatorPriority(const QString &op);
    /**
     * @brief checkStringNumber
     * @param str
     * @return bool
     * \note checks if a string is a number
     */
    bool checkStringNumber(const QString &str);

private:

    QRegularExpression _pattern;
    /**
     * @brief _parenthesisPat may init this const;
     */
    QRegularExpression _parenthesisPat;

    QMap <QString, OperatorPriority> _operatorMap;

};

#endif // SEEKERTOKENIZER_H


