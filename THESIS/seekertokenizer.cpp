#include "seekertokenizer.h"
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QObject>
#include <QVector>

SeekerTokenizer::SeekerTokenizer()
{
    this->_pattern = QRegularExpression("((==|\\!=|<[>=]?|>=?|\\|\\||\\&\\&|\\~=))");
    this->_parenthesisPat = QRegularExpression("(\\(([^()]|(?R))*\\))");
}

SeekerTokenizer::~SeekerTokenizer()
{
    qDebug("SeekerTokenizer::~SeekerTokenizer");
}

const QMap<QString, OperatorPriority> &SeekerTokenizer::operatorMap() const
{
    return(this->_operatorMap);
}

QString SeekerTokenizer::singleOperatorMatcher(const QString &parenthesisContent)
{
    QRegularExpressionMatch match = this->_pattern.match(parenthesisContent);
    QString matched = "none";
    if (match.hasMatch()) {
        matched = match.captured(0);
        return(matched);
    }
    else return(matched);
}

QVector<QRegularExpressionMatch> SeekerTokenizer::multiOperatorMatcher(const QString &baseString)
{
    QRegularExpressionMatchIterator i = this->_pattern.globalMatch(baseString);
    QRegularExpressionMatch match;
    QVector<QRegularExpressionMatch> matchVect;
    QString currentOper;
    while (i.hasNext()) {
        match = i.next();
        currentOper = match.captured(0);
        matchVect.append(match);
    }
    return matchVect;
}

int SeekerTokenizer::operatorsNumberChecker(const QString &baseString)
{
    int num = 0;
    QRegularExpressionMatchIterator i = this->_pattern.globalMatch(baseString);
    while (i.hasNext()) {
        i.next();
        num++;
    }
    return num;
}

OperatorPriority SeekerTokenizer::checkOperatorPriority(const QString &op)
{
    return(this->_operatorMap.value(op));
}

bool SeekerTokenizer::checkStringNumber(const QString &str)
{
    QString currentStr = str;
    currentStr = currentStr.replace(QRegularExpression("[0-9]+\\.[0-9]+|[0-9]+"), "");
    if(currentStr.isEmpty())
        return(true);
    else
        return(false);
}

QString SeekerTokenizer::parenthesisMatcher(const QString &query, QString &wordRef)
{
    QRegularExpressionMatch match = this->_parenthesisPat.match(query);
    if (match.hasMatch()) {
        wordRef = match.captured(0);
        wordRef.remove(0,1);
        wordRef.remove(-1,1);
        parenthesisMatcher(wordRef, wordRef);
    }
    return(wordRef);
}

void SeekerTokenizer::init()
{
    this->_operatorMap.insert("<", OperatorPriority::High);
    this->_operatorMap.insert(">", OperatorPriority::High);
    this->_operatorMap.insert("<=", OperatorPriority::High);
    this->_operatorMap.insert(">=", OperatorPriority::High);
    this->_operatorMap.insert("~=", OperatorPriority::High);
    this->_operatorMap.insert("==", OperatorPriority::Normal);
    this->_operatorMap.insert("!=", OperatorPriority::Normal);
    this->_operatorMap.insert("||", OperatorPriority::Low);
    this->_operatorMap.insert("&&", OperatorPriority::Low);
}

bool SeekerTokenizer::parenthesisChecker(QString query)
{
    if(query.startsWith('(') && query.endsWith(')'))
        return false;
    QRegularExpressionMatchIterator i = this->_parenthesisPat.globalMatch(query);
    QRegularExpressionMatch match;
    QString word, toReplace;
    while (i.hasNext()) {
        match = i.next();
        word = match.captured(0);
        word.remove(0,1);
        word.remove(-1,1);
        toReplace = QString("(%1)").arg(word);
        query.replace(toReplace, "check");
        parenthesisChecker(query);
    }
    if(query.contains(QRegularExpression("(\\(|\\))")))
        return(false);
    else
        return(true);
}

bool SeekerTokenizer::multiOperatorChecker(QString baseString)
{
    baseString = baseString.replace(QRegularExpression("(\\(|\\))"), "");
    QString wrongOperators = baseString.toLower();
    if(!wrongOperators.contains(this->_pattern))
        return false;
    else if(wrongOperators.contains("true") || wrongOperators.contains("false"))
        return false;
    wrongOperators = wrongOperators.replace(this->_pattern, "");
    if(wrongOperators.contains(QRegularExpression("\\!|\\&|\\=|\\~")))
        return false;
    else if(wrongOperators.contains('|'))
        return false;
    QRegularExpressionMatchIterator i = this->_pattern.globalMatch(baseString);
    QRegularExpressionMatch match, nextMatch;
    while (i.hasNext()) {
        match = i.next();
        match.capturedLength(0);
        if(match.capturedStart(0) == 0)
            return(false);
        else if(match.capturedEnd(0) == baseString.length())
            return(false);
        if(i.hasNext()) {
            nextMatch = i.peekNext();
            int dist = nextMatch.capturedStart(0) - match.capturedEnd(0);
            if(dist == 0)
                return(false);
        }
    }
    return(true);
}
