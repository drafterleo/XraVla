#ifndef CP_VOCABULARYPAGE_H
#define CP_VOCABULARYPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QHBoxLayout>

#include "cp_pagewidget.h"
#include "cw_figuresedit.h"
#include "cw_xravlasteview.h"
#include "c_xravlastemodel.h"
#include "cw_pushbutton.h"
#include "cw_protopixrapopup.h"

class CVocabularyPage : public CPageWidget
{
    Q_OBJECT
public:
    explicit CVocabularyPage(QWidget *parent = 0);
    ~CVocabularyPage();

    inline CXravlasteModel *model() {return dynamic_cast<CXravlasteModel *>(m_listView->model());}
    inline bool isModified() {return m_modified;}

protected:
    void relocateWidgets();

    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *);

    void initStyles();

    void setMinMaxSize(QWidget *widget, int w, int h);

    void createPixraEdit(const QString &classNameStr);

    bool readXML(const QDomElement & root, bool clearModel = true, int insRow = 0);
    void writeXML(const QString & fileName);

    void loadItems(bool clearModel = true, int insRow = 0);
    void setFileName(const QString & fileName);

    void initListView(void);

    void updateProtoPixraBtn(CAbstractPixra *pixra);

public slots:
    void loadItemsClear();
    void loadItemsIns();
    void saveItems();

    void insertNewItem();
    void removeCurrItem();
    void moveCurrItemUp();
    void moveCurrItemDown();

protected slots:
    void listViewChanged(const QModelIndex & current, const QModelIndex & previous);
    void wordChanged(const QString & str);
    void specChanged();
    void pixraChanged();
    void protoPixraChanged(CAbstractPixra *pixra);

    void disableEdits();
    void enableEdits();

    void showProtoPixras();
    void randomizePixra();

private:
    QString m_fileName;

    bool m_modified;


    CAbstractEdit  *m_pixraEdit;
    CXravlasteView *m_listView;

    QLineEdit *m_wordEdit;
    QTextEdit *m_specEdit;

    QString m_wordStyle;
    QString m_specStyle;

    CPushButton *m_addItemBtn;
    CPushButton *m_delItemBtn;
    CPushButton *m_toDownBtn;
    CPushButton *m_toUpBtn;
    CPushButton *m_loadItemsBtn;
    CPushButton *m_loadItemsInsBtn;
    CPushButton *m_saveItemsBtn;

    QWidget *m_lwPanel;

    CPushButton *m_protoPixraBtn;
    CPushButton *m_randomizeBtn;

    CProtoPixraPopup *m_protoPixraPopup;

    QHBoxLayout *m_lwBtnsLayout;
};

#endif // CP_VOCABULARYPAGE_H
