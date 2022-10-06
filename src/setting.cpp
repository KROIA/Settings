#include "setting.h"
#include "settings.h"

namespace Settings
{
Setting::Setting(Settings *parent)
    : QObject()
{
    m_parent = parent;
}
Setting::Setting(const Setting &other, Settings *parent)
    : QObject()
{
    m_parent = parent;
    m_parameter = other.m_parameter;
}
Setting::Setting(const QString &name, const QVariant value, Settings *parent)
{
    m_parent = parent;
    m_parameter.first = name;
    m_parameter.second = value;
}
Setting::Setting(const std::pair<QString,QVariant> &setting, Settings *parent)
{
    m_parent = parent;
    m_parameter = setting;
}
Setting::~Setting()
{

}

void Setting::setParent(Settings *parent)
{
    m_parent = parent;
}
Settings* Setting::getParent() const
{
    return m_parent;
}

const Setting &Setting::operator=(const Setting &other)
{
    setName(other.m_parameter.first);
    setValue(other.m_parameter.second);
    return *this;
}
const Setting &Setting::operator=(const QVariant &value)
{
    setValue(value);
    return *this;
}
const QVariant &Setting::getValue() const
{
    return m_parameter.second;
}
const QString &Setting::getName() const
{
    return m_parameter.first;
}
const std::pair<QString,QVariant> &Setting::getPair() const
{
    return m_parameter;
}

QString Setting::toString() const
{
    return "{ "+m_parameter.first+" = "+m_parameter.second.toString()+" }";
}
QDebug operator<<(QDebug debug, const Setting &setting)
{
    debug.nospace() << setting.toString();
    return debug;
}

void Setting::setValue(const QVariant &value)
{
    if(m_parameter.second == value) return;
    m_parameter.second = value;
    emit valueChanged(m_parameter.second);
}
void Setting::setName(const QString &name)
{
    if(m_parameter.first == name) return;

    if(m_parent)
        if(m_parent->exists(name))
        {
            SETTINGS_WARNING_PRETTY << "Unable to change name from" << m_parameter.first
            << " to " << name << "\nsame name already exists in " << m_parent->getName();
            return;
        }
    m_parameter.first = name;
    emit nameChanged(m_parameter.first);
}
}
