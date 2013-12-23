# Add more folders to ship with the application, here
folder_01.source = qml/posqt
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=
QMAKE_CC = gcc-4.8
QMAKE_CXX = g++-4.8

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    src/server/server.cpp \
    src/data.cpp \
    src/listmodel.cpp \
    src/sale.cpp \
    src/ordereditem.cpp \
    src/user.cpp \
    src/menu.cpp \
    src/table.cpp \
    src/itemtraits.cpp \
    src/cmain.cpp \
    src/menuinfo.cpp \
    src/ticketinfo.cpp \
    src/reports.cpp \
    src/listitems.cpp \
    src/userinfo.cpp \
    src/item.cpp \
    src/data_sale.cpp \
    src/server/server_user_commands.cpp \
    src/data_user.cpp \
    src/server/server_sale_commands.cpp \
    src/report_item.cpp \
    src/server/server_reports_commands.cpp \
    src/settings.cpp \
    src/data_menu.cpp \
    src/menuitem.cpp \
    src/settings_prop.cpp \
    src/settings_item.cpp \
    src/print.cpp \
    src/data_table.cpp \
    src/server_order_cmd.cpp \
    src/data_reports.cpp
LIBS += -L/usr/lib/ -lpq -lpgport -lssl -lcrypto -lkrb5 -lcom_err -lgssapi_krb5 -lz -lcrypt -ldl -lm -pthread
LIBS += -lcups -Bsymbolic-functions -lk5crypto -lcom_err -lgnutls -L/lib/i386-linux-gnu
QMAKE_CXXFLAGS += -std=c++11

# Please do not modify the following two lines. Required for deployment.
include(qtquick1applicationviewer/qtquick1applicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    qml/posqt/ParallaxView.qml \
    qml/posqt/UserView.qml \
    qml/posqt/app_nav.js \
    qml/posqt/FadeView.qml \
    qml/posqt/WarningLabel.qml \
    qml/posqt/WarningBox.qml \
    qml/posqt/UserIcon.qml \
    qml/posqt/Toolbar.qml \
    qml/posqt/TimeBar.qml \
    qml/posqt/tickettest.qml \
    qml/posqt/TicketItem.qml \
    qml/posqt/Ticket.qml \
    qml/posqt/TableView.qml \
    qml/posqt/SplitTicket.qml \
    qml/posqt/Slide.qml \
    qml/posqt/SettingsView.qml \
    qml/posqt/ReportView.qml \
    qml/posqt/ReportItem.qml \
    qml/posqt/ReportChart.qml \
    qml/posqt/Navigator.qml \
    qml/posqt/MenuView.qml \
    qml/posqt/MenuCategory.qml \
    qml/posqt/main.qml \
    qml/posqt/LoginScreen.qml \
    qml/posqt/LoginPad.qml \
    qml/posqt/ListSlider.qml \
    qml/posqt/InfoArea.qml \
    qml/posqt/ImageMaskEffect.qml \
    qml/posqt/CashInput.qml \
    qml/posqt/Button.qml \
    qml/posqt/Backdrop.qml \
    qml/posqt/AdminView.qml \
    qml/posqt/AdminConsole.qml \
    qml/posqt/yellow_warning_ico.svg \
    qml/posqt/yellow_exp_ico.svg \
    qml/posqt/yellow_exit_ico.svg \
    qml/posqt/yello_exp_ico.svg \
    qml/posqt/x_ico.svg \
    qml/posqt/user.js \
    qml/posqt/user_ico.svg \
    qml/posqt/tran_plus_ico.svg \
    qml/posqt/ticket.js \
    qml/posqt/ticket_indicator.svg \
    qml/posqt/ticket_functions.js \
    qml/posqt/table_no_legs_ico.svg \
    qml/posqt/table_ico.svg \
    qml/posqt/table_active.svg \
    qml/posqt/table_active_ico.svg \
    qml/posqt/slider.svg \
    qml/posqt/settings_ico2.svg \
    qml/posqt/settings_ico.svg \
    qml/posqt/secondhand_ico.svg \
    qml/posqt/scondhand_ico.svg \
    qml/posqt/rok_black.png \
    qml/posqt/reports_ico3.svg \
    qml/posqt/reports_ico2.svg \
    qml/posqt/reports_ico.svg \
    qml/posqt/red_warning_ico.svg \
    qml/posqt/red_exp_ico.svg \
    qml/posqt/red_exit_ico.svg \
    qml/posqt/plus_ico.svg \
    qml/posqt/pause_ico.svg \
    qml/posqt/money_bag_ico.svg \
    qml/posqt/menu.js \
    qml/posqt/menu_functions.js \
    qml/posqt/mask.png \
    qml/posqt/logout_ico.svg \
    qml/posqt/list_slider.svg \
    qml/posqt/launch_ico.svg \
    qml/posqt/inventory_ico.svg \
    qml/posqt/infinity_ico.svg \
    qml/posqt/indicator3.svg \
    qml/posqt/indicator2.svg \
    qml/posqt/indicator.svg \
    qml/posqt/hourhand_ico.svg \
    qml/posqt/home_ico.svg \
    qml/posqt/hexagon_ico.svg \
    qml/posqt/hardhat_ico.svg \
    qml/posqt/half_clock.svg \
    qml/posqt/green_warning_ico.svg \
    qml/posqt/green_launch_ico.svg \
    qml/posqt/green_exp_ico.svg \
    qml/posqt/fried.jpg \
    qml/posqt/eye_ico.svg \
    qml/posqt/exit_ico.svg \
    qml/posqt/even_ico.svg \
    qml/posqt/edit_ico.svg \
    qml/posqt/dollar_ico.svg \
    qml/posqt/credit_ico.svg \
    qml/posqt/comment_ico.svg \
    qml/posqt/colors.js \
    qml/posqt/clockout_ico.svg \
    qml/posqt/clockin_ico.svg \
    qml/posqt/clock_ico.svg \
    qml/posqt/check_white_ico.svg \
    qml/posqt/check_ico.svg \
    qml/posqt/check_green_ico.svg \
    qml/posqt/check_blue_ico.svg \
    qml/posqt/cash_ico.svg \
    qml/posqt/calendar_ico.svg \
    qml/posqt/barcode_ico.svg \
    qml/posqt/app.js \
    qml/posqt/app_user.js \
    qml/posqt/SettingsLayoutView.qml \
    qml/posqt/TicketView_payactionBar.qml \
    qml/posqt/OnScreenKeyboard.qml

HEADERS += \
    src/connect.h \
    src/server/server.h \
    src/generics.h \
    src/defs.h \
    src/cmain.h \
    src/ticket.h \
    src/table.h \
    src/employee.h \
    src/data.h \
    src/floor.h \
    src/item.h \
    src/user.h \
    src/sale.h \
    src/userinfo.h \
    src/listmodel.h \
    src/menu.h \
    src/activetablelist.h \
    src/tablelistmodel.h \
    src/ticketlistmodel.h \
    src/menulistmodel.h \
    src/itemfactory.h \
    src/itemtraits.h \
    src/ordereditem.h \
    src/ticketinfo.h \
    src/menuinfo.h \
    src/reports.h \
    src/timehelp.h \
    src/settings.h \
    src/menuitem.h \
    src/print.h \
    src/result.h \
    src/empl_cmd.h \
    src/sale_cmd.h \
    src/command.h
