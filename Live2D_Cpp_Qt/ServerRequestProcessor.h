
/*
 ============================================================================
 Header: ServerRequestProcessor.h
 Description: 负责处理来自客户端的请求
 Start time: 2025.11.15
 End time: 2026.01.07
 Maintainer: WangSZ
 ============================================================================
*/

#pragma once

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include  <QJsonDocument>
#include "LAppDefine.hpp"
#include "LAppModelMessage.h"
#include "LAppUserInterface.h"

// 记录聊天
QJsonObject RecordChatContent(const QJsonObject& obj);
// 测试
QJsonObject RecordChatContent_Test(const QJsonObject& obj);
// 展示消息在模型
QJsonObject ShowMessage(const QJsonObject& obj);
// 测试
QJsonObject ShowMessage_Test(const QJsonObject& obj);

// 未知消息
QJsonObject UndefinedMessage(const QJsonObject& obj);
// 获取PID
QJsonObject _GetProcessID(const QJsonObject& obj);

void ReturnMsg(const QJsonObject& obj);
void InitAIMsg(const QJsonObject& obj);