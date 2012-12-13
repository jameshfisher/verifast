#ifndef PRELUDE_H
#define PRELUDE_H

//@ #include "prelude_core.gh"
//@ #include "list.gh"

/*@

lemma void assume(bool b);
    requires true;
    ensures b;


predicate exists<t>(t x;) = true;


fixpoint int abs(int x) { return x < 0 ? -x : x; }

lemma void div_rem(int D, int d);
    requires d != 0;
    ensures D == D / d * d + D % d &*& abs(D % d) < abs(d) &*& abs(D / d * d) <= abs(D);

predicate character(char *p; char c);
predicate u_character(unsigned char *p; unsigned char c);

predicate integer(int *p; int v);
predicate u_integer(unsigned int *p; unsigned int v);

predicate pointer(void **pp; void *p);


lemma void character_limits(char *pc);
    requires [?f]character(pc, ?c);
    ensures [f]character(pc, c) &*& pc > (char *)0 &*& pc < (char *)UINTPTR_MAX &*& -128 <= c &*& c <= 127;

lemma void integer_distinct(int* i, int* j);
    requires integer(i, ?v1) &*& integer(j, ?v2);
    ensures integer(i, v1) &*& integer(j, v2) &*& i != j;

lemma void integer_unique(int *p);
    requires [?f]integer(p, ?v);
    ensures [f]integer(p, v) &*& f <= 1;

lemma void integer_limits(int *p);
    requires [?f]integer(p, ?v);
    ensures [f]integer(p, v) &*& p > (int *)0 &*& p + 1 <= (int *)UINTPTR_MAX &*& INT_MIN <= v &*& v <= INT_MAX;


lemma void pointer_distinct(void *pp1, void *pp2);
    requires pointer(pp1, ?p1) &*& pointer(pp2, ?p2);
    ensures pointer(pp1, p1) &*& pointer(pp2, p2) &*& pp1 != pp2;

lemma void pointer_unique(void *pp);
    requires [?f]pointer(pp, ?p);
    ensures [f]pointer(pp, p) &*& f <= 1;

lemma_auto void pointer_nonzero();
    requires pointer(?pp, ?p);
    ensures pointer(pp, p) &*& pp != 0;

lemma void pointer_limits(void *pp);
    requires [?f]pointer(pp, ?p);
    ensures [f]pointer(pp, p) &*& pp > (void *)0 &*& pp + sizeof(void *) <= (void *)UINTPTR_MAX &*& p >= (void *)0 &*& p <= (void *)UINTPTR_MAX;

fixpoint void *pointer_of_chars(list<char> cs);
fixpoint list<char> chars_of_pointer(void * p);
fixpoint bool chars_within_limits(list<char> cs);

lemma_auto(pointer_of_chars(chars_of_pointer(p))) void pointer_of_chars_of_pointer(void *p);
    requires p >= (void *)0 && p <= (void *)UINTPTR_MAX;
    ensures pointer_of_chars(chars_of_pointer(p)) == p && chars_within_limits(chars_of_pointer(p)) == true && length(chars_of_pointer(p)) == sizeof(void *);

lemma_auto(chars_of_pointer(pointer_of_chars(cs))) void chars_of_pointer_of_chars(list<char> cs);
    requires length(cs) == sizeof(void *) && chars_within_limits(cs) == true;
    ensures chars_of_pointer(pointer_of_chars(cs)) == cs;


predicate chars(char *array, int count; list<char> cs) =
    count == 0 ?
        cs == nil
    :
        character(array, ?c) &*& chars(array + 1, count - 1, ?cs0) &*& cs == cons(c, cs0);

lemma_auto void chars_inv();
    requires [?f]chars(?array, ?count, ?cs);
    ensures [f]chars(array, count, cs) &*& length(cs) == count;

lemma void chars_zero();
    requires [?f]chars(0, _, ?cs);
    ensures cs == nil;

lemma void chars_limits(char *array);
    requires [?f]chars(array, ?n, ?cs) &*& true == ((char *)0 <= array) &*& array <= (char *)UINTPTR_MAX;
    ensures [f]chars(array, n, cs) &*& true == ((char *)0 <= array) &*& array + n <= (char *)UINTPTR_MAX;

lemma void chars_split(char *array, int offset);
   requires [?f]chars(array, ?n, ?cs) &*& 0 <= offset &*& offset <= n;
   ensures
       [f]chars(array, offset, take(offset, cs))
       &*& [f]chars(array + offset, n - offset, drop(offset, cs))
       &*& append(take(offset, cs), drop(offset, cs)) == cs;

lemma void chars_join(char *array);
    requires [?f]chars(array, ?n, ?cs) &*& [f]chars(array + n, ?n0, ?cs0);
    ensures [f]chars(array, n + n0, append(cs, cs0));


// chars to ...
lemma_auto void chars_to_integer(void *p);
    requires [?f]chars(p, sizeof(int), ?cs);
    ensures [f]integer(p, _);

lemma_auto void chars_to_u_integer(void *p);
    requires [?f]chars(p, sizeof(unsigned int), ?cs);
    ensures [f]u_integer(p, _);

lemma_auto void chars_to_pointer(void *p);
    requires [?f]chars(p, sizeof(void *), ?cs);
    ensures [f]pointer(p, pointer_of_chars(cs));

// ... to chars
lemma_auto void integer_to_chars(void *p);
    requires [?f]integer(p, _);
    ensures [f]chars(p, sizeof(int), ?cs);

lemma_auto void u_integer_to_chars(void *p);
    requires [?f]u_integer(p, _);
    ensures [f]chars(p, sizeof(unsigned int), ?cs);

lemma_auto void pointer_to_chars(void *p);
    requires [?f]pointer(p, ?v);
    ensures [f]chars(p, sizeof(void *), chars_of_pointer(v));

// u_character to/from character
lemma_auto void u_character_to_character(void *p);
    requires [?f]u_character(p, _);
    ensures [f]character(p, _);

lemma_auto void character_to_u_character(void *p);
    requires [?f]character(p, _);
    ensures [f]u_character(p, _);


predicate uchars(unsigned char *p, int count; list<unsigned char> cs) =
    count == 0 ?
        cs == nil
    :
        u_character(p, ?c) &*& uchars(p + 1, count - 1, ?cs0) &*& cs == cons(c, cs0);

lemma_auto void uchars_inv();
    requires [?f]uchars(?p, ?count, ?cs);
    ensures [f]uchars(p, count, cs) &*& count == length(cs);

predicate ints(int *p, int count; list<int> vs) =
    count == 0 ?
        vs == nil
    :
        integer(p, ?v) &*& ints(p + 1, count - 1, ?vs0) &*& vs == cons(v, vs0);

lemma_auto void ints_inv();
    requires [?f]ints(?p, ?count, ?vs);
    ensures [f]ints(p, count, vs) &*& count == length(vs);

predicate uints(unsigned int *p, int count; list<unsigned int> vs) =
    count == 0 ?
        vs == nil
    :
        u_integer(p, ?v) &*& uints(p + 1, count - 1, ?vs0) &*& vs == cons(v, vs0);

lemma_auto void uints_inv();
    requires [?f]uints(?p, ?count, ?vs);
    ensures [f]uints(p, count, vs) &*& count == length(vs);

predicate pointers(void **pp, int count; list<void *> ps) =
    count == 0 ?
        ps == nil
    :
        pointer(pp, ?p) &*& pointers(pp + 1, count - 1, ?ps0) &*& ps == cons(p, ps0);

lemma_auto void pointers_inv();
    requires [?f]pointers(?pp, ?count, ?ps);
    ensures [f]pointers(pp, count, ps) &*& count == length(ps);

lemma void pointers_split(void **pp, int offset);
    requires [?f]pointers(pp, ?count, ?ps) &*& 0 <= offset &*& offset <= count;
    ensures [f]pointers(pp, offset, take(offset, ps)) &*& [f]pointers(pp + offset, count - offset, drop(offset, ps));

lemma void pointers_join(void **pp);
    requires [?f]pointers(pp, ?count1, ?ps1) &*& [f]pointers(pp + count1, ?count2, ?ps2);
    ensures [f]pointers(pp, count1 + count2, append(ps1, ps2));


lemma_auto void chars_to_uchars(void *p);
    requires [?f]chars(p, ?n, _);
    ensures [f]uchars(p, n, _);

lemma_auto void uchars_to_chars(void *p);
    requires [?f]uchars(p, ?n, _);
    ensures [f]chars(p, n, _);

lemma void chars_to_ints(void *p, int n);
    requires [?f]chars(p, n * sizeof(int), _);
    ensures [f]ints(p, n, _);

lemma_auto void ints_to_chars(void *p);
    requires [?f]ints(p, ?n, _);
    ensures [f]chars(p, n * sizeof(int), _);

lemma void chars_to_uints(void *p, int n);
    requires [?f]chars(p, n * sizeof(unsigned int), _);
    ensures [f]uints(p, n, _);

lemma_auto void uints_to_chars(void *p);
    requires [?f]uints(p, ?n, _);
    ensures [f]chars(p, n * sizeof(unsigned int), _);

fixpoint list<void *> pointers_of_chars(list<char> cs);
fixpoint list<char> chars_of_pointers(list<void *> ps);

lemma void chars_to_pointers(void *p, int n);
    requires [?f]chars(p, n * sizeof(void *), ?cs);
    ensures [f]pointers(p, n, pointers_of_chars(cs)) &*& chars_of_pointers(pointers_of_chars(cs)) == cs;

lemma_auto void pointers_to_chars(void *pp);
    requires [?f]pointers(pp, ?n, ?ps) &*& true;
    ensures [f]chars(pp, n * sizeof(void *), chars_of_pointers(ps)) &*& pointers_of_chars(chars_of_pointers(ps)) == ps;


predicate divrem(int D, int d; int q, int r); // Rounds towards negative infinity, unlike C integer division and remainder.

lemma void divrem_intro(int D, int d, int q, int r);
    requires 0 <= r &*& r < d &*& D == q * d + r;
    ensures divrem(D, d, q, r);

lemma_auto void divrem_elim();
    requires divrem(?D, ?d, ?q, ?r);
    ensures divrem(D, d, q, r) &*& 0 <= r &*& r <= d &*& D == q * d + r;

predicate malloc_block(void *p; int size);
predicate malloc_block_chars(char *p; int count) = malloc_block(p, count);
predicate malloc_block_uchars(unsigned char *p; int count) = malloc_block(p, count);
predicate malloc_block_ints(int *p; int count) = malloc_block(p, ?size) &*& divrem(size, sizeof(int), count, 0);
predicate malloc_block_uints(unsigned int *p; int count) = malloc_block(p, ?size) &*& divrem(size, sizeof(unsigned int), count, 0);
predicate malloc_block_pointers(void **p; int count) = malloc_block(p, ?size) &*& divrem(size, sizeof(void *), count, 0);

@*/

/*@

predicate string(char *s; list<char> cs) =
    character(s, ?c) &*&
    c == 0 ?
        cs == nil
    :
        string(s + 1, ?cs0) &*& cs == cons(c, cs0);

lemma_auto void string_to_body_chars(char *s);
    requires [?f]string(s, ?cs);
    ensures [f]chars(s, length(cs), cs) &*& [f]character(s + length(cs), 0) &*& !mem('\0', cs);

lemma void body_chars_to_string(char *s);
    requires [?f]chars(s, _, ?cs) &*& [f]character(s + length(cs), 0) &*& !mem('\0', cs);
    ensures [f]string(s, cs);

lemma_auto void chars_to_string(char *s);
    requires [?f]chars(s, ?n, ?cs) &*& index_of('\0', cs) == n - 1;
    ensures [f]string(s, take(n - 1, cs));

lemma_auto void string_to_chars(char *s);
    requires [?f]string(s, ?cs);
    ensures [f]chars(s, length(cs) + 1, append(cs, cons('\0', nil))) &*& !mem('\0', cs);

lemma_auto void chars_separate_string(char *s);
    requires [?f]chars(s, ?n, ?cs) &*& mem('\0', cs) == true;
    ensures [f]string(s, take(index_of('\0', cs), cs)) &*& [f]chars(s + index_of('\0', cs) + 1, n - index_of('\0', cs) - 1, drop(index_of('\0', cs) + 1, cs));

lemma void chars_unseparate_string(char *s);
    requires [?f]string(s, ?cs) &*& [f]chars(s + length(cs) + 1, ?n, ?cs1);
    ensures [f]chars(s, length(cs) + 1 + n, append(cs, cons('\0', cs1)));

lemma void string_limits(char *s);
    requires [?f]string(s, ?cs);
    ensures [f]string(s, cs) &*& true == ((char *)0 < s) &*& s + length(cs) < (char *)UINTPTR_MAX;


inductive vararg = vararg_int(int) | vararg_uint(unsigned int) | vararg_pointer(void *);

@*/

/*@

predicate module(int moduleId, bool initialState);
predicate module_code(int moduleId;);

predicate argv(char **argv, int argc) =
    argc <= 0 ? true : pointer(argv, ?arg) &*& string(arg, _) &*& argv(argv + 1, argc - 1);

@*/

typedef int main(int argc, char **argv);
    //@ requires 0 <= argc &*& [_]argv(argv, argc);
    //@ ensures true;

typedef int main_full/*@(int mainModule)@*/(int argc, char **argv);
    //@ requires module(mainModule, true) &*& [_]argv(argv, argc);
    //@ ensures true;

#endif
