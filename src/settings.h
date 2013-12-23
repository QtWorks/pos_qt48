#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include "data.h"
#include "listmodel.h"

//! Class responsible for handling user settings
//! interractions. Modifying menu(s) & users.
class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ListModel* menuModel READ menuModel NOTIFY menuModelChanged)
    Q_PROPERTY(QString item_name READ item_name NOTIFY item_nameChanged)
    Q_PROPERTY(QString item_parentname READ item_parentname NOTIFY item_parentnameChanged)
    Q_PROPERTY(double item_price READ item_price NOTIFY item_priceChanged)
    Q_PROPERTY(int item_id READ item_id NOTIFY item_idChanged)
    Q_PROPERTY(int item_parentid READ item_parentid NOTIFY item_parentidChanged)

    Q_PROPERTY(ListModel* userModel READ userModel NOTIFY userModelChanged)
    Q_PROPERTY(QString user_name READ user_name NOTIFY user_nameChanged)
    Q_PROPERTY(QString user_first_name READ user_first_name NOTIFY user_first_nameChanged)
    Q_PROPERTY(QString user_last_name READ user_last_name NOTIFY user_last_nameChanged)
    Q_PROPERTY(QString user_password READ user_password NOTIFY user_passwordChanged)
    Q_PROPERTY(int user_id READ user_id NOTIFY user_idChanged)
    Q_PROPERTY(int user_merchant READ user_merchant NOTIFY user_merchantChanged)
    Q_PROPERTY(QString user_title READ user_title NOTIFY user_titleChanged)
    Q_PROPERTY(int user_level READ user_level NOTIFY user_levelChanged)

    DataHandler&    data;
    std::unique_ptr<ListModel>  menu_list;
    std::unique_ptr<ListModel>  user_list;
    MenuItem*       selection   = nullptr;
    User*           user        = nullptr;

public:
    Settings(QObject* parent = nullptr);
    virtual ~Settings() {}

public:
    //Menu Methods
    Q_INVOKABLE void select(const int&);
    Q_INVOKABLE void reset_menu();

    /// @brief Insert new item into menu
    /// @param parent_id Id for the inserted items parent
    /// @param name Name for the inserted item
    /// @return Id of the new item created
    Q_INVOKABLE int  new_item(const int&,const QString&);
    Q_INVOKABLE void delete_item(const int&);
    Q_INVOKABLE void save_item(const int&);
    Q_INVOKABLE void set_itemname(const QString&);
    /// @brief Modify menuitem price
    Q_INVOKABLE void set_itemprice(const double&);
    /// @brief Check if the item is modifiable
    /// @param Item id to be checked
    /// @return Returns false if item can not be modified. True otherwise
    Q_INVOKABLE bool menuitem_modifiable(const int& id);

    //User Methods
    /// @brief  Select a user from a list of available
    Q_INVOKABLE void    select_user(const int&);
    /// @brief  Reset userlist to its default view
    Q_INVOKABLE void    reset_user_list();
    /// @brief  Create new user
    Q_INVOKABLE int     create_new_user();
    /// @brief  Delete existing user
    Q_INVOKABLE void    delete_user();
    /// @brief  Set users passkey
    Q_INVOKABLE void    set_user_password(const QString&);
    /// @brief  Set user first name
    Q_INVOKABLE void    set_user_firstname(const QString&);
    /// @brief  Set user last name
    Q_INVOKABLE void    set_user_lastname(const QString&);
    /// @brief  Set users title
    Q_INVOKABLE void    set_user_title(const QString&);
    /// @brief  Set users merchant capability
    Q_INVOKABLE void    set_user_merchant(const int&);
    /// @brief  Set users level
    Q_INVOKABLE void    set_user_level(const int&);
    /// @brief  Get user first name from an id
    Q_INVOKABLE QString find_user_first_name(const int&);
    Q_INVOKABLE QString find_user_last_name(const int&);

    Q_INVOKABLE void    save_user(const int&);
public:
    //properties
    ListModel*  menuModel();
    QString     item_name();
    QString     item_parentname();
    double      item_price();
    int         item_id();
    int         item_parentid();

    ListModel*  userModel();
    QString     user_name();
    QString     user_last_name();
    QString     user_first_name();
    QString     user_title();
    QString     user_password();
    int         user_id();
    int         user_merchant();
    int         user_level();

signals:
    void item_nameChanged();
    void item_priceChanged();
    void item_idChanged();
    void menuModelChanged();
    void item_parentidChanged();
    void item_parentnameChanged();

    void userModelChanged();
    void user_nameChanged();
    void user_first_nameChanged();
    void user_last_nameChanged();
    void user_idChanged();
    void user_merchantChanged();
    void user_titleChanged();
    void user_passwordChanged();
    void user_levelChanged();
};

class UserSettingsItem : public ListItem
{
    QHash<int, QByteArray>  roles;
    std::unique_ptr<Item>   item;
    User*                   user = nullptr;
    void set_roles();

public:
    UserSettingsItem(User* user_ptr = nullptr);
    QHash<int, QByteArray> roleNames() const { return roles; }
    QString id() const { return QString("undefined"); }

    enum Roles {
        FirstNameRole = Qt::UserRole + 1,
        LastNameRole,
        FullNameRole,
        TitleRole,
        LevelRole,
        PasswordRole,
        MerchantRole,
        IdRole,
        RoleCount
    };
    QVariant data(int) const;
};

#endif // SETTINGS_H
