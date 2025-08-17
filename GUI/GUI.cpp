#include "GUI.h"
#include <QInputDialog>
#include <QEventLoop>
#include <QScreen>
#include <QApplication>
#include <QFont>
#include <QPainter>
#include <QPainterPath>
#include <QIcon>

// Реализация AnimatedButton
AnimatedButton::AnimatedButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent), m_fillProgress(0.0f), isDarkTheme(false) {
    fillAnimation = new QPropertyAnimation(this, "fillProgress", this);
    fillAnimation->setDuration(300);
    fillAnimation->setEasingCurve(QEasingCurve::OutQuad);

    setMouseTracking(true);
    setFixedSize(300, 300);
    updateStyle(false);
}

float AnimatedButton::fillProgress() const {
    return m_fillProgress;
}

void AnimatedButton::setFillProgress(float progress) {
    m_fillProgress = progress;
    update();
}

void AnimatedButton::updateStyle(bool darkTheme) {
    isDarkTheme = darkTheme;

    QString style = QString(R"(
        QPushButton {
            border: 3px solid %1;
            border-radius: 15px;
            padding: 20px;
            font-weight: bold;
            color: %2;
            background-color: %3;
        }
    )").arg(darkTheme ? "#444" : "#555")
      .arg(darkTheme ? "#eee" : "#333")
      .arg(darkTheme ? "#333" : "#f0f0f0");

    setStyleSheet(style);
}

void AnimatedButton::paintEvent(QPaintEvent *event) {
    QPushButton::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (m_fillProgress > 0.0f) {
        QPainterPath path;
        path.addRoundedRect(rect(), 15, 15);

        QRect fillRect = rect();
        fillRect.setWidth(rect().width() * m_fillProgress);

        QPainterPath fillPath;
        fillPath.addRoundedRect(fillRect, 15, 15);

        painter.setClipPath(fillPath);
        painter.fillPath(path, QColor(255, 255, 255, isDarkTheme ? 60 : 120));
        painter.setClipping(false);
    }

    painter.setPen(isDarkTheme ? Qt::white : Qt::black);
    painter.drawText(rect(), Qt::AlignCenter, text());
}

void AnimatedButton::enterEvent(QEnterEvent *event) {
    fillAnimation->setStartValue(0.0f);
    fillAnimation->setEndValue(1.0f);
    fillAnimation->start();
    QPushButton::enterEvent(event);
}

void AnimatedButton::leaveEvent(QEvent *event) {
    fillAnimation->setStartValue(1.0f);
    fillAnimation->setEndValue(0.0f);
    fillAnimation->start();
    QPushButton::leaveEvent(event);
}

// Реализация GUI
GUI::GUI(QWidget *parent) : QMainWindow(parent), game(nullptr),
    easyButton(nullptr), mediumButton(nullptr), hardButton(nullptr),
    customButton(nullptr), themeButton(nullptr), darkTheme(false) {

    centralWidget = new QWidget(this);
    centralWidget->setMouseTracking(true);
    setCentralWidget(centralWidget);
    resize(1000, 800);

    // Кнопка переключения темы
    themeButton = new QPushButton("🌙", centralWidget);
    themeButton->setFixedSize(50, 50);
    themeButton->setStyleSheet(R"(
        QPushButton {
            border: 2px solid #555;
            border-radius: 25px;
            font-size: 20px;
        }
        QPushButton:hover {
            background-color: #ddd;
        }
    )");
    connect(themeButton, &QPushButton::clicked, this, &GUI::toggleTheme);
}

GUI::GUI(QWidget *parent, Game *game) : GUI(parent) {
    this->game = game;
}

void GUI::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    calculateButtonPositions();

    // Позиция кнопки темы
    if (themeButton) {
        themeButton->move(20, 20);
    }
}

void GUI::calculateButtonPositions() {
    if (!easyButton) return;

    int totalWidth = 2 * BUTTON_SIZE + BUTTON_SPACING;
    int totalHeight = 2 * BUTTON_SIZE + BUTTON_SPACING;

    int startX = (width() - totalWidth) / 2;
    int startY = (height() - totalHeight) / 2;

    easyButton->move(startX, startY);
    mediumButton->move(startX + BUTTON_SIZE + BUTTON_SPACING, startY);
    hardButton->move(startX, startY + BUTTON_SIZE + BUTTON_SPACING);
    customButton->move(startX + BUTTON_SIZE + BUTTON_SPACING, startY + BUTTON_SIZE + BUTTON_SPACING);
}

void GUI::toggleTheme() {
    darkTheme = !darkTheme;
    updateTheme();

    // Обновляем иконку кнопки
    themeButton->setText(darkTheme ? "☀️" : "🌙");
}

void GUI::updateTheme() {
    // Устанавливаем цвет фона
    centralWidget->setStyleSheet(QString("background-color: %1;").arg(darkTheme ? "#222" : "#f5f5f5"));

    // Обновляем стиль кнопок
    if (easyButton) easyButton->updateStyle(darkTheme);
    if (mediumButton) mediumButton->updateStyle(darkTheme);
    if (hardButton) hardButton->updateStyle(darkTheme);
    if (customButton) customButton->updateStyle(darkTheme);

    // Обновляем стиль кнопки темы
    if (themeButton) {
        themeButton->setStyleSheet(QString(R"(
            QPushButton {
                border: 2px solid %1;
                border-radius: 25px;
                font-size: 20px;
                background-color: %2;
            }
            QPushButton:hover {
                background-color: %3;
            }
        )").arg(darkTheme ? "#666" : "#555")
          .arg(darkTheme ? "#333" : "#f0f0f0")
          .arg(darkTheme ? "#444" : "#ddd"));
    }
}

void GUI::getDifficulty(unsigned int &number_of_rows, unsigned int &number_of_cols, unsigned int &number_of_mines) {
    rowsPtr = &number_of_rows;
    colsPtr = &number_of_cols;
    minesPtr = &number_of_mines;

    if (easyButton) easyButton->deleteLater();
    if (mediumButton) mediumButton->deleteLater();
    if (hardButton) hardButton->deleteLater();
    if (customButton) customButton->deleteLater();

    easyButton = new AnimatedButton("ЛЁГКИЙ\n\n9×9 клеток\n10 мин", centralWidget);
    mediumButton = new AnimatedButton("СРЕДНИЙ\n\n16×16 клеток\n40 мин", centralWidget);
    hardButton = new AnimatedButton("ТЯЖЁЛЫЙ\n\n30×16 клеток\n99 мин", centralWidget);
    customButton = new AnimatedButton("НАСТРОЙКИ\n\nВыбрать параметры", centralWidget);

    // Обновляем стили согласно текущей теме
    easyButton->updateStyle(darkTheme);
    mediumButton->updateStyle(darkTheme);
    hardButton->updateStyle(darkTheme);
    customButton->updateStyle(darkTheme);

    QFont font;
    font.setPointSize(16);
    font.setBold(true);

    easyButton->setFont(font);
    mediumButton->setFont(font);
    hardButton->setFont(font);
    customButton->setFont(font);

    connect(easyButton, &QPushButton::clicked, this, &GUI::handleEasy);
    connect(mediumButton, &QPushButton::clicked, this, &GUI::handleMedium);
    connect(hardButton, &QPushButton::clicked, this, &GUI::handleHard);
    connect(customButton, &QPushButton::clicked, this, &GUI::handleCustom);

    calculateButtonPositions();

    easyButton->show();
    mediumButton->show();
    hardButton->show();
    customButton->show();

    QEventLoop loop;
    bool choiceMade = false;
    choiceMadePtr = &choiceMade;
    connect(this, &GUI::difficultyChosen, &loop, &QEventLoop::quit);
    loop.exec();
}

// Остальные методы без изменений
void GUI::print(std::string message) {
    QMessageBox::information(this, "Message", QString::fromStdString(message));
}

void GUI::printField() {
    // Implementation for printField will be added later
}

Command GUI::getCommand() {
    return Command::Attack;
}

std::tuple<int, int> GUI::getCoords() const {
    return std::make_tuple(1, 1);
}

void GUI::handleEasy() {
    *rowsPtr = 9;
    *colsPtr = 9;
    *minesPtr = 10;
    *choiceMadePtr = true;
    emit difficultyChosen();
}

void GUI::handleMedium() {
    *rowsPtr = 16;
    *colsPtr = 16;
    *minesPtr = 40;
    *choiceMadePtr = true;
    emit difficultyChosen();
}

void GUI::handleHard() {
    *rowsPtr = 16;
    *colsPtr = 30;
    *minesPtr = 99;
    *choiceMadePtr = true;
    emit difficultyChosen();
}

void GUI::handleCustom() {
    QDialog dialog(this);
    dialog.setWindowTitle("Пользовательские настройки");
    dialog.setFixedSize(400, 300);

    // Настройка стиля диалога согласно теме
    dialog.setStyleSheet(QString(R"(
        QDialog {
            background-color: %1;
            color: %2;
        }
        QLabel {
            color: %2;
        }
    )").arg(darkTheme ? "#333" : "#f5f5f5")
      .arg(darkTheme ? "#eee" : "#333"));

    QFormLayout form(&dialog);

    QSpinBox *widthSpin = new QSpinBox(&dialog);
    widthSpin->setRange(5, 50);
    widthSpin->setValue(10);

    QSpinBox *heightSpin = new QSpinBox(&dialog);
    heightSpin->setRange(5, 50);
    heightSpin->setValue(10);

    QSpinBox *minesSpin = new QSpinBox(&dialog);
    minesSpin->setRange(1, widthSpin->value() * heightSpin->value() - 1);
    minesSpin->setValue(20);

    connect(widthSpin, QOverload<int>::of(&QSpinBox::valueChanged), [=](int value) {
        minesSpin->setMaximum(value * heightSpin->value() - 1);
    });

    connect(heightSpin, QOverload<int>::of(&QSpinBox::valueChanged), [=](int value) {
        minesSpin->setMaximum(widthSpin->value() * value - 1);
    });

    form.addRow("Ширина (колонки):", widthSpin);
    form.addRow("Высота (строки):", heightSpin);
    form.addRow("Количество мин:", minesSpin);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                              Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        *colsPtr = widthSpin->value();
        *rowsPtr = heightSpin->value();
        *minesPtr = minesSpin->value();
        *choiceMadePtr = true;
        emit difficultyChosen();
    }
}

void GUI::setGame(Game *game) {
    this->game = game;
}