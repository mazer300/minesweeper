#include "GUI.h"
#include <QInputDialog>
#include <QEventLoop>
#include <QScreen>
#include <QApplication>
#include <QFont>
#include <QPainter>
#include <QPainterPath>
#include <QIcon>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QSizePolicy>
#include <QSpacerItem>

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

AnimatedButton::~AnimatedButton() {
    delete fillAnimation;
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
    customButton(nullptr), themeButton(nullptr), gridLayout(nullptr),
    restartButton(nullptr), exitButton(nullptr), mineCounterLabel(nullptr),
    commandLoop(nullptr), gameEnded(false), endGameDialog(nullptr),
    gameResultLabel(nullptr) {

    // Инициализация настроек
    settings = new QSettings("Minesweeper", "ThemeSettings", this);

    // Загрузка сохраненной темы
    loadTheme();

    centralWidget = new QWidget(this);
    centralWidget->setMouseTracking(true);
    setCentralWidget(centralWidget);
    resize(1000, 800);

    // Кнопка переключения темы
    themeButton = new QPushButton(darkTheme ? "☀️" : "🌙", centralWidget);
    themeButton->setFixedSize(50, 50);
    connect(themeButton, &QPushButton::clicked, this, &GUI::toggleTheme);

    // Применяем текущую тему
    updateTheme();
}

GUI::GUI(QWidget *parent, Game *game) : GUI(parent) {
    this->game = game;
}

GUI::~GUI() {
    // Сохраняем тему при закрытии
    saveTheme();
    clearGameField();
    delete settings;
}

void GUI::loadTheme() {
    // Загружаем сохраненную тему или используем светлую по умолчанию
    darkTheme = settings->value("darkTheme", false).toBool();
}

void GUI::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    if (gridLayout) {
        resizeCells();
    } else {
        calculateButtonPositions();
    }

    // Позиция кнопки темы
    if (themeButton) {
        themeButton->move(20, 20);
    }
}

void GUI::closeEvent(QCloseEvent *event) {
    saveTheme();
    QMainWindow::closeEvent(event);
}

void GUI::saveTheme() {
    // Сохраняем текущую тему
    settings->setValue("darkTheme", darkTheme);
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

    // Сохраняем выбор темы
    saveTheme();
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
                color: %3;
            }
            QPushButton:hover {
                background-color: %4;
            }
        )").arg(darkTheme ? "#666" : "#555")
          .arg(darkTheme ? "#333" : "#f0f0f0")
          .arg(darkTheme ? "#eee" : "#333")
          .arg(darkTheme ? "#444" : "#ddd"));
    }

    // Обновляем стиль игрового поля
    if (gridLayout) {
        printField();
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

void GUI::print(std::string message) {
    QMessageBox::information(this, "Сообщение", QString::fromStdString(message));
}

void GUI::printField() {
    if (!game) return;

    for (unsigned int i = 0; i < game->getRows(); i++) {
        for (unsigned int j = 0; j < game->getCols(); j++) {
            updateCell(i, j);
        }
    }
    updateMineCounter();

    // При победе или поражении открываем все поле
    if (gameEnded) {
        for (unsigned int i = 0; i < game->getRows(); i++) {
            for (unsigned int j = 0; j < game->getCols(); j++) {
                game->getCell(i, j).setOpen(true);
                updateCell(i, j);
            }
        }
    }
}

Command GUI::getCommand() {
    QEventLoop loop;
    commandLoop = &loop;
    loop.exec();
    commandLoop = nullptr;
    return pendingCommand;
}

std::tuple<int, int> GUI::getCoords() const {
    return std::make_tuple(lastRow, lastCol);
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
    widthSpin->setRange(2, 69);
    widthSpin->setValue(9);

    QSpinBox *heightSpin = new QSpinBox(&dialog);
    heightSpin->setRange(2, 45);
    heightSpin->setValue(9);

    QSpinBox *minesSpin = new QSpinBox(&dialog);
    minesSpin->setRange(1, widthSpin->value() * heightSpin->value() - 1);
    minesSpin->setValue(10);

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
    createGameField();
}

// Game field implementation
void GUI::createGameField() {
    clearGameField();

    QWidget *gameWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(gameWidget);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Top panel with mine counter and buttons
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->setAlignment(Qt::AlignCenter);

    mineCounterLabel = new QLabel(QString("Мины: %1").arg((int)game->getMines()));
    mineCounterLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    topLayout->addWidget(mineCounterLabel);

    // Add spacer for centering
    topLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    restartButton = new QPushButton("Рестарт");
    exitButton = new QPushButton("Выход в меню");
    topLayout->addWidget(restartButton);
    topLayout->addWidget(exitButton);

    mainLayout->addLayout(topLayout);

    // Game field grid
    gridLayout = new QGridLayout;
    gridLayout->setSpacing(1);

    cellButtons.resize(game->getRows());
    for (unsigned int i = 0; i < game->getRows(); i++) {
        cellButtons[i].resize(game->getCols());
        for (unsigned int j = 0; j < game->getCols(); j++) {
            QPushButton *button = new QPushButton;
            button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            button->setMinimumSize(20, 20);
            button->setProperty("row", static_cast<int>(i));
            button->setProperty("col", static_cast<int>(j));

            // Enable right-click context menu
            button->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(button, &QPushButton::clicked, [this, button]() {
                if (!gameEnded) { // Block clicks after game ends
                    int row = button->property("row").toInt();
                    int col = button->property("col").toInt();
                    cellClicked(row, col);
                }
            });
            connect(button, &QPushButton::customContextMenuRequested, [this, button]() {
                if (!gameEnded) { // Block clicks after game ends
                    int row = button->property("row").toInt();
                    int col = button->property("col").toInt();
                    cellRightClicked(row, col);
                }
            });

            gridLayout->addWidget(button, i, j);
            cellButtons[i][j] = button;
        }
    }

    // Center the game field
    QHBoxLayout *fieldLayout = new QHBoxLayout;
    fieldLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    fieldLayout->addLayout(gridLayout);
    fieldLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    mainLayout->addLayout(fieldLayout);
    setCentralWidget(gameWidget);

    // Connect buttons
    connect(restartButton, &QPushButton::clicked, this, &GUI::restartGame);
    connect(exitButton, &QPushButton::clicked, this, &GUI::exitToMenu);

    gameEnded = false;
    printField();
    resizeCells();
}

void GUI::clearGameField() {
    if (gridLayout) {
        for (auto& row : cellButtons) {
            for (auto button : row) {
                if (button) {
                    gridLayout->removeWidget(button);
                    delete button;
                }
            }
            row.clear();
        }
        cellButtons.clear();
        delete gridLayout;
        gridLayout = nullptr;
    }

    delete restartButton;
    restartButton = nullptr;
    delete exitButton;
    exitButton = nullptr;
    delete mineCounterLabel;
    mineCounterLabel = nullptr;
}

void GUI::updateCell(int row, int col) {
    if (!game || row >= cellButtons.size() || col >= cellButtons[0].size()) return;

    Cell& cell = game->getCell(row, col);
    QPushButton* button = cellButtons[row][col];

    QString style = "font-weight: bold; font-size: 14px;";
    QString text = "";

    // Determine colors for dark theme
    QString bgColor = darkTheme ? "#555" : "#bdbdbd";
    QString openBgColor = darkTheme ? "#333" : "#e0e0e0";
    QString textColor = darkTheme ? "#eee" : "#333";
    QString numberColors[] = {
        darkTheme ? "#4fc3f7" : "#1976d2", // 1: blue
        darkTheme ? "#81c784" : "#388e3c", // 2: green
        darkTheme ? "#e57373" : "#d32f2f", // 3: red
        darkTheme ? "#ba68c8" : "#7b1fa2", // 4: purple
        darkTheme ? "#ffb74d" : "#f57c00", // 5: orange
        darkTheme ? "#4db6ac" : "#00796b", // 6: teal
        darkTheme ? "#212121" : "#212121", // 7: black
        darkTheme ? "#9e9e9e" : "#616161"  // 8: gray
    };

    if (cell.isOpen()) {
        if (cell.isFlag() && game->getState()!=StateGame::Lose) {
            text = "🚩";
            style += "background-color: #ffcc00;";

        } else if (cell.isMine()) {
            text = "💣";
            style += "background-color: #ff4444;";
        } else {
            int state = cell.getState();
            if (state > 0) {
                style += QString("color: %1; background-color: %2;")
                         .arg(state <= 8 ? numberColors[state-1] : textColor)
                         .arg(openBgColor);
                text = QString::number(state);
            } else {
                style += QString("background-color: %1;").arg(openBgColor);
            }
        }
    } else {
        style += QString("background-color: %1;").arg(bgColor);
    }

    button->setStyleSheet(style);
    button->setText(text);
}

void GUI::updateMineCounter() {
    if (mineCounterLabel && game) {
        mineCounterLabel->setText(QString("Мины: %1").arg((int)game->getMines()));
    }
}

void GUI::resizeCells() {
    if (!game || !gridLayout) return;

    // Рассчитываем доступное пространство
    int availableWidth = width() - 40;
    int availableHeight = height() - 100;

    if (availableWidth <= 0 || availableHeight <= 0) return;

    // Рассчитываем размер кнопки
    int buttonWidth = availableWidth / static_cast<int>(game->getCols());
    int buttonHeight = availableHeight / static_cast<int>(game->getRows());
    int buttonSize = qBound(20, qMin(buttonWidth, buttonHeight), 40);

    // Устанавливаем размер для всех кнопок
    for (unsigned int i = 0; i < game->getRows(); i++) {
        for (unsigned int j = 0; j < game->getCols(); j++) {
            if (i < cellButtons.size() && j < cellButtons[i].size() && cellButtons[i][j]) {
                cellButtons[i][j]->setFixedSize(buttonSize, buttonSize);
            }
        }
    }
}

void GUI::cellClicked(int row, int col) {
    if (gameEnded) return;

    lastRow = row;
    lastCol = col;
    pendingCommand = Command::Attack;

    if (commandLoop) {
        commandLoop->quit();
    }
}

void GUI::cellRightClicked(int row, int col) {
    if (gameEnded) return;

    lastRow = row;
    lastCol = col;
    pendingCommand = Command::PutFlag;

    if (commandLoop) {
        commandLoop->quit();
    }
}

void GUI::restartGame() {
    pendingCommand = Command::Restart;

    if (commandLoop) {
        commandLoop->quit();
    }
}

void GUI::exitToMenu() {
    pendingCommand = Command::Exit;

    if (commandLoop) {
        commandLoop->quit();
    }
}

void GUI::showEndGameDialog(bool win) {
    gameEnded = true;
    printField(); // Open all cells

    if (endGameDialog) {
        endGameDialog->deleteLater();
        endGameDialog = nullptr;
    }

    endGameDialog = new QDialog(this);
    endGameDialog->setWindowTitle(win ? "Победа!" : "Поражение");
    endGameDialog->setFixedSize(300, 150);

    QVBoxLayout *layout = new QVBoxLayout(endGameDialog);

    gameResultLabel = new QLabel(win ? "🎉 Вы победили! 🎉" : "💥 Вы проиграли! 💥");
    gameResultLabel->setAlignment(Qt::AlignCenter);
    gameResultLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    layout->addWidget(gameResultLabel);

    QPushButton *restartBtn = new QPushButton("Новая игра");
    QPushButton *menuBtn = new QPushButton("Главное меню");

    connect(restartBtn, &QPushButton::clicked, [this]() {
        endGameDialog->accept();
        restartGame();
    });

    connect(menuBtn, &QPushButton::clicked, [this]() {
        endGameDialog->accept();
        exitToMenu();
    });

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(restartBtn);
    btnLayout->addWidget(menuBtn);

    layout->addLayout(btnLayout);

    // Apply theme
    QString dialogStyle = QString(R"(
        QDialog {
            background-color: %1;
        }
        QLabel, QPushButton {
            color: %2;
        }
        QPushButton {
            background-color: %3;
            border: 1px solid %4;
            padding: 5px;
            border-radius: 5px;
        }
    )").arg(darkTheme ? "#333" : "#f5f5f5")
      .arg(darkTheme ? "#eee" : "#333")
      .arg(darkTheme ? "#444" : "#ddd")
      .arg(darkTheme ? "#666" : "#aaa");

    endGameDialog->setStyleSheet(dialogStyle);

    endGameDialog->exec();
}