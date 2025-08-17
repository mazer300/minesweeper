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
#include <QVBoxLayout>
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

void AnimatedButton::setFixedSize(int width, int height) {
    QPushButton::setFixedSize(width, height);
}

void AnimatedButton::paintEvent(QPaintEvent *event) {
    QPushButton::paintEvent(event);

    if (m_fillProgress > 0.0f) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Центр кнопки
        QPoint center = rect().center();

        // Максимальный радиус (расстояние от центра до угла)
        double maxRadius = qSqrt(qPow(rect().width()/2.0, 2) + qPow(rect().height()/2.0, 2));
        double currentRadius = maxRadius * m_fillProgress;

        // Создаем путь для круга, расширяющегося из центра
        QPainterPath clipPath;
        clipPath.addEllipse(center, currentRadius, currentRadius);

        // Применяем маску
        painter.setClipPath(clipPath);

        // Заливаем область внутри круга
        QPainterPath backgroundPath;
        backgroundPath.addRoundedRect(rect(), 15, 15);
        painter.fillPath(backgroundPath, QColor(255, 255, 255, isDarkTheme ? 60 : 120));

        painter.setClipping(false);
    }

    // Рисуем текст поверх анимации
    QPainter textPainter(this);
    textPainter.setRenderHint(QPainter::Antialiasing);
    textPainter.setPen(isDarkTheme ? Qt::white : Qt::black);
    textPainter.drawText(rect(), Qt::AlignCenter, text());
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

// Реализация MainMenuWidget
MainMenuWidget::MainMenuWidget(QWidget *parent, bool darkTheme)
    : QWidget(parent), darkTheme(darkTheme) {

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Top layout for theme button
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->setContentsMargins(0, 0, 0, 20);
    topLayout->addStretch();

    themeButton = new QPushButton(darkTheme ? "☀️" : "🌙", this);
    themeButton->setFixedSize(50, 50);
    topLayout->addWidget(themeButton);
    mainLayout->addLayout(topLayout);

    // Добавляем растяжку перед кнопками
    mainLayout->addStretch(1);

    // Grid layout for difficulty buttons
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setSpacing(BUTTON_SPACING);
    gridLayout->setContentsMargins(20, 20, 20, 20);  // Фиксированные отступы

    easyButton = new AnimatedButton("ЛЁГКИЙ\n\n9×9 клеток\n10 мин", this);
    mediumButton = new AnimatedButton("СРЕДНИЙ\n\n16×16 клеток\n40 мин", this);
    hardButton = new AnimatedButton("ТЯЖЁЛЫЙ\n\n30×16 клеток\n99 мин", this);
    customButton = new AnimatedButton("ПОЛЬЗОВАТЕЛЬСКИЙ\n\nВыбрать параметры", this);

    // Set fixed size for buttons
    easyButton->setFixedSize(BUTTON_SIZE, BUTTON_SIZE);
    mediumButton->setFixedSize(BUTTON_SIZE, BUTTON_SIZE);
    hardButton->setFixedSize(BUTTON_SIZE, BUTTON_SIZE);
    customButton->setFixedSize(BUTTON_SIZE, BUTTON_SIZE);

    // Add buttons to grid
    gridLayout->addWidget(easyButton, 0, 0, Qt::AlignCenter);
    gridLayout->addWidget(mediumButton, 0, 1, Qt::AlignCenter);
    gridLayout->addWidget(hardButton, 1, 0, Qt::AlignCenter);
    gridLayout->addWidget(customButton, 1, 1, Qt::AlignCenter);

    // Центрируем сетку с помощью горизонтального лейаута с растяжками
    QHBoxLayout *centerLayout = new QHBoxLayout();
    centerLayout->addStretch(1);
    centerLayout->addLayout(gridLayout);
    centerLayout->addStretch(1);

    mainLayout->addLayout(centerLayout);

    // Добавляем растяжку после кнопок
    mainLayout->addStretch(1);

    // Обновляем стили
    updateTheme(darkTheme);

    QFont font;
    font.setPointSize(16);
    font.setBold(true);

    easyButton->setFont(font);
    mediumButton->setFont(font);
    hardButton->setFont(font);
    customButton->setFont(font);

    connect(easyButton, &QPushButton::clicked, this, &MainMenuWidget::handleEasy);
    connect(mediumButton, &QPushButton::clicked, this, &MainMenuWidget::handleMedium);
    connect(hardButton, &QPushButton::clicked, this, &MainMenuWidget::handleHard);
    connect(customButton, &QPushButton::clicked, this, &MainMenuWidget::handleCustom);
    connect(themeButton, &QPushButton::clicked, this, [this]() { emit themeToggled(); });

    setLayout(mainLayout);
}

void MainMenuWidget::updateTheme(bool darkTheme) {
    this->darkTheme = darkTheme;
    themeButton->setText(darkTheme ? "☀️" : "🌙");

    // Устанавливаем цвет фона для всего виджета
    QString bgColor = darkTheme ? "#222" : "#f5f5f5";
    setStyleSheet(QString("background-color: %1;").arg(bgColor));

    // Обновляем стиль кнопок
    easyButton->updateStyle(darkTheme);
    mediumButton->updateStyle(darkTheme);
    hardButton->updateStyle(darkTheme);
    customButton->updateStyle(darkTheme);

    // Обновляем стиль кнопки темы
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

void MainMenuWidget::handleEasy() {
    emit difficultySelected(9, 9, 10);
}

void MainMenuWidget::handleMedium() {
    emit difficultySelected(16, 16, 40);
}

void MainMenuWidget::handleHard() {
    emit difficultySelected(16, 30, 99);
}

void MainMenuWidget::handleCustom() {
    QDialog dialog(this);
    dialog.setWindowTitle("Пользовательские настройки");
    dialog.setFixedSize(400, 300);

    // Настройка стиля диалога согласно теме
    QString dialogStyle = QString(R"(
        QDialog {
            background-color: %1;
            color: %2;
        }
        QLabel, QSpinBox, QDialogButtonBox {
            color: %2;
        }
        QSpinBox {
            background-color: %3;
            color: %2;
            border: 1px solid %4;
        }
        QPushButton {
            background-color: %5;
            color: %6;
            border: 1px solid %7;
            border-radius: 5px;
            padding: 5px 10px;
            min-width: 80px;
        }
        QPushButton:hover {
            background-color: %8;
        }
    )").arg(darkTheme ? "#333" : "#f5f5f5")  // фон диалога
      .arg(darkTheme ? "#fff" : "#333")      // цвет текста (белый в тёмной теме)
      .arg(darkTheme ? "#555" : "#fff")      // фон спинбоксов
      .arg(darkTheme ? "#777" : "#ccc")      // граница спинбоксов
      .arg(darkTheme ? "#444" : "#e0e0e0")   // фон кнопок
      .arg(darkTheme ? "#eee" : "#333")      // цвет текста кнопок
      .arg(darkTheme ? "#666" : "#999")      // граница кнопок
      .arg(darkTheme ? "#555" : "#d0d0d0");  // фон кнопок при наведении

    dialog.setStyleSheet(dialogStyle);

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
        emit difficultySelected(heightSpin->value(), widthSpin->value(), minesSpin->value());
    }
}

void MainMenuWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
}

// Реализация GameWidget
GameWidget::GameWidget(Game *game, bool darkTheme, QWidget *parent)
    : QWidget(parent), game(game), darkTheme(darkTheme), gameEnded(false), endGameDialog(nullptr) {

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Top panel with mine counter
    QHBoxLayout *topLayout = new QHBoxLayout;
    mineCounterLabel = new QLabel(QString("Мины: %1").arg((int)game->getMines()));
    mineCounterLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    topLayout->addWidget(mineCounterLabel, 0, Qt::AlignLeft);
    topLayout->addStretch();
    mainLayout->addLayout(topLayout);

    // Game field grid
    QHBoxLayout *fieldLayout = new QHBoxLayout;
    fieldLayout->setAlignment(Qt::AlignCenter);

    gridLayout = new QGridLayout;
    gridLayout->setSpacing(2);

    cellButtons.resize(game->getRows());
    for (unsigned int i = 0; i < game->getRows(); i++) {
        cellButtons[i].resize(game->getCols());
        for (unsigned int j = 0; j < game->getCols(); j++) {
            QPushButton *button = new QPushButton;
            button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            button->setMinimumSize(30, 30);
            button->setProperty("row", static_cast<int>(i));
            button->setProperty("col", static_cast<int>(j));
            button->setContextMenuPolicy(Qt::CustomContextMenu);

            connect(button, &QPushButton::clicked, [this, button]() {
                int row = button->property("row").toInt();
                int col = button->property("col").toInt();
                if (!gameEnded) {
                    emit cellClicked(row, col, Command::Attack);
                }
            });

            connect(button, &QPushButton::customContextMenuRequested, [this, button]() {
                int row = button->property("row").toInt();
                int col = button->property("col").toInt();
                if (!gameEnded) {
                    emit cellClicked(row, col, Command::PutFlag);
                }
            });

            gridLayout->addWidget(button, i, j);
            cellButtons[i][j] = button;
        }
    }

    fieldLayout->addLayout(gridLayout);
    mainLayout->addLayout(fieldLayout);

    // Bottom panel with buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setAlignment(Qt::AlignCenter);

    restartButton = new AnimatedButton("Рестарт", this);
    exitButton = new AnimatedButton("Выход в меню", this);

    restartButton->setFixedSize(180, 60);
    exitButton->setFixedSize(180, 60);

    QFont buttonFont;
    buttonFont.setPointSize(14);
    buttonFont.setBold(true);
    restartButton->setFont(buttonFont);
    exitButton->setFont(buttonFont);

    buttonLayout->addWidget(restartButton);
    buttonLayout->addSpacing(20);
    buttonLayout->addWidget(exitButton);

    mainLayout->addLayout(buttonLayout);

    connect(restartButton, &QPushButton::clicked, this, [this]() { emit restartRequested(); });
    connect(exitButton, &QPushButton::clicked, this, [this]() { emit exitToMenuRequested(); });

    updateTheme(darkTheme);
    updateGameField();
}

GameWidget::~GameWidget() {
    if (endGameDialog) {
        endGameDialog->deleteLater();
    }
}

void GameWidget::setGame(Game* newGame) {
    game = newGame;
    gameEnded = false;
    updateGameField();
    resizeCells();
}

void GameWidget::updateTheme(bool darkTheme) {
    this->darkTheme = darkTheme;

    // Устанавливаем цвет фона для всего виджета
    QString bgColor = darkTheme ? "#222" : "#f5f5f5";
    setStyleSheet(QString("background-color: %1;").arg(bgColor));

    // Обновляем стиль кнопок
    if (restartButton) restartButton->updateStyle(darkTheme);
    if (exitButton) exitButton->updateStyle(darkTheme);

    // Обновляем стиль счетчика мин
    if (mineCounterLabel) {
        mineCounterLabel->setStyleSheet(
            QString("font-size: 18px; font-weight: bold; color: %1;")
            .arg(darkTheme ? "#eee" : "#333")
        );
    }

    // Обновляем игровое поле
    updateGameField();
}

void GameWidget::updateGameField() {
    if (!game) return;

    for (unsigned int i = 0; i < game->getRows(); i++) {
        for (unsigned int j = 0; j < game->getCols(); j++) {
            updateCell(i, j);
        }
    }
    updateMineCounter();
}

void GameWidget::updateCell(int row, int col) {
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
        if (cell.isFlag() && game->getState() != StateGame::Lose) {
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

void GameWidget::updateMineCounter() {
    if (mineCounterLabel && game) {
        mineCounterLabel->setText(QString("Мины: %1").arg((int)game->getMines()));
    }
}

void GameWidget::resizeCells() {
    if (!game || !gridLayout || cellButtons.empty()) return;

    // Рассчитываем доступное пространство
    int availableWidth = width() - 40;
    int availableHeight = height() - 200; // Учитываем место для кнопок и счетчика

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

void GameWidget::showEndGameDialog(bool win) {
    gameEnded = true;
    updateGameField(); // Open all cells

    if (endGameDialog) {
        endGameDialog->deleteLater();
    }

    endGameDialog = new QDialog(this);
    endGameDialog->setWindowTitle(win ? "Победа!" : "Поражение");
    endGameDialog->setFixedSize(300, 150);

    QVBoxLayout *layout = new QVBoxLayout(endGameDialog);

    QLabel *gameResultLabel = new QLabel(win ? "🎉 Вы победили! 🎉" : "💥 Вы проиграли! 💥");
    gameResultLabel->setAlignment(Qt::AlignCenter);
    gameResultLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    layout->addWidget(gameResultLabel);

    QPushButton *restartBtn = new QPushButton("Новая игра");
    // QPushButton *menuBtn = new QPushButton("Главное меню");

    connect(restartBtn, &QPushButton::clicked, [this]() {
        endGameDialog->accept();
        emit restartRequested();
    });

    /*connect(menuBtn, &QPushButton::clicked, [this]() {
        endGameDialog->accept();
        emit exitToMenuRequested();
    });*/

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(restartBtn);
    // btnLayout->addWidget(menuBtn);

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

// Реализация GUI
GUI::GUI(QWidget *parent) : QMainWindow(parent), game(nullptr),
    commandLoop(nullptr), mainMenuWidget(nullptr), gameWidget(nullptr) {

    // Инициализация настроек
    settings = new QSettings("Minesweeper", "ThemeSettings", this);

    // Загрузка сохраненной темы
    loadTheme();

    centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    stackedWidget = new QStackedWidget(centralWidget);
    mainLayout->addWidget(stackedWidget);

    setCentralWidget(centralWidget);
    resize(1000, 800);

    // Устанавливаем цвет фона для центрального виджета
    updateTheme();

    showMainMenu();
}

GUI::~GUI() {
    // Сохраняем тему при закрытии
    saveTheme();
    delete settings;
}

void GUI::loadTheme() {
    // Загружаем сохраненную тему или используем светлую по умолчанию
    darkTheme = settings->value("darkTheme", false).toBool();
}

void GUI::saveTheme() {
    // Сохраняем текущую тему
    settings->setValue("darkTheme", darkTheme);
}

void GUI::closeEvent(QCloseEvent *event) {
    saveTheme();
    QMainWindow::closeEvent(event);
}

void GUI::showMainMenu() {
    if (gameWidget) {
        stackedWidget->removeWidget(gameWidget);
        gameWidget->deleteLater();
        gameWidget = nullptr;
    }

    if (!mainMenuWidget) {
        mainMenuWidget = new MainMenuWidget(stackedWidget, darkTheme);
        stackedWidget->addWidget(mainMenuWidget);
        connect(mainMenuWidget, &MainMenuWidget::difficultySelected,
                this, &GUI::handleDifficultySelected);
        connect(mainMenuWidget, &MainMenuWidget::themeToggled,
                this, &GUI::toggleTheme);
    }

    stackedWidget->setCurrentWidget(mainMenuWidget);
}

void GUI::showGameScreen() {
    if (!gameWidget) {
        gameWidget = new GameWidget(game, darkTheme, stackedWidget);
        stackedWidget->addWidget(gameWidget);
        connect(gameWidget, &GameWidget::cellClicked,
                this, &GUI::handleCellClicked);
        connect(gameWidget, &GameWidget::restartRequested,
                this, &GUI::handleRestart);
        connect(gameWidget, &GameWidget::exitToMenuRequested,
                this, &GUI::handleExitToMenu);
    }

    stackedWidget->setCurrentWidget(gameWidget);
    gameWidget->resizeCells();
}

void GUI::toggleTheme() {
    darkTheme = !darkTheme;
    updateTheme();
    saveTheme();
}

void GUI::updateTheme() {
    // Устанавливаем цвет фона для центрального виджета
    QString bgColor = darkTheme ? "#222" : "#f5f5f5";
    centralWidget->setStyleSheet(QString("background-color: %1;").arg(bgColor));

    // Обновляем текущий экран
    if (mainMenuWidget) {
        mainMenuWidget->updateTheme(darkTheme);
    }
    if (gameWidget) {
        gameWidget->updateTheme(darkTheme);
    }
}

void GUI::getDifficulty(unsigned int &number_of_rows, unsigned int &number_of_cols, unsigned int &number_of_mines) {
    selectedRows = number_of_rows;
    selectedCols = number_of_cols;
    selectedMines = number_of_mines;

    QEventLoop loop;
    connect(this, &GUI::difficultyChosen, &loop, &QEventLoop::quit);
    loop.exec();

    number_of_rows = selectedRows;
    number_of_cols = selectedCols;
    number_of_mines = selectedMines;
}

void GUI::print(std::string message) {
    QMessageBox::information(this, "Сообщение", QString::fromStdString(message));
}

void GUI::printField() {
    if (gameWidget) {
        gameWidget->updateGameField();
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

void GUI::setGame(Game *game) {
    this->game = game;

    if (gameWidget) {
        gameWidget->setGame(game);
    } else {
        showGameScreen();
    }
}

void GUI::showEndGameDialog(bool win) {
    if (gameWidget) {
        gameWidget->showEndGameDialog(win);
    }
}

void GUI::handleDifficultySelected(unsigned int rows, unsigned int cols, unsigned int mines) {
    selectedRows = rows;
    selectedCols = cols;
    selectedMines = mines;
    emit difficultyChosen();
}

void GUI::handleCellClicked(int row, int col, Command command) {
    lastRow = row;
    lastCol = col;
    pendingCommand = command;

    if (commandLoop) {
        commandLoop->quit();
    }
}

void GUI::handleRestart() {
    pendingCommand = Command::Restart;

    if (commandLoop) {
        commandLoop->quit();
    }
}

void GUI::handleExitToMenu() {
    pendingCommand = Command::Exit;
    showMainMenu();

    if (commandLoop) {
        commandLoop->quit();
    }
}