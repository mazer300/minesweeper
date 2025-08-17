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
#include "../include/GameLoop.h"
#include "../include/Game.h"
#include <string>

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

    // Кнопки
    QPushButton *easyButton;
    QPushButton *mediumButton;
    QPushButton *hardButton;
    QPushButton *customButton;

    // Параметры расположения
    const int BUTTON_SIZE = 300;        // Фиксированный размер кнопок
    const int BUTTON_SPACING = 40;      // Фиксированное расстояние между кнопками
    const int MARGIN = 60;              // Отступ от краев окна
};
#endif // GUI_H