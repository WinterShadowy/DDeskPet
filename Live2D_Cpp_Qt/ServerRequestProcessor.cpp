#include "ServerRequestProcessor.h"
#include "LAppChatHistoryManager.h"

QJsonObject RecordChatContent(const QJsonObject& obj)
{
	QJsonObject objBuf = obj;

	QByteArray data = QJsonDocument(objBuf).toJson(QJsonDocument::Compact);
	qDebug() << data;
	QString reqMsgBody = objBuf["message_body"].toString();
	QString returnMsgBody = objBuf["return_msg"].toString();
	qDebug() << "reqMsgBody:" << reqMsgBody << "\nreturnMsgBody:" << returnMsgBody;
	// 保存请求内容与返回消息
	QString ts = QString::number(QDateTime::currentMSecsSinceEpoch());
	bool isSuccess = GetChatHistoryManagerInstance().InsertMessage(
		QString("text"), 
		reqMsgBody,
		QString("User"),
		ts);
	isSuccess = (isSuccess && GetChatHistoryManagerInstance().InsertMessage(
		QString("text"),
		returnMsgBody,
		QString("AI"),
		ts));
	// 插入处理状态
	objBuf.insert("Processing_Status", isSuccess);
	return objBuf;
}

QJsonObject RecordChatContent_Test(const QJsonObject& obj)
{
	QJsonObject objBuf = obj;
	QString reqMsgBody = objBuf["message_body"].toString();
	QString returnMsgBody;
	if (objBuf.contains("return_msg") == false)
	{
		returnMsgBody = "Ciallo～ (∠・ω< )⌒★";
	}
	else
	{
		returnMsgBody = objBuf["return_msg"].toString();
	}
	// 保存请求内容与返回消息
	QString ts = QString::number(QDateTime::currentMSecsSinceEpoch());
	bool isSuccess = GetChatHistoryManagerInstance().InsertMessage(
		QString("text"),
		reqMsgBody,
		QString("User"),
		ts);
	isSuccess = (isSuccess && GetChatHistoryManagerInstance().InsertMessage(
		QString("text"),
		returnMsgBody,
		QString("AI"),
		ts
	));
	// 插入处理状态
	objBuf.insert("Processing_Status", isSuccess);
	return objBuf;
}

QJsonObject ShowMessage(const QJsonObject& obj)
{
	QJsonObject objBuf = obj;
	QString messageBody;
	unsigned int timeout_ms = 3000;
	messageBody = obj["message_body"].toString();
	if (messageBody.isEmpty())
		messageBody = "需要显示的消息为空";
	if (obj.contains("timeout_ms"))  // 超时时间
		timeout_ms = obj["timeout_ms"].toInt() > 300000 ? 300000 : obj["timeout_ms"].toInt();
	QJsonObject qjson_Style;
	if (obj.contains("about_show_message"))
	{
		qjson_Style = ConvenientTool::GetValueFromQJson<QJsonObject>(obj, "about_show_message", "text_display_mode");
	}
	else
	{
		qjson_Style.insert("description", "文本显示模式");

		QJsonObject fixedSpeed;
		fixedSpeed.insert("use", true);
		fixedSpeed.insert("interval", 233);
		fixedSpeed.insert("description", "控制文本以固定速度显示, interval的值为每个字显示的间隔");
		qjson_Style.insert("fixed_speed", fixedSpeed);

		QJsonObject randomSpeed;
		randomSpeed.insert("use", false);
		randomSpeed.insert("range_min", 100);
		randomSpeed.insert("range_max", 500);
		randomSpeed.insert("description", "控制文本以随机速度显示，range_max/min的值为随机的时间范围，单位：ms");
		qjson_Style.insert("random_speed", randomSpeed);

		QJsonObject stream;
		stream.insert("use", false);
		stream.insert("description", "文本以流式显示，否则直接显示文本");
		qjson_Style.insert("stream", stream);

		QJsonObject fixed_time;
		fixed_time.insert("use", false);
		fixed_time.insert("timeout", 3000);
		fixed_time.insert("description", "文本以固定时间显示");
		qjson_Style.insert("fixed_time", fixed_time);

		QJsonObject	audio_duration;
		audio_duration.insert("use", false);
		audio_duration.insert("duration", 3000);
		audio_duration.insert("description", "文本以音频时长显示");
		qjson_Style.insert("audio_duration", audio_duration);
	}

	// 显示消息
	ModelWindowPromptWindow::ShowMessageOnModelWindow(ModelWindowPromptWindow::Style_V_3_0, messageBody, qjson_Style);
	objBuf.insert("Processing_Status", true);
	return objBuf;
}

QJsonObject ShowMessage_Test(const QJsonObject& obj)
{
	QJsonObject objBuf = obj;
	QString messageBody = "这是一条测试消息";
	unsigned int timeout_ms = 3000;

	ModelWindowPromptWindow::ShowMessageOnModelWindow(11, messageBody, timeout_ms, true);
	objBuf.insert("Processing_Status", true);
	return objBuf;
}

QJsonObject UndefinedMessage(const QJsonObject& obj)
{
	QJsonObject objBuf = obj;
	objBuf.insert("Processing_Status", false);
	objBuf.insert("Undefined_Message", true);
	return objBuf;
}

QJsonObject _GetProcessID(const QJsonObject& obj)
{
	QJsonObject objBuf = obj;
	objBuf.insert("ProcessID", QJsonValue::fromVariant(UserInterface_GetProcessID()));
	objBuf.insert("Processing_Status", true);
	return objBuf;
}

void ReturnMsg(const QJsonObject& obj)
{
	QString message;
	if (obj["message_body"].toBool())
		message = "服务端正常启动\n等待AI服务启动...";
	else
		message = "服务启动失败\n部分功能可能无法使用！";
	ModelWindowPromptWindow::ShowMessageOnModelWindow(11, message, 4000, true);
	return;
}

void InitAIMsg(const QJsonObject& obj)
{
	QString message;
	if (obj.contains("init_id"))
	{
		message = QString("AI服务启动信息:\nID:%1\n").arg(obj["init_id"].toInt());
	}
	else
	{
		message = QString("AI服务启动信息:\nID: 未知\n");
	}
	if (obj.contains("init_status"))
	{
		message = message + QString("status: %1").arg(obj["init_status"].toBool() ? "Success" : "Failed");
	}
	else
	{
		message = message + QString("status: 未知");
	}
	ModelWindowPromptWindow::ShowMessageOnModelWindow(11, message, 4000, true);
	return;
}
