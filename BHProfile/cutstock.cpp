// cutstock.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <Mmsystem.h>
#include "cutstock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4996)

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

#if 0
/////////////////////////////////////////////////////////////////////////////
// CCutstockApp

BEGIN_MESSAGE_MAP(CCutstockApp, CWinApp)
	//{{AFX_MSG_MAP(CCutstockApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCutstockApp construction

CCutstockApp::CCutstockApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCutstockApp object

extern "C" _declspec(dllexport) void cutstock_main();
CCutstockApp theApp;
#endif // 0

/*------------------------------------------
   CUTSTOCK IN C  VER 2.2

   PROGRAMMED BY SUH, jeongdae
                 KIM, changgon
                 HAHN, kyuhun
                 JU, hientaek.
                 DOH, seung yong
                 KIM, donghee
   DIRECTED BY PROF. PARK, soondal
   SEOUL NATIONAL UNIVERSITY

   PROGRAMMED   ON  JUN  24, 1986
   LAST REVISED ON  DEC  17, 1999
 ------------------------------------------*/
/*
   features :
    (1) global static entcol data used.
    (2) LPRESE25.C  subroutines are used.
        btran(), ftran(), chuzr(), upbinv(), upsol()
*/
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>
#include  <ctype.h>

#ifdef  PI
#undef  PI
#endif
#define MAXR    1000 //10
#define MAXC    1000 // 10
#define MAXN    2    // 5      /*  MAX number of source type */
#define MAXL    6000 // 165        /*  MAX LENGTH                */

#define TOL1    1e-05
#define FLTMAX  1e+30
#define float   double

/*--------------------<  INPUT DATA  >------------------------*/
short    SL[MAXN],SC[MAXN],OL[MAXR],OQ[MAXR];
int    IPR, IT, NSOURCE, NPART;    /*  NROW : number of parts */

/*--------------------<  SIMPLEX DATA  >------------------------*/
float  BI[MAXR][MAXR],BBAR[MAXR], B[MAXR], PI[MAXC], C1[MAXC],
       AR[MAXR],ARINV[MAXR];
float  ZB, RMAX, CMIN;
short  A[MAXR][MAXC];
int    NR,NROW;

/*--------------------<  ENTCOL DATA  >------------------------*/
float  SCOST;
short  SLENGTH[MAXR];     /*   SLENGTH == a selected length   */
int    SSOURCE;

/*--------------------<  KNAPSACK DATA  >------------------------*/
short    RESOURCE,KA[MAXR],STATE[MAXR],NODE[MAXR][MAXL],D[MAXR][MAXL],XS[MAXR];
short    VAR[MAXR],DEMAND[MAXR][MAXL],INV[MAXR][MAXL];
int    FORW,NCOL,NSTAGE,OPTN,OPTD;
float  C[MAXR],COST[MAXR][MAXL],F[MAXR][MAXL];
float  OPTF;

/*--------------------<  control variables  >--------------------*/
char SW ;
int IERROR;
char ERR_MSG[100];
//FILE *fp;

/*---------------------------
            TITLE
 ---------------------------*/
void   title()
{
   printf("\n");
   printf("  ********************************************\n");
   printf("            WELCOME TO CUTSTOCK\n"           );
   printf("        FOR SOLVING CUTTING STOCK PROB\n"    );
   printf("  ********************************************\n\n");
   printf("         YOU CAN ENTER DATA BY \n\n");
   printf("           > INTERACTIVE MODE ----- I\n\n");
   printf("           > FILE MODE ------------ F\n\n");
   printf("         IF YOU WANT FILE MODE\n");
   printf("         MAKE DATA FILE  \n");
   printf("         BEFORE SELECT 'F' \n\n");
   printf("         ARE YOU READY ? NOW,\n");
   printf("          Select I or F ! ");
   scanf("%c",&SW);
   return;
}

#if 0
/*---------------------------
          FILEMODE
 ---------------------------*/
void   filemode()
{
   int  i;
//   char name[79];
   FILE *fin;
   fin=fopen("DATA.dat", "rt");
/*
   printf("\n\n    INPUT  FILE NAME : ");
   scanf("%s", name);
   if((fin=fopen(name,"rt"))==NULL){
      printf("\n\n     Caution !!! \n");
      sprintf(ERR_MSG, "Cannot open file %s.\n",name);
      IERROR=2; return;
   } 
   else { */
      fscanf(fin, "%d %d %d", &NSOURCE, &NPART, &IPR);
      for(i=1; i<=NSOURCE; i++) fscanf(fin,  "%d",  &SL[i]);
      for(i=1; i<=NSOURCE; i++) fscanf(fin,  "%d",  &SC[i]);
      for(i=1; i<=NPART; i++) fscanf(fin,  "%d",  &OL[i]);
      for(i=1; i<=NPART; i++) fscanf(fin,  "%d",  &OQ[i]);
//      fclose(fin);
}
#endif // 0

/*---------------------------
           INTEMODE
 ---------------------------*/
void   intemode()
{
   int i;
   char response;

   do {
      printf(" NUMBER OF STOCK LENGTH TYPE = \n");
      scanf("%d",&NSOURCE);
      printf(" NUMBER OF ORDER LENGTH TYPE = \n");
      scanf("%d",&NPART);
      printf(" <1> FOR ONLY OPTIMAL SOLUTIONS \n");
      printf(" <2> FOR DETAIL PRINTOUT \n");
      printf(" WHAT IS YOUR CHOICE ?\n");
//      scanf("%d",&IPR);
      printf("\nIS DATA CORRECT ? (Y/N) "); response = getchar(); response = getchar();
   } while(response == 'N'|| response == 'n');
   do {
      for(i=1; i<=NSOURCE; i++){
         printf(" STOCK LENGTH(%4d), STOCK COST(%4d)=\n\n",i,i);
         scanf("%d %d", &SL[i], &SC[i]);
      }
      printf("\nIS DATA CORRECT ? (Y/N) "); response = getchar(); response = getchar();
   } while(response == 'N'|| response =='n');
   do {
      for(i=1; i<=NPART; i++){
         printf(" ORDER LENGTH(%4d), ORDER QUANTITY(%4d)=\n\n",i,i);
         scanf("%d %d", &OL[i], &OQ[i]);
      }
      printf("\nIS DATA CORRECT ? (Y/N) "); response = getchar(); response = getchar();
   } while(response == 'N'|| response == 'n');
}

#if 1
/*------------------------
    PRINT INPUT DATA
-------------------------*/
void   pr_data()
{  int i;
   TRACE(" ***** INPUT DATA *****\n\n");
   TRACE(" STOCK LENGTH   /  COST \n");
      for(i=1; i<=NSOURCE; i++)
         TRACE("%4d              %4d \n", SL[i], SC[i]);
   TRACE("\n ORDER LENGTH  / QUANTITY \n");
      for(i=1; i<=NPART; i++)
         TRACE("%4d              %4d \n", OL[i], OQ[i]);
   TRACE(" ***** INPUT DATA END *****\n\n");
   TRACE("\n");
}
#endif // 0

/*---------------------------
           INISOLC
 ---------------------------*/
void   inisolc()
{
   int i,j;

   for(i=1; i<=NROW; i++){
      for(j=1; j<=NROW; j++){
         if(i!=j)  A[i][j] = 0;
         else      A[i][j] = (int) (SL[1]/OL[i]);  /* use first source */
      }
   }
   for(i=1; i<=NROW; i++){
      for(j=1; j<=NROW; j++){
         if(i==j) BI[i][j] = 1.0/A[i][j];
         else     BI[i][j] = 0;
      }
   }
   for(i=1; i<=NROW; i++){
      BBAR[i]=0;
      for(j=1; j<=NROW; j++)
         BBAR[i] += BI[i][j] * OQ[j];
   }

   for(i=1; i<=NROW; i++){
      C1[i] = SC[1];
      SLENGTH[i] = SL[1];
   }

   ZB=0;
   for(i=1; i<=NROW; i++) ZB += C1[i]*BBAR[i] ;
#if 0
   if (IPR>1){
      fprintf(fp,"\n  INITIAL CUTTING PATTERN \n");
      for(i=1; i<=NROW; i++){
         fprintf(fp," PATTERN(%4d)\n",i);
         fprintf(fp," STOCK LENGTH=%4d   QUANTITY=%4f\n", SL[1],BBAR[i]);
         for(j=1; j<=NROW; j++){
            if (A[j][i]==0) continue;
            fprintf(fp," ORDER LENGTH=%4d NUMBER OF  CUT =%4d \n"
                   ,OL[i],A[j][i]);
         }
      }
      fprintf(fp,"\n COST SUM OF THE USED STOCK = %f\n", ZB);
   }
#endif // 0
   return;
}

/*---------------------------
            BTRAN
  : subroutine in LPRESE25.C
 ---------------------------*/
void   btran()
{
   int  i, j;

   for(i=1; i<=NROW; i++){
      PI[i] = 0;
      for(j=1; j<=NROW; j++) PI[i] += C1[j] * BI[j][i] ;
   }
#if 0
   if (IPR>1){
      fprintf(fp,"*** BTRAN : CALCULATING PI *** \n\n" );
      for(i=1; i<=NROW; i++) fprintf(fp,"  %.5f  ", PI[i]);
   }
#endif // 0
   return;
}

/*---------------------------
           DPKNAP
    Knapsack prob by DP
 ----------------------------*/
void dpknap()
{
   int i,j,k,nvar,nstate,nstate1,stage,stage1,invent,tempn;
   float temp;

/* formulation */
   NSTAGE = NCOL; STATE[1] = 1;
   for(i=1; i<=NSTAGE; i++) VAR[i] = (int)(RESOURCE/KA[i])+1;
   for(i=2; i<=NCOL; i++) STATE[i] = RESOURCE + 1;
   for(i=1; i<=NSTAGE; i++){
      nstate=VAR[i];
      for(j=1; j<=nstate; j++) NODE[i][j] = j;
   }
   for(i=1; i<=NCOL; i++){
      nstate = VAR[i];
      for(j=2; j<=nstate; j++) DEMAND[i][j] = (j-1)*KA[i];
      DEMAND[i][1] = 0;
   }
   for(i=1; i<=NCOL; i++){
      nstate = VAR[i];
      for(j=2; j<=nstate; j++) COST[i][j] = (j-1)*C[i];
      COST[i][1] = 0;
   }
/* initialization  */
   stage = NSTAGE; nstate=STATE[NSTAGE];
   nstate1 = VAR[stage];

   for(j=1; j<= nstate; j++){F[stage][j] = 0; D[stage][j] = 0;}
   for(j=1; j<= nstate; j++){
      for(k=1; k<= nstate1; k++){
         invent = RESOURCE - (j-1);
         if (invent < DEMAND[stage][k]) continue;
         temp = COST[stage][k];
         if (F[stage][j] < temp){
            F[stage][j] = temp; D[stage][j] = k;
            INV[stage][j] = invent - DEMAND[stage][k];
         }
      }
   }
/*  DP loop  */
   for(i=2; i<= NSTAGE; i++){
      stage=NSTAGE-i+1; stage1 = stage + 1; /* backward */
      nvar = VAR[stage]; nstate = STATE[stage]; nstate1 = STATE[stage1];

      for(j=1; j<=nstate; j++){F[stage][j] = 0; NODE[stage][j] = 0; D[stage][j] = 0;}
      for(j=1; j<=nstate; j++){
         temp = 0; tempn = 0;
         for(k=1; k<=nvar; k++){
            invent = RESOURCE - (j-1);
            if (invent < DEMAND[stage][k]) continue;
            invent -= DEMAND[stage][k];
            tempn = RESOURCE - invent + 1;
            temp = COST[stage][k] + F[stage1][tempn];
            if (F[stage][j] < temp){
               F[stage][j] = temp; D[stage][j] = k; NODE[stage][j] = tempn;
               invent = INV[stage1][tempn];
            }
         }
         INV[stage][j] = invent;
      }
   }

   OPTF = F[1][1]; OPTN = 1; OPTD = D[1][1];
   XS[1] = OPTD; nstate = NODE[1][OPTN];
   for(i=2; i<=NSTAGE; i++){
      XS[i] = D[i][nstate]; nstate = NODE[i][nstate];
   }
   for(i=1; i<=NSTAGE; i++){
      if (XS[i] <=1 ) XS[i] = 0; else XS[i] -= 1;
   }
#if 0
   if (IPR>1){
      fprintf(fp,"\n***  DPKNAP : SOLVE KNAPSACK PROBLEM  ***\n");
      fprintf(fp,"      OPTIMAL  POLICY    \n");
      for(j=1; j<=NPART; j++) fprintf(fp,"  %3d  ", XS[j]);
      fprintf(fp,"\n\n");
   }
#endif // 0
}

/*---------------------------
           ENTCOL
   column generation function
 ---------------------------*/
void   entcol()
{
   int   i, j, k;
   int   tempx[MAXR];
   float val, maxval;

   for(i=1; i<=NPART; i++)
      XS[i] = 0;
   /*  Check Pi_value before column generation */
   /*           not implimented yet.           */

   /* SLACK ENTERING */
   for(i=1; i<=NPART; i++){
      if(PI[i] < -TOL1){
         XS[i] = -1;
         SCOST = 0.0;
         for(j=1; j<=NSOURCE; j++)
            if( OL[i] <= SL[j]) SSOURCE = j;
#if 0
         if (IPR>1){
            fprintf(fp,"\n***  ENTCOL : GENERATE ENTERING COLUMN  ***\n");
            for(j=1; j<=NPART; j++) fprintf(fp,"  %3d  ", XS[j]);
            fprintf(fp,"\n\n");
         }
#endif // 0
         return;
      }
   }

   /*  Find integer solution  by dynamic programming */

   maxval=0.0;
   /*     for NSOURCE type of source length         */
   for(k=1; k<=NSOURCE; k++){
      /* DP data formation */
      NCOL = NROW;
      RESOURCE=SL[k];
      for(i=1; i<=NROW; i++) KA[i] = OL[i];
      for(i=1; i<=NROW; i++) C[i] = PI[i];

      /* Call dynamic program */
      dpknap();

      /*  Check optimal condition  Zj -Cj<=0  */
      /* Select source type by maxval          */
      val=0.0;
      for(i=1; i<=NPART; i++) val += XS[i] * PI[i];
      if( (val - SC[k]) > maxval ){
         maxval =val - SC[k];
         SSOURCE = k;
         SCOST = SC[k];
         /* Return a generated entering column */
         for(j=1; j<=NPART;j++) tempx[j]=XS[j];
      }
   } /* End of Selection loop */
   for(j=1; j<=NPART;j++) XS[j]=tempx[j];

   CMIN = -maxval;

#if 0
   if (IPR>1){
      fprintf(fp,"\n*** ENTCOL : GENERATE ENTERING COLUMN ***\n");
      for(j=1; j<=NPART; j++) fprintf(fp,"  %3d  ", XS[j]);
      fprintf(fp,"\n  Selected source length =%d", SL[SSOURCE]);
      fprintf(fp,"\n  Reduced  cost          =%.6f", maxval);
      fprintf(fp,"\n\n");
   }
#endif // 0
   return;
}

/*---------------------------
           FTRAN
  : subroutine in LPRESE25.C
      < conversion table >
       XS[.] <-- A[.][JP]
 ---------------------------*/
void   ftran()
{
   int  i, j;

/*  Note :
  no index variable is needed such that JP, NCOL, N1.
*/
   for(i=1; i<=NROW; i++)  {
      AR[i] = 0;
      for(j=1; j<=NROW; j++) AR[i] += BI[i][j] * XS[j] ;
   }
#if 0
   if (IPR>1){
      fprintf(fp,"\n*** FTRAN : UPDATE COLUMN ***\n");
      for(i=1; i<=NROW; i++) fprintf(fp,"  %.5f  ", AR[i]);
   }
#endif // 0
   return;
}

/*---------------------------
            CHUZR
  : subroutine in LPRESE25.C
 ---------------------------*/
void   chuzr()
{
   int i;
   float temp;

   NR = 0; RMAX = 1.0E+30;
   for(i=1; i<=NROW; i ++){
      temp  = 1.0E+30;
      if(AR[i] > 0) temp = BBAR[i] / AR[i];
      if(RMAX > temp){
         RMAX = temp; NR = i;
      }
   }
#if 0
   if (IPR>1){
      fprintf(fp,"\n*** CHUZR : PIVOT ROW *** \n");
      fprintf(fp,"\n  %d -th basis  is leaving, at min ratio  %f \n",NR,RMAX) ;
   }
#endif // 0
   return;
}

/*---------------------------
           UPBINV
  : subroutine in LPRESE25.C
 ---------------------------*/
void   upbinv()
{
   int i, j;

   for(i=1; i<=NROW; i++) ARINV[i] = - AR[i] / AR[NR];
   ARINV[NR] = 1 / AR[NR];

   for(i=1; i<=NROW; i++){
      if (i != NR){
         for(j=1; j<=NROW; j++) BI[i][j] += ARINV[i] * BI[NR][j];
      }
   }
   for(j=1; j<=NROW; j++) BI[NR][j] = ARINV[NR] * BI[NR][j];
#if 0
   if (IPR>1){
      fprintf(fp,"\n*** UPBINV: updates B INVERSE ***\n");
      fprintf(fp,"\n(BI(i,j),j=1,NROW),i=1,NROW\n");
      for(i=1; i<=NROW; i++)
         for(j=1; j<=NROW; j++)
            if (j%5 != 0 && j != NROW) fprintf(fp,"%10.3G  ",BI[i][j]);
            else fprintf(fp,"%10.3G\n",BI[i][j]);
   }
#endif // 0
   return;
}

/*---------------------------
            UPSOL
  : subroutine in LPRESE25.C
 ---------------------------*/
void   upsol()
{
   int  i;

   for(i=1; i<=NROW; i++) BBAR[i] += -AR[i]*RMAX;
   BBAR[NR] = RMAX ;

   ZB += CMIN*RMAX ;

#if 0
   if (IPR>1){
      fprintf(fp,"\n*** UPSOL: UPDATED SOLUTION ***\n");
      fprintf(fp," index, basic variable solution\n");
      for(i=1; i<=NROW; i++)
         fprintf(fp,"\n%5d  %10.3G",i,BBAR[i]);
      fprintf(fp,"\n\nThe value of the objective function = %g\n",ZB);
   }
#endif // 0
   return;
}

void   changecol()
{
   int i;

   for(i=1; i<=NROW; i++) A[i][NR] = XS[i];
   C1[NR] = SCOST;
   SLENGTH[NR] = SL[SSOURCE];
   return;
}

#if 0
void   optsolc_org()
{
   int  i, j ;

   fprintf(fp, "\n*** OPTIMAL CUTTING PATTERN ***\n");
   for(i=1; i<=NROW; i++){
      if (BBAR[i] > TOL1){
         fprintf(fp, "\n Pattern(%d) \n", i);
         fprintf(fp, "   Stock length  %5d    ", SLENGTH[i]);
         fprintf(fp, "Quantity = %.5f \n", BBAR[i]);
         for(j=1; j<=NROW; j++){
            if (A[j][i] != 0){
               fprintf(fp,  "   Order length %5d    ", OL[j]);
               fprintf(fp, "Number of Cut = %5d \n", A[j][i]);
            }
         }
      }
   }

   ZB = 0.0;
   for(i=1; i<=NROW; i++) ZB += C1[i] * BBAR[i] ;
   fprintf(fp, "\n Minimum stock cost = %.5f\n", ZB);
   fprintf(fp, "\n Number of Iteration = %3d\n", IT);
   return;
}
#endif

typedef struct _PATTERN_PART
{
    int iLength;
    int iCount;
} PATTERN_PART;

typedef struct _CUTSTOCK_DATA
{
    int NPart;
    int NSource;                    /* Nbr of required parts and supply parts */
    bool Verbose;
    bool Fractional;
    vector <float> StockLengths;
    vector <float> StockCost;
    vector <int>   StockUsed;
    vector <float> Partlengths;
    InputMap       PartQty;         /* required part lengths and quanties */
    InputMap       totparts;        /* for each length, sum of parts cut from all patterns */
    vector <float> StockusedByPattern;  /* required nbr of pieces for each pattern */
    vector <int>   IStockUsedByPattern; /* integer version */
    vector <vector<PATTERN_PART>> A;             /* array of  columns (patterns) and rows (part lengths) */
    vector <float> C1;                  /* */
} CUTSTOCK_DATA;


inline float totCost(CUTSTOCK_DATA* pData)
{
    vector <float>::iterator fIter;
    vector <int>::iterator iIter;
    float  result = 0.0f;

    ASSERT(pData->C1.size() == pData->IStockUsedByPattern.size());
    for (fIter = pData->C1.begin(), iIter = pData->IStockUsedByPattern.begin(); fIter != pData->C1.end(); fIter++, iIter++)
    {
        result += (*fIter)*(*iIter);
    }
    return result;
}

/* copy a case record from "FromRec" to "ToRec" */
void copydata(CUTSTOCK_DATA* FromRec, CUTSTOCK_DATA* ToRec)
{
    vector <float>::iterator fIter;
    vector <int>::iterator iIter;
    InputMap::iterator mIter;
    vector <vector<PATTERN_PART>>::iterator vvIter;


    ToRec->NPart        = FromRec->NPart;
    ToRec->NSource      = FromRec->NSource;
    ToRec->Verbose      = FromRec->Verbose;
    ToRec->Fractional   = FromRec->Fractional;
    ToRec->StockLengths.clear();
    for (fIter = FromRec->StockLengths.begin(); fIter != FromRec->StockLengths.end(); fIter++)
        ToRec->StockLengths.push_back(*fIter);
#if 1
    ToRec->StockCost.clear();
    for (fIter = FromRec->StockCost.begin(); fIter != FromRec->StockCost.end(); fIter++)
        ToRec->StockCost.push_back(*fIter);
    ToRec->StockUsed.clear();
    for (iIter = FromRec->StockUsed.begin(); iIter != FromRec->StockUsed.end(); iIter++)
        ToRec->StockUsed.push_back(*iIter);
    ToRec->Partlengths.clear();
    for (fIter = FromRec->Partlengths.begin(); fIter != FromRec->Partlengths.end(); fIter++)
        ToRec->Partlengths.push_back(*fIter);
    ToRec->PartQty.clear();
    for (mIter = FromRec->PartQty.begin(); mIter != FromRec->PartQty.end(); mIter++)
        ToRec->PartQty[mIter->first] = mIter->second;
    ToRec->totparts.clear();
    for (mIter = FromRec->totparts.begin(); mIter != FromRec->totparts.end(); mIter++)
        ToRec->totparts[mIter->first] = mIter->second;
    ToRec->StockusedByPattern.clear();
    for (fIter = FromRec->StockusedByPattern.begin(); fIter != FromRec->StockusedByPattern.end(); fIter++)
        ToRec->StockusedByPattern.push_back(*fIter);
    ToRec->IStockUsedByPattern.clear();
    for (iIter = FromRec->IStockUsedByPattern.begin(); iIter != FromRec->IStockUsedByPattern.end(); iIter++)
        ToRec->IStockUsedByPattern.push_back(*iIter);

    ToRec->A.clear();
    for (vvIter = FromRec->A.begin(); vvIter != FromRec->A.end(); vvIter++)
    {
        vector<PATTERN_PART> pattern(*vvIter);
        ToRec->A.push_back(pattern);
#if 0
        for (ppIter = vvIter->begin(); ppIter != vvIter->end(); ppIter++)
        {
            mIter = pData->totparts.find(ppIter->iLength);
            if (mIter != pData->totparts.end())
            {
                mIter->second += (*iIter)*ppIter->iCount;
            }
        }
#endif
    }
    ToRec->C1.clear();
    for (fIter = FromRec->C1.begin(); fIter != FromRec->C1.end(); fIter++)
        ToRec->C1.push_back(*fIter);
#endif 
}

static float mincost = 1.0e6;
static CUTSTOCK_DATA bestcasedata;
static DWORD dwStartTime = 0;
static DWORD dwLoopCnt = 0;
static bool bTimeout = false;

bool FindIntegerSolution(CUTSTOCK_DATA* pData)
{
    CUTSTOCK_DATA NextCase;
    bool result;
    vector <int>::iterator iIter;
    InputMap::iterator mIter, mIter1;
    vector <vector<PATTERN_PART>>::iterator vvIter;
    float fCost;

    // mehr als 2 dauert zu lange
//    if (pData->IStockUsedByPattern.size() > 22)
//        return false;

    result = true;
    /* check if nbr of parts cut is >= nbr of parts ordered for each size */

#ifndef DEBUG
//    xxx
    dwLoopCnt++;
    if (dwLoopCnt%1000)
    {
        if ((timeGetTime() - dwStartTime) > 9000)
        {
            // maximal 10 sek
            bTimeout = true;
            return false;
        }

    }
#endif // DEBUG

    /* first total each part size across pattern */
    pData->totparts.clear();
    for (mIter = pData->PartQty.begin(); mIter != pData->PartQty.end(); mIter++)
    {
        pData->totparts[mIter->first] = 0;
    }

    for (vvIter = pData->A.begin(), iIter = pData->IStockUsedByPattern.begin(); 
        vvIter != pData->A.end(); vvIter++, iIter++)
    {
        /* total the parts cut */
        vector<PATTERN_PART>::iterator ppIter;
        for (ppIter = vvIter->begin(); ppIter != vvIter->end(); ppIter++)
        {
            mIter = pData->totparts.find(ppIter->iLength);
            if (mIter != pData->totparts.end())
            {
                mIter->second += (*iIter)*ppIter->iCount;
            }else
            {
                ASSERT(false);
            }
        }
    }

    /* Check if order requirements are  met, we're done */
    ASSERT(pData->PartQty.size() == pData->totparts.size());
    for (mIter = pData->PartQty.begin(), mIter1 = pData->totparts.begin();
         mIter != pData->PartQty.end(); mIter++, mIter1++)
    {
        if (mIter1->second < mIter->second)
            return false;
    }

    /* Otherwise, see if cost was lowered, if so save it */
    if ((fCost = totCost(pData))<mincost)
    {
        /* save a new best solution */
        copydata(pData,&bestcasedata);
        mincost = fCost;
    }

    /* current set is OK, make a new set using this solution */
    copydata(pData,&NextCase);
    int iLoop = 0;
    for (iIter = NextCase.IStockUsedByPattern.begin(); iIter != NextCase.IStockUsedByPattern.end(); iIter++)
    {
        iLoop++;
        if (*iIter > 0)
        {
            (*iIter)--;           /* Try taking one stock piece away from each pattern */
            if (FindIntegerSolution(&NextCase) == false)  /* and check to see if it is a solution */
            {
                *iIter += 1;
                if (bTimeout == true)
                    return false;
            }
        }
    }

#if 0
    for i:=1 to NPart do
       begin
         {total the parts cut}
         for j:=1 to high(a[i]) do {for each piece cut in this pattern}
              totparts[j]:=totparts[j]+A[i,j]*IStockUsedByPattern[i];
       end;


       {Check if order requirements are  met, we're done}
       for i:=1 to NPart do
       if totparts[i]<PartQty[i] then
       begin
         result:=false;
         exit;
       end;

       {Otherwise, see if cost was lowered, if so save it}
       If totcost<mincost then
       begin
         {save a new best solution}
         copydata(casedata,bestcasedata);
       end;

       {current set is OK, make a new set using this solution}
       copydata(casedata,nextcase);
       with nextcase do
       for i:= 1 to NPart do
       If (iStockUsedByPattern[i]>0) then
       begin
         dec(IStockusedByPattern[i]);  {Try taking one stock piece away from each pattern}
         if not FindIntegerSolution(Nextcase) then {and check to see if it is a solution}
         begin {Put that piece back before trying the next reduction}
           inc(IStockusedByPattern[i]);
         end;
       end;
     end;
#endif // 0
    return true;
}


/*---------------------------
           OPTSOLC
 ---------------------------*/
void   optsolc(CUTSTOCK_INPUT* pInput, CUTSTOCK_OUTPUT* pOutput)
{
    int  i, j, c, p, iFirst, iCount, iLength, iOldCount, iNewCount;
    InputMap inMap;
    pair< MIIter, bool > pr;
    MIIter miter, mNewIter;


    TRACE("\n*** OPTIMAL CUTTING PATTERN ***\n");
    for(i=1; i<=NROW; i++){
        if (BBAR[i] > TOL1){
            TRACE("\n Pattern(%d) \n", i);
            TRACE("   Stock length  %5d    ", SLENGTH[i]);
            TRACE("Quantity = %.5f \n", BBAR[i]);
            for(j=1; j<=NROW; j++){
                if (A[j][i] != 0){
                    TRACE("   Order length %5d    ", OL[j]);
                    TRACE("Number of Cut = %5d \n", A[j][i]);
                }
            }
        }
    }

    TRACE("\n*** OPTIMAL CUTTING PATTERN END ***\n");

    bestcasedata.IStockUsedByPattern.clear();
    bestcasedata.StockusedByPattern.clear();
    bestcasedata.A.clear();
    for(i=1; i<=NROW; i++){
        if (BBAR[i] > TOL1){
            //         fprintf(fp, "\n Pattern(%d) \n", i);
            //         fprintf(fp, "   Stock length  %5d    ", SLENGTH[i]);
            //         fprintf(fp, "Quantity = %.5f \n", BBAR[i]);
            vector<PATTERN_PART> vp;
            vector<vector<PATTERN_PART>>::reverse_iterator  rIter;

            bestcasedata.StockusedByPattern.push_back(BBAR[i]);
            bestcasedata.IStockUsedByPattern.push_back((int)ceil(BBAR[i]));

            bestcasedata.A.push_back(vp);
            rIter = bestcasedata.A.rbegin();
            for(j=1; j<=NROW; j++){
                if (A[j][i] != 0){
                    //               fprintf(fp,  "   Order length %5d    ", OL[j]);
                    //               fprintf(fp, "Number of Cut = %5d \n", A[j][i]);
                    PATTERN_PART pp;
                    pp.iCount = A[j][i];
                    pp.iLength = OL[j];
                    rIter->push_back(pp);
                }
            }
        }
    }

    bestcasedata.C1.clear();
    ZB = 0.0;
    for(i=1; i<=NROW; i++) 
    {
        if (BBAR[i] > TOL1)
        {
            bestcasedata.C1.push_back(C1[i]);
            ZB += C1[i] * BBAR[i];
        }
    }
    //   fprintf(fp, "\n Minimum stock cost = %.5f\n", ZB);
    //   fprintf(fp, "\n Number of Iteration = %3d\n", IT);
    CUTSTOCK_DATA iSol;
    copydata(&bestcasedata, &iSol);  
    
    dwStartTime = timeGetTime();
    dwLoopCnt = 0;
    bTimeout = false;
    FindIntegerSolution(&iSol);

    vector <int>::iterator iIter;
    vector<PATTERN_PART> vp;
    vector<PATTERN_PART>::iterator vpIter;
    vector<vector<PATTERN_PART>>::iterator vvPIter;

    TRACE("\n*** OPTIMAL INTEGER CUTTING PATTERN ***\n");
    i = 1;
    for (iIter = bestcasedata.IStockUsedByPattern.begin(), vvPIter = bestcasedata.A.begin(); 
        iIter != bestcasedata.IStockUsedByPattern.end(); iIter++, vvPIter++)
    {
       TRACE("\n Pattern(%d) \n", i);
       TRACE("   Stock length  %5d    ", SLENGTH[i]);
       TRACE("Quantity = %ld \n", *iIter);
       vp = *vvPIter;
       iFirst = 1;
       for (p = 0; p < *iIter; p++)
       {
           ProfileCutList* pCutList = new ProfileCutList;
           iLength = pInput->iSrcLength;
           for (vpIter = vp.begin(); vpIter != vp.end(); vpIter++)
           {
                mNewIter = inMap.find(vpIter->iLength);
                if (mNewIter != inMap.end())
                    iOldCount = mNewIter->second;
                else
                    iOldCount = 0;
                iNewCount = vpIter->iCount;
                miter = pInput->pInpuMap->find(vpIter->iLength);
                if (miter != pInput->pInpuMap->end())
                {
                    if (miter->second < (iNewCount + iOldCount))
                    {
                        // zuviele Profile von der Länge, Anzahl abziehen
                        iNewCount -= (iOldCount + iNewCount) - miter->second;
                    }
                }
                else
                {
                    // darf nicht auftreten
                    ASSERT(false);
                }
                if (iNewCount > 0)
                {
                    pr = inMap.insert ( Int_Pair ( vpIter->iLength, iNewCount /* es werden immer zwei geschnitten */) );
                    if( pr.second == true ) 
                    {
                        // The element was inserted successfully
                    }
                    else 
                    {
                        // Key number already exists, add it
                        ( pr.first )->second += vpIter->iCount;  //?? es werden immer zwei geschnitten
                    }
                   for (c = 0; c < iNewCount; c++)
                   {
                       pCutList->push_back(vpIter->iLength);
                       iLength -= vpIter->iLength;
                   }
                   if (iFirst)
                   {
                       TRACE("   Order length %5d    ", vpIter->iLength);
                       TRACE("   Number of Cut = %5d \n", vpIter->iCount);
                   }
                }
           }
           iFirst = 0;
           pOutput->pCutList->push_back(pCutList);
       }
       i++;
    }
    TRACE("\n*** OPTIMAL INTEGER CUTTING PATTERN END ***\n");

    return;
}

#if 0
/************
ERROR_HANDLE
************/
void error_handle()
{
   /***************************************/
   /* Error Code                          */
   /***************************************/
   /* IERROR=0    No Error                */
   /* IERROR=1    Normal Exit             */
   /* IERROR=2    File Open Error         */
   /***************************************/

   if (IERROR==0) return;
   else {
      printf("\n%s\n",ERR_MSG);
      fprintf(fp,"\n%s\n",ERR_MSG);
//      fclose(fp); 
// exit(0);
   }
}
#endif // 0

/*---------------------------
            MAIN
 ---------------------------*/
void cutstock_main(CUTSTOCK_INPUT* pInput, CUTSTOCK_OUTPUT* pOutput)
{
    MIIter iter;
    vector <float>::iterator fIter;
    vector <int>::iterator iIter;
    int i;
    
#ifdef DEBUG_
    IPR = 2;
#endif // DEBUG_

    NSOURCE = 1;  // immer nur 1 Ausgangsprofil
    NPART   = pInput->pInpuMap->size();  // Anzahl verschiedene Module
    SL[1] = pInput->iSrcLength;
    SC[1] = pInput->iSrcCost;


//      fscanf(fin, "%d %d %d", &NSOURCE, &NPART, &IPR);
//      for(i=1; i<=NSOURCE; i++) fscanf(fin,  "%d",  &SL[i]);
//      for(i=1; i<=NSOURCE; i++) fscanf(fin,  "%d",  &SC[i]);
    i = 1;
    for (iter = pInput->pInpuMap->begin(); iter != pInput->pInpuMap->end(); iter++)
    {
        OL[i] = iter->first;    // Laenge
        OQ[i] = iter->second;   // Anzahl
        i++;
    }
//      for(i=1; i<=NPART; i++) fscanf(fin,  "%d",  &OL[i]);
//      for(i=1; i<=NPART; i++) fscanf(fin,  "%d",  &OQ[i]);

/*   
   do{
      title();
      if(SW=='I' || SW=='i')     {  intemode(); break; }
      else if(SW=='F' || SW=='f'){  filemode(); break; }
   } while(1);
   error_handle();
*/
    mincost = 1.0e6;

    if(IPR > 1)  pr_data();
    // init bestcasedata

    bestcasedata.NPart = NPART;
    bestcasedata.NSource = NSOURCE;                    /* Nbr of required parts and supply parts */
    bestcasedata.Verbose = false;
    bestcasedata.Fractional = true;
    bestcasedata.StockLengths.push_back(SL[1]);
    bestcasedata.StockCost.push_back(SC[1]);
    bestcasedata.StockUsed.clear();
    bestcasedata.Partlengths.clear();
    for (iter = pInput->pInpuMap->begin(); iter != pInput->pInpuMap->end(); iter++)
    {
        bestcasedata.PartQty[iter->first] = iter->second;
    }
    bestcasedata.totparts.clear();        /* for each length, sum of parts cut from all patterns */

   NROW =NPART;
   IT=1;
   inisolc();
   btran();
   entcol();
   while( CMIN <= -TOL1 ){
      ftran();
      chuzr();
      upbinv();
      upsol();
      IT++;
      changecol();
      btran();
      entcol();
   }
   optsolc(pInput, pOutput);
//   IERROR=1; error_handle();
//   fclose(fp);
}/*End of main() */

