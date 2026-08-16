#include "mainwindow.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      currentLang(AppLanguage::English),
      isBigEndian(true)
{
    setupUI();
    updateLanguage(); // 初始渲染英文界面
}

MainWindow::~MainWindow(){}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    resize(820, 580);

    pathLineEdit = new QLineEdit(this);
    pathLineEdit->setPlaceholderText("Select .txt file...");
    browseBtn = new QPushButton(this);

    frameLineEdit = new QLineEdit(this);
    frameLineEdit->setText("A5 BE");

    offsetLineEdit = new QLineEdit(this);
    offsetLineEdit->setText("5");

    lengthLineEdit = new QLineEdit(this);
    lengthLineEdit->setText("2"); // 默认提取字节数 2

    forwardBtn = new QPushButton(this);
    backwardBtn = new QPushButton(this);
    endianBtn = new QPushButton(this);

    // 下拉框选择语言
    langComboBox = new QComboBox(this);
    langComboBox->addItem("English", static_cast<int>(AppLanguage::English));
    langComboBox->addItem("中文", static_cast<int>(AppLanguage::Chinese));
    langComboBox->addItem("Deutsch", static_cast<int>(AppLanguage::German));
    langComboBox->addItem("Français", static_cast<int>(AppLanguage::French));
    langComboBox->addItem("日本語", static_cast<int>(AppLanguage::Japanese));
    langComboBox->addItem("Русский", static_cast<int>(AppLanguage::Russian));
    langComboBox->addItem("Español", static_cast<int>(AppLanguage::Spanish));
    langComboBox->addItem("العربية", static_cast<int>(AppLanguage::Arabic));
    langComboBox->setCurrentIndex(0); // 默认选择 English

    resultTextEdit = new QTextEdit(this);
    resultTextEdit->setReadOnly(true);

    pathLabel = new QLabel(this);
    frameLabel = new QLabel(this);
    offsetLabel = new QLabel(this);
    lengthLabel = new QLabel(this);
    resultLabel = new QLabel(this);

    QHBoxLayout *fileLayout = new QHBoxLayout();
    fileLayout->addWidget(pathLabel);
    fileLayout->addWidget(pathLineEdit);
    fileLayout->addWidget(browseBtn);

    QHBoxLayout *paramLayout1 = new QHBoxLayout();
    paramLayout1->addWidget(frameLabel);
    paramLayout1->addWidget(frameLineEdit);
    paramLayout1->addWidget(offsetLabel);
    paramLayout1->addWidget(offsetLineEdit);

    QHBoxLayout *paramLayout2 = new QHBoxLayout();
    paramLayout2->addWidget(lengthLabel);
    paramLayout2->addWidget(lengthLineEdit);
    paramLayout2->addWidget(forwardBtn);
    paramLayout2->addWidget(backwardBtn);

    QHBoxLayout *topControlLayout = new QHBoxLayout();
    topControlLayout->addWidget(endianBtn);
    topControlLayout->addWidget(langComboBox);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addLayout(topControlLayout);
    mainLayout->addLayout(fileLayout);
    mainLayout->addLayout(paramLayout1);
    mainLayout->addLayout(paramLayout2);
    mainLayout->addWidget(resultLabel);
    mainLayout->addWidget(resultTextEdit);

    connect(browseBtn, &QPushButton::clicked, this, &MainWindow::onBrowseFile);
    connect(forwardBtn, &QPushButton::clicked, this, &MainWindow::onSearchForward);
    connect(backwardBtn, &QPushButton::clicked, this, &MainWindow::onSearchBackward);
    connect(endianBtn, &QPushButton::clicked, this, &MainWindow::onToggleEndianness);
    connect(langComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onLanguageChanged);
}

void MainWindow::onLanguageChanged(int index) {
    currentLang = static_cast<AppLanguage>(langComboBox->itemData(index).toInt());

    // 阿拉伯语自动启用从右向左 (RTL) 的排版，其余使用 LTR
    if (currentLang == AppLanguage::Arabic) {
        qApp->setLayoutDirection(Qt::RightToLeft);
    } else {
        qApp->setLayoutDirection(Qt::LeftToRight);
    }

    updateLanguage();
}

void MainWindow::updateLanguage() {
    // 界面多语言文本翻译映射
    switch (currentLang) {
    case AppLanguage::Chinese:
        setWindowTitle("16进制帧数据解析工具 (Auth:ZSP)");
        pathLabel->setText("文件路径:");
        browseBtn->setText("浏览文件");
        frameLabel->setText("帧结构(Hex):");
        offsetLabel->setText("距离字节数:");
        lengthLabel->setText("提取字节数:");
        forwardBtn->setText("向前查找");
        backwardBtn->setText("向后查找");
        endianBtn->setText(isBigEndian ? "模式: 大端 (切换)" : "模式: 小端 (切换)");
        resultLabel->setText("解析结果（含10进制转换）：");
        break;
    case AppLanguage::German:
        setWindowTitle("Hex-Frame-Parser-Tool (Auth:ZSP)");
        pathLabel->setText("Dateipfad:");
        browseBtn->setText("Durchsuchen");
        frameLabel->setText("Frame-Muster:");
        offsetLabel->setText("Abstand:");
        lengthLabel->setText("Extr. Byte-Anzahl:");
        forwardBtn->setText("Vorwärts suchen");
        backwardBtn->setText("Rückwärts suchen");
        endianBtn->setText(isBigEndian ? "Modus: Big-Endian (Wechseln)" : "Modus: Little-Endian (Wechseln)");
        resultLabel->setText("Analyseergebnisse (Dezimal konvertiert):");
        break;
    case AppLanguage::French:
        setWindowTitle("Outil d'analyse de trame Hex (Auth:ZSP)");
        pathLabel->setText("Chemin du fichier:");
        browseBtn->setText("Parcourir");
        frameLabel->setText("Motif de trame:");
        offsetLabel->setText("Distance:");
        lengthLabel->setText("Octets à extraire:");
        forwardBtn->setText("Chercher en avant");
        backwardBtn->setText("Chercher en arrière");
        endianBtn->setText(isBigEndian ? "Mode: Gros-boutiste (Changer)" : "Mode: Petit-boutiste (Changer)");
        resultLabel->setText("Résultats d'analyse (Convertis en décimal):");
        break;
    case AppLanguage::Japanese:
        setWindowTitle("Hexフレーム解析ツール (Auth:ZSP)");
        pathLabel->setText("ファイルパス:");
        browseBtn->setText("参照");
        frameLabel->setText("フレーム構造:");
        offsetLabel->setText("距離バイト数:");
        lengthLabel->setText("抽出バイト数:");
        forwardBtn->setText("前方検索");
        backwardBtn->setText("後方検索");
        endianBtn->setText(isBigEndian ? "モード: ビッグエンディアン (切替)" : "モード: リトルエンディアン (切替)");
        resultLabel->setText("解析结果（10進数変換済み）：");
        break;
    case AppLanguage::Russian:
        setWindowTitle("Парсер шестнадцатеричных кадров (Auth:ZSP)");
        pathLabel->setText("Путь к файлу:");
        browseBtn->setText("Обзор");
        frameLabel->setText("Шаблон кадра:");
        offsetLabel->setText("Смещение (байт):");
        lengthLabel->setText("Извлечь байтов:");
        forwardBtn->setText("Искать вперед");
        backwardBtn->setText("Искать назад");
        endianBtn->setText(isBigEndian ? "Режим: Big-Endian (Сменить)" : "Режим: Little-Endian (Сменить)");
        resultLabel->setText("Результаты анализа (в десятичном виде):");
        break;
    case AppLanguage::Spanish:
        setWindowTitle("Analizador de tramas Hex (Auth:ZSP)");
        pathLabel->setText("Ruta del archivo:");
        browseBtn->setText("Examinar");
        frameLabel->setText("Patrón de trama:");
        offsetLabel->setText("Distancia:");
        lengthLabel->setText("Bytes a extraer:");
        forwardBtn->setText("Buscar adelante");
        backwardBtn->setText("Buscar atrás");
        endianBtn->setText(isBigEndian ? "Modo: Big-Endian (Cambiar)" : "Modo: Little-Endian (Cambiar)");
        resultLabel->setText("Resultados del análisis (Decimal):");
        break;
    case AppLanguage::Arabic:
        setWindowTitle("أداة تحليل إطارات ستة عشرية (Auth:ZSP)");
        pathLabel->setText("مسار الملف:");
        browseBtn->setText("استعراض");
        frameLabel->setText("نمط الإطار:");
        offsetLabel->setText("المسافة (بايت):");
        lengthLabel->setText("عدد البايتات:");
        forwardBtn->setText("بحث للأمام");
        backwardBtn->setText("بحث للخلف");
        endianBtn->setText(isBigEndian ? "الوضع: Big-Endian (تبديل)" : "الوضع: Little-Endian (تبديل)");
        resultLabel->setText("نتائج التحليل (تحويل عشري):");
        break;
    case AppLanguage::English:
    default:
        setWindowTitle("Hex Frame Parser Tool (Auth:ZSP)");
        pathLabel->setText("File Path:");
        browseBtn->setText("Browse");
        frameLabel->setText("Frame Pattern:");
        offsetLabel->setText("Distance:");
        lengthLabel->setText("Extract Bytes:");
        forwardBtn->setText("Search Forward");
        backwardBtn->setText("Search Backward");
        endianBtn->setText(isBigEndian ? "Mode: Big-Endian (Switch)" : "Mode: Little-Endian (Switch)");
        resultLabel->setText("Parsing Results (Decimal Converted):");
        break;
    }

    // 重绘解析结果文本框中的历史日志（实现日志无缝中英法德日俄西阿同步翻译）
    resultTextEdit->clear();
    if (m_resultCache.isEmpty()) return;

    for (const auto &item : m_resultCache) {
        QString dirStr;
        switch (currentLang) {
            case AppLanguage::Chinese:  dirStr = item.isForward ? "向前查找" : "向后查找"; break;
            case AppLanguage::German:   dirStr = item.isForward ? "Vorwärts" : "Rückwärts"; break;
            case AppLanguage::French:   dirStr = item.isForward ? "En avant" : "En arrière"; break;
            case AppLanguage::Japanese: dirStr = item.isForward ? "前方" : "後方"; break;
            case AppLanguage::Russian:  dirStr = item.isForward ? "Вперед" : "Назад"; break;
            case AppLanguage::Spanish:  dirStr = item.isForward ? "Adelante" : "Atrás"; break;
            case AppLanguage::Arabic:   dirStr = item.isForward ? "للأمام" : "للخلف"; break;
            default:                    dirStr = item.isForward ? "Forward" : "Backward"; break;
        }

        QString modeStr = item.isBigEndianAtParse ? "Big-Endian" : "Little-Endian";

        QString logMsg;
        if (currentLang == AppLanguage::Chinese) {
            logMsg = QString("【%1 第 %2 帧】 位置: %3 | Hex: [ %4 ] -> 10进制(%5): %6")
                     .arg(dirStr).arg(item.frameIndex).arg(item.matchPos).arg(item.hexStr).arg(modeStr).arg(item.decStr);
        } else if (currentLang == AppLanguage::Arabic) {
            logMsg = QString("[%1 الإطار %2] الموقع: %3 | Hex: [ %4 ] -> عشري(%5): %6")
                     .arg(dirStr).arg(item.frameIndex).arg(item.matchPos).arg(item.hexStr).arg(modeStr).arg(item.decStr);
        } else {
            logMsg = QString("[%1 Frame %2] Pos: %3 | Hex: [ %4 ] -> Dec(%5): %6")
                     .arg(dirStr).arg(item.frameIndex).arg(item.matchPos).arg(item.hexStr).arg(modeStr).arg(item.decStr);
        }

        resultTextEdit->append(logMsg);
    }
}

void MainWindow::onBrowseFile() {
    QString currentPath = pathLineEdit->text().trimmed();
    QString defaultDir = currentPath.isEmpty() ? QDir::currentPath() : QFileInfo(currentPath).absolutePath();

    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Select Text File",
        defaultDir,
        "Text Files (*.txt);;All Files (*.*)"
    );

    if (!fileName.isEmpty()) {
        pathLineEdit->setText(fileName);
    }
}

QByteArray MainWindow::parseHexInput(const QString &hexStr) {
    QString cleanStr = hexStr;
    cleanStr.remove(' ');
    return QByteArray::fromHex(cleanStr.toUtf8());
}

void MainWindow::onSearchForward()  { processSearch(true); }
void MainWindow::onSearchBackward() { processSearch(false); }

void MainWindow::processSearch(bool isForward) {
    resultTextEdit->clear();
    m_resultCache.clear();

    QString filePath = pathLineEdit->text().trimmed();
    QByteArray framePattern = parseHexInput(frameLineEdit->text().trimmed());

    bool okOffset, okLength;
    int distance = offsetLineEdit->text().trimmed().toInt(&okOffset);
    int extractLength = lengthLineEdit->text().trimmed().toInt(&okLength);

    if (filePath.isEmpty() || framePattern.isEmpty() || !okOffset || !okLength || distance < 0 || extractLength <= 0) {
        QMessageBox::warning(this, "Warning", "Please verify inputs!");
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Cannot open file!");
        return;
    }

    QString fileContent = QString::fromUtf8(file.readAll()).simplified().toUpper();
    file.close();

    QStringList hexTokens = fileContent.split(' ', QString::SkipEmptyParts);
    QByteArray fileBytes;
    for (const QString &token : hexTokens) {
        bool ok;
        uchar val = static_cast<uchar>(token.toUInt(&ok, 16));
        if (ok) fileBytes.append(static_cast<char>(val));
    }

    int lastIndex = 0;
    int frameCount = 0;
    int patternSize = framePattern.size();

    while (true) {
        int pos = fileBytes.indexOf(framePattern, lastIndex);
        if (pos == -1) break;

        int targetIndex = isForward ? (pos - distance) : (pos + patternSize + distance);

        if (targetIndex >= 0 && (targetIndex + extractLength) <= fileBytes.size()) {
            frameCount++;
            lastExtractedBytes = fileBytes.mid(targetIndex, extractLength);

            QString hexStr = lastExtractedBytes.toHex(' ').toUpper();

            // 计算 10 进制表示
            QString decStr;
            if (extractLength == 1) {
                decStr = QString::number(static_cast<uchar>(lastExtractedBytes.at(0)));
            } else if (extractLength == 2) {
                quint16 val = isBigEndian ? qFromBigEndian<quint16>(reinterpret_cast<const uchar*>(lastExtractedBytes.constData()))
                                          : qFromLittleEndian<quint16>(reinterpret_cast<const uchar*>(lastExtractedBytes.constData()));
                decStr = QString::number(val);
            } else if (extractLength == 4) {
                quint32 val = isBigEndian ? qFromBigEndian<quint32>(reinterpret_cast<const uchar*>(lastExtractedBytes.constData()))
                                          : qFromLittleEndian<quint32>(reinterpret_cast<const uchar*>(lastExtractedBytes.constData()));
                decStr = QString::number(val);
            } else {
                QStringList byteDecs;
                for (int i = 0; i < lastExtractedBytes.size(); ++i) {
                    byteDecs.append(QString::number(static_cast<uchar>(lastExtractedBytes.at(i))));
                }
                decStr = "[" + byteDecs.join(", ") + "]";
            }

            ParseResultItem item;
            item.isForward = isForward;
            item.frameIndex = frameCount;
            item.matchPos = pos;
            item.hexStr = hexStr;
            item.decStr = decStr;
            item.isBigEndianAtParse = isBigEndian;
            m_resultCache.append(item);

        } else {
            resultTextEdit->append(QString("Pattern at %1, but target index out of range.").arg(pos));
        }

        lastIndex = pos + patternSize;
    }

    updateLanguage();

    if (frameCount == 0) {
        resultTextEdit->append("No matching frame found.");
    }
}

void MainWindow::onToggleEndianness() {
    isBigEndian = !isBigEndian;
    updateLanguage();
}

