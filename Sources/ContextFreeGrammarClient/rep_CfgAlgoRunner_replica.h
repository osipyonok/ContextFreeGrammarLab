#ifndef REP_CFGALGORUNNER_REPLICA_H
#define REP_CFGALGORUNNER_REPLICA_H

// This is an autogenerated file.
// Do not edit this file, any changes made will be lost the next time it is generated.

#include <QtCore/qobject.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qvariant.h>
#include <QtCore/qmetatype.h>

#include <QtRemoteObjects/qremoteobjectnode.h>
#include <QtRemoteObjects/qremoteobjectpendingcall.h>
#include <QtRemoteObjects/qremoteobjectreplica.h>


class CfgAlgoRunnerReplica : public QRemoteObjectReplica
{
    Q_OBJECT
    Q_CLASSINFO(QCLASSINFO_REMOTEOBJECT_TYPE, "CfgAlgoRunner")
    Q_CLASSINFO(QCLASSINFO_REMOTEOBJECT_SIGNATURE, "aeee93e5f725811b9a507b94b7be8a30f96a0e8d")
    Q_PROPERTY(QList<QPair<QChar, QStringList>> solution READ solution NOTIFY solutionChanged)

public:
    CfgAlgoRunnerReplica() : QRemoteObjectReplica() { initialize(); }
    static void registerMetatypes()
    {
        static bool initialized = false;
        if (initialized)
            return;
        initialized = true;
        qRegisterMetaType<QVector<QPair<bool,QString>>>();
        qRegisterMetaTypeStreamOperators<QVector<QPair<bool,QString>>>();
        qRegisterMetaType<QList<QPair<QChar, QStringList>>>();
        qRegisterMetaTypeStreamOperators<QList<QPair<QChar, QStringList>>>();

    }

private:
    CfgAlgoRunnerReplica(QRemoteObjectNode *node, const QString &name = QString())
        : QRemoteObjectReplica(ConstructWithNode)
    {
        initializeNode(node, name);
    }

    void initialize() override
    {
        CfgAlgoRunnerReplica::registerMetatypes();
        QVariantList properties;
        properties.reserve(1);
        properties << QVariant::fromValue(QList<QPair<QChar, QStringList>>({}));
        setProperties(properties);
    }

public:
    virtual ~CfgAlgoRunnerReplica() {}

    QList<QPair<QChar, QStringList>> solution() const
    {
        const QVariant variant = propAsVariant(0);
        if (!variant.canConvert<QList<QPair<QChar, QStringList>>>()) {
            qWarning() << "QtRO cannot convert the property solution to type QList<QPair<QChar, QStringList>>";
        }
        return variant.value<QList<QPair<QChar, QStringList>> >();
    }


Q_SIGNALS:
    void solutionChanged(QList<QPair<QChar, QStringList>> solution);

public Q_SLOTS:
    void pushSolution(QList<QPair<QChar, QStringList>> solution)
    {
        static int __repc_index = CfgAlgoRunnerReplica::staticMetaObject.indexOfSlot("pushSolution(QList<QPair<QChar, QStringList>>)");
        QVariantList __repc_args;
        __repc_args << QVariant::fromValue(solution);
        send(QMetaObject::InvokeMetaMethod, __repc_index, __repc_args);
    }
    void add_rule(QChar __repc_variable_1, QVector<QPair<bool,QString>> __repc_variable_2)
    {
        static int __repc_index = CfgAlgoRunnerReplica::staticMetaObject.indexOfSlot("add_rule(QChar,QVector<QPair<bool,QString> >)");
        QVariantList __repc_args;
        __repc_args
            << QVariant::fromValue(__repc_variable_1)
            << QVariant::fromValue(__repc_variable_2)
        ;
        send(QMetaObject::InvokeMetaMethod, __repc_index, __repc_args);
    }
    void find_shortest_words()
    {
        static int __repc_index = CfgAlgoRunnerReplica::staticMetaObject.indexOfSlot("find_shortest_words()");
        QVariantList __repc_args;
        send(QMetaObject::InvokeMetaMethod, __repc_index, __repc_args);
    }
    void clear()
    {
        static int __repc_index = CfgAlgoRunnerReplica::staticMetaObject.indexOfSlot("clear()");
        QVariantList __repc_args;
        send(QMetaObject::InvokeMetaMethod, __repc_index, __repc_args);
    }

private:
    friend class QT_PREPEND_NAMESPACE(QRemoteObjectNode);
};

#if (QT_VERSION < QT_VERSION_CHECK(5, 5, 0))
#endif


QT_BEGIN_NAMESPACE
QT_END_NAMESPACE


#endif // REP_CFGALGORUNNER_REPLICA_H
