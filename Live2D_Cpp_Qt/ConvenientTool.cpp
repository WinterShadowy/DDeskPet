#include "ConvenientTool.hpp"

int ConvenientTool::CompareVersion(std::string v1, std::string v2)
{
    // 去掉前面的 'v'
    v1 = v1.substr(1, v1.length() - 1);
	v2 = v2.substr(1, v2.length() - 1);
    // 处理字符串，替换 '.' 为 ' '，方便使用 stringstream 解析
    for (int index = 0; index < v1.length(); ++index)
    {
        if (v1[index] == '.')
        {
            v1[index] = ' ';
        }
    }
    for (int index = 0; index < v2.length(); ++index)
    {
        if (v2[index] == '.')
        {
            v2[index] = ' ';
        }
    }
    int _cur_v_1, _cur_v_2, _cur_v_3, _cur_v_4;
    int _cli_v_1, _cli_v_2, _cli_v_3, _cli_v_4;

    // 处理版本数据，使用 try-catch 捕获异常，避免版本格式异常导致程序崩溃
    try
    {
        std::istringstream _cur_v_iss(v1);
        std::istringstream _cli_v_iss(v2);
        // 处理版本数据
        _cur_v_iss >> _cur_v_1 >> _cur_v_2 >> _cur_v_3 >> _cur_v_4;
        _cli_v_iss >> _cli_v_1 >> _cli_v_2 >> _cli_v_3 >> _cli_v_4;

		//qDebug() << "当前版本：" << _cur_v_1 << "." << _cur_v_2 << "." << _cur_v_3 << "." << _cur_v_4;
		//qDebug() << "比较版本：" << _cli_v_1 << "." << _cli_v_2 << "." << _cli_v_3 << "." << _cli_v_4;

        if (_cur_v_1 != _cli_v_1)
        {
            return (_cur_v_1 < _cli_v_1 ? -1 : 1);
        }
        else if (_cur_v_2 != _cli_v_2)
        {
            return (_cur_v_2 < _cli_v_2 ? -1 : 1);
        }
        else if (_cur_v_3 != _cli_v_3)
        {
            return (_cur_v_3 < _cli_v_3 ? -1 : 1);
        }
        else if (_cur_v_4 != _cli_v_4)
        {
            return (_cur_v_4 < _cli_v_4 ? -1 : 1);
        }
        else
        {
            return 0;
        }
    }
    catch (...)
    {
        return 2;  // 版本格式异常
    }
}

QPoint ConvenientTool::CheckModelWindowOffScreen(QPoint curPos, int nWindowHeight, int nWindowWidth)
{
    // 遍历所有屏幕，检查 curPos 是否在任一屏幕内
    const QList<QScreen*> screens = QGuiApplication::screens();
    for (QScreen* screen : screens) 
    {
        // 屏幕可用几何（排除任务栏等系统保留区域）
        QRect available = screen->availableGeometry();
        // 若左上角在屏幕内，直接返回原位置
        if (available.contains(curPos)) 
        {
            return curPos;
        }
    }

    // 全部 miss → 窗口在屏幕外，挪到主屏幕左下角
    QScreen* primary = QGuiApplication::primaryScreen();
    if (!primary)
        return QPoint(0, 0);  // 极端 fallback

    QRect primaryGeo = primary->availableGeometry();

    // 左下角：x=0, y=屏幕底 - 窗口高度（这里假设窗口高 600，你可改）
    const int windowHeight   = nWindowHeight;  // 你的实际窗口高度
    const int windowWidth    = nWindowWidth;  // 你的实际窗口宽度

    int newX = primaryGeo.left();
    int newY = primaryGeo.bottom() - windowHeight;

    // 防止超出上边界（小屏幕时）
    if (newY < primaryGeo.top())
        newY = primaryGeo.top();

    return QPoint(newX, newY);
}

