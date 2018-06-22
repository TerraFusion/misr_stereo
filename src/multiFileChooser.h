#ifndef MULTIFILECHOOSER_H_INCLUDED
#define MULTIFILECHOOSER_H_INCLUDED

#include <vector>

#include <qdialog.h>
#include <qstring.h>
#include <qstringlist.h>

class fileChooser;

class multiFileChooser : public QDialog
{
    Q_OBJECT

public:

    multiFileChooser( const QStringList & theCodeNames, QWidget * parent = 0 );

    virtual ~multiFileChooser();

    QString fileName( int index ) const;

protected slots:

    void SmartSearch_Click();

protected:

    QStringList codeNames;

	std::vector<fileChooser*> chooser;
};

#endif // MULTIFILECHOOSER_H_INCLUDED
