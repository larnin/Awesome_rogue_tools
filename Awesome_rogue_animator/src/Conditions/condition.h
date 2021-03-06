#ifndef CONDITION_H
#define CONDITION_H

#include <QJsonObject>
#include <QWidget>
#include <QObject>
#include <memory>

template <typename T>
using shared_unique_ptr = std::shared_ptr<std::unique_ptr<T>>;

template <typename U, typename T, typename ... Args>
shared_unique_ptr<U> make_shared_unique(Args && ... args)
{
    return std::make_shared<std::unique_ptr<U>>(std::make_unique<T>(std::forward<Args>(args)...));
}

enum ConditionType
{
    AND_CONDITION = 0,
    OR_CONDITION = 1,
    NOT_CONDITION = 2,
    ANIMATION_FINISHED_CONDITION = 10,
    WAIT_CONDITION = 20,
    WAIT_AFTER_ACTIVE_CONDITION = 21,
    PROPERTY_EQUAL_CONDITION = 30,
    PROPERTY_NOT_EQUAL_CONDITION = 31,
    PROPERTY_INFERIOR_CONDITION = 32,
    PROPERTY_INFERIOR_OR_EQUAL_CONDITION = 33,
    PROPERTY_SUPERIOR_CONDITION = 34,
    PROPERTY_SUPERIOR_OR_EQUAL_CONDITION = 35
};

class Condition : public QObject
{
    Q_OBJECT
public:
    Condition(ConditionType type);
    virtual ~Condition() = default;

    QJsonObject save() const;
    inline ConditionType type() const {return m_conditionType;}

    static shared_unique_ptr<Condition> load(const QJsonObject & j);

    static void saveProperty(QJsonObject & obj, const std::string & property, int value);
    static QJsonArray saveConditionList(const std::vector<shared_unique_ptr<Condition> > & list);
    static std::unique_ptr<Condition> create(ConditionType type);

    virtual void draw(QWidget * parent) = 0;
    virtual void reset() = 0;

protected:
    virtual void saveData(QJsonObject & o) const = 0;

private:
    ConditionType m_conditionType;
};

#endif // CONDITION_H
