//
// Created by Jlisowskyy on 11/04/24.
//


/* internal includes */
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "UiObjects/ToolBar.hpp"
#include "UiObjects/DisplaySpace.hpp"
#include "Management/StateMgr.hpp"

/* external includes */
#include <QFrame>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_ui(new Ui::MainWindow),
      m_toolBar(new ToolBar(this)),
      m_stateMgr(new StateMgr(this)),
      m_displaySpace(new DisplaySpace(this)) {
    setMinimumSize(900, 600);
    setWindowTitle("Dithering algorithms");

    m_ui->setupUi(this);
    m_toolBar->setupToolBar(m_ui->toolBar);
    m_stateMgr->setup(m_toolBar);
    m_displaySpace->setup(m_stateMgr);

    m_ui->verticalLayout->addWidget(m_displaySpace);

    m_stateMgr->loadDefaults();
}


MainWindow::~MainWindow() {
    delete m_ui;
}
