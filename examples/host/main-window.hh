﻿#pragma once

#include <QMainWindow>

class Application;
class SettingsDialog;
class PluginParametersWidget;

class MainWindow : public QMainWindow {
   Q_OBJECT

public:
   explicit MainWindow(Application &app);
   ~MainWindow();

   WId getEmbedWindowId();

public:
   void showSettingsDialog();
   void showPluginParametersWindow();
   void resizePluginView(int width, int height);

private:
   void createMenu();

   Application &   application_;
   SettingsDialog *settingsDialog_ = nullptr;
   QWindow *       pluginViewWindow_ = nullptr;
   QWidget *       pluginViewWidget_ = nullptr;

   QMainWindow *           pluginParametersWindow_ = nullptr;
   PluginParametersWidget *pluginParametersWidget_ = nullptr;
};
