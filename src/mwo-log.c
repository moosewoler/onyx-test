#include <stdio.h>
#include <string.h>

#define MWO_LOG_IMPLEMENTATION
#include "mwo-log.h"

/*******************************************************************************
 *          Log Dump
 ******************************************************************************/
struct _TLogItem
{
    void* vp;
    int   vtype;
    char  vname[MWO_LOG_NAME_LENGTH];
};
typedef struct _TLogItem TLogItem;
TLogItem logdump[MWO_LOG_DUMP_SIZE];
int k=0;

/*******************************************************************************
 ******************************************************************************/
void mwo_log_var(void* vp, int vtype, char* vname)
{
    if (k<MWO_LOG_DUMP_SIZE)
    {
        logdump[k].vp   = vp;
        logdump[k].vtype= vtype;
        strcpy(logdump[k].vname, vname);
        k++;
    }
}

void mwo_log_detail(int index)
{
    switch( logdump[index].vtype)
    {
        case MWO_LOG_TYPE_INT:
            printf("%s = %d\n", logdump[index].vname, *((int*)logdump[index].vp));
            break;
        case MWO_LOG_TYPE_UINT:
            printf("%s = %u\n", logdump[index].vname, *((unsigned int*)logdump[index].vp));
            break;
        case MWO_LOG_TYPE_FLOAT:
            printf("%s = %f\n", logdump[index].vname, *((float*)logdump[index].vp));
            break;
    }
}

void mwo_log(void* p)
{
    int i;
    for (i=0;i<MWO_LOG_DUMP_SIZE;i++)
    {
        if (logdump[i].vp == p)
        {
            mwo_log_detail(i);
        }
    }
}

void mwo_log_once(int e, int etype, char* ename)
{
    switch(etype)
    {
        case MWO_LOG_TYPE_INT:
            printf("%s = %d\n", ename, (int)e);
            break;
        case MWO_LOG_TYPE_UINT:
            printf("%s = %u\n", ename, (unsigned int)e);
            break;
        case MWO_LOG_TYPE_FLOAT:
            printf("%s = %f\n", ename, (float)e);
            break;
    }
}
