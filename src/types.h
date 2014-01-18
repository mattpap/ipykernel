#ifndef __IPYKERNEL_TYPES_H__
#define __IPYKERNEL_TYPES_H__

#define List(Type) struct { Type* list; size_t size; }

typedef char* String;
typedef List(String) StringList;

#endif // __IPYKERNEL_TYPES_H__
