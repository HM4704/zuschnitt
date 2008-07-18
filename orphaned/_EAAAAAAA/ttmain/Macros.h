#ifndef MACROS_H_30092001
#define MACROS_H_30092001

// siches Löschen
#define SAFE_DELETE_ARR(PDATA)  if (PDATA != NULL)      \
                                {                       \
                                    delete [] PDATA;       \
                                    PDATA = NULL;       \
                                }  
                     
#define SAFE_DELETE(PDATA)      if (PDATA != NULL)      \
                                {                       \
                                    delete PDATA;       \
                                    PDATA = NULL;       \
                                }  

#endif