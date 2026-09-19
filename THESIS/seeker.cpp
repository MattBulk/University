#include "seeker.h"
#include <QDebug>

Seeker::Seeker(const QStringList wordList)
{
    this->_theSeekerTokenizer = new SeekerTokenizer();
    this->_theSeekerParser = new SeekerParser(this->_theSeekerTokenizer, wordList);
    this->_theSeekerTokenizer->init();
}

Seeker::~Seeker()
{
    qDebug("Seeker::~Seeker");
    delete this->_theSeekerParser;
}

bool Seeker::search(const QString &query)
{
    this->_query = query.simplified();
    this->_query.replace(" ", "");
    if(this->_theSeekerTokenizer->parenthesisChecker(query) && this->_theSeekerTokenizer->multiOperatorChecker(this->_query)) {
        this->_theSeekerParser->queryParsing(this->_query);
        return(true);
    }
    else {
        return(false);
    }
}

void Seeker::addDoc(const QJsonObject &doc)
{
    this->_theSeekerParser->setJsonObj(doc);
}

QString Seeker::result()
{
   if(this->_theSeekerParser->killTheParser())
       return("quit");
   else return(this->_theSeekerParser->query());
}

