
#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QWidget>
#include <QMessageBox>
#include <QEventLoop>
#include <QDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QResizeEvent>
#include <QPropertyAnimation>
#include <QSettings>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <string>
#include <vector>
#include <algorithm>
#include "../include/GameLoop.h"
#include "../include/Game.h"

class AnimatedButton : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(float fillProgress READ fillProgress WRITE setFillProgress)
public:
    explicit AnimatedButton(const QString &text, QWidget *parent = nullptr);
    ~AnimatedButton() override;
    float fillProgress() const;
    void setFillProgress(float progress);
    void updateStyle(bool darkTheme);
    void setFixedSize(int width, int height);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QPropertyAnimation *fillAnimation;
    float m_fillProgress;
    bool isDarkTheme;
};

class MainMenuWidget : public QWidget {
    Q_OBJECT
public:
    explicit MainMenuWidget(QWidget *parent = nullptr, bool darkTheme = false);
    void updateTheme(bool darkTheme);

signals:
    void difficultySelected(unsigned int rows, unsigned int cols, unsigned int mines);
    void themeToggled();

private slots:
    void handleEasy();
    void handleMedium();
    void handleHard();
    void handleCustom();

private:
    void resizeEvent(QResizeEvent *event) override;

    AnimatedButton *easyButton;
    AnimatedButton *mediumButton;
    AnimatedButton *hardButton;
    AnimatedButton *customButton;
    QPushButton *themeButton;
    bool darkTheme;

    const int BUTTON_SIZE = 280;
    const int BUTTON_SPACING = 30;
};

class GameWidget : public QWidget {
    Q_OBJECT
public:
    explicit GameWidget(Game *game, bool darkTheme, QWidget *parent = nullptr);
    ~GameWidget();

    void setGame(Game* newGame);
    void resetGameState();
    void updateTheme(bool darkTheme);
    void updateGameField();
    void resizeCells();
    void showEndGameDialog(bool win);

    signals:
        void cellClicked(int row, int col, Command command);
    void restartRequested();
    void exitToMenuRequested();

private:
    void createGameField();
    void updateCell(int row, int col);
    void updateMineCounter();

    Game *game;
    bool darkTheme;
    bool gameEnded;

    QGridLayout *gridLayout;
    std::vector<std::vector<QPushButton*>> cellButtons;
    AnimatedButton *restartButton;
    AnimatedButton *exitButton;
    QLabel *mineCounterLabel;
    QDialog *endGameDialog;
};

class GUI : public QMainWindow {
    Q_OBJECT

public:
    explicit GUI(QWidget *parent = nullptr);
    ~GUI() override;

    void print(std::string message);
    void printField();
    Command getCommand();
    std::tuple<int, int> getCoords() const;
    void getDifficulty(unsigned int &number_of_rows, unsigned int &number_of_cols, unsigned int &number_of_mines);
    void setGame(Game *game);
    void showEndGameDialog(bool win);

signals:
    void difficultyChosen();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void toggleTheme();
    void handleCellClicked(int row, int col, Command command);
    void handleDifficultySelected(unsigned int rows, unsigned int cols, unsigned int mines);
    void handleRestart();
    void handleExitToMenu();

private:
    void updateTheme();
    void loadTheme();
    void saveTheme();
    void showMainMenu();
    void showGameScreen();

    Game *game;
    QWidget *centralWidget;
    QStackedWidget *stackedWidget;
    unsigned int selectedRows;
    unsigned int selectedCols;
    unsigned int selectedMines;
    bool darkTheme;
    QSettings *settings;

    MainMenuWidget *mainMenuWidget;
    GameWidget *gameWidget;

    // Command handling
    QEventLoop *commandLoop;
    Command pendingCommand;
    int lastRow;
    int lastCol;
};
#endif // GUI_H
