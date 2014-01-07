#ifndef MENUINFO_HEADER
#define MENUINFO_HEADER

#include <QObject>
#include <memory>

class DataHandler;
class ListModel;
class MenuItem;

class MenuInfo : public QObject {
    Q_OBJECT
    Q_PROPERTY(int catCount READ catCount NOTIFY catCountChanged)
    Q_PROPERTY(int selected READ selected NOTIFY selectedChanged)
    Q_PROPERTY(double item_price READ item_price NOTIFY item_priceChanged)
    Q_PROPERTY(QString item_name READ item_name NOTIFY item_nameChanged)
    Q_PROPERTY(ListModel* catModel READ catModel NOTIFY catModelChanged)
    Q_PROPERTY(ListModel* itemModel READ itemModel NOTIFY itemModelChanged)
    Q_PROPERTY(ListModel* subModel READ subModel NOTIFY subModelChanged)

    DataHandler&    data;
    int             selected_id     = -1;
    ListModel*      category_model  = nullptr;
    ListModel*      item_model      = nullptr;
    ListModel*      sub_model       = nullptr;
    MenuItem*       current_item    = nullptr;

//    bool select_menuitem_ptr    (const std::unique_ptr<MenuItem>&);
    void append_to_sale         (const int&, int);

    void update_cat_list();
    void update_item_list();

    public:
        MenuInfo(QObject* parent = nullptr);

    public:
    //Properties
        int         catCount        ( void );
        int         selected        ( void );
        double item_price    ( void );
        QString item_name    ( void );
        ListModel*  catModel        ( void );
        ListModel*  itemModel       ( void );
        ListModel*  subModel        ( void );

    public:
    //Methods
        Q_INVOKABLE void    place_order (const int& sale_id, const int& item_id, const int& qty);
        Q_INVOKABLE bool    select      (const int& item_id, int qty = 1);
        Q_INVOKABLE void    navigate    (const int& item_id);
        Q_INVOKABLE void    add_comment (const QString&);
        Q_INVOKABLE int     create_new_item(const int& parent_id, const QString& name);
        Q_INVOKABLE void    save_menuitem(const int& item_id);
        Q_INVOKABLE void    delete_menuitem(const int&);
        Q_INVOKABLE void    change_itemname(const QString&);
        Q_INVOKABLE void    set_item_price(const double&);
        Q_INVOKABLE bool    iscat(const int&);
        Q_INVOKABLE bool    hassubs(const int&);

    signals:
        void catModelChanged();
        void itemModelChanged();
        void subModelChanged();
        void catCountChanged();
        void selectedChanged();
        void item_priceChanged();
        void item_nameChanged();
};

#endif//#define MENUINFO_HEADER
