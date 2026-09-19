#ifndef SEEKER_H
#define SEEKER_H

#include <QRegularExpression>
#include <seekertokenizer.h>
#include <seekerparser.h>
#include <operatorpriority.h>
#include <QMap>
#include <QJsonObject>
/**
 * @brief The Seeker class
 * \note this class cope with the
 */
class Seeker
{
public:
    /**
     * @brief Seeker
     */
    explicit Seeker(const QStringList wordList);

    ~Seeker();
    /**
     * @brief search
     * @param query
     * @return boolean
     * \note set the query ready to be passed to the parser if it is correct
     */
    bool search(const QString &query);
    /**
     * @brief addDoc
     * @param doc
     * \note adding a single json to be parsed
     */
    void addDoc(const QJsonObject &doc);
    /**
     * @brief result
     * @return qString
     * \notes returns the true result
     */
    QString result();

private:

    QRegularExpression _pattern;

    QString _query;

    SeekerTokenizer *_theSeekerTokenizer;

    SeekerParser *_theSeekerParser;
};

#endif // SEEKER_H
