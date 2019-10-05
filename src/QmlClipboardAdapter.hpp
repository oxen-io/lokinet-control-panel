#ifndef QMLCLIPBOARDADAPTER_H
#define QMLCLIPBOARDADAPTER_H
 
#include <QClipboard>
#include <QObject>
 
/**
 * A helper class that allows cliboard access in QML.
 *
 * Source: Rüdiger Gad provided this code on his blog:
 * https://ruedigergad.com/2011/08/06/qml-and-clipboard-interaction/
 */
class QmlClipboardAdapter : public QObject
{
    Q_OBJECT
public:
    explicit QmlClipboardAdapter(QObject *parent = 0);
 
    Q_INVOKABLE void setText(QString text);
 
private:
    QClipboard *clipboard;
};
 
#endif // QMLCLIPBOARDADAPTER_H
