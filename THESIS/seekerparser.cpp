#include "seekerparser.h"
#include "validatejson.h"
#include <QDebug>

SeekerParser::SeekerParser(SeekerTokenizer *tok, const QStringList wordList)
{
    this->_theTok = tok;
    this->_theExchanger = new SeekerExchange();
    this->_wordList = wordList;
    this->fillCaseInsensitive();
}

SeekerParser::~SeekerParser()
{
    qDebug("SeekerParser::~SeekerParser");
    delete this->_theTok;
    delete this->_theExchanger;
}

void SeekerParser::queryParsing(const QString &query)
{
    this->_query = query;
    this->_operatorsPriVect.clear();
    this->_killTheParser = false;
    this->_nakedQuery = false;
    QString parenthCont;
    //QString currentOper;
    while(true) {
        QString queryRef;
        parenthCont = this->_theTok->parenthesisMatcher(this->_query, queryRef);
        if(parenthCont.isEmpty()) {
            this->_nakedQuery = true;
            break;
        }
        if(this->_theTok->operatorsNumberChecker(parenthCont) == 1) {
            //currentOper = this->_theTok->singleOperatorMatcher(parenthCont);
            this->parserSingleTokenizer(parenthCont, this->_theTok->singleOperatorMatcher(parenthCont));
        }
        else {
            this->_operatorsPosVect = this->_theTok->multiOperatorMatcher(parenthCont);
            this->_operatorsPriVect = this->_operatorsPosVect;
            this->sortByProperty(this->_operatorsPriVect);
            this->parseNakedQuery(parenthCont);
        }
    }
    this->_operatorsPosVect = this->_theTok->multiOperatorMatcher(this->_query);
    this->_operatorsPriVect = this->_operatorsPosVect;
    this->sortByProperty(this->_operatorsPriVect);
    this->parseNakedQuery(this->_query);
    if(this->_killTheParser == false)this->parserMultiTokenizer(this->_query, this->_theTok->singleOperatorMatcher(this->_query), this->_query);

    this->_operatorsPosVect.clear();
    this->_operatorsPriVect.clear();
}

void SeekerParser::setJsonObj(const QJsonObject &doc)
{
    this->_jsonToParse = doc;
}

void SeekerParser::sortByProperty(QVector<QRegularExpressionMatch> &vect)
{
    std::stable_sort(vect.begin(), vect.end(), [this](const QRegularExpressionMatch &a, const QRegularExpressionMatch &b) {
        int val_a = static_cast<int>(this->_theTok->checkOperatorPriority(a.captured(0)));
        int val_b = static_cast<int>(this->_theTok->checkOperatorPriority(b.captured(0)));
        return val_a > val_b;
    });
}

void SeekerParser::parseNakedQuery(QString &query)
{
    while(this->_operatorsPriVect.size() > 1) {
        QRegularExpressionMatch currentOp = this->_operatorsPriVect.at(0);
        QString subStr, currentOpSign;
        int currentOpPos = 0, currentPos = 0, rightPos = 0, leftPos = 0, dist = 0;
        for(int i = 0; i < this->_operatorsPosVect.size(); ++i) {
            currentOpPos = currentOp.capturedStart(0);
            currentOpSign = this->_operatorsPosVect.at(i).captured(0);
            currentPos = this->_operatorsPosVect.at(i).capturedStart(0);
            if(currentOpPos == currentPos) {
                if(i == 0 && this->_operatorsPosVect.size() > 0) {
                    leftPos = 0;
                    rightPos = this->_operatorsPosVect.at(i+1).capturedStart(0);
                    subStr = query.mid(leftPos, rightPos);
                }
                else if(i > 0) {
                    leftPos = this->_operatorsPosVect.at(i-1).capturedStart(0) + this->_operatorsPosVect.at(i-1).capturedLength(0);
                    if(i < this->_operatorsPosVect.size()-1) {
                        rightPos = this->_operatorsPosVect.at(i+1).capturedStart(0);
                        dist = rightPos - leftPos;
                    }
                    else if(i == this->_operatorsPosVect.size()-1) {
                        dist = -1;
                    }
                    subStr = query.mid(leftPos, dist);
                }
                this->parserMultiTokenizer(subStr, currentOpSign, query);
                if(this->_killTheParser == true) {
                    this->_operatorsPriVect.clear();
                    break;
                }
                this->_operatorsPosVect = this->_theTok->multiOperatorMatcher(query);
                this->_operatorsPriVect = this->_operatorsPosVect;
                this->sortByProperty(this->_operatorsPriVect);
                break;
            }
        }
    }
}

void SeekerParser::parserMultiTokenizer(QString &parenthCont, const QString &currentOperator, QString &currentQuery)
{
    QStringList listParenthCont = parenthCont.split(currentOperator);
    QString toReplace = QString("%1").arg(parenthCont);
    bool result = this->analyzeParenthesisContents(listParenthCont, currentOperator);
    QString boolText = result ? "true" : "false";
    int pos = currentQuery.indexOf(toReplace);
    currentQuery.replace(pos, toReplace.size(), boolText);
    if(!this->_nakedQuery) {
        this->_query.replace(this->_query.indexOf(toReplace), toReplace.size(), boolText);
    }
    if(this->_killTheParser) this->_query = "";
}

void SeekerParser::parserSingleTokenizer(const QString &parenthCont, const QString &currentOperator)
{
    QStringList listParenthCont = parenthCont.split(currentOperator);
    QString toReplace = QString("(%1)").arg(parenthCont);
    bool result = this->analyzeParenthesisContents(listParenthCont, currentOperator);
    QString boolText = result ? "true" : "false";
    // this->_query.replace(toReplace, boolText); moved to indexOf to prevent multiple replace at once
    int pos = this->_query.indexOf(toReplace);
    this->_query.replace(pos, toReplace.size(), boolText);
    if(this->_killTheParser) this->_query = "";
}

bool SeekerParser::killTheParser() const
{
    return(this->_killTheParser);
}

const QString& SeekerParser::query()
{
    return(this->_query);
}

bool SeekerParser::analyzeParenthesisContents(const QStringList &listParenthCont, const QString &currentOperator)
{
    QVariant leftValue, rightValue;
    QString leftString, rightString;
    bool wordListCheck = false;
    bool result;
    double leftDouble = 0, rightDouble = 0;
    if(this->_wordList.contains(listParenthCont.at(0), Qt::CaseInsensitive)) {
        leftValue = this->typeChecker(listParenthCont.at(0));
        wordListCheck = true;
    }
    else {
        leftValue = listParenthCont.at(0);
    }
    if(this->_wordList.contains(listParenthCont.at(1), Qt::CaseInsensitive)) {
        rightValue = this->typeChecker(listParenthCont.at(1));
        wordListCheck = true;
    }
    else {
        rightValue = listParenthCont.at(1);
    }
    if(leftValue.userType() == QMetaType::QString && rightValue.userType() == QMetaType::QString) {
        leftString = leftValue.toString();
        leftString = leftString.replace(" ", "");
        rightString = rightValue.toString();
        rightString = rightString.replace(" ", "");
        if((leftString == "true" || leftString == "false") && (rightString == "true" || rightString == "false")) {
            wordListCheck = true;
        }
    }
    if(leftValue.isNull() || rightValue.isNull() || !wordListCheck) {
        this->_killTheParser = true;
        return(result = false);
    }
    if(leftValue.userType() == QMetaType::QString && rightValue.userType() == QMetaType::QString) {
        leftString = leftValue.toString().toLower();
        leftString = leftString.replace(" ", "");
        rightString = rightValue.toString().toLower();
        rightString = rightString.replace(" ", "");
        if(currentOperator == "&&" || currentOperator == "||") {
            if((leftString == "true" || leftString == "false") && (rightString == "true" || rightString == "false")) {
                result = this->_theExchanger->findOperator(currentOperator, leftString, rightString);
                return(result);
            }
            this->_killTheParser = true;
            return(result = false);

        }
        else if(currentOperator == "~=") {
            result = leftString.contains(rightString);
            return(result);
        }
        else {
            result = this->_theExchanger->findOperator(currentOperator, leftString, rightString);
            return(result);
        }
    }
    else if(leftValue.userType() == QMetaType::Double && rightValue.userType() == QMetaType::Double)  {
        leftDouble = leftValue.toDouble();
        rightDouble = rightValue.toDouble();
        result = this->_theExchanger->findOperator(currentOperator, leftDouble, rightDouble);
        return(result);
    }
    else if(leftValue.userType() == QMetaType::Double) {
        leftDouble = leftValue.toDouble();
        if(rightValue.userType() == QMetaType::QString) {
            rightString = rightValue.toString();
            if(this->_theTok->checkStringNumber(rightString))
                rightDouble = QLocale::c().toDouble(rightString);
            else {
                this->_killTheParser = true;
                return(result = false);
            }
        }
        result = this->_theExchanger->findOperator(currentOperator, leftDouble, rightDouble);
        return(result);
    }
    else if(rightValue.userType() == QMetaType::Double) {
         rightDouble = rightValue.toDouble();
         if(leftValue.userType() == QMetaType::QString) {
             leftString = leftValue.toString();
             if(this->_theTok->checkStringNumber(leftString))
                 leftDouble = QLocale::c().toDouble(leftString);
             else {
                 this->_killTheParser = true;
                 return(result = false);
             }
         }
         result = this->_theExchanger->findOperator(currentOperator, leftDouble, rightDouble);
         return(result);
    }
    else return(result = false);
}

void SeekerParser::fillCaseInsensitive()
{
    for (int i = 0; i < this->_wordList.size(); ++i) {
        this->_caseInsensitive.insert(this->_wordList.at(i).toLower(), this->_wordList.at(i));
    }
}

QVariant SeekerParser::typeChecker(const QString &str) {

    QString tolowerStr = str.toLower();
    tolowerStr = this->_caseInsensitive.value(tolowerStr);
    QStringList currentStr = tolowerStr.split('.');
    QVariant value;
    QJsonObject obj = this->_jsonToParse.value(currentStr.at(0)).toObject();
    if(obj.value(currentStr.at(1)).isString()) {
        value = obj.value(currentStr.at(1)).toString();
        return(value);
    }
    else if(obj.value(currentStr.at(1)).isDouble()) {
        value = obj.value(currentStr.at(1)).toDouble();
        return(value);
    }
    else
        return(value);
}
