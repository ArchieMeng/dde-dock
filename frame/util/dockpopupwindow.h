// Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2018 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DOCKPOPUPWINDOW_H
#define DOCKPOPUPWINDOW_H

#include "org_deepin_dde_xeventmonitor1.h"

#include <darrowrectangle.h>
#include <dregionmonitor.h>
#include <DWindowManagerHelper>

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

using XEventMonitor = org::deepin::dde::XEventMonitor1;

class DockPopupWindow : public Dtk::Widget::DArrowRectangle
{
    Q_OBJECT

public:
    explicit DockPopupWindow(QWidget *parent = 0);
    ~DockPopupWindow();

    bool model() const;

    void setContent(QWidget *content);
    void setExtendWidget(QWidget *widget);
    QWidget *extengWidget() const;

public slots:
    void show(const QPoint &pos, const bool model = false);
    void show(const int x, const int y);
    void hide();

signals:
    void accept() const;
    // 在把专业版的仓库降级到debian的stable时, dock出现了一个奇怪的问题:
    // 在plugins/tray/system-trays/systemtrayitem.cpp中的showPopupWindow函数中
    // 无法连接到上面这个信号: "accept", qt给出一个运行时警告提示找不到信号
    // 目前的解决方案就是在下面增加了这个信号
    void unusedSignal();

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *event);
    void enterEvent(QEvent *e);
    bool eventFilter(QObject *o, QEvent *e);
    void blockButtonRelease();

private slots:
    void compositeChanged();
    void ensureRaised();
    void onButtonPress(int type, int x, int y, const QString &key);

private:
    bool m_model;
    QPoint m_lastPoint;

    XEventMonitor *m_eventMonitor;
    QString m_eventKey;
    DWindowManagerHelper *m_wmHelper;
    bool m_enableMouseRelease;
    QWidget *m_extendWidget;
};

class PopupSwitchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PopupSwitchWidget(QWidget *parent = nullptr);
    ~PopupSwitchWidget();

    void pushWidget(QWidget *widget);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QVBoxLayout *m_containerLayout;
    QWidget *m_topWidget;
};

#endif // DOCKPOPUPWINDOW_H
