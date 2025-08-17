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
#include <QGraphicsEffect>
#include "../include/GameLoop.h"
#include "../include/Game.h"
#include <string>

class AnimatedButton : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(float fillProgress READ fillProgress WRITE setFillProgress)
public:
    explicit AnimatedButton(const QString &text, QWidget *parent = nullptr);
    float fillProgress() const;
    void setFillProgress(float progress);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QPropertyAnimation *fillAnimation;
    float m_fillProgress;
    QColor fillColor;
};

class GUI : public QMainWindow {
    Q_OBJECT

public:
    explicit GUI(QWidget *parent = nullptr);
    GUI(QWidget *parent, Game *game);
    ~GUI() override = default;

    void print(std::string message);
    void printField();
    Command getCommand();
    std::tuple<int, int> getCoords() const;
    void getDifficulty(unsigned int &number_of_rows, unsigned int &number_of_cols, unsigned int &number_of_mines);
    void setGame(Game *game);

signals:
    void difficultyChosen();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void calculateButtonPositions();

private slots:
    void handleEasy();
    void handleMedium();
    void handleHard();
    void handleCustom();

private:
    Game *game;
    QWidget *centralWidget;
    unsigned int *rowsPtr;
    unsigned int *colsPtr;
    unsigned int *minesPtr;
    bool *choiceMadePtr;

    AnimatedButton *easyButton;
    AnimatedButton *mediumButton;
    AnimatedButton *hardButton;
    AnimatedButton *customButton;

    const int BUTTON_SIZE = 300;
    const int BUTTON_SPACING = 40;
    const int MARGIN = 60;
};
#endif // GUI_H