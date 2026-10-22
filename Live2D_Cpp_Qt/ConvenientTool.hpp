#pragma once

// 禁min和max宏函数
#define NOMINMAX

#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <sstream>
#include <string>
#include <QPoint>
#include <QScreen>
#include <QRect>
#include <type_traits>
#include <limits>
#include <cmath>
#include "nlohmann/json.hpp"

namespace ConvenientTool_Impl {
    // 默认值 trait，可针对自定义类型扩展
    template <typename T>
    struct DefaultValue {
        static T value()
        {
            return T{};
        }
    };
    /*
    *
    *   修改日期：2025/12/08
    *	修改者： WangSZ
    *	用处：获取值
    *	函数：template <typename _T>
              _T _GetValue(const QJsonObject& obj, const QString& key)
    */
    // 递归终止：只剩一个键，取值
    template <typename _T>
    _T _GetValue(const QJsonObject& obj, const QString& key)
    {
        if (!obj.contains(key))
        {
            return DefaultValue<_T>::value();
        }
        const QJsonValue jv = obj[key];
        // 支持直接返回 QJsonObject / QJsonArray
        if constexpr (std::is_same_v<_T, QJsonObject>)
        {
            return jv.toObject();
        }
        else if constexpr (std::is_same_v<_T, QJsonArray>)
        {
            return jv.toArray();
        }
        else
        {
            return jv.toVariant().value<_T>();  // int/double/bool/QString 等
        }
    }
    template <typename _T, typename... Args>
    _T _GetValue(const QJsonObject& obj, const QString& first, Args... rest)
    {
        if (!obj.contains(first))
        {
            return DefaultValue<_T>::value();
        }
        const QJsonValue jv = obj[first];
        if (!jv.isObject())                 // 中间层必须是 object
        {
            return DefaultValue<_T>::value();
        }
        return ConvenientTool_Impl::_GetValue<_T>(jv.toObject(), rest...);
    }
    /*
    *
    *   修改日期：2025/12/08
    *	修改者： WangSZ
    *	用处：设置Json中某个键的值
    *	函数：template <typename _T, typename... Args>
              _T GetValueFromQJson(const QJsonObject& qJson, Args... _args)
    */
    template <typename T>
    bool _SetValue(QJsonObject& obj, const T& val, const QString& key)
    {
        if constexpr (std::is_same_v<T, QJsonObject>)
        {
            obj[key] = val;
        }
        else if constexpr (std::is_same_v<T, QJsonArray>)
        {
            obj[key] = val;
        }
        else
        {
            obj[key] = QJsonValue::fromVariant(QVariant::fromValue(val));
        }
        return true;
    }
    //递归中间层
    template <typename T, typename... Args>
    bool _SetValue(QJsonObject& obj, const T& val, const QString& first, Args... rest)
    {
        // 当前层还没有这个 key，或者已有但不是 object，建一个空 object
        if (!obj.contains(first) || !obj[first].isObject())
        {
            obj[first] = QJsonObject{};
        }
        QJsonObject sub = obj[first].toObject();
        bool state = _SetValue(sub, val, rest...);
        obj[first] = std::move(sub);
        return state;
    }
}

// 对外接口
namespace ConvenientTool {
    /*
    *
    *   修改日期：2025/12/07
    *	修改者： WangSZ
    *	用处：从Json中获取值
    *	函数：template <typename _T, typename... Args>
              _T GetValueFromQJson(const QJsonObject& qJson, Args... _args)
    */

    template <typename _T, typename... Args>
    _T GetValueFromQJson(const QJsonObject& qJson, Args... _args)
    {
        static_assert((std::is_convertible_v<Args, QString> && ...),
            "All keys must be convertible to QString");
        return ConvenientTool_Impl::_GetValue<_T>(qJson, QString(_args)...);
    }

    /*
     ============================================================================
     Function: SetQJsonValue
     Description: 设置Json中的某个值
     Parameters:
         - QJsonObject& root: Json对象
     Return: <返回值描述>
     ============================================================================
    */
    template <typename _T, typename... Args>
    bool SetQJsonValue(QJsonObject& root, const _T& _value, Args... _keys)
    {
        static_assert((std::is_convertible_v<Args, QString> && ...),
            "All keys must be convertible to QString");
        return ConvenientTool_Impl::_SetValue(root, _value, QString(_keys)...);
    }

    /*
    ============================================================================
    Function: CompareVersion
    Description: 版本比较
    Parameters:
        - const QString& v1: 版本1 - 当前最新版本
	    - const QString& v2: 版本2 - 客户端版本
    Return: 返回一个整数，表示比较结果：-1（v1 < v2），0（v1 == v2），1（v1 > v2），2（异常）
    ============================================================================
    */
    int CompareVersion(std::string v1, std::string v2);

    /*
     ============================================================================
     Function: CheckModelWindowOffScreen
	 Description: 检查模型窗口是否超出屏幕边界，如果是，则调整位置使其完全显示在屏幕内
     Parameters:
		 - QPoint curPos: 当前窗口位置/配置文件中保存的位置
     Return: 
     ============================================================================
    */
    QPoint CheckModelWindowOffScreen(QPoint curPos, int nWindowHeight, int nWindowWidth);
}