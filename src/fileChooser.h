#ifndef FILECHOOSER_H_INCLUDED
#define FILECHOOSER_H_INCLUDED

#include <qwidget.h>

class QLineEdit;
class QPushButton;

class fileChooser : public QWidget
{
    Q_OBJECT

        Q_ENUMS( Mode )
        Q_PROPERTY( Mode mode READ mode WRITE setMode )
        Q_PROPERTY( QString fileName READ fileName WRITE setFileName )

public:

    fileChooser( QWidget * parent = 0 );

    virtual ~fileChooser();

    enum Mode { File, Directory };

    QString fileName() const;

    Mode mode() const;

public slots:

    void setFileName( const QString & fn );

    void setMode( Mode m );

signals:

    void fileNameChanged( const QString & );

private slots:

    void chooseFile();

private:

    QLineEdit *lineEdit;
    QPushButton *button;
    Mode md;
};

#endif //FILECHOOSER_H_INCLUDED
