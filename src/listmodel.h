/*
 * Author: Christophe Dumez <dchris@gmail.com>
 * License: Public domain (No attribution required)
 * Website: http://cdumez.blogspot.com/
 * Version: 1.0
 */
#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QVariant>

class CMain;
class TicketInfo;

class ListItem: public QObject {
    Q_OBJECT

    public:
        ListItem(QObject* parent = 0) : QObject(parent) {}
        virtual ~ListItem() {}
        virtual QString id() const = 0;
        virtual QVariant data(int role) const = 0;
        virtual QHash<int, QByteArray> roleNames() const = 0;

    signals:
        void dataChanged();
};

class ListModel : public QAbstractListModel
{
    friend class CMain;
    friend class TicketInfo;
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

    public:
        ListModel() : m_prototype(0), m_list() {}
        explicit ListModel(ListItem* prototype, QObject* parent = 0);

        ListModel(const ListModel& pCopy) :
            QAbstractListModel(0),
            m_prototype(pCopy.m_prototype),
            m_list(pCopy.m_list)
        {
        }

        ~ListModel();
        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        Q_INVOKABLE void appendRow(ListItem* item);
        void appendRows(const QList<ListItem*> &items);
        void insertRow(int row, ListItem* item);
        void silentAppendRow(ListItem* item);
        Q_INVOKABLE bool removeRow(int row, const QModelIndex &parent = QModelIndex());
        bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
        ListItem* takeRow(int row);
        ListItem* find(const QString &id) const;
        QModelIndex indexFromItem( const ListItem* item) const;
        const QHash<int, QByteArray> roleNames() const;
        Q_INVOKABLE QVariantMap get(int idx) const {
            //http://www.qtcentre.org/threads/52850-How-to-get-data-from-TableView-QML-Desktop-Components#8
            QVariantMap map;
            foreach(int k, roleNames().keys()) {
                map[roleNames().value(k)] = data(index(idx, 0), k);
            }
            return map;
        }
        Q_INVOKABLE void clear();

        inline int count() const { return m_list.count(); }

    public:
        ListModel& operator=(const ListModel& pCopy)
        {
            m_prototype = pCopy.m_prototype;
            m_list = pCopy.m_list;
            return *this;
        }

    signals:
        void countChanged();

    private slots:
        void handleItemChange();

    public:
        ListItem* m_prototype;
        QList<ListItem*> m_list;
};

#endif // LISTMODEL_H
