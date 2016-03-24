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
    inline QString currentFileName() {return m_currentFileName;}
    void loadVocabularyFromFile(const QString &fileName, bool clearModel, int insRow);

protected:
    void relocateWidgets();

    /* virtuals */
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *);
    bool eventFilter(QObject *target, QEvent *event);
    void keyPressEvent(QKeyEvent *event);

    void initStyles();

    void setMinMaxSize(QWidget *widget, int w, int h);

    void createPixraEdit(const QString &classNameStr);

    bool readXML(const QDomElement & root, bool clearModel = true, int insRow = 0);
    void writeXML(const QString & fileName);

    void loadItems(bool clearModel = true, int insRow = 0);
    void setCurrentFileName(const QString & fileName);

    void initListView(void);

    void updateProtoPixraBtn(CAbstractPixra *pixra);

    bool saveRequest();

public slots:
    void newVocabulary();
    void loadItemsClear();
    void loadItemsIns();
    void saveItems(bool saveAs = true);

    void insertNewItem();
    void copyCurrItem();
    void removeCurrItem();
    void moveCurrItemUp();
    void moveCurrItemDown();
    void setCurrItem(int idx);

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
    QString m_currentFileName;
    QString m_unknownFileName;

    bool m_modified;
    bool m_showInfo;


    CAbstractEdit  *m_pixraEdit;
    CXravlasteView *m_listView;

    QLineEdit *m_nameEdit;
    QTextEdit *m_specEdit;

    QString m_wordStyle;
    QString m_specStyle;

    CPushButton *m_addItemBtn;
    CPushButton *m_delItemBtn;
    CPushButton *m_toDownBtn;
    CPushButton *m_toUpBtn;
    CPushButton *m_newVocabularyBtn;
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
