#ifndef SEEKERPARSER_H
#define SEEKERPARSER_H

#include <seekertokenizer.h>
#include <QHash>
#include <QVariant>
#include <seekerexchange.h>
#include <QJsonObject>
/**
 * @brief The SeekerParser class
 * \note this parser class collects informations from the QRegularExpressionMatcher and works the contents of parenthesis recursively
 */
class SeekerParser
{
public:
    /**
     * @brief SeekerParser
     * @param tok
     * @param query
     */
    SeekerParser(SeekerTokenizer *tok, const QStringList wordList);

    ~SeekerParser();
    /**
     * @brief queryParsing starts the parsing
     */
    void queryParsing(const QString &query);
    /**
     * @brief setJsonObj
     * @param doc
     */
    void setJsonObj(const QJsonObject &doc);
    /**
     * @brief killTheParser
     * @return bool
     * \note if true the parser will terminate at the end of the current loop index
     */
    bool killTheParser() const;
    /**
     * @brief query
     * @return the query to be copied
     */
    const QString &query();

private:
    /**
     * @brief sortByProperty
     * @param vect
     * @param property
     * \note this method uses the stable_sort descending order
     */
    void sortByProperty(QVector<QRegularExpressionMatch> &vect);
    /**
     * @brief parseNakedQuery
     * @param query
     * \note it parses a line of operations without parenthesis
     */
    void parseNakedQuery(QString &query);
    /**
     * @brief parserSingleTokenizer
     * @param parenthCont
     * @param currentOperator
     * \note it parse the content on a single parenthesis
     */
    void parserSingleTokenizer(const QString &parenthCont, const QString &currentOperator);
    /**
     * @brief parserMultiTokenizer
     * @param parenthCont
     * @param currentOperator
     * @param currentQuery
     * \note it takes the content from a line of operation and return the query with the right results correctly positioned
     */
    void parserMultiTokenizer(QString &parenthCont, const QString &currentOperator, QString &currentQuery);
    /**
     * @brief analyzeParenthesisContents
     * @param listParenthCont
     * @param currentOperator
     * @return boolean
     * \note it takes the parenthesis content and decides which is the leading value and returns the results if no error are
     * \note found. In case of errors the parsing will stop.
     */
    bool analyzeParenthesisContents(const QStringList &listParenthCont, const QString &currentOperator);
    /**
     * @brief fillCaseInsensitive this method keeps the a qhash with lowercase values and the relative CaseSensitive values
     */
    void fillCaseInsensitive();
    /**
     * @brief typeChecker
     * @param str
     * @return qvariant
     * \note it return the value to be matched for each json doc
     */
    QVariant typeChecker(const QString &str);

    SeekerTokenizer *_theTok;

    SeekerExchange *_theExchanger;

    QString _query;

    QStringList _wordList;

    QHash<QString, QString> _caseInsensitive;

    QVector<QRegularExpressionMatch> _operatorsPriVect;

    QVector<QRegularExpressionMatch> _operatorsPosVect;

    QJsonObject _jsonToParse;

    bool _killTheParser;

    bool _nakedQuery;

};

#endif // SEEKERPARSER_H
