#include "cp_vocabularypage.h"
#include "c_xravlasteitem.h"
#include "c_xravlastemodel.h"
#include "cw_styledscrollbar.h"
#include "c_editorsfactory.h"
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <QDomDocument>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QtDebug>
#include <QDesktopWidget>

CVocabularyPage::CVocabularyPage(QWidget *parent) :
    CPageWidget(parent)
{
    m_pixraEdit = 0;
    createPixraEdit("CFiguresEdit");

    m_wordEdit = new QLineEdit(this);
    m_specEdit = new QTextEdit(this);

    m_addItemBtn = new CPushButton(QPixmap(":/images/iconPlus.png"), this);
    m_delItemBtn = new CPushButton(QPixmap(":/images/iconMinus.png"), this);
    m_toDownBtn = new CPushButton(QPixmap(":/images/iconDown.png"), this);
    m_toUpBtn = new CPushButton(QPixmap(":/images/iconUp.png"), this);
    m_loadItemsBtn = new CPushButton(QPixmap(":/images/iconOpen.png"), this);
    m_loadItemsInsBtn = new CPushButton(QPixmap(":/images/iconOpenPlus.png"), this);
    m_saveItemsBtn = new CPushButton(QPixmap(":/images/iconSave.png"), this);

    m_protoPixraBtn = new CPushButton(QPixmap(), this);
    m_randomizeBtn = new CPushButton(QPixmap(":/images/iconRandomize.png"), this);

    m_protoPixraPopup = new CProtoPixraPopup(this);
    m_protoPixraPopup->hide();
    connect(m_protoPixraPopup, SIGNAL(pixraSelected(CAbstractPixra*)),
            this             , SLOT(protoPixraChanged(CAbstractPixra*)));

    initStyles();
    m_wordEdit->setStyleSheet(m_wordStyle);
    m_specEdit->setStyleSheet(m_specStyle);
    m_loadItemsBtn->setToolTip("Load Items");
    m_loadItemsInsBtn->setToolTip("Load-Insert Items");
    m_saveItemsBtn->setToolTip("Save Items");
    m_addItemBtn->setToolTip("Add Item");
    m_delItemBtn->setToolTip("Delete Item");
    m_toDownBtn->setToolTip("Move Item Down");
    m_toUpBtn->setToolTip("Move Item Up");
    m_randomizeBtn->setToolTip("Randomizer");
    m_protoPixraBtn->setToolTip("Prototype");

    m_lwBtnsLayout = new QHBoxLayout(this);
    m_lwBtnsLayout->setMargin(0);
    m_lwBtnsLayout->setSpacing(0);
    setMinMaxSize(m_addItemBtn, 33, 33);
    setMinMaxSize(m_delItemBtn, 33, 33);
    setMinMaxSize(m_loadItemsBtn, 33, 33);
    setMinMaxSize(m_loadItemsInsBtn, 33, 33);
    setMinMaxSize(m_saveItemsBtn, 33, 33);
    setMinMaxSize(m_toDownBtn, 33, 33);
    setMinMaxSize(m_toUpBtn, 33, 33);
    m_lwBtnsLayout->addWidget(m_loadItemsBtn);
    m_lwBtnsLayout->addWidget(m_loadItemsInsBtn);
    m_lwBtnsLayout->addWidget(m_saveItemsBtn);
    m_lwBtnsLayout->addStretch(1);
    m_lwBtnsLayout->addWidget(m_addItemBtn);
    m_lwBtnsLayout->addWidget(m_delItemBtn);
    m_lwBtnsLayout->addWidget(m_toDownBtn);
    m_lwBtnsLayout->addWidget(m_toUpBtn);

    m_lwPanel = new QWidget(this);
    m_lwPanel->setLayout(m_lwBtnsLayout);

    m_listView = new CXravlasteView(this);
    m_listView->setVerticalScrollBar(new CStyledScrollBar());
    initListView();

    connect(m_listView,  SIGNAL(changed(QModelIndex, QModelIndex)),
            this,        SLOT(listViewChanged(QModelIndex, QModelIndex)));
    connect(m_wordEdit,  SIGNAL(textChanged(QString)),
            this,        SLOT(wordChanged(QString)));
    connect(m_specEdit,  SIGNAL(textChanged()),
            this,        SLOT(specChanged()));

    connect(m_addItemBtn, SIGNAL(clicked()),
            this        , SLOT(insertNewItem()));
    connect(m_delItemBtn, SIGNAL(clicked()),
            this        , SLOT(removeCurrItem()));
    connect(m_toDownBtn , SIGNAL(clicked()),
            this        , SLOT(moveCurrItemDown()));
    connect(m_toUpBtn   , SIGNAL(clicked()),
            this        , SLOT(moveCurrItemUp()));

    connect(m_loadItemsBtn   , SIGNAL(clicked()),
            this             , SLOT(loadItemsClear()));
    connect(m_loadItemsInsBtn, SIGNAL(clicked()),
            this             , SLOT(loadItemsIns()));
    connect(m_saveItemsBtn   , SIGNAL(clicked()),
            this             , SLOT(saveItems()));

    connect(m_protoPixraBtn, SIGNAL(clicked()),
            this           , SLOT(showProtoPixras()));

    connect(m_randomizeBtn, SIGNAL(clicked()),
            this          , SLOT(randomizePixra()));

    setFileName("unknown.xvl");
    m_modified = false;

}

CVocabularyPage::~CVocabularyPage()
{
}

void CVocabularyPage::createPixraEdit(const QString &classNameStr)
{
    delete m_pixraEdit;
    m_pixraEdit = editFactoryInstance.createEditor(classNameStr);
    m_pixraEdit->setParent(this);
    connect(m_pixraEdit, SIGNAL(modified()), SLOT(pixraChanged()));
    m_pixraEdit->show();
}

void CVocabularyPage::initListView(void)
{
    CXravlasteModel *xmodel = dynamic_cast<CXravlasteModel *>(m_listView->model());
    if (xmodel) {
        xmodel->insertRow(0);
        m_listView->setCurrentIndex(xmodel->index(0, 0));
        updateProtoPixraBtn(xmodel->protoPixra());
        listViewChanged(m_listView->currentIndex(), QModelIndex());

//        CAbstractPixra *pixra = xmodel->protoPixra();
//        pixra->resize(50, 50);
//        QPixmap protoPix(xmodel->protoPixra()->size());
//        pixra->setColor(Qt::gray);
//        pixra->setFrameColor(Qt::black);
//        pixra->render(&protoPix, QPoint(), QRegion(), 0);
//        m_protoPixraBtn->setPixmap(protoPix);
    }
}

void CVocabularyPage::setMinMaxSize(QWidget *widget, int w, int h)
{
    if (widget) {
        widget->setMinimumSize(w, h);
        widget->setMaximumSize(w, h);
    }
}

void CVocabularyPage::relocateWidgets()
{
    int xwlWidth = 300;
    int xwlHeight = height() - 37;
    int xwlX = width() - xwlWidth - 2;
    int xwlY = 37;

//    int feWidth = 400;
//    int feHeight = 400;
//    int feX = (width() - feWidth - xwlWidth - 4)/2;
//    int feY = (height() - feHeight + 50)/2;


    int feWidth = width() - xwlWidth - 12;
    int feHeight = height() - 170;
    if (feWidth > feHeight)
        feWidth = feHeight;
    else
        feHeight = feWidth;
    int feX = (width() - feWidth - xwlWidth - 6)/2;
    int feY = (height() - feHeight + 40)/2;

    int weX = feX;
    int weY = feY - 95;
    int weWidth = feWidth;
    int weHeight = 25;

    int seX = weX;
    int seY = weY + weHeight + 5;
    int seWidth = weWidth;
    int seHeight = 60;

    int layoutW = xwlWidth;
    int layoutH = 33;
    int layoutY = xwlY - 35;
    int layoutX = xwlX + 1;

    m_listView->setGeometry(xwlX, xwlY, xwlWidth, xwlHeight);
    m_pixraEdit->setGeometry(feX, feY, feWidth, feHeight);
    m_wordEdit->setGeometry(weX, weY, weWidth, weHeight);
    m_specEdit->setGeometry(seX, seY, seWidth, seHeight);

    m_protoPixraBtn->setGeometry(feX + feWidth - 58, feY + feHeight + 5, 60, 60);
    m_randomizeBtn->setGeometry(m_protoPixraBtn->pos().x() - 65, feY + feHeight + 5, 60, 60);

    m_lwPanel->setGeometry(QRect(layoutX, layoutY, layoutW, layoutH));
}

void CVocabularyPage::resizeEvent(QResizeEvent *)
{
   relocateWidgets();
   listViewChanged(m_listView->currentIndex(), QModelIndex());
}

void CVocabularyPage::listViewChanged(const QModelIndex & current, const QModelIndex & previous)
{
    bool modifiedTmp = m_modified;

    QAbstractItemModel *model = m_listView->model();

    if (previous.isValid()) {
        CXravlasteItem *prevItem = model->data(previous).value<CXravlasteItem *>();
        if (prevItem) {

        }
    }

    if (current.isValid()) {
        CXravlasteItem *currItem = model->data(current).value<CXravlasteItem *>();
        if (currItem) {
            enableEdits();
            m_wordEdit->setText(currItem->word);
            m_specEdit->setText(currItem->spec);
            if (currItem->pixra) {
                createPixraEdit(currItem->pixra->metaObject()->className());
                relocateWidgets();
                m_pixraEdit->assignPixra(currItem->pixra);
                m_pixraEdit->clearHIstory();
            }
        }
    } else {
        m_wordEdit->setText("");
        m_specEdit->setText("");
        m_pixraEdit->clear();
        disableEdits();
    }

    m_modified = modifiedTmp;
}

void CVocabularyPage::wordChanged(const QString & str)
{
    QModelIndex current = m_listView->currentIndex();
    if (current.isValid()) {
        CXravlasteItem *currItem = m_listView->model()->data(current).value<CXravlasteItem *>();
        if (currItem) {
            currItem->word = str;
            m_listView->update(current);
            m_listView->scrollTo(current);
            m_modified = true;
        }
    }
}

void CVocabularyPage::specChanged()
{
    QModelIndex current = m_listView->currentIndex();
    if (current.isValid()) {
        CXravlasteItem *currItem = m_listView->model()->data(current).value<CXravlasteItem *>();
        if (currItem) {
            currItem->spec = m_specEdit->toPlainText();
            m_listView->update(current);
            m_listView->scrollTo(current);
            m_modified = true;
        }
    }
}

void CVocabularyPage::pixraChanged()
{
    QModelIndex current = m_listView->currentIndex();
    if (current.isValid()) {
        CXravlasteItem *currItem = m_listView->model()->data(current).value<CXravlasteItem *>();
        if (currItem && currItem->pixra) {
            currItem->setPixra(m_pixraEdit->pixra());
            m_listView->update(current);
            m_listView->scrollTo(current);
            m_modified = true;
        }
    }
}

void CVocabularyPage::insertNewItem()
{
    QAbstractItemModel *model = m_listView->model();
    int row = m_listView->currentIndex().row();
    if (model && model->insertRow(row + 1)) {
        if (row >= 0)
            m_listView->setCurrentIndex(model->index(row + 1, 0));
        else
            m_listView->setCurrentIndex(model->index(0, 0));

        m_modified = true;
    }
}

void CVocabularyPage::removeCurrItem()
{
    QAbstractItemModel *model = m_listView->model();
    if (model &&  m_listView->currentIndex().isValid()) {
        model->removeRow(m_listView->currentIndex().row());
        m_modified = true;
    }
}

void CVocabularyPage::moveCurrItemUp()
{
    CXravlasteModel *model = dynamic_cast<CXravlasteModel *> (m_listView->model());
    if (model && m_listView->currentIndex().isValid()) {
        model->moveRowToUp(m_listView->currentIndex().row());
        m_listView->scrollTo(m_listView->currentIndex());
        m_modified = true;
    }
}

void CVocabularyPage::moveCurrItemDown()
{
    CXravlasteModel *model = dynamic_cast<CXravlasteModel *> (m_listView->model());
    if (model && m_listView->currentIndex().isValid()) {
        model->moveRowToDown(m_listView->currentIndex().row());
        m_listView->scrollTo(m_listView->currentIndex());
        m_modified = true;
    }
}


void CVocabularyPage::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int x = m_listView->pos().x() - 2;
    //int w = m_ListView->width() + 2;
    painter.setPen(QPen(QColor(0x4E4E4E), 2));
    //painter.drawRect(x, 1, w, height() - 2);
    painter.drawLine(x, 0, x, height());
    painter.end();
}


bool CVocabularyPage::readXML(const QDomElement & root, bool clearModel, int insRow)
{
    //QAbstractItemModel *xmodel = m_listView->model();
    CXravlasteModel *xmodel = dynamic_cast<CXravlasteModel *> (m_listView->model());

    if (!xmodel)
        return false;

    if (clearModel) {
        xmodel->removeRows(0, xmodel->rowCount());
        insRow = 0;
        m_modified = false;
    } else {
        m_modified = true;
    }

    QDomNode node;
    int currRow = insRow;
    node = root.firstChild();
    while(!node.isNull()) {
        if(node.isElement()) {
            QDomElement itemElement = node.toElement();
            if (itemElement.tagName() == "item") {
                xmodel->insertRow(currRow);
                CXravlasteItem *xitem = xmodel->data(xmodel->index(currRow, 0)).value<CXravlasteItem *>();
                if (xitem)
                    xitem->readXML(itemElement);
                currRow ++;
            } else
            if (itemElement.tagName() == "proto") {
                QDomElement pixraElement = itemElement.firstChildElement("pixra");
                if (!pixraElement.isNull()) {
                    CAbstractPixra *pixra = pixraFactoryInstance.createPixra(pixraElement.attribute("type"));
                    if (pixra) {
                        pixra->readXML(pixraElement);
                        xmodel->setProtoPixra(pixra);
                        updateProtoPixraBtn(pixra);
                    }
                }
            }
        }
        node = node.nextSibling();
    }

    m_listView->setCurrentIndex(xmodel->index(insRow, 0));

    return true;
}


void CVocabularyPage::loadItemsClear()
{
    loadItems(true, 0);
}

void CVocabularyPage::loadItemsIns()
{
    loadItems(false, m_listView->currentIndex().row() + 1);
}

void CVocabularyPage::loadItems(bool clearModel, int insRow)
{
    QString xmlFileName = QFileDialog::getOpenFileName(this,
                                                       tr("Load Items"),
                                                       qApp->applicationDirPath(),
                                                       tr("XVL (*.xvl)"));

    if (xmlFileName.isEmpty())
        return;

    QFile file(xmlFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Can't open xvl file " << xmlFileName;

    } else {

        QString errorStr;
        int errorLine;
        int errorColumn;

        QDomDocument doc("xravlaste");
        if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) {
            qDebug()  << "Error: Parse error at line " << errorLine << ", "
                      << "column " << errorColumn << ": "
                      << errorStr;
        }  else {
            QDomElement root = doc.documentElement();
            if (root.tagName() != "xravlaste") {
                qDebug() << "Error: Not a xravlaste file";
            } else {
                readXML(root, clearModel, insRow);
                setFileName(xmlFileName);
            }
        }

        file.close();
    }
}

void CVocabularyPage::writeXML(const QString & fileName)
{
    CXravlasteModel *model = dynamic_cast<CXravlasteModel *> (m_listView->model());
    if (!model)
        return;

    QFile file(fileName);
    QXmlStreamWriter xml;
    if(file.open(QIODevice::WriteOnly)) {
        xml.setDevice(&file);
        xml.setAutoFormatting(true);

        xml.writeStartDocument();
        xml.writeDTD("<!DOCTYPE xravlaste>");
        xml.writeStartElement("xravlaste");
        xml.writeAttribute("version", "1.0");

        xml.writeStartElement("proto");
            if (model->protoPixra())
                model->protoPixra()->writeXML(xml);
        xml.writeEndElement();

        for (int i = 0; i < model->rowCount(); ++i) {
            CXravlasteItem *item = model->data(model->index(i), Qt::DisplayRole).value<CXravlasteItem *>();
            if (item)
                item->writeXML(xml);
        }

        xml.writeEndDocument();

        file.close();
     }
}

void CVocabularyPage::saveItems()
{
    QFileInfo finfo(m_fileName);
    QString fileStr;

    if (finfo.exists()) {
        fileStr = m_fileName;
    } else {
        fileStr = qApp->applicationDirPath() + "/unknown.xvl";
    }

    fileStr = QFileDialog::getSaveFileName(this, tr("Save Items"), fileStr, tr("XVL (*.xvl)"));

    if (fileStr.isEmpty())
        return;
    else
        setFileName(fileStr);

    writeXML(m_fileName);
    m_modified = false;
}


void CVocabularyPage::setFileName(const QString & fileName)
{
    m_fileName = fileName;

    foreach(QWidget *widget, QApplication::topLevelWidgets()) {
      if(widget->objectName() == "MainWindow") {
          QFileInfo fileInfo(m_fileName);
          qobject_cast<QWidget*>(widget)->setWindowTitle(fileInfo.fileName());
          break;
      }
    }
}

void CVocabularyPage::disableEdits()
{
    m_pixraEdit->setEnabled(false);
    m_wordEdit->setEnabled(false);
    m_specEdit->setEnabled(false);
}

void CVocabularyPage::enableEdits()
{
    m_pixraEdit->setEnabled(true);
    m_wordEdit->setEnabled(true);
    m_specEdit->setEnabled(true);
}

void CVocabularyPage::showProtoPixras()
{
//    QDesktopWidget *desktop = QApplication::desktop();
    QPoint globalPos = mapToGlobal(m_protoPixraBtn->pos());
    m_protoPixraPopup->move(globalPos);
//    if (!desktop->availableGeometry().contains(m_protoPixraPopup->geometry())) {
//        QRect cross = desktop->availableGeometry().intersected(m_protoPixraPopup->geometry());
//    }
    m_protoPixraPopup->show();
    m_protoPixraPopup->raise();
    m_protoPixraPopup->setFocus();
}

void CVocabularyPage::updateProtoPixraBtn(CAbstractPixra *pixra)
{
    if(pixra) {
        pixra->resize(50, 50);
        QPixmap protoPix(pixra->size());
        pixra->setColor(Qt::gray);
        pixra->setFrameColor(Qt::black);
        pixra->render(&protoPix, QPoint(), QRegion(), 0);
        m_protoPixraBtn->setPixmap(protoPix);
    }
}

void CVocabularyPage::protoPixraChanged(CAbstractPixra *pixra)
{
    CXravlasteModel *model = dynamic_cast<CXravlasteModel *> (m_listView->model());

    if (pixra && model && m_pixraEdit) {
        model->setProtoPixra(pixra);

        createPixraEdit(pixra->metaObject()->className());
        relocateWidgets();
        m_pixraEdit->assignPixra(pixra);
        m_pixraEdit->setFocus();

        updateProtoPixraBtn(pixra);
    }
}

void CVocabularyPage::randomizePixra()
{
    if (m_pixraEdit) {
        m_pixraEdit->randomize();
        m_pixraEdit->setFocus();
    }
}

void CVocabularyPage::initStyles()
{
    m_wordStyle  = "QLineEdit {"
                      "color: white;"
                      "background-color: transparent;"
                      "font-family: \"Verdana\";"
                      "font-size: 12pt;"
                      "border: 2px solid #466A7E;"
                   "}"
                   "QLineEdit:hover {"
                      "color: white;"
                      "background-color: transparent;"
                      "font-family: \"Verdana\";"
                      "font-size: 12pt;"
                      "border: 2px solid #538BC4;"
                   "}"
                   "QLineEdit:focus {"
                      "color: white;"
                      "background-color: transparent;"
                      "font-family: \"Verdana\";"
                      "font-size: 12pt;"
                      "border: 2px solid #538BC4;"
                   "}";

    m_specStyle = "QTextEdit {"
                     "color: #91E22C;"
                     "background-color: transparent;"
                     "font-family: \"Verdana\";"
                     "font-size: 10pt;"
                     "border: 2px solid #466A7E;"
                  "}"
                  "QTextEdit:hover {"
                     "color: #91E22C;"
                     "background-color: transparent;"
                     "font-family: \"Verdana\";"
                     "font-size: 10pt;"
                     "border: 2px solid #538BC4;"
                  "}"
                  "QTextEdit:focus {"
                     "color: #91E22C;"
                     "background-color: transparent;"
                     "font-family: \"Verdana\";"
                     "font-size: 10pt;"
                     "border: 2px solid #538BC4;"
                  "}";
}


