#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tgc.h"

static tgc_t gc;

typedef struct TodoItem {
  char* name;
  struct TodoItem* next;
} TodoItem;

void pretty_print(tgc_ptr_t* ptr) {
  if (ptr->flags & TGC_LEAF) {
    printf("string \"%s\"", ptr->ptr);
  } else {
    TodoItem* todo = (TodoItem*)ptr->ptr;
    printf("TodoItem { name = \"%s\", next = %p }", todo->name, todo->next);
  }
}

void todo_print(TodoItem* todo) {
  if (todo == NULL) {
    printf("No todos! :D\n");
  }

  int id = 1;
  while (todo) {
    printf("%d. %s\n", id, todo->name);
    todo = todo->next;
    id++;
  }
}

void str_dtor(void* _str) {
  char* str = (char*)_str;
  printf("Destructing string '%s'\n", str);
}

TodoItem* todo_add(TodoItem* todo, const char* name) {
  TodoItem* todo_item = (TodoItem*)tgc_alloc_opt(&gc, sizeof(TodoItem), 0, NULL);
  size_t len = strlen(name);

  todo_item->next = todo;
  todo_item->name = (char*)tgc_alloc_opt(&gc, len + 1, TGC_LEAF, str_dtor);
  strcpy(todo_item->name, name);

  return todo_item;
}

TodoItem* todo_remove(TodoItem* todo, int index) {
  if (index == 1 && todo) {
    return todo->next;
  }

  int id = 1;
  TodoItem* cur = todo;
  while (cur) {
    if (id == index - 1) {
      if (cur->next) {
        cur->next = cur->next->next;
      }
      break;
    }
    cur = cur->next;
    id++;
  }

  return todo;
}

TodoItem* todo_snip(TodoItem* todo, int index) {
  if (index < 2) {
    return NULL;
  }

  int id = 1;
  TodoItem* cur = todo;
  while (cur) {
    if (id == index - 1) {
      cur->next = NULL;
      break;
    }
    cur = cur->next;
    id++;
  }

  return todo;
}

int main(int argc, char **argv) {
  char input[1024];
  TodoItem* todo = NULL;
  bool quitting = false;

  tgc_start(&gc, &argc);

  while (!quitting) {
    todo_print(todo);

    printf("\ntodo> ");
    fflush(stdout);

    if (fgets(input, sizeof(input) - 1, stdin)) {
      if (input[0]) {
        // Trim trailing newline.
        input[strlen(input) - 1] = '\0';
      }

      switch (input[0]) {
        case '+':
          todo = todo_add(todo, &input[1]);
          break;
        case '-':
          todo = todo_remove(todo, atoi(&input[1]));
          break;
        case '/':
          todo = todo_snip(todo, atoi(&input[1]));
          break;
        case 'p':
          tgc_debug(&gc, pretty_print);
          break;
        case 'q':
          quitting = true;
          break;
        case '\0':
          break;
        default:
          printf("Invalid command.\nAvailable commands: + - / q\n");
          break;
      }
    }

    printf("\n");

    if (feof(stdin)) {
      quitting = true;
    }
  }

  tgc_stop(&gc);

  return 0;
}

