#include "QmlClipboardAdapter.hpp"

#include <QApplication>

// QmlClipboardAdapter constructor
QmlClipboardAdapter::QmlClipboardAdapter(QObject *parent)
        : QObject(parent)
{
    clipboard = QGuiApplication::clipboard();
}

Q_INVOKABLE void QmlClipboardAdapter::setText(QString text) {
    clipboard->setText(text, QClipboard::Clipboard);
    clipboard->setText(text, QClipboard::Selection);
}

