#include "setting.h"
#include "settingGroup.h"

namespace Settings
{
Setting::Setting()
    : QObject()
{
    m_parent = nullptr;
}
Setting::Setting(const Setting &other)
    : QObject()
{
    m_parent = nullptr;
    m_parameter = other.m_parameter;
}
Setting::Setting(const QString &name, const QVariant value)
{
    m_parent = nullptr;
    m_parameter.first = name;
    m_parameter.second = value;
}
Setting::Setting(const std::pair<QString,QVariant> &setting)
{
    m_parent = nullptr;
    m_parameter = setting;
}
Setting::~Setting()
{
    m_parent = nullptr;
}

void Setting::setParent(SettingGroup *parent)
{
    m_parent = parent;
}
SettingGroup* Setting::getParent() const
{
    return m_parent;
}

bool Setting::operator==(const Setting& other)
{
    if(m_parameter.second == other.m_parameter.second)
        return true;
    return false;
}
bool Setting::operator==(const QVariant& otherValue)
{
    if(m_parameter.second == otherValue)
        return true;
    return false;
}
bool Setting::operator!=(const Setting& other)
{
    if(m_parameter.second != other.m_parameter.second)
        return true;
    return false;
}
bool Setting::operator!=(const QVariant& otherValue)
{
    if(m_parameter.second != otherValue)
        return true;
    return false;
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
        if(m_parent->settingExists(name))
        {
            SETTINGS_WARNING_PRETTY << "Unable to change name from" << m_parameter.first
            << " to " << name << "\nsame name already exists in " << m_parent->getName();
            return;
        }
    if(name == "")
    {
        SETTINGS_WARNING_PRETTY << "Unable to change name from" << m_parameter.first
        << " to " << name << "\nnew name is empty";
        return;
    }
    m_parameter.first = name;
    emit nameChanged(m_parameter.first);
}
}
