
#ifndef COLUMNLIST_H
#define COLUMNLIST_H


#ifndef MAX_COLUMNS
#define MAX_COLUMNS 2
#endif

typedef struct tagCOLUMNLISTENTRY
{
    int     iColumnItemOffset;
    DWORD   dwId;               // Id des Eintrags: pid, cntThreads, ...
    BOOL    bChangeable;
    int     iFmt;               // Format der Spalte
    char*   pstText;            // Text in Header-Spalte
    int     cx;                 // Groesse der Spalte
}COLUMNLISTENTRY;


class CColumnList
{
public:
    COLUMNLISTENTRY m_ColumnList[MAX_COLUMNS];
    int              m_iFreeEntry;
    int              m_iActEntry;

    CColumnList() { m_iFreeEntry = 0; m_iActEntry = 0;};
    void InsertAt(int iPos, COLUMNLISTENTRY *pCol)
    {
        for (int i=m_iFreeEntry; i>iPos; i--)
        {
            m_ColumnList[i] = m_ColumnList[i-1];
        }
        m_ColumnList[iPos] = *pCol;
        m_iActEntry = m_iFreeEntry;
        m_iFreeEntry++;
    }
    int GetNext(void)
    {
        return m_ColumnList[m_iActEntry++].iColumnItemOffset;
    };
    BOOL  GetNextIsChangeable(int* piOffset)
    {
        *piOffset = m_ColumnList[m_iActEntry].iColumnItemOffset;
        return m_ColumnList[m_iActEntry++].bChangeable;
    };
    void BeginFromEnd(void)
    {
        m_iActEntry = 0;
    };
    void BeginFromStart(void)
    {
        m_iActEntry = m_iFreeEntry-1;
    };
    void Reset(void)
    {
        m_iActEntry = m_iFreeEntry-1;
        m_iFreeEntry = 0;
    };
    int GetColsUsed(void) { return m_iFreeEntry; };
    int GetIndexFromId(DWORD dwId)
    {
        BeginFromEnd();
        for (int i=0; i<m_iFreeEntry; i++)
        {
            if (m_ColumnList[i].dwId == dwId)
                return (i);
        }
        return -1;
    };
    void RemoveEntry(int iCol)
    {
        memcpy(&m_ColumnList[iCol], &m_ColumnList[iCol+1], 
            (GetColsUsed()-iCol)*sizeof(COLUMNLISTENTRY));
        m_iFreeEntry--;
    }
};

#endif