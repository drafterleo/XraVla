#include "cp_vocabularypage.h"
#include "c_xravlasteitem.h"
#include "c_xravlastemodel.h"
#include "cw_styledscrollbar.h"
#include "c_editorsfactory.h"
#include "cw_styledmessagebox.h"
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
#include <QClipboard>
#include <QMimeData>
#include <QResizeEvent>

CVocabularyPage::CVocabularyPage(QWidget *parent) :
    CPageWidget(parent)
{
    m_unknownFileName = QString("unknown.xvl");
    m_pixraEdit = 0;
    createPixraEdit("CFiguresEdit");

    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText("name");
    m_nameEdit->installEventFilter(this);

    m_specEdit = new QTextEdit(this);
    m_specEdit->setAcceptRichText(false);
    m_specEdit->setVerticalScrollBar(new CStyledScrollBar);
    m_specEdit->setPlaceholderText("description");
    m_specEdit->installEventFilter(this);

    m_protoPixraBtn = new CPushButton(QPixmap(), this);
    m_randomizeBtn = new CPushButton(QPixmap(":/images/iconRandomize.png"), this);

    m_protoPixraPopup = new CProtoPixraPopup(this);
    m_protoPixraPopup->hide();
    connect(m_protoPixraPopup, SIGNAL(pixraSelected(CAbstractPixra*)),
            this             , SLOT(protoPixraChanged(CAbstractPixra*)));

    m_addItemBtn = new CPushButton(QPixmap(":/images/iconPlus.png"));
    m_delItemBtn = new CPushButton(QPixmap(":/images/iconMinus.png"));
    m_toDownBtn = new CPushButton(QPixmap(":/images/iconDown.png"));
    m_toUpBtn = new CPushButton(QPixmap(":/images/iconUp.png"));
    m_loadItemsBtn = new CPushButton(QPixmap(":/images/iconOpen.png"));
    m_loadItemsInsBtn = new CPushButton(QPixmap(":/images/iconOpenPlus.png"));
    m_saveItemsBtn = new CPushButton(QPixmap(":/images/iconSave.png"));
    m_newVocabularyBtn = new CPushButton(QPixmap(":/images/iconNew.png"));

    m_lwBtnsLayout = new QHBoxLayout(this);
    m_lwBtnsLayout->setMargin(0);
    m_lwBtnsLayout->setSpacing(0);
    setMinMaxSize(m_addItemBtn, 33, 33);
    setMinMaxSize(m_delItemBtn, 33, 33);
    setMinMaxSize(m_loadItemsBtn, 33, 33);
    setMinMaxSize(m_loadItemsInsBtn, 33, 33);
    setMinMaxSize(m_saveItemsBtn, 33, 33);
    setMinMaxSize(m_newVocabularyBtn, 33, 33);
    setMinMaxSize(m_toDownBtn, 33, 33);
    setMinMaxSize(m_toUpBtn, 33, 33);
    m_lwBtnsLayout->addWidget(m_newVocabularyBtn);
    m_lwBtnsLayout->addWidget(m_loadItemsBtn);
    m_lwBtnsLayout->addWidget(m_loadItemsInsBtn);
    m_lwBtnsLayout->addSpacing(10);
    m_lwBtnsLayout->addWidget(m_saveItemsBtn);
    m_lwBtnsLayout->addStretch(1);
    m_lwBtnsLayout->addWidget(m_addItemBtn);
    m_lwBtnsLayout->addWidget(m_delItemBtn);
    m_lwBtnsLayout->addWidget(m_toDownBtn);
    m_lwBtnsLayout->addWidget(m_toUpBtn);

    m_lwPanel = new QWidget(this);
    m_lwPanel->setLayout(m_lwBtnsLayout);

    initStyles();
    m_nameEdit->setStyleSheet(m_wordStyle);
    m_specEdit->setStyleSheet(m_specStyle);

    m_newVocabularyBtn->setToolTip("New Vocabulary");
    m_loadItemsBtn->setToolTip("Load Items");
    m_loadItemsInsBtn->setToolTip("Load-Insert Items");
    m_saveItemsBtn->setToolTip("Save Items");
    m_addItemBtn->setToolTip("Add Item");
    m_delItemBtn->setToolTip("Delete Item");
    m_toDownBtn->setToolTip("Move Item Down");
    m_toUpBtn->setToolTip("Move Item Up");
    m_randomizeBtn->setToolTip("Randomizer");
    m_protoPixraBtn->setToolTip("Prototype");

    m_xravlasteView = new CXravlasteView(this);
    m_xravlasteView->setVerticalScrollBar(new CStyledScrollBar());
    m_xravlasteView->installEventFilter(this);
    initListView();

    connect(m_xravlasteView,  SIGNAL(changed(QModelIndex, QModelIndex)),
            this,        SLOT(listViewChanged(QModelIndex, QModelIndex)));
    connect(m_nameEdit,  SIGNAL(textChanged(QString)),
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

    connect(m_newVocabularyBtn  , SIGNAL(clicked()),
            this             , SLOT(newVocabulary()));
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

    setCurrentFileName(m_unknownFileName);
    m_modified = false;
    m_showInfo = false;
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
    m_pixraEdit->installEventFilter(this);
    m_pixraEdit->show();
}

void CVocabularyPage::initListView(void)
{
    CXravlasteModel *xmodel = dynamic_cast<CXravlasteModel *>(m_xravlasteView->model());
    if (xmodel) {
        xmodel->insertRow(0);
        m_xravlasteView->setCurrentIndex(xmodel->index(0, 0));
        updateProtoPixraBtn(xmodel->protoPixra());
        listViewChanged(m_xravlasteView->currentIndex(), QModelIndex());

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

    m_xravlasteView->setGeometry(xwlX, xwlY, xwlWidth, xwlHeight);
    m_pixraEdit->setGeometry(feX, feY, feWidth, feHeight);
    m_nameEdit->setGeometry(weX, weY, weWidth, weHeight);
    m_specEdit->setGeometry(seX, seY, seWidth, seHeight);

    m_protoPixraBtn->setGeometry(feX + feWidth - 58, feY + feHeight + 5, 60, 60);
    m_randomizeBtn->setGeometry(m_protoPixraBtn->pos().x() - 65, feY + feHeight + 5, 60, 60);

    m_lwPanel->setGeometry(QRect(layoutX, layoutY, layoutW, layoutH));
}

void CVocabularyPage::resizeEvent(QResizeEvent *event)
{
    relocateWidgets();
    if (!event->oldSize().isValid())
        listViewChanged(m_xravlasteView->currentIndex(), QModelIndex());
}

bool CVocabularyPage::eventFilter(QObject *target, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if (keyEvent->modifiers().testFlag(Qt::ControlModifier) &&
            keyEvent->modifiers().testFlag(Qt::ShiftModifier) &&
            (keyEvent->key() == Qt::Key_S ||
             keyEvent->key() == Qt::Key_P ||
             keyEvent->key() == Qt::Key_O ||
             keyEvent->key() == Qt::Key_Plus ||
             keyEvent->key() == Qt::Key_R ||
             keyEvent->key() == Qt::Key_I ||
             keyEvent->key() == Qt::Key_D ||
             keyEvent->key() == Qt::Key_C ||
             keyEvent->key() == Qt::Key_V )) {
            // double current item in the xravlaste
            this->keyPressEvent(keyEvent);
            return true;
        }

        if (target == m_nameEdit) {
            if (keyEvent->key() == Qt::Key_Up || keyEvent->key() == Qt::Key_Down) {
                //qDebug() << "up or down";
                QApplication::postEvent(m_xravlasteView, keyEvent);
                QApplication::processEvents();
                return true;
            }
        }
    }
    return CPageWidget::eventFilter(target, event);
}

void CVocabularyPage::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers().testFlag(Qt::ControlModifier) &&
        event->modifiers().testFlag(Qt::ShiftModifier) &&
        event->key() == Qt::Key_S){
        // <Ctrl + Shidt + S> : set vocabulary protoPixra to current Item
        CXravlasteModel *model = dynamic_cast<CXravlasteModel *> (m_xravlasteView->model());
        createPixraEdit(model->protoPixra()->metaObject()->className());
        relocateWidgets();
        m_pixraEdit->assignPixra(model->protoPixra());
        return;
    }

    if (event->modifiers().testFlag(Qt::ControlModifier) &&
        event->modifiers().testFlag(Qt::ShiftModifier) &&
        event->key() == Qt::Key_P) {
        // <Ctrl + Shift + P> : set current pixra as Proto
        CXravlasteModel *xmodel = dynamic_cast<CXravlasteModel *> (m_xravlasteView->model());
        xmodel->setProtoPixra(m_pixraEdit->pixra());
        updateProtoPixraBtn(m_pixraEdit->pixra());
        return;
    }

    if (event->modifiers().testFlag(Qt::ControlModifier) &&
        event->modifiers().testFlag(Qt::ShiftModifier) &&
        event->key() == Qt::Key_O) {
        // <Ctrl + Shift + O> : sort xravlaste items
        CXravlasteModel *xmodel = dynamic_cast<CXravlasteModel *> (m_xravlasteView->model());
        xmodel->sortItemsByName();
        return;
    }

    if (event->modifiers().testFlag(Qt::ControlModifier) &&
        event->modifiers().testFlag(Qt::ShiftModifier) &&
        event->key() == Qt::Key_I) {
        // <Ctrl + Shift + "I"> show/hide info
        m_showInfo = !m_showInfo;
        update();
        return;
    }

    if (event->modifiers().testFlag(Qt::ControlModifier) &&
        event->modifiers().testFlag(Qt::ShiftModifier) &&
        event->key() == Qt::Key_Plus) {
        // <Ctrl + Shift + "+"> add new proto item
        insertNewItem();
        m_nameEdit->setFocus();
        return;
    }

    if (event->modifiers().testFlag(Qt::ControlModifier) &&
        event->modifiers().testFlag(Qt::ShiftModifier) &&
        event->key() == Qt::Key_R) {
        // <Ctrl + Shift + "R"> add new randomized item
        insertNewItem();
        randomizePixra();
        m_nameEdit->setFocus();
        return;
    }

    if (event->modifiers().testFlag(Qt::ControlModifier) &&
        event->modifiers().testFlag(Qt::ShiftModifier) &&
        event->key() == Qt::Key_D) {
        // <Ctrl + Shift + "D"> add new randomized item
        doubleCurrItem();
        m_nameEdit->setFocus();
        return;
    }

    if (event->modifiers().testFlag(Qt::ControlModifier) &&
        event->modifiers().testFlag(Qt::ShiftModifier) &&
        event->key() == Qt::Key_C) {
        // <Ctrl + Shift + "C">
        copyCurrItemToClipboard();
        return;
    }

    if (event->modifiers().testFlag(Qt::ControlModifier) &&
        event->modifiers().testFlag(Qt::ShiftModifier) &&
        event->key() == Qt::Key_V) {
        // <Ctrl + Shift + "V">
        pasteItemFromClipboard();
        m_nameEdit->setFocus();
        return;
    }
}

void CVocabularyPage::listViewChanged(const QModelIndex & current, const QModelIndex & previous)
{
    bool modifiedTmp = m_modified;

    QAbstractItemModel *model = m_xravlasteView->model();

    if (previous.isValid()) {
        CXravlasteItem *prevItem = model->data(previous).value<CXravlasteItem *>();
        if (prevItem) {

        }
    }

    if (current.isValid()) {
        CXravlasteItem *currItem = model->data(current).value<CXravlasteItem *>();
        if (currItem) {
            enableEdits();
            m_nameEdit->setText(currItem->name);
            m_specEdit->setText(currItem->spec);
            if (currItem->pixra) {
                createPixraEdit(currItem->pixra->metaObject()->className());
                relocateWidgets();
                m_pixraEdit->assignPixra(currItem->pixra);
                m_pixraEdit->clearHIstory();
            }
            m_nameEdit->setFocus();
        }
    } else {
        m_nameEdit->setText("");
        m_specEdit->setText("");
        m_pixraEdit->clear();
        disableEdits();
    }

    m_modified = modifiedTmp;
    if (m_showInfo) {
        update();
    }
}

void CVocabularyPage::wordChanged(const QString & str)
{
    QModelIndex current = m_xravlasteView->currentIndex();
    if (current.isValid()) {
        CXravlasteItem *currItem = m_xravlasteView->model()->data(current).value<CXravlasteItem *>();
        if (currItem) {
            currItem->name = str;
            m_xravlasteView->update(current);
            m_xravlasteView->scrollTo(current);
            m_modified = true;
        }
    }
}

void CVocabularyPage::specChanged()
{
    QModelIndex current = m_xravlasteView->currentIndex();
    if (current.isValid()) {
        CXravlasteItem *currItem = m_xravlasteView->model()->data(current).value<CXravlasteItem *>();
        if (currItem) {
            currItem->spec = m_specEdit->toPlainText();
            m_xravlasteView->update(current);
            m_xravlasteView->scrollTo(current);
            m_modified = true;
        }
    }
}

void CVocabularyPage::pixraChanged()
{
    QModelIndex current = m_xravlasteView->currentIndex();
    if (current.isValid()) {
        CXravlasteItem *currItem = m_xravlasteView->model()->data(current).value<CXravlasteItem *>();
        if (currItem && currItem->pixra) {
            currItem->setPixra(m_pixraEdit->pixra());
            m_xravlasteView->update(current);
            m_xravlasteView->scrollTo(current);
            m_modified = true;
        }
    }
}


void CVocabularyPage::setCurrItem(int idx)
{
    QAbstractItemModel *model = m_xravlasteView->model();
    if (model) {
        QModelIndex modelIndex = model->index(idx, 0);
        m_xravlasteView->setCurrentIndex(modelIndex);
    }
}


void CVocabularyPage::insertNewItem()
{
    QAbstractItemModel *model = m_xravlasteView->model();
    int row = m_xravlasteView->currentIndex().row();
    if (model && model->insertRow(row + 1)) {
        if (row >= 0)
            m_xravlasteView->setCurrentIndex(model->index(row + 1, 0));
        else
            m_xravlasteView->setCurrentIndex(model->index(0, 0));

        m_modified = true;
    }
}

void CVocabularyPage::doubleCurrItem()
{
    QModelIndex prevCurrentIndex = m_xravlasteView->currentIndex();
    if (prevCurrentIndex.isValid()) {
        CXravlasteItem *prevCurrItem = m_xravlasteView->model()->data(prevCurrentIndex).value<CXravlasteItem *>();
        if (prevCurrItem && prevCurrItem->pixra) {
            insertNewItem();
            QModelIndex currentIndex = m_xravlasteView->currentIndex();
            if (prevCurrentIndex.isValid()) {
                CXravlasteItem *currItem = m_xravlasteView->model()->data(currentIndex).value<CXravlasteItem *>();
                if (currItem) {
                    currItem->setPixra(prevCurrItem->pixra);
                    listViewChanged(currentIndex, prevCurrentIndex);
                    m_modified = true;
                }
            }
        }
    }
}

void CVocabularyPage::copyCurrItemToClipboard()
{
    QModelIndex currentIndex = m_xravlasteView->currentIndex();
    if (currentIndex.isValid()) {
        QString xmlStr;
        QXmlStreamWriter xml(&xmlStr);
        xml.writeStartDocument();
        xml.writeDTD("<!DOCTYPE xravlaste>");
        xml.writeStartElement("xravlaste");
        CXravlasteItem *currItem = m_xravlasteView->model()->data(currentIndex).value<CXravlasteItem *>();
        currItem->writeXML(xml);
        xml.writeEndDocument();
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(xmlStr);
    }
}

void CVocabularyPage::pasteItemFromClipboard()
{
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    if (mimeData->hasText()) {
        QString xmlStr = mimeData->text();
        QDomDocument doc("xravlaste");
        if(doc.setContent(xmlStr, false)) {
            QDomElement root = doc.documentElement();
            if (root.tagName() == "xravlaste") {
                QModelIndex currentIndex = m_xravlasteView->currentIndex();
                int insRow = 0;
                if (currentIndex.isValid()) {
                    insRow = m_xravlasteView->currentIndex().row() + 1;
                }
                readXML(root, false, insRow);
            }
        }
    }
}

void CVocabularyPage::removeCurrItem()
{
    QAbstractItemModel *model = m_xravlasteView->model();
    if (model &&  m_xravlasteView->currentIndex().isValid()) {
        model->removeRow(m_xravlasteView->currentIndex().row());
        m_modified = true;
    }
}

void CVocabularyPage::moveCurrItemUp()
{
    CXravlasteModel *model = dynamic_cast<CXravlasteModel *> (m_xravlasteView->model());
    if (model && m_xravlasteView->currentIndex().isValid()) {
        model->moveRowToUp(m_xravlasteView->currentIndex().row());
        m_xravlasteView->scrollTo(m_xravlasteView->currentIndex());
        m_modified = true;
    }
}

void CVocabularyPage::moveCurrItemDown()
{
    CXravlasteModel *model = dynamic_cast<CXravlasteModel *> (m_xravlasteView->model());
    if (model && m_xravlasteView->currentIndex().isValid()) {
        model->moveRowToDown(m_xravlasteView->currentIndex().row());
        m_xravlasteView->scrollTo(m_xravlasteView->currentIndex());
        m_modified = true;
    }
}


void CVocabularyPage::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int x = m_xravlasteView->pos().x() - 2;
    //int w = m_ListView->width() + 2;
    painter.setPen(QPen(QColor(0x4E4E4E), 2));
    //painter.drawRect(x, 1, w, height() - 2);
    painter.drawLine(x, 0, x, height());
    if (m_showInfo) {
        QString infoStr = QString("Item Count: %1(%2)")
                            .arg(m_xravlasteView->model()->rowCount())
                            .arg(m_xravlasteView->currentIndex().row() + 1);
        painter.setPen(Qt::lightGray);
        painter.drawText(5, height() - 10, infoStr);
    }
    painter.end();
}


bool CVocabularyPage::readXML(const QDomElement & root, bool clearModel, int insRow)
{
    //QAbstractItemModel *xmodel = m_listView->model();
    CXravlasteModel *xmodel = dynamic_cast<CXravlasteModel *> (m_xravlasteView->model());

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

    m_xravlasteView->setCurrentIndex(xmodel->index(insRow, 0));

    return true;
}


void CVocabularyPage::loadVocabularyFromFile(const QString &fileName, bool clearModel, int insRow)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Can't open xvl file " << fileName;
    } else {
        QString errorStr;
        int errorLine;
        int errorColumn;

        QDomDocument doc("xravlaste");
        if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) {
            qDebug()  << "Error: Parse error at line " << errorLine << ", "
                      << "column " << errorColumn << ": "
                      << errorStr;
        } else {
            QDomElement root = doc.documentElement();
            if (root.tagName() != "xravlaste") {
                qDebug() << "Error: Not a xravlaste file";
            } else {
                readXML(root, clearModel, insRow);
                setCurrentFileName(fileName);
            }
        }

        file.close();
    }
}

void CVocabularyPage::loadItems(bool clearModel, int insRow)
{
    if (m_modified && saveRequest() == false) {
        return;
    }

    QFileInfo fileInfo(m_currentFileName);
    QString fileStr;

    if (fileInfo.exists()) {
        fileStr = fileInfo.dir().path();
    }

    QString xmlFileName = QFileDialog::getOpenFileName(this,
                                                       tr("Load Items"),
                                                       fileStr,
                                                       tr("XVL (*.xvl)"));

    fileInfo.setFile(xmlFileName);
    if (fileInfo.exists()) {
        loadVocabularyFromFile(xmlFileName, clearModel, insRow);
    }
}

void CVocabularyPage::loadItemsClear()
{
    loadItems(true, 0);
}

void CVocabularyPage::loadItemsIns()
{
    loadItems(false, m_xravlasteView->currentIndex().row() + 1);
}

bool CVocabularyPage::saveRequest()
{
    CStyledMessageBox msgBox(this);
    msgBox.setText("Vocabulary has not been saved.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    msgBox.setButtonText(QMessageBox::Cancel, tr("Back"));
    QPixmap msgIcon(":/images/iconWarning.png");
    msgBox.setIconPixmap(msgIcon);

    int reply = msgBox.exec();

    if (reply == QMessageBox::Save) {
        saveItems(false);
        return true;
    } else
    if (reply == QMessageBox::Cancel) {
        return false;
    }
    return true;
}

void CVocabularyPage::newVocabulary()
{
    if (m_modified && saveRequest() == false) {
        return;
    }

    CXravlasteModel *xmodel = dynamic_cast<CXravlasteModel *> (m_xravlasteView->model());
    if (xmodel) {
        xmodel->removeRows(0, xmodel->rowCount());
        insertNewItem();
        setCurrentFileName(m_unknownFileName);
        m_modified = true;
    }
}

void CVocabularyPage::writeXML(const QString & fileName)
{
    CXravlasteModel *model = dynamic_cast<CXravlasteModel *> (m_xravlasteView->model());
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

void CVocabularyPage::saveItems(bool saveAs)
{
    QFileInfo finfo(m_currentFileName);
    QString fileStr;

    if (finfo.exists()) {
        fileStr = m_currentFileName;
    } else {
        fileStr = m_unknownFileName;
    }

    if (saveAs || finfo.exists() == false) {
        fileStr = QFileDialog::getSaveFileName(this, tr("Save Items"), fileStr, tr("XVL (*.xvl)"));
    }

    if (fileStr.isEmpty() == false) {
        writeXML(fileStr);
        setCurrentFileName(fileStr);
        m_modified = false;
    }
}


void CVocabularyPage::setCurrentFileName(const QString & fileName)
{
    QFileInfo fileInfo(fileName);
    m_currentFileName = fileName;
    QString title = "XraVla";
    if (!m_currentFileName.isEmpty()) {
        title = fileInfo.fileName();
    }

    foreach(QWidget *widget, QApplication::topLevelWidgets()) {
      if(widget->objectName() == "MainWindow") {
          qobject_cast<QWidget*>(widget)->setWindowTitle(title);
          break;
      }
    }
}

void CVocabularyPage::disableEdits()
{
    m_pixraEdit->setEnabled(false);
    m_nameEdit->setEnabled(false);
    m_specEdit->setEnabled(false);
}

void CVocabularyPage::enableEdits()
{
    m_pixraEdit->setEnabled(true);
    m_nameEdit->setEnabled(true);
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
    CXravlasteModel *model = dynamic_cast<CXravlasteModel *> (m_xravlasteView->model());

    if (pixra && model && m_pixraEdit) {
        model->setProtoPixra(pixra);
//        createPixraEdit(pixra->metaObject()->className());
//        relocateWidgets();
//        m_pixraEdit->assignPixra(pixra);
        insertNewItem();
        m_pixraEdit->setFocus();
        updateProtoPixraBtn(pixra);
    }
}

void CVocabularyPage::randomizePixra()
{
    if (m_pixraEdit) {
        m_pixraEdit->randomize();
        //m_pixraEdit->setFocus();
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
                      "font-family: \"Verdana\";"
                      "font-size: 12pt;"
                      "border: 2px solid #538BC4;"
                   "}"
                   "QLineEdit:focus {"
                      "color: white;"
                      "font-family: \"Verdana\";"
                      "font-size: 12pt;"
                      "border: 2px solid #538BC4;"
                   "}";

    m_specStyle = "QTextEdit {"
                     "color: #91E22C;"
                     "background-color: transparent;"
                     //"background-color: #272822;"
                     "font-family: \"Verdana\";"
                     "font-size: 10pt;"
                     "border: 2px solid #466A7E;"
                  "}"
                  "QTextEdit:hover {"
                     "color: #91E22C;"
                     "font-family: \"Verdana\";"
                     "font-size: 10pt;"
                     "border: 2px solid #538BC4;"
                  "}"
                  "QTextEdit:focus {"
                     "color: #91E22C;"
                     "font-family: \"Verdana\";"
                     "font-size: 10pt;"
                     "border: 2px solid #538BC4;"
                  "}";
}


