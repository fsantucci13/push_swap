#ifndef LIST_H
#define LIST_H

typedef struct s_list {
  struct s_list *next;
  int data;
  // pour sort_list sinon pour les autres void data;
} t_list;

#endif

// a rename list.h et pas sort_list.h