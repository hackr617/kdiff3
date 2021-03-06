/*
 * KDiff3 - Text Diff And Merge Tool
 *
 * SPDX-FileCopyrightText: 2002-2011 Joachim Eibl, joachim.eibl at gmx.de
 * SPDX-FileCopyrightText: 2018-2020 Michael Reeves reeves.87@gmail.com
 * SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MERGERESULTWINDOW_H
#define MERGERESULTWINDOW_H

#include "diff.h"
#include "FileNameLineEdit.h"
#include "MergeEditLine.h"
#include "Overview.h"

#include "selection.h"

#include <boost/signals2.hpp>

#include <QLineEdit>
#include <QPointer>
#include <QStatusBar>
#include <QTextLayout>
#include <QTimer>
#include <QWidget>

class QPainter;
class RLPainter;
class QScrollBar;
class KActionCollection;
class KToggleAction;

class KDiff3App;

class MergeResultWindow : public QWidget
{
    Q_OBJECT
  public:
    static QScrollBar* mVScrollBar;

    MergeResultWindow(QWidget* pParent, const QSharedPointer<Options>& pOptions, QStatusBar* pStatusBar);

    void init(
        const QVector<LineData>* pLineDataA, LineRef sizeA,
        const QVector<LineData>* pLineDataB, LineRef sizeB,
        const QVector<LineData>* pLineDataC, LineRef sizeC,
        const Diff3LineList* pDiff3LineList,
        TotalDiffStatus* pTotalDiffStatus
    );

    void setupConnections(const KDiff3App* app);

    inline void clearMergeList()
    {
        m_mergeLineList.clear();
    }

    static void initActions(KActionCollection* ac);

    void connectActions() const;
    void reset();

    bool saveDocument(const QString& fileName, QTextCodec* pEncoding, e_LineEndStyle eLineEndStyle);
    int getNrOfUnsolvedConflicts(int* pNrOfWhiteSpaceConflicts = nullptr);
    void choose(e_SrcSelector selector);
    void chooseGlobal(e_SrcSelector selector, bool bConflictsOnly, bool bWhiteSpaceOnly);

    int getMaxTextWidth();         // width of longest text line
    int getNofLines() const;
    int getVisibleTextAreaWidth(); // text area width without the border
    int getNofVisibleLines();
    QString getSelection();
    void resetSelection();
    void showNrOfConflicts();
    bool isDeltaAboveCurrent();
    bool isDeltaBelowCurrent();
    bool isConflictAboveCurrent();
    bool isConflictBelowCurrent();
    bool isUnsolvedConflictAtCurrent();
    bool isUnsolvedConflictAboveCurrent();
    bool isUnsolvedConflictBelowCurrent();
    bool findString(const QString& s, LineRef& d3vLine, int& posInLine, bool bDirDown, bool bCaseSensitive);
    void setSelection(int firstLine, int startPos, int lastLine, int endPos);
    e_OverviewMode getOverviewMode();

    void slotUpdateAvailabilities();

  public Q_SLOTS:
    void setOverviewMode(e_OverviewMode eOverviewMode);
    void setFirstLine(QtNumberType firstLine);
    void setHorizScrollOffset(int horizScrollOffset);

    void slotGoCurrent();
    void slotGoTop();
    void slotGoBottom();
    void slotGoPrevDelta();
    void slotGoNextDelta();
    void slotGoPrevUnsolvedConflict();
    void slotGoNextUnsolvedConflict();
    void slotGoPrevConflict();
    void slotGoNextConflict();
    void slotAutoSolve();
    void slotUnsolve();
    void slotMergeHistory();
    void slotRegExpAutoMerge();
    void slotSplitDiff(LineIndex firstD3lLineIdx, LineIndex lastD3lLineIdx);
    void slotJoinDiffs(LineIndex firstD3lLineIdx, LineIndex lastD3lLineIdx);
    void slotSetFastSelectorLine(LineIndex);
    void setPaintingAllowed(bool);
    void updateSourceMask();
    void slotStatusMessageChanged(const QString&);

    void slotChooseAEverywhere() { chooseGlobal(e_SrcSelector::A, false, false); }
    void slotChooseBEverywhere() { chooseGlobal(e_SrcSelector::B, false, false); }
    void slotChooseCEverywhere() { chooseGlobal(e_SrcSelector::C, false, false); }
    void slotChooseAForUnsolvedConflicts() { chooseGlobal(e_SrcSelector::A, true, false); }
    void slotChooseBForUnsolvedConflicts() { chooseGlobal(e_SrcSelector::B, true, false); }
    void slotChooseCForUnsolvedConflicts() { chooseGlobal(e_SrcSelector::C, true, false); }
    void slotChooseAForUnsolvedWhiteSpaceConflicts() { chooseGlobal(e_SrcSelector::A, true, true); }
    void slotChooseBForUnsolvedWhiteSpaceConflicts() { chooseGlobal(e_SrcSelector::B, true, true); }
    void slotChooseCForUnsolvedWhiteSpaceConflicts() { chooseGlobal(e_SrcSelector::C, true, true); }
    void slotRefresh();

    void slotResize();

    void slotCut();

    void slotSelectAll();

  Q_SIGNALS:
    void statusBarMessage(const QString& message);
    void scrollVertically(QtNumberType deltaY);
    void scrollMergeResultWindow(int deltaX, int deltaY);
    void modifiedChanged(bool bModified);
    void setFastSelectorRange(LineRef line1, LineCount nofLines);
    void sourceMask(int srcMask, int enabledMask);
    void resizeSignal();
    void selectionEnd();
    void newSelection();
    void updateAvailabilities();
    void showPopupMenu(const QPoint& point);
    void noRelevantChangesDetected();

  private:
    void merge(bool bAutoSolve, e_SrcSelector defaultSelector, bool bConflictsOnly = false, bool bWhiteSpaceOnly = false);
    QString getString(int lineIdx);
    void showUnsolvedConflictsStatusMessage();

    static QPointer<QAction> chooseAEverywhere;
    static QPointer<QAction> chooseBEverywhere;
    static QPointer<QAction> chooseCEverywhere;
    static QPointer<QAction> chooseAForUnsolvedConflicts;
    static QPointer<QAction> chooseBForUnsolvedConflicts;
    static QPointer<QAction> chooseCForUnsolvedConflicts;
    static QPointer<QAction> chooseAForUnsolvedWhiteSpaceConflicts;
    static QPointer<QAction> chooseBForUnsolvedWhiteSpaceConflicts;
    static QPointer<QAction> chooseCForUnsolvedWhiteSpaceConflicts;

    QSharedPointer<Options> m_pOptions = nullptr;

    const QVector<LineData>* m_pldA = nullptr;
    const QVector<LineData>* m_pldB = nullptr;
    const QVector<LineData>* m_pldC = nullptr;
    LineRef m_sizeA = 0;
    LineRef m_sizeB = 0;
    LineRef m_sizeC = 0;

    const Diff3LineList* m_pDiff3LineList = nullptr;
    TotalDiffStatus* m_pTotalDiffStatus = nullptr;

    int m_delayedDrawTimer = 0;
    e_OverviewMode mOverviewMode;
    QString m_persistentStatusMessage;

  private:
    static bool sameKindCheck(const MergeLine& ml1, const MergeLine& ml2);
    struct HistoryMapEntry {
        MergeEditLineList mellA;
        MergeEditLineList mellB;
        MergeEditLineList mellC;
        MergeEditLineList& choice(bool bThreeInputs);
        bool staysInPlace(bool bThreeInputs, Diff3LineList::const_iterator& iHistoryEnd);
    };
    typedef std::map<QString, HistoryMapEntry> HistoryMap;
    void collectHistoryInformation(e_SrcSelector src, Diff3LineList::const_iterator& iHistoryBegin, Diff3LineList::const_iterator& iHistoryEnd, HistoryMap& historyMap, std::list<HistoryMap::iterator>& hitList);

    MergeLineList m_mergeLineList;
    MergeLineList::iterator m_currentMergeLineIt;
    bool isItAtEnd(bool bIncrement, MergeLineList::iterator i)
    {
        if(bIncrement)
            return i != m_mergeLineList.end();
        else
            return i != m_mergeLineList.begin();
    }

    int m_currentPos;
    bool checkOverviewIgnore(MergeLineList::iterator& i);

    enum e_Direction
    {
        eUp,
        eDown
    };
    enum e_EndPoint
    {
        eDelta,
        eConflict,
        eUnsolvedConflict,
        eLine,
        eEnd
    };
    void go(e_Direction eDir, e_EndPoint eEndPoint);
    void calcIteratorFromLineNr(
        int line,
        MergeLineList::iterator& mlIt,
        MergeEditLineList::iterator& melIt);
    MergeLineList::iterator splitAtDiff3LineIdx(int d3lLineIdx);

    void paintEvent(QPaintEvent* e) override;

    int getTextXOffset();
    QVector<QTextLayout::FormatRange> getTextLayoutForLine(int line, const QString& s, QTextLayout& textLayout);
    void myUpdate(int afterMilliSecs);
    void timerEvent(QTimerEvent*) override;
    void writeLine(
        RLPainter& p, int line, const QString& str,
        enum e_SrcSelector srcSelect, e_MergeDetails mergeDetails, int rangeMark, bool bUserModified, bool bLineRemoved, bool bWhiteSpaceConflict
    );
    void setFastSelector(MergeLineList::iterator i);
    LineRef convertToLine(QtNumberType y);
    bool event(QEvent*) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseDoubleClickEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void resizeEvent(QResizeEvent* e) override;
    void keyPressEvent(QKeyEvent* e) override;
    void wheelEvent(QWheelEvent* e) override;
    void focusInEvent(QFocusEvent* e) override;

    bool canCut() { return hasFocus() && !getSelection().isEmpty(); }

    QPixmap m_pixmap;
    LineRef m_firstLine = 0;
    int m_horizScrollOffset = 0;
    LineCount m_nofLines = 0;
    int m_maxTextWidth = -1;
    bool m_bMyUpdate = false;
    bool m_bInsertMode = true;
    bool m_bModified = false;
    void setModified(bool bModified = true);

    int m_scrollDeltaX = 0;
    int m_scrollDeltaY = 0;
    int m_cursorXPos = 0;
    int m_cursorXPixelPos;
    int m_cursorYPos = 0;
    int m_cursorOldXPixelPos = 0;
    bool m_bCursorOn = true; // blinking on and off each second
    QTimer m_cursorTimer;
    bool m_bCursorUpdate = false;
    QStatusBar* m_pStatusBar;

    Selection m_selection;

    bool deleteSelection2(QString& str, int& x, int& y,
                          MergeLineList::iterator& mlIt, MergeEditLineList::iterator& melIt);
    bool doRelevantChangesExist();

    /*
      This list exists solely to auto disconnect boost signals.
    */
    std::list<boost::signals2::scoped_connection> connections;
  public Q_SLOTS:
    void deleteSelection();
    void pasteClipboard(bool bFromSelection);
  private Q_SLOTS:
    void slotCursorUpdate();
};

class QLineEdit;
class QTextCodec;
class QComboBox;
class QLabel;

class WindowTitleWidget : public QWidget
{
    Q_OBJECT
  private:
    QLabel*      m_pLabel;
    FileNameLineEdit*   m_pFileNameLineEdit;
    //QPushButton* m_pBrowseButton;
    QLabel*      m_pModifiedLabel;
    QLabel*      m_pLineEndStyleLabel;
    QComboBox*   m_pLineEndStyleSelector;
    QLabel*      m_pEncodingLabel;
    QComboBox*   m_pEncodingSelector;
    QSharedPointer<Options> m_pOptions;

  public:
    explicit WindowTitleWidget(const QSharedPointer<Options>& pOptions);
    QTextCodec* getEncoding();
    void setFileName(const QString& fileName);
    QString getFileName();
    void setEncodings(QTextCodec* pCodecForA, QTextCodec* pCodecForB, QTextCodec* pCodecForC);
    void setEncoding(QTextCodec* pEncoding);
    void setLineEndStyles(e_LineEndStyle eLineEndStyleA, e_LineEndStyle eLineEndStyleB, e_LineEndStyle eLineEndStyleC);
    e_LineEndStyle getLineEndStyle();

    bool eventFilter(QObject* o, QEvent* e) override;
  public Q_SLOTS:
    void slotSetModified(bool bModified);
    //private Q_SLOTS:
    //   void slotBrowseButtonClicked();
};

#endif
