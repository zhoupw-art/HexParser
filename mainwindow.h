#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QByteArray>
#include <QtEndian>
#include <QFileInfo>
#include <QVector>

// 语言枚举定义（支持8种语言）
enum class AppLanguage {
    English = 0,
    Chinese,
    German,
    French,
    Japanese,
    Russian,
    Spanish,
    Arabic
};

// 解析结果缓存结构体
struct ParseResultItem {
    bool isForward;        // 查找方向
    int frameIndex;        // 帧序号
    int matchPos;          // 匹配位置
    QString hexStr;        // 16进制字符串
    QString decStr;        // 10进制字符串
    bool isBigEndianAtParse;// 解析时的端序
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onBrowseFile();
    void onSearchForward();
    void onSearchBackward();
    void onToggleEndianness();
    void onLanguageChanged(int index); // 下拉框语言切换槽

private:
    void setupUI();
    void updateLanguage();
    void processSearch(bool isForward);
    QByteArray parseHexInput(const QString &hexStr);

    // 控件定义
    QLineEdit *pathLineEdit;
    QPushButton *browseBtn;

    QLineEdit *frameLineEdit;
    QLineEdit *offsetLineEdit;
    QLineEdit *lengthLineEdit;

    QPushButton *forwardBtn;
    QPushButton *backwardBtn;
    QPushButton *endianBtn;
    QComboBox   *langComboBox;  // 多语言下拉选择框

    QTextEdit *resultTextEdit;

    QLabel *pathLabel;
    QLabel *frameLabel;
    QLabel *offsetLabel;
    QLabel *lengthLabel;
    QLabel *resultLabel;

    // 状态控制变量
    AppLanguage currentLang;    // 当前选中语言
    bool isBigEndian;           // 当前大小端状态
    QByteArray lastExtractedBytes;

    // 解析结果数据缓存
    QVector<ParseResultItem> m_resultCache;
};
#endif // MAINWINDOW_H
