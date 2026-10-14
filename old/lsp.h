/*
lsp.h
*/
#ifndef __LSP_H__
#define __LSP_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

typedef struct NODE
{
   unsigned char ntype;
   union
   {
      struct
      {
         struct NODE *_car,*_cdr;
      } n_cons;
      double fvalue;
      long ivalue;
      char *svalue;

   }
   n_value;
} *LVAL;

#define NTYPE_CON 1
#define NTYPE_FLO 2
#define NTYPE_INT 3
#define NTYPE_STR 4
#define NTYPE_SYM 5
#define NTYPE_CHR 6
#define NTYPE_FRE 7


#define null(x)       ((x) == NIL)
#define freep(x)      ((x)->ntype == NTYPE_FRE)
#define numberp(x)    (floatp(x)||integerp(x))
#define endp(x) null(x)
#define eq(x,y)   ((x)==(y))

#define evenp(x)  (!oddp(x))
#define oddp(x)   (getint(x)&1)
#define minusp(x) (floatp(x) ? getfloat(x) < 0.0 : getint(x) < 0 )
#define plusp(x)  (floatp(x) ? getfloat(x) > 0.0 : getint(x) > 0 )
#define zerop(x)  (floatp(x) ? getfloat(x) == 0.0 : getint(x) == 0 )

#define first(x)  car(x)
#define second(x) cadr(x)
#define third(x)  caddr(x)
#define fourth(x) cadddr(x)

#define listp(x)  (consp(x)||null(x))
#define gettype(x)     ((x)->ntype)
#define getstring(x)   ((x)->n_value.svalue)
#define getint(x)      ((x)->n_value.ivalue)
#define getfloat(x)    ((x)->n_value.fvalue)
#define getchr(x)      getint(x)
#define getsymbol(x)   getstring(x)

#define caar(x) car(car(x))
#define cadr(x) car(cdr(x))
#define cdar(x) cdr(car(x))
#define cddr(x) cdr(cdr(x))

#define caaar(x) car(car(car(x)))
#define caadr(x) car(car(cdr(x)))
#define cadar(x) car(cdr(car(x)))
#define caddr(x) car(cdr(cdr(x)))
#define cdaar(x) cdr(car(car(x)))
#define cdadr(x) cdr(car(cdr(x)))
#define cddar(x) cdr(cdr(car(x)))
#define cdddr(x) cdr(cdr(cdr(x)))

#define caaaar(x) car(car(car(car(x))))
#define caaadr(x) car(car(car(cdr(x))))
#define caadar(x) car(car(cdr(car(x))))
#define caaddr(x) car(car(cdr(cdr(x))))
#define cadaar(x) car(cdr(car(car(x))))
#define cadadr(x) car(cdr(car(cdr(x))))
#define caddar(x) car(cdr(cdr(car(x))))
#define cadddr(x) car(cdr(cdr(cdr(x))))
#define cdaaar(x) cdr(car(car(car(x))))
#define cdaadr(x) cdr(car(car(cdr(x))))
#define cdadar(x) cdr(car(cdr(car(x))))
#define cdaddr(x) cdr(car(cdr(cdr(x))))
#define cddaar(x) cdr(cdr(car(car(x))))
#define cddadr(x) cdr(cdr(car(cdr(x))))
#define cdddar(x) cdr(cdr(cdr(car(x))))
#define cddddr(x) cdr(cdr(cdr(cdr(x))))

#define settype(x,v)    ((x)->ntype=(v))
#define setcar(x,v)     ((x)->n_value.n_cons._car=(v))
#define setcdr(x,v)     ((x)->n_value.n_cons._cdr=(v))
#define setint(x,v)     ((x)->n_value.ivalue=(v))
#define setfloat(x,v)   ((x)->n_value.fvalue=(v))
#define setstring(x,v)  ((x)->n_value.svalue=(v))
#define setchar(x,v)    setint(x,v)
#define setsymbol(x,v)  setstring(x,v)
#define sassoc(x,y) nthsassoc((x),(y),1)


#define dolist(X,Y,z) for( X= (z=Y)      ? car(z) : NIL ; z ; \
                           X= (z=cdr(z)) ? car(z) : NIL )

#define dotimes(i,x) for(i = 0 ; i < x ; i++ )
#define loop         for(;;)

#define newstring()  newnode(NTYPE_STR)
#define newsymbol()  newnode(NTYPE_SYM)
#define newint()     newnode(NTYPE_INT)
#define newfloat()   newnode(NTYPE_FLO)
#define newchar()    newnode(NTYPE_CHR)
#define NIL (LVAL)0

#define SCR_WIDTH 70
#define BUFFERLENGTH 1024
#define BUFFERINC    1024
#define ERRSTRLEN 5
#define UNGET_BUFFER_LENGTH 10

typedef struct _tLspObject {
  void *(*memory_allocating_function)(size_t, void *);
  void (*memory_releasing_function)(void *, void *);
  void *pMemorySegment;
  FILE *f;
  char cOpen,cClose; 
  int tabpos,scrsize;
  char *buffer;
  long cbBuffer;
  int SymbolLength;
  int CaseFlag;
  int UngetBuffer[UNGET_BUFFER_LENGTH];
  int UngetCounter;
  } tLspObject,*tpLspObject;

/*FUNDEF*/

LVAL lsp_init(tpLspObject pLSP,
              int SymLen,
              int CaseFlg,
              void *(*memory_allocating_function)(size_t, void *),
              void (*memory_releasing_function)(void *, void *),
              void *pMemorySegment);
/*FEDNUF*/
/*FUNDEF*/

LVAL c_cons(tpLspObject pLSP);
/*FEDNUF*/
#define cons() c_cons(pLSP)
#define newnode(x) c_newnode(pLSP,(x))
/*FUNDEF*/

LVAL c_newnode(tpLspObject pLSP,
               unsigned char type);
/*FEDNUF*/
#define symcmp(x,y) c_symcmp(pLSP,(x),(y))
/*FUNDEF*/

LVAL c_symcmp(tpLspObject pLSP,
              LVAL p,
              char *s);
/*FEDNUF*/
#define nthsassoc(x,y,z) c_nthsassoc(pLSP,(x),(y),(z))
/*FUNDEF*/

LVAL c_nthsassoc(tpLspObject pLSP,
                 LVAL p,
                 char *s,
                 int n);
/*FEDNUF*/
#define freelist(x) c_freelist(pLSP,(x))
/*FUNDEF*/

LVAL c_freelist(tpLspObject pLSP,
              LVAL p);
/*FEDNUF*/
#define flatc(x) c_flatc(pLSP,(x))
/*FUNDEF*/

int c_flatc(tpLspObject pLSP,
            LVAL p);
/*FEDNUF*/
#define pprint(x,y) c_pprint(pLSP,(x),(y))
/*FUNDEF*/

LVAL c_pprint(tpLspObject pLSP,
            LVAL p,
            FILE *file);
/*FEDNUF*/
#define readlist(x) c_readlist(pLSP,(x))
/*FUNDEF*/

LVAL c_readlist(tpLspObject pLSP,
                FILE *f);
/*FEDNUF*/
#define readexpr(x) c_readexpr(pLSP,(x))
/*FUNDEF*/

LVAL c_readexpr(tpLspObject pLSP,
                FILE *f);
/*FEDNUF*/
#define skipexpr(x) c_skipexpr(pLSP,(x))
/*FUNDEF*/

LVAL c_skipexpr(tpLspObject pLSP,
                FILE *f);
/*FEDNUF*/
#define llength(x) c_llength(pLSP,(x))
/*FUNDEF*/

int c_llength(tpLspObject pLSP,
              LVAL p);
/*FEDNUF*/
#define nth(x,y) c_nth(pLSP,(x),(y))
/*FUNDEF*/

LVAL c_nth(tpLspObject pLSP,
         int n,
         LVAL p);
/*FEDNUF*/
#define nthcdr(x,y) c_nthcdr(pLSP,(x),(y))
/*FUNDEF*/

LVAL c_nthcdr(tpLspObject pLSP,
              int n,
              LVAL p);
/*FEDNUF*/
#define char_code(x) c_char_code(pLSP,(x))
/*FUNDEF*/

LVAL c_char_code(tpLspObject pLSP,
                 LVAL p);
/*FEDNUF*/
#define code_char(x) c_code_char(pLSP,(x))
/*FUNDEF*/

LVAL c_code_char(tpLspObject pLSP,
                 LVAL p);
/*FEDNUF*/
#define char_downcase(x) c_char_downcase(pLSP,(x))
/*FUNDEF*/

LVAL c_char_downcase(tpLspObject pLSP,
                     LVAL p);
/*FEDNUF*/
#define char_upcase(x) c_char_upcase(pLSP,(x))
/*FUNDEF*/

LVAL c_char_upcase(tpLspObject pLSP,
                   LVAL p);
/*FEDNUF*/
#define equal(x,y) c_equal(pLSP,(x),(y))
/*FUNDEF*/

int c_equal(tpLspObject pLSP,
            LVAL p,
            LVAL q);
/*FEDNUF*/
#define car(x) c_car(pLSP,(x))
/*FUNDEF*/

LVAL c_car(tpLspObject pLSP,
         LVAL x);
/*FEDNUF*/
#define cdr(x) c_cdr(pLSP,(x))
/*FUNDEF*/

LVAL c_cdr(tpLspObject pLSP,
         LVAL x);
/*FEDNUF*/
#define consp(x) c_consp(pLSP,(x))
/*FUNDEF*/

int c_consp(tpLspObject pLSP,
            LVAL x);
/*FEDNUF*/
#define floatp(x) c_floatp(pLSP,(x))
/*FUNDEF*/

int c_floatp(tpLspObject pLSP,
         LVAL x);
/*FEDNUF*/
#define integerp(x) c_integerp(pLSP,(x))
/*FUNDEF*/

int c_integerp(tpLspObject pLSP,
         LVAL x);
/*FEDNUF*/
#define stringp(x) c_stringp(pLSP,(x))
/*FUNDEF*/

int c_stringp(tpLspObject pLSP,
         LVAL x);
/*FEDNUF*/
#define symbolp(x) c_symbolp(pLSP,(x))
/*FUNDEF*/

int c_symbolp(tpLspObject pLSP,
         LVAL x);
/*FEDNUF*/
#define characterp(x) c_characterp(pLSP,(x))
/*FUNDEF*/

int c_characterp(tpLspObject pLSP,
         LVAL x);
/*FEDNUF*/
#define atom(x) c_atom(pLSP,(x))
/*FUNDEF*/

int c_atom(tpLspObject pLSP,
         LVAL x);
/*FEDNUF*/
#ifdef __cplusplus
}
#endif
#endif
