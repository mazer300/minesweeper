#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
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

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QPropertyAnimation *fillAnimation;
    float m_fillProgress;
    bool isDarkTheme;
};

class GUI : public QMainWindow {
    Q_OBJECT

public:
    explicit GUI(QWidget *parent = nullptr);
    GUI(QWidget *parent, Game *game);
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
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void handleEasy();
    void handleMedium();
    void handleHard();
    void handleCustom();
    void toggleTheme();
    void cellClicked(int row, int col);
    void cellRightClicked(int row, int col);
    void restartGame();
    void exitToMenu();

private:
    void updateTheme();
    void loadTheme();
    void saveTheme();
    void createGameField();
    void clearGameField();
    void updateCell(int row, int col);
    void updateMineCounter();
    void calculateButtonPositions();
    void resizeCells();

    Game *game;
    QWidget *centralWidget;
    unsigned int *rowsPtr;
    unsigned int *colsPtr;
    unsigned int *minesPtr;
    bool *choiceMadePtr;
    bool darkTheme;
    bool gameEnded;
    QSettings *settings;

    AnimatedButton *easyButton;
    AnimatedButton *mediumButton;
    AnimatedButton *hardButton;
    AnimatedButton *customButton;
    QPushButton *themeButton;

    // Game field components
    QGridLayout *gridLayout;
    std::vector<std::vector<QPushButton*>> cellButtons;
    QPushButton *restartButton;
    QPushButton *exitButton;
    QLabel *mineCounterLabel;

    // Command handling
    QEventLoop *commandLoop;
    Command pendingCommand;
    int lastRow;
    int lastCol;

    // End game dialog
    QDialog *endGameDialog;
    QLabel *gameResultLabel;

    const int BUTTON_SIZE = 300;
    const int BUTTON_SPACING = 40;
    const int MARGIN = 60;
};
#endif // GUI_H