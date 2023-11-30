#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class MainMenu : public QWidget {
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

private:
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    QVBoxLayout *layout;
};