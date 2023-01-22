#ifndef DEQUE_H
#define DEQUE_H

#include <stddef.h>
#include <cstdlib>

#define Deque_DEFINE(t)\
struct Deque_##t##_Iterator;\
struct Deque_##t{\
	t * data;\
	int phys_size = 2;\
	size_t count = 0;\
	int front_ind = 1;\
	int back_ind = 0;\
	const char type_name[6 + sizeof(#t)] = "Deque_" #t;	\
	bool (*cmp)(t const&, t const&);\
	size_t (*size)(Deque_##t *); \
    bool (*empty)(Deque_##t *); \
    void (*push_back)(Deque_##t *, t); \
    void (*push_front)(Deque_##t *, t);\
    t &(*back)(Deque_##t *);\
    t &(*front)(Deque_##t *); \
    t &(*at)(Deque_##t *, int i);\
    void (*pop_back)(Deque_##t *); \
    void (*pop_front)(Deque_##t *);\
	Deque_##t##_Iterator (*end)(Deque_##t *); \
	Deque_##t##_Iterator (*begin)(Deque_##t *); \
	void (*sort)(Deque_##t *, Deque_##t##_Iterator, Deque_##t##_Iterator);\
	void (*clear)(Deque_##t *);	\
    void (*dtor)(Deque_##t *); \
}; \
size_t Deque_size(Deque_##t *deq){\
	return deq->count;\
}\
\
bool Deque_empty(Deque_##t *deq){\
	return deq->count == 0;\
}\
void Deque_push_back(Deque_##t *deq, t obj){\
	deq->back_ind = (deq->back_ind + 1) % deq->phys_size;\
	if (deq->phys_size - deq->count == 1){\
		t *bigger = (t *)malloc(sizeof(t)*deq->phys_size * 2);\
		for (int i = 0; i < (int)deq->count; i++){\
			bigger[i] = deq->data[(deq->front_ind + i) % deq->phys_size];\
		}\
		deq->phys_size *= 2;\
		free(deq->data);\
		deq->data = bigger;\
		deq->front_ind = 0;\
		deq->back_ind = deq->count;\
	}\
	deq->data[deq->back_ind] = obj;\
	deq->count++;\
}\
void Deque_push_front(Deque_##t *deq, t obj){\
	deq->front_ind = (deq->front_ind + deq->phys_size - 1) % deq->phys_size;\
	if (deq->phys_size - deq->count == 1){\
		t* bigger = (t *)malloc(sizeof(t)*deq->phys_size * 2);\
		for (int i = 0; i < (int)deq->count; i++){\
			bigger[i+1] = deq->data[(deq->front_ind + 1 + i) % deq->phys_size];\
		} \
		deq->phys_size *= 2;\
		free(deq->data);\
		deq->data = bigger;\
		deq->front_ind = 0;\
		deq->back_ind = deq->count;\
	}\
	deq->data[deq->front_ind] = obj;\
	deq->count++;\
}\
t &Deque_back(Deque_##t *deq){\
	return deq->data[deq->back_ind];\
}\
t &Deque_front(Deque_##t *deq){\
	return deq->data[deq->front_ind];\
}\
t &Deque_at(Deque_##t *deq, int i){\
	return deq->data[(deq->front_ind + i) % deq->phys_size];\
}\
void Deque_pop_back(Deque_##t* deq){\
	deq->back_ind = (deq->back_ind + deq->phys_size - 1) % deq->phys_size;\
	deq->count--;\
}\
void Deque_pop_front(Deque_##t* deq){\
	deq->front_ind = (deq->front_ind + 1) % deq->phys_size;\
	deq->count--;\
}\
void Deque_clear(Deque_##t *deq){\
	deq->front_ind = 1;\
	deq->back_ind = 0;\
	deq->count = 0;\
}\
void Deque_dtor(Deque_##t *deq){\
	free(deq->data);\
}\
Deque_##t##_Iterator Deque_end(Deque_##t* deq);\
Deque_##t##_Iterator Deque_begin(Deque_##t* deq);\
void Deque_sort(Deque_##t *deq, Deque_##t##_Iterator start, Deque_##t##_Iterator end);\
void Deque_##t##_ctor(Deque_##t *deq, bool (*compare)(t const&, t const&)){\
	deq->data = (t *) malloc(deq->phys_size * sizeof(t));\
	deq->cmp = compare;\
	deq->size = Deque_size;\
	deq->empty = Deque_empty;\
	deq->push_back = Deque_push_back;\
	deq->push_front = Deque_push_front;\
	deq->back = Deque_back;\
	deq->front = Deque_front;\
	deq->at = Deque_at;\
	deq->pop_back = Deque_pop_back;\
	deq->pop_front = Deque_pop_front;\
	deq->end = Deque_end;\
	deq->begin = Deque_begin;\
	deq->sort = Deque_sort;\
	deq->clear = Deque_clear;\
	deq->dtor = Deque_dtor;\
} \
bool Deque_##t##_equal(Deque_##t deq1, Deque_##t deq2){\
	if (deq1.size(&deq1) != deq2.size(&deq2))\
		return false;\
	for (long unsigned int i = 0; i < deq1.size(&deq1); i++){\
		bool cmp1 = deq1.cmp(deq1.at(&deq1, i), deq2.at(&deq2, i));\
		bool cmp2 = deq1.cmp(deq2.at(&deq2, i), deq2.at(&deq1, i));\
		if (cmp1 || cmp2)\
			return false;\
	}\
	return true;\
}\
struct Deque_##t##_Iterator{\
	int ind; \
	Deque_##t * deque;\
	void (*inc)(Deque_##t##_Iterator *); \
	void (*dec)(Deque_##t##_Iterator *); \
	t &(*deref)(Deque_##t##_Iterator *); \
}; \
void Deque_##t##_Iterator_inc(Deque_##t##_Iterator *it){\
	it->ind = (it->ind + 1) % it->deque->phys_size;\
}\
void Deque_##t##_Iterator_dec(Deque_##t##_Iterator *it){\
	it->ind = (it->ind + it->deque->phys_size - 1) % it->deque->phys_size;\
}\
t &Deque_##t##_Iterator_deref(Deque_##t##_Iterator *it){\
	return it->deque->at(it->deque, it->ind);\
}\
bool Deque_##t##_Iterator_equal(Deque_##t##_Iterator it1, Deque_##t##_Iterator it2){\
	return it1.ind == it2.ind;\
}\
Deque_##t##_Iterator Deque_begin(Deque_##t *deq){\
	Deque_##t##_Iterator iter;\
	iter.ind = deq->front_ind;\
	iter.deque = deq;\
	iter.inc = Deque_##t##_Iterator_inc;\
	iter.dec = Deque_##t##_Iterator_dec;\
	iter.deref = Deque_##t##_Iterator_deref;\
	return iter;\
}\
Deque_##t##_Iterator Deque_end(Deque_##t *deq){\
	Deque_##t##_Iterator iter;\
	iter.ind = (deq->back_ind + 1) % deq->phys_size;\
	iter.deque = deq;\
	iter.inc = Deque_##t##_Iterator_inc;\
	iter.dec = Deque_##t##_Iterator_dec;\
	iter.deref = Deque_##t##_Iterator_deref;\
	return iter;\
}\
int Deque_##t##_compare(void *vp, const void *o1, const void *o2){ \
	Deque_##t *deq = (Deque_##t *)vp;\
	if (deq->cmp(*((t *)o1), *((t *)o2)))\
		return -1;\
	else if (deq->cmp(*((t *)o2), *((t *)o1)))\
		return 1;\
	return 0;\
}\
void Deque_sort(Deque_##t *deq, Deque_##t##_Iterator start, Deque_##t##_Iterator end){\
	void* base = (void *)&deq->data[start.ind];\
	size_t nmemb = end.ind - start.ind;\
	size_t size = sizeof(t);\
	void* arg = (void*)deq;\
\
	qsort_r(base, nmemb, size, arg, Deque_##t##_compare);	\
}
#endif
