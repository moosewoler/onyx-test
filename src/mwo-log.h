#ifndef _MWO_LOG_H_
#define _MWO_LOG_H_

#define MWO_LOG_DUMP_SIZE   64
#define MWO_LOG_NAME_LENGTH 32

#define MWO_LOG_TYPE_INT    1
#define MWO_LOG_TYPE_UINT   2 
#define MWO_LOG_TYPE_FLOAT  3 

#define MWO_LOG_VAR(v,vtype)    mwo_log_var((void*)(&(v)), vtype, #v)
#define MWO_LOG_INT(v)          MWO_LOG_VAR(v, MWO_LOG_TYPE_INT)
#define MWO_LOG_UINT(v)         MWO_LOG_VAR(v, MWO_LOG_TYPE_UINT)
#define MWO_LOG_FLOAT(v)        MWO_LOG_VAR(v, MWO_LOG_TYPE_FLOAT)

#define MWO_LOG(v)              mwo_log(&v)

#define MWO_LOG_ONCE(e, etype)  mwo_log_once((int)(e), etype, #e)
#define MWO_LOG_INT_ONCE(e)     MWO_LOG_ONCE(e, MWO_LOG_TYPE_INT)
#define MWO_LOG_UINT_ONCE(e)    MWO_LOG_ONCE(e, MWO_LOG_TYPE_UINT)
//#define MWO_LOG_FLOAT_ONCE(e)   MWO_LOG_ONCE(e, MWO_LOG_TYPE_FLOAT)


#ifdef MWO_LOG_IMPLEMENTATION
void mwo_log_var(void* vp, int vtype, char* vname);
void mwo_log(void* p);
void mwo_log_once(int e, int etype, char* ename);
#else
extern void mwo_log_var(void* vp, int vtype, char* vname);
extern void mwo_log(void* p);
extern void mwo_log_once(int e, int etype, char* ename); // used for expression
#endif

#endif
