#include "main.h"

bool vectorCrear(Vector* v,size_t tamElem)
{
    v->ce =0;
    v->vec = malloc(CAP_INI * tamElem);
    if(v->vec == NULL)
    {
        v->cap = 0;
        v->tamElem=0;
        return false;
    }

    v->cap = CAP_INI;
    v->tamElem = tamElem;
    return true;
}

void vectorDestruir(Vector* v)
{
    free(v->vec);
    v->vec = NULL;
    v->ce = 0;
    v->cap =0;
    v->tamElem =0;
}

bool redimensionarVector(Vector* v,int operacion)
{
    size_t nuevaCap= operacion == AUMENTAR? v->cap * FACTOR_INCR: max(v->cap * FACTOR_DECR, CAP_INI);
    int* nVec = realloc(v->vec, nuevaCap * v->tamElem);
    if(!nVec)
    {
        return false;
    }
    printf("Redimension de %lu a %lu \n",v->cap, nuevaCap);
    v->vec = nVec;
    v->cap = nuevaCap;
    return true;
}
size_t max(size_t a, size_t b)
{
    return a >= b? a:b;
}
int vectorOrdInsertar(Vector* v,const void* elem,Cmp cmp,Cmp2 cmp2)
{
    if(v->ce == v->cap)
    {
        if(!redimensionarVector(v,AUMENTAR))
        {
            return SIN_MEM;
        }
    }

    void* i= v->vec;
    void* ult=v->vec +(v->ce-1)*v->tamElem;

    while(i <= ult && cmp(elem,i )> 0)
    {
        if()
        i+=v->tamElem;
    }
    if(i <=ult && cmp(elem,i) == 0)//duplicado
    {
        cmp2(elem,i)
    }

    void* posIns = i;
    for(i=ult; i>=posIns; i-= v->tamElem)//Resolver con memmove
    {
        memcpy(i + v->tamElem,i,v->tamElem);
    }

    memcpy(posIns, elem,v->tamElem);
    v->ce++;
    return TODO_OK;
}
int compararPorPeriodoYClasificador(void* a,void* b)
{
    RegistroICC* reg1 = a;
    RegistroICC* reg2 =b;
    reg1->clasificador

}
int cmpFechas(const void* a, const void* b)
{
    const RegistroICC* f1 = a;
    const RegistroICC* f2 = b;
    return (f1->periodo.anio * 10000 + f1->periodo.mes * 100 + f1->periodo.dia) - (f2->periodo.anio* 10000 + f2->periodo.mes * 100 + f2->periodo.dia);
}
