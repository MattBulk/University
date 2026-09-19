#ifndef SEEKEREXCHANGE_H
#define SEEKEREXCHANGE_H

#include <QObject>
#include <QString>
/**
 * @brief operator ||
 * @param s1
 * @param s2
 * @return defined an operator || that works with strings
 */
inline bool operator ||(const QString& s1, const QString& s2)
{
    if(s1 == "false" && s2 == "false")
        return(false);
    else if((s1 == s2) || (s1 != s2))
        return(true);
    else
        return(false);
}
/**
 * @brief operator &&
 * @param s1
 * @param s2
 * @return defined an operator && that works with strings
 */
inline bool operator &&(const QString& s1, const QString& s2)
{
    if(s1 == "false" && s2 == "false")
        return(false);
    else if(s1 == s2)
        return(true);
    else
        return(false);
}
/**
 * @brief The SeekerExchange class
 */
class SeekerExchange : public QObject
{
    Q_OBJECT
public:
    explicit SeekerExchange(QObject *parent = nullptr);

    ~SeekerExchange();

    template<class T>
    /**
     * @brief findOperator
     * @param op
     * @param left
     * @param right
     * @return bool
     * \note this method finds the operator used and execute the operation
     */
    bool findOperator(const QString &op, const T &left, const T &right)
    {
        if(op == ">") return(left > right);
        else if(op == "<") return(left < right);
        else if(op == ">=") return(left >= right);
        else if(op == "<=") return(left <= right);
        else if(op == "==") return(left == right);
        else if(op == "!=") return(left != right);
        else if(op == "||") return(left || right);
        else if(op == "&&") return(left && right);
        else return(false);
    }

private:

};

#endif // SEEKEREXCHANGE_H
