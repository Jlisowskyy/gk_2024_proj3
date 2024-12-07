//
// Created by Jlisowskyy on 11/04/24.
//


/* internal includes */
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "UiObjects/ToolBar.hpp"
#include "Management/StateMgr.hpp"

/* external includes */
#include <QFrame>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_ui(new Ui::MainWindow),
      m_toolBar(new ToolBar(this)),
      m_stateMgr(new StateMgr(this)) {
    setMinimumSize(600, 900);
    setWindowTitle("Bezier 3D drawer");

    m_ui->setupUi(this);
    m_toolBar->setupToolBar(m_ui->toolBar);
    m_stateMgr->setup(m_toolBar);

    // QFrame *frame = new QFrame(this);
    // frame->setFrameShape(QFrame::HLine);
    // frame->setFrameShadow(QFrame::Sunken);
    // frame->setLineWidth(3);
    // frame->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    // m_ui->verticalLayout->addWidget(frame);
}


MainWindow::~MainWindow() {
    delete m_ui;
}
