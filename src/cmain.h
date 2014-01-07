#ifndef CMAIN_HEADER
#define CMAIN_HEADER

#include<QObject>
#include<QtDebug>
//#include<QtQml/QQmlEngine>
//#include<QQmlContext>
#include<QMetaMethod>
//#include "server/server.h"
#include "data.h"
#include "tablelistmodel.h"
#include "ticketlistmodel.h"
#include "menulistmodel.h"
#include "menuinfo.h"

extern QObject* getRootObjectFromQml();

class CMain : public QObject {
        Q_OBJECT
        Q_PROPERTY(ListModel* activeSaleModel READ activeSaleModel NOTIFY activeSaleModelChanged)
        Q_PROPERTY(ListModel* tableListModel READ tableListModel NOTIFY tableListModelChanged)
        Q_PROPERTY(ListModel* tableSaleModel READ tableSaleModel NOTIFY tableSaleModelChanged)
        Q_PROPERTY(ListModel* allSaleModel READ allSaleModel NOTIFY allSaleModelChanged)
        Q_PROPERTY(int screenHeight READ screenHeight NOTIFY screenHeightChanged)
        Q_PROPERTY(int screenWidth READ screenWidth NOTIFY screenWidthChanged)
        Q_PROPERTY(QString bgFilePath READ bgFilePath NOTIFY bgFilePathChanged)

        QObject*        mRootObject         = nullptr;

        struct {
            std::unique_ptr<ListModel>  user_sales;
            std::unique_ptr<ListModel>  tables;
            std::unique_ptr<ListModel>  table_sales;
            std::unique_ptr<ListModel>  all_sales;
        } model;

        //ListModel*      mActiveSaleModel    = nullptr;
        //ListModel*      mTableModel         = nullptr;
        //ListModel*      ts_model            = nullptr;
        DataHandler*    mData               = nullptr;

    public:
        CMain(QObject* parent = nullptr);

    public:
        Q_INVOKABLE int  CheckLogin(const QString& passcode){ return check_user_login(passcode.toStdString()); }
        Q_INVOKABLE int  LoginUser(const int& id)           { return login_user(id);}
        Q_INVOKABLE void LogOutUser(void)                   { log_out_user(); }
        //! @brief  return users level based on password
        Q_INVOKABLE int check_user_level(const QString&);
        //! @brief  return users id based on password
        Q_INVOKABLE int get_user_id(const QString&);

                    int  check_user_login(const std::string&);
                    int  login_user(const int&);
                    void log_out_user();

    public:
        Q_INVOKABLE void refresh();
        Q_INVOKABLE void refreshActiveSales();
        Q_INVOKABLE void refreshTables();
        Q_INVOKABLE void ActivateTable(const int& id)       { activate_table(id);   }
                    void activate_table(const int&);
                    void select_menuitem(const int&);
                    void update_sale_list(int,int);
                    void update_table_list();
                    void update_ts_model();
        Q_INVOKABLE void reserve_table(const int&, const QString&);
        Q_INVOKABLE QString table_isreserved(const int&);
    public:
        //View methods
        //Q_INVOKABLE void SetRoot(QObject* root) { mRootObject = root; }
        Q_INVOKABLE void ShowLastScreen()       { }
        Q_INVOKABLE int  addNewSale(const int& table_id);
        Q_INVOKABLE void testCB(QObject* obj) {
                        if( obj->metaObject() )
                            std::cout << "Has meta object" << std::endl;
                    }
    public:
        //Property memthods
        inline ListModel* activeSaleModel()    { return model.user_sales.get(); }
        inline ListModel* tableListModel()     { return model.tables.get();     }
        inline ListModel* tableSaleModel()     { return model.table_sales.get();    }
        ListModel*        allSaleModel();

        int screenHeight() { return 1080;}
        int screenWidth() { return 1920;}

        QString bgFilePath() { return QString("rok_black.png");}
    signals:
        void tableSaleModelChanged();
        void activeSaleModelChanged();
        void tableListModelChanged();
        void screenHeightChanged();
        void screenWidthChanged();
        void bgFilePathChanged();
        void allSaleModelChanged();
};

#endif
